/*******************************************************************************
  Interface definition of Metrology Driver.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_metrology.h

  Summary:
    Interface definition of the Metrology Driver.

  Description:
    This file defines the interface for the Metrology driver.
*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef DRV_METROLOGY_H    // Guards against multiple inclusion
#define DRV_METROLOGY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "driver/driver.h"
#include "system/system.h"
#include "device.h"
#include "configuration.h"
#include "drv_metrology_definitions.h"

#ifdef __cplusplus // Provide C++ Compatibility
    extern "C" {
#endif

<#if DRV_MET_WAVEFORM_CAPTURE == true>    
#define MET_CAPTURE_BUF_SIZE     DRV_METROLOGY_CAPTURE_BUF_SIZE

</#if>
// *****************************************************************************
/* Metrology Driver Result

  Summary:
    Result of a Metrology driver interface operation.

  Description:
    Identifies the result of certain Metrology driver operations.
*/

typedef enum {
    DRV_METROLOGY_SUCCESS,
    DRV_METROLOGY_ERROR
} DRV_METROLOGY_RESULT;

// *****************************************************************************
/* Metrology Driver Start mode

  Summary:
    Describes how is the startup process.

  Description:
    HARD mode implies a full reload of Core 1 application. SOFT mode does not
    any effects on Core 1 application.
*/
typedef enum {
    DRV_METROLOGY_START_SOFT,
    DRV_METROLOGY_START_HARD
} DRV_METROLOGY_START_MODE;

// *****************************************************************************
/* Metrology Driver RMS sign

  Summary:
    Describes the sign of some metrology measurements.

  Description:
    This quality only affects to some RMS measurements.
*/
typedef enum {
    RMS_SIGN_POSITIVE = 0,
    RMS_SIGN_NEGATIVE = 1,
} DRV_METROLOGY_RMS_SIGN;

// *****************************************************************************
// *****************************************************************************
// Section: Interface
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ DRV_METROLOGY_Initialize (
        SYS_MODULE_INIT * init,
        uint32_t resetValue
    );

  Summary:
    Initializes the metrology driver according to the init parameter and the cause of the reset of the main processor. 

  Description:
    This routine initializes the metrology driver making it ready for clients to open and use.
    The initialization data is specified by the init parameter.
    It is a single instance driver.

  Precondition:
    None.

  Parameters:
    init  - Pointer to the init data structure containing any data necessary to initialize the driver.
    resetValue - Reset cause of the main processor.

  Returns:
    If successful, returns a valid handle to a driver instance object. 
    Otherwise, it returns SYS_MODULE_OBJ_INVALID. 

  Example:
    <code>
    SYS_MODULE_OBJ   sysObjDrvMet;

    extern uint8_t met_bin_start;
    extern uint8_t met_bin_end;

    DRV_METROLOGY_INIT drvMetrologyInitData = {
        .regBaseAddress = DRV_METROLOGY_REG_BASE_ADDRESS,
        .binStartAddress = (uint32_t)&met_bin_start,
        .binEndAddress = (uint32_t)&met_bin_end,

    };

    sysObjDrvMet = DRV_METROLOGY_Initialize((SYS_MODULE_INIT *)&drvMetrologyInitData, RSTC_ResetCauseGet());
    </code>

  Remarks:
    This routine must be called before any other DRV_METROLOGY routine is called. 
*/
SYS_MODULE_OBJ DRV_METROLOGY_Initialize(SYS_MODULE_INIT * init, uint32_t resetValue);

// *****************************************************************************
/* Function:
    DRV_METROLOGY_RESULT DRV_METROLOGY_Open (
        DRV_METROLOGY_START_MODE mode
    );

  Summary:
    Opens the metrology driver according to the mode parameter.  

  Description:
    This routine enables the IPC peripheral and, only if a HARD start mode has been selected, also handles the reset and clock lines for enabling the metrology library application.
    On the other hand, SOFT mode does not any effects on metrology library application running in the second processor.

  Precondition:
    None.

  Parameters:
    mode - Metrology Driver Start mode

  Returns:
    If successful, returns DRV_METROLOGY_SUCCESS. Otherwise, it returns DRV_METROLOGY_ERROR.

  Example:
    <code>
        // Detection of the WDOG0 Reset
        if (RSTC_ResetCauseGet() == RSTC_SR_RSTTYP(RSTC_SR_RSTTYP_WDT0_RST_Val))
        {
            app_metrologyData.startMode = DRV_METROLOGY_START_SOFT;
        }
        else
        {
            app_metrologyData.startMode = DRV_METROLOGY_START_HARD;        
        }

        ...
    
    
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

            break;
        }
    </code>

  Remarks:
    None. 
*/
DRV_METROLOGY_RESULT DRV_METROLOGY_Open(DRV_METROLOGY_START_MODE mode);

// *****************************************************************************
/* Function:
    DRV_METROLOGY_RESULT DRV_METROLOGY_Close (void);

  Summary:
    Closes the metrology driver.  

  Description:
    This routine closes the metrology driver making it unusable.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    If successful, returns DRV_METROLOGY_SUCCESS. Otherwise, it returns DRV_METROLOGY_ERROR. 

  Example:
    <code>
        app_metrologyData.state = APP_METROLOGY_STATE_INIT;
        app_metrologyData.startMode = DRV_METROLOGY_START_HARD;

        DRV_METROLOGY_Close();
        DRV_METROLOGY_Initialize(NULL, RSTC_SR_RSTTYP(RSTC_SR_RSTTYP_SOFT_RST_Val));
    </code>

  Remarks:
    None. 
*/
DRV_METROLOGY_RESULT DRV_METROLOGY_Close (void);

// *****************************************************************************
/* Function:
    DRV_METROLOGY_RESULT DRV_METROLOGY_Start (void);

  Summary:
    Starts the metrology driver.  

  Description:
    This routine shoud be called until the metrology library application running in the second processor is ready to be used.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Returns DRV_METROLOGY_SUCCESS when the metrology library application is ready to be used. Otherwise, it returns DRV_METROLOGY_ERROR.

  Example:
    <code>
        case APP_METROLOGY_STATE_START:
        {
            if (DRV_METROLOGY_Start() == DRV_METROLOGY_SUCCESS)
            {
                app_metrologyData.state = APP_METROLOGY_STATE_RUNNING;
            }
            else
            {
                app_metrologyData.state = APP_METROLOGY_STATE_ERROR;
            }

            break;
        }
    </code>

  Remarks:
    None. 
*/
DRV_METROLOGY_RESULT DRV_METROLOGY_Start(void);

// *****************************************************************************
/* Function:
    DRV_METROLOGY_RESULT DRV_METROLOGY_IntegrationCallbackRegister (
        DRV_METROLOGY_CALLBACK callback 
    );

  Summary:
    Allows a client to set a metrology integration event handling function for the driver to call back when a integration period has completed. 

  Description:
    This function allows a client to register an integration period event handling function with the driver to call back when a integration period has completed.   
    This function must be always called after DRV_METROLOGY_Initialize routine is called, since the initialization routine sets a NULL pointer to indicate no callback.

  Precondition:
    DRV_METROLOGY_Initialize routine must have been called before.

  Parameters:
    callback - Pointer to the callback function.

  Returns:
    If successful, returns DRV_METROLOGY_SUCCESS. Otherwise, it returns DRV_METROLOGY_ERROR. 

  Example:
    <code>
        static void _APP_METROLOGY_NewIntegrationCallback(void)
        {
            if (app_metrologyData.state == APP_METROLOGY_STATE_RUNNING)
            {
                // Signal Metrology thread to update measurements for an integration period 
                OSAL_SEM_PostISR(&appMetrologySemID);
            }
        }

        (...)

        // Set Callback for each metrology integration process 
        DRV_METROLOGY_IntegrationCallbackRegister(_APP_METROLOGY_NewIntegrationCallback);
    </code>

  Remarks:
    None. 
*/
DRV_METROLOGY_RESULT DRV_METROLOGY_IntegrationCallbackRegister(DRV_METROLOGY_CALLBACK callback);
<#if DRV_MET_NOT_FULL_CYCLE == true>  
DRV_METROLOGY_RESULT DRV_METROLOGY_FullCycleCallbackRegister(DRV_METROLOGY_CALLBACK callback);
</#if>
<#if DRV_MET_NOT_HALF_CYCLE == true>  
DRV_METROLOGY_RESULT DRV_METROLOGY_HalfCycleCallbackRegister(DRV_METROLOGY_CALLBACK callback);
</#if>
<#if DRV_MET_RAW_ZERO_CROSSING == true>  
DRV_METROLOGY_RESULT DRV_METROLOGY_ZeroCrossCallbackRegister(DRV_METROLOGY_CALLBACK callback);
</#if>
<#if DRV_MET_PULSE_0 == true>  
DRV_METROLOGY_RESULT DRV_METROLOGY_Pulse0CallbackRegister(DRV_METROLOGY_CALLBACK callback);
</#if>
<#if DRV_MET_PULSE_1 == true>  
DRV_METROLOGY_RESULT DRV_METROLOGY_Pulse1CallbackRegister(DRV_METROLOGY_CALLBACK callback);
</#if>
<#if DRV_MET_PULSE_2 == true>  
DRV_METROLOGY_RESULT DRV_METROLOGY_Pulse2CallbackRegister(DRV_METROLOGY_CALLBACK callback);
</#if>

// *****************************************************************************
/* Function:
    DRV_METROLOGY_STATE DRV_METROLOGY_GetState (void);

  Summary:
    Get the status of the metrology library application running on the second processor.  

  Description:
    Metrology function state:
        - 0 = HALT: Metrology module is halted.
        - 1 = RESET: Resetting metrology module. 
        - 2 = INIT_DSP: Initializing DSP filters.
        - 3 = DSP_READY: DSP filters have been initialized.
        - 4 = INIT_ATSENSE: Initializing ATSENSE.
        - 5 = ATSENSE_READY: ATSENSE has been initialized.
        - 6 = READY: ATSENSE temperature configuration & calibration data has been copied into output registers.
        - 7 = DSP_SETTLING: waiting for DSP filters to stabilize to full accuracy.
        - 8 = DSP_RUNNING: DSP filters have stabilized to full accuracy.
 
    For further information about the state diagram, refer to online documentation.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Returns the metrology state. 

  Example:
    <code>
        case APP_METROLOGY_STATE_START:
        {
            if (DRV_METROLOGY_GetState() == DRV_METROLOGY_STATE_READY)
            {
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
    </code>

  Remarks:
    None. 
*/
DRV_METROLOGY_STATE DRV_METROLOGY_GetState(void);

// *****************************************************************************
/* Function:
    DRV_METROLOGY_STATUS * DRV_METROLOGY_GetStatus (void);

  Summary:
    Get the pointer to the status registers of the metrology library application running on the second processor.  

  Description:
    The Metrology library generates primary 32-bit output measurement quantities for each measurement interval.
    For a detailed description of the status registers, refer to "drv_metrology_regs.h" header file.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Pointer to the 32-bit metrology output measurement quantities for each measurement interval. 

  Example:
    <code>
        // Get Pointers to metrology data regions 
        app_metrologyData.pMetControl = DRV_METROLOGY_GetControl();
        app_metrologyData.pMetStatus = DRV_METROLOGY_GetStatus();
        app_metrologyData.pMetAccData = DRV_METROLOGY_GetAccData();
        app_metrologyData.pMetHarData = DRV_METROLOGY_GetHarData();
    </code>

  Remarks:
    None. 
*/
DRV_METROLOGY_STATUS * DRV_METROLOGY_GetStatus(void);

// *****************************************************************************
/* Function:
    DRV_METROLOGY_CONTROL * DRV_METROLOGY_GetControl (void);

  Summary:
    Get the pointer to the control registers of the metrology library application running on the second processor.  

  Description:
    Control registers are acting as 32-bit metrology input control registers.
    For a detailed description of the control registers, refer to "drv_metrology_regs.h" header file.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Pointer to the 32-bit metrology input control registers.. 

  Example:
    <code>
        // Get Pointers to metrology data regions 
        app_metrologyData.pMetControl = DRV_METROLOGY_GetControl();
        app_metrologyData.pMetStatus = DRV_METROLOGY_GetStatus();
        app_metrologyData.pMetAccData = DRV_METROLOGY_GetAccData();
        app_metrologyData.pMetHarData = DRV_METROLOGY_GetHarData();
    </code>

  Remarks:
    None. 
*/
DRV_METROLOGY_CONTROL * DRV_METROLOGY_GetControl(void);

// *****************************************************************************
/* Function:
    DRV_METROLOGY_CONTROL * DRV_METROLOGY_GetControlByDefault (void);

  Summary:
    Get the pointer to the control registers defined by default via the Microchip Code Configurator (MCC) included in Harmony 3 framework. 

  Description:
    After generating the code application via MCC, a struct data is defined as a constant. It includes all control registers obtained according to the MCC configuration established in the Harmony 3 project.
    The content of each control register is defined in "configuration.h" header file.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Pointer to the control registers defined as the default configuration. 

  Example:
    <code>
      const DRV_METROLOGY_CONTROL gDrvMetControlDefault =
        {
            STATE_CTRL_STATE_CTRL_RESET_Val,                // 00 STATE_CTRL
            _UINT32_(DRV_METROLOGY_CONF_FCTRL0),            // 01 FEATURE_CTRL0
            _UINT32_(DRV_METROLOGY_CONF_FCTRL1),            // 02 FEATURE_CTRL1
            _UINT32_(DRV_METROLOGY_CONF_MT),                // 03 METER_TYPE sensor_type =0 CT, 1 SHUNT, 2 ROGOWSKI
            _UINT32_(0x00000000),                           // 04 M M=50->50Hz M=60->60Hz
            _UINT32_(0x00001130),                           // 05 N_MAX 4400=0x1130
            _UINT32_(DRV_METROLOGY_CONF_PULSE0_CTRL),       // 06 PULSE0_CTRL
            _UINT32_(DRV_METROLOGY_CONF_PULSE1_CTRL),       // 07 PULSE1_CTRL
            _UINT32_(DRV_METROLOGY_CONF_PULSE2_CTRL),       // 08 PULSE2_CTRL
            _UINT32_(DRV_METROLOGY_CONF_PKT),               // 09 P_K_T
            _UINT32_(DRV_METROLOGY_CONF_PKT),               // 10 Q_K_T
            _UINT32_(DRV_METROLOGY_CONF_PKT),               // 11 I_K_T
            _UINT32_(DRV_METROLOGY_CONF_CREEP_P),           // 12 CREEP_THR_P
            _UINT32_(DRV_METROLOGY_CONF_CREEP_Q),           // 13 CREEP_THR_Q
            _UINT32_(DRV_METROLOGY_CONF_CREEP_I),           // 14 CREEP_THR_I
            _UINT32_(0x00000000),                           // 15 POWER_OFFSET_CTRL
            _UINT32_(0x00000000),                           // 16 POWER_OFFSET_P
            _UINT32_(0x00000000),                           // 17 POWER_OFFSET_Q
            _UINT32_(DRV_METROLOGY_CONF_SWELL),             // 18 SWELL_THR_VA
            _UINT32_(DRV_METROLOGY_CONF_SWELL),             // 19 SWELL_THR_VB
            _UINT32_(DRV_METROLOGY_CONF_SWELL),             // 20 SWELL_THR_VC
            _UINT32_(DRV_METROLOGY_CONF_SAG),               // 21 SAG_THR_VA
            _UINT32_(DRV_METROLOGY_CONF_SAG),               // 22 SAG_THR_VB
            _UINT32_(DRV_METROLOGY_CONF_SAG),               // 23 SAG_THR_VC
            _UINT32_(DRV_METROLOGY_CONF_KI),                // 24 K_IA
            _UINT32_(DRV_METROLOGY_CONF_KV),                // 25 K_VA
            _UINT32_(DRV_METROLOGY_CONF_KI),                // 26 K_IB
            _UINT32_(DRV_METROLOGY_CONF_KV),                // 27 K_VB
            _UINT32_(DRV_METROLOGY_CONF_KI),                // 28 K_IC
            _UINT32_(DRV_METROLOGY_CONF_KV),                // 29 K_VC
            _UINT32_(DRV_METROLOGY_CONF_KI),                // 30 K_IN
            _UINT32_(0x20000000),                           // 31 CAL_M_IA
            _UINT32_(0x20000000),                           // 32 CAL_M_VA
            _UINT32_(0x20000000),                           // 33 CAL_M_IB
            _UINT32_(0x20000000),                           // 34 CAL_M_VB
            _UINT32_(0x20000000),                           // 35 CAL_M_IC
            _UINT32_(0x20000000),                           // 36 CAL_M_VC
            _UINT32_(0x20000000),                           // 37 CAL_M_IN
            _UINT32_(0x00000000),                           // 38 CAL_PH_IA
            _UINT32_(0x00000000),                           // 39 CAL_PH_VA
            _UINT32_(0x00000000),                           // 40 CAL_PH_IB
            _UINT32_(0x00000000),                           // 41 CAL_PH_VB
            _UINT32_(0x00000000),                           // 42 CAL_PH_IC
            _UINT32_(0x00000000),                           // 43 CAL_PH_VC
            _UINT32_(0x00000000),                           // 44 CAL_PH_IN
            _UINT32_(DRV_METROLOGY_CONF_WAVEFORM),          // 45 CAPTURE_CTRL
            _UINT32_(DRV_METROLOGY_CAPTURE_BUF_SIZE),       // 46 CAPTURE_BUFF_SIZE
            _UINT32_(0x00000000),                           // 47 CAPTURE_ADDR
            _UINT32_(0x00000000),                           // 48 RESERVED_C48
            _UINT32_(0x00000000),                           // 49 RESERVED_C49
            _UINT32_(DRV_METROLOGY_CONF_ATS2023),           // 51 ATSENSE_CTRL_20_23
            _UINT32_(DRV_METROLOGY_CONF_ATS2427),           // 52 ATSENSE_CTRL_24_27
            _UINT32_(0x00000003),                           // 53 ATSENSE_CTRL_28_2B: MSB_MODE=0,OSR=3
            _UINT32_(0x00000000),                           // 54 RESERVED_C54
            _UINT32_(0x00000000),                           // 55 POWER_OFFSET_P_A
            _UINT32_(0x00000000),                           // 56 POWER_OFFSET_P_B
            _UINT32_(0x00000000),                           // 57 POWER_OFFSET_P_C
            _UINT32_(0x00000000),                           // 58 POWER_OFFSET_Q_A
            _UINT32_(0x00000000),                           // 59 POWER_OFFSET_Q_B
            _UINT32_(0x00000000)                            // 60 POWER_OFFSET_Q_C
        };

        (...) 
 
        DRV_METROLOGY_CONTROL *pSrc;

        pSrc = DRV_METROLOGY_GetControlByDefault();
  
    </code>

  Remarks:
    None. 
*/
DRV_METROLOGY_CONTROL * DRV_METROLOGY_GetControlByDefault(void);

// *****************************************************************************
/* Function:
    DRV_METROLOGY_ACCUMULATORS * DRV_METROLOGY_GetAccData (void);

  Summary:
    Get the pointer to the accumulator registers of the metrology library application running on the second processor. 

  Description:
    Accumulator registers are acting as 64-bit metrology output accumulator registers.
    All values are integrated at an equivalent sampling rate of 4.000KHz. The Metrology library shall
    generate the following primary output measurement quantities for each measurement interval, per-phase quantities
    with each phase designated by the subscript '_x' (where x = [A, B, or C] for voltage channels and x = [A, B, C or N]
    for current channels)
    For a detailed description of the control registers, refer to "drv_metrology_regs.h" header file.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Pointer to the 32-bit metrology input control registers. 

  Example:
    <code>
        // Get Pointers to metrology data regions 
        app_metrologyData.pMetControl = DRV_METROLOGY_GetControl();
        app_metrologyData.pMetStatus = DRV_METROLOGY_GetStatus();
        app_metrologyData.pMetAccData = DRV_METROLOGY_GetAccData();
        app_metrologyData.pMetHarData = DRV_METROLOGY_GetHarData();
    </code>

  Remarks:
    None. 
*/
DRV_METROLOGY_ACCUMULATORS * DRV_METROLOGY_GetAccData(void);

// *****************************************************************************
/* Function:
    DRV_METROLOGY_HARMONICS * DRV_METROLOGY_GetHarData (void);

  Summary:
    Get the pointer to the harmonic analysis registers of the metrology library application running on the second processor.  

  Description:
    Harmonic analysis registers are acting as 32-bit metrology harmonic analysis registers.
    The Metrology module shall generate the following primary output measurement quantities for each measurement interval, 
    per-phase quantities with each phase designated by the subscript '_x' (where x = [A, B, or C] for voltage channels 
    and x = [A, B, C] for current channels). 
    Internal scaling allows accurate harmonic analysis for integration periods of up to 16 seconds for up to full-scale magnitude input waveforms.
    For a detailed description of the harmonic analysis registers, refer to "drv_metrology_regs.h" header file.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Pointer to the 32-bit metrology harmonic analysis registers. 

  Example:
    <code>
        // Get Pointers to metrology data regions 
        app_metrologyData.pMetControl = DRV_METROLOGY_GetControl();
        app_metrologyData.pMetStatus = DRV_METROLOGY_GetStatus();
        app_metrologyData.pMetAccData = DRV_METROLOGY_GetAccData();
        app_metrologyData.pMetHarData = DRV_METROLOGY_GetHarData();
    </code>

  Remarks:
    None. 
*/
DRV_METROLOGY_HARMONICS * DRV_METROLOGY_GetHarData(void);

// *****************************************************************************
/* Function:
    void DRV_METROLOGY_SetControl (DRV_METROLOGY_CONTROL * pControl);

  Summary:
    Establishes the content of all control registers at once.  

  Description:
    Overwrite the value of the every control register in the shared memory at once.

  Precondition:
    None.

  Parameters:
    pControl - Pointer to data of the new control registers to overwrite the old ones.

  Returns:
    None. 

  Example:
    <code>
        void APP_METROLOGY_SetControlByDefault(void)
       {
           DRV_METROLOGY_CONTROL *pSrc;

           pSrc = DRV_METROLOGY_GetControlByDefault();
           DRV_METROLOGY_SetControl(pSrc);
       }
    </code>

  Remarks:
    None. 
*/
void DRV_METROLOGY_SetControl(DRV_METROLOGY_CONTROL * pControl);

// *****************************************************************************
/* Function:
    void DRV_METROLOGY_UpdateMeasurements(void);

  Summary:
    Updates the RMS values, metrology events and AFE energy value.

  Description:
    This function is used to obtain the RMS values and the metrology events from the control and accumulator registers of the metrology library. AFE energy value is also calculated and updated. 
    Control and Accumulator registers are updated by the metrology library application once per an integration period.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None. 

  Example:
    <code>
        case APP_METROLOGY_STATE_RUNNING:
        {
            // Wait for the metrology semaphore to get measurements at the end of the integration period.
            OSAL_SEM_Pend(&appMetrologySemID, OSAL_WAIT_FOREVER);

            DRV_METROLOGY_UpdateMeasurements();
        }
    </code>

  Remarks:
    None. 
*/
void DRV_METROLOGY_UpdateMeasurements(void);

// *****************************************************************************
/* Function:
    uint32_t DRV_METROLOGY_GetEnergyValue(bool restartEnergy);

  Summary:
    Gets the active energy value. 

  Description:
    Gets the active energy value. Energy value should be previously updated by the DRV_METROLOGY_UpdateMeasurements() routine.

  Precondition:
    None.

  Parameters:
    restartEnergy - Flag to indicate if the energy value should be restarted or accumulated to the previous calculated value.

  Returns:
    The active energy value obtained according to the last call to DRV_METROLOGY_UpdateMeasurements() routine.. 

  Example:
    <code>
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
    </code>

  Remarks:
    None. 
*/
uint32_t DRV_METROLOGY_GetEnergyValue(bool restartEnergy);

// *****************************************************************************
/* Function:
    uint32_t DRV_METROLOGY_GetRMSValue(DRV_METROLOGY_RMS_TYPE type);

  Summary:
    Gets the last RMS value of the selected measurement type. 

  Description:
    RMS values should be previously updated by the DRV_METROLOGY_UpdateMeasurements() routine. 
    For further information about RMS types, refer to DRV_METROLOGY_RMS_TYPE definition.
 
  Precondition:
    None.

  Parameters:
    type - Indicate what type of measurement is obtained.

  Returns:
    The RMS value of the selected type. 

  Example:
    <code>
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
    </code>

  Remarks:
    None. 
*/
uint32_t DRV_METROLOGY_GetRMSValue(DRV_METROLOGY_RMS_TYPE type);

// *****************************************************************************
/* Function:
    DRV_METROLOGY_RMS_SIGN DRV_METROLOGY_GetRMSSign(DRV_METROLOGY_RMS_TYPE type);

  Summary:
    Gets the sign of the last RMS value of the selected measurement type. 

  Description:
    RMS sign values should be previously updated by the DRV_METROLOGY_UpdateMeasurements() routine. 
    For further information about RMS types, refer to DRV_METROLOGY_RMS_TYPE definition.

  Precondition:
    None.

  Parameters:
    type - Indicate what type of measurement is obtained.

  Returns:
    The RMS sign of the last RMS value. Positive sign is identified as RMS_SIGN_POSITIVE (0), negative sign as RMS_SIGN_NEGATIVE (1). 

  Example:
    <code>
        
    </code>

  Remarks:
    None. 
*/
DRV_METROLOGY_RMS_SIGN DRV_METROLOGY_GetRMSSign(DRV_METROLOGY_RMS_TYPE type);

// *****************************************************************************
/* Function:
    void DRV_METROLOGY_SetConfiguration(DRV_METROLOGY_CONFIGURATION * config);

  Summary:
    Set metrology configuration. 

  Description:
    This routine is used to configure the metrology library according to the hardware 
    in the input networks, as well as the programmable gain of the internal amplifiers.
    This function has to be called before the DRV_METROLOGY_StartCalibration() routine, 
    in order to ensure that the proper configuration has been applied before calibrating the system. 
    If the programmable gain of the internal amplifiers changes, then the metrology library must be 
    reinitialized in order to apply the changes.
    The metrology driver code receives the parameters and writes the proper metrology control registers.

  Precondition:
    None.

  Parameters:
    config - Pointer to configuration data to be used by the metrology library.

  Returns:
    None. 

  Example:
    <code>
        DRV_METROLOGY_CONFIGURATION newConf;

        newConf.mc = 800;
        newConf.st = SENSOR_CT;
        newConf.freq = 50;
        newConf.gain = GAIN_1;
        newConf.tr = 1000;
        newConf.rl = 3.24;
        newConf.ku = 1651;

        DRV_METROLOGY_SetConfiguration(&newConf);
    </code>

  Remarks:
    None. 
*/
void DRV_METROLOGY_SetConfiguration(DRV_METROLOGY_CONFIGURATION * config);

// *****************************************************************************
/* Function:
    void DRV_METROLOGY_GetEventsData(DRV_METROLOGY_AFE_EVENTS * events);

  Summary:
    Gets the last metrology AFE events data. 

  Description:
    Events data should be previously updated by the DRV_METROLOGY_UpdateMeasurements() routine. 
    For further information about the event data, refer to DRV_METROLOGY_AFE_EVENTS definition.

  Precondition:
    None.

  Parameters:
    events - Pointer to the data where the events related information will be written.

  Returns:
    None. 

  Example:
    <code>
        DRV_METROLOGY_UpdateMeasurements();
            
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
    </code>

  Remarks:
    None. 
*/
void DRV_METROLOGY_GetEventsData(DRV_METROLOGY_AFE_EVENTS * events);

// *****************************************************************************
/* Function:
    DRV_METROLOGY_CALIBRATION * DRV_METROLOGY_GetCalibrationData (void);

  Summary:
    Get the pointer to the data used in the internal calibration process of the metrology library. 

  Description:
    These internal calibration values are used to obtain the RMS values correctly. 
    They also includes the result of the calibration process to be checked by the main application.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Pointer to the internal calibration data. 

  Example:
    <code>
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
    </code>

  Remarks:
    None. 
*/
DRV_METROLOGY_CALIBRATION * DRV_METROLOGY_GetCalibrationData(void);

// *****************************************************************************
/* Function:
    void APP_METROLOGY_StartCalibration(APP_METROLOGY_CALIBRATION * calibration);

  Summary:
    Starts internal calibration process. 

  Description:
    This routine is used to automatically calibrate the board. 
    It requires accurate and stable voltage and current sources and loads, which can be 
    provided by a meter test bench like the WECO (or equivalent equipment).
    This routine must be called after a configuration routine (APP_METROLOGY_SetConfiguration), 
    in order to ensure that the proper configuration has been applied.

  Precondition:
    None.

  Parameters:
    calibration - Pointer to calibration data to be used by the metrology library.

  Returns:
    None. 

  Example:
    <code>
        APP_METROLOGY_CALIBRATION newCal;
  
        // Calibrate phase A, applying 220.00V, 5.000A, angle=60.00
        newCal.aimVA = 220.00;
        newCal.aimIA = 5.000;
        newCal.angleA = 60.00;

        APP_METROLOGY_StartCalibration(&newCal);
    </code>

  Remarks:
    None. 
*/
void DRV_METROLOGY_StartCalibration(void);

// *****************************************************************************
/* Function:
    void DRV_METROLOGY_UpdateCalibration(void);

  Summary:
    Performs the internal calibration process. 

  Description:
    Once the calibration process is started by calling DRV_METROLOGY_StartCalibration() routine, 
    this funtion has to be called every integration period.
    The metrology driver internally sets the duration of the calibration process as 4 integration periods.
    CAL_M_Ix (X = A, B, C or N) and CAL_M_Vx (X = A, B or C) are used to calibrate 
    the current and voltage magnitudes of phase x. Calibration process is iterative, 
    using prior or old magnitude calibration factors to perform the next calibration cycle.
    CAL_PH_Ix(x = A, B, C or N) and CAL_PH_Vx(x = A, B or C) are used to align 
    the phase delays between each pair of current and voltage channels, and also 
    to optionally equalize the phase delays between voltage channels. 
    All phase shift coefficients used by the metrology library must be normalized to 
    equivalent linear-phase shift amounts at 60Hz.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None. 

  Example:
    <code>
        case APP_METROLOGY_STATE_CHECK_CALIBRATION:
        {
            // Wait for the metrology semaphore to get measurements at the end of the integration period. 
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
    </code>

  Remarks:
    None. 
*/
void DRV_METROLOGY_UpdateCalibration(void);

// *****************************************************************************
/* Function:
    bool DRV_METROLOGY_CalibrationIsCompleted(void);

  Summary:
    Checks if the calibration process is finished. 

  Description:
    This routine should be called to know if the calibration process is finished.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    True if the calibration process has been completed, otherwise False.. 

  Example:
    <code>
        case APP_METROLOGY_STATE_CHECK_CALIBRATION:
        {
            // Wait for the metrology semaphore to get measurements at the end of the integration period. 
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
    </code>

  Remarks:
    None. 
*/
bool DRV_METROLOGY_CalibrationIsCompleted(void);

// *****************************************************************************
/* Function:
    bool DRV_METROLOGY_GetCalibrationResult(void);

  Summary:
    Gets the result of the calibration process. 

  Description:
    This routine should be used to check if the calibration process has been completed successfully.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    True if at least 1 phase has been calibrated successfully, otherwise False. 

  Example:
    <code>
        case APP_METROLOGY_STATE_CHECK_CALIBRATION:
        {
            // Wait for the metrology semaphore to get measurements at the end of the integration period. 
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
    </code>

  Remarks:
    None. 
*/
bool DRV_METROLOGY_GetCalibrationResult(void);

// *****************************************************************************
/* Function:
    void DRV_METROLOGY_StartHarmonicAnalysis(
        uint8_t harmonicNum, 
        DRV_METROLOGY_HARMONIC *pHarmonicResponse
    );

  Summary:
    Starts the harmonic Analysis process. 

  Description:
    This routine configures the metrology library to enable the harmonics computation 
    and to calculate the data corresponding to the harmonic number n. 
    Then, the metrology driver receives the data from the metrology library and 
    computes the RMS currents and voltages of the selected harmonic.

  Precondition:
    None.

  Parameters:
    harmonicNum - Harmonic number.
    pHarmonicResponse - Pointer to the harmonic analysis struct data to store the harmonic data result.

  Returns:
    None. 

  Example:
    <code>
        DRV_METROLOGY_HARMONIC harmonicAnalysisResponse;
    
        DRV_METROLOGY_StartHarmonicAnalysis(3, &harmonicAnalysisResponse);
    </code>

  Remarks:
    None. 
*/
void DRV_METROLOGY_StartHarmonicAnalysis(uint8_t harmonicNum, DRV_METROLOGY_HARMONIC *pHarmonicResponse);

// *****************************************************************************
/* Function:
    bool DRV_METROLOGY_HarmonicAnalysisIsRun(void);

  Summary:
    Checks if the Harmonic analysis is finished.

  Description:
    This routine should be called to know if the Harmonic analysis is finished.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    This routine should be called to know if the Harmonic analysis is finished. 

  Example:
    <code>
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
    </code>

  Remarks:
    None. 
*/
bool DRV_METROLOGY_HarmonicAnalysisIsRun(void);

// *****************************************************************************
/* Function:
    bool DRV_METROLOGY_GetHarmonicAnalysisResult(void);

  Summary:
    Gets the result of the Harmonic analysis. 

  Description:
    This routine should be used to check if the Harmonic analysis has been completed successfully.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    True if the Harmonic analysis has been completed successfully, otherwise False. 

  Example:
    <code>
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
    </code>

  Remarks:
    None. 
*/
bool DRV_METROLOGY_GetHarmonicAnalysisResult(void);

#ifdef __cplusplus // Provide C++ Compatibility
 }
#endif

#endif
