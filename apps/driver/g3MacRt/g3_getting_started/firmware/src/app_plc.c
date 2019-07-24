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
#include <stdbool.h>
#include "app_plc.h"
#include "user.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#define DRV_G3_MACRT_INDEX_0   0

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

static void APP_PLC_ExceptionCb ( DRV_G3_MACRT_EXCEPTION exceptionObj, 
        uintptr_t context )
{
    /* Avoid warning */
    (void)context;
    
    /* Clear App flag */
    appPlc.waitingTxCfm = false;
}

static void APP_PLC_TxCfmCb ( MAC_RT_TX_CFM_OBJ *cfmObj, uintptr_t context )
{
    /* Avoid warning */
    (void)context;
    
    /* Update App flags */
    appPlc.waitingTxCfm = false;
    
    /* Handle result of transmission : Show it through Console */
    switch(cfmObj->status)
    {
        case MAC_RT_STATUS_SUCCESS:
            APP_CONSOLE_Print("...MAC_RT_STATUS_SUCCESS\r\n");
            break;   
        case MAC_RT_STATUS_CHANNEL_ACCESS_FAILURE:
            APP_CONSOLE_Print("...MAC_RT_STATUS_CHANNEL_ACCESS_FAILURE\r\n");
            break;   
        case MAC_RT_STATUS_NO_ACK:
            APP_CONSOLE_Print("...MAC_RT_STATUS_NO_ACK\r\n");
            break;
        case MAC_RT_STATUS_DENIED:
            APP_CONSOLE_Print("...MAC_RT_STATUS_DENIED\r\n");
            break;
        case MAC_RT_STATUS_INVALID_INDEX:
            APP_CONSOLE_Print("...MAC_RT_STATUS_INVALID_INDEX\r\n");
            break;
        case MAC_RT_STATUS_INVALID_PARAMETER:
            APP_CONSOLE_Print("...MAC_RT_STATUS_INVALID_PARAMETER\r\n");
            break;
        case MAC_RT_STATUS_TRANSACTION_OVERFLOW:
            APP_CONSOLE_Print("...MAC_RT_STATUS_TRANSACTION_OVERFLOW\r\n");
            break;
        case MAC_RT_STATUS_UNSUPPORTED_ATTRIBUTE:
            APP_CONSOLE_Print("...MAC_RT_STATUS_UNSUPPORTED_ATTRIBUTE\r\n");
            break;
        default:
            APP_CONSOLE_Print("...UNKNWED STATUS\r\n");
    }
    
    /* Update App to waiting state */
    appPlc.state = APP_PLC_STATE_WAITING;
}

static void APP_PLC_MlmeGetCfmCb ( MAC_RT_RX_PARAMETERS_OBJ *pParameters, 
        uintptr_t context )
{
    /* Avoid warning */
    (void)context;

    /* Store parameters of the last received message */
    appPlc.g3MacRtRxParameters = *pParameters;
}

static void APP_PLC_DataIndCb ( uint8_t *pData, uint16_t length, 
        uintptr_t context )
{
    /* Avoid warning */
    (void)context;
    
    /* Store data message */
    if ((length > 0) && (length <= sizeof(appPlc.pPlcRxData)))
    {
        /* Access to data content directly */
        memcpy(appPlc.pPlcRxData, pData + sizeof(APP_G3_MAC_HEADER), length);
    }
    appPlc.pPlcRxData[length] = '\0';

    /* Show Data Message */
    APP_CONSOLE_Print("\nRx (");

    switch(appPlc.g3MacRtRxParameters.modType)
    {
        case MAC_RT_MOD_ROBUST:
            APP_CONSOLE_Print("BPSK ROBUST ");
            break;
            
        case MAC_RT_MOD_BPSK:
            APP_CONSOLE_Print("BPSK ");
            break;
            
        case MAC_RT_MOD_QPSK:
            APP_CONSOLE_Print("QPSK ");
            break;
            
        case MAC_RT_MOD_8PSK:
            APP_CONSOLE_Print("8PSK ");
            break;
            
        case MAC_RT_MOD_16_QAM:
            APP_CONSOLE_Print("16QAM ");
            break;
    }

    switch(appPlc.g3MacRtRxParameters.modScheme)
    {
        case MAC_RT_MOD_SCHEME_DIFFERENTIAL:
            APP_CONSOLE_Print("Differential, ");
            break;
            
        case MAC_RT_MOD_SCHEME_COHERENT:
            APP_CONSOLE_Print("Coherent, ");
            break;
    }
    
    APP_CONSOLE_Print("LQI %udB): ", appPlc.g3MacRtRxParameters.pduLinkQuality);
    APP_CONSOLE_Print("%s\r\n", appPlc.pPlcRxData);
    APP_CONSOLE_Print("\r\n>>>");
    
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
static void APP_PLC_SetCouplingConfiguration ( void )
{
    APP_PLC_COUPLING_DATA* pCfgData;
    MAC_RT_PIB_OBJ pibObj;

#ifdef MAX_RMS_HI_TABLE_CENA
    /* CEN B */
    pCfgData = &appPLCCoupConfigCENA;
#endif

#ifdef MAX_RMS_HI_TABLE_CENB
    /* CEN B */
    pCfgData = &appPLCCoupConfigCENB;
#endif

#ifdef MAX_RMS_HI_TABLE_FCC
    /* CEN B */
    pCfgData = &appPLCCoupConfigFCC;
#endif    
    
    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_NUM_TX_LEVELS;
    pibObj.length = 1;
    pibObj.value[0] = pCfgData->numTxLevels;
    DRV_G3_MACRT_PIBSet(appPlc.drvG3MacRtHandle, &pibObj);

    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_MAX_RMS_TABLE_HI;
    pibObj.length = sizeof(pCfgData->maxRMSHigh);
    memcpy(pibObj.value, (uint8_t *)&pCfgData->maxRMSHigh, 
            sizeof(pCfgData->maxRMSHigh));
    DRV_G3_MACRT_PIBSet(appPlc.drvG3MacRtHandle, &pibObj);

    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_MAX_RMS_TABLE_VLO;
    pibObj.length = sizeof(pCfgData->maxRMSVeryLow);
    memcpy(pibObj.value, (uint8_t *)&pCfgData->maxRMSVeryLow, 
            sizeof(pCfgData->maxRMSVeryLow));
    DRV_G3_MACRT_PIBSet(appPlc.drvG3MacRtHandle, &pibObj);

    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_THRESHOLDS_TABLE_HI;
    pibObj.length = sizeof(pCfgData->thresholdHigh);
    memcpy(pibObj.value, (uint8_t *)&pCfgData->thresholdHigh, 
            sizeof(pCfgData->thresholdHigh));
    DRV_G3_MACRT_PIBSet(appPlc.drvG3MacRtHandle, &pibObj);

    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_THRESHOLDS_TABLE_VLO;
    pibObj.length = sizeof(pCfgData->thresholdVeryLow);
    memcpy(pibObj.value, (uint8_t *)&pCfgData->thresholdVeryLow, 
            sizeof(pCfgData->thresholdVeryLow));
    DRV_G3_MACRT_PIBSet(appPlc.drvG3MacRtHandle, &pibObj);

    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_GAIN_TABLE_HI;
    pibObj.length = sizeof(pCfgData->gainHigh);
    memcpy(pibObj.value, (uint8_t *)&pCfgData->gainHigh, 
            sizeof(pCfgData->gainHigh));
    DRV_G3_MACRT_PIBSet(appPlc.drvG3MacRtHandle, &pibObj);

    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_GAIN_TABLE_VLO;
    pibObj.length = sizeof(pCfgData->gainVeryLow);
    memcpy(pibObj.value, (uint8_t *)&pCfgData->gainVeryLow, 
            sizeof(pCfgData->gainVeryLow));
    DRV_G3_MACRT_PIBSet(appPlc.drvG3MacRtHandle, &pibObj);

    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_DACC_TABLE_CFG;
    pibObj.length = sizeof(pCfgData->daccConfig);
    memcpy(pibObj.value, (uint8_t *)&pCfgData->daccConfig, 
            sizeof(pCfgData->daccConfig));
    DRV_G3_MACRT_PIBSet(appPlc.drvG3MacRtHandle, &pibObj);

    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_PREDIST_COEF_TABLE_HI;
    pibObj.length = sizeof(pCfgData->predistorsionHigh);
    memcpy(pibObj.value, (uint8_t *)&pCfgData->predistorsionHigh, 
            sizeof(pCfgData->predistorsionHigh));
    DRV_G3_MACRT_PIBSet(appPlc.drvG3MacRtHandle, &pibObj);

    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_PREDIST_COEF_TABLE_VLO;
    pibObj.length = sizeof(pCfgData->predistorsionVeryLow);
    memcpy(pibObj.value, (uint8_t *)&pCfgData->predistorsionVeryLow, 
            sizeof(pCfgData->predistorsionVeryLow));
    DRV_G3_MACRT_PIBSet(appPlc.drvG3MacRtHandle, &pibObj);
    
}

static void APP_PLC_SetG3MacRtConfiguration ( void )
{
    MAC_RT_TX_PARAMETERS_OBJ *pParams;
    uint8_t plcBand;
    
    /* Init G3 Mac RT Parameters */
    pParams = &appPlc.g3MacRtTxParameters;
    
    pParams->dstAddr.addressMode = MAC_RT_SHORT_ADDRESS;
    pParams->dstAddr.shortAddress = appPlc.destinationAddress;
    pParams->forceRobo = 0;
    pParams->highPriority = 0;
    pParams->macHeaderLen = sizeof(APP_G3_MAC_HEADER);
    pParams->modScheme = appPlc.g3MacRtTxParameters.modScheme;
    pParams->modType = appPlc.g3MacRtTxParameters.modType;
    pParams->requestAck = 0;
    pParams->securityEnabled = 0;
    pParams->toneMapRequest = 0;
    memset(pParams->txCoef, 0, sizeof(pParams->txCoef)); 
    pParams->txGain = 0;
    pParams->txRes = 0;
    
    /* plcBand corresponds to G3 PHY band 
     * [0x01: CEN-A, 0x02: FCC, 0x03: ARIB, 0x04: CEN-B] */
    memset(pParams->toneMap, 0, sizeof(pParams->toneMap));
    plcBand = (uint8_t)(appPlc.phyVersion >> 16);
    switch (plcBand) 
    {
        case 0x01:
            /* G3 CEN A */
            pParams->toneMap[0] = 0x3F;
            break;

        case 0x02:
            /* G3 FCC */
            pParams->toneMap[0] = 0xFF;
            pParams->toneMap[1] = 0xFF;
            pParams->toneMap[2] = 0xFF;
            break;

        case 0x03:
            /* G3 ARIB */
            pParams->toneMap[0] = 0xFF;
            pParams->toneMap[1] = 0xFF;
            pParams->toneMap[2] = 0xFF;
            break;

        case 0x04:
            /* G3 CEN B */
            pParams->toneMap[0] = 0x0F;
            break;
    }
    
}

static void APP_PLC_BuildG3MacHeader ( void )
{
    APP_G3_MAC_HEADER *macHeader;
    
    macHeader = (APP_G3_MAC_HEADER *)appPlc.pPlcTxData;
    
    /* Set Frame Control */
    macHeader->frameControl.srcAddressingMode = 2; // 16b
    macHeader->frameControl.frameVersion = 0;
    macHeader->frameControl.destAddressingMode = 2; // 16b
    macHeader->frameControl.reserved = 0;
    macHeader->frameControl.panIdCompression = 1; // Only one PAN ID
    macHeader->frameControl.ackRequest = 0;
    macHeader->frameControl.framePending = 0;
    macHeader->frameControl.securityEnabled = 0;
    macHeader->frameControl.frameType = 1; // DAT
    /* Init Sequence Number */
    macHeader->sequenceNumber = appPlc.sequenceNumber;
    /* Set PAN ID */
    macHeader->panId = APP_G3_PAN_ID;
    /* Set Destination Address */
    macHeader->destinationAddress = appPlc.destinationAddress;
    /* Set Source Address */
    macHeader->sourceAddress = appPlc.sourceAddress;

}

/*******************************************************************************
  Function:
    void APP_PLC_Initialize ( void )

  Remarks:
    See prototype in app_plc.h.
 */

void APP_PLC_Initialize ( void )
{
    /* Set initial state */
    appPlc.state = APP_PLC_STATE_INIT;
    
    /* Set configuration of coupling parameters */
    appPlc.couplingConfig = APP_CONFIG_PLC_COUP;
    
    /* Select second binary file to start PLC Application
       false : CEN A, true : FCC */
    appPlc.bin2InUse = 0;
    
    /* Init flags of PLC transmission */
    appPlc.waitingTxCfm = 0;
    
    /* Init Sequence Number */
    appPlc.sequenceNumber = (uint8_t)TRNG_ReadData();
    
    /* Get Unique Short Address from True Random Number Generator */
    appPlc.sourceAddress = (uint16_t)TRNG_ReadData();
    
    /* Set Destination Address */
    appPlc.destinationAddress = MAC_RT_SHORT_ADDRESS_BROADCAST;
    
    /* Set G3 MAC RT payload pointers */
    appPlc.pG3MacRtTxPayload = appPlc.pPlcTxData + sizeof(APP_G3_MAC_HEADER);
    appPlc.pG3MacRtRxPayload = appPlc.pPlcRxData + sizeof(APP_G3_MAC_HEADER);
       
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
        case APP_PLC_STATE_INIT:
        {
            /* Open PLC driver */
            appPlc.drvG3MacRtHandle = DRV_G3_MACRT_Open(DRV_G3_MACRT_INDEX_0, NULL);
            
            if (appPlc.drvG3MacRtHandle != DRV_HANDLE_INVALID)
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
            if (DRV_G3_MACRT_Status(DRV_G3_MACRT_INDEX_0) == SYS_STATUS_READY)
            { 
                MAC_RT_PIB_OBJ pibObj;
                uint8_t plcBand;
                
                /* Configure G3 MACRT call-backs */
                DRV_G3_MACRT_ExceptionCallbackRegister(appPlc.drvG3MacRtHandle, 
                        APP_PLC_ExceptionCb, (const uintptr_t)&appPlc);
                DRV_G3_MACRT_TxCfmCallbackRegister(appPlc.drvG3MacRtHandle, 
                        APP_PLC_TxCfmCb, (const uintptr_t)&appPlc);
                DRV_G3_MACRT_MlmeGetCfmCallbackRegister(appPlc.drvG3MacRtHandle, 
                        APP_PLC_MlmeGetCfmCb, (const uintptr_t)&appPlc);
                DRV_G3_MACRT_DataIndCallbackRegister(appPlc.drvG3MacRtHandle, 
                        APP_PLC_DataIndCb, (const uintptr_t)&appPlc);
                
                /* Get version of the PLC Physical Layer */
                pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
                pibObj.index = PHY_PARAM_VERSION_NUM;
                pibObj.length = 4;
                DRV_G3_MACRT_PIBGet(appPlc.drvG3MacRtHandle, &pibObj);   
                appPlc.phyVersion = *(uint32_t *)pibObj.value;
                APP_CONSOLE_Print("\r\nPHY version: 0x%08x - ", (unsigned int)appPlc.phyVersion);
                
                plcBand = (uint8_t)(appPlc.phyVersion >> 16);
                /* plcBand corresponds to G3 PHY band 
                 * [0x01: CEN-A, 0x02: FCC, 0x03: ARIB, 0x04: CEN-B] */
                switch (plcBand) 
                {
                    case 0x01:
                        APP_CONSOLE_Print("CENELEC-A band: 35 - 91 kHz\r\n");
                        break;

                    case 0x02:
                        APP_CONSOLE_Print("FCC band: 154 - 488 kHz\r\n");
                        break;

                    case 0x03:
                        APP_CONSOLE_Print("ARIB band: 154 - 404 kHz\r\n");
                        break;

                    case 0x04:
                        APP_CONSOLE_Print("CENELEC-B band: 98 - 122 kHz\r\n");
                        break;

                    default:
                        APP_CONSOLE_Print("G3 PLC Critical Error!!!! Unknown band\r\n");
                        while(1);
                        break;
                }
                
                /* Update App to next state */
                appPlc.state = APP_PLC_STATE_TX_CONFIG;
            }                
            break;
        }

        case APP_PLC_STATE_TX_CONFIG:
        {
            /* Apply PLC coupling configuration */
            if (appPlc.couplingConfig)
            {
                APP_PLC_SetCouplingConfiguration();
            }
            
            /* Update App to next state */
            appPlc.state = APP_PLC_STATE_WAITING;
            
            break;
        }

        case APP_PLC_STATE_TX:
        {
            /* Waiting TX CFM */
            break;
        }

        case APP_PLC_STATE_WAITING:
        {
            break;
        }

        case APP_PLC_STATE_SET_MODULATION:
        {
            /* Clear Transmission flags */
            appPlc.inTx = 0;
            appPlc.waitingTxCfm = 0;
            
            /* Close PLC Driver */
            DRV_G3_MACRT_Close(appPlc.drvG3MacRtHandle);
            
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

void APP_PLC_SendMessage( uint8_t *pData, uint16_t length ) 
{
    /* Update App to TX state */
    appPlc.state = APP_PLC_STATE_TX;
    
    APP_CONSOLE_Print("\r\nTx (%u bytes): ", length + sizeof(APP_G3_MAC_HEADER));
    
    /* G3 MAC RT TX Configuration */
    APP_PLC_SetG3MacRtConfiguration();
    
    /* Send MLME Set to fix TX parameters */
    DRV_G3_MACRT_MlmeSet(appPlc.drvG3MacRtHandle, &appPlc.g3MacRtTxParameters);
    
    /* Build G3 MAC RT Header */
    APP_PLC_BuildG3MacHeader();
            
    /* Build G3 MAC RT Payload */
    memcpy(appPlc.pG3MacRtTxPayload, pData, length);
    
    /* Send G3 MAC RT message. Length: HDR + DATA */
    DRV_G3_MACRT_Send(appPlc.drvG3MacRtHandle, appPlc.pPlcTxData,
            length + sizeof(APP_G3_MAC_HEADER)); 
    
    /* Update Sequence Number */
    appPlc.sequenceNumber++;
}



/*******************************************************************************
 End of File
 */
