/*******************************************************************************
  MAC RT Driver Communication Definitions Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_g3_macrt_comm.h

  Summary:
    MAC RT Driver Communication Definitions Header File

  Description:
    This file provides implementation-specific definitions for G3 MAC RT 
    interface.
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

#ifndef DRV_G3_MACRT_COMM_H
#define DRV_G3_MACRT_COMM_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

/* G3 Bandplan */
#define G3_CEN_A                                   0
#define G3_CEN_B                                   1
#define G3_FCC                                     2
#define G3_ARIB                                    3
#define G3_INVALID                                 0xFF

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************
#define MAC_RT_REG_CMD_RD               (0)
#define MAC_RT_REG_CMD_WR               (1)

#define MAC_RT_SPEC_COMPLIANCE_17       (0)
#define MAC_RT_SPEC_COMPLIANCE_15       (1)

#define MAC_RT_MAX_PAYLOAD_SIZE         (404u)
#define MAC_RT_FULL_HEADER_SIZE         (32)
#define MAC_RT_SECURITY_HEADER_SIZE     (6)
#define MAC_RT_SEGMENT_CONTROL_SIZE     (3)
#define MAC_RT_FCS_SIZE                 (2)
        
#define MAC_RT_SHORT_ADDRESS_BROADCAST  (0xFFFFu)
#define MAC_RT_SHORT_ADDRESS_UNDEFINED  (0xFFFFu)
        
#define MAC_RT_PIB_MAX_VALUE_LENGTH     (144)        

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************  
// *****************************************************************************
        
/* G3 MAC RT PIB list

   Summary
    The list of G3 MAC RT PIBs.

   Remarks:
    None
*/
typedef enum {
    MAC_RT_PIB_ACK_WAIT_DURATION = 0x00000040,
    MAC_RT_PIB_MAX_BE = 0x00000047,
    MAC_RT_PIB_MAX_CSMA_BACKOFFS = 0x0000004E,
    MAC_RT_PIB_MIN_BE = 0x0000004F,
    MAC_RT_PIB_PAN_ID = 0x00000050,
    MAC_RT_PIB_SHORT_ADDRESS = 0x00000053,
    MAC_RT_PIB_MAX_FRAME_RETRIES = 0x00000059,
    MAC_RT_PIB_HIGH_PRIORITY_WINDOW_SIZE = 0x00000100,
    MAC_RT_PIB_CSMA_NO_ACK_COUNT = 0x00000106,
    MAC_RT_PIB_BAD_CRC_COUNT = 0x00000109,
    MAC_RT_PIB_CSMA_FAIRNESS_LIMIT = 0x0000010C,
    MAC_RT_PIB_TONE_MASK = 0x00000110,
    MAC_RT_PIB_A = 0x00000112,
    MAC_RT_PIB_K = 0x00000113,
    MAC_RT_PIB_MIN_CW_ATTEMPTS = 0x00000114,
    MAC_RT_PIB_CENELEC_LEGACY_MODE = 0x00000115,
    MAC_RT_PIB_FCC_LEGACY_MODE = 0x00000116,
    MAC_RT_PIB_BROADCAST_MAX_CW_ENABLE = 0x0000011E,
    MAC_RT_PIB_TRANSMIT_ATTEN = 0x0000011F,
    /* manufacturer specific */
    /* Extended address of this node. */
    MAC_RT_PIB_MANUF_EXTENDED_ADDRESS = 0x08000001,
    /* returns the maximum number of tones used by the band */
    MAC_RT_PIB_MANUF_BAND_INFORMATION = 0x08000003,
    /* Forces Modulation Scheme in every transmitted frame */
    /* 0 - Not forced, 1 - Force Differential, 2 - Force Coherent */
    MAC_RT_PIB_MANUF_FORCED_MOD_SCHEME = 0x08000007,
    /* Forces Modulation Type in every transmitted frame */
    /* 0 - Not forced, 1 - Force BPSK_ROBO, 2 - Force BPSK, 3 - Force QPSK, 4 - Force 8PSK */
    MAC_RT_PIB_MANUF_FORCED_MOD_TYPE = 0x08000008,
    /* Forces ToneMap in every transmitted frame */
    /* {0} - Not forced, other value will be used as tonemap */
    MAC_RT_PIB_MANUF_FORCED_TONEMAP = 0x08000009,
    /* Gets number of discarded packets due to Segment Decode Error */
    MAC_RT_PIB_MANUF_RX_SEGMENT_DECODE_ERROR_COUNT = 0x0800001C,
    /* Gets or Sets number of retires left before forcing ROBO mode */
    MAC_RT_PIB_MANUF_RETRIES_LEFT_TO_FORCE_ROBO = 0x0800001F,
    /* Gets internal MAC RT version */
    MAC_RT_PIB_MANUF_MAC_RT_INTERNAL_VERSION = 0x08000022,
    /* IB used to set the complete MIB structure at once */
    MAC_RT_PIB_GET_SET_ALL_MIB = 0x08000100,
    /* Gets or sets a parameter in Phy layer. Index will be used to contain PHY parameter ID. */
    /* Check 'MAC_RT_PHY_PIB' below for available Phy parameter IDs */
    MAC_RT_PIB_MANUF_PHY_PARAM = 0x08000020
}MAC_RT_PIB;        

/* G3 PHY Parameters list

   Summary
    The list of G3 PHY parameters.

   Remarks:
    None
*/
typedef enum {
    /* Product Identifier of firmware embedded */
    PHY_PARAM_PRODID = 0,
    /* Model Identifier of firmware embedded */
	PHY_PARAM_MODEL,
    /* Version number of embedded firmware in string format */
	PHY_PARAM_VERSION_STR,
    /* Version number of embedded firmware in hexadecimal format */
	PHY_PARAM_VERSION_NUM,
    /* Tone mask for static notching */
	PHY_PARAM_TONE_MASK,
    /* Tone Map response data */
	PHY_PARAM_TONE_MAP_RSP_DATA,
    /* Number of successfully transmitted PDUs */
	PHY_PARAM_TX_TOTAL,
    /* Number of bytes in successfully transmitted PDUs */
	PHY_PARAM_TX_TOTAL_BYTES,
    /* Number of unsuccessfully transmitted PDUs */
	PHY_PARAM_TX_TOTAL_ERRORS,
    /* Number of times when the device received new data to transmit (send_data)
     * and there is already data in the TX chain. */
	PHY_PARAM_TX_BAD_BUSY_TX,
    /* Number of times when the device received new data to transmit (send_data) 
     * and the PLC channel is busy */
	PHY_PARAM_TX_BAD_BUSY_CHANNEL,
    /* Number of times when the device received new data to transmit (send_data)
     * and the specified length in transmission parameters is invalid */
	PHY_PARAM_TX_BAD_LEN,
    /* Number of times when the device received new data to transmit (send_data) 
     * and the transmission parameters are not valid */
	PHY_PARAM_TX_BAD_FORMAT,
    /* Number of times when the PLC transceiver received new data to transmit 
     * (send_data) and it cannot transmit data in the specified time provided 
     * by the transmission parameters */
	PHY_PARAM_TX_TIMEOUT,
    /* Number of successfully received PDUs */
	PHY_PARAM_RX_TOTAL,
    /* Number of bytes in successfully received PDUs */
	PHY_PARAM_RX_TOTAL_BYTES,
    /* Number of corrected errors by RS block in received PDUs */
	PHY_PARAM_RX_RS_ERRORS,
    /* Number of time-out errors in received PDUs */
	PHY_PARAM_RX_EXCEPTIONS,
    /* Number of errors in FCH length in received PDUs */
	PHY_PARAM_RX_BAD_LEN,
    /* Number of errors in FCH CRC in received PDUs */
	PHY_PARAM_RX_BAD_CRC_FCH,
    /* Number of errors in PDU synchronization phase */
	PHY_PARAM_RX_FALSE_POSITIVE,
    /* Number of errors in modulation type field included in FCH of received 
     * PDUs */
	PHY_PARAM_RX_BAD_FORMAT,
    /* Enable periodic noise auto-detect and adaptation */       
	PHY_PARAM_ENABLE_AUTO_NOISE_CAPTURE,
    /* Time in milliseconds between noise captures */
	PHY_PARAM_TIME_BETWEEN_NOISE_CAPTURES,
    /* Noise detection timer reloaded after a correct reception */
	PHY_PARAM_DELAY_NOISE_CAPTURE_AFTER_RX,
    /* Number of notched frequencies with RRC notch filter */
	PHY_PARAM_RRC_NOTCH_ACTIVE,
    /* Array of RRC notch filter index values in format unsigned Q7.8 */
	PHY_PARAM_RRC_NOTCH_INDEX,
    /* Indicate noise power in dBuV for the noisier carrier */
	PHY_PARAM_NOISE_PEAK_POWER,
    /* Capability to compute Payload CRC in PHY Layer */
	PHY_PARAM_CRC_TX_RX_CAPABILITY,
    /* Number of errors in Payload CRC in received PDUs */
	PHY_PARAM_RX_BAD_CRC_PAY,
    /* Auto-Detect Impedance Mode */
	PHY_PARAM_CFG_AUTODETECT_IMPEDANCE,
    /* Transmission Mode (HIGH, LOW, VERY_LOW) */
	PHY_PARAM_CFG_IMPEDANCE,
    /* Estimated last Zero Cross period in microseconds */
	PHY_PARAM_ZC_PERIOD,
    /* Number of symbols in Frame Control Header */
	PHY_PARAM_FCH_SYMBOLS,
    /* Number of payload symbols in last transmitted message */
	PHY_PARAM_PAY_SYMBOLS_TX,
    /* Number of payload symbols in last received message */
	PHY_PARAM_PAY_SYMBOLS_RX,
    /* Trigger to start noise analysis */
	PHY_PARAM_RRC_NOTCH_AUTODETECT,
    /* Target RMS_CALC in HIGH Tx Mode */
	PHY_PARAM_MAX_RMS_TABLE_HI,
    /* Target RMS_CALC in VERY_LOW Tx Mode */
	PHY_PARAM_MAX_RMS_TABLE_VLO,
    /* Thresholds table to automatically update Tx Mode */
	PHY_PARAM_THRESHOLDS_TABLE_HI,
    /* Thresholds table to automatically update Tx Mode */
	PHY_PARAM_THRESHOLDS_TABLE_LO,
    /* Thresholds table to automatically update Tx Mode */
	PHY_PARAM_THRESHOLDS_TABLE_VLO,
    /* Equalization Coefficients table in HIGH Tx mode */
	PHY_PARAM_PREDIST_COEF_TABLE_HI,
    /* Equalization Coefficients table in LOW Tx mode */
	PHY_PARAM_PREDIST_COEF_TABLE_LO,
    /* Equalization Coefficients table in VERY_LOW Tx mode */
	PHY_PARAM_PREDIST_COEF_TABLE_VLO,
    /* Gain values table for HIGH Tx Mode */
	PHY_PARAM_GAIN_TABLE_HI,
    /* Gain values table for LOW Tx Mode */
	PHY_PARAM_GAIN_TABLE_LO,
    /* Gain values table for VERY_LOW Tx Mode */
	PHY_PARAM_GAIN_TABLE_VLO,
    /* Configuration values of DACC peripheral according to hardware 
     * configuration */
	PHY_PARAM_DACC_TABLE_CFG,
    /* Reserved for future uses */
	PHY_PARAM_RSV0,
    /* Number of Tx attenuation levels (3 dB steps) for normal transmission
     * behavior */
	PHY_PARAM_NUM_TX_LEVELS,
    /* RMS_CALC value obtained in the last transmitted message */
	PHY_PARAM_CORRECTED_RMS_CALC,
    /* Activation threshold for narrow band noise */
	PHY_PARAM_RRC_NOTCH_THR_ON,
    /* Deactivation threshold for narrow band noise */
	PHY_PARAM_RRC_NOTCH_THR_OFF,
    /* Transmission Gain which will be used in the next transmitted message */
	PHY_PARAM_CURRENT_GAIN,
    /* Inverted output of Zero-Crossing Detector */
	PHY_PARAM_ZC_CONF_INV,
    /* Initial frequency in Hz for Zero-Crossing Detector */
	PHY_PARAM_ZC_CONF_FREQ,
    /* Time Delay in microseconds of external Zero-Crossing Detection circuit */
	PHY_PARAM_ZC_CONF_DELAY,
    /* Estimation of noise (in dB?V) in each carrier belonging to the 
     * corresponding band */
	PHY_PARAM_NOISE_PER_CARRIER,
    /* Correlation threshold for synchronization (preamble detection) */
	PHY_PARAM_SYNC_XCORR_THRESHOLD,
    /* Correlation value in last received PDU */
	PHY_PARAM_SYNC_XCORR_PEAK_VALUE,
    /* Threshold for SYNCM detection (once preamble is detected with 
     * correlation) */
	PHY_PARAM_SYNC_SYNCM_THRESHOLD,
    /* Bit-mask to enable/disable modulations for modulation and Tone Map 
     * selection algorithm */
	PHY_PARAM_TONE_MAP_RSP_ENABLED_MODS,
    /* Enable the oscillator clock signal to go out through TXRX1 pad */
	PHY_PARAM_PPM_CALIB_ON,
    /* Estimation of clock frequency deviation on last received PDU */
	PHY_PARAM_SFO_ESTIMATION_LAST_RX,
    /* PDC value (field in G3 Frame Control Header) corresponding to last 
     * received PDU */
	PHY_PARAM_PDC_LAST_RX,
    /* Parameters for Maximum PSDU length computation */
	PHY_PARAM_MAX_PSDU_LEN_PARAMS,
    /* Maximum PSDU length depending on TX parameters */
	PHY_PARAM_MAX_PSDU_LEN,
	PHY_PARAM_RESET_STATS,
	PHY_PARAM_IC_DRIVER_CFG,
	PHY_PARAM_RX_CHN_EST_REAL,
	PHY_PARAM_RX_CHN_EST_IMAG,
    /* Index of the notch filter. 8 bits. */
    PHY_PARAM_RRC_NOTCH_INDEX_LEGACY = 0x0164,
    /* Disable PLC Tx/Rx. 8 bits. */
    PHY_PARAM_PLC_DISABLE = 0x016A,
    /* LQI value of the last received message */
    PHY_PARAM_LAST_MSG_LQI = 0x016C,
    /* LQI value of the last received message */
    PHY_PARAM_LAST_MSG_RSSI = 0x016D,
    /* Success transmission of ACK packets */
    PHY_PARAM_ACK_TX_CFM = 0x016E,
}MAC_RT_PHY_PIB;

// *****************************************************************************
/* G3 Modulation types

   Summary
    The list of all types of modulation supported by G3 spec.

   Remarks:
    None
*/
typedef enum {
    MAC_RT_MOD_ROBUST = 0x00,
    MAC_RT_MOD_BPSK = 0x01,
    MAC_RT_MOD_QPSK = 0x02,
    MAC_RT_MOD_8PSK = 0x03,
}MAC_RT_MOD_TYPE;

// *****************************************************************************
/* G3 Modulation schemes

   Summary
    The list of all modulation schemes supported by G3 spec.

   Remarks:
    None
*/
typedef enum {
    MAC_RT_MOD_SCHEME_DIFFERENTIAL = 0x00,
    MAC_RT_MOD_SCHEME_COHERENT = 0x01
}MAC_RT_MOD_SCHEME;

// *****************************************************************************
/* G3 MAC RT Address Mode

   Summary
    The list of addressing modes supported by G3 spec.

   Remarks:
    None
*/
typedef enum {
    MAC_RT_NO_ADDRESS = 0x00,
    MAC_RT_SHORT_ADDRESS = 0x01,
    MAC_RT_EXTENDED_ADDRESS = 0x02
}MAC_RT_ADDRESS_MODE;

// *****************************************************************************
/* G3 MAC RT Status

   Summary
    The list of status values.

   Remarks:
    None
*/
typedef enum {
    MAC_RT_STATUS_SUCCESS = 0x00,
    MAC_RT_STATUS_CHANNEL_ACCESS_FAILURE = 0xE1,
    MAC_RT_STATUS_DENIED = 0xE2,
    MAC_RT_STATUS_INVALID_INDEX = 0xF9,
    MAC_RT_STATUS_INVALID_PARAMETER = 0xE8,
    MAC_RT_STATUS_NO_ACK = 0xE9,
    MAC_RT_STATUS_READ_ONLY = 0xFB,
    MAC_RT_STATUS_TRANSACTION_OVERFLOW = 0xF1,
    MAC_RT_STATUS_UNSUPPORTED_ATTRIBUTE = 0xF4
}MAC_RT_STATUS;

// *****************************************************************************
/* G3 MAC RT State of Transmission

   Summary
    This list involves all available results from MCHP implementation

   Remarks:
    None
*/
typedef enum {
    /* Start of transmission */
    MAC_RT_TX_START,
    /* Start of CSMA_CA */
    MAC_RT_TX_CSMA_CA,
    /* CSMA_CA channel not idle */
    MAC_RT_TX_FAIL_CSMA_CA,
    /* Wait until a data request can be issued */
    MAC_RT_TX_WAIT_SEND,
    /* Wait for data confirm */
    MAC_RT_TX_WAIT_CONFIRM,
    /* Wait for ACK / NAK */
    MAC_RT_TX_WAIT_ACK,
    /* Segment send succeeded and positive ACK received / no ACK requested */
    MAC_RT_TX_SEND_OK,
    /* No ACK was received */
    MAC_RT_TX_BIG_FAIL,
    /* Negative ACK was received */
    MAC_RT_TX_LITTLE_FAIL,
    /* Unexpected PHY error */
    MAC_RT_TX_ABORT
}MAC_RT_TX_STATE;

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
    MAC_RT_MOD_TYPE modType;       
    /* Modulation scheme */             
    MAC_RT_MOD_SCHEME modScheme;   
    /* Tone Map */             
    uint8_t toneMap[3];           
} MAC_RT_TONE_MAP_RSP;

// *****************************************************************************
/* G3 MAC RT Address

   Summary
    This struct includes addressing mode and address. 

   Remarks:
    The format of the address depends on the addressing mode field.
*/
typedef struct {
    /* Modulation type */
    MAC_RT_ADDRESS_MODE addressMode;       
    union {
        /* Short Address */
        uint16_t shortAddress;
        /* Extended Address */
        uint8_t extendedAddress[8];
    };        
} MAC_RT_ADDRESS;

// *****************************************************************************
/* G3 Frame Control information

   Summary
    This struct includes information related to frame control

   Remarks:
    For more information, please refer to G3 Specification
    
*/
typedef struct {
    /* Frame Type */
    uint16_t frameType : 3;
    /* Security Enable */
    uint16_t securityEnabled : 1;
    /* Frame pending */
    uint16_t framePending : 1;
    /* Ack Request */
    uint16_t ackRequest : 1;
    /* PAN ID Compression */
    uint16_t panIdCompression : 1;
    /* Reserved */
    uint16_t reserved : 3;
    /* Destination Addressing Mode */
    uint16_t destAddressingMode : 2;
    /* Frame Version  */
    uint16_t frameVersion : 2;
    /* Source Addressing Mode */
    uint16_t srcAddressingMode : 2;  
} MAC_RT_FRAME_CONTROL;

// *****************************************************************************
/* G3 Segment Control information

   Summary
    This struct includes information related to segment control

   Remarks:
    For more information, please refer to G3 Specification
    
*/
typedef struct {
    /* Reserved */
    uint8_t reserved : 4;
    /* Tone Map Request */
    uint8_t toneMapRequest : 1;
    /* Contention Control */
    uint8_t contentionControl : 1;
    /* Channel Access Priority */
    uint8_t channelAccessPriority : 1;
    /* Last Segment Flag */
    uint8_t lastSegmentFlag : 1;
    /* Segment Count */
    uint16_t segmentCount : 6;
    /* Segment Length of MAC frame */
    uint16_t segmentLength : 10;  
} MAC_RT_SEGMENT_CONTROL;

// *****************************************************************************
/* G3 MAC RT Auxiliary Security Header

   Summary
    This struct includes information related to auxiliary security header

   Remarks:
    For more information, please refer to G3 Specification
    
*/
typedef struct {
    /* Security Level */
    uint8_t securityLevel : 3;
    /* Key Identifier Mode */
    uint8_t keyIdentifierMode : 2;
    /* Reserved */
    uint8_t reserved : 3;
    /* Frame Counter */
    uint32_t frameCounter;
    /* Key Identifier */
    uint8_t keyIdentifier;
} MAC_RT_AUX_SECURITY_HEADER;

// *****************************************************************************
/* G3 MAC RT header

   Summary
    This struct includes information related to MAC header

   Remarks:
    For more information, please refer to G3 Specification
    
*/
typedef struct __attribute__((packed, aligned(1))) {
    /* Frame Control Information */
    MAC_RT_FRAME_CONTROL frameControl;
    /* Sequence Number */
    uint8_t sequenceNumber;
    /* Destination PAN Identifier */
    uint16_t destinationPAN;
    /* Destination Address */
    MAC_RT_ADDRESS destinationAddress;
    /* Source PAN Identifier */
    uint16_t sourcePAN;
    /* Source Address */
    MAC_RT_ADDRESS sourceAddress;
    /* Security Header */
    MAC_RT_AUX_SECURITY_HEADER securityHeader;
} MAC_RT_HEADER;

// *****************************************************************************
/* G3 MAC RT frame

   Summary
    This struct includes information related to MAC frame

   Remarks:
    For more information, please refer to G3 Specification
    
*/
typedef struct __attribute__((packed, aligned(1))) {
    /* Frame Control Information */
    MAC_RT_SEGMENT_CONTROL segmentControl;
    /* MAC Header */
    MAC_RT_HEADER header;
    /* Length of the payload */
    uint16_t payloadLength;
    /* Pointer to Payload */
    uint8_t* pPayload;
    /* Length of the padding */
    uint16_t padLength;
    /* Frame Check Sequence */
    uint16_t fcs;
} MAC_RT_FRAME;

// *****************************************************************************
/* MAC RT MIB Initialization Data

  Summary:
    Defines the data required to initialize the MIB layer

  Description:
    This data type defines the data required to initialize and track MIB layer.

  Remarks:
    None.
*/
typedef struct __attribute__((packed, aligned(1))) {
    /* Counter of CSMA No ACK */
    uint32_t csmaNoAckCount;
    /* Counter of CRC errors */
    uint32_t badCrcCount;
    /* m_u32RxSegmentDecodeErrorCount */
    uint32_t rxSegmentDecodeErrorCount;
    /* PAN Identification */
    uint16_t panId;
    /* Short Address */
    uint16_t shortAddress;
    /* Tone Mask */
    uint8_t toneMask[9];
    /* Extended Address */
    uint8_t extendedAddress[8];
    /* Forced Tone Map */
    uint8_t forcedToneMap[3];
    /* High Priority Window Size */
    uint8_t highPriorityWindowSize;
    /* CSMA Fairness Limit */
    uint8_t csmaFairnessLimit;
    /* A */
    uint8_t A;
    /* K */
    uint8_t K;
    /* Min CW Attempts */
    uint8_t minCwAttempts;
    /* Max Be */
    uint8_t maxBe;
    /* Max CSMA Backoffs */
    uint8_t maxCsmaBackoffs;
    /* Max Frame Retries */
    uint8_t maxFrameRetries;
    /* Min Be */
    uint8_t minBe;
    /* Forced Mod Scheme */
    uint8_t forcedModScheme;
    /* Forced Mod Type  */
    uint8_t forcedModType;
    /* Retries To Force Robo mode */
    uint8_t retriesToForceRobo;
    /* Transmit Attenuation */
    uint8_t transmitAtten;
    /* Broadcast Max Cw Enable */
    bool broadcastMaxCwEnable;
    /* Coordinator Enable */
    bool coordinator;
}MAC_RT_MIB_INIT_OBJ;

// *****************************************************************************
/* G3 MAC RT Information Base (PIB)

   Summary
    This struct includes all information to access any defined PIB.

   Remarks:
    None
*/
typedef struct {
    MAC_RT_PIB pib;
    uint16_t index;
    uint8_t length;
    uint8_t pData[MAC_RT_PIB_MAX_VALUE_LENGTH];
}MAC_RT_PIB_OBJ;

// *****************************************************************************
/* G3 MAC RT Transmission setup parameters

   Summary
    This struct includes information to configure any transmission.

   Remarks:
    None
*/
typedef struct __attribute__((packed, aligned(1))) {
    /* Destination Address */
    MAC_RT_ADDRESS dstAddr;
    /* Gain of transmission */
    uint8_t txGain;
    /* */
    uint8_t txCoef[6];
    /* TX Resolution */
    uint8_t txRes;
    /* Modulation Type */
    MAC_RT_MOD_TYPE modType;
    /* Modulation Scheme */
    MAC_RT_MOD_SCHEME modScheme;
    /* Tone Map */
    uint8_t toneMap[3];
    /* Length of MAC Header */
    uint8_t macHeaderLen;
    /* Flag to request ACK */
    bool requestAck;
    /* Flag to set high priority */
    bool highPriority;
    /* Flag to request Tone Map */
    bool toneMapRequest;
    /* Flag to force Robo mode */
    bool forceRobo;
    /* Flag to enable security */
    bool securityEnabled;
} MAC_RT_TX_PARAMETERS_OBJ;

// *****************************************************************************
/* G3 MAC RT Reception parameters

   Summary
    This struct includes information to describe any new received message.

   Remarks:
    None
*/
typedef struct __attribute__((packed, aligned(1))) {
    /* Tone Map request */
    bool toneMapRequest;
    /* High Priority */
    bool highPriority;
    /* PDU Link Quality */
    uint8_t pduLinkQuality;
    /* Phase Differential */
    uint8_t phaseDifferential;
    /* Modulation Type */
    MAC_RT_MOD_TYPE modType;
    /* Modulation Scheme */
    MAC_RT_MOD_SCHEME modScheme;
    /* Tone map */
    uint8_t toneMap[3];
} MAC_RT_RX_PARAMETERS_OBJ;

// *****************************************************************************
/* G3 MAC RT Reception parameters

   Summary
    This struct includes information to describe any new received message.

   Remarks:
    None
*/
typedef struct {
    /* MAC RT Status */
    MAC_RT_STATUS status;
    /* Flag to indicate Timestamp should be updated */
    bool updateTimestamp;
    /* Modulation Type */
    MAC_RT_MOD_TYPE modType;
} MAC_RT_TX_CFM_OBJ;

// *****************************************************************************
/* G3 PHY sniffer parameters

   Summary
    This struct includes information about G3 PHY Sniffer packet.

   Remarks:
    None
*/
typedef struct {
    /* SNIFFER_IF_PHY_COMMAND_G3_VERSION */
    uint8_t commandVersion;    
    /* SNIFFER_VERSION */
	uint8_t snifferVersion;     
    /* SNIFFER_PLC_G3 */
	uint8_t deviceVersion;  
    /* ModType (high) + ModScheme (low) */
	uint8_t modTypeScheme;      
    /* Tone Map */
	uint8_t toneMap[3];     
    /* Number of symbols */
	uint16_t symbols;  
    /* Link Quality Indicator */
	uint8_t lqi;      
    /* Delimiter Type */
	uint8_t dt;       
    /* Init time */
	uint32_t timeIni;            
    /* End time */
	uint32_t timeEnd;     
    /* RSSI */
	uint16_t rssi;      
    /* AGC factor */
	uint16_t agcFactor;       
    /* Data length */
	uint16_t dataLength;                       
} MAC_RT_SNIFFER_HEADER;

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // #ifndef DRV_G3_MACRT_COMM_H

/*******************************************************************************
 End of File
*/
