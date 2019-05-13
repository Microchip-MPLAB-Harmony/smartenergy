/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_plc.c

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
#define APP_PLC_CONFIG_KEY  0xA55A

extern uint8_t pl360_bin_start;
extern uint8_t pl360_bin_end;

/* PL360 Driver Initialization Data (initialization.c) */
extern DRV_PL360_INIT drvPL360InitData;

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

APP_PLC_DATA CACHE_ALIGN appPlc;
APP_PLC_DATA_TX CACHE_ALIGN appPlcTx;

// *****************************************************************************
/* TX & Coupling parameters for each channel

  Summary:
    Holds PLC configuration data

  Description:
    These structures hold the PLC coupling configuration data.
    For each channel, there are 8 values, corresponding to first 8 TX 
    attenuation levels (1 dB steps).
    - plcMaxRMSHigh: Target RMS_CALC in High impedance mode value when dynamic 
      gain is enabled (PL360_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2).
    - plcMaxRMSVeryLow: Target RMS_CALC in Very low impedance mode value when 
      dynamic gain is enabled (PL360_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2).
    - plcThresholdsHigh: Thresholds to change impedance mode 
      (PL360_ID_CFG_AUTODETECT_IMPEDANCE = 1) from High impedance mode.
      For each channel there are 16 values. First 8 values (one per TX level) 
      are thresholds to change from High imp. to Low imp. (0 to disable). 
      Next 8 values (one per TX level) are thresholds to change from High to 
      VeryLow. When RMS_CALC is below threshold, impedance mode changes to VeryLow.
    - plcThresholdsVeryLow: Thresholds to change impedance mode 
      (PL360_ID_CFG_AUTODETECT_IMPEDANCE = 1) from Very Low impedance mode.
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

static void APP_PLC_ExceptionCb(DRV_PL360_EXCEPTION exceptionObj, uintptr_t context )
{
    /* Avoid warning */
    (void)context;
    
    /* Clear App flag */
    appPlc.waitingTxCfm = false;
    /* Restore TX configuration */
    appPlc.state = APP_PLC_STATE_READ_CONFIG;
}

static void APP_PLC_DataCfmCb(DRV_PL360_TRANSMISSION_CFM_OBJ *cfmObj, uintptr_t context )
{
    /* Avoid warning */
    (void)context;
    
    /* Update App flags */
    appPlc.waitingTxCfm = false;
    
    /* Handle result of transmission : Show it through Console */
    switch(cfmObj->result)
    {
        case DRV_PL360_TX_RESULT_PROCESS:
            printf("...DRV_PL360_TX_RESULT_PROCESS\r\n");
            break;   
        case DRV_PL360_TX_RESULT_SUCCESS:
            printf("...DRV_PL360_TX_RESULT_SUCCESS\r\n");
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
        case DRV_PL360_TX_RESULT_INV_BUFFER:
            printf("...DRV_PL360_TX_RESULT_INV_BUFFER\r\n");
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
static void APP_PLC_ApplyPlcConfiguration ( void )
{
    DRV_PL360_PIB_OBJ pibObj;   
    uint16_t *pEquHi, *pEquVlo, *pGainHi, *pGainVlo;
	uint32_t *pDaccTbl;
    uint8_t index;
    
    index = appPlcTx.channel - 1;
    if (index > 7)
    {
        /* Error in PLC Channel */
        return;
    }
    
    pibObj.id = PL360_ID_NUM_TX_LEVELS;
    pibObj.length = 1;
    pibObj.pData = &appPlc.couplingLevels;
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);
    
    pibObj.id = PL360_ID_MAX_RMS_TABLE_HI;
    pibObj.length = 8 << 2;
    pibObj.pData = (uint8_t *)&plcMaxRMSHigh[index];
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PL360_ID_MAX_RMS_TABLE_VLO;
    pibObj.length = 8 << 2;
    pibObj.pData = (uint8_t *)&plcMaxRMSVeryLow[index];
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PL360_ID_THRESHOLDS_TABLE_HI;
    pibObj.length = 16 << 2;
    pibObj.pData = (uint8_t *)&plcThresholdsHigh[index];
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PL360_ID_THRESHOLDS_TABLE_VLO;
    pibObj.length = 16 << 2;
    pibObj.pData = (uint8_t *)&plcThresholdsVeryLow[index];
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);
    
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

    pibObj.id = PL360_ID_GAIN_TABLE_HI;
    pibObj.length = 3 << 1;
    pibObj.pData = (uint8_t *)pGainHi;
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);
    
    pibObj.id = PL360_ID_GAIN_TABLE_VLO;
    pibObj.length = 3 << 1;
    pibObj.pData = (uint8_t *)pGainVlo;
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);
    
    pibObj.id = PL360_ID_PREDIST_COEF_TABLE_HI;
    pibObj.length = 97 << 1;
    pibObj.pData = (uint8_t *)pEquHi;
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);
    
    pibObj.id = PL360_ID_PREDIST_COEF_TABLE_VLO;
    pibObj.length = 97 << 1;
    pibObj.pData = (uint8_t *)pEquVlo;
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);
    
    pibObj.id = PL360_ID_DACC_TABLE_CFG;
    pibObj.length = 17 << 2;
    pibObj.pData = (uint8_t *)pDaccTbl;
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);
    
}

/*******************************************************************************
  Function:
    void APP_PLC_Initialize ( void )

  Remarks:
    See prototype in app_plc.h.
 */

void APP_PLC_Initialize ( void )
{
    /* Read configuration from NVM memory */
    appPlc.state = APP_PLC_STATE_READ_CONFIG;
    
    /* Set configuration of coupling parameters */
    appPlc.couplingConfig = APP_CONFIG_PLC_COUP;
    appPlc.couplingLevels = NUM_TX_LEVELS;
    
    /* Init flags of PLC transmission */
    appPlc.waitingTxCfm = false;
    
    /* Init PLC objects */
    appPlcTx.pl360Tx.pTransmitData = appPlcTx.pDataTx;
    
}

/******************************************************************************
  Function:
    void APP_PLC_Tasks ( void )

  Remarks:
    See prototype in app_plc.h.
 */

void APP_PLC_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appPlc.state )
    {
        case APP_PLC_STATE_READ_CONFIG:
        {
            if (appNvm.state == APP_NVM_STATE_CMD_WAIT)
            {
                appNvm.pData = (uint8_t*)&appPlcTx;
                appNvm.dataLength = sizeof(appPlcTx);
                appNvm.state = APP_NVM_STATE_READ_MEMORY;
                
                appPlc.state = APP_PLC_STATE_CHECK_CONFIG;
            }
            break;
        }

        case APP_PLC_STATE_CHECK_CONFIG:
        {
            if (appNvm.state == APP_NVM_STATE_CMD_WAIT)
            {
                if (appPlcTx.configKey != APP_PLC_CONFIG_KEY)
                {
                    uint8_t index;
                    uint8_t* pData;
                    
                    /* Set configuration by default */
                    appPlcTx.configKey = APP_PLC_CONFIG_KEY;
                    appPlcTx.pl360PhyVersion = 0;
                    appPlcTx.txImpedance = HI_STATE;
                    appPlcTx.txAuto = 1;
                    appPlcTx.txForceNoOutput = 0;
                    appPlcTx.pl360Tx.bufferId = 0;
                    appPlcTx.pl360Tx.attenuation = 0;
                    appPlcTx.pl360Tx.scheme = SCHEME_DBPSK;
                    appPlcTx.pl360Tx.frameType = FRAME_TYPE_A;
                    appPlcTx.pl360Tx.forced = 0;
                    appPlcTx.pl360Tx.mode = TX_MODE_RELATIVE;
                    appPlcTx.pl360Tx.time = 1000000;        
                    appPlcTx.pl360Tx.pTransmitData = appPlcTx.pDataTx;
                    appPlcTx.pl360Tx.dataLength = 64;
                    pData = appPlcTx.pDataTx;
                    for(index = 0; index < appPlcTx.pl360Tx.dataLength; index++)
                    {
                        *pData++ = index;
                    }                       
                    
                    /* Clear Transmission flag */
                    appPlcTx.inTx = false;
                    
                    /* Select PLC binary by default */
                    appPlcTx.bin2InUse = false;
                }
                    
                /* Initialize PLC driver */
                appPlc.state = APP_PLC_STATE_INIT;
            }
            break;
        }

        case APP_PLC_STATE_WRITE_CONFIG:
        {
            if (appNvm.state == APP_NVM_STATE_CMD_WAIT)
            {
                appNvm.pData = (uint8_t*)&appPlcTx;
                appNvm.dataLength = sizeof(appPlcTx);
                appNvm.state = APP_NVM_STATE_WRITE_MEMORY;
                
                appPlc.state = APP_PLC_STATE_WAIT_CONFIG;
            }
            break;
        }

        case APP_PLC_STATE_WAIT_CONFIG:
        {
            if (appNvm.state == APP_NVM_STATE_CMD_WAIT)
            {
                if (appPlcTx.inTx)
                {
                    appPlc.state = APP_PLC_STATE_TX;
                }
                else
                {
                    appPlc.state = APP_PLC_STATE_WAITING;
                }                
            }
            break;
        }
        
        case APP_PLC_STATE_INIT:
        {
            /* Open PL360 driver */
            appPlc.drvPl360Handle = DRV_PL360_Open(DRV_PL360_INDEX_0, DRV_IO_INTENT_READWRITE);
            
            if (appPlc.drvPl360Handle != DRV_HANDLE_INVALID)
            {
                appPlc.state = APP_PLC_STATE_OPEN;
            }
            else
            {
                appPlc.state = APP_PLC_STATE_ERROR;
            }
            break;
        }
            
        case APP_PLC_STATE_OPEN:
        {
            /* Check PL360 device */
            if (DRV_PL360_Status(DRV_PL360_INDEX_0) == SYS_STATUS_READY)
            { 
                DRV_PL360_PIB_OBJ pibObj;                
                uint32_t version;
                
                /* Configure PL360 callbacks */
                DRV_PL360_ExceptionCallbackRegister(appPlc.drvPl360Handle, APP_PLC_ExceptionCb, DRV_PL360_INDEX_0);
                DRV_PL360_DataCfmCallbackRegister(appPlc.drvPl360Handle, APP_PLC_DataCfmCb, DRV_PL360_INDEX_0);
                
                /* Get PL360 PHY version */
                pibObj.id = PL360_ID_VERSION_NUM;
                pibObj.length = 4;
                pibObj.pData = (uint8_t *)&version;
                DRV_PL360_PIBGet(appPlc.drvPl360Handle, &pibObj);     
                
                if (version == appPlcTx.pl360PhyVersion)
                {
                    if (appPlcTx.inTx)
                    {
                        /* Previous Transmission state */
                        appPlc.state = APP_PLC_STATE_TX;                        
                    }
                    else
                    {
                        /* Nothing To Do */
                        appPlc.state = APP_PLC_STATE_WAITING;
                    }
                }
                else
                {
                    appPlcTx.pl360PhyVersion = version;
                    
                    /* Get PL360 PHY Gains for High Impedance Mode */
                    pibObj.id = PL360_ID_GAIN_TABLE_HI;
                    pibObj.length = 6;
                    pibObj.pData = (uint8_t *)&appPlcTx.pl360GainHigh;
                    DRV_PL360_PIBGet(appPlc.drvPl360Handle, &pibObj);                
                    /* Get PL360 PHY Gains for Very Low Impedance Mode */
                    pibObj.id = PL360_ID_GAIN_TABLE_VLO;
                    pibObj.length = 6;
                    pibObj.pData = (uint8_t *)&appPlcTx.pl360GainVeryLow;
                    DRV_PL360_PIBGet(appPlc.drvPl360Handle, &pibObj);

                    /* Store configuration in NVM memory */
                    appPlc.state = APP_PLC_STATE_WRITE_CONFIG;
                }        
                
                /* Apply PLC coupling configuration */
                if (appPlc.couplingConfig)
                {
                    APP_PLC_ApplyPlcConfiguration();
                }
            }                
            break;
        }

        case APP_PLC_STATE_WAITING:
        {
            break;
        }

        case APP_PLC_STATE_TX:
        {
            if (!appPlcTx.inTx)
            {
                DRV_PL360_PIB_OBJ pibObj;
                
                /* Apply TX configuration */
                /* Set Autodetect Mode */
                pibObj.id = PL360_ID_CFG_AUTODETECT_IMPEDANCE;
                pibObj.length = 1;
                pibObj.pData = (uint8_t *)&appPlcTx.txAuto;
                DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);
                /* Set Impedance Mode */
                pibObj.id = PL360_ID_CFG_IMPEDANCE;
                pibObj.length = 1;
                pibObj.pData = (uint8_t *)&appPlcTx.txImpedance;
                DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);
                /* Force No Output Signal */
                if (appPlcTx.txForceNoOutput)
                {
                    uint16_t nullGain[3] = {0};
                    
                    pibObj.id = PL360_ID_GAIN_TABLE_HI;
                    pibObj.length = 6;
                    pibObj.pData = (uint8_t *)nullGain;
                    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);
                    pibObj.id = PL360_ID_GAIN_TABLE_VLO;
                    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);
                }
                else
                {
                    pibObj.id = PL360_ID_GAIN_TABLE_HI;
                    pibObj.length = 6;
                    pibObj.pData = (uint8_t *)appPlcTx.pl360GainHigh;
                    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);
                    pibObj.id = PL360_ID_GAIN_TABLE_VLO;
                    pibObj.length = 6;
                    pibObj.pData = (uint8_t *)appPlcTx.pl360GainVeryLow;
                    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);                    
                }
                
                /* Set Time mode */
                appPlcTx.pl360Tx.mode = TX_MODE_RELATIVE;
                
                /* Set Transmission flag */
                appPlcTx.inTx = true;  
                
                /* Store TX configuration */
                appPlc.state = APP_PLC_STATE_WRITE_CONFIG;
            }
            else
            {
                if (!appPlc.waitingTxCfm)
                {
                    appPlc.waitingTxCfm = true;
                    /* Send PLC message */
                    DRV_PL360_Send(appPlc.drvPl360Handle, &appPlcTx.pl360Tx);                    
                }
            }            
            
            break;
        }

        case APP_PLC_STATE_STOP_TX:
        {
            /* Clear Transmission flag */
            appPlcTx.inTx = false;
            
            /* Store TX configuration */
            appPlc.state = APP_PLC_STATE_WRITE_CONFIG;
            
            /* Cancel last transmission */
            if (appPlc.waitingTxCfm)
            {
                /* Send PLC Cancel message */
                appPlcTx.pl360Tx.mode = TX_MODE_CANCEL | TX_MODE_RELATIVE;   
                DRV_PL360_Send(appPlc.drvPl360Handle, &appPlcTx.pl360Tx);
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
