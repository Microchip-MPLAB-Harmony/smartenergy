/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_energy.c

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

#include "definitions.h"
#include "app_energy.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* Define a semaphore to signal the Energy Tasks to process new measurements */
OSAL_SEM_DECLARE(energySemaphore);

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_ENERGY_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_ENERGY_DATA app_energyData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

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
    void APP_ENERGY_Initialize ( void )

  Remarks:
    See prototype in app_energy.h.
 */

void APP_ENERGY_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_energyData.state = APP_ENERGY_STATE_INIT;
    
    /* TOU Initialization */
    
    /* Demand Initialization */
    
    /* Energy Initialization */
    
    /* Met Events Initialization */

    /* Create the Switches Semaphore. */
    if (OSAL_SEM_Create(&energySemaphore, OSAL_SEM_TYPE_BINARY, 0, 0) == OSAL_RESULT_FALSE)
    {
        /* Handle error condition. Not sufficient memory to create semaphore */
    }
}


/******************************************************************************
  Function:
    void APP_ENERGY_Tasks ( void )

  Remarks:
    See prototype in app_energy.h.
 */

void APP_ENERGY_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app_energyData.state )
    {
        /* Application's initial state. */
        case APP_ENERGY_STATE_INIT:
        {
            /* TOU Management */

            app_energyData.state = APP_ENERGY_STATE_RUNNING;

            break;
        }

        case APP_ENERGY_STATE_RUNNING:
        {
             /* Wait for the switches semaphore to modify the visualization. */
            OSAL_SEM_Pend(&energySemaphore, OSAL_WAIT_FOREVER);
            
            

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
