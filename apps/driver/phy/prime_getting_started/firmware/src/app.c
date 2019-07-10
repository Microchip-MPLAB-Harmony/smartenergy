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
#include "app.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

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

APP_DATA appData;

#define APP_TX_DELAY_US           1000000 /* 1 second */

#define div_ceil(a, b)            (((a) + (b) - 1) / (b))

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void Timer1_Callback ( uintptr_t context )
{
    /* RX Led off */
    LED_Set();
}

static void _pl360ExceptionCb(DRV_PLC_PHY_EXCEPTION exceptionObj, uintptr_t context )
{
    switch (exceptionObj) 
    {
        case DRV_PLC_PHY_EXCEPTION_UNEXPECTED_KEY:
            appData.plc_phy_err_unexpected++;
            SYS_DEBUG_Print("Exception: DRV_PLC_PHY_EXCEPTION_UNEXPECTED_KEY [%u]\r\n", appData.plc_phy_err_unexpected);
            break;

        case DRV_PLC_PHY_EXCEPTION_CRITICAL_ERROR:
            appData.plc_phy_err_critical++;
            SYS_DEBUG_Print("Exception: DRV_PLC_PHY_EXCEPTION_CRITICAL_ERROR [%u]\r\n", appData.plc_phy_err_critical);
            break;

        case DRV_PLC_PHY_EXCEPTION_RESET:
            appData.plc_phy_err_reset++;
            SYS_DEBUG_Print("Exception: DRV_PLC_PHY_EXCEPTION_RESET [%u]\r\n", appData.plc_phy_err_reset);
            break;

        default:
            appData.plc_phy_err_unknow++;
            SYS_DEBUG_Print("Exception: UNKNOWN [%u]\r\n", appData.plc_phy_err_unknow);
	}

	appData.plc_phy_exception = true;
}

static void _pl360DataCfmCb(DRV_PLC_PHY_TRANSMISSION_CFM_OBJ *cfmObj, uintptr_t context )
{
    appData.state = APP_STATE_TX;
    
    /* Handle result of transmission : Show it through Console */
    switch(cfmObj->result)
    {
        case DRV_PLC_PHY_TX_RESULT_PROCESS:
            SYS_DEBUG_Print("...DRV_PLC_PHY_TX_RESULT_PROCESS\r\n");
            break;   
        case DRV_PLC_PHY_TX_RESULT_SUCCESS:
            SYS_DEBUG_Print("...DRV_PLC_PHY_TX_RESULT_PROCESS\r\n");
            break;   
        case DRV_PLC_PHY_TX_RESULT_INV_LENGTH:
            SYS_DEBUG_Print("...DRV_PLC_PHY_TX_RESULT_INV_LENGTH\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_BUSY_CH:
            SYS_DEBUG_Print("...DRV_PLC_PHY_TX_RESULT_BUSY_CH\r\n");
            break;    
        case DRV_PLC_PHY_TX_RESULT_BUSY_TX:
            SYS_DEBUG_Print("...DRV_PLC_PHY_TX_RESULT_BUSY_TX\r\n");
            break;    
        case DRV_PLC_PHY_TX_RESULT_BUSY_RX:
            SYS_DEBUG_Print("...DRV_PLC_PHY_TX_RESULT_BUSY_RX\r\n");
            break;   
        case DRV_PLC_PHY_TX_RESULT_INV_SCHEME:
            SYS_DEBUG_Print("...DRV_PLC_PHY_TX_RESULT_INV_SCHEME\r\n");
            break; 
        case DRV_PLC_PHY_TX_RESULT_TIMEOUT:
            SYS_DEBUG_Print("...DRV_PLC_PHY_TX_RESULT_TIMEOUT\r\n");
            break;   
        case DRV_PLC_PHY_TX_RESULT_INV_BUFFER:
            SYS_DEBUG_Print("...DRV_PLC_PHY_TX_RESULT_INV_BUFFER\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_INV_MODE:
            SYS_DEBUG_Print("...DRV_PLC_PHY_TX_RESULT_INV_MODE\r\n");
            break;   
        case DRV_PLC_PHY_TX_RESULT_NO_TX:
            SYS_DEBUG_Print("...DRV_PLC_PHY_TX_RESULT_NO_TX\r\n");
            break;   
    }
    
}

static void _pl360DataIndCb(DRV_PLC_PHY_RECEPTION_OBJ *indObj, uintptr_t context )
{   
    /* Shows Received PLC message */
	if (indObj->dataLength) {
        /* Start Timer: LED blinking for each received message */
        LED_Clear();
        appData.tmr1Handle = SYS_TIME_CallbackRegisterMS(Timer1_Callback, 0, LED_RX_OFF_RATE_MS, SYS_TIME_SINGLE);
        /* Copy data to Application buffer, excluding CRC info */
        memcpy(appData.pDataRx, indObj->pReceivedData, indObj->dataLength);

        /* Show received message */
        appData.pDataRx[indObj->dataLength] = 0;
        SYS_DEBUG_Print("<- Received new msg (%u bytes)\r\n", (unsigned int)indObj->dataLength);
        SYS_DEBUG_Print("\tData msg: %s", appData.pDataRx);
	}
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
#define DRV_PLC_PHY_INDEX_0   0

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
static void _setupTransmissionParameters (void)
{    
	/* Set scheme of modulation */
	appData.pl360Tx.scheme = SCHEME_DBPSK_C;
	/* Set frame type */
	appData.pl360Tx.frameType = FRAME_TYPE_A;
	/* Disable forced mode */
	appData.pl360Tx.forced = false;
	/* Set buffer ID */
	appData.pl360Tx.bufferId = 0;
	/* Set Transmission Mode. */
	appData.pl360Tx.mode = TX_MODE_RELATIVE;
	/* Value depends on TX time mode. Relative + TX_Time = 0 to set instantaneous transmission */
	appData.pl360Tx.time = APP_TX_DELAY_US;
	/* Set transmission power. It represents 3dBs of Attenuation signal per Unit. 0 value for maximum signal level. */
	appData.pl360Tx.attenuation = 0;
    /* Set data pointer */
    appData.pl360Tx.pTransmitData = appData.pDataTx;
    
}

static void _setupTransmissionData (void)
{
    uint16_t dataLength;

    /* Set data content */
	dataLength = sprintf((char *)appData.pDataTx, "PRIME PLC Message num:%u\r\n", (unsigned int)appData.counterTx);
    appData.pl360Tx.dataLength = dataLength;
    
}

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_IDLE;
    appData.mode = APP_MODE;
    
    /* Init Timer handler */
    appData.tmr1Handle = SYS_TIME_HANDLE_INVALID;
    
    /* Reset PLC exceptions statistics */
    appData.plc_phy_err_unexpected = 0;
    appData.plc_phy_err_critical = 0;
    appData.plc_phy_err_reset = 0;
    appData.plc_phy_err_unknow = 0;
    
    /* Init PLC statistics */
    appData.counterTx = 0;
    appData.counterRx = 0;
    appData.counterTxCfm = 0;
    
    /* Update state machine */
    appData.state = APP_STATE_INIT;
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            /* Open PLC driver */
            appData.drvPl360Handle = DRV_PLC_PHY_Open(DRV_PLC_PHY_INDEX_0, NULL);
            
            if (appData.drvPl360Handle != DRV_HANDLE_INVALID)
            {
                appData.state = APP_STATE_DEV_OPEN;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }
            break;
        }
            
        case APP_STATE_DEV_OPEN:
        {
            /* Check PLC transceiver */
            if (DRV_PLC_PHY_Status(DRV_PLC_PHY_INDEX_0) == SYS_STATUS_READY)
            {
                /* Configure PLC callbacks */
                DRV_PLC_PHY_ExceptionCallbackRegister(appData.drvPl360Handle, _pl360ExceptionCb, DRV_PLC_PHY_INDEX_0);

                if (appData.mode == APP_MODE_TRANSMISION)
                {
                    SYS_DEBUG_Print("PLC transceiver ready to transmit data...\r\n");
                    DRV_PLC_PHY_DataCfmCallbackRegister(appData.drvPl360Handle, _pl360DataCfmCb, DRV_PLC_PHY_INDEX_0);
                    appData.state = APP_STATE_TX_SETUP;
                    appData.pl360Tx.pTransmitData = appData.pDataTx;
                }
                else
                {
                    SYS_DEBUG_Print("PLC transceiver ready to receive data...\r\n");
                    DRV_PLC_PHY_DataIndCallbackRegister(appData.drvPl360Handle, _pl360DataIndCb, DRV_PLC_PHY_INDEX_0);
                    appData.state = APP_STATE_WAITING_RX;
                }

            }                
            break;
        }

        case APP_STATE_TX_SETUP:
        {
            _setupTransmissionParameters();
            appData.state = APP_STATE_TX;
            break;
        }

        case APP_STATE_TX:
        {
            /* update application counters */
            appData.counterTx++;
            /* update data message */
            _setupTransmissionData();
            
            SYS_DEBUG_Print("\r\n-> Sending new PLC msg(%u)...size:%u bytes\r\n", appData.counterTx, appData.pl360Tx.dataLength);
            
            DRV_PLC_PHY_Send(appData.drvPl360Handle, &appData.pl360Tx);
            appData.state = APP_STATE_WAITING_TX_CFM;
            break;
        }

        case APP_STATE_WAITING_TX_CFM:
        {
            /* Waiting */
            break;
        }

        case APP_STATE_RX:
        {
            appData.state = APP_STATE_WAITING_RX;
            break;
        }

        case APP_STATE_WAITING_RX:
        {
            /* Waiting */
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
