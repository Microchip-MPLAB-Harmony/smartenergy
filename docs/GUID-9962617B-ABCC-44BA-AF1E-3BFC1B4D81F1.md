# DRV_METROLOGY_CalibrationIsCompleted Function

## C

```c
bool DRV_METROLOGY_CalibrationIsCompleted(void);
```

## Summary

Checks if the calibration process is finished. 

## Description

This routine should be called to know if the calibration process is finished.

## Parameters

None.

## Returns

True if the calibration process has been completed, otherwise False.

## Example

```c
    case APP_METROLOGY_STATE_CHECK_CALIBRATION:
    {
        /* Wait for the metrology semaphore to get measurements at the end of the integration period. */
        OSAL_SEM_Pend(&appMetrologySemID, OSAL_WAIT_FOREVER);
        
        DRV_METROLOGY_UpdateCalibration();
        
        if (DRV_METROLOGY_CalibrationIsCompleted())
        {
            bool result = DRV_METROLOGY_GetCalibrationResult();
            
            if (app_metrologyData.pCalibrationCallback)
            {
                app_metrologyData.pCalibrationCallback(result);
            }
            
            app_metrologyData.state = APP_METROLOGY_STATE_RUNNING;
        }
        
        vTaskDelay(10 / portTICK_PERIOD_MS);
        break;
    }

```

## Remarks

None.

