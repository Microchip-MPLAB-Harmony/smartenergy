# DRV_G3_MACRT_CommStatusCallbackRegister Function

## C

```c
void DRV_G3_MACRT_CommStatusCallbackRegister (
    const DRV_HANDLE handle,
    const DRV_G3_MACRT_COMM_STATUS_IND_CALLBACK callback
);
```

## Summary

Allows a client to set a Comm Status indication event handling function for the driver to call back when a Comm Status event has to be reported.

## Description

This function allows a client to register a MAC RT Comm Status indication event handling function with the driver to call back when a Comm Status event occurs.   
Comm Status events are defined in IEEE 802.15.4 standard.   
The callback once set, persists until the client closes the driver or sets another callback (which could be a *NULL* pointer to indicate no callback).

## Precondition

[*DRV_G3_MACRT_Open*](GUID-CFC0A3D7-6B3D-4D47-A061-7314346BFFCF.html) must have been called to obtain a valid opened device handle.

## Parameters

| Param | Description |
|:----- |:----------- |
| handle | A valid instance handle, returned from the driver [*DRV_G3_MACRT_Open*](GUID-CFC0A3D7-6B3D-4D47-A061-7314346BFFCF.html) routine |
| callback | Pointer to the callback function |

## Returns

None.

## Example

```c
void APP_G3_MAC_RT_Comm_Status_callback(uint8_t *pData)
{
    // Extract Comm Status Data from packet.
}

// myAppObj is an application specific state data object.
MY_APP_OBJ myAppObj;

// Client registers a Comm Status indication callback with driver. This is done once
// 'myHandle', returned from DRV_G3_MACRT_Open previously called
DRV_G3_MACRT_CommStatusCallbackRegister( myAppObj.myHandle, APP_G3_MAC_RT_Comm_Status_callback );
```

## Remarks

None.

