/*******************************************************************************
  Interprocessor Communications PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_ipc1.h

  Summary:
    IPC1 Header File

  Description:
    This file declares the interface to the IPC peripheral library.
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
#ifndef PLIB_IPC1_H
#define PLIB_IPC1_H

#include "plib_ipc_common.h"

static inline void IPC1_SetIRQ(ipc_irq_mask_t irq)
{
    IPC1_REGS->IPC_ISCR = irq;
}

static inline void IPC1_ClearIRQ(ipc_irq_mask_t irq)
{
    IPC1_REGS->IPC_ICCR = irq;
}

static inline ipc_irq_mask_t IPC1_GetPendingIRQs(void)
{
    return IPC1_REGS->IPC_IPR;
}

static inline void IPC1_EnableIRQ(ipc_irq_mask_t irq)
{
    IPC1_REGS->IPC_IECR = irq;
}

static inline void IPC1_DisableIRQ(ipc_irq_mask_t irq)
{
    IPC1_REGS->IPC_IDCR = irq;
}

static inline ipc_irq_mask_t IPC1_GetEnabledIRQs(void)
{
    return IPC1_REGS->IPC_IMR;
}

static inline ipc_irq_mask_t IPC1_GetIRQStatus(void)
{
    return IPC1_REGS->IPC_ISR;
}

void  IPC1_Initialize(void);

void IPC1_SetIRQHandler(ipc_irq_mask_t irq, IPC_CALLBACK handler, uintptr_t context);

#endif //PLIB_IPC1_H