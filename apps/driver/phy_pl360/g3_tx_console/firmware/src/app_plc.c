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
#define APP_PLC_CONFIG_KEY  0xA55A

extern uint8_t pl360_bin_start;
extern uint8_t pl360_bin_end;
#if defined APP_CONFIG_PLC_MULTIBAND == true
extern uint8_t pl360_bin2_start;
extern uint8_t pl360_bin2_end;
#endif

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
/* PLC Coupling configuration data

  Summary:
    Holds PLC configuration data

  Description:
    This structure holds the PLC coupling configuration data.

  Remarks: 
    Parameters are defined in user.h file
 */ 

APP_PLC_COUPLING_DATA appPLCCoupConfigCENA = {MAX_RMS_HI_TABLE_CENA, 
                                              MAX_RMS_VLO_TABLE_CENA, 
                                              THRESHOLD_HI_TABLE_CENA, 
                                              THRESHOLD_VLO_TABLE_CENA,
                                              DACC_CFG_TABLE_CENA,
                                              PREDIST_HI_TABLE_CENA, 
                                              PREDIST_VLO_TABLE_CENA,
                                              GAIN_HI_CENA, 
                                              GAIN_VLO_CENA,
                                              NUM_TX_LEVELS};

APP_PLC_COUPLING_DATA appPLCCoupConfigFCC  = {MAX_RMS_HI_TABLE_FCC, 
                                              MAX_RMS_VLO_TABLE_FCC, 
                                              THRESHOLD_HI_TABLE_FCC, 
                                              THRESHOLD_VLO_TABLE_FCC,
                                              DACC_CFG_TABLE_FCC,
                                              PREDIST_HI_TABLE_FCC, 
                                              PREDIST_VLO_TABLE_FCC,
                                              GAIN_HI_FCC, 
                                              GAIN_VLO_FCC,
                                              NUM_TX_LEVELS};

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
static void APP_PLC_ApplyPlcConfiguration ( void )
{
    DRV_PL360_PIB_OBJ pibObj; 
    APP_PLC_COUPLING_DATA* pCfgData;
    
    if (appPlcTx.bin2InUse)
    {
        /* FCC */
        pCfgData = &appPLCCoupConfigFCC;
    }
    else
    {
        /* CEN A */
        pCfgData = &appPLCCoupConfigCENA;
    }
    
    pibObj.id = PL360_ID_NUM_TX_LEVELS;
    pibObj.length = 1;
    *pibObj.pData = pCfgData->numTxLevels;
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PL360_ID_MAX_RMS_TABLE_HI;
    pibObj.length = sizeof(pCfgData->maxRMSHigh);
    memcpy(pibObj.pData, (uint8_t *)pCfgData->maxRMSHigh, 
            pibObj.length);
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PL360_ID_MAX_RMS_TABLE_VLO;
    pibObj.length = sizeof(pCfgData->maxRMSVeryLow);
    memcpy(pibObj.pData, (uint8_t *)pCfgData->maxRMSVeryLow, 
            pibObj.length);
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PL360_ID_THRESHOLDS_TABLE_HI;
    pibObj.length = sizeof(pCfgData->thresholdHigh);
    memcpy(pibObj.pData, (uint8_t *)pCfgData->thresholdHigh, 
            pibObj.length);
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PL360_ID_THRESHOLDS_TABLE_VLO;
    pibObj.length = sizeof(pCfgData->thresholdVeryLow);
    memcpy(pibObj.pData, (uint8_t *)pCfgData->thresholdVeryLow, 
            pibObj.length);
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PL360_ID_GAIN_TABLE_HI;
    pibObj.length = sizeof(pCfgData->gainHigh);
    memcpy(pibObj.pData, (uint8_t *)pCfgData->gainHigh, 
            pibObj.length);
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PL360_ID_GAIN_TABLE_VLO;
    pibObj.length = sizeof(pCfgData->gainVeryLow);
    memcpy(pibObj.pData, (uint8_t *)pCfgData->gainVeryLow, 
            pibObj.length);
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PL360_ID_DACC_TABLE_CFG;
    pibObj.length = sizeof(pCfgData->daccConfig);
    memcpy(pibObj.pData, (uint8_t *)pCfgData->daccConfig, 
            pibObj.length);
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PL360_ID_PREDIST_COEF_TABLE_HI;
    pibObj.length = sizeof(pCfgData->predistorsionHigh);
    memcpy(pibObj.pData, (uint8_t *)pCfgData->predistorsionHigh, 
            pibObj.length);
    DRV_PL360_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PL360_ID_PREDIST_COEF_TABLE_VLO;
    /* Not use size of array. It depends on PHY band in use */
    pibObj.length = sizeof(pCfgData->predistorsionVeryLow);
    memcpy(pibObj.pData, (uint8_t *)pCfgData->predistorsionVeryLow, 
            pibObj.length);
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
    
    /* Set PLC Multi-band option */
    appPlc.plcMultiband = APP_CONFIG_PLC_MULTIBAND;
    
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
                    appPlcTx.pl360Tx.time = 1000000;
                    appPlcTx.pl360Tx.attenuation = 0;
                    appPlcTx.pl360Tx.modScheme = MOD_SCHEME_DIFFERENTIAL;
                    appPlcTx.pl360Tx.modType = MOD_TYPE_BPSK;
                    appPlcTx.pl360Tx.delimiterType = DT_SOF_NO_RESP;
                    appPlcTx.pl360Tx.mode = TX_MODE_RELATIVE;
                    appPlcTx.pl360Tx.rs2Blocks = 0;
                    appPlcTx.pl360Tx.pdc = 0;                    
                    appPlcTx.pl360Tx.pTransmitData = appPlcTx.pDataTx;
                    appPlcTx.pl360Tx.dataLength = 64;
                    pData = appPlcTx.pDataTx;
                    for(index = 0; index < appPlcTx.pl360Tx.dataLength; index++)
                    {
                        *pData++ = index;
                    }                       
                    
                    /* CEN A */
                    appPlcTx.toneMapSize = 1;
                    appPlcTx.pl360Tx.toneMap[0] = 0x3F;
                    appPlcTx.preemphasisSize = 6;                       
                    
                    memset(appPlcTx.pl360Tx.preemphasis, 0, sizeof(appPlcTx.pl360Tx.preemphasis));
                    
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
#if defined APP_CONFIG_PLC_MULTIBAND == true            
            /* Select PLC Binary file for multiband solution */
            if ((appPlcTx.bin2InUse) && (appPlc.plcMultiband))
            {
                drvPL360InitData.binStartAddress = (uint32_t)&pl360_bin2_start;
                drvPL360InitData.binEndAddress = (uint32_t)&pl360_bin2_end;
            }
            else
            {
                drvPL360InitData.binStartAddress = (uint32_t)&pl360_bin_start;
                drvPL360InitData.binEndAddress = (uint32_t)&pl360_bin_end;
            }
#endif            
            /* Initialize PL360 Driver Instance */
            sysObj.drvPL360 = DRV_PL360_Initialize(DRV_PL360_INDEX, (SYS_MODULE_INIT *)&drvPL360InitData);
            /* Register Callback function to handle PL360 interruption */
            PIO_PinInterruptCallbackRegister(DRV_PL360_EXT_INT_PIN, DRV_PL360_ExternalInterruptHandler, sysObj.drvPL360);
    
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
                /* Set Transmission Mode */
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

        case APP_PLC_STATE_SET_BAND:
        {
            if (!appPlc.plcMultiband) 
            {
                /* PLC Multi-band is not supported */
                appPlc.state = APP_PLC_STATE_WAITING;
                break;
            }
            
            /* Clear Transmission flags */
            appPlcTx.inTx = false;
            appPlc.waitingTxCfm = false;
            
            /* Adjust ToneMap and Preemphasis by default */
            if (appPlcTx.bin2InUse)
            {
                /* FCC */
                appPlcTx.toneMapSize = 3;
                memset(appPlcTx.pl360Tx.toneMap, 0xFF, 3);
                appPlcTx.preemphasisSize = 24;                   
            }
            else
            {
                /* CEN A */
                appPlcTx.toneMapSize = 1;
                appPlcTx.pl360Tx.toneMap[0] = 0x3F;
                appPlcTx.preemphasisSize = 6;    
            }
            
            /* Close PLC Driver */
            DRV_PL360_Close(appPlc.drvPl360Handle);
            
            /* Restart PLC Driver */
            appPlc.state = APP_PLC_STATE_INIT;
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
