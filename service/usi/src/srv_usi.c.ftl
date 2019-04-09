/*******************************************************************************
  USI Service Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    srv_usi.c

  Summary:
    Source code for the USI Service implementation.

  Description:
    This file contains the source code for the implementation of the
    USI Service.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "driver/driver_common.h"
#include "service/usi/srv_usi.h"
#include "srv_usi_local.h"
<#if __PROCESSOR?matches("PIC32M.*") == false>
<#if core.DATA_CACHE_ENABLE?? && core.DATA_CACHE_ENABLE == true >
#include "system/cache/sys_cache.h"
</#if>
</#if>
<#if SRV_USI_USART_API == true>
#include "srv_usi_usart.h"
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
//__attribute__ ((aligned (32))) static char usart0_StartMessage[192] = 
//"*** USART Driver Multi-Instance Echo Demo Application ***\r\n"
//"*** Type 10 characters and observe it echo back using DMA ***\r\n"
//"*** LED toggles each time the data is echoed ***\r\n";

/* This is the service instance object array. */
static SRV_USI_OBJ gSrvUSIOBJ[SRV_USI_INSTANCES_NUMBER] = {NULL};

/* This is the USI callback object for each USI instance. */
static SRV_USI_CALLBACK gSrvUSICallbackOBJ[SRV_USI_INSTANCES_NUMBER][10] = {NULL};

typedef struct
{
    uint8_t *pData;
    uint16_t length;
}usi_msg_t;

static usi_msg_t usi_msg[8] = {0};
static uint8_t msg_idx = 0;

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
 <#if SRV_USI_USART_API == true>  
static void _SRV_USART_Callback_Handle ( uint8_t *data, uint16_t length ) 
{
    if (length) 
    {
        /* new received message */
        usi_msg[msg_idx].pData = data;
        usi_msg[msg_idx].length = length;
        msg_idx++;
        msg_idx &= 0x07;
        
    }
}
</#if>

static SRV_USI_HANDLE _SRV_USI_HandleValidate(SRV_USI_HANDLE handle)
{
    /* This function returns the same handle if the handle is valid. Returns 
       SRV_USI_HANDLE_INVALID otherwise. */

    uint8_t srvIndex;

    if((handle != SRV_USI_HANDLE_INVALID) && (handle != 0))
    {
        /* Look for the handle */
        for (srvIndex = 0; srvIndex < SRV_USI_INSTANCES_NUMBER; srvIndex++)
        {
            if (handle == (SRV_USI_HANDLE)&gSrvUSIOBJ[srvIndex]) {
                return handle;
            }
        }
    }

    return(SRV_USI_HANDLE_INVALID);
}

static SRV_USI_CALLBACK_INDEX _SRV_USI_GetCallbackIndexFromProtocol(SRV_USI_PROTOCOL_ID protocol)
{
    SRV_USI_CALLBACK_INDEX callbackIndex;

    switch(protocol)
    {
        case SRV_USI_PROT_ID_MNGP_PRIME_GETQRY:
        case SRV_USI_PROT_ID_MNGP_PRIME_GETRSP:
        case SRV_USI_PROT_ID_MNGP_PRIME_SET:
        case SRV_USI_PROT_ID_MNGP_PRIME_RESET:
        case SRV_USI_PROT_ID_MNGP_PRIME_REBOOT:
        case SRV_USI_PROT_ID_MNGP_PRIME_FU:
        case SRV_USI_PROT_ID_MNGP_PRIME_GETQRY_EN:
        case SRV_USI_PROT_ID_MNGP_PRIME_GETRSP_EN:
            callbackIndex = 0;
            break;

        case SRV_USI_PROT_ID_SNIF_PRIME:
            callbackIndex = 1;
            break;
            
        case SRV_USI_PROT_ID_PHY_SERIAL_PRIME:
            callbackIndex = 2;
            break;
            
        case SRV_USI_PROT_ID_PHY:
            callbackIndex = 3;
            break;
            
        case SRV_USI_PROT_ID_SNIFF_G3:
            callbackIndex = 4;
            break;
            
        case SRV_USI_PROT_ID_MAC_G3:
            callbackIndex = 5;
            break;
            
        case SRV_USI_PROT_ID_ADP_G3:
            callbackIndex = 6;
            break;
            
        case SRV_USI_PROT_ID_COORD_G3:
            callbackIndex = 7;
            break;
            
        case SRV_USI_PROT_ID_PHY_MICROPLC:
            callbackIndex = 8;
            break;
            
        case SRV_USI_PROT_ID_PRIME_API:
            callbackIndex = 9;
            break;

        default:
            callbackIndex = SRV_USI_CALLBACK_INDEX_INVALID;
    }

    return callbackIndex;    
}

static uint8_t* _SRV_USI_EscapeData( uint8_t *pDstData, uint8_t *pSrcData,
                                      uint16_t length, uint8_t *pEndData )
{
    while (length)
    {
        if (*pSrcData == USI_ESC_KEY_7D)
        {
            *pDstData++ = USI_ESC_KEY_7D;
            *pDstData++ = USI_ESC_KEY_5D;
        } 
        else if (*pSrcData == USI_ESC_KEY_7E)
        {
            *pDstData++ = USI_ESC_KEY_7D;
            *pDstData++ = USI_ESC_KEY_5E;
        } 
        else
        {
            *pDstData++ = *pSrcData;
        }
                  
        if (pDstData == pEndData)
        {
            /* Escaped Message can't fit in Write buffer */
            return 0;
        }
        
        pSrcData++;
        length--;
    }
    
    return pDstData;
}

static size_t _SRV_USI_BuildMessage( SRV_USI_OBJ* dObj, 
                                     SRV_USI_PROTOCOL_ID protocol, 
                                     uint8_t *pData, uint16_t length )
{
    uint8_t* pNewData;
    uint8_t* pEndData;
    uint8_t valueTmp[4];
    
    /* Build new message */
    pNewData = (uint8_t*)dObj->writeBuffer;
    pEndData = pNewData + (dObj->writeSizeMax - 3);
    
    /* Build header message */
    *pNewData++ = USI_ESC_KEY_7E;
    valueTmp[0] = USI_LEN_HI_PROTOCOL(length);
    valueTmp[1] = USI_LEN_LO_PROTOCOL(length) + USI_TYPE_PROTOCOL(protocol);
    pNewData = _SRV_USI_EscapeData(pNewData, valueTmp, 2, pEndData);
    if (pNewData == 0)
    {
        /* Error in Escape Data: can't fit in Destiny buffer */
        return 0;
    }
    pNewData = _SRV_USI_EscapeData(pNewData, pData, length, pEndData);
    if (pNewData == 0)
    {
        /* Error in Escape Data: can't fit in Destiny buffer */
        return 0;
    }
    /* get CRC */
    valueTmp[0] = 0x11;
    valueTmp[1] = 0x22;
    valueTmp[2] = 0x33;
    valueTmp[3] = 0x44;
    pNewData = _SRV_USI_EscapeData(pNewData, valueTmp, 4, pEndData);
    if (pNewData == 0)
    {
        /* Error in Escape Data: can't fit in Destiny buffer */
        return 0;
    }
    
    *pNewData++ = USI_ESC_KEY_7E;
    
    return (pNewData - (uint8_t*)dObj->writeBuffer);
}

// *****************************************************************************
// *****************************************************************************
// Section: USI SeRvice Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ SRV_USI_Initialize(
    const SYS_MODULE_INDEX index, 
    const SYS_MODULE_INIT * const init 
)
{
    SRV_USI_OBJ* dObj = NULL;
    SRV_USI_INIT* usiInit = (SRV_USI_INIT *)init ;

    /* Validate the request */
    if(index >= SRV_USI_INSTANCES_NUMBER)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Is the service instance already initialized? */
    if(gSrvUSIOBJ[index].inUse == true)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Allocate the service object */
    dObj = &gSrvUSIOBJ[index];

    dObj->inUse                 = true;
    dObj->usiInterfaceApi       = usiInit->usiInterfaceApi;
    dObj->usiApi                = usiInit->usiApi;
    dObj->readBuffer            = usiInit->readBuffer;
    dObj->readSizeMax           = usiInit->readSizeMax;
    dObj->writeBuffer           = usiInit->writeBuffer;
    dObj->writeSizeMax          = usiInit->writeSizeMax;

    dObj->apiDriver             = DRV_HANDLE_INVALID;
    dObj->callback              = &gSrvUSICallbackOBJ[index][0];

    /* Update the status */
    dObj->status = SYS_STATUS_READY;

    /* Return the object structure */
    return ( (SYS_MODULE_OBJ)index );
}

SRV_USI_HANDLE SRV_USI_Open(
    const SYS_MODULE_INDEX index
)
{
    SRV_USI_OBJ* dObj = NULL;

    /* Validate the request */
    if (index >= SRV_USI_INSTANCES_NUMBER)
    {
        return SRV_USI_HANDLE_INVALID;
    }

    dObj = &gSrvUSIOBJ[index];

    if((dObj->status != SYS_STATUS_READY) || (dObj->inUse == false))
    {
        return SRV_USI_HANDLE_INVALID;
    }
    
<#if SRV_USI_USART_API == true>
    if (dObj->usiInterfaceApi == SRV_USI_USART_API)
    {
        USI_USART_INIT init;
        
        init.plib = dObj->usiApi;
        init.pRdBuffer = dObj->readBuffer;
        init.rdBufferSize = dObj->readSizeMax;
        init.pWrBuffer = dObj->writeBuffer;
        init.wrBufferSize = dObj->writeSizeMax;
        
        /* Open Serial interface */
        dObj->apiDriver = USI_USART_Initialize(&init);
        if (dObj->apiDriver == DRV_HANDLE_INVALID)
        {
            return SRV_USI_HANDLE_INVALID;
        }
        
        /* Register callback and launch reception transfer */
        USI_USART_RegisterCallback(dObj->apiDriver, _SRV_USART_Callback_Handle);
    }
    
</#if>
<#if SRV_USI_CDC_API == true>
    if (dObj->usiInterfaceApi == SRV_USI_CDC_API)
    {
        /* Open USB CDC interface */
    }
    
</#if>
<#if SRV_USI_TCP_API == true>
if (dObj->usiInterfaceApi == SRV_USI_TCP_API)
    {
        /* Open TCP interface */
    }
    
</#if>

    /* Update USI object status */
    dObj->inUse = true;
    dObj->status = SYS_STATUS_READY;
    
    /* Initialize other elements in USI Object */
    dObj->usiIndex = index;

    return ((SRV_USI_HANDLE)dObj);
}

void SRV_USI_Close( SRV_USI_HANDLE handle )
{
    SRV_USI_OBJ* dObj = NULL;

    /* Validate the driver handle */
    if (_SRV_USI_HandleValidate(handle) == SRV_USI_HANDLE_INVALID)
    {
        return;
    }

    dObj = (SRV_USI_OBJ*)handle;

    if((dObj->status != SYS_STATUS_READY) || (dObj->inUse == false))
    {
        return;
    }
  
<#if SRV_USI_USART_API == true>
    if (dObj->usiInterfaceApi == SRV_USI_USART_API)
    {
        /* Close Serial interface */
        USI_USART_Close(dObj->apiDriver);
    }
    
</#if>
<#if SRV_USI_CDC_API == true>
    if (dObj->usiInterfaceApi == SRV_USI_CDC_API)
    {
        /* Close USB CDC interface */
    }
    
</#if>
<#if SRV_USI_TCP_API == true>
if (dObj->usiInterfaceApi == SRV_USI_TCP_API)
    {
        /* Close TCP interface */
    }
    
</#if>

    /* De-Allocate the service object */
    dObj->inUse = false;

}

SYS_STATUS SRV_USI_Status( SYS_MODULE_OBJ object)
{
    /* Validate the request */
    if( (object == SYS_MODULE_OBJ_INVALID) || (object >= SRV_USI_INSTANCES_NUMBER) )
    {
        return SYS_STATUS_UNINITIALIZED;
    }

    return (gSrvUSIOBJ[object].status);
}

void SRV_USI_CallbackRegister ( const SRV_USI_HANDLE handle, 
        SRV_USI_PROTOCOL_ID protocol, SRV_USI_CALLBACK callback )
{
    SRV_USI_CALLBACK_INDEX callbackIndex;
    SRV_USI_OBJ* dObj = NULL;
    SRV_USI_CALLBACK *cb;

    /* Validate the driver handle */
    if (_SRV_USI_HandleValidate(handle) == SRV_USI_HANDLE_INVALID)
    {
        return;
    }

    dObj = (SRV_USI_OBJ*)handle;

    /* Get callback index from USI protocol */
    callbackIndex = _SRV_USI_GetCallbackIndexFromProtocol(protocol);

    if (callbackIndex == SRV_USI_CALLBACK_INDEX_INVALID)
    {
        return;
    }

    /* Validate callback function */
    if (callback == NULL)
    {
        return;
    }
    
    /* Register callback to the USI protocol */ 
    cb = &(dObj->callback[callbackIndex]);
    *cb = callback;

}

void SRV_USI_Tasks( const SYS_MODULE_INDEX index )
{
    /* Validate the request */
    if(index >= SRV_USI_INSTANCES_NUMBER)
    {
        return;
    }

    /* Is the service instance already initialized? */
    if(gSrvUSIOBJ[index].inUse == false)
    {
        return;
    }
    
    /* Is the read buffer already registered? */
    if(gSrvUSIOBJ[index].readBuffer == NULL)
    {
        return;
    }
 
<#if SRV_USI_USART_API == true>   
    USI_USART_Tasks(gSrvUSIOBJ[index].apiDriver);
</#if>    
    
}

void SRV_USI_Send_Message( const SRV_USI_HANDLE handle, 
        SRV_USI_PROTOCOL_ID protocol, uint8_t *data, size_t length )
{
    SRV_USI_OBJ* dObj = NULL;
    size_t writeLength;

    /* Validate the driver handle */
    if (_SRV_USI_HandleValidate(handle) == SRV_USI_HANDLE_INVALID)
    {
        return;
    }

    dObj = (SRV_USI_OBJ*)handle;
    
    /* Check length */
    if ((length == 0) || (length > dObj->writeSizeMax))
    {
        return;
    }
    
    /* Build USI message */
    writeLength = _SRV_USI_BuildMessage(dObj, protocol, data, length);
    
<#if SRV_USI_USART_API == true>      
    /* Send message */
    USI_USART_Write(dObj->apiDriver, writeLength);
</#if>  
}