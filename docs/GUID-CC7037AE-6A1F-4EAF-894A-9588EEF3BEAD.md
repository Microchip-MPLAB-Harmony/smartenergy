# DRV_PLC_PHY_Open Function

## C

```c
SYS_MODULE_OBJ DRV_PLC_PHY_Open (
    const SYS_MODULE_INDEX index,
    const DRV_PLC_BOOT_DATA_CALLBACK callback
);
```

## Summary

Opens the specified PLC PHY driver instance and returns a handle to it.

## Description

This routine opens the specified PLC PHY driver instance and provides a handle that must be provided to all other client-level operations to identify the caller and the instance of the driver.   
This driver is a single client driver, so *DRV_PLC_PHY_Open* API should be called only once until driver is closed.

## Precondition

Function [*DRV_PLC_PHY_Initialize*](GUID-3FE78AB9-5672-4748-BEEE-ADD364C8774A.html) must have been called before calling this function.

## Parameters

| Param | Description |
|:----- |:----------- |
| index | Index for the instance to be opened. As driver is single instance, index '0' must always be used. |
| callback | Boot Data Callback Function Pointer. In case of use NULL, *.binStartAddress* and *.binEndAddress* fields must be configured in initialization data *DRV_PLC_PHY_INIT*. |

## Returns

If successful, the routine returns a valid open-instance handle (a number identifying both the caller and the module instance).
As driver is single instance, handle '0' is always returned.    
If an error occurs, the return value is DRV_HANDLE_INVALID. Error can occur:
- if the  driver has been already opened once and in use.
- if the driver peripheral instance being opened is not initialized or is invalid.

## Example

```c
DRV_HANDLE handle;

handle = DRV_PLC_PHY_Open(DRV_PLC_PHY_INDEX_0, NULL);
if (handle == DRV_HANDLE_INVALID)
{
    // Unable to open the driver
    // May be the driver is not initialized
}
```

## Remarks

The handle returned is valid until the [*DRV_PLC_PHY_Close*](GUID-CC2FC1BB-40F6-41C6-9676-40E1BA7DB412.html) routine is called.   
This routine will NEVER block waiting for hardware.

