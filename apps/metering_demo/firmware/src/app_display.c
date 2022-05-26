/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_display.c

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

#include "app_display.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* Define a switch detection semaphore to signal the Display to select new 
 * measurement to visualize. */
OSAL_SEM_DECLARE(switchesSemaphore);

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_DISPLAY_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DISPLAY_DATA app_displayData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
static void APP_DISPLAY_ScrollUp_Callback ( PIO_PIN pin, uintptr_t context)
{
    app_displayData.scrup_pressed = true;
    /* Signal Display thread to modify visualization. */
    OSAL_SEM_PostISR(&switchesSemaphore);
}

static void APP_DISPLAY_ScrollDown_Callback ( PIO_PIN pin, uintptr_t context)
{
    app_displayData.scrdown_pressed = true;
    /* Signal Display thread to modify visualization. */
    OSAL_SEM_PostISR(&switchesSemaphore);
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
    void APP_DISPLAY_Initialize ( void )

  Remarks:
    See prototype in app_display.h.
 */

void APP_DISPLAY_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_displayData.state = APP_DISPLAY_STATE_INIT;

    app_displayData.scrdown_pressed = false;
    app_displayData.scrup_pressed = false;
    
    /* Create the Switches Semaphore. */
    if (OSAL_SEM_Create(&switchesSemaphore, OSAL_SEM_TYPE_BINARY, 0, 0) == OSAL_RESULT_FALSE)
    {
        /* Handle error condition. Not sufficient memory to create semaphore */
    }
}


/******************************************************************************
  Function:
    void APP_DISPLAY_Tasks ( void )

  Remarks:
    See prototype in app_display.h.
 */

void APP_DISPLAY_Tasks ( void )
{
    switch ( app_displayData.state )
    {
        case APP_DISPLAY_STATE_INIT:
        {
            PIO_PinInterruptCallbackRegister(SWITCH_SCRUP_PIN, 
                    APP_DISPLAY_ScrollUp_Callback, (uintptr_t)NULL);
            
            PIO_PinInterruptCallbackRegister(SWITCH_SCRDOWN_PIN, 
                    APP_DISPLAY_ScrollDown_Callback, (uintptr_t)NULL);
            
            PIO_PinInterruptEnable(SWITCH_SCRUP_PIN);
            PIO_PinInterruptEnable(SWITCH_SCRDOWN_PIN);
            
            app_displayData.state = APP_DISPLAY_STATE_SERVICE_TASKS;
            break;
        }

        case APP_DISPLAY_STATE_SERVICE_TASKS:
        {
            /* Wait for the switches semaphore to modify the visualization. */
            OSAL_SEM_Pend(&switchesSemaphore, OSAL_WAIT_FOREVER);
            
            if (app_displayData.scrup_pressed)
            {
                LED_RED_Set();
                app_displayData.scrup_pressed = false;
            }
            
            if (app_displayData.scrdown_pressed)
            {
                LED_RED_Clear();
                app_displayData.scrdown_pressed = false;
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
