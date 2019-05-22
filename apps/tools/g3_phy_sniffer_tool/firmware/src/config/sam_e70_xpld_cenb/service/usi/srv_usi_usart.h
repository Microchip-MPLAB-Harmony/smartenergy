/*******************************************************************************
  USART wrapper used from USI service Header File

  Company
    Microchip Technology Inc.

  File Name
    srv_usi_usart.h

  Summary
    USART wrapper used from USI service interface.

  Description
    The USART wrapper provides a simple interface to manage the USART_PLIB
    module on Microchip microcontrollers. This file implements the core
    interface routines for the USI PLC service.

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#ifndef SRV_USART_USI_H    // Guards against multiple inclusion
#define SRV_USART_USI_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "system/system.h"
#include "service/usi/srv_usi_definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef void ( * USI_USART_CALLBACK ) ( uint8_t *data, uint16_t length, uintptr_t context ); 

typedef struct
{
    uint8_t*                                 pMessage;
    uint8_t*                                 pDataRd;
    size_t                                   length;
    bool                                     inUse;
    struct USI_USART_MSG*                    next; 
} USI_USART_MSG; 

typedef struct
{
    USI_USART_MSG*                           front;
    USI_USART_MSG*                           rear;
} USI_USART_MSG_QUEUE; 

typedef enum
{
    USI_USART_IDLE,
    USI_USART_RCV,
    USI_USART_ESC
} USI_USART_STATE;

typedef struct
{
    void*                                    plib;
    void*                                    pRdBuffer;
    void*                                    pWrBuffer;
    size_t                                   rdBufferSize;
    size_t                                   wrBufferSize;
} USI_USART_INIT; 

typedef struct
{
    SRV_USI_USART_INTERFACE*                 plib;
    USI_USART_CALLBACK                       cbFunc;
    void*                                    pWrBuffer;
    void*                                    pRdBuffer;
    size_t                                   wrBufferSize;
    size_t                                   rdBufferSize;
    size_t                                   writeLength;
    size_t                                   byteCount;
    uint8_t                                  rcvChar;
    USI_USART_MSG*                           pRcvMsg;
    USI_USART_MSG_QUEUE*                     pMsgQueue;
    USI_USART_STATE                          status;
    uintptr_t                                context;
} USI_USART_OBJ;
        
// *****************************************************************************
// *****************************************************************************
// Section: SRV_USI Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

DRV_HANDLE USI_USART_Initialize(const USI_USART_INIT* const init);

void USI_USART_Tasks (SYS_MODULE_OBJ object);

size_t USI_USART_Write(DRV_HANDLE handle, size_t length);

void USI_USART_RegisterCallback(DRV_HANDLE handle, USI_USART_CALLBACK cbFunc, uintptr_t context);

void USI_USART_Flush(DRV_HANDLE handle);

void USI_USART_Close(DRV_HANDLE handle);

#endif //SRV_USART_USI_H
