# SRV_PSNIFFER_SetRxPayloadSymbols Function

## C

```c
void SRV_PSNIFFER_SetRxPayloadSymbols
(
    uint16_t payloadSym
);
```

## Summary

Sets the number of received symbols on Sniffer library for further use.

## Description

Number of received symbols is set on Sniffer library so that info can be later serialized along with other reception parameters.

## Precondition

None.

## Parameters

| Param | Description |
|:----- |:----------- |
| payloadSym | Number of symbols |

## Returns

None.

## Example

```c
static void APP_PLCDataIndCb(DRV_PLC_PHY_RECEPTION_OBJ *indObj, uintptr_t context)
{
    /* Report RX Symbols */
    appData.plcPIB.id = PLC_ID_RX_PAY_SYMBOLS;
    appData.plcPIB.length = 2;
    DRV_PLC_PHY_PIBGet(appData.drvPl360Handle, &appData.plcPIB);

    SRV_PSNIFFER_SetRxPayloadSymbols(*(uint16_t *)appData.plcPIB.pData);

    /* Serialize received message */
    length = SRV_PSNIFFER_SerialRxMessage(appData.pSerialData, indObj);
    /* Send through USI */
    SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_SNIFF_G3,
            appData.pSerialData, length);
}
```

## Remarks

None.
