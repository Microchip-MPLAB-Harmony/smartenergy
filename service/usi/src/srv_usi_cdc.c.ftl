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
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stddef.h>
#include "configuration.h"
#include "driver/driver_common.h"
#include "usb/usb_device.h"
#include "usb/usb_device_cdc.h"
#include "srv_usi_local.h"
#include "srv_usi_definitions.h"
#include "srv_usi_cdc.h"
#include "definitions.h"                // SYS function prototypes

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
/* This is the service instance object array. */
const SRV_USI_DEV_DESC srvUSICDCDevDesc =
{
    .init                       = USI_CDC_Initialize,
    .open                       = USI_CDC_Open,
    .setReadCallback            = USI_CDC_RegisterCallback,
    .writeData                  = USI_CDC_Write,
    .writeIsBusy                = USI_CDC_WriteIsBusy,
    .task                       = USI_CDC_Tasks,
    .close                      = USI_CDC_Close,
    .status                     = USI_CDC_Status,
};

static USI_CDC_OBJ gUsiCdcOBJ[SRV_USI_CDC_CONNECTIONS] = {0};

#define USI_CDC_GET_INSTANCE(index)    (((index) >= SRV_USI_CDC_CONNECTIONS)? NULL : &gUsiCdcOBJ[index])

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

static void lUSI_CDC_TransferReceivedData(USI_CDC_OBJ* dObj)
{
    uint8_t *pData = dObj->cdcReadBuffer;

    while(dObj->cdcNumBytesRead > 0U)
    {
        bool store = false;
        uint8_t charStore = 0;

        switch(dObj->devStatus)
        {
            case USI_CDC_IDLE:
                /* Waiting to MSG KEY */
                if (*pData == USI_ESC_KEY_7E)
                {
                    /* Start of USI Message */
                    dObj->usiNumBytesRead = 0;
                    /* New Message, start reception */
                    dObj->devStatus = USI_CDC_RCV;
                }
                break;

            case USI_CDC_RCV:
                if (*pData == USI_ESC_KEY_7E)
                {
                    if ((dObj->usiNumBytesRead > 0U) && (dObj->cbFunc != NULL))
                    {
                        /* Report via USI callback */
                        dObj->cbFunc(dObj->usiReadBuffer, dObj->usiNumBytesRead, dObj->context);
                    }
                    
                    /* End of USI Message */
                    dObj->devStatus = USI_CDC_IDLE;
                }
                else if (*pData == USI_ESC_KEY_7D)
                {
                    /* Escape character */
                    dObj->devStatus = USI_CDC_ESC;
                } 
                else
                {
                    store = true;
                    charStore = *pData;
                }

                break;

            case USI_CDC_ESC:
            default:
                if (*pData == USI_ESC_KEY_5E)
                {
                    /* Store character after escape it */
                    store = true;
                    charStore = USI_ESC_KEY_7E;
                    dObj->devStatus = USI_CDC_RCV;
                }  
                else if (*pData == USI_ESC_KEY_5D)
                {
                    /* Store character after escape it */
                    store = true;
                    charStore = USI_ESC_KEY_7D;
                    dObj->devStatus = USI_CDC_RCV;
                }
                else
                {
                    /* ERROR: Escape format - restore USI buffer */
                    dObj->devStatus = USI_CDC_IDLE;
                }

                break;
        }

        if (store)
        {
            if (dObj->usiNumBytesRead < dObj->usiBufferSize)
            {
                /* Store character */
                dObj->usiReadBuffer[dObj->usiNumBytesRead++] = charStore;
            }
            else
            {
                /* ERROR: Full buffer - restore USI buffer */
                dObj->devStatus = USI_CDC_IDLE;
            }
        }

        if (dObj->cdcNumBytesRead > 0U)
        {
            dObj->cdcNumBytesRead--;
            pData++;
        }
    }
}

/*******************************************************
 * USB CDC Device Events - Event Handler
 *******************************************************/
static USB_DEVICE_CDC_EVENT_RESPONSE lUSB_CDC_DeviceCDCEventHandler(USB_DEVICE_CDC_INDEX index,
    USB_DEVICE_CDC_EVENT event, void * pData, uintptr_t userData)
{
    USI_CDC_OBJ* dObj;
    USB_CDC_CONTROL_LINE_STATE * controlLineStateData;
    USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE * eventDataRead;
    
    dObj = (USI_CDC_OBJ *)userData;

    switch(event)
    {
        case USB_DEVICE_CDC_EVENT_GET_LINE_CODING:

            /* This means the host wants to know the current line
             * coding. This is a control transfer request. Use the
             * USB_DEVICE_ControlSend() function to send the data to
             * host.  */

            (void) USB_DEVICE_ControlSend(dObj->devHandle, &dObj->getLineCodingData, sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:

            /* This means the host wants to set the line coding.
             * This is a control transfer request. Use the
             * USB_DEVICE_ControlReceive() function to receive the
             * data from the host */

            (void) USB_DEVICE_ControlReceive(dObj->devHandle, &dObj->setLineCodingData, sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:

            /* This means the host is setting the control line state.
             * Read the control line state. We will accept this request
             * for now. */

            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *)pData;
            dObj->controlLineStateData.dtr = controlLineStateData->dtr;
            dObj->controlLineStateData.carrier = controlLineStateData->carrier;

            (void) USB_DEVICE_ControlStatus(dObj->devHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:

            /* This means that the host is requesting that a break of the
             * specified duration be sent. Read the break duration */

            dObj->breakData = ((USB_DEVICE_CDC_EVENT_DATA_SEND_BREAK *)pData)->breakDuration;
            
            /* Complete the control transfer by sending a ZLP  */
            (void) USB_DEVICE_ControlStatus(dObj->devHandle, USB_DEVICE_CONTROL_STATUS_OK);
            
            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:

            /* This means that the host has sent some data*/
            eventDataRead = (USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE *)pData;
            
            if(eventDataRead->status != USB_DEVICE_CDC_RESULT_ERROR)
            {
                dObj->cdcIsReadComplete = true;
                
                dObj->cdcNumBytesRead = eventDataRead->length;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">

                /* Post semaphore to resume task */
                if (dObj->semaphoreID != NULL)
                {
                    (void) OSAL_SEM_PostISR(&dObj->semaphoreID);
                }
</#if>
            }
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            /* The data stage of the last control transfer is complete. */

            (void) USB_DEVICE_ControlStatus(dObj->devHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:

            /* This means the GET LINE CODING function data is valid. */
            break;

        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:
            /* This means that the data write got completed. */
            break;

        /* MISRA C-2012 deviation block start */
        /* MISRA C-2012 Rule 16.4 deviated once. Deviation record ID - H3_MISRAC_2012_R_16_4_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    <#if core.COMPILER_CHOICE == "XC32">
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    </#if>
        #pragma coverity compliance block deviate "MISRA C-2012 Rule 16.4" "H3_MISRAC_2012_R_16_4_DR_1"
</#if>
        default:
            break;

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
        #pragma coverity compliance end_block "MISRA C-2012 Rule 16.4"
    <#if core.COMPILER_CHOICE == "XC32">
        #pragma GCC diagnostic pop
    </#if>
</#if>
        /* MISRA C-2012 deviation block end */
    }

    return USB_DEVICE_CDC_EVENT_RESPONSE_NONE;
}

/***********************************************
 * USB Device Layer Event Handler.
 ***********************************************/
static void lUSI_CDC_DeviceEventHandler(USB_DEVICE_EVENT event, void * eventData,
    uintptr_t context)
{
    USB_DEVICE_EVENT_DATA_CONFIGURED *configuredEventData;
    USI_CDC_OBJ* dObj;
    
    dObj = (USI_CDC_OBJ*)context;

    switch(event)
    {
        case USB_DEVICE_EVENT_SOF:
            /* Flag determines SOF event occurrence */
            dObj->sofEventHasOccurred = true;
            break;

        case USB_DEVICE_EVENT_RESET:
            dObj->devStatus = USI_CDC_IDLE;
            dObj->usiStatus = SRV_USI_STATUS_UNINITIALIZED;
            break;

        case USB_DEVICE_EVENT_CONFIGURED:
            /* Check the configuration. We only support configuration 1 */
            configuredEventData = (USB_DEVICE_EVENT_DATA_CONFIGURED*)eventData;
            if (configuredEventData->configurationValue == 1U)
            {
                /* Register the CDC Device event handler */
                (void) USB_DEVICE_CDC_EventHandlerSet(dObj->cdcInstanceIndex, lUSB_CDC_DeviceCDCEventHandler, (uintptr_t)dObj);
                /* Mark that the device is now configured */
                dObj->usiStatus = SRV_USI_STATUS_CONFIGURED;
                /* Request first read */
                (void) USB_DEVICE_CDC_Read(dObj->cdcInstanceIndex, &dObj->readTransferHandle,
                        dObj->cdcReadBuffer, dObj->cdcBufferSize);
            }
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:
            /* VBUS was detected. We can attach the device */
            USB_DEVICE_Attach(dObj->devHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:
            /* VBUS is not available. We can detach the device */
            USB_DEVICE_Detach(dObj->devHandle);
            dObj->usiStatus = SRV_USI_STATUS_NOT_CONFIGURED;
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_ERROR:
            break;

        /* MISRA C-2012 deviation block start */
        /* MISRA C-2012 Rule 16.4 deviated once. Deviation record ID - H3_MISRAC_2012_R_16_4_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    <#if core.COMPILER_CHOICE == "XC32">
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    </#if>
        #pragma coverity compliance block deviate "MISRA C-2012 Rule 16.4" "H3_MISRAC_2012_R_16_4_DR_1"
</#if>
        default:
            break;

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
        #pragma coverity compliance end_block "MISRA C-2012 Rule 16.4"
    <#if core.COMPILER_CHOICE == "XC32">
        #pragma GCC diagnostic pop
    </#if>
</#if>
        /* MISRA C-2012 deviation block end */
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: USI CDC Service Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

void USI_CDC_Initialize(uint32_t index, const void * const initData)
{
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    OSAL_RESULT semResult;
</#if>
    USI_CDC_OBJ* dObj = USI_CDC_GET_INSTANCE(index);
    const USI_CDC_INIT_DATA * const dObjInit = (const USI_CDC_INIT_DATA * const)initData;
    
    if (dObj == NULL)
    {
        return;
    }

    dObj->cdcInstanceIndex = dObjInit->cdcInstanceIndex;
    dObj->cdcReadBuffer = dObjInit->cdcReadBuffer;
    dObj->usiReadBuffer = dObjInit->usiReadBuffer;
    dObj->cdcBufferSize = dObjInit->cdcBufferSize;
    dObj->usiBufferSize = dObjInit->usiBufferSize;

    dObj->cbFunc = NULL;
    dObj->devStatus = USI_CDC_IDLE;
    dObj->usiStatus = SRV_USI_STATUS_NOT_CONFIGURED;

    dObj->cdcIsReadComplete = false;
    dObj->readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    dObj->writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">

    /* Create semaphore. It is used to suspend and resume task. */
    semResult = OSAL_SEM_Create(&dObj->semaphoreID, OSAL_SEM_TYPE_BINARY, 0, 0);
    if ((semResult != OSAL_RESULT_SUCCESS) || (dObj->semaphoreID == NULL))
    {
        /* Error: Not enough memory to create semaphore */
        dObj->usiStatus = SRV_USI_STATUS_ERROR;
    }
</#if>
}

DRV_HANDLE USI_CDC_Open(uint32_t index)
{
    USI_CDC_OBJ* dObj = USI_CDC_GET_INSTANCE(index);
    
    if (dObj == NULL)
    {
        return DRV_HANDLE_INVALID;
    }

    /* Open the USB device layer */
    dObj->devHandle = USB_DEVICE_Open(dObj->cdcInstanceIndex, DRV_IO_INTENT_READWRITE);

    if(dObj->devHandle != USB_DEVICE_HANDLE_INVALID)
    {
        /* Register a callback with device layer to get event notification (for end point 0) */
        USB_DEVICE_EventHandlerSet(dObj->devHandle, lUSI_CDC_DeviceEventHandler, (uintptr_t)dObj);
        return (DRV_HANDLE)index;
    }
    else
    {
        return DRV_HANDLE_INVALID;
    }
}

void USI_CDC_Write(uint32_t index, void* pData, size_t length)
{
    USI_CDC_OBJ* dObj = USI_CDC_GET_INSTANCE(index);
    
    /* Check handler */
    if (dObj == NULL)
    {
        return;
    }

    if (length == 0U)
    {
        return;
    }
    
    if (dObj->usiStatus != SRV_USI_STATUS_CONFIGURED)
    {
        return;
    }
    
    (void) USB_DEVICE_CDC_Write(dObj->cdcInstanceIndex,
            &dObj->writeTransferHandle, pData, length,
            USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
}

bool USI_CDC_WriteIsBusy(uint32_t index)
{
    /* USB CDC is always free for writing */
    return false;
}

void USI_CDC_RegisterCallback(uint32_t index, USI_CDC_CALLBACK cbFunc,
        uintptr_t context)
{
    USI_CDC_OBJ* dObj = USI_CDC_GET_INSTANCE(index);
    
    /* Check handler */
    if (dObj == NULL)
    {
        return;
    }

    /* Set callback function */
    dObj->cbFunc = cbFunc;
    
    /* Set context related to cbFunc */
    dObj->context = context;
}

void USI_CDC_Close(uint32_t index)
{
    USI_CDC_OBJ* dObj = USI_CDC_GET_INSTANCE(index);
    
    /* Check handler */    
    if (dObj == NULL)
    {
        return;
    }

    /* Close the USB device layer */
    USB_DEVICE_Close(dObj->devHandle);

    dObj->usiStatus = SRV_USI_STATUS_NOT_CONFIGURED;
}

SRV_USI_STATUS USI_CDC_Status(uint32_t index)
{
    USI_CDC_OBJ* dObj = USI_CDC_GET_INSTANCE(index);
    
    /* Check handler */    
    if (dObj == NULL)
    {
        return SRV_USI_STATUS_ERROR;
    }

    return dObj->usiStatus;
}

void USI_CDC_Tasks (uint32_t index)
{
    USI_CDC_OBJ* dObj = USI_CDC_GET_INSTANCE(index);
    
    /* Check handler */    
    if (dObj == NULL)
    {
        return;
    }

<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    /* Suspend task until semaphore is posted */
    if (dObj->semaphoreID != NULL)
    {
        (void) OSAL_SEM_Pend(&dObj->semaphoreID, OSAL_WAIT_FOREVER);
    }

</#if>
    if (dObj->usiStatus != SRV_USI_STATUS_CONFIGURED)
    {
        return;
    }
    
    /* CDC reception process */
    if (dObj->cdcIsReadComplete == true)
    {
        dObj->cdcIsReadComplete = false;
        
        /* Extract CDC received data to USI buffer */
        lUSI_CDC_TransferReceivedData(dObj);

        /* Request next read */
        (void) USB_DEVICE_CDC_Read(dObj->cdcInstanceIndex, &dObj->readTransferHandle, dObj->cdcReadBuffer, dObj->cdcBufferSize);
    } 
}
