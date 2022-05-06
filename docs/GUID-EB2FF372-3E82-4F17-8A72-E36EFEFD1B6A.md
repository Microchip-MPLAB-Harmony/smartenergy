# DRV_PLC_PHY_TxRequest Function

## C

```c
void DRV_PLC_PHY_TxRequest (
    const DRV_HANDLE handle,
    DRV_PLC_PHY_TRANSMISSION_OBJ *transmitObj
);
```

## Summary

Allows a client to transmit PLC PHY data through Power Line (PLC).

## Description

This routine sends a new data message through PLC using the PLC PHY driver.

## Precondition

[*DRV_PLC_PHY_Open*](GUID-CC7037AE-6A1F-4EAF-894A-9588EEF3BEAD.html) must have been called to obtain a valid opened device handle.
[*DRV_PLC_PHY_TRANSMISSION_OBJ*](.html) must be configured before to send any data message.

## Parameters

| Param | Description |
|:----- |:----------- |
| handle | A valid instance handle, returned from the driver [*DRV_PLC_PHY_Open*](GUID-CC7037AE-6A1F-4EAF-894A-9588EEF3BEAD.html) routine |
| transmitObj | Pointer to the object to transmit |

## Returns

None.

## Example

```c
// 'handle', returned from DRV_PLC_PHY_Open previously called

// Local function implemented in the user application
_setupTransmissionParameters();

memcpy(transmitObj.data, src_buf, data_len);
transmitObj.length = data_len;

DRV_PLC_PHY_TxRequest(handle, &transmitObj);
```

