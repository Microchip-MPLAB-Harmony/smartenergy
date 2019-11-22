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
/* PLC Coupling configuration data

  Summary:
    Holds PLC configuration data

  Description:
    This structure holds the PLC coupling configuration data.

  Remarks:
    Parameters are defined in user.h file
 */

APP_PLC_COUPLING_DATA appPLCCoupConfig = {MAX_RMS_HI_TABLE, MAX_RMS_VLO_TABLE,
                   THRESHOLD_HI_TABLE, THRESHOLD_VLO_TABLE,
				   DACC_CFG_TABLE,
				   PREDIST_HI_TABLE, PREDIST_VLO_TABLE,
				   GAIN_HI, GAIN_VLO,
				   NUM_TX_LEVELS};

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
    /* Disable AUTO mode and set VLO behavior by default in order to
     * maximize signal level in any case */
    appData.plcPIB.id = PLC_ID_CFG_AUTODETECT_IMPEDANCE;
    appData.plcPIB.length = 1;
    *appData.plcPIB.pData = 0;
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

    appData.plcPIB.id = PLC_ID_CFG_IMPEDANCE;
    appData.plcPIB.length = 1;
    *appData.plcPIB.pData = 2;
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

    if (appData.plcConfigureCoupling)
    {
        appData.plcPIB.id = PLC_ID_NUM_TX_LEVELS;
        appData.plcPIB.length = 1;
        *appData.plcPIB.pData = appPLCCoupConfig.numTxLevels;
        DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

        appData.plcPIB.id = PLC_ID_MAX_RMS_TABLE_HI;
        appData.plcPIB.length = sizeof(appPLCCoupConfig.maxRMSHigh);
        memcpy(appData.plcPIB.pData, (uint8_t *)appPLCCoupConfig.maxRMSHigh,
                appData.plcPIB.length);
        DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

        appData.plcPIB.id = PLC_ID_MAX_RMS_TABLE_VLO;
        appData.plcPIB.length = sizeof(appPLCCoupConfig.maxRMSVeryLow);
        memcpy(appData.plcPIB.pData, (uint8_t *)appPLCCoupConfig.maxRMSVeryLow,
                appData.plcPIB.length);
        DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

        appData.plcPIB.id = PLC_ID_THRESHOLDS_TABLE_HI;
        appData.plcPIB.length = sizeof(appPLCCoupConfig.thresholdHigh);
        memcpy(appData.plcPIB.pData, (uint8_t *)appPLCCoupConfig.thresholdHigh,
                appData.plcPIB.length);
        DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

        appData.plcPIB.id = PLC_ID_THRESHOLDS_TABLE_VLO;
        appData.plcPIB.length = sizeof(appPLCCoupConfig.thresholdVeryLow);
        memcpy(appData.plcPIB.pData, (uint8_t *)appPLCCoupConfig.thresholdVeryLow,
                appData.plcPIB.length);
        DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

        appData.plcPIB.id = PLC_ID_GAIN_TABLE_HI;
        appData.plcPIB.length = sizeof(appPLCCoupConfig.gainHigh);
        memcpy(appData.plcPIB.pData, (uint8_t *)appPLCCoupConfig.gainHigh,
                appData.plcPIB.length);
        DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

        appData.plcPIB.id = PLC_ID_GAIN_TABLE_VLO;
        appData.plcPIB.length = sizeof(appPLCCoupConfig.gainVeryLow);
        memcpy(appData.plcPIB.pData, (uint8_t *)appPLCCoupConfig.gainVeryLow,
                appData.plcPIB.length);
        DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

        appData.plcPIB.id = PLC_ID_DACC_TABLE_CFG;
        appData.plcPIB.length = sizeof(appPLCCoupConfig.daccConfig);
        memcpy(appData.plcPIB.pData, (uint8_t *)appPLCCoupConfig.daccConfig,
                appData.plcPIB.length);
        DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

        appData.plcPIB.id = PLC_ID_PREDIST_COEF_TABLE_HI;
        appData.plcPIB.length = sizeof(appPLCCoupConfig.predistorsionHigh);
        memcpy(appData.plcPIB.pData, (uint8_t *)appPLCCoupConfig.predistorsionHigh,
                appData.plcPIB.length);
        DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

        appData.plcPIB.id = PLC_ID_PREDIST_COEF_TABLE_VLO;
        /* Not use size of array. It depends on PHY band in use */
        appData.plcPIB.length = sizeof(appPLCCoupConfig.predistorsionVeryLow);
        memcpy(appData.plcPIB.pData, (uint8_t *)appPLCCoupConfig.predistorsionVeryLow,
                appData.plcPIB.length);
        DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

    }
}

static void APP_PLCExceptionCb(DRV_PLC_PHY_EXCEPTION exceptionObj,
        uintptr_t context)
{
    /* Avoid warning */
    (void)context;

    switch (exceptionObj)
    {
        case DRV_PLC_PHY_EXCEPTION_UNEXPECTED_KEY:
            appData.plc_phy_err_unexpected++;
            break;

        case DRV_PLC_PHY_EXCEPTION_CRITICAL_ERROR:
            appData.plc_phy_err_critical++;
            break;

        case DRV_PLC_PHY_EXCEPTION_RESET:
            appData.plc_phy_err_reset++;
            break;

        default:
            appData.plc_phy_err_unknow++;
	}

	appData.plc_phy_exception = true;
}

static void APP_PLCDataIndCb(DRV_PLC_PHY_RECEPTION_OBJ *indObj, uintptr_t context)
{
    /* Avoid warning */
    (void)context;

    /* Send Received PLC message through USI */
    if (indObj->dataLength)
    {
        size_t length;

        /* Start Timer: LED blinking for each received message */
        LED_Off();
        appData.tmr2Handle = SYS_TIME_CallbackRegisterMS(Timer2_Callback, 0,
                LED_BLINK_PLC_MSG_MS, SYS_TIME_SINGLE);

        /* Add received message */
        length = SRV_PSERIAL_SerialRxMessage(appData.pSerialData, indObj);
        /* Send through USI */
        SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_PHY,
                appData.pSerialData, length);
    }
}

static void APP_PLCDataCfmCb(DRV_PLC_PHY_TRANSMISSION_CFM_OBJ *cfmObj, uintptr_t context)
{
    size_t length;

    /* Avoid warning */
    (void)context;

    /* Add received message */
    length = SRV_PSERIAL_SerialCfmMessage(appData.pSerialData, cfmObj);
    /* Send through USI */
    SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_PHY,
            appData.pSerialData, length);

}

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
void APP_USIPhyProtocolEventHandler(uint8_t *pData, size_t length)
{
    /* Message received from PLC Tool - USART */
    SRV_PSERIAL_COMMAND command;

    /* Protection for invalid us_length */
    if (!length)
    {
        return;
    }

    /* Process received message */
    command = SRV_PSERIAL_GetCommand(pData);

    switch (command) {
        case SRV_PSERIAL_CMD_PHY_GET_CFG:
        {
            /* Extract PIB information */
            SRV_PSERIAL_ParseGetPIB(&appData.plcPIB, pData);

            if (DRV_PLC_PHY_PIBGet(appData.drvPl360Handle, &appData.plcPIB))
            {
                size_t len;

                /* Serialize PIB data */
                len = SRV_PSERIAL_SerialGetPIB(appData.pSerialData, &appData.plcPIB);
                /* Send through USI */
                SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_PHY,
                        appData.pSerialData, len);
            }
        }
        break;

        case SRV_PSERIAL_CMD_PHY_SET_CFG:
        {
            /* Extract PIB information */
            SRV_PSERIAL_ParseSetPIB(&appData.plcPIB, pData);

            if (DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB))
            {
                size_t len;

                /* Serialize PIB data */
                len = SRV_PSERIAL_SerialSetPIB(&appData.pSerialData[1], &appData.plcPIB);
                /* Send through USI */
                SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_PHY,
                        appData.pSerialData, len);
            }
        }
        break;

        case SRV_PSERIAL_CMD_PHY_SEND_MSG:
        {
            /* Capture and parse data from USI */
            SRV_PSERIAL_ParseTxMessage(&appData.plcTxObj, pData);

            /* Send Message through PLC */
            DRV_PLC_PHY_Send(appData.drvPl360Handle, &appData.plcTxObj);
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

    /* Reset PLC exceptions statistics */
    appData.plc_phy_err_unexpected = 0;
    appData.plc_phy_err_critical = 0;
    appData.plc_phy_err_reset = 0;
    appData.plc_phy_err_unknow = 0;

    appData.state = APP_STATE_INIT;

    /* Initialize PLC objects */
    appData.plcTxObj.pTransmitData = appData.pPLCDataTx;
    appData.plcRxObj.pReceivedData = appData.pPLCDataRx;
    appData.plcPIB.pData = appData.pPLCDataPIB;

    /* In case to needs your own calibration, set to True.
       Configured by user.h file */
    appData.plcConfigureCoupling = APP_CONFIG_PLC_COUP;

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
            /* Open PLC driver : Start uploading process */
            appData.drvPl360Handle = DRV_PLC_PHY_Open(DRV_PLC_PHY_INDEX, NULL);

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

        /* Waiting to PLC transceiver be opened and register callback functions */
        case APP_STATE_REGISTER:
        {
            /* Check PLC transceiver */
            if (DRV_PLC_PHY_Status(DRV_PLC_PHY_INDEX) == SYS_STATUS_READY)
            {
                /* Register PLC callback */
                DRV_PLC_PHY_ExceptionCallbackRegister(appData.drvPl360Handle,
                        APP_PLCExceptionCb, DRV_PLC_PHY_INDEX);
                DRV_PLC_PHY_DataIndCallbackRegister(appData.drvPl360Handle,
                        APP_PLCDataIndCb, DRV_PLC_PHY_INDEX);
                DRV_PLC_PHY_DataCfmCallbackRegister(appData.drvPl360Handle,
                        APP_PLCDataCfmCb, DRV_PLC_PHY_INDEX);

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
                    appData.state = APP_STATE_CONFIG_PLC;
                }
                else
                {
                    /* Set Application to ERROR state */
                    appData.state = APP_STATE_ERROR;
                }
            }
            break;
        }

        case APP_STATE_CONFIG_PLC:
        {
            /* Set configuration fro PLC */
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
