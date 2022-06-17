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

#include "definitions.h"
#include "app_metrology.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* Define a semaphore to signal the Metrology Tasks to process new integration
 * data */
OSAL_SEM_DECLARE(metrologySemaphore);

extern OSAL_SEM_DECLARE(energySemaphore);

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

void APP_METROLOGY_NewIntegrationCallback(void)
{
    /* Signal Metrology thread to update measurements for an integration period */
    OSAL_SEM_PostISR(&metrologySemaphore);
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
    
    /* Get Pointers to metrology data regions */
    app_metrologyData.pMetControl = DRV_METROLOGY_GetControl();
    app_metrologyData.pMetAccData = DRV_METROLOGY_GetAccData();
    app_metrologyData.pMetHarData = DRV_METROLOGY_GetHarData();
    
    /* Set Callback for each metrology integration process */
    DRV_METROLOGY_IntegrationCallbackRegister(APP_METROLOGY_NewIntegrationCallback);
    
    /* Create the Switches Semaphore. */
    if (OSAL_SEM_Create(&metrologySemaphore, OSAL_SEM_TYPE_BINARY, 0, 0) == OSAL_RESULT_FALSE)
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
//extern uint32_t counterIPC;
//extern uint32_t counterIPC20;
//extern uint32_t counterIPC0;
//uint32_t counter = 0;
void APP_METROLOGY_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app_metrologyData.state )
    {
        /* Application's initial state. */
        case APP_METROLOGY_STATE_INIT:
        {
            DRV_METROLOGY_START_MODE startMode = DRV_METROLOGY_START_HARD;
            
            /* Detection of the WDOG0 Reset */
            if (RSTC_ResetCauseGet() == RSTC_SR_RSTTYP(RSTC_SR_RSTTYP_WDT0_RST_Val))
            {
                startMode = DRV_METROLOGY_START_SOFT;
            }
            
            if (DRV_METROLOGY_Open(startMode) == DRV_METROLOGY_SUCCESS)
            {
                if (startMode == DRV_METROLOGY_START_HARD)
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_START;
                }
                else
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_RUNNING;
                }
            }
            else
            {
                app_metrologyData.state = APP_METROLOGY_STATE_ERROR;
            }
            
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_METROLOGY_STATE_START:
        {
            if (DRV_METROLOGY_GetState() == DRV_METROLOGY_STATE_READY)
            {
//                /* Check and update if there are some MET_CONTROL values stored in DataLog */
//                MET_CONTROL metControl;
//                
//                /* Read Metrology Control Configuration from Data Log */
//                if (APP_DATALOG_Read(ID_MET_CONTROL, &metControl, sizeof(MET_CONTROL)))
//                {
//                    /* Apply the stored Control configuration */
//                    DRV_METROLOGY_SetControl(&metControl);
//                }
//                else
//                {
//                    /* Store Metrology Control Configuration in Data Log */
//                    APP_DATALOG_Write(ID_MET_CONTROL, app_metrologyData.pMetControl, sizeof(MET_CONTROL));
//                }
                
                if (DRV_METROLOGY_Start() == DRV_METROLOGY_SUCCESS)
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_RUNNING;
                }
                else
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_ERROR;
                }
                
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
            
            break;
        }

        case APP_METROLOGY_STATE_RUNNING:
        {
            /* Wait for the switches semaphore to modify the visualization. */
            OSAL_SEM_Pend(&metrologySemaphore, OSAL_WAIT_FOREVER);
            
//            counter++;
//            SYS_CONSOLE_Print(SYS_CONSOLE_INDEX_0, "Metrology -> received new Integration Data %u : %u\t%u\t%u \r\n", 
//                    counter, counterIPC, counterIPC20, counterIPC0);
            
            GPIODBG_Set();
            DRV_METROLOGY_UpdateMeasurements();
            GPIODBG_Clear();
            
            /* Signal Energy thread to process energy values */
            OSAL_SEM_Post(&energySemaphore);
            
            break;
        }

        /* The default state should never be executed. */
        case APP_METROLOGY_STATE_ERROR:
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
