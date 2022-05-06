# DRV_G3_MACRT_MacSnifferCallbackRegister Function

## C

```c
void DRV_G3_MACRT_MacSnifferCallbackRegister (
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_MAC_SNIFFER_CALLBACK callback, 
    uint8_t* pDataBuffer
);
```

## Summary

Allows enabling G3 MAC RT Sniffer capabilities and set the data buffer in which the content of the sniffer packet will be stored when a new PLC message is received.

## Description

This function allows a client to register a G3 MAC RT Sniffer event handling function with the driver to call back when either a data reception event or confirmation of the last transmission event occurs.   
A MAC RT Sniffer will receive any frame in the PLC medium, regardless of addressing or frame type.   
The callback once set, persists until the client closes the driver or sets another callback (which could be a *NULL* pointer to indicate no callback).

## Precondition

[*DRV_G3_MACRT_Open*](GUID-CFC0A3D7-6B3D-4D47-A061-7314346BFFCF.html) must have been called to obtain a valid opened device handle.

## Parameters

| Param | Description |
|:----- |:----------- |
| handle | A valid instance handle, returned from the driver [*DRV_G3_MACRT_Open*](GUID-CFC0A3D7-6B3D-4D47-A061-7314346BFFCF.html) routine |
| callback | Pointer to the callback function |
| pDataBuffer | Pointer to buffer where sniffer message is stored. Data buffer should be defined as MAC_RT_PHY_DATA_MAX_SIZE to avoid memory overlaps. |

## Returns

None.

## Example

```c
void APP_G3_MAC_RT_Mac_Sniffer_callback(uint8_t *pData, uint16_t length)
{
    if (length)
    {
        // Mac Sniffer data handling here.
    }
}

// myAppObj is an application specific state data object.
MY_APP_OBJ myAppObj;

// Client registers a sniffer callback with driver. This is done once
// 'myHandle', returned from DRV_G3_MACRT_Open previously called
DRV_G3_MACRT_MacSnifferCallbackRegister( myAppObj.myHandle, APP_G3_MAC_RT_Mac_Sniffer_callback, myAppObj.pMacSnfBuffer );
```

## Remarks

None.

