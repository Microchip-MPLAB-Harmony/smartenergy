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
#include "app_console.h"
#include "service/pcrc/srv_pcrc.h"
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
/* TX & Coupling parameters for each channel

  Summary:
    Holds PLC TX & Coupling configuration data

  Description:
    These structures hold the PLC TX & Coupling configuration data.
    - plcMaxRMSHigh: Target RMS_CALC value in High impedance mode when dynamic
      gain is enabled (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2). For each
      channel, there are 8 values, corresponding to first 8 TX attenuation
      levels (1 dB steps).
    - plcMaxRMSVeryLow: Target RMS_CALC value in Very Low impedance mode when
      dynamic gain is enabled (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2). For
      each channel, there are 8 values, corresponding to first 8 TX attenuation
      levels (1 dB steps).
    - plcThresholdsHigh: Thresholds to change impedance mode
      (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1) from High impedance mode.
      For each channel there are 16 values. First 8 values (one per TX level)
      are thresholds to change from High to Low. (0 to disable). Next 8 values
      (one per TX level) are thresholds to change from High to Very Low.
      When RMS_CALC is below threshold, impedance mode changes to Very Low.
    - plcThresholdsVeryLow: Thresholds to change impedance mode
      (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1) from Very Low impedance mode.
      For each channel there are 16 values. First 8 values (one per TX level)
      are thresholds to change from High to Low (0 to disable). Next 8 values
      (one per TX level) are thresholds to change from High to Very Low. When
      RMS_CALC is below threshold, impedance mode changes to Very Low.
    - plcGainHighChn1: [Channel 1] Gain values for High and Very Low modes.
      There are three values: {GAIN_INI, GAIN_MIN, GAIN_MAX}.
    - plcGainHighChn2_8: [Channel 2 to 8] Gain values for High and Very Low
      modes. There are three values: {GAIN_INI, GAIN_MIN, GAIN_MAX}.
    - plcPredistorsionVeryLow1: [Channel 1] Equalization values for Very Low
      mode. Specific gain for each carrier to equalize transmission and
      compensate HW filter frequency response.
    - plcPredistorsionVeryLow2_8: [Channel 2 to 8] Equalization values for Very
      Low mode. Specific gain for each carrier to equalize transmission and
      compensate HW filter frequency response.
    - plcDaccConfiguration1: [Channel 1] DACC Configuration.
    - plcDaccConfiguration2_8: [Channel 2 to 8] DACC Configuration.

  Remarks:
    The values are defined in the "user.h" file. They have been calibrated for
    MCHP reference designs. For other Hardware designs, it may be needed to
    calibrate and obtain your own values. MCHP PHY Calibration Tool should be
    used. The maximum number of levels is 8.
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

static const uint16_t plcEquHighChn1[97] = PREDIST_HI_TABLE_CH1;
static const uint16_t plcEquHighChn2_8[97] = PREDIST_HI_TABLE_CH2_8;

static const uint16_t plcEquVeryLowChn1[97] = PREDIST_VLO_TABLE_CH1;
static const uint16_t plcEquVeryLowChn2_8[97] = PREDIST_VLO_TABLE_CH2_8;

static const uint32_t plcDaccConfigChn1[17] = DACC_CFG_TABLE_CH1;
static const uint32_t plcDaccConfigChn2_8[17] = DACC_CFG_TABLE_CH2_8;


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
    uint16_t dataLen;

    /* Avoid warning */
    (void)context;

    /* Start Timer: LED blinking for each received message */
    LED_On();
    SYS_TIME_CallbackRegisterMS(_APP_PLC_TimerLedDataIndCb, 0,
            LED_BLINK_PLC_MSG_MS, SYS_TIME_SINGLE);

    /* Get data length in bytes */
	dataLen = indObj->dataLength;

    if (dataLen <= 4)
    {
		/* Length error. Data length should be at least 5 bytes
         * (1 data byte + 4 CRC bytes) */
		APP_CONSOLE_HandleRxMsgCrcBad();
	}
    else
    {
        uint8_t *pData;
        uint8_t rssi;
        uint8_t cinrAvg;
        DRV_PLC_PHY_SCH scheme;
        uint32_t crc32Calc;
        uint32_t crc32Received;

		/* Discount 4 bytes corresponding to 32-bit CRC */
		dataLen -= 4;

		/* Get pointer to buffer containing received data */
		pData = indObj->pReceivedData;

		/* Compute PRIME 32-bit CRC. Use PCRC service */
		crc32Calc = SRV_PCRC_GetValue(pData, dataLen, PCRC_HT_USI,
                PCRC_CRC32, 0);

		/* Get CRC from last 4 bytes of the message */
		crc32Received = pData[dataLen + 3];
		crc32Received += (uint32_t)pData[dataLen + 2] << 8;
		crc32Received += (uint32_t)pData[dataLen + 1] << 16;
		crc32Received += (uint32_t)pData[dataLen] << 24;

		/* Check integrity of received message comparing the computed CRC with
         * the received CRC in last 4 bytes */
		if (crc32Calc == crc32Received)
        {
			/* CRC Ok. Get some parameters to show in console.  There are more
             * parameters not used in this example application(see
             * "DRV_PLC_PHY_RECEPTION_OBJ" struct in "drv_plc_phy_comm.h") */

			/* Get Modulation Scheme used in received message */
			scheme = indObj->scheme;

			/* Get RSSI (Received Signal Strength Indicator) in dBuV */
			rssi = indObj->rssiAvg;

			/* Get Averaged CINR (Carrier to Interference-plus-Noise Ratio) in
             * quarters of dB and 10-dB offset (cinrAvg = 0 means -10 dB) */
			cinrAvg = indObj->cinrAvg;

			APP_CONSOLE_HandleRxMsgCrcOk(pData, dataLen, scheme, rssi, cinrAvg);
		}
        else
        {
			/* CRC Error */
			APP_CONSOLE_HandleRxMsgCrcBad();
		}
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

        case DRV_PLC_PHY_TX_RESULT_BUSY_CH:
            /* Transmission aborted because there is a reception in progress
             * (PLC channel is busy). If TX_MODE_FORCED is used in
             * TX parameters, transmission is never aborted by reception */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_BUSY_CH");
            break;

        case DRV_PLC_PHY_TX_RESULT_BUSY_TX:
            /* There is another transmission that has not been transmitted yet
             */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_BUSY_TX");
            break;

        case DRV_PLC_PHY_TX_RESULT_TIMEOUT:
            /* Timeout Error */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_TIMEOUT");
            break;

        case DRV_PLC_PHY_TX_RESULT_INV_BUFFER:
            /* Invalid buffer (bufferId) */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_INV_BUFFER");
            break;

        case DRV_PLC_PHY_TX_RESULT_INV_MODE:
            /* Invalid PRIME mode (or frame type) (uc_mod_type) */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_INV_MODE");
            break;

        case DRV_PLC_PHY_TX_RESULT_CANCELLED:
            /* Transmission cancelled */
            APP_CONSOLE_Print(" DRV_PLC_PHY_TX_RESULT_CANCELLED");
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
    APP_CONSOLE_Print("PHY version: %.*s\r\n", 11, versionStr);

    /* Get PLC PHY version (hex) */
    pibObj.id = PLC_ID_VERSION_NUM;
    pibObj.length = 4;
    pibObj.pData = versionNum;
    DRV_PLC_PHY_PIBGet(appPlcData.drvPlcHandle, &pibObj);

    /* versionNum[2] corresponds to protocol [0x05: PRIME] */
    if (versionNum[2] != 0x05)
    {
        APP_CONSOLE_Print("ERROR: PHY band does not match with band configured "
                "in application\r\n");
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
}

static void _APP_PLC_SetCouplingConfiguration(void)
{
    /* Configure Coupling Parameters. Default values defined in "user.h" have
     * been calibrated for MCHP reference designs. For other Hardware designs,
     * it may be needed to calibrate and obtain your own values. MCHP PHY
     * Calibration Tool should be used. Here, all parameters related to the
     * coupling are configured. The user can customize it depending on the
     * requirements. */
    DRV_PLC_PHY_PIB_OBJ pibObj;
    uint8_t pibValue;
    uint8_t chnIndex;

    /* Adjust your own parameters : PLC_ID_NUM_TX_LEVELS. 1 byte */
    pibValue = NUM_TX_LEVELS;
    pibObj.id = PLC_ID_NUM_TX_LEVELS;
    pibObj.length = 1;
    pibObj.pData = &pibValue;
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

    /* Get channel index for tables */
    chnIndex = appPlcData.plcChannel - 1;
    if (chnIndex > 7)
    {
        /* Protection: Error in PLC Channel */
        return;
    }

    /* Adjust your own parameters : PLC_ID_MAX_RMS_TABLE_HI. 32 bytes */
    pibObj.id = PLC_ID_MAX_RMS_TABLE_HI;
    pibObj.length = NUM_TX_LEVELS << 2;
    pibObj.pData = (uint8_t *)plcMaxRMSHigh[chnIndex];
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

    /* Adjust your own parameters : PLC_ID_MAX_RMS_TABLE_VLO. 32 bytes */
    pibObj.id = PLC_ID_MAX_RMS_TABLE_VLO;
    pibObj.length = NUM_TX_LEVELS << 2;
    pibObj.pData = (uint8_t *)plcMaxRMSVeryLow[chnIndex];
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

    /* Adjust your own parameters : PLC_ID_THRESHOLDS_TABLE_HI. 64 bytes */
    pibObj.id = PLC_ID_THRESHOLDS_TABLE_HI;
    pibObj.length = NUM_TX_LEVELS << 3;
    pibObj.pData = (uint8_t *)plcThresholdsHigh[chnIndex];
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

    /* Adjust your own parameters : PLC_ID_THRESHOLDS_TABLE_VLO. 64 bytes */
    pibObj.id = PLC_ID_THRESHOLDS_TABLE_VLO;
    pibObj.length = NUM_TX_LEVELS << 3;
    pibObj.pData = (uint8_t *)plcThresholdsVeryLow[chnIndex];
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

    uint16_t const *pEquHi;
    uint16_t const *pEquVlo;
    uint16_t const *pGainHi;
    uint16_t const *pGainVlo;
	uint32_t const *pDaccTbl;

    if (chnIndex >= 1)
    {
		/* Channel 2 - 8 */
		pEquHi = plcEquHighChn2_8;
		pEquVlo = plcEquVeryLowChn2_8;
		pGainHi = plcGainHighChn2_8;
		pGainVlo = plcGainVeryLowChn2_8;
		pDaccTbl = plcDaccConfigChn2_8;
	}
    else
    {
		/* Channel 1 */
		pEquHi = plcEquHighChn1;
		pEquVlo = plcEquVeryLowChn1;
		pGainHi = plcGainHighChn1;
		pGainVlo = plcGainVeryLowChn1;
		pDaccTbl = plcDaccConfigChn1;
	}

    /* Adjust your own parameters : PLC_ID_GAIN_TABLE_HI. 6 bytes */
    pibObj.id = PLC_ID_GAIN_TABLE_HI;
    pibObj.length = 6;
    pibObj.pData = (uint8_t *)pGainHi;
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

    /* Adjust your own parameters : PLC_ID_GAIN_TABLE_VLO. 6 bytes */
    pibObj.id = PLC_ID_GAIN_TABLE_VLO;
    pibObj.length = 6;
    pibObj.pData = (uint8_t *)pGainVlo;
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

    /* Adjust your own parameters : PLC_ID_DACC_TABLE_CFG. 68 bytes */
    pibObj.id = PLC_ID_DACC_TABLE_CFG;
    pibObj.length = 17 << 2;
    pibObj.pData = (uint8_t *)pDaccTbl;
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

    /* Adjust your own parameters : PLC_ID_PREDIST_COEF_TABLE_HI.
     * (97 * 2) bytes */
    pibObj.id = PLC_ID_PREDIST_COEF_TABLE_HI;
    pibObj.length = 97 << 1;
    pibObj.pData = (uint8_t *)pEquHi;
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

    /* Adjust your own parameters : PLC_ID_PREDIST_COEF_TABLE_VLO.
     * (97 * 2) bytes */
    pibObj.id = PLC_ID_PREDIST_COEF_TABLE_VLO;
    pibObj.length = 97 << 1;
    pibObj.pData = (uint8_t *)pEquVlo;
    DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);
}

/*******************************************************************************
  Function:
    uint16_t _APP_PLC_GetMaxPsduLen(void)

  Summary:
    Calculates maximum length in bytes allowed depending on configuration

  Description:
    The maximum data length depends on:
      - Modulation (BPSK Robust, QPSK Roubust, BPSK, QPSK or 8PSK). See
        "DRV_PLC_PHY_SCH" in "drv_plc_phy_comm.h"
      - Frame Type (Type A, Type B or Type BC). See "DRV_PLC_PHY_FRAME_TYPE" in
        "drv_plc_phy_comm.h"
      - Maximum data length allowed by the PLC transceiver PHY Layer (511).

 Returns:
    Maximum length in bytes (uint16_t).

 */

static uint16_t _APP_PLC_GetMaxPsduLen(void)
{
    uint16_t maxMpdu2Len;
	uint16_t maxPsduLen;
	uint8_t mpdu1Len;

	/* Maximum MPDU2 length depending on modulation scheme. Length of data in
     * bytes sent in payload */
	switch (appPlcData.plcTxObj.scheme)
    {
        case SCHEME_DBPSK:
            maxMpdu2Len = 756;
            break;

        case SCHEME_DQPSK:
            maxMpdu2Len = 1512;
            break;

        case SCHEME_D8PSK:
            maxMpdu2Len = 2268;
            break;

        case SCHEME_DBPSK_C:
        case SCHEME_R_DBPSK:
            maxMpdu2Len = 377;
            break;

        case SCHEME_DQPSK_C:
        case SCHEME_R_DQPSK:
            maxMpdu2Len = 755;
            break;

        case SCHEME_D8PSK_C:
            maxMpdu2Len = 1133;
            break;

        default:
            maxMpdu2Len = 0;
            break;
	}

	/* MPDU1 length. Length of data in bytes that is sent in header */
	switch (appPlcData.plcTxObj.frameType)
    {
        case FRAME_TYPE_A:
            mpdu1Len = 7;
            break;

        case FRAME_TYPE_B:
        case FRAME_TYPE_BC:
        default:
            mpdu1Len = 0;
            break;
	}

	/* Maximum data length: Maximum MPDU2 length + MPDU1 length */
	maxPsduLen = maxMpdu2Len + mpdu1Len;

	/* Saturate to maximum data length allowed by the PLC transceiver
     * PHY Layer (511). */
	if (maxPsduLen > 511) {
		maxPsduLen = 511;
	}

	return maxPsduLen;
}

static void _APP_PLC_TxParamsInit(void)
{
    /* Modulation Scheme. See "DRV_PLC_PHY_SCH" in "drv_plc_phy_comm.h" file.
     * Ordered from higher to lower data rate and from higher to lower required
     * SNR (Signal to Noise Ratio): D8PSK, DQPSK, D8PSK_CC, DBPSK, DQPSK_C,
     * DBPSK_C, R_DQPSK, R_DBPSK. Get maximum data length allowed with
     * configured TX Parameters */
	APP_PLC_SetModulation(SCHEME_DBPSK_C);

	/* Transmission Mode. See TX Mode Bit Mask in "drv_plc_phy_comm.h" file.
     * TX_MODE_RELATIVE: Time in relative mode. The message is sent with a delay
     * from the time of TX Request.
     * TX_MODE_ABSOLUTE: Time in absolute mode. The message is sent at the
     * specified time, referred to PLC transceiver internal timer (1 us) */
	appPlcData.plcTxObj.mode = TX_MODE_RELATIVE;

	/* Transmission Forced Mode: If there is a reception in progress at the same
     * time of transmission, the message is transmitted and the reception is
     * aborted. */
	appPlcData.plcTxObj.forced = 1;

	/* Transmission Time in us. Relative or Absolute time (depending on
     * Transmission Mode). TX_MODE_RELATIVE and time = 0: Instantaneous
     * transmission */
	appPlcData.plcTxObj.time = 0;

	/* Set transmission attenuation power. It represents 1dB of signal level
     * attenuation per Unit. 0 value means maximum signal level. */
	appPlcData.plcTxObj.attenuation = 0;

	/* Select buffer. There are two buffers and it is possible to schedule two
     * different transmissions, but both transmissions cannot be overlapped in
     * time. If they overlap, the first one will be transmitted and the second
     * one will be canceled with DRV_PLC_PHY_TX_RESULT_CANCELLED result. */
	appPlcData.plcTxObj.bufferId = TX_BUFFER_0;
}

void APP_PLC_SendMsg(uint8_t *pData, uint16_t dataLen)
{
    uint32_t crc32Calc;

    /* Compute PRIME 32-bit CRC and add to buffer after message. Use PCRC
     * service */
	crc32Calc = SRV_PCRC_GetValue(pData, dataLen, PCRC_HT_USI, PCRC_CRC32, 0);
	pData[dataLen]  = (uint8_t)(crc32Calc >> 24);
	pData[dataLen + 1]  = (uint8_t)(crc32Calc >> 16);
	pData[dataLen + 2]  = (uint8_t)(crc32Calc >> 8);
	pData[dataLen + 3]  = (uint8_t)(crc32Calc);

	/* Set pointer to data buffer in TX Parameters structure */
	appPlcData.plcTxObj.pTransmitData = pData;

    /* Set data length in TX Parameters structure. Add 4 bytes corresponding to
     * CRC. It should be equal or less than Maximum Data Length (see
     * _APP_PLC_GetMaxPsduLen). Otherwise DRV_PLC_PHY_TX_RESULT_INV_LENGTH will
     * be reported in TX Confirm (_APP_PLC_DataCfmCb) */
	appPlcData.plcTxObj.dataLength = dataLen + 4;

	/* Send PLC message. The result will be reported in TX Confirm
     * (_APP_PLC_DataCfmCb) when message is completely sent */
    appPlcData.waitingTxCfm = true;
    APP_CONSOLE_Print("\r\nTx (%u bytes): ", dataLen);
    DRV_PLC_PHY_Send(appPlcData.drvPlcHandle, &appPlcData.plcTxObj);
}

void APP_PLC_SetModulation(DRV_PLC_PHY_SCH scheme)
{
	/* Store Modulation Scheme in TX Parameters structure */
	appPlcData.plcTxObj.scheme = scheme;

    /* Set PRIME mode (Frame type) depending on modulation */
	switch (scheme)
    {
        case SCHEME_DBPSK:
        case SCHEME_DQPSK:
        case SCHEME_D8PSK:
        case SCHEME_DBPSK_C:
        case SCHEME_DQPSK_C:
        case SCHEME_D8PSK_C:
            /* Not robust modulation: supported in both Type A and Type B. Use
             * Type A because it has less overhead. */
            appPlcData.plcTxObj.frameType = FRAME_TYPE_A;
            break;

        case SCHEME_R_DBPSK:
        case SCHEME_R_DQPSK:
        default:
            /* Robust modulation: only supported in Type B */
            appPlcData.plcTxObj.frameType = FRAME_TYPE_B;
            break;
	}

	/* Get maximum data length that can be transmitted with new Modulation */
	appPlcData.maxDataLen = _APP_PLC_GetMaxPsduLen();
}

bool APP_PLC_SetChannel(uint8_t newChannel)
{
    if ((newChannel >= 1) && (newChannel <= 8)) {
		/* Valid channel. Set channel on PLC transceiver */
        DRV_PLC_PHY_PIB_OBJ pibObj;

        pibObj.id = PLC_ID_CHANNEL_CFG;
        pibObj.length = 1;
        pibObj.pData = &newChannel;
        DRV_PLC_PHY_PIBSet(appPlcData.drvPlcHandle, &pibObj);

        /* Store channel in static variable */
		appPlcData.plcChannel = newChannel;

		/* Reconfigure TX and Coupling parameters, according to new configured
         * channel */
		_APP_PLC_SetCouplingConfiguration();

		switch (newChannel)
        {
            case 1:
                APP_CONSOLE_Print("Channel 1 (42 - 89 kHz, CENELEC-A)\r\n");
                break;

            case 2:
                APP_CONSOLE_Print("Channel 2 (97 - 144 kHz, CENELEC-BCD)\r\n");
                break;

            case 3:
                APP_CONSOLE_Print("Channel 3 (151 - 198 kHz, FCC)\r\n");
                break;

            case 4:
                APP_CONSOLE_Print("Channel 4 (206 - 253 kHz, FCC)\r\n");
                break;

            case 5:
                APP_CONSOLE_Print("Channel 5 (261 - 308 kHz, FCC)\r\n");
                break;

            case 6:
                APP_CONSOLE_Print("Channel 6 (315 - 362 kHz, FCC)\r\n");
                break;

            case 7:
                APP_CONSOLE_Print("Channel 7 (370 - 417 kHz, FCC)\r\n");
                break;

            case 8:
                APP_CONSOLE_Print("Channel 8 (425 - 472 kHz, FCC)\r\n");
                break;

            default:
                APP_CONSOLE_Print("Unknown channel)\r\n");
                break;
		}

		return true;
	}
    else
    {
		/* Invalid channel */
		APP_CONSOLE_Print("\r\nInvalid channel: Skipping configuration\r\n");
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

    /* Flag to indicate that we are waiting for a Transmission Confirm */
    appPlcData.waitingTxCfm = false;

    /* PRIME PLC Channel at initialization (defined in "user.h") */
    appPlcData.plcChannel = APP_CONFIG_PRIME_CHANNEL_INI;

    /* PRIME PLC Channel allowed channels (defined in "user.h") */
    appPlcData.plcAllowedChannels = APP_CONFIG_PRIME_ALLOWED_CHANNELS;
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
            /* Open PLC driver */
            appPlcData.drvPlcHandle = DRV_PLC_PHY_Open(DRV_PLC_PHY_INDEX, NULL);

            if (appPlcData.drvPlcHandle != DRV_HANDLE_INVALID)
            {
                APP_CONSOLE_Print("\r\nOpening PLC driver: Loading PHY"
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

            /* Initialize PRIME channel and corresponding TX and Coupling
             * parameters */
            APP_PLC_SetChannel(appPlcData.plcChannel);

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
