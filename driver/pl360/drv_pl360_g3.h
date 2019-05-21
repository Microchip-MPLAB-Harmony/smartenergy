/*******************************************************************************
  PL360 Driver G3 Definitions Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360_comm.h

  Summary:
    PL360 Driver G3 Definitions Header File

  Description:
    This file provides implementation-specific definitions for the PL360
    driver's system G3 interface.
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

#ifndef DRV_PL360_G3_H
#define DRV_PL360_G3_H

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



// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* DRV_PL360 G3 Profiles

 Summary:
    Defines all G3 PHY bands supported by PL360 device.

 Description:
    None.

 Remarks:
    None.
*/
      
typedef enum {
  G3_CEN_A = 0,
  G3_CEN_B = 1,
  G3_FCC = 2
}DRV_PL360_PROFILE;      

/* DRV_PL360_COMM Handle Macros*/      
#define FCH_LEN_CENELEC_A                          5
#define FCH_LEN_FCC                                9
#define FCH_LEN_ARIB                               9
#define FCH_LEN_CENELEC_B                          5

#define NUM_CARRIERS_CENELEC_A                     36
#define NUM_CARRIERS_FCC                           72
#define NUM_CARRIERS_ARIB                          54
#define NUM_CARRIERS_CENELEC_B                     16

/* Subbands for Cenelec bandplan */
#define NUM_SUBBANDS_CENELEC_A                     6
/* Subbands for FCC bandplan */
#define NUM_SUBBANDS_FCC                           24
/* Subbands for Cenelec-B bandplan */
#define NUM_SUBBANDS_CENELEC_B                     4

/* CENELEC A Band Plan (35 - 91 Khz) */
#define PL360_CENELEC_A                            0
/* CENELEC-B Band Plan (98 - 122 Khz) */
#define PL360_CENELEC_B                            1
/* FCC Band Plan (155 - 487 Khz) */
#define PL360_FCC                                  2
      
/* Tone Map size for Cenelec bandplan */
#define TONE_MAP_SIZE_CENELEC                      1
/* Tone Map size for FCC and ARIB bandplans */
#define TONE_MAP_SIZE_FCC                          3
/* Maximum number of protocol carriers */
#define PROTOCOL_CARRIERS_MAX                      NUM_CARRIERS_FCC
/* Maximum number of tone map */
#define TONE_MAP_SIZE_MAX                          TONE_MAP_SIZE_FCC     
/* Maximum Length of FCH */
#define FCH_LEN_MAX                                FCH_LEN_FCC
/* Maximum number of subbands */
#define NUM_SUBBANDS_MAX                           NUM_SUBBANDS_FCC

/* TX Mode: Forced transmission */
#define TX_MODE_FORCED                             (1 << 0)
/* TX Mode: Absolute transmission */          
#define TX_MODE_ABSOLUTE                           (0 << 1)
/* TX Mode: Delayed transmission */          
#define TX_MODE_RELATIVE                           (1 << 1)
/* TX Mode: SYNCP Continuous transmission */
#define TX_MODE_SYNCP_CONTINUOUS                   (1 << 2)
/* TX Mode: Symbols Continuous transmission */
#define TX_MODE_SYMBOLS_CONTINUOUS                 (1 << 3)
/* TX Mode: Cancel transmission */
#define TX_MODE_CANCEL                             (1 << 4)

/* Impedance Configuration */
#define HI_STATE                                   0x00
#define LOW_STATE                                  0x01
#define VLO_STATE                                  0x02  

// *****************************************************************************
/* G3 PHY Information Base (PIBs)

   Summary
    The list of all available PIB attributes.

   Description
    The G3 FW stack supports all the mandatory attributes of the PLC Information 
    Base (PIB) defined in the G3 specification. In addition, Microchip has added 
    several proprietary PIB attributes to support extra functionalities. 
    The list of all available PIB attributes can be found in this file.

   Remarks:
    None
*/

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
  PL360_ID_TONE_MASK,
  PL360_ID_TONE_MAP_RSP_DATA,
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
  PL360_ID_RX_RS_ERRORS,
  PL360_ID_RX_EXCEPTIONS,
  PL360_ID_RX_BAD_LEN,
  PL360_ID_RX_BAD_CRC_FCH,
  PL360_ID_RX_FALSE_POSITIVE,
  PL360_ID_RX_BAD_FORMAT,
  PL360_ID_ENABLE_AUTO_NOISE_CAPTURE,
  PL360_ID_TIME_BETWEEN_NOISE_CAPTURES,
  PL360_ID_DELAY_NOISE_CAPTURE_AFTER_RX,
  PL360_ID_RRC_NOTCH_ACTIVE,
  PL360_ID_RRC_NOTCH_INDEX,
  PL360_ID_NOISE_PEAK_POWER,
  PL360_ID_RSV0,
  PL360_ID_RSV1,
  PL360_ID_CFG_AUTODETECT_IMPEDANCE,
  PL360_ID_CFG_IMPEDANCE,
  PL360_ID_ZC_PERIOD,
  PL360_ID_FCH_SYMBOLS,
  PL360_ID_PAY_SYMBOLS_TX,
  PL360_ID_PAY_SYMBOLS_RX,
  PL360_ID_RRC_NOTCH_AUTODETECT,
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
  PL360_ID_RSV2,
  PL360_ID_NUM_TX_LEVELS,
  PL360_ID_CORRECTED_RMS_CALC,
  PL360_ID_RRC_NOTCH_THR_ON,
  PL360_ID_RRC_NOTCH_THR_OFF,
  PL360_ID_CURRENT_GAIN,
  PL360_ID_ZC_CONF_INV,
  PL360_ID_ZC_CONF_FREQ,
  PL360_ID_ZC_CONF_DELAY,
  PL360_ID_NOISE_PER_CARRIER,
  PL360_ID_SYNC_XCORR_THRESHOLD,
  PL360_ID_SYNC_XCORR_PEAK_VALUE,
  PL360_ID_SYNC_SYNCM_THRESHOLD,
  PL360_ID_TONE_MAP_RSP_ENABLED_MODS,
  PL360_ID_PPM_CALIB_ON,
  PL360_ID_SFO_ESTIMATION_LAST_RX,
  PL360_ID_END_ID,
} DRV_PL360_ID;    

// *****************************************************************************
/* G3 Modulation types

   Summary
    The list of all types of modulation supported by G3 spec.

   Remarks:
    None
*/
typedef enum {
  MOD_TYPE_BPSK = 0,
  MOD_TYPE_QPSK = 1,
  MOD_TYPE_8PSK = 2,
  MOD_TYPE_QAM = 3,
  MOD_TYPE_BPSK_ROBO = 4,
}DRV_PL360_MOD_TYPE;

// *****************************************************************************
/* G3 Modulation schemes

   Summary
    The list of all modulation schemes supported by G3 spec.

   Remarks:
    None
*/
typedef enum {
  MOD_SCHEME_DIFFERENTIAL = 0,
  MOD_SCHEME_COHERENT = 1,
}DRV_PL360_MOD_SCHEME;

// *****************************************************************************
/* G3 Frame Delimiter Types

   Summary
    The list of all delimiter types supported by G3 spec.

   Remarks:
    None
*/
typedef enum {
  DT_SOF_NO_RESP = 0,
  DT_SOF_RESP = 1,
  DT_ACK = 2,
  DT_NACK = 3,
}DRV_PL360_DEL_TYPE;

// *****************************************************************************
/* G3 Result values of a previous transmission

   Summary
    This list involves all available results from MCHP implementation

   Remarks:
    None
*/
typedef enum {
  /* Transmission result: already in process */
  DRV_PL360_TX_RESULT_PROCESS = 0,   
  /* Transmission result: end successfully */            
  DRV_PL360_TX_RESULT_SUCCESS = 1,      
  /* Transmission result: invalid length error */
  DRV_PL360_TX_RESULT_INV_LENGTH = 2,            
  /* Transmission result: busy channel error */
  DRV_PL360_TX_RESULT_BUSY_CH = 3,               
  /* Transmission result: busy in transmission error */
  DRV_PL360_TX_RESULT_BUSY_TX = 4,               
  /* Transmission result: busy in reception error */
  DRV_PL360_TX_RESULT_BUSY_RX = 5,               
  /* Transmission result: invalid modulation scheme error */
  DRV_PL360_TX_RESULT_INV_SCHEME = 6,            
  /* Transmission result: timeout error */
  DRV_PL360_TX_RESULT_TIMEOUT = 7,               
  /* Transmission result: invalid tone map error */
  DRV_PL360_TX_RESULT_INV_TONEMAP = 8,           
  /* Transmission result: invalid G3 Mode error */
  DRV_PL360_TX_RESULT_INV_MODE = 9,              
  /* Transmission result: No transmission ongoing */
  DRV_PL360_TX_RESULT_NO_TX = 255,               
}DRV_PL360_TX_RESULT;

// *****************************************************************************
/* G3 Tone map response data

   Summary
    This struct includes modulation type, modulation scheme and Tone Map data

   Remarks:
    For more information about Tone Map Response functionality, please refer to
    G3 Specification
*/
typedef struct {
  /* Modulation type */
  DRV_PL360_MOD_TYPE modType;       
  /* Modulation scheme */             
  DRV_PL360_MOD_SCHEME modScheme;   
   /* Tone Map */             
  uint8_t toneMap[TONE_MAP_SIZE_MAX];           
} DRV_PL360_TONE_MAP_RSP;

// *****************************************************************************
/* G3 Transmission setup data

   Summary
    This struct includes all information to describe any transmissions.

   Remarks:
    None
*/
typedef struct __attribute__((packed, aligned(1))) {
  /* Pointer to data buffer to transmit */
  uint8_t *pTransmitData;       
  /* Instant when transmission has to start referred to 1ms PHY counter */                 
  uint32_t time;         
  /* Length of the data to transmit */                        
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
  /* Flag to indicate whether 2 RS blocks have to be used (only used for FCC) */                
  uint8_t rs2Blocks;         
  /* Modulation type */                    
  DRV_PL360_MOD_TYPE modType;             
  /* Modulation scheme */       
  DRV_PL360_MOD_SCHEME modScheme;            
  /* DT field to be used in header */    
  DRV_PL360_DEL_TYPE delimiterType;              
} DRV_PL360_TRANSMISSION_OBJ;

// *****************************************************************************
/* G3 Result of a transmission

   Summary
    This struct includes all information to describe any result of a previous 
    transmission.

   Remarks:
    None
*/
typedef struct {
  /* Instant when frame transmission ended referred to 1ms PHY counter */
  uint32_t time;    
  /* RMS_CALC it allows to estimate tx power injected */                             
  uint32_t rmsCalc;            
  /* Tx Result (see "TX Result values" above) */                  
  DRV_PL360_TX_RESULT result;                    
} DRV_PL360_TRANSMISSION_CFM_OBJ;

// *****************************************************************************
/* G3 Reception parameters

   Summary
    This struct includes all information to describe any new received message.

   Remarks:
    None
*/
typedef struct __attribute__((packed, aligned(1))) {
   /* Pointer to received data buffer */
  uint8_t *pReceivedData;                       
  /* Instant when frame was received */
  uint32_t time;                                 
  /* Frame duration referred to 1ms PHY counter (FCH + Payload) */
  uint32_t frameDuration;                        
  /* Length of the received data */
  uint16_t dataLength;                           
  /* Reception RSSI */
  uint16_t rssi;                                 
  /* ZCT info */
  uint8_t zctDiff;                               
  /* Errors corrected by RS */
  uint8_t rsCorrectedErrors;                     
  /* Modulation type */
  DRV_PL360_MOD_TYPE modType;                    
  /* Modulation scheme */
  DRV_PL360_MOD_SCHEME modScheme;                
  /* DT field coming in header */
  DRV_PL360_DEL_TYPE delimiterType;              
  /* Reserved */
  uint8_t rsrv0;                                 
  /* Test data information */
  uint32_t agcFactor;          
  /* Test data information */  
  uint16_t agcFine;            
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
  /* BER: Number of corrupted carriers */
  uint16_t payloadCorruptedCarriers;             
  /* BER: Number of noised symbols */
  uint16_t payloadNoisedSymbols;                 
  /* BER: SNR of the worst carrier */
  uint8_t payloadSnrWorstCarrier;                
  /* BER: SNR of the worst symbol */
  uint8_t payloadSnrWorstSymbol;                 
  /* BER: SNR on impulsive noise */
  uint8_t payloadSnrImpulsive;                  
   /* BER: Narrowband SNR */ 
  uint8_t payloadSnrBand;                       
  /* BER: Background SNR */
  uint8_t payloadSnrBackground;                  
  /* BER: Link Quality */
  uint8_t lqi;                                   
  /* Reception Tone Map */
  uint8_t toneMap[TONE_MAP_SIZE_MAX];            
  /* SNR per carrier */
  uint8_t carrierSnr[PROTOCOL_CARRIERS_MAX];     
} DRV_PL360_RECEPTION_OBJ;

// *****************************************************************************
/* G3 PHY Information Base (PIB)

   Summary
    This struct includes all information to access any defined PIB.

   Remarks:
    None
*/
typedef struct {
  /* Pointer to PIB data */
  uint8_t *pData;                                
  /* PLC Information base identification */
  DRV_PL360_ID id;                               
  /* Length in bytes of the data information */
  uint16_t length;                               
} DRV_PL360_PIB_OBJ;

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // #ifndef DRV_PL360_G3_H

/*******************************************************************************
 End of File
*/
