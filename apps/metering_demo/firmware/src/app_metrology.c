/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_metrology.c

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

#include "app_metrology.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* Define a switch detection semaphore to signal the Metrology to process new 
 * measurements */
OSAL_SEM_DECLARE(metSemaphore);

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_METROLOGY_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_METROLOGY_DATA CACHE_ALIGN app_metrologyData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void IPC0_Handler(void)
{
    uint32_t status = IPC0_REGS->IPC_ISR;
    
    if (status & IPC_ISR_IRQ20_Msk)
    {
        app_metrologyData.ipc_init = true;
    }
    
    if (status & IPC_ISR_IRQ5_Msk)
    {
        app_metrologyData.ipc_half_cycle = true;
    }
    
    if (status & IPC_ISR_IRQ4_Msk)
    {
        app_metrologyData.ipc_full_cycle = true;
    }
    
    if (status & IPC_ISR_IRQ0_Msk)
    {
        app_metrologyData.ipc_integration = true;
    }
    
    /* Signal Display thread to modify visualization. */
    OSAL_SEM_PostISR(&metSemaphore);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_METROLOGY_Initialize ( void )

  Remarks:
    See prototype in app_metrology.h.
 */

void APP_METROLOGY_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_metrologyData.state = APP_METROLOGY_STATE_INIT;


    /* Create the Switches Semaphore. */
    if (OSAL_SEM_Create(&metSemaphore, OSAL_SEM_TYPE_BINARY, 0, 0) == OSAL_RESULT_FALSE)
    {
        /* Handle error condition. Not sufficient memory to create semaphore */
    }
}


/******************************************************************************
  Function:
    void APP_METROLOGY_Tasks ( void )

  Remarks:
    See prototype in app_metrology.h.
 */

void APP_METROLOGY_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app_metrologyData.state )
    {
        /* Application's initial state. */
        case APP_METROLOGY_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                app_metrologyData.state = APP_METROLOGY_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_METROLOGY_STATE_SERVICE_TASKS:
        {
            /* Wait for the switches semaphore to modify the visualization. */
            OSAL_SEM_Pend(&metSemaphore, OSAL_WAIT_FOREVER);
            
            break;
        }

        /* TODO: implement your application state machine.*/


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
