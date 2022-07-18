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

#define MET_CAPTURE_BUF_SIZE     DRV_METROLOGY_CAPTURE_BUF_SIZE

// *****************************************************************************
/* Metrology Driver Result

  Summary:
    Result of a Metrology driver interface operation.

  Description:
    Identifies the result of certain Metrology driver operations.
*/

typedef enum
{
    // Operation completed with success.
    DRV_METROLOGY_SUCCESS,

    // Invalid handle or operation failed.
    DRV_METROLOGY_ERROR

} DRV_METROLOGY_RESULT;


typedef enum
{
    // Operation completed with success.
    DRV_METROLOGY_START_SOFT,

    // Invalid handle or operation failed.
    DRV_METROLOGY_START_HARD

} DRV_METROLOGY_START_MODE;

typedef enum
{
    // Positive Value
    RMS_SIGN_POSITIVE = 0,

    //  Negative Value
    RMS_SIGN_NEGATIVE = 1,

} DRV_METROLOGY_RMS_SIGN;

// *****************************************************************************
// *****************************************************************************
// Section: Interface
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_METROLOGY_Initialize(SYS_MODULE_INIT * init, uint32_t resetValue);
DRV_METROLOGY_RESULT DRV_METROLOGY_Open(DRV_METROLOGY_START_MODE mode);
DRV_METROLOGY_RESULT DRV_METROLOGY_Close (void);
DRV_METROLOGY_RESULT DRV_METROLOGY_Start(void);
DRV_METROLOGY_RESULT DRV_METROLOGY_IntegrationCallbackRegister(DRV_METROLOGY_CALLBACK callback);
DRV_METROLOGY_STATE DRV_METROLOGY_GetState(void);
DRV_METROLOGY_STATUS * DRV_METROLOGY_GetStatus(void);
DRV_METROLOGY_CONTROL * DRV_METROLOGY_GetControl(void);
DRV_METROLOGY_CONTROL * DRV_METROLOGY_GetControlByDefault(void);
DRV_METROLOGY_ACCUMULATORS * DRV_METROLOGY_GetAccData(void);
DRV_METROLOGY_HARMONICS * DRV_METROLOGY_GetHarData(void);

void DRV_METROLOGY_SetControl(DRV_METROLOGY_CONTROL * pControl);
void DRV_METROLOGY_UpdateMeasurements(void);
uint32_t DRV_METROLOGY_GetEnergyValue(bool restartEnergy);
uint32_t DRV_METROLOGY_GetRMSValue(DRV_METROLOGY_RMS_TYPE type);
DRV_METROLOGY_RMS_SIGN DRV_METROLOGY_GetRMSSign(DRV_METROLOGY_RMS_TYPE type);

void DRV_METROLOGY_SetConfiguration(DRV_METROLOGY_CONFIGURATION * config);

void DRV_METROLOGY_GetEventsData(DRV_METROLOGY_AFE_EVENTS * events);

DRV_METROLOGY_CALIBRATION * DRV_METROLOGY_GetCalibrationData(void);
void DRV_METROLOGY_StartCalibration(void);
void DRV_METROLOGY_UpdateCalibration(void);
bool DRV_METROLOGY_CalibrationIsCompleted(void);
bool DRV_METROLOGY_GetCalibrationResult(void);

void DRV_METROLOGY_RequestHarmonicAnalysis(uint8_t harmonicNum, DRV_METROLOGY_HARMONIC *pHarmonicResponse);
bool DRV_METROLOGY_HarmonicAnalysisIsRun(void);
bool DRV_METROLOGY_GetHarmonicAnalysisResult(void);

#ifdef __cplusplus // Provide C++ Compatibility
 }
#endif

#endif
