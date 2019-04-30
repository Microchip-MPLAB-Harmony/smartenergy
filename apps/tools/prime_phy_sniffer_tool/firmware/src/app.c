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
 ******************************************************************************/

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

// *****************************************************************************
/* Coupling parameters for each channel

  Summary:
    Holds PLC configuration data

  Description:
    These structures hold the PLC coupling configuration data.
    - plcDaccConfiguration1: DACC Configuration for channel 1.
    - plcDaccConfiguration2_8: DACC Configuration for channel 2 to 8.

  Remarks: 
    Parameters are defined in user.h file
    The following values are valid for MCHP EKs.
    For other hardware designs, calibration values should be checked.
 */ 
static const uint32_t plcDaccConfiguration1[17] = DACC_CFG_TABLE_CH1;
static const uint32_t plcDaccConfiguration2_8[17] = DACC_CFG_TABLE_CH2_8;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void Timer1_Callback(uintptr_t context)
{
    /* Avoid warning */
    (void)context;
    
    /* Status Led Toogle */
    LED_Toggle();
}

void Timer2_Callback(uintptr_t context)
{
    /* Avoid warning */
    (void)context;
    
    /* RX Led Signalling */
    LED_On();
}

static void APP_PLC360SetConfiguration(void)
{
    uint32_t* pDaccTbl;
    uint8_t index;
    
    index = appData.plcChannel - 1;
    if (index > 7)
    {
        /* Error in PLC Channel */
        return;
    }
    
    if (index >= 1) {
		/* Channel 2 - 8 */
		pDaccTbl = (uint32_t*)plcDaccConfiguration2_8;
	} else {
		/* Channel 1 */
		pDaccTbl = (uint32_t*)plcDaccConfiguration1;
	}
    
    appData.plcPIB.id = PL360_ID_CHANNEL_CFG;
    appData.plcPIB.length = 1;
    *appData.plcPIB.pData = appData.plcChannel;
    DRV_PL360_PIBSet(appData.drvPl360Handle, &appData.plcPIB);
    
    appData.plcPIB.id = PL360_ID_DACC_TABLE_CFG;
    appData.plcPIB.length = 17 << 2;
    memcpy(appData.plcPIB.pData, (uint8_t *)pDaccTbl, appData.plcPIB.length);
    DRV_PL360_PIBSet(appData.drvPl360Handle, &appData.plcPIB);
    
}

static void APP_PLCDataIndCb(DRV_PL360_RECEPTION_OBJ *indObj, uintptr_t context)
{   
    /* Avoid warning */
    (void)context;
    
    /* Send Received PLC message through USI */
	if (indObj->dataLength) {
        size_t length;
        
        /* Start Timer: LED blinking for each received message */
        LED_Off();
        appData.tmr2Handle = SYS_TIME_CallbackRegisterMS(Timer2_Callback, 0, 
                LED_BLINK_PLC_MSG_MS, SYS_TIME_SINGLE);
        
        /* Report RX Symbols */
        appData.plcPIB.id = PL360_ID_RX_PAY_SYMBOLS;
        appData.plcPIB.length = 2;        
        DRV_PL360_PIBGet(appData.drvPl360Handle, &appData.plcPIB);
        
        SRV_PSNIFFER_SetRxPayloadSymbols(*(uint16_t *)appData.plcPIB.pData);
        
        /* Add received message */
        length = SRV_PSNIFFER_SerialRxMessage(appData.pSerialData, indObj);
        /* Send through USI */
        SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_SNIF_PRIME, 
                appData.pSerialData, length);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
void APP_USIPhyProtocolEventHandler(uint8_t *pData, size_t length)
{
    /* Message received from PLC Tool - USART */
	uint8_t command;

	/* Protection for invalid us_length */
	if (!length) 
    {
		return;
	}

	/* Process received message */
	command = SRV_PSNIFFER_GetCommand(pData);

	switch (command) {
        case SRV_PSNIFFER_CMD_SET_CHANNEL:
        {
            uint8_t channel;
            
            channel = *(pData + 1);
            if ((channel > 0) && (channel < 9))
            {
                /* Set channel for Sniffer */
                SRV_PSNIFFER_SetPLCChannel(channel);
                
                /* Modify Channel in Application data */
                appData.plcChannel = channel;
                
                /* Set Application to PL360 configuration state */
                appData.state = APP_STATE_CONFIG_PL360;
            }            
        }
        break;

        default:
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
    void APP_Initialize(void)

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize(void)
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_IDLE;
    
    /* Initialize Timer handler */
    appData.tmr1Handle = SYS_TIME_HANDLE_INVALID;
    appData.tmr2Handle = SYS_TIME_HANDLE_INVALID;
    
    appData.state = APP_STATE_INIT;
    
    /* Initialize PLC objects */
    appData.plcRxObj.pReceivedData = appData.pPLCDataRx;
    appData.plcPIB.pData = appData.pPLCDataPIB;
    
    /* Set initial PLC channel */
    appData.plcChannel = 1;
    
}


/******************************************************************************
  Function:
    void APP_Tasks(void)

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks(void)
{
    /* Check the application's current state. */
    switch(appData.state)
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            /* Open PL360 driver : Start uploading process */
            appData.drvPl360Handle = DRV_PL360_Open(DRV_PL360_INDEX, 
                    DRV_IO_INTENT_READWRITE);

            if (appData.drvPl360Handle != DRV_HANDLE_INVALID)
            {
                /* Set Application to next state */
                appData.state = APP_STATE_REGISTER;
            } 
            else 
            {
                /* Set Application to ERROR state */
                appData.state = APP_STATE_ERROR;
            }
            break;
        }
            
        /* Waiting to PL360 device be opened and register callback functions */
        case APP_STATE_REGISTER:
        {
            /* Check PL360 device */
            if (DRV_PL360_Status(DRV_PL360_INDEX) == SYS_STATUS_READY)
            {
                /* Register PL360 callback */
                DRV_PL360_DataIndCallbackRegister(appData.drvPl360Handle, 
                        APP_PLCDataIndCb, DRV_PL360_INDEX);
                
                /* Open USI Service */
                appData.srvUSIHandle = SRV_USI_Open(SRV_USI_INDEX_0);

                if (appData.srvUSIHandle != DRV_HANDLE_INVALID)
                {
                    /* Register USI callback */
                    SRV_USI_CallbackRegister(appData.srvUSIHandle, 
                            SRV_USI_PROT_ID_SNIF_PRIME, APP_USIPhyProtocolEventHandler);

                    /* Register Timer Callback */
                    appData.tmr1Handle = SYS_TIME_CallbackRegisterMS(
                            Timer1_Callback, 0, LED_BLINK_RATE_MS, 
                            SYS_TIME_PERIODIC);

                    /* Enable Led */
                    LED_On();
                    
                    /* Set default channel */
                    SRV_PSNIFFER_SetPLCChannel(appData.plcChannel);

                    /* Set Application to next state */
                    appData.state = APP_STATE_CONFIG_PL360;
                }
                else
                {
                    /* Set Application to ERROR state */
                    appData.state = APP_STATE_ERROR;
                }
            }                
            break;
        }

        case APP_STATE_CONFIG_PL360:
        {
            /* Set configuration fro PL360 */
            APP_PLC360SetConfiguration();
            /* Set Application to next state */
            appData.state = APP_STATE_READY;
            break;
        }

        case APP_STATE_READY:
        {
            /* waiting commands from Microchip PHY tester tool application */
            break;
        }

        case APP_STATE_ERROR:
        {
            /* Handle error in application's state machine */
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
