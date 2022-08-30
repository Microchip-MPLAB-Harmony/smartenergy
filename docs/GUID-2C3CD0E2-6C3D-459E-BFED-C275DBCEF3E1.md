# DRV_METROLOGY_GetAccData Function

## C

```c
DRV_METROLOGY_ACCUMULATORS * DRV_METROLOGY_GetAccData (void);
```

## Summary

Get the pointer to the accumulator registers of the metrology library application running on the second processor. 

## Description

Accumulator registers are acting as 64-bit metrology output accumulator registers.
All values are integrated at an equivalent sampling rate of 4.000KHz. The Metrology library shall
generate the following primary output measurement quantities for each measurement interval, per-phase quantities
with each phase designated by the subscript “_x” (where x = [A, B, or C] for voltage channels and x = [A, B, C or N]
for current channels)
For a detailed description of the control registers, refer to "drv_metrology_regs.h" header file.

## Parameters

None.

## Returns

Pointer to the 32-bit metrology input control registers.

## Example

```c
    /* Get Pointers to metrology data regions */
    app_metrologyData.pMetControl = DRV_METROLOGY_GetControl();
    app_metrologyData.pMetStatus = DRV_METROLOGY_GetStatus();
    app_metrologyData.pMetAccData = DRV_METROLOGY_GetAccData();
    app_metrologyData.pMetHarData = DRV_METROLOGY_GetHarData();
```

## Remarks

None.

