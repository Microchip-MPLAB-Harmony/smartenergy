/*******************************************************************************
  Interprocessor Communications PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_ipc_common.h

  Summary:
    IPC common header file

  Description:
    This file declares common interfaces and datatypes for all IPC peripherals.

  Remarks:
    None.

*******************************************************************************/
// DOM-IGNORE-BEGIN
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
#ifndef PLIB_IPC_COMMON_H
#define PLIB_IPC_COMMON_H

typedef uint32_t ipc_irq_mask_t;

#define IPC_IRQ0_MASK    ((uint32_t)1U << 0)
#define IPC_IRQ1_MASK    ((uint32_t)1U << 1)
#define IPC_IRQ2_MASK    ((uint32_t)1U << 2)
#define IPC_IRQ3_MASK    ((uint32_t)1U << 3)
#define IPC_IRQ4_MASK    ((uint32_t)1U << 4)
#define IPC_IRQ5_MASK    ((uint32_t)1U << 5)
#define IPC_IRQ6_MASK    ((uint32_t)1U << 6)
#define IPC_IRQ7_MASK    ((uint32_t)1U << 7)
#define IPC_IRQ8_MASK    ((uint32_t)1U << 8)
#define IPC_IRQ9_MASK    ((uint32_t)1U << 9)
#define IPC_IRQ10_MASK    ((uint32_t)1U << 10)
#define IPC_IRQ11_MASK    ((uint32_t)1U << 11)
#define IPC_IRQ12_MASK    ((uint32_t)1U << 12)
#define IPC_IRQ13_MASK    ((uint32_t)1U << 13)
#define IPC_IRQ14_MASK    ((uint32_t)1U << 14)
#define IPC_IRQ15_MASK    ((uint32_t)1U << 15)
#define IPC_IRQ16_MASK    ((uint32_t)1U << 16)
#define IPC_IRQ17_MASK    ((uint32_t)1U << 17)
#define IPC_IRQ18_MASK    ((uint32_t)1U << 18)
#define IPC_IRQ19_MASK    ((uint32_t)1U << 19)
#define IPC_IRQ20_MASK    ((uint32_t)1U << 20)
#define IPC_IRQ21_MASK    ((uint32_t)1U << 21)
#define IPC_IRQ22_MASK    ((uint32_t)1U << 22)
#define IPC_IRQ23_MASK    ((uint32_t)1U << 23)
#define IPC_IRQ24_MASK    ((uint32_t)1U << 24)
#define IPC_IRQ25_MASK    ((uint32_t)1U << 25)
#define IPC_IRQ26_MASK    ((uint32_t)1U << 26)
#define IPC_IRQ27_MASK    ((uint32_t)1U << 27)
#define IPC_IRQ28_MASK    ((uint32_t)1U << 28)
#define IPC_IRQ29_MASK    ((uint32_t)1U << 29)
#define IPC_IRQ30_MASK    ((uint32_t)1U << 30)
#define IPC_IRQ31_MASK    ((uint32_t)1U << 31)

#define TOTAL_IPC_IRQS      32U

typedef void (*IPC_CALLBACK) (ipc_irq_mask_t irq_mask, uintptr_t context);

#endif //PLIB_IPC_COMMON_H