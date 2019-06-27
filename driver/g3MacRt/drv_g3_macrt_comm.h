/*******************************************************************************
  MAC RT Driver Communication Definitions Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360_macrt_comm.h

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

#ifndef DRV_PL360_MACRT_COMM_H
#define DRV_PL360_MACRT_COMM_H

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

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************
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
    /* Phy layer version number. 32 bits. */
    PHY_PARAM_VERSION = 0x010c,
    /* Correctly transmitted frame count. 32 bits. */
    PHY_PARAM_TX_TOTAL = 0x0110,
    /* Transmitted bytes count. 32 bits. */
    PHY_PARAM_TX_TOTAL_BYTES = 0x0114,
    /* Transmission errors count. 32 bits. */
    PHY_PARAM_TX_TOTAL_ERRORS = 0x0118,
    /* Transmission failure due to already in transmission. 32 bits. */
    PHY_PARAM_BAD_BUSY_TX = 0x011C,
    /* Transmission failure due to busy channel. 32 bits. */
    PHY_PARAM_TX_BAD_BUSY_CHANNEL = 0x0120,
    /* Bad len in message (too short - too long). 32 bits. */
    PHY_PARAM_TX_BAD_LEN = 0x0124,
    /* Message to transmit in bad format. 32 bits. */
    PHY_PARAM_TX_BAD_FORMAT = 0x0128,
    /* Timeout error in transmission. 32 bits. */
    PHY_PARAM_TX_TIMEOUT = 0x012C,
    /* Received correctly messages count. 32 bits. */
    PHY_PARAM_RX_TOTAL = 0x0130,
    /* Received bytes count. 32 bits. */
    PHY_PARAM_RX_TOTAL_BYTES = 0x0134,
    /* Reception RS errors count. 32 bits. */
    PHY_PARAM_RX_RS_ERRORS = 0x0138,
    /* Reception Exceptions count. 32 bits. */
    PHY_PARAM_RX_EXCEPTIONS = 0x013C,
    /* Bad len in message (too short - too long). 32 bits. */
    PHY_PARAM_RX_BAD_LEN = 0x0140,
    /* Bad CRC in received FCH. 32 bits. */
    PHY_PARAM_RX_BAD_CRC_FCH = 0x0144,
    /* CRC correct but invalid protocol. 32 bits. */
    PHY_PARAM_RX_FALSE_POSITIVE = 0x0148,
    /* Received message in bad format. 32 bits. */
    PHY_PARAM_RX_BAD_FORMAT = 0x014C,
    /* Time between noise captures (in ms). 32 bits. */
    PHY_PARAM_TIME_BETWEEN_NOISE_CAPTURES = 0x0158,
    /* Auto detect impedance */
    PHY_PARAM_CFG_AUTODETECT_BRANCH = 0x0161,
    /* Manual impedance configuration */
    PHY_PARAM_CFG_IMPEDANCE = 0x0162,
    /* Indicate if notch filter is active or not. 8 bits. */
    PHY_PARAM_RRC_NOTCH_ACTIVE = 0x0163,
    /* Index of the notch filter. 8 bits. */
    PHY_PARAM_RRC_NOTCH_INDEX = 0x0164,
    /* Enable periodic noise autodetect and adaptation. 8 bits. */
    PHY_PARAM_ENABLE_AUTO_NOISE_CAPTURE = 0x0166,
    /* Noise detection timer reloaded after a correct reception. 8 bits. */
    PHY_PARAM_DELAY_NOISE_CAPTURE_AFTER_RX = 0x0167,
    /* Disable PLC Tx/Rx. 8 bits. */
    PHY_PARAM_PLC_DISABLE = 0x016A,
    /* Indicate noise power in dBuV for the noisier carrier */
    PHY_PARAM_NOISE_PEAK_POWER = 0x016B,
    /* LQI value of the last received message */
    PHY_PARAM_LAST_MSG_LQI = 0x016C,
    /* LQI value of the last received message */
    PHY_PARAM_LAST_MSG_RSSI = 0x016D,
    /* Success transmission of ACK packets */
    PHY_PARAM_ACK_TX_CFM = 0x016E,
    /* Inform PHY layer about enabled modulations on TMR */
    PHY_PARAM_TONE_MAP_RSP_ENABLED_MODS = 0x0174
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
    MAC_RT_MOD_DBPSK_BPSK = 0x01,
    MAC_RT_MOD_DQPSK_QPSK = 0x02,
    MAC_RT_MOD_D8PSK_8PSK = 0x03,
    MAC_RT_MOD_16_QAM = 0x04
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
    uint16_t ft : 3;
    /* Security Enable */
    uint16_t se : 1;
    /* Frame pending */
    uint16_t fp : 1;
    /* Ack Request */
    uint16_t ackr : 1;
    /* PAN ID Compression */
    uint16_t pic : 1;
    /* Reserved */
    uint16_t res : 3;
    /* Destination Addressing Mode */
    uint16_t dam : 2;
    /* Frame Version  */
    uint16_t fv : 2;
    /* Source Addressing Mode */
    uint16_t sam : 2;  
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
    uint8_t res : 4;
    /* Tone Map Request */
    uint8_t tmr : 1;
    /* Contention Control */
    uint8_t cc : 1;
    /* Channel Access Priority */
    uint8_t cap : 1;
    /* Last Segment Flag */
    uint8_t lsf : 1;
    /* Segment Count */
    uint16_t sc : 6;
    /* Segment Length of MAC frame */
    uint16_t sl : 10;  
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
    uint8_t se : 3;
    /* Key Identifier Mode */
    uint8_t kim : 2;
    /* Reserved */
    uint8_t res : 3;
    /* Frame Counter */
    uint32_t fc;
    /* Key Identifier */
    uint8_t ki;
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
    uint8_t value[MAC_RT_PIB_MAX_VALUE_LENGTH];
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
    MAC_RT_MODE_TYPE modType;
    /* Modulation Scheme */
    MAC_RT_MODE_SCHEME modScheme;
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
    /* Modulation Type */
    MAC_RT_MOD_TYPE modType;
    /* Flag to indicate Timestamp should be updated */
    bool updateTimestamp;
} MAC_RT_TX_CFM_OBJ;

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // #ifndef DRV_PL360_MACRT_COMM_H

/*******************************************************************************
 End of File
*/
