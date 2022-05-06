# DRV_PLC_PHY_TRANSMISSION_OBJ Struct

## C

```c
typedef struct __attribute__((packed, aligned(1))) {
    /* Pointer to data buffer to transmit */
    uint8_t *pTransmitData;
    /* Instant when transmission has to start referred to 1us PHY counter */
    uint32_t time;
    /* Length of the data to transmit in bytes */
    uint16_t dataLength;
    /* Transmission Mode (absolute, relative, cancel, continuous). Constants above */
    uint8_t mode;
    /* Attenuation level with which the message will be transmitted */
    uint8_t attenuation;
    /* Forced transmission */
    uint8_t forced;
    /* Buffer Id used for transmission */
    DRV_PLC_PHY_BUFFER_ID bufferId;
    /* Scheme of Modulation */
    DRV_PLC_PHY_SCH scheme;
    /* PRIME Frame type */
    DRV_PLC_PHY_FRAME_TYPE frameType;
} DRV_PLC_PHY_TRANSMISSION_OBJ;
```

## Summary

PRIME Transmission setup data.

## Description

This structure contains the information of a transmission, including parameters of transmission and data to be sent.

## Remarks

None.
