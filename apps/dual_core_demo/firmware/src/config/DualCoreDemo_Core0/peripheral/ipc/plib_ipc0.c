/*******************************************************************************
  Interprocessor Communications PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_ipc0.c

  Summary:
    IPC0 Source File

  Description:
    This file defines the interface to the IPC peripheral library.
    This library provides access to and control of the associated
    InterProcessor Communication peripheral.

  Remarks:
    None.

*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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
#include <stddef.h>
#include "device.h"
#include "interrupts.h"
#include "plib_ipc0.h"

typedef struct
{
    IPC_CALLBACK handler;
    uintptr_t context;
}ipc0_callback_object_t;

static ipc0_callback_object_t ipc0_callback_obj[TOTAL_IPC_IRQS];

void  IPC0_Initialize(void)
{
    /* Clear interrupts */
    IPC0_REGS->IPC_ICCR = IPC_ICCR_Msk;
    /* Enable interrupts */
    IPC0_REGS->IPC_IECR = (IPC_IECR_IRQ1_Msk);

}

void IPC0_SetIRQHandler(ipc_irq_mask_t irq_mask, IPC_CALLBACK handler, uintptr_t context)
{
    for (uint32_t irq_id = 0; irq_id < TOTAL_IPC_IRQS; irq_id++)
    {
        if ((irq_mask & ((uint32_t)1U << irq_id)) != 0U)
        {
            ipc0_callback_obj[irq_id].handler = handler;
            ipc0_callback_obj[irq_id].context = context;
        }
    }
}


void  IPC0_InterruptHandler(void)
{
    for (uint32_t irq_id = 0 ; irq_id < TOTAL_IPC_IRQS; irq_id++)
    {
        ipc_irq_mask_t irq_mask =  ((uint32_t)1U << irq_id);
        if ((IPC0_GetIRQStatus() & irq_mask) != 0U)
        {
            IPC_CALLBACK handler = ipc0_callback_obj[irq_id].handler;
            uintptr_t context = ipc0_callback_obj[irq_id].context;
            if (handler != NULL)
            {
                handler(irq_mask, context);
            }
            /* Clear interrupts */
            IPC0_REGS->IPC_ICCR = (irq_mask);
        }
    }
}

