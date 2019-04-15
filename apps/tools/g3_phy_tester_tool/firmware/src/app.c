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

static void APP_PLCExceptionCb(DRV_PL360_EXCEPTION exceptionObj, 
        uintptr_t context)
{
    /* Avoid warning */
    (void)context;
    
    switch (exceptionObj) 
    {
        case DRV_PL360_EXCEPTION_UNEXPECTED_KEY:
            appData.pl360_err_unexpected++;
            break;

        case DRV_PL360_EXCEPTION_CRITICAL_ERROR:
            appData.pl360_err_critical++;
            break;

        case DRV_PL360_EXCEPTION_RESET:
            appData.pl360_err_reset++;
            break;

        default:
            appData.pl360_err_unknow++;
	}

	appData.pl360_exception = true;
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
        
        /* Add command */
        appData.pSerialData[0] = APP_CMD_PHY_RECEIVE_MSG;
        /* Add received message */
        length = SRV_PSERIAL_SerialRxMessage(&appData.pSerialData[1], indObj);
        /* Send through USI */
        SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_PHY, 
                appData.pSerialData, length + 1);
    }
}

static void APP_PLCDataCfmCb(DRV_PL360_TRANSMISSION_CFM_OBJ *cfmObj, uintptr_t context)
{   
    size_t length;
    
    /* Avoid warning */
    (void)context;
    
    /* Send Received PLC message through USI */
    /* Add command */
    appData.pSerialData[0] = APP_CMD_PHY_SEND_MSG_RSP;
    /* Add received message */
    length = SRV_PSERIAL_SerialCfmMessage(&appData.pSerialData[1], cfmObj);
    /* Send through USI */
    SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_PHY, 
            appData.pSerialData, length + 1);

}

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
void APP_USIPhyProtocolEventHandler(uint8_t *pData, size_t length)
{
    /* Message received from PLC Tool - USART */
    uint8_t *pDataMsg;
	uint8_t command;

	/* Protection for invalid us_length */
	if (!length) 
    {
		return;
	}

	/* Process received message */
    pDataMsg = pData;
	command = *pDataMsg++;

	switch (command) {
        case APP_CMD_PHY_GET_CFG:
        {
            /* Extract PIB information */
            SRV_PSERIAL_ParseGetPIB(&appData.plcPIB, pDataMsg);

            if (DRV_PL360_PIBGet(appData.drvPl360Handle, &appData.plcPIB))
            {
                size_t length;
                
                /* Add command */
                appData.pSerialData[0] = APP_CMD_PHY_GET_CFG_RSP;
                /* Serialize PIB data */
                length = SRV_PSERIAL_SerialGetPIB(&appData.pSerialData[1], &appData.plcPIB);
                /* Send through USI */
                SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_PHY, 
                        appData.pSerialData, length + 1);
            }
        }
        break;

        case APP_CMD_PHY_SET_CFG:
        {            
            /* Extract PIB information */
            SRV_PSERIAL_ParseSetPIB(&appData.plcPIB, pDataMsg);

            if (DRV_PL360_PIBSet(appData.drvPl360Handle, &appData.plcPIB))
            {
                size_t length;
                
                /* Add command */
                appData.pSerialData[0] = APP_CMD_PHY_SET_CFG_RSP;
                /* Serialize PIB data */
                length = SRV_PSERIAL_SerialSetPIB(&appData.pSerialData[1], &appData.plcPIB);
                /* Send through USI */
                SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_PHY, 
                        appData.pSerialData, length + 1);
            }
        }
        break;

        case APP_CMD_PHY_SEND_MSG:
        {
            /* Capture and parse data from USI */
            SRV_PSERIAL_ParseTxMessage(&appData.plcTxObj, pDataMsg);
            
            /* Send Message through PLC */
            DRV_PL360_Send(appData.drvPl360Handle, &appData.plcTxObj);
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
    
    /* Reset PL360 exceptions statistics */
    appData.pl360_err_unexpected = 0;
    appData.pl360_err_critical = 0;
    appData.pl360_err_reset = 0;
    appData.pl360_err_unknow = 0;
    
    appData.state = APP_STATE_INIT;
    
    /* Initialize PLC objects */
    appData.plcTxObj.pTransmitData = appData.pPLCDataTx;
    appData.plcRxObj.pReceivedData = appData.pPLCDataRx;
    appData.plcPIB.pData = appData.pPLCDataPIB;
    
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
                DRV_PL360_ExceptionCallbackRegister(appData.drvPl360Handle, 
                        APP_PLCExceptionCb, DRV_PL360_INDEX);
                DRV_PL360_DataIndCallbackRegister(appData.drvPl360Handle, 
                        APP_PLCDataIndCb, DRV_PL360_INDEX);
                DRV_PL360_DataCfmCallbackRegister(appData.drvPl360Handle, 
                        APP_PLCDataCfmCb, DRV_PL360_INDEX);
                
                /* Open USI Service */
                appData.srvUSIHandle = SRV_USI_Open(SRV_USI_INDEX_0);

                if (appData.srvUSIHandle != DRV_HANDLE_INVALID)
                {
                    /* Register USI callback */
                    SRV_USI_CallbackRegister(appData.srvUSIHandle, 
                            SRV_USI_PROT_ID_PHY, APP_USIPhyProtocolEventHandler);

                    /* Register Timer Callback */
                    appData.tmr1Handle = SYS_TIME_CallbackRegisterMS(
                            Timer1_Callback, 0, LED_BLINK_RATE_MS, 
                            SYS_TIME_PERIODIC);

                    /* Enable Led */
                    LED_On();

                    /* Set Application to next state */
                    appData.state = APP_STATE_READY;
                }
                else
                {
                    /* Set Application to ERROR state */
                    appData.state = APP_STATE_ERROR;
                }
            }                
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
