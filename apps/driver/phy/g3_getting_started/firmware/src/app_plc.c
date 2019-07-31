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

#include "app_plc.h"
#include "system/system_module.h"
#include <string.h>

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
    This structure should be initialized by the APP_PLC_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_PLC_DATA appPlcData;


// *****************************************************************************
/* Static Notching Data

  Summary:
    Default Static Notching arrays for each band.

  Description:
    These arrays contain the default values for Tone Mask (Static Notching).
    They are only used if APP_CONFIG_STATIC_NOTCHING is true in the "user.h"
    file.

  Remarks:
    Only needed to define the static notching for the band(s) in use.
    They are defined in the "user.h" file.
*/

#ifdef STATIC_NOTCHING_CENA
static const uint8_t staticNotchingCenA[NUM_CARRIERS_CENELEC_A] =
                                                        STATIC_NOTCHING_CENA;
#endif

#ifdef STATIC_NOTCHING_CENB
static const uint8_t staticNotchingCenB[NUM_CARRIERS_CENELEC_B] =
                                                        STATIC_NOTCHING_CENB;
#endif

#ifdef STATIC_NOTCHING_FCC
static const uint8_t staticNotchingFcc[NUM_CARRIERS_FCC] =
                                                        STATIC_NOTCHING_FCC;
#endif

#ifdef STATIC_NOTCHING_ARIB
static const uint8_t staticNotchingArib[NUM_CARRIERS_ARIB] =
                                                        STATIC_NOTCHING_ARIB;
#endif


// *****************************************************************************
/* PLC TX & Coupling configuration data

  Summary:
    Holds the PLC TX & Coupling configuration data for each band

  Description:
    This structure holds the PLC TX & Coupling configuration data for each band.
    It is only used if APP_CONFIG_PLC_COUP is true in the "user.h" file.

  Remarks:
    Only needed to define them for the band(s) in use. The values are defined in
    the "user.h" file. They have been calibrated for MCHP reference designs. For
    other Hardware designs, it may be needed to calibrate and obtain your own
    values. MCHP PHY Calibration Tool should be used.
    The maximum number of levels is 8.
 */

#ifdef DACC_CFG_TABLE_CENA
APP_PLC_COUPLING_DATA const plcCouplingConfigCenA =
{
    .maxRMSHigh = MAX_RMS_HI_TABLE_CENA,
    .maxRMSVeryLow = MAX_RMS_VLO_TABLE_CENA,
    .thresholdHigh = THRESHOLD_HI_TABLE_CENA,
    .thresholdVeryLow = THRESHOLD_VLO_TABLE_CENA,
    .daccConfig = DACC_CFG_TABLE_CENA,
    .predistorsionHigh = PREDIST_HI_TABLE_CENA,
    .predistorsionVeryLow = PREDIST_VLO_TABLE_CENA,
    .gainHigh = GAIN_HI_CENA,
    .gainVeryLow = GAIN_VLO_CENA,
    .numTxLevels = NUM_TX_LEVELS_CENA
};
#endif

#ifdef DACC_CFG_TABLE_CENB
APP_PLC_COUPLING_DATA const plcCouplingConfigCenB =
{
    .maxRMSHigh = MAX_RMS_HI_TABLE_CENB,
    .maxRMSVeryLow = MAX_RMS_VLO_TABLE_CENB,
    .thresholdHigh = THRESHOLD_HI_TABLE_CENB,
    .thresholdVeryLow = THRESHOLD_VLO_TABLE_CENB,
    .daccConfig = DACC_CFG_TABLE_CENB,
    .predistorsionHigh = PREDIST_HI_TABLE_CENB,
    .predistorsionVeryLow = PREDIST_VLO_TABLE_CENB,
    .gainHigh = GAIN_HI_CENB,
    .gainVeryLow = GAIN_VLO_CENB,
    .numTxLevels = NUM_TX_LEVELS_CENB
};
#endif

#ifdef DACC_CFG_TABLE_FCC
APP_PLC_COUPLING_DATA const plcCouplingConfigFcc =
{
    .maxRMSHigh = MAX_RMS_HI_TABLE_FCC,
    .maxRMSVeryLow = MAX_RMS_VLO_TABLE_FCC,
    .thresholdHigh = THRESHOLD_HI_TABLE_FCC,
    .thresholdVeryLow = THRESHOLD_VLO_TABLE_FCC,
    .daccConfig = DACC_CFG_TABLE_FCC,
    .predistorsionHigh = PREDIST_HI_TABLE_FCC,
    .predistorsionVeryLow = PREDIST_VLO_TABLE_FCC,
    .gainHigh = GAIN_HI_FCC,
    .gainVeryLow = GAIN_VLO_FCC,
    .numTxLevels = NUM_TX_LEVELS_FCC
};
#endif

#ifdef DACC_CFG_TABLE_ARIB
APP_PLC_COUPLING_DATA const plcCouplingConfigArib =
{
    .maxRMSHigh = MAX_RMS_HI_TABLE_ARIB,
    .maxRMSVeryLow = MAX_RMS_VLO_TABLE_ARIB,
    .thresholdHigh = THRESHOLD_HI_TABLE_ARIB,
    .thresholdVeryLow = THRESHOLD_VLO_TABLE_ARIB,
    .daccConfig = DACC_CFG_TABLE_ARIB,
    .predistorsionHigh = PREDIST_HI_TABLE_ARIB,
    .predistorsionVeryLow = PREDIST_VLO_TABLE_ARIB,
    .gainHigh = GAIN_HI_ARIB,
    .gainVeryLow = GAIN_VLO_ARIB,
    .numTxLevels = NUM_TX_LEVELS_ARIB
};
#endif

/* External variables needed if Multi-band is enabled. DRV_PLC_PHY_Initialize()
 * is called from application instead from "initialization.c" if Multi-band is
 * enabled. */
#if (APP_CONFIG_PLC_MULTIBAND == true)
/* PLC PHY binaries address */
extern uint8_t plc_phy_bin_start;
extern uint8_t plc_phy_bin_end;
extern uint8_t plc_phy_bin2_start;
extern uint8_t plc_phy_bin2_end;
/* PLC Driver Initialization Data (initialization.c) */
extern DRV_PLC_PHY_INIT drvPlcPhyInitData;
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void _APP_PLC_TimerLedDataIndCb(uintptr_t context)
{
    /* Avoid warning */
    (void)context;

    /* Turn off led to indicate reception of message */
    LED_Off();
}

static void _APP_PLC_ExceptionCb(DRV_PLC_PHY_EXCEPTION exception,
        uintptr_t context)
{
    /* Avoid warning */
    (void)context;

    /* Check the exception type. */
    switch (exception)
    {
        case DRV_PLC_PHY_EXCEPTION_UNEXPECTED_KEY:
            APP_CONSOLE_Print("\r\nDRV_PLC_PHY_EXCEPTION_UNEXPECTED_KEY\r\n");
            break;

        case DRV_PLC_PHY_EXCEPTION_CRITICAL_ERROR:
            APP_CONSOLE_Print("\r\nDRV_PLC_PHY_EXCEPTION_CRITICAL_ERROR\r\n");
            break;

        case DRV_PLC_PHY_EXCEPTION_RESET:
            APP_CONSOLE_Print("\r\nDRV_PLC_PHY_EXCEPTION_RESET\r\n");
            break;

        default:
            APP_CONSOLE_Print("\r\nDRV_PLC_PHY_EXCEPTION: UNKNOWN [%u]\r\n");
            break;
    }

    /* Update application's state machine to wait for binary reload */
    APP_CONSOLE_Print("Reloading PHY binary\r\n");
    appPlcData.state = APP_PLC_STATE_OPEN;
}

static void _APP_PLC_DataIndCb(DRV_PLC_PHY_RECEPTION_OBJ *indObj,
        uintptr_t context)
{
    /* Avoid warning */
    (void)context;

    /* Start Timer: LED blinking for each received message */
    LED_On();
    SYS_TIME_CallbackRegisterMS(_APP_PLC_TimerLedDataIndCb, 0,
            LED_BLINK_PLC_MSG_MS, SYS_TIME_SINGLE);

    /* CRC capability (PLC_ID_CRC_TX_RX_CAPABILITY) is enabled. The CRC
     * correctness is checked by the PLC PHY Layer. The result is reported in
     * crcOk (1: OK; 0: BAD; 0xFE: Timeout Error; 0xFF: CRC capability disabled)
     */
	if (indObj->crcOk == 1) {
		/* CRC Ok. Get some parameters to show in console. There are more
         * parameters not used in this example application (see
         * "DRV_PLC_PHY_RECEPTION_OBJ" in "drv_plc_phy_comm.h") */
        uint8_t *pData;
        uint16_t dataLen;
        uint16_t rssi;
        uint8_t lqi;
        DRV_PLC_PHY_MOD_SCHEME modScheme;
        DRV_PLC_PHY_MOD_TYPE modType;

		/* Get pointer to buffer containing received data */
		pData = indObj->pReceivedData;

		/* Get data length in bytes. 2 bytes corresponding to CRC are already
         * discounted (PLC_ID_CRC_TX_RX_CAPABILITY is enabled) */
		dataLen = indObj->dataLength;

		/* Get Modulation Scheme used in received message */
		modScheme = indObj->modScheme;

		/* Get Modulation Type used in received message */
		modType = indObj->modType;

		/* Get RSSI (Received Signal Strength Indicator) in dBuV */
		rssi = indObj->rssi;

		/* Get LQI (Link Quality Indicator). It is in quarters of dB and 10-dB
         * offset: SNR(dB) = (LQI - 40) / 4 */
		lqi = indObj->lqi;

		APP_CONSOLE_HandleRxMsgCrcOk(pData, dataLen, modScheme, modType, rssi,
                lqi);
	} else {
		/* CRC Error */
		APP_CONSOLE_HandleRxMsgCrcBad();
	}
}

static void _APP_PLC_DataCfmCb(DRV_PLC_PHY_TRANSMISSION_CFM_OBJ *cfmObj,
        uintptr_t context)
{
    /* Avoid warning */
    (void)context;

    switch (cfmObj->result)
    {
        case DRV_PLC_PHY_TX_RESULT_SUCCESS:
            /* PLC message was successfully transmitted */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_SUCCESS");
            break;

        case DRV_PLC_PHY_TX_RESULT_INV_LENGTH:
            /* Data length is invalid */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_INV_LENGTH");
            break;

        case DRV_PLC_PHY_TX_RESULT_INV_SCHEME:
            /* Modulation Scheme is invalid */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_INV_SCHEME");
            break;

        case DRV_PLC_PHY_TX_RESULT_INV_TONEMAP:
            /* Tone Map is invalid */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_INV_TONEMAP");
            break;

        case DRV_PLC_PHY_TX_RESULT_INV_MODTYPE:
            /* Modulation Type is invalid */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_INV_MODTYPE");
            break;

        case DRV_PLC_PHY_TX_RESULT_INV_DT:
            /* Delimiter Type is invalid */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_INV_DT");
            break;

        case DRV_PLC_PHY_TX_RESULT_BUSY_CH:
            /* Transmission aborted because there is a reception in progress
             * (PLC channel is busy). If TX_MODE_FORCED is used in
             * TX parameters, transmission is never aborted by reception */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_BUSY_CH");
            break;

        case DRV_PLC_PHY_TX_RESULT_BUSY_RX:
            /* Transmission aborted because there is a reception in progress
             * (PLC channel is busy). If TX_MODE_FORCED is used in
             * TX parameters, transmission is never aborted by reception */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_BUSY_RX");
            break;

        case DRV_PLC_PHY_TX_RESULT_BUSY_TX:
            /* There is another transmission that has not been transmitted yet */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_BUSY_TX");
            break;

        case DRV_PLC_PHY_TX_RESULT_TIMEOUT:
            /* Timeout Error */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_TIMEOUT");
            break;

        case DRV_PLC_PHY_TX_RESULT_NO_TX:
        default:
            /* No transmission ongoing */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_NO_TX");
            break;
	}

    /* Clear waiting confirm flag */
    appPlcData.waitingTxCfm = false;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

static void _APP_PLC_GetPhyVersion(void)
{
    DRV_PLC_PHY_PIB_OBJ pibObj;
    uint8_t versionNum[4];
    uint8_t versionStr[11];

    /* Get PLC PHY version (string) */
    pibObj.id = PLC_ID_VERSION_STR;
    pibObj.length = 11;
    pibObj.pData = versionStr;
    DRV_PLC_PHY_PIBGet(appPlcData.drvPlcHandle, &pibObj);
    APP_CONSOLE_Print("PHY version: %.*s (", 11, versionStr);

    /* Get PLC PHY version (hex) */
    pibObj.id = PLC_ID_VERSION_NUM;
    pibObj.length = 4;
    pibObj.pData = versionNum;
    DRV_PLC_PHY_PIBGet(appPlcData.drvPlcHandle, &pibObj);

    /* versionNum[2] corresponds to G3 band [0x01: CEN-A, 0x02: FCC,
     * 0x03: ARIB, 0x04: CEN-B]. Set number of carriers, number of sub-bands and
     * Tone Map size depending on the band in use. */

    switch (versionNum[2])
    {
        case 0x01:
            APP_CONSOLE_Print("CENELEC-A band: 35 - 91 kHz)\r\n");
            appPlcData.plcBand = G3_CEN_A;
            appPlcData.numCarriers = NUM_CARRIERS_CENELEC_A;
            appPlcData.numSubbands = NUM_SUBBANDS_CENELEC_A;
            appPlcData.toneMapSize = TONE_MAP_SIZE_CENELEC;
            break;

        case 0x02:
            APP_CONSOLE_Print("FCC band: 154 - 488 kHz)\r\n");
            appPlcData.plcBand = G3_FCC;
            appPlcData.numCarriers = NUM_CARRIERS_FCC;
            appPlcData.numSubbands = NUM_SUBBANDS_FCC;
            appPlcData.toneMapSize = TONE_MAP_SIZE_FCC;
            break;

        case 0x03:
            APP_CONSOLE_Print("ARIB band: 154 - 404 kHz)\r\n");
            appPlcData.plcBand = G3_ARIB;
            appPlcData.numCarriers = NUM_CARRIERS_ARIB;
            appPlcData.numSubbands = NUM_SUBBANDS_ARIB;
            appPlcData.toneMapSize = TONE_MAP_SIZE_FCC;
            break;

        case 0x04:
            APP_CONSOLE_Print("CENELEC-B band: 98 - 122 kHz)\r\n");
            appPlcData.plcBand = G3_CEN_B;
            appPlcData.numCarriers = NUM_CARRIERS_CENELEC_B;
            appPlcData.numSubbands = NUM_SUBBANDS_CENELEC_B;
            appPlcData.toneMapSize = TONE_MAP_SIZE_CENELEC;
            break;

        default:
            APP_CONSOLE_Print("Unknown band)\r\n");
            break;
    }
}

static void _APP_PLC_SetConfiguration(void)
{
    DRV_PLC_PHY_PIB_OBJ pibObj;
    uint8_t pibValue;

    /* This function shows how to configure different PIB parameters on the PLC
     * transceiver. The user can customize it depending on the requirements */

    /* Force Transmission to VERY_LOW mode by default in order to maximize
     * signal level in any case */

    /* Disable autodetect mode */
    pibValue = 0;
    pibObj.id = PLC_ID_CFG_AUTODETECT_IMPEDANCE;
    pibObj.length = 1;
    pibObj.pData = &pibValue;
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

    /* Set VERY_LOW mode */
    pibValue = VLO_STATE;
    pibObj.id = PLC_ID_CFG_IMPEDANCE;
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

    /* Enable CRC calculation for transmission and reception in PLC PHY layer.
     * In Transmission, 16-bit CRC is computed and added to data payload by PHY
     * layer in the PLC transceiver. In Reception, CRC is checked by PHY layer
     * in the PLC transceiver and the result is reported in crcOk field in
     * DRV_PLC_PHY_RECEPTION_OBJ structure. The CRC format is the same that uses
     * the G3-PLC stack, which is described in the IEEE 802.15.4 standard. */
	pibValue = 1;
    pibObj.id = PLC_ID_CRC_TX_RX_CAPABILITY;
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);
}

static void _APP_PLC_SetStaticNotching(void)
{
    /* Example to configure Tone Mask (Static Notching). Each carrier
     * corresponding to the band can be notched (no energy is sent in those
     * carriers). Each carrier is represented by one byte (0: carrier used;
     * 1: carrier notched). By default it is all 0's in the PLC transceiver.
     * The length is the number of carriers corresponding to the band in use
     * (see "drv_plc_phy_comm.h"). The same Tone Mask must be set in both
     * transmitter and receiver, otherwise they don't understand each other */

    if (appPlcData.toneMaskConfig)
    {
        uint8_t const *pToneMask = NULL;
        bool staticNotchingDefined = true;

        /* Select the Tone Mask corresponding to the band in use */
        switch (appPlcData.plcBand)
        {
            case G3_CEN_A:
            {
#ifdef STATIC_NOTCHING_CENA
                pToneMask = staticNotchingCenA;
#else
                APP_CONSOLE_Print("Warning: STATIC_NOTCHING_CENA not defined in"
                        " user.h. ");
                staticNotchingDefined = false;
#endif
                break;
            }

            case G3_CEN_B:
            {
#ifdef STATIC_NOTCHING_CENB
                pToneMask = staticNotchingCenB;
#else
                APP_CONSOLE_Print("Warning: STATIC_NOTCHING_CENB not defined in"
                        " user.h. ");
                staticNotchingDefined = false;
#endif
                break;
            }

            case G3_FCC:
            {
#ifdef STATIC_NOTCHING_FCC
                pToneMask = staticNotchingFcc;
#else
                APP_CONSOLE_Print("Warning: STATIC_NOTCHING_FCC not defined in"
                        " user.h. ");
                staticNotchingDefined = false;
#endif
                break;
            }

            case G3_ARIB:
            {
#ifdef STATIC_NOTCHING_ARIB
                pToneMask = staticNotchingArib;
#else
                APP_CONSOLE_Print("Warning: STATIC_NOTCHING_ARIB not defined in"
                        " user.h. ");
                staticNotchingDefined = false;
#endif
                break;
            }
        }

        if (staticNotchingDefined)
        {
            DRV_PLC_PHY_PIB_OBJ pibObj;

            pibObj.id = PLC_ID_TONE_MASK;
            pibObj.length = appPlcData.numCarriers;
            pibObj.pData = (uint8_t *)pToneMask;
            DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);
        }
        else
        {
            APP_CONSOLE_Print("Skipping static notching configuration\r\n");
        }
    }
}

static void _APP_PLC_SetCouplingConfiguration(void)
{
    /* Configure TX & Coupling Parameters. The default values defined in
     * "user.h" have been calibrated for MCHP reference designs. For other
     * Hardware designs, it may be needed to calibrate and obtain your own
     * values. MCHP PHY Calibration Tool should be used. Here, all parameters
     * related to TX & Coupling are configured. The user can customize it
     * depending on the requirements. */

    if (appPlcData.couplingConfig)
    {
        APP_PLC_COUPLING_DATA const *pCoupData = NULL;
        bool couplingParamsDefined = true;

        /* Select the Coupling parameters corresponding to the band in use */
        switch (appPlcData.plcBand)
        {
            case G3_CEN_A:
            {
#ifdef DACC_CFG_TABLE_CENA
                pCoupData = &plcCouplingConfigCenA;
#else
                APP_CONSOLE_Print("Warning: Coupling Configuration Data for "
                        "CENELEC-A not defined in user.h. ");
                couplingParamsDefined = false;
#endif
                break;
            }

            case G3_CEN_B:
            {
#ifdef DACC_CFG_TABLE_CENB
                pCoupData = &plcCouplingConfigCenB;
#else
                APP_CONSOLE_Print("Warning: Coupling Configuration Data for "
                        "CENELEC-B not defined in user.h. ");
                couplingParamsDefined = false;
#endif
                break;
            }

            case G3_FCC:
            {
#ifdef DACC_CFG_TABLE_FCC
                pCoupData = &plcCouplingConfigFcc;
#else
                APP_CONSOLE_Print("Warning: Coupling Configuration Data for "
                        "FCC not defined in user.h. ");
                couplingParamsDefined = false;
#endif
                break;
            }

            case G3_ARIB:
            {
#ifdef DACC_CFG_TABLE_ARIB
                pCoupData = &plcCouplingConfigArib;
#else
                APP_CONSOLE_Print("Warning: Coupling Configuration Data for "
                        "ARIB not defined in user.h. ");
                couplingParamsDefined = false;
#endif
                break;
            }
        }

        if (couplingParamsDefined)
        {
            DRV_PLC_PHY_PIB_OBJ pibObj;

            /* Adjust your own parameters : PLC_ID_NUM_TX_LEVELS. 1 byte */
            pibObj.id = PLC_ID_NUM_TX_LEVELS;
            pibObj.length = 1;
            pibObj.pData = (uint8_t *)&pCoupData->numTxLevels;
            DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

            /* Adjust your own parameters : PLC_ID_MAX_RMS_TABLE_HI. 32 bytes */
            pibObj.id = PLC_ID_MAX_RMS_TABLE_HI;
            pibObj.length = sizeof(pCoupData->maxRMSHigh);
            pibObj.pData = (uint8_t *)pCoupData->maxRMSHigh;
            DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

            /* Adjust your own parameters : PLC_ID_MAX_RMS_TABLE_VLO. 32 bytes
             */
            pibObj.id = PLC_ID_MAX_RMS_TABLE_VLO;
            pibObj.length = sizeof(pCoupData->maxRMSVeryLow);
            pibObj.pData = (uint8_t *)pCoupData->maxRMSVeryLow;
            DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

            /* Adjust your own parameters : PLC_ID_THRESHOLDS_TABLE_HI. 64 bytes
             */
            pibObj.id = PLC_ID_THRESHOLDS_TABLE_HI;
            pibObj.length = sizeof(pCoupData->thresholdHigh);
            pibObj.pData = (uint8_t *)pCoupData->thresholdHigh;
            DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

            /* Adjust your own parameters : PLC_ID_THRESHOLDS_TABLE_VLO. 64
             * bytes */
            pibObj.id = PLC_ID_THRESHOLDS_TABLE_VLO;
            pibObj.length = sizeof(pCoupData->thresholdVeryLow);
            pibObj.pData = (uint8_t *)pCoupData->thresholdVeryLow;
            DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

            /* Adjust your own parameters : PLC_ID_GAIN_TABLE_HI. 6 bytes */
            pibObj.id = PLC_ID_GAIN_TABLE_HI;
            pibObj.length = sizeof(pCoupData->gainHigh);
            pibObj.pData = (uint8_t *)pCoupData->gainHigh;
            DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

            /* Adjust your own parameters : PLC_ID_GAIN_TABLE_VLO. 6 bytes */
            pibObj.id = PLC_ID_GAIN_TABLE_VLO;
            pibObj.length = sizeof(pCoupData->gainVeryLow);
            pibObj.pData = (uint8_t *)pCoupData->gainVeryLow;
            DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

            /* Adjust your own parameters : PLC_ID_DACC_TABLE_CFG. 68 bytes */
            pibObj.id = PLC_ID_DACC_TABLE_CFG;
            pibObj.length = sizeof(pCoupData->daccConfig);
            pibObj.pData = (uint8_t *)pCoupData->daccConfig;
            DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

            /* Adjust your own parameters : PLC_ID_PREDIST_COEF_TABLE_HI.
             * (numCarriers * 2) bytes */
            pibObj.id = PLC_ID_PREDIST_COEF_TABLE_HI;
            pibObj.length = appPlcData.numCarriers << 1;
            pibObj.pData = (uint8_t *)pCoupData->predistorsionHigh;
            DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

            /* Adjust your own parameters : PLC_ID_PREDIST_COEF_TABLE_VLO.
             * (numCarriers * 2) bytes */
            pibObj.id = PLC_ID_PREDIST_COEF_TABLE_VLO;
            pibObj.length = appPlcData.numCarriers << 1;
            pibObj.pData = (uint8_t *)pCoupData->predistorsionVeryLow;
            DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);
        }
        else
        {
            APP_CONSOLE_Print("Skipping Coupling parameters configuration\r\n");
        }
    }
}

/*******************************************************************************
  Function:
    uint16_t _APP_PLC_GetMaxPsduLen(void)

  Summary:
    Calculates maximum length in bytes allowed depending on configuration

  Description:
    The maximum data length depends on:
      - G3 band (CENELEC-A, CENELEC-B, FCC or ARIB)
      - Modulation Type (BPSK Robust, BPSK, QPSK or 8PSK). See
        "DRV_PLC_PHY_MOD_TYPE" in "drv_plc_phy_comm.h"
      - Modulation Scheme (Differential of coherent). See
        "DRV_PLC_PHY_MOD_SCHEME" in "drv_plc_phy_comm.h"
      - Number of Reed-Solomon blocks (rs2Blocks). Only applies to FCC band.
        1 or 2 blocks. With 2 blocks the maximum length is the double
      - Tone Map (toneMap). Dynamic notching: The sub-bands used to send the
        data can be chosen with Tone Map
      - Tone Mask (PLC_ID_TONE_MASK). Static notching: Carriers can be notched
        and no energy is sent in those carriers
    If CRC capability is enabled (PLC_ID_CRC_TX_RX_CAPABILITY), 2 bytes
    corresponding to CRC are discounted

 Returns:
    Maximum length in bytes (uint16_t).

 */

static uint16_t _APP_PLC_GetMaxPsduLen(void)
{
    DRV_PLC_PHY_MAX_PSDU_LEN_PARAMS maxPsduLenParamsObj;
	uint16_t maxPsduLen;
    DRV_PLC_PHY_PIB_OBJ pibObj;

	/* First, PLC_ID_MAX_PSDU_LEN_PARAMS has to be set with TX message
     * parameters to compute maximum PSDU length. See
     * "DRV_PLC_PHY_MAX_PSDU_LEN_PARAMS" struct in "drv_plc_phy_comm.h" */

	/* Modulation Type */
	maxPsduLenParamsObj.modType = appPlcData.plcTxObj.modType;

	/* Modulation scheme */
	maxPsduLenParamsObj.modScheme = appPlcData.plcTxObj.modScheme;

	/* 1 or 2 Reed-Solomon blocks (only for FCC) */
	if (appPlcData.plcBand == G3_FCC) {
		maxPsduLenParamsObj.rs2Blocks = appPlcData.plcTxObj.rs2Blocks;
	}

	/* Tone Map */
	memset(maxPsduLenParamsObj.toneMap, 0, TONE_MAP_SIZE_MAX);
	memcpy(maxPsduLenParamsObj.toneMap, appPlcData.plcTxObj.toneMap,
            appPlcData.toneMapSize);

	/* Set parameters for MAX_PSDU_LEN computation in the PLC transceiver */
    pibObj.id = PLC_ID_MAX_PSDU_LEN_PARAMS;
    pibObj.length = sizeof(DRV_PLC_PHY_MAX_PSDU_LEN_PARAMS);
    pibObj.pData = (uint8_t *)&maxPsduLenParamsObj;
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

	/* Get MAX_PSDU_LEN from the PLC transceiver */
    pibObj.id = PLC_ID_MAX_PSDU_LEN;
    pibObj.length = 2;
    pibObj.pData = (uint8_t *)&maxPsduLen;
    DRV_PLC_PHY_PIBGet(appPlcData.drvPlcHandle, &pibObj);

	return maxPsduLen;
}

static void _APP_PLC_TxParamsInit(void)
{
    uint8_t subbandInd;
	uint8_t toneMapByteInd;
	uint8_t toneMapBitInd;

	/* Configure Tone Map (Dynamic Notching) to use all carriers. Each bit
     * corresponds to a sub-band. If the bit is '1' the sub-band is used to
     * carry data. If the bit is '0' the subband does not carry data, but energy
     * is sent in those carriers. The number of sub-bands is different in each
     * G3 band (CENELEC-A, CENELEC-B, FCC, ARIB). See "drv_plc_phy_comm.h". The
     * number of carriers per sub-band is 6 for CENELEC-A, 4 for CENELEC-B and
     * 3 for FCC and ARIB. Full Tone Map: 0x3F0000 (CENELEC-A, 6 sub-bands);
     * 0x0F0000 (CENELEC-B, 4 sub-bands); 0xFFFFFF (FCC, 24 sub-bands);
     * 0xFFFF03 (ARIB, 18 sub-bands). The next loop shows how to go across all
     * sub-bands from lower to higher frequency */
	memset(appPlcData.plcTxObj.toneMap, 0, TONE_MAP_SIZE_MAX);
	for (subbandInd = 0; subbandInd < appPlcData.numSubbands; subbandInd++)
    {
		/* Byte index in Tone Map array */
		toneMapByteInd = subbandInd >> 3;

		/* Bit index in Tone Map byte */
		toneMapBitInd = subbandInd - (toneMapByteInd << 3);

		/* Set sub-band active */
		appPlcData.plcTxObj.toneMap[toneMapByteInd] |= (1 << toneMapBitInd);
	}

	/* Number of Reed-Solomon blocks (parameter only used in FCC).
     * "rs2Blocks = 0" means 1 block and "rs2Blocks = 1" means 2 blocks.
     * 2 Reed-Solomon blocks allows to send the double number of bytes in the
     * same frame than 1 Reed-Solomon blocks (see "_APP_PLC_GetMaxPsduLen()").
     * However, if the message can be sent in one single block, it is better to
     * use 1 block because 2 blocks would add overhead */
	if (appPlcData.plcBand == G3_FCC)
    {
		/* Set 1 Reed-Solomon block. In this example it cannot be configured
         * dynamically. To test 2 Reed-Solomon blocks change 0 by 1 */
		appPlcData.plcTxObj.rs2Blocks = 0;
	}

	/* Delimiter Type. Data Frame (requiring ACK or not) or acknowledgement
     * (positive or negative). See "DRV_PLC_PHY_DEL_TYPE" in
     * "drv_plc_phy_comm.h" file. In this example ACK management is not needed
     */
	appPlcData.plcTxObj.delimiterType = DT_SOF_NO_RESP;

	/* Modulation Scheme. Differential or Coherent. See "DRV_PLC_PHY_MOD_SCHEME"
     * in "drv_plc_phy_comm.h" file. Coherent Scheme supports worst SNR
     * (about 3 dB) than Differential Scheme. Differential Scheme provides a bit
     * higher data rate because Coherent Scheme uses some carriers for pilots.
     * Coherent Scheme requires an accurate crystal oscillator. G3-PLC specifies
     *  that the frequency error must be less than 25 PPM */
    appPlcData.plcTxObj.modScheme = MOD_SCHEME_DIFFERENTIAL;

	/* Modulation Type. See "DRV_PLC_PHY_MOD_TYPE" in "drv_plc_phy_comm.h" file
     * Ordered from higher to lower data rate and from higher to lower required
     * SNR (Signal to Noise Ratio): 8PSK, QPSK, BPSK, Robust BPSK */
	appPlcData.plcTxObj.modType = MOD_TYPE_BPSK;

	/* Transmission Mode. See TX Mode Bit Mask in "drv_plc_phy_comm.h" file.
     * TX_MODE_RELATIVE: Time in relative mode. The message is sent with a delay
     * from the time of TX Request.
     * TX_MODE_ABSOLUTE: Time in absolute mode. The message is sent at the
     * specified time, referred to the PLC transceiver internal timer.
     * TX_MODE_FORCED: If there is a reception in progress at the same time of
     * transmission, the message is transmitted and the reception is aborted */
    appPlcData.plcTxObj.mode = TX_MODE_FORCED | TX_MODE_RELATIVE;

	/* Transmission Time in us. Relative or Absolute time (depending on
     * Transmission Mode).
     * TX_MODE_RELATIVE and time = 0: Instantaneous transmission */
	appPlcData.plcTxObj.time = 0;

	/* Set transmission power. It represents 3dBs of signal level attenuation
     * per Unit. 0 value means maximum signal level. */
	appPlcData.plcTxObj.attenuation = 0;

	/* Get maximum data length allowed with configured TX Parameters */
	appPlcData.maxDataLen = _APP_PLC_GetMaxPsduLen();
}

void APP_PLC_SendMsg(uint8_t *pData, uint16_t dataLen)
{
	/* Set pointer to data buffer in TX Parameters structure */
	appPlcData.plcTxObj.pTransmitData = pData;

	/* Set data length in TX Parameters structure. It should be equal or less
     * than Maximum Data Length (see _APP_PLC_GetMaxPsduLen). Otherwise
     * DRV_PLC_PHY_TX_RESULT_INV_LENGTH will be reported in TX Confirm
     * (_APP_PLC_DataCfmCb) */
	appPlcData.plcTxObj.dataLength = dataLen;

	/* Send PLC message. The result will be reported in TX Confirm
     * (_APP_PLC_DataCfmCb) when message is completely sent */
    appPlcData.waitingTxCfm = true;
    APP_CONSOLE_Print("\r\nTx (%u bytes): ", dataLen);
    DRV_PLC_PHY_Send(appPlcData.drvPlcHandle, &appPlcData.plcTxObj);
}

void APP_PLC_SetModulation(DRV_PLC_PHY_MOD_SCHEME modScheme,
        DRV_PLC_PHY_MOD_TYPE modType)
{
	/* Store Modulation Type and Modulation Scheme in TX Parameters structure */
	appPlcData.plcTxObj.modScheme = modScheme;
	appPlcData.plcTxObj.modType = modType;

	/* Get maximum data length that can be transmitted with new Modulation */
	appPlcData.maxDataLen = _APP_PLC_GetMaxPsduLen();
}

bool APP_PLC_SetBand(uint8_t newBand)
{
    if (newBand == appPlcData.plcBand)
    {
        APP_CONSOLE_Print("\r\nBand has not changed: Skipping Band "
                "Reconfiguration\r\n");
        return false;
    }

    if (!appPlcData.plcMultiband)
    {
        APP_CONSOLE_Print("\r\nMulti-band is not supported (see user.h)\r\n");
        return false;
    }

    if ((newBand == G3_CEN_A) || (newBand == G3_FCC))
    {
		/* Reset the PLC transceiver to load new binary */
		DRV_PLC_PHY_Close(appPlcData.drvPlcHandle);

        /* Select the binary corresponding to the new band */
		if (newBand == G3_CEN_A)
        {
            appPlcData.bin2InUse = false;
        }
        else
        {
            appPlcData.bin2InUse = true;
        }

        /* Reset application state machine (load new binary into the PLC
         * transceiver and wait until the new binary is running in the PLC
         * transceiver). */
        appPlcData.state = APP_PLC_STATE_INIT;

		return true;
	} else {
		APP_CONSOLE_Print("Band not supported: Skipping Band Configuration"
                "\r\n");
		return false;
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
    See prototype in app_plc.h.
 */

void APP_PLC_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appPlcData.state = APP_PLC_STATE_INIT;

    /* Enable/Disable Tone Mask (Static Notching) configuration */
    appPlcData.toneMaskConfig = APP_CONFIG_STATIC_NOTCHING;

    /* TX & Enable/Disable Coupling parameters configuration */
    appPlcData.couplingConfig = APP_CONFIG_PLC_COUP;

    /* Enable/Disable Multi-band */
    appPlcData.plcMultiband = APP_CONFIG_PLC_MULTIBAND;

    /* Flag to indicate that we are waiting for a Transmission Confirm */
    appPlcData.waitingTxCfm = false;

    /* Flag to indicate if the second binary has to be used (for multi-band) */
    appPlcData.bin2InUse = false;
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
    switch ( appPlcData.state )
    {
        /* Application's initial state. */
        case APP_PLC_STATE_INIT:
        {
#if (APP_CONFIG_PLC_MULTIBAND == true)
            /* Select PLC Binary file for multi-band solution */
            if (appPlcData.bin2InUse)
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
            sysObj.drvPlcPhy = DRV_PLC_PHY_Initialize(DRV_PLC_PHY_INDEX,
                    (SYS_MODULE_INIT *)&drvPlcPhyInitData);
            /* Register Callback function to handle PLC interruption */
            PIO_PinInterruptCallbackRegister(DRV_PLC_EXT_INT_PIN,
                    DRV_PLC_PHY_ExternalInterruptHandler, sysObj.drvPlcPhy);
#endif

            /* Open PLC driver */
            appPlcData.drvPlcHandle = DRV_PLC_PHY_Open(DRV_PLC_PHY_INDEX, NULL);

            if (appPlcData.drvPlcHandle != DRV_HANDLE_INVALID)
            {
                APP_CONSOLE_Print("\r\nOpening PLC driver: Loading PHY "
                        "binary\r\n");

                /* Next Application state: Wait to PLC binary download */
                appPlcData.state = APP_PLC_STATE_OPEN;
            }
            else
            {
                /* Unexpected error opening the PLC driver */
                APP_CONSOLE_Print("\r\nUnexpected error opening PLC "
                        "driver\r\n");
                appPlcData.state = APP_PLC_STATE_ERROR;
            }

            break;
        }

        /* PLC driver open process in progress (downloading binary) */
        case APP_PLC_STATE_OPEN:
        {
            /* Check PLC driver status */
            SYS_STATUS drvPlcStatus = DRV_PLC_PHY_Status(DRV_PLC_PHY_INDEX);

            if (drvPlcStatus == SYS_STATUS_ERROR)
            {
                /* Error in the binary download process */
                APP_CONSOLE_Print("\r\nCRITICAL ERROR: PHY binary load "
                        "failed\r\n");
                appPlcData.state = APP_PLC_STATE_ERROR;
            }
            else if (drvPlcStatus == SYS_STATUS_READY)
            {
                /* Binary download process is completed and PLC transceiver is
                 * ready */
                APP_CONSOLE_Print("\r\nPLC PHY binary loaded correctly\r\n");

                /* Configure PLC driver callbacks */
                DRV_PLC_PHY_ExceptionCallbackRegister(appPlcData.drvPlcHandle,
                        _APP_PLC_ExceptionCb, DRV_PLC_PHY_INDEX);
                DRV_PLC_PHY_DataIndCallbackRegister(appPlcData.drvPlcHandle,
                        _APP_PLC_DataIndCb, DRV_PLC_PHY_INDEX);
                DRV_PLC_PHY_DataCfmCallbackRegister(appPlcData.drvPlcHandle,
                        _APP_PLC_DataCfmCb, DRV_PLC_PHY_INDEX);

                /* Get PHY version to know the G3-PLC band */
                _APP_PLC_GetPhyVersion();

                /* Next Application state: Configure parameters */
                appPlcData.state = APP_PLC_STATE_PARAM_CONFIG;
            }

            break;
        }

        /* Initialize parameters on PLC transceiver once it is ready after
         * binary download */
        case APP_PLC_STATE_PARAM_CONFIG:
        {
            /* Configure parameters through PIBs */
            _APP_PLC_SetConfiguration();

            /* Configure Static notching through PIB.
             * Only if APP_CONFIG_STATIC_NOTCHING is true */
            _APP_PLC_SetStaticNotching();

            /* Configure Coupling Parameters through PIB.
             * Only if APP_CONFIG_PLC_COUP is true */
            _APP_PLC_SetCouplingConfiguration();

            /* Next Application state: Configure transmission parameters */
            appPlcData.state = APP_PLC_STATE_INIT_TX_PARAMS;

            break;
        }

        /* Initialize transmission parameters */
        case APP_PLC_STATE_INIT_TX_PARAMS:
        {
            /* Setup parameters to use in TX message */
            _APP_PLC_TxParamsInit();

            /* Next Application state: Wait for commands from console */
            appPlcData.state = APP_PLC_STATE_WAITING;

            break;
        }

        /* Wait for commands from console */
        case APP_PLC_STATE_WAITING:
        {
            /* Nothing to do */
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
