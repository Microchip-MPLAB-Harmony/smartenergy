# DRV_PLC_PHY_RECEPTION_OBJ Struct

## C

```c
typedef struct __attribute__((packed, aligned(1))) {
       /* Pointer to received data buffer */
      uint8_t *pReceivedData;
      /* Instant when frame was received (end of message) referred to 1us PHY counter */
      uint32_t time;
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
```

## Summary

Contains information related to a received message.

## Description

This structure contains necessary information of a received frame, such as quality, modulation, ...

## Remarks

None.
