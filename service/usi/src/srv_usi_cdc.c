/*******************************************************************************
  USB CDC wrapper used from USI service Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    srv_usi_cdc.c

  Summary:
    USB CDC wrapper used from USI service implementation.

  Description:
    The USB CDC wrapper provides a simple interface to manage the USB
    module on Microchip microcontrollers. This file implements the core
    interface routines for the USI PLC service. 
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "stddef.h"
#include "configuration.h"
#include "driver/driver_common.h"
#include "system/dma/sys_dma.h"
#include "srv_usi_local.h"
#include "srv_usi_cdc.h"
#include "srv_usi_definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
/* This is the service instance object array. */
const SRV_USI_DEV_DESC srvUSIUSARTDevDesc =
{
    .usiDevice                  = SRV_USI_DEV_USART,
    .intent                     = DRV_IO_INTENT_READWRITE,
    .init                       = USI_CDC_Initialize,
    .setReadCallback            = USI_CDC_RegisterCallback,
    .write                      = USI_CDC_Write,
    .task                       = USI_CDC_Tasks,
    .flush                      = USI_CDC_Flush,
    .close                      = USI_CDC_Close,
};

static USI_CDC_OBJ gUsiCdcOBJ[SRV_USI_CDC_CONNECTIONS] = {NULL};
static USI_CDC_MSG gUsiCdcMsgPool[SRV_USI_MSG_POOL_SIZE] = {0};
static USI_CDC_MSG_QUEUE gUsiCdcMsgQueue[SRV_USI_CDC_CONNECTIONS] = {NULL};

#define USI_CDC_GET_INSTANCE(index)    (index >= SRV_USI_CDC_CONNECTIONS)? NULL : &gUsiCdcOBJ[index]

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
static USI_CDC_MSG* _USI_CDC_PUT_MSG_TO_QUEUE( USI_CDC_OBJ* dObj )
{
    USI_CDC_MSG* pMsg;
    uint8_t index;
    
    /* Get free buffer from POOL */
    for (index = 0; index < SRV_USI_MSG_POOL_SIZE; index++)
    {
        pMsg = &gUsiCdcMsgPool[index];
        if (pMsg->inUse == 0)
        {
            USI_CDC_MSG_QUEUE* dObjQueue;
            
            /* Initialize Message data */
            pMsg->inUse = 1;
            pMsg->next = NULL;
            
            /* Update queue */
            dObjQueue = dObj->pMsgQueue;
            if (dObjQueue->rear == NULL)
            {   /* Queue is empty */
                dObjQueue->front = dObjQueue->rear = pMsg;
            }
            else
            {   /* Queue is not empty. Add new msg */
                dObjQueue->rear->next = (struct USI_CDC_MSG*)pMsg;
                dObjQueue->rear = pMsg;
            }           
            
            return pMsg;
        }
    }
    
    return NULL;
}

static void _USI_CDC_GET_MSG_FROM_QUEUE( USI_CDC_OBJ* dObj )
{
    USI_CDC_MSG_QUEUE* dObjQueue;
    
    /* Get Queue */
    dObjQueue = dObj->pMsgQueue;
    
    if (dObjQueue->front == NULL)
    {   /* Queue is empty */
        return;
    }
    
    /* Update buffer content */
    dObjQueue->front->inUse = 0;
    
    /* Update queue front */
    dObjQueue->front = (USI_CDC_MSG*)dObjQueue->front->next;
    
    if (dObjQueue->front == NULL)
    {   /* There isn't anymore elements */
        dObjQueue->rear = NULL;
    }
}

static void _USI_CDC_ABORT_MSG_IN_QUEUE( USI_CDC_OBJ* dObj )
{
    USI_CDC_MSG_QUEUE* dObjQueue;
    USI_CDC_MSG* pMsgTmp;
    
    /* Get Queue */
    dObjQueue = dObj->pMsgQueue;
    
    if (dObjQueue->front == NULL)
    {   /* Queue is empty */
        return;
    }
    
    /* Find previous element in queue */
    pMsgTmp = dObjQueue->front;
    while (pMsgTmp != NULL)
    {
        if (pMsgTmp->next == (struct USI_CDC_MSG*)dObjQueue->rear)
        {
            /* Update buffer in use. Return it to pool */
            dObjQueue->rear->inUse = false;
            /* Update last link */
            pMsgTmp->next = NULL;
            /* Update queue to the previous element */
            dObjQueue->rear = pMsgTmp;            
            return;
        }
        /* Check next element */
        pMsgTmp = (USI_CDC_MSG*)pMsgTmp->next;        
    }
}

static void _USI_CDC_PLIB_CALLBACK( uintptr_t context)
{
    USI_CDC_OBJ* dObj;
    USI_CDC_MSG* pMsg;
    bool next;
    
    dObj = (USI_CDC_OBJ*)context;
    pMsg = dObj->pRcvMsg;
    next = false;
    
    switch(dObj->status)
    {
        case USI_CDC_IDLE:
            /* Waiting to MSG KEY */
            if (dObj->rcvChar == USI_ESC_KEY_7E)
            {
                uint8_t *pData; 
                
                /* Restart Byte Counter */
                if (dObj->pMsgQueue->front == NULL)
                {
                    /* Not received anymore characters: Restart reception buffer */
                    dObj->byteCount = 0;
                }
                
                /* Create new message */
                pMsg = _USI_CDC_PUT_MSG_TO_QUEUE(dObj);
                
                /* Fill in the message */  
                pData = (uint8_t *)(dObj->pRdBuffer);
                pMsg->pMessage = pData + dObj->byteCount;
                pMsg->pDataRd = pMsg->pMessage;
                pMsg->length = 0;    
                
                /* Update Msg in reception data */
                dObj->pRcvMsg = pMsg;
                        
                /* New Message, start reception */
                dObj->status = USI_CDC_RCV;
            }            
            break;
            
        case USI_CDC_RCV:
            if (dObj->rcvChar == USI_ESC_KEY_7E)
            {
                /* End of Message */
                pMsg->length = pMsg->pDataRd - pMsg->pMessage;
                dObj->pRcvMsg = NULL;
                dObj->status = USI_CDC_IDLE;
            }              
            else if (dObj->rcvChar == USI_ESC_KEY_7D)
            {
                /* Escape character */
                dObj->status = USI_CDC_ESC;
            } 
            else
            {
                /* Store character */
                next = true;
            }
      
            break;
            
        case USI_CDC_ESC:
            if (dObj->rcvChar == USI_ESC_KEY_5E)
            {
                /* Store character after escape it */
                *pMsg->pDataRd++ = USI_ESC_KEY_7E;
                dObj->status = USI_CDC_RCV;
            }  
            else if (dObj->rcvChar == USI_ESC_KEY_5D)
            {
                /* Store character after escape it */
                *pMsg->pDataRd++ = USI_ESC_KEY_7D;
                dObj->status = USI_CDC_RCV;
            }
            else
            {
                /* ERROR: Escape format */
                _USI_CDC_ABORT_MSG_IN_QUEUE(dObj);
                dObj->pRcvMsg = NULL;
                dObj->status = USI_CDC_IDLE;
            }
      
            break;
    }    
    
    /* Update pointers */
    if (next)
    {
        dObj->byteCount++;
        if (dObj->byteCount == dObj->rdBufferSize)
        {
            /* ERROR: Overflow */
            _USI_CDC_ABORT_MSG_IN_QUEUE(dObj);
            dObj->pRcvMsg = NULL;
            dObj->byteCount = 0;
        }
        else
        {
            *pMsg->pDataRd++ = dObj->rcvChar;
        }        
    }
    
    /* Read next char */
    dObj->plib->read(&dObj->rcvChar, 1);
}

// *****************************************************************************
// *****************************************************************************
// Section: USI USART Service Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

DRV_HANDLE USI_CDC_Initialize(uint32_t index, const void* initData)
{
    USI_CDC_OBJ* dObj = USI_CDC_GET_INSTANCE(index);
    USI_CDC_INIT_DATA* dObjInit = (USI_CDC_INIT_DATA*)initData;
    
    if (dObj == NULL)
    {
        return DRV_HANDLE_INVALID;
    }
    
    if (index >= SRV_USI_CDC_CONNECTIONS)
    {
        return DRV_HANDLE_INVALID;
    }
    
    dObj->plib = (SRV_USI_CDC_INTERFACE*)dObjInit->plib;
    dObj->pRdBuffer = dObjInit->pRdBuffer;
    dObj->rdBufferSize = dObjInit->rdBufferSize;
    
    dObj->pRcvMsg = NULL;
    dObj->pMsgQueue = &gUsiCdcMsgQueue[index];
    dObj->byteCount = 0;
    dObj->cbFunc = NULL;
    dObj->status = USI_CDC_IDLE;

    return (DRV_HANDLE)index;
}

size_t USI_CDC_Write(DRV_HANDLE handle, void* pData, size_t length)
{
    USI_CDC_OBJ* dObj = USI_CDC_GET_INSTANCE(handle);
    
    /* Check handler */
    if (dObj == NULL)
    {
        return 0;
    }
    
    if (handle >= SRV_USI_CDC_CONNECTIONS)
    {
        return 0;
    }
    
    if (length == 0)
    {
        return 0;
    }

    /* Waiting for USART is free */
    while (dObj->plib->writeIsBusy());

    dObj->plib->write(pData, length);
    
    return length;
}

void USI_CDC_RegisterCallback(DRV_HANDLE handle, USI_CDC_CALLBACK cbFunc,
        uintptr_t context)
{
    USI_CDC_OBJ* dObj = USI_CDC_GET_INSTANCE(handle);
    
    /* Check handler */
    if (dObj == NULL)
    {
        return;
    }
    
    if (handle >= SRV_USI_CDC_CONNECTIONS)
    {
        return;
    }
    
    /* Set USART PLIB handler */
    dObj->plib->readCallbackRegister(_USI_CDC_PLIB_CALLBACK, (uintptr_t)dObj);
    
    /* Set callback function */
    dObj->cbFunc = cbFunc;
    
    /* Set context related to cbFunc */
    dObj->context = context;
    
    /* Launch reception */
    dObj->plib->read(&dObj->rcvChar, 1);
}

void USI_CDC_Flush(DRV_HANDLE handle)
{
    (void)handle;
}

void USI_CDC_Close(DRV_HANDLE handle)
{
    (void)handle;
}

void USI_CDC_Tasks ( DRV_HANDLE handle )
{
    USI_CDC_OBJ* dObj = USI_CDC_GET_INSTANCE(handle);
    USI_CDC_MSG* pMsg;
    
    /* Check handler */    
    if (dObj == NULL)
    {
        return;
    }
    
    if (handle >= SRV_USI_CDC_CONNECTIONS)
    {
        return;
    }
    
    /* Handle callback functions */
    pMsg = dObj->pMsgQueue->front;
    if ((pMsg != NULL) && (pMsg->length > 0))
    {        
        if (dObj->cbFunc)
        {
            dObj->cbFunc(pMsg->pMessage, pMsg->length, dObj->context);
        }
        
        /* Remove Message from Queue */
        _USI_CDC_GET_MSG_FROM_QUEUE(dObj);
    }
}
