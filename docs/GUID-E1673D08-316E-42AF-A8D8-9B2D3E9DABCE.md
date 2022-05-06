# DRV_PLC_PHY_Status Function

## C

```c
SYS_STATUS DRV_PLC_PHY_Status (
    const SYS_MODULE_INDEX index
);
```

## Summary

Gets the current status of the PLC driver module.

## Description

This routine provides the current status of the PLC driver module.

## Precondition

[*DRV_PLC_PHY_Initialize*](GUID-3FE78AB9-5672-4748-BEEE-ADD364C8774A.html) should have been called before calling this function.

## Parameters

| Param | Description |
|:----- |:----------- |
| index | Identifier for the instance used to initialize driver |

## Returns

| Status | Description |
|:----- |:----------- |
| SYS_STATUS_READY | Driver is ready to accept requests for new operations |
| SYS_STATUS_UNINITIALIZED | Driver is not initialized |
| SYS_STATUS_ERROR | Driver is not initialized correctly |
| SYS_STATUS_BUSY | Driver is initializing |

## Example

```c
SYS_STATUS status;
// 'DRV_PLC_PHY_INDEX' is the one used before for Driver Initialization
status = DRV_PLC_PHY_Status(DRV_PLC_PHY_INDEX);
```

## Remarks

None.
