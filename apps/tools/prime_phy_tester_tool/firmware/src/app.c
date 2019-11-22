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
/* TX & Coupling parameters for each channel

  Summary:
    Holds PLC configuration data

  Description:
    These structures hold the PLC coupling configuration data.
    For each channel, there are 8 values, corresponding to first 8 TX 
    attenuation levels (1 dB steps).
    - plcMaxRMSHigh: Target RMS_CALC in High impedance mode value when dynamic 
      gain is enabled (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2).
    - plcMaxRMSVeryLow: Target RMS_CALC in Very low impedance mode value when 
      dynamic gain is enabled (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2).
    - plcThresholdsHigh: Thresholds to change impedance mode 
      (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1) from High impedance mode.
      For each channel there are 16 values. First 8 values (one per TX level) 
      are thresholds to change from High imp. to Low imp. (0 to disable). 
      Next 8 values (one per TX level) are thresholds to change from High to 
      VeryLow. When RMS_CALC is below threshold, impedance mode changes to VeryLow.
    - plcThresholdsVeryLow: Thresholds to change impedance mode 
      (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1) from Very Low impedance mode.
      For each channel there are 16 values. First 8 values (one per TX level) 
      are thresholds to change from High imp. to Low imp. (0 to disable). 
      Next 8 values (one per TX level) are thresholds to change from High to 
      VeryLow. When RMS_CALC is below threshold, impedance mode changes to VeryLow.
    - plcGainHighChn1: Gain values for HI and VLO modes for channel 1.
      There are three values: {GAIN_INI, GAIN_MIN, GAIN_MAX}.
    - plcGainHighChn2_8: Gain values for HI and VLO modes for channels 2 to 8.
      There are three values: {GAIN_INI, GAIN_MIN, GAIN_MAX}.
    - plcPredistorsionHigh1: Equalization values for High Impedance mode. Specific 
      gain for each carrier to equalize transmission and compensate HW filter 
      frequency response. [Channel 1]
    - plcPredistorsionHigh2_8: Equalization values for High Impedance mode. Specific 
      gain for each carrier to equalize transmission and compensate HW filter 
      frequency response. [Channel 2 to 8]
    - plcPredistorsionVeryLow1: Equalization values for Very Low Impedance mode. 
      Specific gain for each carrier to equalize transmission and compensate HW filter 
      frequency response. [Channel 1]
    - plcPredistorsionVeryLow2_8: Equalization values for Very Low Impedance mode. 
      Specific gain for each carrier to equalize transmission and compensate HW filter 
      frequency response. [Channel 2 to 8]
    - plcDaccConfiguration1: DACC Configuration for channel 1.
    - plcDaccConfiguration2_8: DACC Configuration for channel 2 to 8.

  Remarks: 
    Parameters are defined in user.h file
    The following values are valid for MCHP EKs.
    For other hardware designs, calibration values should be checked with 
    MCHP PHY Calibration Tool.
 */ 

static const uint32_t plcMaxRMSHigh[8][8] = {
	MAX_RMS_HI_TABLE_CH1,
	MAX_RMS_HI_TABLE_CH2,
	MAX_RMS_HI_TABLE_CH3,
	MAX_RMS_HI_TABLE_CH4,
	MAX_RMS_HI_TABLE_CH5,
	MAX_RMS_HI_TABLE_CH6,
	MAX_RMS_HI_TABLE_CH7,
	MAX_RMS_HI_TABLE_CH8
};

static const uint32_t plcMaxRMSVeryLow[8][8] = {
	MAX_RMS_VLO_TABLE_CH1,
	MAX_RMS_VLO_TABLE_CH2,
	MAX_RMS_VLO_TABLE_CH3,
	MAX_RMS_VLO_TABLE_CH4,
	MAX_RMS_VLO_TABLE_CH5,
	MAX_RMS_VLO_TABLE_CH6,
	MAX_RMS_VLO_TABLE_CH7,
	MAX_RMS_VLO_TABLE_CH8
};

static const uint32_t plcThresholdsHigh[8][16] = {
	THRESHOLD_HI_TABLE_CH1,
	THRESHOLD_HI_TABLE_CH2,
	THRESHOLD_HI_TABLE_CH3,
	THRESHOLD_HI_TABLE_CH4,
	THRESHOLD_HI_TABLE_CH5,
	THRESHOLD_HI_TABLE_CH6,
	THRESHOLD_HI_TABLE_CH7,
	THRESHOLD_HI_TABLE_CH8
};

static const uint32_t plcThresholdsVeryLow[8][16] = {
	THRESHOLD_VLO_TABLE_CH1,
	THRESHOLD_VLO_TABLE_CH2,
	THRESHOLD_VLO_TABLE_CH3,
	THRESHOLD_VLO_TABLE_CH4,
	THRESHOLD_VLO_TABLE_CH5,
	THRESHOLD_VLO_TABLE_CH6,
	THRESHOLD_VLO_TABLE_CH7,
	THRESHOLD_VLO_TABLE_CH8
};

static const uint16_t plcGainHighChn1[3] = GAIN_HI_CH1;
static const uint16_t plcGainHighChn2_8[3] = GAIN_HI_CH2_8;

static const uint16_t plcGainVeryLowChn1[3] = GAIN_VLO_CH1;
static const uint16_t plcGainVeryLowChn2_8[3] = GAIN_VLO_CH2_8;

static const uint16_t plcPredistorsionHigh1[97] = PREDIST_HI_TABLE_CH1;
static const uint16_t plcPredistorsionHigh2_8[97] = PREDIST_HI_TABLE_CH2_8;

static const uint16_t plcPredistorsionVeryLow1[97] = PREDIST_VLO_TABLE_CH1;
static const uint16_t plcPredistorsionVeryLow2_8[97] = PREDIST_VLO_TABLE_CH2_8;

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

static void APP_PLC360SetCouplingConfiguration(uint8_t plcChannel)
{  
    uint16_t *pEquHi, *pEquVlo, *pGainHi, *pGainVlo;
	uint32_t *pDaccTbl;
    uint8_t index;
    
    index = plcChannel - 1;
    if (index > 7)
    {
        /* Error in PLC Channel */
        return;
    }
    
    appData.plcPIB.id = PLC_ID_MAX_RMS_TABLE_HI;
    appData.plcPIB.length = 8 << 2;
    memcpy(appData.plcPIB.pData, (uint8_t *)plcMaxRMSHigh[index], 
            appData.plcPIB.length);
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

    appData.plcPIB.id = PLC_ID_MAX_RMS_TABLE_VLO;
    appData.plcPIB.length = 8 << 2;
    memcpy(appData.plcPIB.pData, (uint8_t *)plcMaxRMSVeryLow[index], 
            appData.plcPIB.length);
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

    appData.plcPIB.id = PLC_ID_THRESHOLDS_TABLE_HI;
    appData.plcPIB.length = 16 << 2;
    memcpy(appData.plcPIB.pData, (uint8_t *)plcThresholdsHigh[index],  
            appData.plcPIB.length);
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

    appData.plcPIB.id = PLC_ID_THRESHOLDS_TABLE_VLO;
    appData.plcPIB.length = 16 << 2;
    memcpy(appData.plcPIB.pData, (uint8_t *)plcThresholdsVeryLow[index],  
            appData.plcPIB.length);
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);
    
    if (index >= 1) {
		/* Channel 2 - 8 */
		pEquHi = (uint16_t *)plcPredistorsionHigh2_8;
		pEquVlo = (uint16_t *)plcPredistorsionVeryLow2_8;
		pGainHi = (uint16_t *)plcGainHighChn2_8;
		pGainVlo = (uint16_t *)plcGainVeryLowChn2_8;
		pDaccTbl = (uint32_t *)plcDaccConfiguration2_8;
	} else {
		/* Channel 1 */
		pEquHi = (uint16_t *)plcPredistorsionHigh1;
		pEquVlo = (uint16_t *)plcPredistorsionVeryLow1;
		pGainHi = (uint16_t *)plcGainHighChn1;
		pGainVlo = (uint16_t *)plcGainVeryLowChn1;
		pDaccTbl = (uint32_t *)plcDaccConfiguration1;
	}

    appData.plcPIB.id = PLC_ID_GAIN_TABLE_HI;
    appData.plcPIB.length = 3 << 1;
    memcpy(appData.plcPIB.pData, (uint8_t *)pGainHi, appData.plcPIB.length);
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);
    
    appData.plcPIB.id = PLC_ID_GAIN_TABLE_VLO;
    appData.plcPIB.length = 3 << 1;
    memcpy(appData.plcPIB.pData, (uint8_t *)pGainVlo, appData.plcPIB.length);
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);
    
    appData.plcPIB.id = PLC_ID_PREDIST_COEF_TABLE_HI;
    appData.plcPIB.length = 97 << 1;
    memcpy(appData.plcPIB.pData, (uint8_t *)pEquHi, appData.plcPIB.length);
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);
    
    appData.plcPIB.id = PLC_ID_PREDIST_COEF_TABLE_VLO;
    appData.plcPIB.length = 97 << 1;
    memcpy(appData.plcPIB.pData, (uint8_t *)pEquVlo, appData.plcPIB.length);
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);
    
    appData.plcPIB.id = PLC_ID_DACC_TABLE_CFG;
    appData.plcPIB.length = 17 << 2;
    memcpy(appData.plcPIB.pData, (uint8_t *)pDaccTbl, appData.plcPIB.length);
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);
    
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
    
    appData.plcPIB.id = PLC_ID_NUM_TX_LEVELS;
    appData.plcPIB.length = 1;
    *appData.plcPIB.pData = NUM_TX_LEVELS;
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);
    
    /* Set PLC Channel */
    appData.plcPIB.id = PLC_ID_CHANNEL_CFG;
    appData.plcPIB.length = 1;
    *appData.plcPIB.pData = appData.plcChannel;
    DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB);

    /* Adjust configuration of PLC coupling depending on PLC channel */
    APP_PLC360SetCouplingConfiguration(appData.plcChannel);
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
	if (indObj->dataLength) {
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
	uint8_t command;

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
                size_t length;
                
                /* Serialize PIB data */
                length = SRV_PSERIAL_SerialGetPIB(appData.pSerialData, &appData.plcPIB);
                /* Send through USI */
                SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_PHY, 
                        appData.pSerialData, length);
            }
        }
        break;

        case SRV_PSERIAL_CMD_PHY_SET_CFG:
        {            
            /* Extract PIB information */
            SRV_PSERIAL_ParseSetPIB(&appData.plcPIB, pData);

            if (DRV_PLC_PHY_PIBSet(appData.drvPl360Handle, &appData.plcPIB))
            {
                size_t length;
                
                /* Serialize PIB data */
                length = SRV_PSERIAL_SerialSetPIB(&appData.pSerialData[1], &appData.plcPIB);
                /* Send through USI */
                SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_PHY, 
                        appData.pSerialData, length);
            }
            
            /* Check if PLC channel has been modified by setting request */
            if (appData.plcPIB.id == PLC_ID_CHANNEL_CFG)
            {
                /* Adjust configuration of PLC coupling depending on PLC channel */
                appData.plcChannel = *appData.plcPIB.pData;
                APP_PLC360SetCouplingConfiguration(appData.plcChannel);
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

    /* PLC Channel 1 is used by default in PRIME binary file */
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
