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
    appcore1.c

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

#include "appcore1.h"
#include "definitions.h"
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
    This structure should be initialized by the APPCORE1_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APPCORE1_DATA appcore1Data;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void _APPCORE1_IPC1Callback(ipc_irq_mask_t irq_mask, uintptr_t context)
{
    if (irq_mask == IPC_IRQ0_MASK)
    {
        appcore1Data.core0Signal = true;
    }
}

static void _APPCORE1_SysTimeCallbackLedToggle ( uintptr_t context )
{
    LED_RED_Toggle();
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APPCORE1_Initialize ( void )

  Remarks:
    See prototype in appcore1.h.
 */

void APPCORE1_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appcore1Data.state = APPCORE1_STATE_INIT;
    /* Clear status of IPC0 flag */
    appcore1Data.core0Signal = false;

    /* Initialize shared counter */
    appSharedData.counter = 0;

    /* Start LED activity */
    appcore1Data.timerLed = SYS_TIME_CallbackRegisterMS(_APPCORE1_SysTimeCallbackLedToggle, 0,
            APPCORE1_TIMER_LED_MS, SYS_TIME_PERIODIC);

    /* Set IPC0 callback to attend the IPC signals from Core 0 */
    IPC1_SetIRQHandler(IPC_IRQ0_MASK, _APPCORE1_IPC1Callback, 0);
}


/******************************************************************************
  Function:
    void APPCORE1_Tasks ( void )

  Remarks:
    See prototype in appcore1.h.
 */

void APPCORE1_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( appcore1Data.state )
    {
        /* Application's initial state. */
        case APPCORE1_STATE_INIT:
        {
            if (appcore1Data.core0Signal)
            {
                appcore1Data.core0Signal = false;

                printf("APPCORE1_STATE_INIT done!!\r\n");

                SYS_TIME_DelayMS(APPCORE1_DELAYS_BETWEEN_CMDS_MS, &appcore1Data.timer);
                appcore1Data.state = APPCORE1_STATE_SEND_CMD;
            }
            break;
        }

        case APPCORE1_STATE_WAIT_CMD:
        {
            if (appcore1Data.core0Signal)
            {
                printf("Received signal from Core 0\r\n");
                appcore1Data.core0Signal = false;
                // Increase counter in shared memory
                appSharedData.counter++;

                SYS_TIME_DelayMS(APPCORE1_DELAYS_BETWEEN_CMDS_MS, &appcore1Data.timer);
                appcore1Data.state = APPCORE1_STATE_SEND_CMD;
            }
            break;
        }

        case APPCORE1_STATE_SEND_CMD:
        {
            if (SYS_TIME_DelayIsComplete(appcore1Data.timer) == true)
            {
                printf("Send C1->C0\r\n");
                // Send IPC command to Core 0 through IPC0
                IPC0_SetIRQ(IPC_IRQ1_MASK);

                appcore1Data.state = APPCORE1_STATE_WAIT_CMD;
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
