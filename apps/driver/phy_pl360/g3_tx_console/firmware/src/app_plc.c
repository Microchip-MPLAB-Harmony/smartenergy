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

APP_PLC_DATA CACHE_ALIGN appPlc;
APP_PLC_DATA_TX CACHE_ALIGN appPlcTx;

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

/*******************************************************************************
  Function:
    void APP_PLC_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_PLC_Initialize ( void )
{
    /* Update state machine */
    appPlc.state = APP_PLC_STATE_INIT; 
    
    /* Init flags of PLC transmission */
    appPlc.waitingTxCfm = false;
    
    /* Init PLC objects */
    appPlcTx.pl360Tx.pTransmitData = appPlcTx.pDataTx;
    
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
    switch ( appPlc.state )
    {
        /* Application's initial state. */
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
                /* Configure PL360 callbacks */
                DRV_PL360_ExceptionCallbackRegister(appPlc.drvPl360Handle, APP_PLC_ExceptionCb, DRV_PL360_INDEX_0);
                DRV_PL360_DataCfmCallbackRegister(appPlc.drvPl360Handle, APP_PLC_DataCfmCb, DRV_PL360_INDEX_0);
                
                /* Update configuration from NVM memory */
                appPlc.state = APP_PLC_STATE_READ_CONFIG;
            }                
            break;
        }

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
                DRV_PL360_PIB_OBJ pibObj;
                uint32_t version;
                uint8_t bandID;
                
                /* Get PL360 PHY version */
                pibObj.id = PL360_ID_VERSION_NUM;
                pibObj.length = 4;
                pibObj.pData = (uint8_t *)&version;
                DRV_PL360_PIBGet(appPlc.drvPl360Handle, &pibObj);                
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
                
                if (appPlcTx.pl360PhyVersion == version)
                {
                    if (appPlcTx.inTx)
                    {
                        /* Previous TX state */
                        appPlc.state = APP_PLC_STATE_TX;                        
                    }
                    else
                    {
                        /* Correct version of G3 PHY layer */
                        appPlc.state = APP_PLC_STATE_WAITING;
                    }                    
                }
                else
                {
                    uint8_t index;
                    uint8_t* pData;
                    
                    /* Set configuration by default */
                    appPlcTx.pl360PhyVersion = version;
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
                    
                    bandID = (uint8_t)(version >> 16);
                    if (bandID == 0x01)
                    {
                        /* CEN A */
                        appPlcTx.toneMapSize = 1;
                        appPlcTx.pl360Tx.toneMap[0] = 0x3F;
                        appPlcTx.preemphasisSize = 6;                       
                    }
                    else if (bandID == 0x02)
                    {
                        /* FCC */
                        appPlcTx.toneMapSize = 3;
                        memset(appPlcTx.pl360Tx.toneMap, 0xFF, 3);
                        appPlcTx.preemphasisSize = 24;
                    }
                    else
                    {
                        /* CEN B */
                        appPlcTx.toneMapSize = 1;
                        appPlcTx.pl360Tx.toneMap[0] = 0x0F;
                        appPlcTx.preemphasisSize = 3;
                    }   
                    
                    memset(appPlcTx.pl360Tx.preemphasis, 0, sizeof(appPlcTx.pl360Tx.preemphasis));
                    
                    /* Clear Transmission flag */
                    appPlcTx.inTx = false;
                    
                    /* Save PLC TX Configuration */
                    appPlc.state = APP_PLC_STATE_WRITE_CONFIG;
                }                
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
