/*******************************************************************************
  PLC Driver Meters&More Definitions Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_plc_phy_comm.h

  Summary:
    PLC Driver Meters&More Definitions Header File

  Description:
    This file provides implementation-specific definitions for the PLC
    driver's system Meters&More interface.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2024, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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

</#if>
// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

/* Meters&More Bandplan */
#define MM_CEN_A                                   0U
#define MM_INVALID                                 0xFFU

/* TX Mode: Absolute transmission */
#define TX_MODE_ABSOLUTE                           (0U << 1)
/* TX Mode: Forced transmission */
#define TX_MODE_FORCED                             (1U << 0)
/* TX Mode: Delayed transmission */
#define TX_MODE_RELATIVE                           (1U << 1)
/* TX Mode: Preamble Continuous transmission */
#define TX_MODE_PREAMB_CONTINUOUS                  (1U << 2)
/* TX Mode: Constant Continuous transmission */
#define TX_MODE_CONSTANT_CONTINUOUS                (1U << 3)
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
/* Meters&More PHY Information Base (PIBs)

  Summary:
    The list of all available PIB attributes.

  Description:
    The Meters&More PHY layer supports all the mandatory attributes of the PHY
    Information Base (PIB) defined in the Meters&More specification. In addition,
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
  PLC_ID_TX_TOTAL,
  PLC_ID_TX_TOTAL_BYTES,
  PLC_ID_TX_TOTAL_ERRORS,
  PLC_ID_TX_BAD_BUSY_TX,
  PLC_ID_TX_BAD_BUSY_CHANNEL,
  PLC_ID_TX_BAD_LEN,
  PLC_ID_TX_TIMEOUT,
  PLC_ID_TX_HIGH_TEMP_120,
  PLC_ID_TX_CANCELLED,
  PLC_ID_RX_TOTAL,
  PLC_ID_RX_TOTAL_BYTES,
  PLC_ID_RX_EXCEPTIONS,
  PLC_ID_RX_BAD_LEN,
  PLC_ID_RX_FALSE_POSITIVE,
  PLC_ID_RX_BAD_FORMAT,
  PLC_ID_RX_BAD_CRC_PAY,
  PLC_ID_RESET_STATS,
  PLC_ID_CRC_TX_RX_CAPABILITY,
  PLC_ID_CFG_AUTODETECT_IMPEDANCE,
  PLC_ID_CFG_IMPEDANCE,
  PLC_ID_ZC_PERIOD,
  PLC_ID_ZC_CONF_INV,
  PLC_ID_ZC_CONF_FREQ,
  PLC_ID_ZC_CONF_DELAY,
  PLC_ID_ZC_VALUE,
  PLC_ID_TX_PAY_SYMBOLS,
  PLC_ID_RX_PAY_SYMBOLS,
  PLC_ID_NUM_TX_LEVELS,
  PLC_ID_MAX_RMS_TABLE_HI,
  PLC_ID_MAX_RMS_TABLE_VLO,
  PLC_ID_THRESHOLDS_TABLE_HI,
  PLC_ID_THRESHOLDS_TABLE_LO,
  PLC_ID_THRESHOLDS_TABLE_VLO,
  PLC_ID_GAIN_TABLE_HI,
  PLC_ID_GAIN_TABLE_LO,
  PLC_ID_GAIN_TABLE_VLO,
  PLC_ID_DACC_TABLE_CFG,
  PLC_ID_RSV0,
  PLC_ID_TX_DISABLE,
  PLC_ID_IC_DRIVER_CFG,
  PLC_ID_CORRECTED_RMS_CALC,
  PLC_ID_CURRENT_GAIN,
  PLC_ID_SYNC_XCORR_THRESHOLD,
  PLC_ID_SYNC_XCORR_PEAK_VALUE,
  PLC_ID_PPM_CALIB_ON,
  PLC_ID_SFO_ESTIMATION_LAST_RX,
  PLC_ID_PREAMBLE_LEN_BYTES,
  PLC_ID_END_ID,
} DRV_PLC_PHY_ID;

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
/* Meters&More Result values of a previous transmission

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
  /* Transmission result: timeout error */
  DRV_PLC_PHY_TX_RESULT_TIMEOUT = 7,
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
/* Meters&More Transmission parameters data

  Summary:
    This struct includes all the parameters needed to request a Meters&More PHY
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
  /* Transmission Mode (absolute, relative, forced, continuous, cancel). Constants above */
  uint8_t mode;
  /* Power to transmit */
  uint8_t attenuation;
  /* NB frame */
  uint8_t nbFrame;
} DRV_PLC_PHY_TRANSMISSION_OBJ;

// *****************************************************************************
/* Meters&More Result of a transmission

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
/* Meters&More Reception parameters

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
  /* SNR of the header in quarters of dB (sQ13.2) */
  int16_t snrHeader;
  /* SNR of the payload in quarters of dB (sQ13.2) */
  int16_t snrPayload;
  /* NB frame (ZC info). Difference between last ZC time and payload initial time, scaled to 0-254. Value = 255: ZC not available */
  uint8_t nbRx;
  /* Link Quality Indicator */
  uint8_t lqi;
  /* Reception RSSI in dBuV */
  uint8_t rssi;
  /* MAC CRC. 1: OK; 0: BAD; 0xFE: Timeout Error; 0xFF: CRC capability disabled (PLC_ID_CRC_TX_RX_CAPABILITY) */
  uint8_t crcOk;
} DRV_PLC_PHY_RECEPTION_OBJ;

// *****************************************************************************
/* Meters&More PHY Information Base (PIB)

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
