/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

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

#include <string.h>
#include "app_plc.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#define DRV_PL360_INDEX_0   0

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_PLC_DATA appPLC;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void APP_PLC_ExceptionCb(DRV_PL360_EXCEPTION exceptionObj, uintptr_t context )
{
    /* Avoid warning */
    (void)context;
    
    switch (exceptionObj) 
    {
        case DRV_PL360_EXCEPTION_UNEXPECTED_KEY:
            printf("Exception: DRV_PL360_EXCEPTION_UNEXPECTED_KEY]\r\n");
            break;

        case DRV_PL360_EXCEPTION_CRITICAL_ERROR:
            printf("Exception: DRV_PL360_EXCEPTION_CRITICAL_ERROR\r\n");
            break;

        case DRV_PL360_EXCEPTION_RESET:
            printf("Exception: DRV_PL360_EXCEPTION_RESET\r\n");
            break;

        case DRV_PL360_EXCEPTION_DEBUG:
            printf("Exception: DRV_PL360_EXCEPTION_DEBUG\r\n");
            break;

        default:
            printf("Exception: UNKNOWN\r\n");
	}

	appPLC.pl360_exception = true;
}

static void APP_PLC_DataCfmCb(DRV_PL360_TRANSMISSION_CFM_OBJ *cfmObj, uintptr_t context )
{
    /* Avoid warning */
    (void)context;
    
    /* Handle result of transmission : Show it through Console */
    switch(cfmObj->result)
    {
        case DRV_PL360_TX_RESULT_PROCESS:
            printf("...DRV_PL360_TX_RESULT_PROCESS\r\n");
            break;   
        case DRV_PL360_TX_RESULT_SUCCESS:
            printf("...DRV_PL360_TX_RESULT_PROCESS\r\n");
            break;   
        case DRV_PL360_TX_RESULT_INV_LENGTH:
            printf("...DRV_PL360_TX_RESULT_INV_LENGTH\r\n");
            break;
        case DRV_PL360_TX_RESULT_BUSY_CH:
            printf("...DRV_PL360_TX_RESULT_BUSY_CH\r\n");
            break;    
        case DRV_PL360_TX_RESULT_BUSY_TX:
            printf("...DRV_PL360_TX_RESULT_BUSY_TX\r\n");
            break;    
        case DRV_PL360_TX_RESULT_BUSY_RX:
            printf("...DRV_PL360_TX_RESULT_BUSY_RX\r\n");
            break;   
        case DRV_PL360_TX_RESULT_INV_SCHEME:
            printf("...DRV_PL360_TX_RESULT_INV_SCHEME\r\n");
            break; 
        case DRV_PL360_TX_RESULT_TIMEOUT:
            printf("...DRV_PL360_TX_RESULT_TIMEOUT\r\n");
            break;   
        case DRV_PL360_TX_RESULT_INV_TONEMAP:
            printf("...DRV_PL360_TX_RESULT_INV_TONEMAP\r\n");
            break;
        case DRV_PL360_TX_RESULT_INV_MODE:
            printf("...DRV_PL360_TX_RESULT_INV_MODE\r\n");
            break;   
        case DRV_PL360_TX_RESULT_NO_TX:
            printf("...DRV_PL360_TX_RESULT_NO_TX\r\n");
            break;   
    }
    
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_PLC_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_PLC_Initialize ( void )
{
    /* Update state machine */
    appPLC.state = APP_PLC_STATE_INIT; 
    
    /* Init PLC objects */
    appPLC.pl360Tx.pTransmitData = appPLC.pDataTx;
    
}

/******************************************************************************
  Function:
    void APP_PLC_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_PLC_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appPLC.state )
    {
        /* Application's initial state. */
        case APP_PLC_STATE_INIT:
        {
            /* Open PL360 driver */
            appPLC.drvPl360Handle = DRV_PL360_Open(DRV_PL360_INDEX_0, DRV_IO_INTENT_READWRITE);
            
            if (appPLC.drvPl360Handle != DRV_HANDLE_INVALID)
            {
                appPLC.state = APP_PLC_STATE_CONFIG;
            }
            else
            {
                appPLC.state = APP_PLC_STATE_ERROR;
            }
            break;
        }
            
        case APP_PLC_STATE_CONFIG:
        {
            /* Check PL360 device */
            if (DRV_PL360_Status(DRV_PL360_INDEX_0) == SYS_STATUS_READY)
            {
                DRV_PL360_PIB_OBJ pibObj;
                
                /* Configure PL360 callbacks */
                DRV_PL360_ExceptionCallbackRegister(appPLC.drvPl360Handle, APP_PLC_ExceptionCb, DRV_PL360_INDEX_0);
                DRV_PL360_DataCfmCallbackRegister(appPLC.drvPl360Handle, APP_PLC_DataCfmCb, DRV_PL360_INDEX_0);
                
                /* Get PL360 PHY version */
                pibObj.id = PL360_ID_VERSION_NUM;
                pibObj.length = 4;
                pibObj.pData = (uint8_t *)&appPLC.pl360PhyVersion;
                DRV_PL360_PIBGet(appPLC.drvPl360Handle, &pibObj);
                
                /* Waiting Console action */
                appPLC.state = APP_PLC_STATE_WAITING;
            }                
            break;
        }

        case APP_PLC_STATE_WAITING:
        {
            break;
        }

        case APP_PLC_STATE_TX:
        {
            break;
        }

        case APP_PLC_STATE_STOP_TX:
        {
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
