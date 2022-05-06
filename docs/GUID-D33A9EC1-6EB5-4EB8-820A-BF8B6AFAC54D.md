# DRV_PLC_PHY_DataIndCallbackRegister Function

## C

```c
void DRV_PLC_PHY_DataIndCallbackRegister (
    const DRV_HANDLE handle,
    const DRV_PLC_PHY_DATA_IND_CALLBACK callback, 
    const uintptr_t context 
);
```

## Summary

Allows a client to set a data indication event handling function for the driver to call back when a packet reception has finished.

## Description

This function allows a client to register a MAC RT data indication event handling function with the driver to call back when a PLC data reception event occurs.   
Before this callback is set, any received frame by the PLC transceiver will not be notified.   
The callback once set, persists until the client closes the driver or sets another callback (which could be a *NULL* pointer to indicate no callback).

## Precondition

[*DRV_PLC_PHY_Open*](GUID-CC7037AE-6A1F-4EAF-894A-9588EEF3BEAD.html) must have been called to obtain a valid opened device handle.

## Parameters

| Param | Description |
|:----- |:----------- |
| handle | A valid instance handle, returned from the driver [*DRV_PLC_PHY_Open*](GUID-CC7037AE-6A1F-4EAF-894A-9588EEF3BEAD.html) routine |
| callback | Pointer to the callback function |
| context | The value of parameter will be passed back to the client unchanged, when the callback function is called |

## Returns

None.

## Example

```c
// Event is received when a frame is received
void APP_PLC_Data_Ind_callback(DRV_PLC_PHY_DATA_IND_OBJ *indObj, uintptr_t context)
{
    // The context handle was set to an application specific
    // object. It is now retrievable easily in the event handler.
    MY_APP_OBJ myAppObj = (MY_APP_OBJ *) context;

    // Reception handling here.

}
    
// myAppObj is an application specific state data object.
MY_APP_OBJ myAppObj;

// myHandle is the handle returned from DRV_PLC_PHY_Open API.

// Client registers a data confirm callback with driver. This is done once

DRV_PLC_PHY_DataIndCallbackRegister( myHandle, APP_PLC_Data_Ind_callback, (uintptr_t)&myAppObj );

// Event is received when PLC data is receiving.
```

## Remarks

None.

