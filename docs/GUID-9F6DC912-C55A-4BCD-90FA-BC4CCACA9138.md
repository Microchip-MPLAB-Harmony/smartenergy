# DRV_PLC_PHY_TX_RESULT Enum

## C

```c
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
    /* Transmission result: high temperature error */
    DRV_PLC_PHY_TX_RESULT_HIGH_TEMP_120 = 12,
    /* Transmission result: high temperature warning */
    DRV_PLC_PHY_TX_RESULT_HIGH_TEMP_110 = 13,
    /* Transmission result: No transmission ongoing */
    DRV_PLC_PHY_TX_RESULT_NO_TX = 255,
}DRV_PLC_PHY_TX_RESULT;
```

## Summary

Identifies the possible values that can result from a PLC transmission request.

## Description

This Enum contains every transmission result supported by MCHP implementation.