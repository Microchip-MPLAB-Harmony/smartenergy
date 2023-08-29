/*******************************************************************************
  DRV_METROLOGY Driver Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_metrology_local.h

  Summary:
    Metrology Driver Local Data Structures.

  Description:
    The Metrology Library uses local data structures for internal management.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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
//DOM-IGNORE-END

#ifndef DRV_METROLOGY_LOCAL_H
#define DRV_METROLOGY_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdbool.h>
#include "driver/driver.h"
#include "system/system.h"
#include "drv_metrology_definitions.h"
<#if DRV_MET_RTOS_ENABLE == true> 
#include "osal/osal.h"
</#if>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Metrology Driver Instance Object

  Summary:
    Object used to keep any data required for an instance of the metrology driver.

  Description:
    None.

  Remarks:
    None.
*/

typedef struct
{
    /* Flag to indicate this object is in use */
    bool                                          inUse;

    /* The status of the driver */
    volatile DRV_METROLOGY_STATUS                 status;

<#if DRV_MET_RTOS_ENABLE == false> 
    /* Flag to indicate that a new IPC interrupt has been triggered */
    volatile bool                                 ipcInterruptFlag;
<#else>
    /* Define a semaphore to attend the Metrology IPC interrupts */
    OSAL_SEM_DECLARE(semaphoreID);
</#if>

    /* Flag to indicate that a new integration period has been completed */
    volatile bool                                 integrationFlag;

    /* Size (in Bytes) of the PLC binary file */
    uint32_t                                      binSize;

    /* Address where PLC binary file is located */
    uint32_t                                      binStartAddress;

    /* Metrology Control interface */
    MET_REGISTERS *                               metRegisters;

    /* Metrology Accumulated Output Data */
    DRV_METROLOGY_REGS_ACCUMULATORS               metAccData;

    /* Metrology Harmonic Analysis Output Data */
    DRV_METROLOGY_REGS_HARMONICS                  metHarData;

    /* Metrology Analog Front End Data */
    DRV_METROLOGY_AFE_DATA                        metAFEData;

    /* Metrology Calibration interface */
    DRV_METROLOGY_CALIBRATION                     calibrationData;
    
    /* Harmonic Analysis Data */
    DRV_METROLOGY_HARMONIC_ANALYSIS               harmonicAnalysisData;

    /* IPC metrology lib integration Callback */
    DRV_METROLOGY_CALLBACK                        integrationCallback;

<#if DRV_MET_NOT_FULL_CYCLE == true>  
    /* Flag to indicate every full line cycle */
    volatile bool                                 fullCycleIntFlag;
    /* IPC metrology lib Full Cycle Callback */
    DRV_METROLOGY_CALLBACK                        fullCycleCallback;

</#if>
<#if DRV_MET_NOT_HALF_CYCLE == true>  
    /* Flag to indicate every half line cycle */
    volatile bool                                 halfCycleIntFlag;
    /* IPC metrology lib Half Cycle Callback */
    DRV_METROLOGY_CALLBACK                        halfCycleCallback;

</#if>
<#if DRV_MET_RAW_ZERO_CROSSING == true> 
    /* Flag to indicate zero-crossings using the unfiltered “raw” 16 KHz data stream  */
    volatile bool                                 zeroCrossIntFlag; 
    /* IPC metrology lib Zero Cross Callback */
    DRV_METROLOGY_CALLBACK                        zeroCrossCallback;

</#if>
<#if DRV_MET_PULSE_0 == true>  
    /* Flag to indicate that a pulse 0 has computationally committed to be generated */
    volatile bool                                 pulse0IntFlag;
    /* IPC metrology lib Pulse 0 Callback */
    DRV_METROLOGY_CALLBACK                        pulse0Callback;

</#if>
<#if DRV_MET_PULSE_1 == true>  
    /* Flag to indicate that a pulse 10 has computationally committed to be generated */
    volatile bool                                 pulse1IntFlag;
    /* IPC metrology lib Pulse 1 Callback */
    DRV_METROLOGY_CALLBACK                        pulse1Callback;

</#if>
<#if DRV_MET_PULSE_2 == true>  
    /* Flag to indicate that a pulse 2 has computationally committed to be generated */
    volatile bool                                 pulse2IntFlag;
    /* IPC metrology lib Pulse 2 Callback */
    DRV_METROLOGY_CALLBACK                        pulse2Callback;

</#if>
    /* Calibration Process Callback */
    DRV_METROLOGY_CALIBRATION_CALLBACK            calibrationCallback;

    /* Harmonic Analysis Callback */
    DRV_METROLOGY_HARMONICS_CALLBACK              harmonicAnalysisCallback;

} DRV_METROLOGY_OBJ;

#ifdef __cplusplus
}
#endif

#endif // #ifndef DRV_METROLOGY_LOCAL_H
/*******************************************************************************
 End of File
*/
