# DRV_METROLOGY_AFE_DATA Struct

## C

```c
typedef struct {
    uint32_t energy;
    DRV_METROLOGY_AFE_EVENTS afeEvents;
    uint32_t RMS[RMS_TYPE_NUM];
} DRV_METROLOGY_AFE_DATA;
```

## Summary

Identifies the data calculated from the metrology AFE measurements.

## Description

Field description:
- energy. Active energy calculated value.
- afeEvents. AFE events data.
- RMS[RMS_TYPE_NUM]. RMS calculated values.

## Remarks

None.

