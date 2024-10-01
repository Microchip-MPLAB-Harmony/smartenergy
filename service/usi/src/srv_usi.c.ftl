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
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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

#include <string.h>
#include <stddef.h>
#include "configuration.h"
#include "driver/driver_common.h"
#include "service/usi/srv_usi.h"
#include "service/pcrc/srv_pcrc.h"
<#if SRV_USI_ERROR_LOG_REPORT == true>
#include "service/log_report/srv_log_report.h"
</#if>
#include "srv_usi_local.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
/* This is the service instance object array. */
static SRV_USI_OBJ gSrvUSIOBJ[SRV_USI_INSTANCES_NUMBER] = {
<#list 1..__INSTANCE_COUNT as instance>
    {.status = SRV_USI_STATUS_UNINITIALIZED},
</#list>
};

/* This is the USI callback object for each USI instance. */
static SRV_USI_CALLBACK gSrvUSICallbackOBJ[SRV_USI_INSTANCES_NUMBER][11];

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

static SRV_USI_CALLBACK_INDEX lSRV_USI_GetCallbackIndexFromProtocol(SRV_USI_PROTOCOL_ID protocol)
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

        case SRV_USI_PROT_ID_PHY_RF215:
            callbackIndex = 10;
            break;

        case SRV_USI_PROT_ID_INVALID:
        default:
<#if SRV_USI_ERROR_LOG_REPORT == true>
            SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_ERROR, USI_BAD_PROTOCOL,
                                             "Bad protocol to get callback\r\n");
</#if>
            callbackIndex = SRV_USI_CALLBACK_INDEX_INVALID;
            break;
    }

    return callbackIndex;
}

static PCRC_CRC_TYPE lSRV_USI_GetCRCTypeFromProtocol(SRV_USI_PROTOCOL_ID protocol)
{
    PCRC_CRC_TYPE crcType;

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
            crcType = PCRC_CRC32;
            break;

        case SRV_USI_PROT_ID_SNIF_PRIME:
        case SRV_USI_PROT_ID_PHY_SERIAL_PRIME:
        case SRV_USI_PROT_ID_PHY:
        case SRV_USI_PROT_ID_PHY_RF215:
        case SRV_USI_PROT_ID_SNIFF_G3:
        case SRV_USI_PROT_ID_MAC_G3:
        case SRV_USI_PROT_ID_ADP_G3:
        case SRV_USI_PROT_ID_COORD_G3:
            crcType = PCRC_CRC16;
            break;
            
        case SRV_USI_PROT_ID_PRIME_API:
        case SRV_USI_PROT_ID_PHY_MICROPLC:
        case SRV_USI_PROT_ID_INVALID:
        default:
            crcType = PCRC_CRC8;
            break;
    }

    return crcType;    
}

static SRV_USI_HANDLE lSRV_USI_HandleValidate(SRV_USI_HANDLE handle)
{
    /* This function returns the same handle if the handle is valid. Returns 
       SRV_USI_HANDLE_INVALID otherwise. */

    uint8_t srvIndex;

    if((handle != SRV_USI_HANDLE_INVALID) && (handle != 0U))
    {
        /* Look for the handle */
        for (srvIndex = 0; srvIndex < SRV_USI_INSTANCES_NUMBER; srvIndex++)
        {
            if (handle == (SRV_USI_HANDLE)(&gSrvUSIOBJ[srvIndex])) {
                return handle;
            }
        }
    }

    return(SRV_USI_HANDLE_INVALID);
}

static void lSRV_USI_Callback_Handle ( uint8_t *pData, uint16_t length, uintptr_t context )
{    
    SRV_USI_OBJ* dObj;
    uint32_t crcGetValue;
    uint32_t crcRcvValue;
    uint16_t lengthWithoutCrc;
    SRV_USI_PROTOCOL_ID protocol;
    uint8_t protocolValue;
    PCRC_CRC_TYPE crcType;
    uint16_t dataLength;
    SRV_USI_CALLBACK_INDEX cbIndex;
    
    /* Check valid context : the driver handle */
    if (lSRV_USI_HandleValidate((SRV_USI_HANDLE)context) == SRV_USI_HANDLE_INVALID)
    {
        return;
    }

    dObj = (SRV_USI_OBJ*)context;
    
    if (length > 0U) 
    {      
        /* New received message */
        /* Extract Protocol */
        protocolValue = USI_TYPE_PROTOCOL(pData[1]);
        protocol = (SRV_USI_PROTOCOL_ID)protocolValue;
        
        /* Get CRC type from Protocol */
        crcType = lSRV_USI_GetCRCTypeFromProtocol(protocol);
        
        /* Extract data length */
        dataLength = USI_LEN_PROTOCOL(pData[USI_LEN_HI_OFFSET], pData[USI_LEN_LO_OFFSET]);

        /* Add extended length */
        if ((protocol == SRV_USI_PROT_ID_ADP_G3) ||
            (protocol == SRV_USI_PROT_ID_COORD_G3) ||
            (protocol == SRV_USI_PROT_ID_PRIME_API))
        {
            dataLength += ((uint16_t) pData[USI_XLEN_OFFSET] & USI_XLEN_MSK) << USI_XLEN_SHIFT_L;
        }

        /* Check invalid length : remove Header and CRC bytes */
        lengthWithoutCrc = length - ((uint16_t)1U << (uint8_t)crcType);
        if (dataLength != (lengthWithoutCrc - 2U))
        {
            /* Discard message */
<#if SRV_USI_ERROR_LOG_REPORT == true>
            SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_ERROR, USI_BAD_LENGTH, 
                                             "Received bad length\r\n");
</#if>
            return;
        }
        
        /* Check CRC */
        crcGetValue = SRV_PCRC_GetValue(pData, lengthWithoutCrc,
                PCRC_HT_USI, crcType, 0, false);
 
        if (crcType == PCRC_CRC8)
        {
            crcRcvValue = (uint32_t)(pData[length - 1U]);
        } 
        else if (crcType == PCRC_CRC16)
        {
            crcRcvValue = (((uint32_t)(pData[length - 2U])) << 8) +
                          (uint32_t)(pData[length - 1U]);
        }
        else
        {            
            crcRcvValue = (((uint32_t)(pData[length - 4U])) << 24) +
                          (((uint32_t)(pData[length - 3U])) << 16) +
                          (((uint32_t)(pData[length - 2U])) << 8) +
                          (uint32_t)(pData[length - 1U]);
        }
        
        if (crcGetValue != crcRcvValue) 
        {
            /* Discard message */
<#if SRV_USI_ERROR_LOG_REPORT == true>
            SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_ERROR, USI_BAD_CRC,
                                             "Received wrong CRC\r\n");
</#if>
            return;
        }
    
        /* Launch USI callback */
        cbIndex = lSRV_USI_GetCallbackIndexFromProtocol(protocol);
        if (dObj->callback[cbIndex] != NULL)
        {
            dObj->callback[cbIndex](pData + 2, dataLength);
        }
    }
}

static uint8_t* lSRV_USI_EscapeData( uint8_t *pDstData, uint8_t *pSrcData,
                                      uint16_t length, uint8_t *pEndData )
{
    while (length > 0U)
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
            return NULL;
        }
        
        pSrcData++;
        length--;
    }
    
    return pDstData;
}

static size_t lSRV_USI_BuildMessage( uint8_t *pDstData, size_t maxDstLength, 
                                     SRV_USI_PROTOCOL_ID protocol, 
                                     uint8_t *pData, uint16_t length )
{
    ptrdiff_t size;
    uint8_t* pNewData;
    uint8_t* pEndData;
    uint8_t valueTmp[4];
    uint8_t command;
    uint32_t valueTmp32;
    PCRC_CRC_TYPE crcType;
    
    /* Get CRC type from Protocol */
    crcType = lSRV_USI_GetCRCTypeFromProtocol(protocol);
    
    /* Build new message */
    pNewData = pDstData;
    pEndData = pNewData + (maxDstLength - 3U);
    
    /* Build header message */
    *pNewData++ = USI_ESC_KEY_7E;
    valueTmp[0] = USI_LEN_HI_PROTOCOL(length);
    valueTmp[1] = USI_LEN_LO_PROTOCOL(length) + USI_TYPE_PROTOCOL((uint8_t)protocol);
    
    /* Get CRC from USI header: 2 bytes */
    valueTmp32 = SRV_PCRC_GetValue(&valueTmp[0], 2, PCRC_HT_USI, crcType, 0, false);
    /* Escape USI header */
    pNewData = lSRV_USI_EscapeData(pNewData, valueTmp, 2, pEndData);
    if (pNewData == NULL)
    {
        /* Error in Escape Data: can't fit in destination buffer */
<#if SRV_USI_ERROR_LOG_REPORT == true>
        SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_ERROR, USI_ERROR_ESCAPE, 
            "Error in Escape Data: can't fit in destination buffer\r\n");
</#if>
        return 0;
    }

    if ((protocol == SRV_USI_PROT_ID_ADP_G3) ||
        (protocol == SRV_USI_PROT_ID_COORD_G3) ||
        (protocol == SRV_USI_PROT_ID_PRIME_API))
    {
        /* Adjust extended length */
        command = pData[0];
        pData[0] = USI_LEN_EX_PROTOCOL(length) + USI_CMD_PROTOCOL(command);
    }

    /* Get CRC from USI data. */
    valueTmp32 = SRV_PCRC_GetValue(pData, length, PCRC_HT_USI, crcType, valueTmp32, false);
    /* Escape USI data */
    pNewData = lSRV_USI_EscapeData(pNewData, pData, length, pEndData);
    if (pNewData == NULL)
    {
        /* Error in Escape Data: can't fit in destination buffer */
<#if SRV_USI_ERROR_LOG_REPORT == true>
        SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_ERROR, USI_ERROR_ESCAPE,
            "Error in Escape Data: can't fit in destination buffer\r\n");
</#if>
        return 0;
    }
    
    /* Escape CRC value */
    valueTmp[0] = (uint8_t)(valueTmp32 >> 24);
    valueTmp[1] = (uint8_t)(valueTmp32 >> 16);
    valueTmp[2] = (uint8_t)(valueTmp32 >> 8);
    valueTmp[3] = (uint8_t)valueTmp32;
    if (crcType == PCRC_CRC8)
    {
        pNewData = lSRV_USI_EscapeData(pNewData, &valueTmp[3], 1, pEndData);
    } 
    else if (crcType == PCRC_CRC16)
    {
        pNewData = lSRV_USI_EscapeData(pNewData, &valueTmp[2], 2, pEndData);
    }
    else
    {
        pNewData = lSRV_USI_EscapeData(pNewData, valueTmp, 4, pEndData);
    }
    
    if (pNewData == NULL)
    {
        /* Error in Escape Data: can't fit in destination buffer */
<#if SRV_USI_ERROR_LOG_REPORT == true>
        SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_ERROR, USI_ERROR_ESCAPE,
            "Error in Escape Data: can't fit in destination buffer\r\n");
</#if>
        return 0;
    }
    
    *pNewData++ = USI_ESC_KEY_7E;

    size = pNewData - pDstData;
    return (size_t)size;
}

// *****************************************************************************
// *****************************************************************************
// Section: USI Service Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ SRV_USI_Initialize(
    const SYS_MODULE_INDEX index, 
    const SYS_MODULE_INIT * const init 
)
{
    /* MISRA C-2012 deviation block start */
    /* MISRA C-2012 Rule 11.3 deviated once. Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    </#if>
    #pragma coverity compliance block deviate "MISRA C-2012 Rule 11.3" "H3_MISRAC_2012_R_11_3_DR_1"
</#if>
    const SRV_USI_INIT * const usiInit = (const SRV_USI_INIT * const)init;
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    #pragma coverity compliance end_block "MISRA C-2012 Rule 11.3"
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic pop
    </#if>
</#if>
    /* MISRA C-2012 deviation block end */
    SRV_USI_OBJ* dObj;

    /* Confirm valid arguments */
    if(index >= SRV_USI_INSTANCES_NUMBER)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Allocate the service object */
    dObj = &gSrvUSIOBJ[index];
    
   if ((dObj->status == SRV_USI_STATUS_UNINITIALIZED) && (usiInit != NULL))
    {
        dObj->status                = SRV_USI_STATUS_NOT_CONFIGURED;
        dObj->devIndex              = usiInit->deviceIndex;
        dObj->devDesc               = usiInit->consDevDesc;
        dObj->pWrBuffer             = usiInit->pWrBuffer;
        dObj->wrBufferSize          = usiInit->wrBufferSize;
        dObj->callback              = gSrvUSICallbackOBJ[index];
        (void) memset(gSrvUSICallbackOBJ[index], 0, sizeof(gSrvUSICallbackOBJ[index]));

        dObj->devDesc->init(dObj->devIndex, usiInit->deviceInitData);
        
        /* Return the object structure */
        return ( (SYS_MODULE_OBJ)index );
    }
    
    /* Return the invalid object */
    return SYS_MODULE_OBJ_INVALID;
}

SRV_USI_HANDLE SRV_USI_Open(
    const SYS_MODULE_INDEX index
)
{
    SRV_USI_OBJ* dObj = &gSrvUSIOBJ[index];

    /* Validate the request */
    if (index >= SRV_USI_INSTANCES_NUMBER)
    {
        return SRV_USI_HANDLE_INVALID;
    }

    if(dObj->status == SRV_USI_STATUS_UNINITIALIZED)
    {
        return SRV_USI_HANDLE_INVALID;
    }

    if(dObj->status == SRV_USI_STATUS_NOT_CONFIGURED)
    {
        /* Open USI device */
        if (dObj->devDesc->open(dObj->devIndex) == DRV_HANDLE_INVALID)
        {
            return SRV_USI_HANDLE_INVALID;
        }
    }

    /* Update USI status */
    dObj->status = SRV_USI_STATUS_CONFIGURED;
    
    return ((SRV_USI_HANDLE)dObj);
}

void SRV_USI_Close( SRV_USI_HANDLE handle )
{
    SRV_USI_OBJ* dObj;

    /* Validate the driver handle */
    if (lSRV_USI_HandleValidate(handle) == SRV_USI_HANDLE_INVALID)
    {
        return;
    }

    dObj = (SRV_USI_OBJ*)handle;

    if(dObj->status != SRV_USI_STATUS_CONFIGURED)
    {
        return;
    }

    /* Close USI device */
    dObj->devDesc->close(dObj->devIndex);
    dObj->status = SRV_USI_STATUS_NOT_CONFIGURED;
}

SRV_USI_STATUS SRV_USI_Status( SRV_USI_HANDLE handle )
{
    SRV_USI_OBJ* dObj;

    /* Validate the driver handle */
    if (lSRV_USI_HandleValidate(handle) == SRV_USI_HANDLE_INVALID)
    {
        return SRV_USI_STATUS_ERROR;
    }

    dObj = (SRV_USI_OBJ*)handle;
    
    /* Check USI device status */
    return dObj->devDesc->status(dObj->devIndex);
}

void SRV_USI_CallbackRegister ( SRV_USI_HANDLE handle,
        SRV_USI_PROTOCOL_ID protocol, SRV_USI_CALLBACK callback )
{
    SRV_USI_CALLBACK_INDEX callbackIndex;
    SRV_USI_OBJ* dObj = (SRV_USI_OBJ*)handle;
    SRV_USI_CALLBACK *cb;

    /* Validate the driver handle */
    if (lSRV_USI_HandleValidate(handle) == SRV_USI_HANDLE_INVALID)
    {
        return;
    }

    /* Get callback index from USI protocol */
    callbackIndex = lSRV_USI_GetCallbackIndexFromProtocol(protocol);

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
    
    /* Register reception callback */
    dObj->devDesc->setReadCallback(dObj->devIndex, lSRV_USI_Callback_Handle, (uintptr_t)dObj);

}

void SRV_USI_Tasks( SYS_MODULE_OBJ object )
{
    SRV_USI_OBJ* dObj = &gSrvUSIOBJ[object];
    
    /* Validate the request */
    if(object >= SRV_USI_INSTANCES_NUMBER)
    {
        return;
    }

    /* Is the service instance already opened? */
    if(dObj->status != SRV_USI_STATUS_CONFIGURED)
    {
        return;
    }
    
    dObj->devDesc->task(dObj->devIndex);
    
}

void SRV_USI_Send_Message( SRV_USI_HANDLE handle,
        SRV_USI_PROTOCOL_ID protocol, uint8_t *data, size_t length )
{
    SRV_USI_OBJ* dObj = (SRV_USI_OBJ*)handle;
    size_t writeLength;

    /* Validate the driver handle */
    if (lSRV_USI_HandleValidate(handle) == SRV_USI_HANDLE_INVALID)
    {
        return;
    }
    
    /* Check length */
    if ((length == 0U) || (length > dObj->wrBufferSize))
    {
<#if SRV_USI_ERROR_LOG_REPORT == true>
        SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_ERROR, USI_INVALID_LENGTH, 
                                         "Invalid length\r\n");
</#if>
        return;
    }

    /* Waiting for USART/CDC is free */
    while (dObj->devDesc->writeIsBusy(dObj->devIndex) == true){}

    /* Build USI message */
    writeLength = lSRV_USI_BuildMessage(dObj->pWrBuffer, dObj->wrBufferSize, protocol, data, (uint16_t)length);
    
    /* Send message */
    dObj->devDesc->writeData(dObj->devIndex, dObj->pWrBuffer, writeLength);
}
