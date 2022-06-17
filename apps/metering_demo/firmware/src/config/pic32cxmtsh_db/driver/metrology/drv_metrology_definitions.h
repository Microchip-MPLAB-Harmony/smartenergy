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
typedef void ( * DRV_METROLOGY_CALLBACK ) ( void );
        
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
#define  RMS_DIV_Inx_Q  0x100000 /* (1<< RMS_Inx_Q ) */
#define  RMS_PQ_SYMB    0x8000000000000000       /* p/q symbol bit */
#define  RMS_HARMONIC   0x80000000
#define  CONST_Pi       3.1415926

typedef enum {
	CT        = 0,
	SHUNT     = 1,
	ROGOWSKI  = 2
} MET_SENSOR_TYPE;

typedef struct {
	uint32_t lineId;
	uint32_t mc;        
	uint32_t featureCtrl0Copy;
	uint32_t freq;      
	uint32_t gain_i;    
	uint32_t k_i;       
	uint32_t rl;        
	uint32_t k_u;       
	uint32_t aimVA;    
	uint32_t aimIA;    
	uint32_t angleA;   
	uint32_t aimVB;    
	uint32_t aimIB;    
	uint32_t angleB;   
	uint32_t aimVC;    
	uint32_t aimIC;    
	uint32_t angleC;   
	uint32_t dspUpdateNum;
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
	MET_SENSOR_TYPE sensorType;
	uint8_t  harmonicOrder;
	uint8_t  state;
} MET_CALIBRATION;

typedef union {
	uint32_t WORD;
	struct {
		uint32_t paDir : 1;
		uint32_t pbDir : 1;
		uint32_t pcDir : 1;
		uint32_t ptDir : 1;
		uint32_t qaDir : 1;
		uint32_t qbDir : 1;
		uint32_t qcDir : 1;
		uint32_t qtDir : 1;
		uint32_t sagB : 1;
		uint32_t sagC : 1;
		uint32_t reserved1 : 1;
		uint32_t swellA : 1;
		uint32_t swellB : 1;
		uint32_t swellC : 1;
		uint32_t paRev : 1;
		uint32_t pbRev : 1;
		uint32_t pcRev : 1;
		uint32_t ptRev : 1;
		uint32_t reserved2 : 13;
	} BIT;
} MET_AFE_EVENTS;

typedef enum {
	UA = 0,
	UB,
	UC,
	IA,
	IB,
	IC,
	INI,
	INM,
	INMI,
	PT,
	PA,
	PB,
	PC,
	QT,
	QA,
	QB,
	QC,
	ST,
	SA,
	SB,
	SC,
	FREQ,
	ANGLEA,
	ANGLEB,
	ANGLEC,
	ANGLEN,
	RMSTYPENUM
} MET_RMS_TYPE;

typedef struct {
	uint32_t energy;
	MET_AFE_EVENTS afeEvents;
	uint32_t RMS[RMSTYPENUM];
} MET_AFE_DATA;

typedef enum {
	PENERGY = 0,
	QENERGY = 1,
} MET_ENERGY_TYPE;

typedef enum {
	ABS = 0,
	ALG = 1
} MET_ENERGY_MODE;

// *****************************************************************************
/* DRV_PLC_PHY Transfer Object State

  Summary:
    Defines the status of the DRV_PLC_PHY Transfer Object.

  Description:
    This enumeration defines the status of the DRV_PLC_PHY Transfer Object.

  Remarks:
    None.
*/

typedef enum
{
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
    This data type defines the data required to initialize or the Metrology driver.

  Remarks:
    None.
*/

typedef struct
{
    /* Base Address for Metrology registers */
    uint32_t                        regBaseAddress;

    /* Start Address where MET binary file is located */
    uint32_t                        binStartAddress;

    /* End Address where MET binary file is located */
    uint32_t                        binEndAddress;

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
    bool                            inUse;

    /* The status of the driver */
    SYS_STATUS                      status;

    /* State of the metrology driver  */
    DRV_METROLOGY_STATE             state;

    /* Size (in Bytes) of the PLC binary file */
    uint32_t                        binSize;

    /* Address where PLC binary file is located */
    uint32_t                        binStartAddress;
    
    /* Flag to indicate new integration period has been completed */
    bool                            newMetData;
    
    /* Metrology Control interface */
    MET_REGISTERS *                 metRegisters;
    
    /* Metrology Accumulated Output Data */
    MET_ACCUMULATORS                metAccData;
    
    /* Metrology Harmonic Analysis Output Data */
    MET_HARMONICS                   metHarData;
    
    /* Metrology Analog Front End Data */
    MET_AFE_DATA                    metAFEData;
    
    /* Metrology Calibration interface */
    MET_CALIBRATION                 metCalibration;

    /* IPC metrology lib integration Callback */
    DRV_METROLOGY_CALLBACK          newIntegrationCallback;

    /* Application Calibration Callback */
    //DRV_MET_CALIBRATION_CALLBACK    calibrationCallback;

    /* Application Harmonics Callback */
    //DRV_MET_HARMONICS_CALLBACK      harmonicsCallback;

} DRV_METROLOGY_OBJ;

#ifdef __cplusplus
}
#endif

#endif // #ifndef _DRV_METROLOGY_DEFINITIONS_H
/*******************************************************************************
 End of File
*/
