/*******************************************************************************
  DRV_METROLOGY Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    drv_metrology_definitions.h

  Summary:
    Metrology Library Definitions Interface header.

  Description:
    The Metrology Library provides a interface to access the metrology data
    provided by the application running on Core 1.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _DRV_METROLOGY_DEFINITIONS_H
#define _DRV_METROLOGY_DEFINITIONS_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdbool.h>
#include "driver/driver.h"
#include "system/system.h"
#include "drv_metrology_regs.h"

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
typedef void (* DRV_METROLOGY_CALLBACK)(void);
typedef void (* DRV_METROLOGY_CALIBRATION_CALLBACK) (bool result); 
typedef void (* DRV_METROLOGY_HARMONIC_ANALYSIS_CALLBACK) (uint8_t harmonicNum);          

#define DRV_METROLOGY_IPC_INIT_IRQ_MSK            IPC_ISR_IRQ20_Msk
#define DRV_METROLOGY_IPC_INTEGRATION_IRQ_MSK     IPC_ISR_IRQ0_Msk

#define  FREQ_Q         12
#define  GAIN_P_K_T_Q   24
#define  GAIN_VI_Q      10
#define  RMS_DIV_G      1024    /* (1<<GAIN_VI_Q) */
#define  CAL_VI_Q       29
#define  CAL_PH_Q       31
#define  RMS_Q          40
#define  RMS_DIV_Q      0x10000000000  /* (1<<RMS_Q) */
#define  RMS_Inx_Q      (20)
#define  RMS_DIV_Inx_Q  0x100000 /* (1<< RMS_Inx_Q) */
#define  RMS_PQ_SYMB    0x8000000000000000       /* p/q symbol bit */
#define  RMS_HARMONIC   0x80000000
#define  CONST_Pi       3.1415926

/* Metrology Driver Sensor Type

  Summary:
    Describes the sensor type.

  Description:
    The metrology driver has been designed to interface with Current Transformers, Rogowski Coils and Shunt Resistors current sensors.
*/
typedef enum {
    SENSOR_CT        = 0,
    SENSOR_SHUNT     = 1,
    SENSOR_ROGOWSKI  = 2,
    SENSOR_NUM_TYPE
} DRV_METROLOGY_SENSOR_TYPE;

/* Metrology Driver Gain Type

  Summary:
    Gain selected for use in the ADC front-end.

  Description:
    On Current measurement channels gain for voltage is fixed to 1.
*/
typedef enum {
    GAIN_1        = 0,
    GAIN_2        = 1,
    GAIN_4        = 2,
    GAIN_8        = 3,
    GAIN_NUM_TYPE
} DRV_METROLOGY_GAIN_TYPE;

/* Metrology Driver Phase Identifier

  Summary:
    Line identifier used in the calibration process.

  Description:
    Phase A, B and C are used to identify Ua, Ub and Uc respectively. Phase T includes all phases.
*/
typedef enum {
    PHASE_A        = 1,
    PHASE_B        = 2,
    PHASE_C        = 3,
    PHASE_T        = 5,
    PHASE_ID_NUM   = PHASE_T
} DRV_METROLOGY_PHASE_ID;


/* Metrology Driver Calibration References

  Summary:
    Specifies the all reference values used for the auto calibration process.

  Description:
    - aimVx refers to the RMS voltage applied to the voltage input where x = A,B,C
    - aimIx refers to the Rms current applied to the current input where x = A,B,C
    - anglex refers to the Angle between the voltage and current vectors where x = A,B,C
    - lineID identifies the phase/phases to calibrate
*/
typedef struct {
    double aimVA;  
    double aimIA;  
    double angleA; 
    double aimVB;  
    double aimIB;  
    double angleB; 
    double aimVC;  
    double aimIC;  
    double angleC; 
    DRV_METROLOGY_PHASE_ID lineId;
} DRV_METROLOGY_CALIBRATION_REFS;

/* Metrology Driver Calibration Data

  Summary:
    Specifies all data internally needed for the auto calibration process.

  Description:
    - references. Calibration references. Client must be set the references before starting the calibration process.
    - freq. Stores the mains frequency passed as parameter in the DRV_METROLOGY_SetConfiguration() routine.
    - numIntegrationPeriods. Number of integration periods needed to complete the calibration process. It is set internally to 4.
    - running. Flag used to check if the calibration process was completed.
    - result. Flag used to check if the calibration process has been successful.
    - Rest of the values are internally used to perform the calibration process.
*/
typedef struct {
    DRV_METROLOGY_CONTROL metControlConf;
    DRV_METROLOGY_CALIBRATION_REFS references;
    uint32_t featureCtrl0Backup;
    double freq;                     
    uint32_t numIntegrationPeriods;
    uint64_t dspAccIa;
    uint64_t dspAccIb;
    uint64_t dspAccIc;
    uint64_t dspAccIn;
    uint64_t dspAccUa;
    uint64_t dspAccUb;
    uint64_t dspAccUc;
    uint64_t dspAccUn;
    int64_t  dspAccPa;
    int64_t  dspAccPb;
    int64_t  dspAccPc;
    int64_t  dspAccQa;
    int64_t  dspAccQb;
    int64_t  dspAccQc;
    bool  running;
    bool  result;
} DRV_METROLOGY_CALIBRATION;

/* Metrology Driver Harmonic Data

  Summary:
    Identifies the result of the Harmonic Analysis process.

  Description:
    - Irms_X_m. RMS current value obtained as the result of last the harmonic analysis regarding channel X.
    - Irms_N_m. RMS current value obtained as the result of last the harmonic analysis regarding neutral channel.
    - Vrms_X_m. RMS voltage value obtained as the result of last the harmonic analysis regarding channel X.
*/
typedef struct {
    double Irms_A_m;
    double Irms_B_m;
    double Irms_C_m;
    double Irms_N_m;
    double Vrms_A_m;
    double Vrms_B_m;
    double Vrms_C_m;
} DRV_METROLOGY_HARMONICS_RMS;

typedef struct {
    DRV_METROLOGY_HARMONICS_RMS * pHarmonicAnalysisResponse;
    uint8_t harmonicNum;
    bool  running;
} DRV_METROLOGY_HARMONIC_ANALYSIS;

/* Metrology Driver AFE Events

  Summary:
    Identifies all events related to metrology library.

  Description:
    - pXDir. Identifies the sign of the active power in channel X. "1" means a negative value, "0" is a positive value.
    - qXDir. Identifies the sign of the reactive power in channel X. "1" means a negative value, "0" is a positive value.
    - sagX. Voltage Sag Detected Flag for Channel X. "1" means that voltage sag is detected.
    - swellX. Voltage Swell Detected Flag for Channel X. "1" means that voltage Swell is detected.
*/
typedef struct {
    uint32_t paDir : 1;
    uint32_t pbDir : 1;
    uint32_t pcDir : 1;
    uint32_t ptDir : 1;
    uint32_t qaDir : 1;
    uint32_t qbDir : 1;
    uint32_t qcDir : 1;
    uint32_t qtDir : 1;
    uint32_t sagA : 1;
    uint32_t sagB : 1;
    uint32_t sagC : 1;
    uint32_t reserved1 : 1;
    uint32_t swellA : 1;
    uint32_t swellB : 1;
    uint32_t swellC : 1;
    uint32_t reserved2 : 17;
} DRV_METROLOGY_AFE_EVENTS;

/* Metrology Driver RMS type

  Summary:
    Identifies the all RMS types of measurements.

  Description:
    RMS values are calculated including all harmonics of each phase, where:
        - U = Voltage RMS value
        - I = Current RMS value
        - P = Active power RMS value
        - Q = Reactive power RMS value
        - S = Aparent power RMS value
        - FREQ = Frequency of the line voltage fundamental harmonic component determined by the Metrology library using the dominant phase
        - ANGLE = Angle between the voltage and current vectors
*/
typedef enum {
    RMS_UA = 0,
    RMS_UB,
    RMS_UC,
    RMS_IA,
    RMS_IB,
    RMS_IC,
    RMS_INI,
    RMS_INM,
    RMS_INMI,
    RMS_PT,
    RMS_PA,
    RMS_PB,
    RMS_PC,
    RMS_QT,
    RMS_QA,
    RMS_QB,
    RMS_QC,
    RMS_ST,
    RMS_SA,
    RMS_SB,
    RMS_SC,
    RMS_FREQ,
    RMS_ANGLEA,
    RMS_ANGLEB,
    RMS_ANGLEC,
    RMS_ANGLEN,
    RMS_TYPE_NUM
} DRV_METROLOGY_RMS_TYPE;

/* Metrology Driver AFE calculated data

  Summary:
    Identifies the data calculated from the metrology AFE measurements.

  Description:
    - energy. Active energy calculated value.
    - afeEvents. AFE events data.
    - RMS[RMS_TYPE_NUM]. RMS calculated values.
*/
typedef struct {
    uint32_t energy;
    DRV_METROLOGY_AFE_EVENTS afeEvents;
    uint32_t RMS[RMS_TYPE_NUM];
} DRV_METROLOGY_AFE_DATA;

/* Metrology Driver Configuration

  Summary:
    Identifies values needed to set different metrology configurations.

  Description:
    - mc. Meter Constant (amount of energy signified by one output pulse). Units: pulses/kWh (active energy), pulses/kVARh (reactive energy), or pulses/kAmp2-h (amp square)
    - freq. Mains frequency. Units: Hz.
    - tr. 
      - In the case of a current transformer, this is the current transformer ratio
      - In the case of a Rogowski Coil, this is the current sensitivity (units: uV/A) at the main frequency specified in ?Frequency? parameter.
    - rl.
      - In the case of current transformer, this is the resistor load or burden resistor (units: ?).
      - In the case of shunt resistor, this is the shunt resistor value (units: u?)
    - ku. Voltage divider ratio.
    - st. Sensor Type. Refer to DRV_METROLOGY_SENSOR_TYPE.
    - gain. Programmable Gain Amplifier of the AFE (analog front end). Refer to DRV_METROLOGY_GAIN_TYPE.
*/
typedef struct {
    uint32_t mc;
    double freq;
    double tr;
    double rl;
    uint32_t ku;
    DRV_METROLOGY_SENSOR_TYPE st;
    DRV_METROLOGY_GAIN_TYPE gain;
} DRV_METROLOGY_CONFIGURATION;

/* Metrology Library State

  Summary:
    Identifies the state of the metrology library.

  Description:
    For further information about the state diagram, refer to DRV_METROLOGY_GetState() online documentation.

  Remarks:
    None.
*/
typedef enum {
    DRV_METROLOGY_STATE_HALT = STATUS_STATUS_HALT,
    DRV_METROLOGY_STATE_RESET = STATUS_STATUS_RESET,
    DRV_METROLOGY_STATE_INIT_DSP = STATUS_STATUS_INIT_DSP,
    DRV_METROLOGY_STATE_DSP_READY = STATUS_STATUS_DSP_READY,
    DRV_METROLOGY_STATE_INIT_ATSENSE = STATUS_STATUS_INIT_ATSENSE,
    DRV_METROLOGY_STATE_ATSENSE_READY = STATUS_STATUS_ATSENSE_READY,
    DRV_METROLOGY_STATE_READY = STATUS_STATUS_READY,
    DRV_METROLOGY_STATE_SETTLING = STATUS_STATUS_DSP_SETTLING,
    DRV_METROLOGY_STATE_RUNNING = STATUS_STATUS_DSP_RUNNING
} DRV_METROLOGY_STATE;

/* Metrology Driver Initialization Data

  Summary:
    Defines the data required to initialize the Metrology driver

  Description:
    - regBaseAddress. Base Address for Metrology registers.
    - binStartAddress. Start Address where Metrology library application file is located.
    - binEndAddress. End Address where Metrology library application binary file is located.

  Remarks:
    None.
*/
typedef struct {
    uint32_t regBaseAddress;
    uint32_t binStartAddress;
    uint32_t binEndAddress;
} DRV_METROLOGY_INIT;

// *****************************************************************************
/* Metrology Driver Object

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
    SYS_STATUS                                    status;

    /* State of the metrology driver  */
    volatile DRV_METROLOGY_STATE                  state;

    /* Size (in Bytes) of the PLC binary file */
    uint32_t                                      binSize;

    /* Address where PLC binary file is located */
    uint32_t                                      binStartAddress;

    /* Metrology Control interface */
    MET_REGISTERS *                               metRegisters;

    /* Metrology Accumulated Output Data */
    DRV_METROLOGY_ACCUMULATORS                    metAccData;

    /* Metrology Harmonic Analysis Output Data */
    DRV_METROLOGY_HARMONICS                       metHarData;

    /* Metrology Analog Front End Data */
    DRV_METROLOGY_AFE_DATA                        metAFEData;

    /* Metrology Calibration interface */
    DRV_METROLOGY_CALIBRATION                     calibrationData;
    
    /* Harmonic Analysis Data */
    DRV_METROLOGY_HARMONIC_ANALYSIS               harmonicAnalysisData;

    /* IPC metrology lib integration Callback */
    DRV_METROLOGY_CALLBACK                        integrationCallback;

    /* Calibration Process Callback */
    DRV_METROLOGY_CALIBRATION_CALLBACK            calibrationCallback;

    /* Harmonic Analysis Callback */
    DRV_METROLOGY_HARMONIC_ANALYSIS_CALLBACK      harmonicAnalysisCallback;

} DRV_METROLOGY_OBJ;

#ifdef __cplusplus
}
#endif

#endif // #ifndef _DRV_METROLOGY_DEFINITIONS_H
/*******************************************************************************
 End of File
*/
