/*******************************************************************************
  PL360 Driver Communication Definitions Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360_comm.h

  Summary:
    PL360 Driver Communication Definitions Header File

  Description:
    This file provides implementation-specific definitions for the PL360
    driver's system PRIME interface.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
//DOM-IGNORE-END

#ifndef DRV_PL360_COMM_H
#define DRV_PL360_COMM_H

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

/* PRIME Profile type */
typedef enum {
  PRIME = 4,
}DRV_PL360_PROFILE;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/* TX Mode: Absolute transmission */          
#define TX_MODE_ABSOLUTE                       (0)
/* TX Mode: Delayed transmission */          
#define TX_MODE_RELATIVE                       (1 << 0)
/* TX Mode: Cancel transmission */
#define TX_MODE_CANCEL                         (1 << 1)
/* TX Mode: SYNCP Continuous transmission */
#define TX_MODE_PREAMBLE_CONTINUOUS            (1 << 2)
/* TX Mode: Symbols Continuous transmission */
#define TX_MODE_SYMBOLS_CONTINUOUS             (1 << 3)

/* Impedance Configuration */
#define HI_STATE                               0x00
#define LOW_STATE                              0x01
#define VLO_STATE                              0x02 
  
/* PRIME PLC Information Base (PIBs) */
typedef enum {
  PL360_ID_HOST_DESCRIPTION_ID = 0x0100,
  PL360_ID_HOST_MODEL_ID  = 0x010A,
  PL360_ID_HOST_PHY_ID = 0x010C,
  PL360_ID_HOST_PRODUCT_ID = 0x0110,
  PL360_ID_HOST_VERSION_ID = 0x0112,
  PL360_ID_HOST_BAND_ID = 0x0116,
  PL360_ID_TIME_REF_ID = 0x0200,
  PL360_ID_PRODID = 0x4000,
  PL360_ID_MODEL,
  PL360_ID_VERSION_STR,
  PL360_ID_VERSION_NUM,
  PL360_ID_CFG_AUTODETECT_IMPEDANCE,
  PL360_ID_CFG_IMPEDANCE,
  PL360_ID_ZC_TIME,
  PL360_ID_RX_PAY_SYMBOLS,
  PL360_ID_TX_PAY_SYMBOLS,
  PL360_ID_RSV0,
  PL360_ID_MAX_RMS_TABLE_HI,
  PL360_ID_MAX_RMS_TABLE_VLO,
  PL360_ID_THRESHOLDS_TABLE_HI,
  PL360_ID_THRESHOLDS_TABLE_LO,
  PL360_ID_THRESHOLDS_TABLE_VLO,
  PL360_ID_PREDIST_COEF_TABLE_HI,
  PL360_ID_PREDIST_COEF_TABLE_LO,
  PL360_ID_PREDIST_COEF_TABLE_VLO,
  PL360_ID_GAIN_TABLE_HI,
  PL360_ID_GAIN_TABLE_LO,
  PL360_ID_GAIN_TABLE_VLO,
  PL360_ID_DACC_TABLE_CFG,
  PL360_ID_CHANNEL_CFG,
  PL360_ID_NUM_TX_LEVELS,
  PL360_ID_CORRECTED_RMS_CALC,
  PL360_ID_CURRENT_GAIN,
  PL360_ID_ZC_CONF_INV,
  PL360_ID_ZC_CONF_FREQ,
  PL360_ID_ZC_CONF_DELAY,
  PL360_ID_SIGNAL_CAPTURE_START,
  PL360_ID_SIGNAL_CAPTURE_STATUS,
  PL360_ID_SIGNAL_CAPTURE_FRAGMENT,
  PL360_ID_SIGNAL_CAPTURE_DATA,
  PL360_ID_ENABLE_AUTO_NOISE_CAPTURE,
  PL360_ID_TIME_BETWEEN_NOISE_CAPTURES,
  PL360_ID_DELAY_NOISE_CAPTURE_AFTER_RX,
  PL360_ID_RRC_NOTCH_ACTIVE,
  PL360_ID_RRC_NOTCH_INDEX,
  PL360_ID_NOISE_PEAK_POWER,
  PL360_ID_RRC_NOTCH_AUTODETECT,
  PL360_ID_RRC_NOTCH_THR_ON,
  PL360_ID_RRC_NOTCH_THR_OFF,
  PL360_ID_TX_TOTAL,
  PL360_ID_TX_TOTAL_BYTES,
  PL360_ID_TX_TOTAL_ERRORS,
  PL360_ID_TX_BAD_BUSY_TX,
  PL360_ID_TX_BAD_BUSY_CHANNEL,
  PL360_ID_TX_BAD_LEN,
  PL360_ID_TX_BAD_FORMAT,
  PL360_ID_TX_TIMEOUT,
  PL360_ID_RX_TOTAL,
  PL360_ID_RX_TOTAL_BYTES,
  PL360_ID_RX_EXCEPTIONS,
  PL360_ID_RX_BAD_LEN,
  PL360_ID_RX_BAD_CRC_FCH,
  PL360_ID_RX_FALSE_POSITIVE,
  PL360_ID_RX_BAD_FORMAT,
  PL360_ID_NOISE_PER_CARRIER,
  PL360_ID_PPM_CALIB_ON,
  PL360_ID_ZC_PERIOD,
  PL360_ID_SYNC_THRESHOLDS,
  PL360_ID_OBSOLETE_ID,
  PL360_ID_END_ID,
} DRV_PL360_ID;    

/* PRIME Modulation types */
typedef enum {
  SCHEME_DBPSK = 0,
  SCHEME_DQPSK = 1,
  SCHEME_D8PSK = 2,
  SCHEME_DBPSK_C = 4,
  SCHEME_DQPSK_C = 5,
  SCHEME_D8PSK_C = 6,
  SCHEME_R_DBPSK = 12,
  SCHEME_R_DQPSK = 13,
}DRV_PL360_SCH;

/* PRIME Modulation schemes */
typedef enum {
  FRAME_TYPE_A = 0,
  FRAME_TYPE_B = 2,
  FRAME_TYPE_BC = 3,
}DRV_PL360_FRAME_TYPE;

/* PRIME Frame Delimiter Types */
typedef enum {
  HDR_GENERIC = 0,
  HDR_PROMOTION = 1,
  HDR_BEACON = 2,
}DRV_PL360_HEADER;

/* PRIME Buffer ID */
typedef enum {
  TX_BUFFER_0 = 0,
  TX_BUFFER_1 = 1,
}DRV_PL360_BUFFER_ID;

/* PRIME TX Result values */
typedef enum {
  DRV_PL360_TX_RESULT_PROCESS = 0,               /* Transmission result: already in process */
  DRV_PL360_TX_RESULT_SUCCESS = 1,               /* Transmission result: end successfully */
  DRV_PL360_TX_RESULT_INV_LENGTH = 2,            /* Transmission result: invalid length error */
  DRV_PL360_TX_RESULT_BUSY_CH = 3,               /* Transmission result: busy channel error */
  DRV_PL360_TX_RESULT_BUSY_TX = 4,               /* Transmission result: busy in transmission error */
  DRV_PL360_TX_RESULT_BUSY_RX = 5,               /* Transmission result: busy in reception error */
  DRV_PL360_TX_RESULT_INV_SCHEME = 6,            /* Transmission result: invalid modulation scheme error */
  DRV_PL360_TX_RESULT_TIMEOUT = 7,               /* Transmission result: timeout error */
  DRV_PL360_TX_RESULT_INV_BUFFER = 8,            /* Transmission result: invalid buffer identifier error */
  DRV_PL360_TX_RESULT_INV_MODE = 9,              /* Transmission result: invalid PRIME Mode error */
  DRV_PL360_TX_RESULT_NO_TX = 255,               /* Transmission result: No transmission ongoing */
}DRV_PL360_TX_RESULT;

/* PRIME Transmission setup */
typedef struct __attribute__((packed, aligned(1))) {
  uint8_t *pTransmitData;                        /* Pointer to data buffer to transmit */
  uint32_t time;                                 /* Instant when transmission has to start referred to 1ms PL360 counter */
  uint16_t dataLength;                           /* Length of the data to transmit */
  uint8_t mode;                                  /* Transmission Mode (absolute, relative, cancel, continuous). Constants above */
  uint8_t attenuation;                           /* Power to transmit */
  uint8_t forced;                                /* Forced transmission */
  DRV_PL360_BUFFER_ID bufferId;                  /* Buffer Id used for transmission */
  DRV_PL360_SCH scheme;                          /* Scheme of Modulation */
  DRV_PL360_FRAME_TYPE frameType;                /* PRIME Frame type */
} DRV_PL360_TRANSMISSION_OBJ;

/* PRIME Result of a transmission */
typedef struct {
  uint32_t time;                                 /* Instant when frame transmission ended referred to 1ms PHY counter */
  uint32_t rmsCalc;                              /* RMS_CALC it allows to estimate tx power injected */
  DRV_PL360_FRAME_TYPE frameType;                /* PRIME Frame type */
  DRV_PL360_TX_RESULT result;                    /* Tx Result (see "TX Result values" above) */
  DRV_PL360_BUFFER_ID bufferId;                  /* Buffer Id used for transmission */
} DRV_PL360_TRANSMISSION_CFM_OBJ;

/* PRIME Reception parameters */
typedef struct __attribute__((packed, aligned(1))) {
  uint8_t *pReceivedData;                        /* Pointer to received data buffer */
  uint32_t time;                                 /* Instant when frame was received */
  uint32_t evmHeaderAcum;                        /* Accumulated Error Vector Magnitude for header */
  uint32_t evmPayloadAcum;                       /* Accumulated Error Vector Magnitude for payload */
  uint16_t evmHeader;                            /* Error Vector Magnitude for header */
  uint16_t evmPayload;                           /* Error Vector Magnitude for payload */
  uint16_t dataLength;                           /* Length of the received data */
  DRV_PL360_SCH scheme;                          /* Scheme of Modulation */
  DRV_PL360_FRAME_TYPE frameType;                /* PRIME Frame type */
  DRV_PL360_HEADER headerType;                   /* Header type */
  uint8_t rssiAvg;                               /* Average RSSI (Received Signal Strength Indication) */
  uint8_t cinrAvg;                               /* Average CNIR (Carrier to Interference + Noise ratio) */
  uint8_t cinrMin;                               /* Minimum CNIR (Carrier to Interference + Noise ratio) */
  uint8_t berSoftAvg;                            /* Average Soft BER (Bit Error Rate) */
  uint8_t berSoftMax;                            /* Maximum Soft BER (Bit Error Rate) */
  uint8_t narBandPercent;                        /* Percentage of carriers affected by narrow band noise */
  uint8_t impNoisePercent;                       /* Percentage of symbols affected by impulsive noise */
} DRV_PL360_RECEPTION_OBJ;

/* PRIME PLC Information Base (PIB) */
typedef struct {
  uint8_t *pData;                                /* Pointer to PIB data */
  DRV_PL360_ID id;                               /* PLC Information base identification */
  uint16_t length;                               /* Length in bytes of the data information */
} DRV_PL360_PIB_OBJ;



//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // #ifndef DRV_PL360_COMM_H

/*******************************************************************************
 End of File
*/
