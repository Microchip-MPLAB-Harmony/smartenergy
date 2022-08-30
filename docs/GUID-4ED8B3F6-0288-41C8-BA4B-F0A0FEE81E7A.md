# DRV_METROLOGY_GetEnergyValue Function

## C

```c
uint32_t DRV_METROLOGY_GetEnergyValue(bool restartEnergy);
```

## Summary

Gets the active energy value. 

## Description

Gets the active energy value.
Energy value should be previously updated by the DRV_METROLOGY_UpdateMeasurements() routine.

## Parameters

| Param | Description |
|:----- |:----------- |
| restartEnergy | Flag to indicate if the energy value should be restarted or accumulated to the previous calculated value. |


## Returns

The active energy value obtained according to the last call to DRV_METROLOGY_UpdateMeasurements() routine.

## Example

```c
      DRV_METROLOGY_UpdateMeasurements();
            
      /* Send new Energy values to the Energy Task */
      app_metrologyData.queueFree = uxQueueSpacesAvailable(appEnergyQueueID);
      if (app_metrologyData.queueFree)
      {
          newMetrologyData.energy = DRV_METROLOGY_GetEnergyValue(true);
          newMetrologyData.Pt = DRV_METROLOGY_GetRMSValue(RMS_PT);
          xQueueSend(appEnergyQueueID, &newMetrologyData, (TickType_t) 0);
      }
      else
      {
          SYS_CMD_MESSAGE("ENERGY Queue is FULL!!!\n\r");
      }
```

## Remarks

None.

