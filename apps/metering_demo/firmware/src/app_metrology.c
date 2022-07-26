/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_metrology.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"
#include "app_metrology.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* Define a semaphore to signal the Metrology Tasks to process new integration
 * data */
OSAL_SEM_DECLARE(appMetrologySemID);

extern QueueHandle_t appDatalogQueueID;
APP_DATALOG_QUEUE_DATA appMetrologyDatalogQueueData;

extern QueueHandle_t appEnergyQueueID;
extern QueueHandle_t appEventsQueueID;

const char * _met_control_desc[] =
{
  "00 STATE_CTRL",
  "01 FEATURE_CTRL0",
  "02 FEATURE_CTRL1",
  "03 METER_TYPE",
  "04 M",
  "05 N_MAX",
  "06 PULSE_CTRL0",
  "07 PULSE_CTRL1",
  "08 PULSE_CTRL2",
  "09 P_K_T",
  "10 Q_K_T",
  "11 I_K_T",
  "12 CREEP_THR_P",
  "13 CREEP_THR_Q",
  "14 CREEP_THR_I",
  "15 POWER_OS_CTRL",
  "16 POWER_OFFSET_P",
  "17 POWER_OFFSET_Q",
  "18 SWELL_THR_VA",
  "19 SWELL_THR_VB",
  "20 SWELL_THR_VC",
  "21 SAG_THR_VA",
  "22 SAG_THR_VB",
  "23 SAG_THR_VC",
  "24 K_IA",
  "25 K_VA",
  "26 K_IB",
  "27 K_VB",
  "28 K_IC",
  "29 K_VC",
  "30 K_IN",
  "31 CAL_M_IA",
  "32 CAL_M_VA",
  "33 CAL_M_IB",
  "34 CAL_M_VB",
  "35 CAL_M_IC",
  "36 CAL_M_VC",
  "37 CAL_M_IN",
  "38 CAL_PH_IA",
  "39 CAL_PH_VA",
  "40 CAL_PH_IB",
  "41 CAL_PH_VB",
  "42 CAL_PH_IC",
  "43 CAL_PH_VC",
  "44 CAL_PH_IN",
  "45 CAPTURE_CTRL",
  "46 CAPT_BUFF_SIZE",
  "47 CAPTURE_ADDR",
  "48 RESERVED_C48",
  "49 RESERVED_C49",
  "50 RESERVED_C50",
  "51 AT_CTRL_20_23",
  "52 AT_CTRL_24_27",
  "53 AT_CTRL_28_2B",
  "54 RESERVED_C54",
  "55 POWER_OS_P_A",
  "56 POWER_OS_P_B",
  "57 POWER_OS_P_C",
  "58 POWER_OS_Q_A",
  "59 POWER_OS_Q_B",
  "60 POWER_OS_Q_C"
};

const char * _met_status_desc[] =
{
  "00 VERSION",
  "01 STATUS",
  "02 STATE_FLAG",
  "03 CAPTURE_STATUS",
  "04 INTERVAL_NUM",
  "05 N",
  "06 PH_OFFSET",
  "07 FREQ",
  "08 FREQ_VA",
  "09 FREQ_VB",
  "10 FREQ_VC",
  "11 RESERVED_S11",
  "12 TEMPERATURE",
  "13 I_A_MAX",
  "14 I_B_MAX",
  "15 I_C_MAX",
  "16 I_Nm_MAX",
  "17 I_Ni_MAX",
  "18 V_A_MAX",
  "19 V_B_MAX",
  "20 V_C_MAX",
  "21 FEATURES",
  "22 RESERVED_S22",
  "23 RESERVED_S23",
  "24 RESERVED_S24",
  "25 RESERVED_S25",
  "26 RESERVED_S26",
  "27 RESERVED_S27",
  "28 RESERVED_S28",
  "29 RESERVED_S29",
  "30 ZC_N_VA",
  "31 ZC_N_VB",
  "32 ZC_N_VC",
  "33 AT_CAL_41_44",
  "34 AT_CAL_45_48"
};

const char * _met_acc_desc[] =
{
  "00 I_A",
  "01 I_B",
  "02 I_C",
  "03 I_Ni",
  "04 I_Nm",
  "05 I_A_F",
  "06 I_B_F",
  "07 I_C_F",
  "08 I_Nmi",
  "09 RESERVED_A09",
  "10 RESERVED_A10",
  "11 RESERVED_A11",
  "12 RESERVED_A12",
  "13 RESERVED_A13",
  "14 RESERVED_A14",
  "15 P_A",
  "16 P_B",
  "17 P_C",
  "18 P_A_F",
  "19 P_B_F",
  "20 P_C_F",
  "21 P_N",
  "22 P_N_F",
  "23 RESERVED_A23",
  "24 Q_A",
  "25 Q_B",
  "26 Q_C",
  "27 Q_A_F",
  "28 Q_B_F",
  "29 Q_C_F",
  "30 Q_N",
  "31 Q_N_F",
  "32 RESERVED_A32",
  "33 V_A",
  "34 V_B",
  "35 V_C",
  "36 RESERVED_A36",
  "37 V_A_F",
  "38 V_B_F",
  "39 V_C_F",
  "40 RESERVED_A40",
  "41 V_AB",
  "42 V_BC",
  "43 V_CA",
  "44 V_AB_F",
  "45 V_BC_F",
  "46 V_CA_F",
  "47 RESERVED_A47",
  "48 RESERVED_A48",
  "49 RESERVED_A49",
  "50 ACC_T0",
  "51 ACC_T1",
  "52 ACC_T2"
};

const char * _met_har_desc[] =
{
  "00 I_A_m_R",
  "01 V_A_m_R",
  "02 I_B_m_R",
  "03 V_B_m_R",
  "04 I_C_m_R",
  "05 V_C_m_R",
  "06 I_A_m_I",
  "07 V_A_m_I",
  "08 I_B_m_I",
  "09 V_B_m_I",
  "10 I_C_m_I",
  "11 V_C_m_I"
};

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_METROLOGY_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_METROLOGY_DATA CACHE_ALIGN app_metrologyData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void _APP_METROLOGY_NewIntegrationCallback(void)
{
    if ((app_metrologyData.state == APP_METROLOGY_STATE_RUNNING) ||
        (app_metrologyData.state == APP_METROLOGY_STATE_CHECK_CALIBRATION))
    {
        /* Signal Metrology thread to update measurements for an integration period */
        OSAL_SEM_PostISR(&appMetrologySemID);
    }
}

static void _APP_METROLOGY_GetNVMDataCallback(APP_DATALOG_RESULT result)
{
    if (result == APP_DATALOG_RESULT_SUCCESS)
    {
        app_metrologyData.dataIsRdy = true;
    }

    // Post semaphore to wakeup task
    OSAL_SEM_Post(&appMetrologySemID);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
static void _APP_METROLOGY_LoadControlFromMemory(DRV_METROLOGY_CONTROL * controlReg)
{
    appMetrologyDatalogQueueData.userId = APP_DATALOG_USER_METROLOGY;
    appMetrologyDatalogQueueData.operation = APP_DATALOG_READ;
    appMetrologyDatalogQueueData.pData = (uint8_t *)controlReg;
    appMetrologyDatalogQueueData.dataLen = sizeof(DRV_METROLOGY_CONTROL);
    appMetrologyDatalogQueueData.endCallback = _APP_METROLOGY_GetNVMDataCallback;
    appMetrologyDatalogQueueData.date.month = APP_DATALOG_INVALID_MONTH;
    appMetrologyDatalogQueueData.date.year = APP_DATALOG_INVALID_YEAR;

    xQueueSend(appDatalogQueueID, &appMetrologyDatalogQueueData, (TickType_t) 0);
}

static void _APP_METROLOGY_StoreControlInMemory(DRV_METROLOGY_CONTROL * controlReg)
{
    appMetrologyDatalogQueueData.userId = APP_DATALOG_USER_METROLOGY;
    appMetrologyDatalogQueueData.operation = APP_DATALOG_WRITE;
    appMetrologyDatalogQueueData.pData = (uint8_t *)controlReg;
    appMetrologyDatalogQueueData.dataLen = sizeof(DRV_METROLOGY_CONTROL);
    appMetrologyDatalogQueueData.endCallback = NULL;
    appMetrologyDatalogQueueData.date.month = APP_DATALOG_INVALID_MONTH;
    appMetrologyDatalogQueueData.date.year = APP_DATALOG_INVALID_YEAR;

    xQueueSend(appDatalogQueueID, &appMetrologyDatalogQueueData, (TickType_t) 0);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_METROLOGY_Initialize (void)

  Remarks:
    See prototype in app_metrology.h.
 */

void APP_METROLOGY_Initialize (void)
{
    /* Place the App state machine in its initial state. */
    app_metrologyData.state = APP_METROLOGY_STATE_WAITING_DATALOG;
    
    /* Detection of the WDOG0 Reset */
    if (RSTC_ResetCauseGet() == RSTC_SR_RSTTYP(RSTC_SR_RSTTYP_WDT0_RST_Val))
    {
        app_metrologyData.startMode = DRV_METROLOGY_START_SOFT;
    }
    else
    {
        app_metrologyData.startMode = DRV_METROLOGY_START_HARD;        
    }

    /* Get Pointers to metrology data regions */
    app_metrologyData.pMetControl = DRV_METROLOGY_GetControl();
    app_metrologyData.pMetStatus = DRV_METROLOGY_GetStatus();
    app_metrologyData.pMetAccData = DRV_METROLOGY_GetAccData();
    app_metrologyData.pMetHarData = DRV_METROLOGY_GetHarData();
    
    /* Set Callback for each metrology integration process */
    DRV_METROLOGY_IntegrationCallbackRegister(_APP_METROLOGY_NewIntegrationCallback);
    
    /* Clear Harmonic Analysis Data */
    app_metrologyData.harmonicAnalysisPending = false;
    app_metrologyData.pHarmonicAnalisysCallback = NULL;
    app_metrologyData.pHarmonicAnalysisResponse = NULL;
    
    /* Clear Calibration Data */
    app_metrologyData.pCalibrationCallback = NULL;

    /* Create the Switches Semaphore. */
    if (OSAL_SEM_Create(&appMetrologySemID, OSAL_SEM_TYPE_BINARY, 0, 0) == OSAL_RESULT_FALSE)
    {
        /* Handle error condition. Not sufficient memory to create semaphore */
    }
}


/******************************************************************************
  Function:
    void APP_METROLOGY_Tasks (void)

  Remarks:
    See prototype in app_metrology.h.
 */
void APP_METROLOGY_Tasks (void)
{
    APP_ENERGY_QUEUE_DATA newMetrologyData;
    APP_EVENTS_QUEUE_DATA newEvent;

    /* Check the application's current state. */
    switch (app_metrologyData.state)
    {
        /* Application's initial state. */
        case APP_METROLOGY_STATE_WAITING_DATALOG:
        {
            if (APP_DATALOG_GetStatus() == APP_DATALOG_STATE_READY)
            {
                app_metrologyData.state = APP_METROLOGY_STATE_INIT;
            }

            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_METROLOGY_STATE_INIT:
        {
            if (DRV_METROLOGY_Open(app_metrologyData.startMode) == DRV_METROLOGY_SUCCESS)
            {
                if (app_metrologyData.startMode == DRV_METROLOGY_START_HARD)
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_START;
                }
                else
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_RUNNING;
                }
            }
            else
            {
                app_metrologyData.state = APP_METROLOGY_STATE_ERROR;
            }

            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_METROLOGY_STATE_START:
        {
            if (DRV_METROLOGY_GetState() == DRV_METROLOGY_STATE_READY)
            {
                /* Check if there are Metrology data in memory */
                if (APP_DATALOG_FileExists(APP_DATALOG_USER_METROLOGY, NULL))
                {
                    DRV_METROLOGY_CONTROL controlReg;

                    /* Metrology data exists */
                    _APP_METROLOGY_LoadControlFromMemory(&controlReg);
                    /* Wait for the semaphore to load data from memory */
                    OSAL_SEM_Pend(&appMetrologySemID, OSAL_WAIT_FOREVER);

                    /* Apply Control Data */
                    if (app_metrologyData.dataIsRdy)
                    {
                        DRV_METROLOGY_SetControl(&controlReg);
                    }
                }
                else
                {
                    /* Metrology data does not exists. Store in NVM */
                    _APP_METROLOGY_StoreControlInMemory(app_metrologyData.pMetControl);
                }

                if (DRV_METROLOGY_Start() == DRV_METROLOGY_SUCCESS)
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_RUNNING;
                }
                else
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_ERROR;
                }

                vTaskDelay(10 / portTICK_PERIOD_MS);
            }

            break;
        }

        case APP_METROLOGY_STATE_RUNNING:
        {
            /* Wait for the metrology semaphore to get measurements at the end of the integration period. */
            OSAL_SEM_Pend(&appMetrologySemID, OSAL_WAIT_FOREVER);
            
            if (app_metrologyData.state == APP_METROLOGY_STATE_INIT)
            {
                /* Received Reload Command */
                break;
            }
            else if (app_metrologyData.state == APP_METROLOGY_STATE_CHECK_CALIBRATION)
            {
                /* Received Calibration Command */
                DRV_METROLOGY_StartCalibration();
                /* Discard Measurements of this integration period */
                break;
            }            

            DRV_METROLOGY_UpdateMeasurements();
            
            // Send new Energy values to the Energy Task
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
            
            // Send new Events to the Events Task
            app_metrologyData.queueFree = uxQueueSpacesAvailable(appEventsQueueID);
            if (app_metrologyData.queueFree)
            {
                RTC_TimeGet(&newEvent.eventTime);
                DRV_METROLOGY_GetEventsData(&newEvent.eventFlags);
                xQueueSend(appEventsQueueID, &newEvent, (TickType_t) 0);
            }
            else
            {
                SYS_CMD_MESSAGE("EVENTS Queue is FULL!!!\n\r");
            }
            
            // Check Harmonic Analysis Result and report
            if (app_metrologyData.harmonicAnalysisPending)
            {
                app_metrologyData.state = APP_METROLOGY_STATE_CHECK_HARMONIC_ANALYSIS;
                
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
            
            break;
        }

        case APP_METROLOGY_STATE_CHECK_HARMONIC_ANALYSIS:
        {
            if (DRV_METROLOGY_GetHarmonicAnalysisResult())
            {
                app_metrologyData.harmonicAnalysisPending = false;

                app_metrologyData.pHarmonicAnalisysCallback(app_metrologyData.harmonicAnalysisNum);
            }
            
            app_metrologyData.state = APP_METROLOGY_STATE_RUNNING;
            
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_METROLOGY_STATE_CHECK_CALIBRATION:
        {
            /* Wait for the metrology semaphore to get measurements at the end of the integration period. */
            OSAL_SEM_Pend(&appMetrologySemID, OSAL_WAIT_FOREVER);
            
            DRV_METROLOGY_UpdateCalibration();
            
            if (DRV_METROLOGY_CalibrationIsCompleted())
            {
                bool result = DRV_METROLOGY_GetCalibrationResult();
                
                if (result == true)
                {
                    /* Store CONTROL Regs in NVM */
                    _APP_METROLOGY_StoreControlInMemory(app_metrologyData.pMetControl);
                }
                
                if (app_metrologyData.pCalibrationCallback)
                {
                    app_metrologyData.pCalibrationCallback(result);
                }
                
                app_metrologyData.state = APP_METROLOGY_STATE_RUNNING;
            }
            
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        /* The default state should never be executed. */
        case APP_METROLOGY_STATE_ERROR:
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

bool APP_METROLOGY_GetControlRegister(CONTROL_REG_ID regId, uint32_t * regValue, char *regName)
{
    uint32_t *pData;

    if (regId >= CONTROL_REG_NUM)
    {
        return false;
    }

    pData = (uint32_t *)app_metrologyData.pMetControl;
    pData += regId;
    *regValue = *pData;

    if (regName)
    {
        sprintf(regName,"%s",_met_control_desc[regId]);
    }

    return true;

}

bool APP_METROLOGY_SetControlRegister(CONTROL_REG_ID regId, uint32_t value)
{
    uint32_t *pData;

    if (regId >= CONTROL_REG_NUM)
    {
        return false;
    }
    
    pData = (uint32_t *)app_metrologyData.pMetControl;
    pData += regId;
    *pData = value;

    return true;
}

bool APP_METROLOGY_GetStatusRegister(STATUS_REG_ID regId, uint32_t * regValue, char *regName)
{
    uint32_t *pData;

    if (regId >= STATUS_REG_NUM)
    {
        return false;
    }

    pData = (uint32_t *)app_metrologyData.pMetStatus;
    pData += regId;
    *regValue = *pData;

    if (regName)
    {
        sprintf(regName,"%s",_met_status_desc[regId]);
    }

    return true;
}

bool APP_METROLOGY_GetAccumulatorRegister(ACCUMULATOR_REG_ID regId, uint64_t * regValue, char *regName)
{
    uint64_t *pData;

    if (regId >= ACCUMULATOR_REG_NUM)
    {
        return false;
    }

    pData = (uint64_t *)app_metrologyData.pMetAccData;
    pData += regId;
    *regValue = *pData;

    if (regName)
    {
        sprintf(regName,"%s",_met_acc_desc[regId]);
    }

    return true;
}

bool APP_METROLOGY_GetHarmonicsRegister(HARMONICS_REG_ID regId, uint32_t * regValue, char *regName)
{
    uint32_t *pData;

    if (regId >= HARMONICS_REG_NUM)
    {
        return false;
    }

    pData = (uint32_t *)app_metrologyData.pMetHarData;
    pData += regId;
    *regValue = *pData;

    if (regName)
    {
        sprintf(regName,"%s",_met_har_desc[regId]);
    }

    return true;
}

bool APP_METROLOGY_GetRMS(DRV_METROLOGY_RMS_TYPE rmsId, uint32_t * rmsValue, DRV_METROLOGY_RMS_SIGN * sign)
{
    if (rmsId >= RMS_TYPE_NUM)
    {
        return false;
    }

    if (sign != NULL)
    {
        *sign = DRV_METROLOGY_GetRMSSign(rmsId);
    }

    *rmsValue = DRV_METROLOGY_GetRMSValue(rmsId);
    return true;
}

void APP_METROLOGY_SetControlByDefault(void)
{
    DRV_METROLOGY_CONTROL *pSrc;

    pSrc = DRV_METROLOGY_GetControlByDefault();
    DRV_METROLOGY_SetControl(pSrc);
}

void APP_METROLOGY_StoreMetrologyData(void)
{
    _APP_METROLOGY_StoreControlInMemory(app_metrologyData.pMetControl);
}

void APP_METROLOGY_SetConfiguration(DRV_METROLOGY_CONFIGURATION * config)
{
    DRV_METROLOGY_SetConfiguration(config);

    _APP_METROLOGY_StoreControlInMemory(app_metrologyData.pMetControl);
}

void APP_METROLOGY_StartCalibration(APP_METROLOGY_CALIBRATION * calibration)
{
    DRV_METROLOGY_CALIBRATION * pCalibrationData;
    
    pCalibrationData = DRV_METROLOGY_GetCalibrationData();
    pCalibrationData->references.aimIA = calibration->aimIA;
    pCalibrationData->references.aimVA = calibration->aimVA;
    pCalibrationData->references.angleA = calibration->angleA;
    pCalibrationData->references.aimIB = calibration->aimIB;
    pCalibrationData->references.aimVB = calibration->aimVB;
    pCalibrationData->references.angleB = calibration->angleB;
    pCalibrationData->references.aimIC = calibration->aimIC;
    pCalibrationData->references.aimVC = calibration->aimVC;
    pCalibrationData->references.angleC = calibration->angleC;
    pCalibrationData->references.lineId = calibration->lineId;
    
    app_metrologyData.state = APP_METROLOGY_STATE_CHECK_CALIBRATION;
}

void APP_METROLOGY_SetCalibrationCallback(APP_METROLOGY_CALIBRATION_CALLBACK callback)
{
    app_metrologyData.pCalibrationCallback = callback;
}

size_t APP_METROLOGY_GetWaveformCaptureData(uint32_t *address)
{
    *address = (uint32_t)app_metrologyData.pMetControl->CAPTURE_ADDR;
    return (size_t)app_metrologyData.pMetControl->CAPTURE_BUFF_SIZE;
}

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
    
    DRV_METROLOGY_RequestHarmonicAnalysis(harmonicNum, app_metrologyData.pHarmonicAnalysisResponse);
    
    return true;
}

void APP_METROLOGY_SetHarmonicAnalysisCallback(APP_METROLOGY_HARMONIC_ANALISYS_CALLBACK callback,
        DRV_METROLOGY_HARMONIC * pHarmonicAnalysisResponse)
{
    app_metrologyData.pHarmonicAnalisysCallback = callback;
    app_metrologyData.pHarmonicAnalysisResponse = pHarmonicAnalysisResponse;
}

void APP_METROLOGY_Restart (void)
{   
    app_metrologyData.state = APP_METROLOGY_STATE_INIT;
    app_metrologyData.startMode = DRV_METROLOGY_START_HARD;
    
    DRV_METROLOGY_Close();
    DRV_METROLOGY_Initialize(NULL, RSTC_SR_RSTTYP(RSTC_SR_RSTTYP_SOFT_RST_Val));
    
    OSAL_SEM_PostISR(&appMetrologySemID);
}

void APP_METROLOGY_SetLowPowerMode (void)
{
    SUPC_BackupModeEnter();
}

/*******************************************************************************
 End of File
 */
