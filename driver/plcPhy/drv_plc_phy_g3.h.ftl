/*******************************************************************************
  PLC Driver G3 Definitions Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_plc_phy_comm.h

  Summary:
    PLC Driver G3 Definitions Header File

  Description:
    This file provides implementation-specific definitions for the PLC
    driver's system G3 interface.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
//DOM-IGNORE-END

#ifndef DRV_PLC_PHY_COMM_H
#define DRV_PLC_PHY_COMM_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <device.h>


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

<#if DRV_PLC_BIN_STATIC_ADDRESSING == false>
// *****************************************************************************
// *****************************************************************************
// Section: External Data
// *****************************************************************************
// *****************************************************************************

/* PLC Binary file addressing */
extern uint8_t plc_phy_bin_start;
extern uint8_t plc_phy_bin_end;
extern uint8_t plc_phy_bin2_start;
extern uint8_t plc_phy_bin2_end;

</#if>
// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

/* G3-PLC Bandplan */
#define G3_CEN_A                                   0U
#define G3_CEN_B                                   1U
#define G3_FCC                                     2U
#define G3_ARIB                                    3U
#define G3_INVALID                                 0xFFU

/* Number of carriers for CENELEC-A bandplan */
#define NUM_CARRIERS_CENELEC_A                     36U
/* Number of carriers for CENELEC-B bandplan */
#define NUM_CARRIERS_CENELEC_B                     16U
/* Number of carriers for FCC bandplan */
#define NUM_CARRIERS_FCC                           72U
/* Number of carriers for ARIB bandplan */
#define NUM_CARRIERS_ARIB                          54U

/* Subbands for CENELEC-A bandplan */
#define NUM_SUBBANDS_CENELEC_A                     6U
/* Subbands for CENELEC-B bandplan */
#define NUM_SUBBANDS_CENELEC_B                     4U
/* Subbands for FCC bandplan */
#define NUM_SUBBANDS_FCC                           24U
/* Subbands for ARIB bandplan */
#define NUM_SUBBANDS_ARIB                          16U

/* CENELEC A Band Plan (35 - 91 Khz) */
#define PLC_CENELEC_A                              0U
/* CENELEC-B Band Plan (98 - 122 Khz) */
#define PLC_CENELEC_B                              1U
/* FCC Band Plan (155 - 487 Khz) */
#define PLC_FCC                                    2U
/* ARIB Band Plan (155 - 404 Khz) */
#define PLC_ARIB                                   3U

/* Tone Map size for CENELEC-A/B bandplan */
#define TONE_MAP_SIZE_CENELEC                      1U
/* Tone Map size for FCC bandplan */
#define TONE_MAP_SIZE_FCC                          3U
/* Tone Map size for ARIB bandplan */
#define TONE_MAP_SIZE_ARIB                         3U
/* Maximum number of protocol carriers */
#define PROTOCOL_CARRIERS_MAX                      NUM_CARRIERS_FCC
/* Maximum number of tone map */
#define TONE_MAP_SIZE_MAX                          TONE_MAP_SIZE_FCC
/* Maximum number of subbands */
#define NUM_SUBBANDS_MAX                           NUM_SUBBANDS_FCC

/* TX Mode: Forced transmission */
#define TX_MODE_FORCED                             (1U << 0)
/* TX Mode: Absolute transmission */
#define TX_MODE_ABSOLUTE                           (0U << 1)
/* TX Mode: Delayed transmission */
#define TX_MODE_RELATIVE                           (1U << 1)
/* TX Mode: SYNCP Continuous transmission */
#define TX_MODE_SYNCP_CONTINUOUS                   (1U << 2)
/* TX Mode: Symbols Continuous transmission */
#define TX_MODE_SYMBOLS_CONTINUOUS                 (1U << 3)
/* TX Mode: Cancel transmission */
#define TX_MODE_CANCEL                             (1U << 4)

/* Impedance Configuration: High mode */
#define HI_STATE                                   0x00U
/* Impedance Configuration: Low mode */
#define LOW_STATE                                  0x01U
/* Impedance Configuration: Very Low mode */
#define VLO_STATE                                  0x02U

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* G3-PLC PHY Information Base (PIBs)

  Summary:
    The list of all available PIB attributes.

  Description:
    The G3-PLC PHY layer supports all the mandatory attributes of the PHY
    Information Base (PIB) defined in the G3-PLC specification. In addition,
    Microchip has added several proprietary PIB attributes to support extra
    functionalities.

    The list of all available PIB attributes can be found in this data type.

  Remarks:
    None.
*/

typedef enum {
  PLC_ID_HOST_DESCRIPTION_ID = 0x0100,
  PLC_ID_HOST_MODEL_ID  = 0x010A,
  PLC_ID_HOST_PHY_ID = 0x010C,
  PLC_ID_HOST_PRODUCT_ID = 0x0110,
  PLC_ID_HOST_VERSION_ID = 0x0112,
  PLC_ID_HOST_BAND_ID = 0x0116,
  PLC_ID_TIME_REF_ID = 0x0200,
  PLC_ID_PRODID = 0x4000,
  PLC_ID_MODEL,
  PLC_ID_VERSION_STR,
  PLC_ID_VERSION_NUM,
  PLC_ID_TONE_MASK,
  PLC_ID_TONE_MAP_RSP_DATA,
  PLC_ID_TX_TOTAL,
  PLC_ID_TX_TOTAL_BYTES,
  PLC_ID_TX_TOTAL_ERRORS,
  PLC_ID_TX_BAD_BUSY_TX,
  PLC_ID_TX_BAD_BUSY_CHANNEL,
  PLC_ID_TX_BAD_LEN,
  PLC_ID_TX_BAD_FORMAT,
  PLC_ID_TX_TIMEOUT,
  PLC_ID_RX_TOTAL,
  PLC_ID_RX_TOTAL_BYTES,
  PLC_ID_RX_RS_ERRORS,
  PLC_ID_RX_EXCEPTIONS,
  PLC_ID_RX_BAD_LEN,
  PLC_ID_RX_BAD_CRC_FCH,
  PLC_ID_RX_FALSE_POSITIVE,
  PLC_ID_RX_BAD_FORMAT,
  PLC_ID_ENABLE_AUTO_NOISE_CAPTURE,
  PLC_ID_TIME_BETWEEN_NOISE_CAPTURES,
  PLC_ID_DELAY_NOISE_CAPTURE_AFTER_RX,
  PLC_ID_RRC_NOTCH_ACTIVE,
  PLC_ID_RRC_NOTCH_INDEX,
  PLC_ID_NOISE_PEAK_POWER,
  PLC_ID_CRC_TX_RX_CAPABILITY,
  PLC_ID_RX_BAD_CRC_PAY,
  PLC_ID_CFG_AUTODETECT_IMPEDANCE,
  PLC_ID_CFG_IMPEDANCE,
  PLC_ID_ZC_PERIOD,
  PLC_ID_FCH_SYMBOLS,
  PLC_ID_TX_PAY_SYMBOLS,
  PLC_ID_RX_PAY_SYMBOLS,
  PLC_ID_RRC_NOTCH_AUTODETECT,
  PLC_ID_MAX_RMS_TABLE_HI,
  PLC_ID_MAX_RMS_TABLE_VLO,
  PLC_ID_THRESHOLDS_TABLE_HI,
  PLC_ID_THRESHOLDS_TABLE_LO,
  PLC_ID_THRESHOLDS_TABLE_VLO,
  PLC_ID_PREDIST_COEF_TABLE_HI,
  PLC_ID_PREDIST_COEF_TABLE_LO,
  PLC_ID_PREDIST_COEF_TABLE_VLO,
  PLC_ID_GAIN_TABLE_HI,
  PLC_ID_GAIN_TABLE_LO,
  PLC_ID_GAIN_TABLE_VLO,
  PLC_ID_DACC_TABLE_CFG,
  PLC_ID_RSV0,
  PLC_ID_NUM_TX_LEVELS,
  PLC_ID_CORRECTED_RMS_CALC,
  PLC_ID_RRC_NOTCH_THR_ON,
  PLC_ID_RRC_NOTCH_THR_OFF,
  PLC_ID_CURRENT_GAIN,
  PLC_ID_ZC_CONF_INV,
  PLC_ID_ZC_CONF_FREQ,
  PLC_ID_ZC_CONF_DELAY,
  PLC_ID_NOISE_PER_CARRIER,
  PLC_ID_SYNC_XCORR_THRESHOLD,
  PLC_ID_SYNC_XCORR_PEAK_VALUE,
  PLC_ID_SYNC_SYNCM_THRESHOLD,
  PLC_ID_TONE_MAP_RSP_ENABLED_MODS,
  PLC_ID_PPM_CALIB_ON,
  PLC_ID_SFO_ESTIMATION_LAST_RX,
  PLC_ID_PDC_LAST_RX,
  PLC_ID_MAX_PSDU_LEN_PARAMS,
  PLC_ID_MAX_PSDU_LEN,
  PLC_ID_RESET_STATS,
  PLC_ID_IC_DRIVER_CFG,
  PLC_ID_RX_CHN_EST_REAL,
  PLC_ID_RX_CHN_EST_IMAG,
  PLC_ID_TX_DISABLE,
  PLC_ID_TX_HIGH_TEMP_120,
  PLC_ID_TX_CANCELLED,
  PLC_ID_ZC_VALUE,
  PLC_ID_PDC_RX_LAST_RX,
  PLC_ID_PDC_ZC_TIME_LAST_RX,
  PLC_ID_PDC_ZC_PERIOD_LAST_RX,
  PLC_ID_PREAMBLE_NUM_SYNCP,
  PLC_ID_END_ID,
} DRV_PLC_PHY_ID;

// *****************************************************************************
/* G3-PLC Modulation types

  Summary:
    The list of all types of modulation supported by G3-PLC spec.

  Remarks:
    None.
*/
typedef enum {
  MOD_TYPE_BPSK = 0,
  MOD_TYPE_QPSK = 1,
  MOD_TYPE_8PSK = 2,
  MOD_TYPE_BPSK_ROBO = 4,
}DRV_PLC_PHY_MOD_TYPE;

// *****************************************************************************
/* G3-PLC Modulation schemes

  Summary:
    The list of all modulation schemes supported by G3-PLC spec.

  Remarks:
    None.
*/
typedef enum {
  MOD_SCHEME_DIFFERENTIAL = 0,
  MOD_SCHEME_COHERENT = 1,
}DRV_PLC_PHY_MOD_SCHEME;

// *****************************************************************************
/* G3-PLC Frame Delimiter Types

  Summary:
    The list of all delimiter types supported by G3-PLC spec.

  Remarks:
    None.
*/
typedef enum {
  DT_SOF_NO_RESP = 0,
  DT_SOF_RESP = 1,
  DT_ACK = 2,
  DT_NACK = 3,
}DRV_PLC_PHY_DEL_TYPE;

/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 5.2 deviated once.  Deviation record ID - H3_MISRAC_2012_R_5_2_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block deviate "MISRA C-2012 Rule 5.2" "H3_MISRAC_2012_R_5_2_DR_1"
</#if>

// *****************************************************************************
/* G3-PLC Result values of a previous transmission

  Summary:
    This list provides all available transimission results in MCHP
    implementation.

  Remarks:
    None.
*/
typedef enum {
  /* Transmission result: already in process */
  DRV_PLC_PHY_TX_RESULT_PROCESS = 0,
  /* Transmission result: end successfully */
  DRV_PLC_PHY_TX_RESULT_SUCCESS = 1,
  /* Transmission result: invalid length error */
  DRV_PLC_PHY_TX_RESULT_INV_LENGTH = 2,
  /* Transmission result: busy channel error */
  DRV_PLC_PHY_TX_RESULT_BUSY_CH = 3,
  /* Transmission result: busy in transmission error */
  DRV_PLC_PHY_TX_RESULT_BUSY_TX = 4,
  /* Transmission result: busy in reception error */
  DRV_PLC_PHY_TX_RESULT_BUSY_RX = 5,
  /* Transmission result: invalid modulation scheme error */
  DRV_PLC_PHY_TX_RESULT_INV_SCHEME = 6,
  /* Transmission result: timeout error */
  DRV_PLC_PHY_TX_RESULT_TIMEOUT = 7,
  /* Transmission result: invalid tone map error */
  DRV_PLC_PHY_TX_RESULT_INV_TONEMAP = 8,
  /* Transmission result: invalid modulation type error */
  DRV_PLC_PHY_TX_RESULT_INV_MODTYPE = 9,
  /* Transmission result: invalid delimiter type error */
  DRV_PLC_PHY_TX_RESULT_INV_DT = 10,
  /* Transmission result: transmission cancelled */
  DRV_PLC_PHY_TX_CANCELLED = 11,
<#if DRV_PLC_MODE == "PL460">
  /* Transmission result: high temperature error */
  DRV_PLC_PHY_TX_RESULT_HIGH_TEMP_120 = 12,
</#if>
<#if DRV_PLC_MODE == "PL460" && DRV_PLC_THERMAL_MONITOR == true>
  /* Transmission result: high temperature warning */
  DRV_PLC_PHY_TX_RESULT_HIGH_TEMP_110 = 13,
</#if>
  /* Transmission result: No transmission ongoing */
  DRV_PLC_PHY_TX_RESULT_NO_TX = 255,
}DRV_PLC_PHY_TX_RESULT;

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 5.2"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRA C-2012 deviation block end */

// *****************************************************************************
/* G3-PLC Tone map response data

  Summary:
    This struct includes modulation type, modulation scheme and Tone Map data.

  Remarks:
    For more information about Tone Map Response functionality, please refer to
    G3-PLC Specification.
*/
typedef struct {
  /* Modulation type */
  DRV_PLC_PHY_MOD_TYPE modType;
  /* Modulation scheme */
  DRV_PLC_PHY_MOD_SCHEME modScheme;
  /* Tone Map */
  uint8_t toneMap[TONE_MAP_SIZE_MAX];
} DRV_PLC_PHY_TONE_MAP_RSP;

// *****************************************************************************
/* G3-PLC maximum PSDU length parameters data

  Summary:
    This struct includes the parameters used for the maximum PSDU length
    computation.

  Remarks:
    This struct is related to PLC_ID_MAX_PSDU_LEN_PARAMS.
*/
typedef struct {
  /* Modulation type */
  DRV_PLC_PHY_MOD_TYPE modType;
  /* Modulation scheme */
  DRV_PLC_PHY_MOD_SCHEME modScheme;
  /* Flag to indicate whether 2 Reed-Solomon blocks have to be used (only used for FCC) */
  uint8_t rs2Blocks;
  /* Tone Map */
  uint8_t toneMap[TONE_MAP_SIZE_MAX];
} DRV_PLC_PHY_MAX_PSDU_LEN_PARAMS;

// *****************************************************************************
/* G3-PLC Transmission parameters data

  Summary:
    This struct includes all the parameters needed to request a G3-PLC PHY
    transmission.

  Remarks:
    None.
*/
typedef struct __attribute__((packed, aligned(1))) {
  /* Pointer to data buffer to transmit */
  uint8_t *pTransmitData;
  /* Instant when transmission has to start referred to 1us PHY counter */
  uint32_t timeIni;
  /* Length of the data to transmit in bytes */
  uint16_t dataLength;
  /* Preemphasis for transmission */
  uint8_t preemphasis[NUM_SUBBANDS_MAX];
  /* Tone Map to use on transmission */
  uint8_t toneMap[TONE_MAP_SIZE_MAX];
  /* Transmission Mode (absolute, relative, forced, continuous, cancel). Constants above */
  uint8_t mode;
  /* Power to transmit */
  uint8_t attenuation;
  /* Phase Detector Counter */
  uint8_t pdc;
  /* Flag to indicate whether 2 Reed-Solomon blocks have to be used (only used for FCC) */
  uint8_t rs2Blocks;
  /* Modulation type */
  DRV_PLC_PHY_MOD_TYPE modType;
  /* Modulation scheme */
  DRV_PLC_PHY_MOD_SCHEME modScheme;
  /* DT field to be used in header */
  DRV_PLC_PHY_DEL_TYPE delimiterType;
} DRV_PLC_PHY_TRANSMISSION_OBJ;

// *****************************************************************************
/* G3-PLC Result of a transmission

  Summary:
    This struct includes all the parameters provided in transmission confirm.

  Remarks:
    None.
*/
typedef struct {
  /* Instant when frame transmission ended referred to 1us PHY counter */
  uint32_t timeEnd;
  /* RMS_CALC it allows to estimate tx power injected */
  uint32_t rmsCalc;
  /* Tx Result (see "TX Result values" above) */
  DRV_PLC_PHY_TX_RESULT result;
} DRV_PLC_PHY_TRANSMISSION_CFM_OBJ;

// *****************************************************************************
/* G3-PLC Reception parameters

  Summary:
    This struct includes all the parameters provided for a received message.

  Remarks:
    None.
*/
typedef struct __attribute__((packed, aligned(1))) {
   /* Pointer to received data buffer */
  uint8_t *pReceivedData;
  /* Instant when frame was received (end of message) referred to 1us PHY counter */
  uint32_t timeEnd;
  /* Frame duration referred to 1us PHY counter (Preamble + FCH + Payload) */
  uint32_t frameDuration;
  /* Length of the received data in bytes */
  uint16_t dataLength;
  /* Reception RSSI in dBuV */
  uint16_t rssi;
  /* ZCT info */
  uint8_t zctDiff;
  /* Errors corrected by Reed-Solomon */
  uint8_t rsCorrectedErrors;
  /* Modulation type */
  DRV_PLC_PHY_MOD_TYPE modType;
  /* Modulation scheme */
  DRV_PLC_PHY_MOD_SCHEME modScheme;
  /* DT field coming in header */
  DRV_PLC_PHY_DEL_TYPE delimiterType;
  /* MAC CRC. 1: OK; 0: BAD; 0xFE: Timeout Error; 0xFF: CRC capability disabled (PLC_ID_CRC_TX_RX_CAPABILITY) */
  uint8_t crcOk;
  /* Test data information */
  uint16_t agcFine;
  /* Test data information */
  uint32_t agcFactor;
  /* Test data information */
  int16_t agcOffsetMeas;
  /* Test data information */
  uint8_t agcActive;
  /* Test data information */
  uint8_t agcPgaValue;
  /* Test data information */
  int16_t snrFch;
  /* Test data information */
  int16_t snrPay;
  /* Number of corrupted carriers */
  uint16_t payloadCorruptedCarriers;
  /* Number of noised symbols */
  uint16_t payloadNoisedSymbols;
  /* SNR of the worst carrier */
  uint8_t payloadSnrWorstCarrier;
  /* SNR of the worst symbol */
  uint8_t payloadSnrWorstSymbol;
  /* SNR of impulsive noise */
  uint8_t payloadSnrImpulsive;
   /* SNR of Narrowband noise */
  uint8_t payloadSnrBand;
  /* Background SNR */
  uint8_t payloadSnrBackground;
  /* Link Quality Indicator */
  uint8_t lqi;
  /* Reception Tone Map */
  uint8_t toneMap[TONE_MAP_SIZE_MAX];
  /* SNR per carrier */
  uint8_t carrierSnr[PROTOCOL_CARRIERS_MAX];
} DRV_PLC_PHY_RECEPTION_OBJ;

// *****************************************************************************
/* G3-PLC PHY Information Base (PIB)

  Summary:
    This struct includes all information to access any defined PIB.

  Remarks:
    None.
*/
typedef struct {
  /* Pointer to PIB data */
  uint8_t *pData;
  /* PLC Information base identification */
  DRV_PLC_PHY_ID id;
  /* Length in bytes of the data information */
  uint16_t length;
} DRV_PLC_PHY_PIB_OBJ;

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // #ifndef DRV_PLC_PHY_COMM_H

/*******************************************************************************
 End of File
*/
