/*
Copyright (C) 2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/

/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    appcore0.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "appcore0.h"
#include "definitions.h"
#include "DualCoreDemo_Core1.h"
#include "appcore0.h"
#include "peripheral/ipc/plib_ipc0.h"
#include "peripheral/ipc/plib_ipc1.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
typedef struct
{
    uint32_t counter;
} APP_SHARED_DATA;

volatile APP_SHARED_DATA  __attribute__((persistent,address(0x20088000))) appSharedData;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APPCORE0_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APPCORE0_DATA appcore0Data;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void _APPCORE0_IPC0Callback(ipc_irq_mask_t irq_mask, uintptr_t context)
{
    if (irq_mask == IPC_IRQ1_MASK)
    {
        appcore0Data.core1Signal = true;
    }
}

static void _APPCORE0_SysTimeCallback1sec ( uintptr_t context )
{
    appcore0Data.timerExpired = true;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
static void _APPCORE0_CoprocessorInitialize(void)
{
    uint32_t tmp;

    /* Assert reset of the coprocessor and its peripherals */
    tmp = RSTC_REGS->RSTC_MR;
    tmp &= ~(RSTC_MR_CPROCEN_Msk | RSTC_MR_CPEREN_Msk);
    RSTC_REGS->RSTC_MR = RSTC_MR_KEY_PASSWD | tmp;

    /* Disable coprocessor Clocks */
    PMC_REGS->PMC_SCDR = PMC_SCDR_CPKEY_PASSWD | PMC_SCDR_CPCK_Msk;

    /* Disable coprocessor peripheral clocks */
    PMC_REGS->PMC_SCDR = PMC_SCDR_CPKEY_PASSWD | PMC_SCDR_CPBMCK_Msk;

    /* De-assert reset of the coprocessor peripherals */
    RSTC_REGS->RSTC_MR |= RSTC_MR_KEY_PASSWD | RSTC_MR_CPEREN_Msk;

    /* Enable coprocessor peripheral clocks */
    PMC_REGS->PMC_SCER = PMC_SCER_CPKEY_PASSWD | PMC_SCER_CPBMCK_Msk;

    __xc32_LoadAllSecondarySections();

    /* De-assert the reset of the coprocessor (Core 1) */
    RSTC_REGS->RSTC_MR |= (RSTC_MR_KEY_PASSWD | RSTC_MR_CPROCEN_Msk);

    /* Enable the coprocessor clock (Core 1) */
    PMC_REGS->PMC_SCER = (PMC_SCER_CPKEY_PASSWD | PMC_SCER_CPCK_Msk);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APPCORE0_Initialize ( void )

  Remarks:
    See prototype in appcore0.h.
 */

void APPCORE0_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appcore0Data.state = APPCORE0_STATE_INIT;
    /* Clear status of IPC0 flag */
    appcore0Data.core1Signal = false;
    /* Clear Timer flag */
    appcore0Data.timerExpired = false;
    /* Set IPC0 callback to attend the IPC signals from Core 1 */
    IPC0_SetIRQHandler(IPC_IRQ1_MASK, _APPCORE0_IPC0Callback, 0);

    appSharedData.counter = 0xA5;
}


/******************************************************************************
  Function:
    void APPCORE0_Tasks ( void )

  Remarks:
    See prototype in appcore0.h.
 */

void APPCORE0_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( appcore0Data.state )
    {
        /* Application's initial state. */
        case APPCORE0_STATE_INIT:
        {
            printf("APPCORE0_STATE_INIT done!!\r\n");
            appcore0Data.state = APPCORE0_STATE_CORE1_INIT;
            break;
        }

        case APPCORE0_STATE_CORE1_INIT:
        {
            _APPCORE0_CoprocessorInitialize();
            appcore0Data.timer = SYS_TIME_CallbackRegisterMS(_APPCORE0_SysTimeCallback1sec, 0,
                    APPCORE0_TIMER_BETWEEN_CMDS_MS, SYS_TIME_SINGLE);
            appcore0Data.state = APPCORE0_STATE_SEND_CMD;
            break;
        }

        case APPCORE0_STATE_WAIT_CMD:
        {
            if (appcore0Data.core1Signal)
            {
                appcore0Data.core1Signal = false;
                printf("Received signal from Core 1 - shared counter : %u\r\n",
                        (unsigned int)appSharedData.counter);

                appcore0Data.timer = SYS_TIME_CallbackRegisterMS(_APPCORE0_SysTimeCallback1sec, 0,
                        APPCORE0_TIMER_BETWEEN_CMDS_MS, SYS_TIME_SINGLE);
                appcore0Data.state = APPCORE0_STATE_SEND_CMD;
            }
            break;
        }

        case APPCORE0_STATE_SEND_CMD:
        {
            if (appcore0Data.timerExpired)
            {
                printf("Send C0->C1\r\n");
                appcore0Data.timerExpired = false;
                // Send IPC command to Core 1 through IPC1
                IPC1_SetIRQ(IPC_IRQ0_MASK);

                appcore0Data.state = APPCORE0_STATE_WAIT_CMD;
            }

            break;
        }

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
