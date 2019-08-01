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
#define DRV_PLC_PHY_INDEX_0   0
#define APP_PLC_CONFIG_KEY  0xA55A

extern uint8_t plc_phy_bin_start;
extern uint8_t plc_phy_bin_end;
#if (APP_CONFIG_PLC_MULTIBAND == true)
extern uint8_t plc_phy_bin2_start;
extern uint8_t plc_phy_bin2_end;
#endif

/* PLC Driver Initialization Data (initialization.c) */
extern DRV_PLC_PHY_INIT drvPlcPhyInitData;

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

#ifdef MAX_RMS_HI_TABLE_CENA
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
#endif

#ifdef MAX_RMS_HI_TABLE_FCC
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
#endif

#ifdef MAX_RMS_HI_TABLE_CENB
APP_PLC_COUPLING_DATA appPLCCoupConfigCENB  = {MAX_RMS_HI_TABLE_CENB,
                                              MAX_RMS_VLO_TABLE_CENB,
                                              THRESHOLD_HI_TABLE_CENB,
                                              THRESHOLD_VLO_TABLE_CENB,
                                              DACC_CFG_TABLE_CENB,
                                              PREDIST_HI_TABLE_CENB,
                                              PREDIST_VLO_TABLE_CENB,
                                              GAIN_HI_CENB,
                                              GAIN_VLO_CENB,
                                              NUM_TX_LEVELS};
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void APP_PLC_ExceptionCb(DRV_PLC_PHY_EXCEPTION exceptionObj, uintptr_t context )
{
    /* Avoid warning */
    (void)context;

    /* Clear App flag */
    appPlc.waitingTxCfm = false;
    /* Restore TX configuration */
    appPlc.state = APP_PLC_STATE_READ_CONFIG;
}

static void APP_PLC_DataCfmCb(DRV_PLC_PHY_TRANSMISSION_CFM_OBJ *cfmObj, uintptr_t context )
{
    /* Avoid warning */
    (void)context;

    /* Update App flags */
    appPlc.waitingTxCfm = false;

    /* Handle result of transmission : Show it through Console */
    switch(cfmObj->result)
    {
        case DRV_PLC_PHY_TX_RESULT_PROCESS:
            APP_CONSOLE_Print("...DRV_PLC_PHY_TX_RESULT_PROCESS\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_SUCCESS:
            APP_CONSOLE_Print("...DRV_PLC_PHY_TX_RESULT_SUCCESS\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_INV_LENGTH:
            APP_CONSOLE_Print("...DRV_PLC_PHY_TX_RESULT_INV_LENGTH\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_BUSY_CH:
            APP_CONSOLE_Print("...DRV_PLC_PHY_TX_RESULT_BUSY_CH\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_BUSY_TX:
            APP_CONSOLE_Print("...DRV_PLC_PHY_TX_RESULT_BUSY_TX\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_BUSY_RX:
            APP_CONSOLE_Print("...DRV_PLC_PHY_TX_RESULT_BUSY_RX\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_INV_SCHEME:
            APP_CONSOLE_Print("...DRV_PLC_PHY_TX_RESULT_INV_SCHEME\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_TIMEOUT:
            APP_CONSOLE_Print("...DRV_PLC_PHY_TX_RESULT_TIMEOUT\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_INV_TONEMAP:
            APP_CONSOLE_Print("...DRV_PLC_PHY_TX_RESULT_INV_TONEMAP\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_INV_MODTYPE:
            APP_CONSOLE_Print("...DRV_PLC_PHY_TX_RESULT_INV_MODTYPE\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_INV_DT:
            APP_CONSOLE_Print("...DRV_PLC_PHY_TX_RESULT_INV_DT\r\n");
            break;
        case DRV_PLC_PHY_TX_RESULT_NO_TX:
            APP_CONSOLE_Print("...DRV_PLC_PHY_TX_RESULT_NO_TX\r\n");
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
    DRV_PLC_PHY_PIB_OBJ pibObj;
    APP_PLC_COUPLING_DATA* pCfgData;

#ifdef MAX_RMS_HI_TABLE_CENB
    /* CEN B */
    pCfgData = &appPLCCoupConfigCENB;
#endif

#ifdef MAX_RMS_HI_TABLE_CENA
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
#endif

    pibObj.id = PLC_ID_NUM_TX_LEVELS;
    pibObj.length = 1;
    pibObj.pData = &pCfgData->numTxLevels;
    DRV_PLC_PHY_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PLC_ID_MAX_RMS_TABLE_HI;
    pibObj.length = sizeof(pCfgData->maxRMSHigh);
    pibObj.pData = (uint8_t *)&pCfgData->maxRMSHigh;
    DRV_PLC_PHY_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PLC_ID_MAX_RMS_TABLE_VLO;
    pibObj.length = sizeof(pCfgData->maxRMSVeryLow);
    pibObj.pData = (uint8_t *)&pCfgData->maxRMSVeryLow;
    DRV_PLC_PHY_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PLC_ID_THRESHOLDS_TABLE_HI;
    pibObj.length = sizeof(pCfgData->thresholdHigh);
    pibObj.pData = (uint8_t *)&pCfgData->thresholdHigh;
    DRV_PLC_PHY_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PLC_ID_THRESHOLDS_TABLE_VLO;
    pibObj.length = sizeof(pCfgData->thresholdVeryLow);
    pibObj.pData = (uint8_t *)&pCfgData->thresholdVeryLow;
    DRV_PLC_PHY_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PLC_ID_GAIN_TABLE_HI;
    pibObj.length = sizeof(pCfgData->gainHigh);
    pibObj.pData = (uint8_t *)&pCfgData->gainHigh;
    DRV_PLC_PHY_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PLC_ID_GAIN_TABLE_VLO;
    pibObj.length = sizeof(pCfgData->gainVeryLow);
    pibObj.pData = (uint8_t *)&pCfgData->gainVeryLow;
    DRV_PLC_PHY_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PLC_ID_DACC_TABLE_CFG;
    pibObj.length = sizeof(pCfgData->daccConfig);
    pibObj.pData = (uint8_t *)&pCfgData->daccConfig;
    DRV_PLC_PHY_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PLC_ID_PREDIST_COEF_TABLE_HI;
    pibObj.length = sizeof(pCfgData->predistorsionHigh);
    pibObj.pData = (uint8_t *)&pCfgData->predistorsionHigh;
    DRV_PLC_PHY_PIBSet(appPlc.drvPl360Handle, &pibObj);

    pibObj.id = PLC_ID_PREDIST_COEF_TABLE_VLO;
    /* Not use size of array. It depends on PHY band in use */
    pibObj.length = sizeof(pCfgData->predistorsionVeryLow);
    pibObj.pData = (uint8_t *)&pCfgData->predistorsionVeryLow;
    DRV_PLC_PHY_PIBSet(appPlc.drvPl360Handle, &pibObj);

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
#if (APP_CONFIG_PLC_MULTIBAND == true)
            /* Select PLC Binary file for multi-band solution */
            if ((appPlcTx.bin2InUse) && (appPlc.plcMultiband))
            {
                drvPlcPhyInitData.binStartAddress = (uint32_t)&plc_phy_bin2_start;
                drvPlcPhyInitData.binEndAddress = (uint32_t)&plc_phy_bin2_end;
            }
            else
            {
                drvPlcPhyInitData.binStartAddress = (uint32_t)&plc_phy_bin_start;
                drvPlcPhyInitData.binEndAddress = (uint32_t)&plc_phy_bin_end;
            }

            /* Initialize PLC Driver Instance */
            sysObj.drvPlcPhy = DRV_PLC_PHY_Initialize(DRV_PLC_PHY_INDEX, (SYS_MODULE_INIT *)&drvPlcPhyInitData);
            /* Register Callback function to handle PLC interruption */
            PIO_PinInterruptCallbackRegister(DRV_PLC_EXT_INT_PIN, DRV_PLC_PHY_ExternalInterruptHandler, sysObj.drvPlcPhy);
#endif

            /* Open PLC driver */
            appPlc.drvPl360Handle = DRV_PLC_PHY_Open(DRV_PLC_PHY_INDEX_0, NULL);

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
            /* Check PLC transceiver */
            if (DRV_PLC_PHY_Status(DRV_PLC_PHY_INDEX_0) == SYS_STATUS_READY)
            {
                DRV_PLC_PHY_PIB_OBJ pibObj;
                uint32_t version;

                /* Configure PLC callbacks */
                DRV_PLC_PHY_ExceptionCallbackRegister(appPlc.drvPl360Handle, APP_PLC_ExceptionCb, DRV_PLC_PHY_INDEX_0);
                DRV_PLC_PHY_DataCfmCallbackRegister(appPlc.drvPl360Handle, APP_PLC_DataCfmCb, DRV_PLC_PHY_INDEX_0);

                /* Get PLC PHY version */
                pibObj.id = PLC_ID_VERSION_NUM;
                pibObj.length = 4;
                pibObj.pData = (uint8_t *)&version;
                DRV_PLC_PHY_PIBGet(appPlc.drvPl360Handle, &pibObj);

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

                    /* Adjust ToneMap Info */
                    switch ((uint8_t)(appPlcTx.pl360PhyVersion >> 16))
                    {
                        case 1:
                            /* CEN A */
                            appPlcTx.toneMapSize = TONE_MAP_SIZE_CENELEC;
                            appPlcTx.pl360Tx.toneMap[0] = 0x3F;
                            break;

                        case 2:
                            /* FCC */
                            appPlcTx.toneMapSize = TONE_MAP_SIZE_FCC;
                            appPlcTx.pl360Tx.toneMap[0] = 0xFF;
                            appPlcTx.pl360Tx.toneMap[1] = 0xFF;
                            appPlcTx.pl360Tx.toneMap[2] = 0xFF;
                            break;

                        case 3:
                            /* ARIB */
                            appPlcTx.toneMapSize = TONE_MAP_SIZE_FCC;
                            appPlcTx.pl360Tx.toneMap[0] = 0x03;
                            appPlcTx.pl360Tx.toneMap[1] = 0xFF;
                            appPlcTx.pl360Tx.toneMap[2] = 0xFF;
                            break;

                        case 4:
                            /* CEN B */
                            appPlcTx.toneMapSize = TONE_MAP_SIZE_CENELEC;
                            appPlcTx.pl360Tx.toneMap[0] = 0x0F;
                            break;
                    }

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
                DRV_PLC_PHY_PIB_OBJ pibObj;

                /* Apply TX configuration */
                /* Set Autodetect Mode */
                pibObj.id = PLC_ID_CFG_AUTODETECT_IMPEDANCE;
                pibObj.length = 1;
                pibObj.pData = (uint8_t *)&appPlcTx.txAuto;
                DRV_PLC_PHY_PIBSet(appPlc.drvPl360Handle, &pibObj);
                /* Set Impedance Mode */
                pibObj.id = PLC_ID_CFG_IMPEDANCE;
                pibObj.length = 1;
                pibObj.pData = (uint8_t *)&appPlcTx.txImpedance;
                DRV_PLC_PHY_PIBSet(appPlc.drvPl360Handle, &pibObj);

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
                    DRV_PLC_PHY_Send(appPlc.drvPl360Handle, &appPlcTx.pl360Tx);
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
                DRV_PLC_PHY_Send(appPlc.drvPl360Handle, &appPlcTx.pl360Tx);
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

            /* Close PLC Driver */
            DRV_PLC_PHY_Close(appPlc.drvPl360Handle);

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
