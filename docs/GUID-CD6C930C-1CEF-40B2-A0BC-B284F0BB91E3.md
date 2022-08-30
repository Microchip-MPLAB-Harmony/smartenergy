# DRV_METROLOGY_HarmonicAnalysisIsRun Function

## C

```c
bool DRV_METROLOGY_HarmonicAnalysisIsRun(void);
```

## Summary

Checks if the Harmonic analysis is finished. 

## Description

This routine should be called to know if the Harmonic analysis is finished.

## Parameters

None.

## Returns

True if the Harmonic analysis is still running, otherwise False.

## Example

```c
bool APP_METROLOGY_StartHarmonicAnalysis(uint8_t harmonicNum)
{
    if (DRV_METROLOGY_HarmonicAnalysisIsRun())
    {
        return false;
    }
    
    if (app_metrologyData.pHarmonicAnalisysCallback == NULL)
    {
        return false;
    }
    
    if (app_metrologyData.pHarmonicAnalysisResponse == NULL)
    {
        return false;
    }
    
    app_metrologyData.harmonicAnalysisPending = true;
    app_metrologyData.harmonicAnalysisNum = harmonicNum;
    
    DRV_METROLOGY_StartHarmonicAnalysis(harmonicNum, app_metrologyData.pHarmonicAnalysisResponse);
    
    return true;
}

```

## Remarks

None.

