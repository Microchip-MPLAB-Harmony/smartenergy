# DRV_PLC_PHY_Tasks Function

## C

```c
void DRV_PLC_PHY_Tasks (
    SYS_MODULE_OBJ object
);
```

## Summary

Maintains the driver's state machine.

## Description

This function is used to maintain the driver's internal state machine.

## Precondition

[*DRV_PLC_PHY_Initialize*](GUID-3FE78AB9-5672-4748-BEEE-ADD364C8774A.html) must have been called before calling this function.

## Parameters

| Param | Description |
|:----- |:----------- |
| object | Object handle for the specified driver instance (returned from [*DRV_PLC_PHY_Initialize*](GUID-3FE78AB9-5672-4748-BEEE-ADD364C8774A.html)) |

## Returns

None.

## Example

```c
SYS_MODULE_OBJ      object;     // Returned from DRV_PLC_PHY_Initialize

while (true)
{
    DRV_PLC_PHY_Tasks (object);

    // Do other tasks
}
```

## Remarks

This function is normally not called directly by an application. It is called by the system's Tasks routine (SYS_Tasks).  
This function will never block or access any resources that may cause it to block.
