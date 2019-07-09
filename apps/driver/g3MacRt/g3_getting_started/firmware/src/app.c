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

static bool _rxCheckCrc(DRV_G3_MACRT_RECEPTION_OBJ *indObj);

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

static void _appMacRtExceptionCb(DRV_G3_MACRT_EXCEPTION excObj, uintptr_t context )
{
    switch (excObj) 
    {
        case DRV_G3_MACRT_EXCEPTION_UNEXPECTED_KEY:
            appData.g3MacRtErrorUnexpected++;
            SYS_DEBUG_Print("Exception: DRV_G3_MACRT_EXCEPTION_UNEXPECTED_KEY [%u]\r\n", 
                    appData.g3MacRtErrorUnexpected);
            break;

        case DRV_G3_MACRT_EXCEPTION_CRITICAL_ERROR:
            appData.g3MacRtErrorCritical++;
            SYS_DEBUG_Print("Exception: DRV_G3_MACRT_EXCEPTION_CRITICAL_ERROR [%u]\r\n", 
                    appData.g3MacRtErrorReset);
            break;

        case DRV_G3_MACRT_EXCEPTION_RESET:
            appData.g3MacRtErrorReset++;
            SYS_DEBUG_Print("Exception: DRV_G3_MACRT_EXCEPTION_RESET [%u]\r\n", 
                    appData.g3MacRtErrorReset);
            break;

        default:
            appData.g3MacRtErrorUnknow++;
            SYS_DEBUG_Print("Exception: UNKNOWN [%u]\r\n", 
                    appData.g3MacRtErrorUnknow);
	}

	appData.g3MacRtException = true;
}

static void _appMacRtTxCfmCb(MAC_RT_TX_CFM_OBJ *cfmObj, uintptr_t context )
{
    SYS_DEBUG_Print("_appMacRtTxCfmCb");
    
    /* Handle result of transmission : Show it through Console */
    switch(cfmObj->status)
    {
        case MAC_RT_STATUS_SUCCESS:
            SYS_DEBUG_Print("...MAC_RT_STATUS_SUCCESS\r\n");
            break;   
        case MAC_RT_STATUS_CHANNEL_ACCESS_FAILURE:
            SYS_DEBUG_Print("...MAC_RT_STATUS_CHANNEL_ACCESS_FAILURE\r\n");
            break;   
        case MAC_RT_STATUS_NO_ACK:
            SYS_DEBUG_Print("...MAC_RT_STATUS_NO_ACK\r\n");
            break;
        default:
            SYS_DEBUG_Print("...UNKOWN STATUS\r\n");
    }    
}

static void _appMacRtDataIndCb(uint8_t *pData, uint16_t length, uintptr_t context )
{   
    /* Show Received G3 MAC RT Data message */
	if (length) {

	}
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/
#define DRV_G3_MACRT_INDEX_0   0

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
static void _appG3MacRtSetupTxParameters (void)
{
    /* Configure Tone Map, it depends on PLC profile */
#if (DRV_G3_MACRT_PLC_PROFILE == G3_CEN_A)
	appData.pl360Tx.toneMap[0] = 0x3F;
#elif (DRV_G3_MACRT_PLC_PROFILE == G3_CEN_B)
	appData.pl360Tx.toneMap[0] = 0x0F;
#elif (DRV_G3_MACRT_PLC_PROFILE == G3_FCC)
	appData.pl360Tx.toneMap[0] = 0xFF;
    appData.pl360Tx.toneMap[1] = 0xFF;
    appData.pl360Tx.toneMap[2] = 0xFF;
#else
#error ERROR in PHY band definition
#endif    
    
    /* Set pre-emphasis filter */
	memset(appData.pl360Tx.preemphasis, 0, sizeof(appData.pl360Tx.preemphasis));
	/* Use only 1 RS block */
	appData.pl360Tx.rs2Blocks = 0;
	/* ACK management is not needed. */
	appData.pl360Tx.delimiterType = DT_SOF_NO_RESP;
	/* Set modulation scheme. */
	appData.pl360Tx.modScheme = MOD_SCHEME_DIFFERENTIAL;
	/* Set modulation type. */
	appData.pl360Tx.modType = MOD_TYPE_BPSK;
	/* Set phase detector counter */
	appData.pl360Tx.pdc = 0;
	/* Set Transmission Mode. */
	appData.pl360Tx.mode = TX_MODE_FORCED | TX_MODE_RELATIVE;
	/* Value depends on TX time mode. Relative + TX_Time = 0 to set instantaneous transmission */
	appData.pl360Tx.time = APP_TX_DELAY_US;
	/* Set transmission power. It represents 3dBs of Attenuation signal per Unit. 0 value for maximum signal level. */
	appData.pl360Tx.attenuation = 0;
    
}

static void _setupTransmissionData (void)
{
    uint16_t dataLength;

    /* Set data content */
	dataLength = sprintf((char *)appData.pDataTx, "G3 PLC Message num:%u\r\n", (unsigned int)appData.counterTx);
    appData.pl360Tx.dataLength = dataLength;
    
    /* Insert padding and CRC */
	_txInsertPaddingCcrc();
    
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
    
    /* Init Timer handler */
    appData.tmr1Handle = SYS_TIME_HANDLE_INVALID;
    
    /* Reset G3 MACRT exceptions statistics */
    appData.g3MacRtErrorUnexpected = 0;
    appData.g3MacRtErrorCritical = 0;
    appData.g3MacRtErrorReset = 0;
    appData.g3MacRtErrorUnknow = 0;
    appData.g3MacRtException = false;
    
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
            /* Open G3 MACRT driver */
            appData.g3MacRtHandle = DRV_G3_MACRT_Open(DRV_G3_MACRT_INDEX_0, NULL);
            
            if (appData.g3MacRtHandle != DRV_HANDLE_INVALID)
            {
                appData.state = APP_STATE_DEV_OPEN;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }
            break;
            
        case APP_STATE_DEV_OPEN:
            /* Check G3 MACRT device */
            if (DRV_G3_MACRT_Status(DRV_G3_MACRT_INDEX_0) == SYS_STATUS_READY)
            {
                /* Configure G3 MACRT callbacks */
                DRV_G3_MACRT_ExceptionCallbackRegister(appData.g3MacRtHandle, 
                        _appMacRtExceptionCb, (const uintptr_t)&appData);

                DRV_G3_MACRT_TxCfmCallbackRegister(appData.g3MacRtHandle, 
                        _pl360DataCfmCb, (const uintptr_t)&appData);

                DRV_G3_MACRT_DataIndCallbackRegister(appData.g3MacRtHandle, 
                        _appMacRtDataIndCb, (const uintptr_t)&appData);

            }                
            break;

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
            
            DRV_G3_MACRT_Send(appData.g3MacRtHandle, &appData.pl360Tx);
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
