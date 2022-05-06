# SRV_PSERIAL_SerialCfmMessage Function

## C

```c
size_t SRV_PSERIAL_SerialCfmMessage
(
    uint8_t* pDataDst,
    DRV_PLC_PHY_TRANSMISSION_CFM_OBJ* pDataSrc
);
```

## Summary

Serializes the result of a PLC transmitted frame.

## Description

Takes a *DRV_PLC_PHY_TRANSMISSION_CFM_OBJ* object as parameter, and builds a serialized frame containing the PLC transmission result and parameters, see [*G3 DRV_PLC_PHY_TRANSMISSION_CFM_OBJ Struct*](GUID-F65CE03F-4B50-4092-AB96-CA7DE388B46B.html) and [*PRIME DRV_PLC_PHY_TRANSMISSION_CFM_OBJ Struct*](GUID-D0C15B1D-39F9-4EE2-8C8E-E0D13695B096.html).

## Precondition

None.

## Parameters

| Param | Description |
|:----- |:----------- |
| pDataDst | Pointer to buffer where frame will be serialized |
| pDataSrc | Pointer to a *DRV_PLC_PHY_TRANSMISSION_CFM_OBJ* object containing the PLC transmission result and parameters |

## Returns

Length of serialized frame.

## Example

```c
static void APP_PLCDataCfmCb(DRV_PLC_PHY_TRANSMISSION_CFM_OBJ *cfmObj, uintptr_t context)
{
    size_t length;

    /* Serialize received message */
    length = SRV_PSERIAL_SerialCfmMessage(appData.pSerialData, cfmObj);
    /* Send through USI */
    SRV_USI_Send_Message(appData.srvUSIHandle, SRV_USI_PROT_ID_PHY,
            appData.pSerialData, length);
}
```

## Remarks

None.
