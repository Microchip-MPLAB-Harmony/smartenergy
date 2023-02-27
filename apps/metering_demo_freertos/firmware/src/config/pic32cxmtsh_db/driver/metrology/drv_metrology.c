/*******************************************************************************
  Metrology Driver.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_metrology.c

  Summary:
    Metrology Driver source file.

  Description:
    None
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

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "system/int/sys_int.h"
#include "drv_metrology.h"
#include "drv_metrology_definitions.h"
#include "osal/osal.h"
#include "peripheral/pio/plib_pio.h"

#ifdef __cplusplus // Provide C++ Compatibility
    extern "C" {
#endif

/* Define a semaphore to signal the Metrology Tasks to process new integration
 * data */
OSAL_SEM_DECLARE(drvMetrologySemID);

typedef enum {
    PENERGY = 0,
    QENERGY = 1,
} DRV_METROLOGY_ENERGY_TYPE;        

/* This is the driver instance object array. */
DRV_METROLOGY_OBJ gDrvMetObj;

static CACHE_ALIGN uint32_t sCaptureBuffer[CACHE_ALIGNED_SIZE_GET(MET_CAPTURE_BUF_SIZE)];

const DRV_METROLOGY_CONTROL gDrvMetControlDefault =
{
    STATE_CTRL_STATE_CTRL_RESET_Val,                /* 00 STATE_CTRL */
    _UINT32_(DRV_METROLOGY_CONF_FCTRL0),            /* 01 FEATURE_CTRL0 */
    _UINT32_(DRV_METROLOGY_CONF_FCTRL1),            /* 02 FEATURE_CTRL1 */
    _UINT32_(DRV_METROLOGY_CONF_MT),                /* 03 METER_TYPE sensor_type =0 CT, 1 SHUNT, 2 ROGOWSKI */
    _UINT32_(0x00000000),                           /* 04 M M=50->50Hz M=60->60Hz */
    _UINT32_(0x00001130),                           /* 05 N_MAX 4400=0x1130 */
    _UINT32_(DRV_METROLOGY_CONF_PULSE0_CTRL),       /* 06 PULSE0_CTRL */
    _UINT32_(DRV_METROLOGY_CONF_PULSE1_CTRL),       /* 07 PULSE1_CTRL */
    _UINT32_(DRV_METROLOGY_CONF_PULSE2_CTRL),       /* 08 PULSE2_CTRL */
    _UINT32_(DRV_METROLOGY_CONF_PKT),               /* 09 P_K_T */
    _UINT32_(DRV_METROLOGY_CONF_PKT),               /* 10 Q_K_T */
    _UINT32_(DRV_METROLOGY_CONF_PKT),               /* 11 I_K_T */
    _UINT32_(DRV_METROLOGY_CONF_CREEP_P),           /* 12 CREEP_THR_P */
    _UINT32_(DRV_METROLOGY_CONF_CREEP_Q),           /* 13 CREEP_THR_Q */
    _UINT32_(DRV_METROLOGY_CONF_CREEP_I),           /* 14 CREEP_THR_I */
    _UINT32_(0x00000000),                           /* 15 POWER_OFFSET_CTRL */
    _UINT32_(0x00000000),                           /* 16 POWER_OFFSET_P */
    _UINT32_(0x00000000),                           /* 17 POWER_OFFSET_Q */
    _UINT32_(DRV_METROLOGY_CONF_SWELL),             /* 18 SWELL_THR_VA */
    _UINT32_(DRV_METROLOGY_CONF_SWELL),             /* 19 SWELL_THR_VB */
    _UINT32_(DRV_METROLOGY_CONF_SWELL),             /* 20 SWELL_THR_VC */
    _UINT32_(DRV_METROLOGY_CONF_SAG),               /* 21 SAG_THR_VA */
    _UINT32_(DRV_METROLOGY_CONF_SAG),               /* 22 SAG_THR_VB */
    _UINT32_(DRV_METROLOGY_CONF_SAG),               /* 23 SAG_THR_VC */
    _UINT32_(DRV_METROLOGY_CONF_KI),                /* 24 K_IA */
    _UINT32_(DRV_METROLOGY_CONF_KV),                /* 25 K_VA */
    _UINT32_(DRV_METROLOGY_CONF_KI),                /* 26 K_IB */
    _UINT32_(DRV_METROLOGY_CONF_KV),                /* 27 K_VB */
    _UINT32_(DRV_METROLOGY_CONF_KI),                /* 28 K_IC */
    _UINT32_(DRV_METROLOGY_CONF_KV),                /* 29 K_VC */
    _UINT32_(DRV_METROLOGY_CONF_KI),                /* 30 K_IN */
    _UINT32_(0x20000000),                           /* 31 CAL_M_IA */
    _UINT32_(0x20000000),                           /* 32 CAL_M_VA */
    _UINT32_(0x20000000),                           /* 33 CAL_M_IB */
    _UINT32_(0x20000000),                           /* 34 CAL_M_VB */
    _UINT32_(0x20000000),                           /* 35 CAL_M_IC */
    _UINT32_(0x20000000),                           /* 36 CAL_M_VC */
    _UINT32_(0x20000000),                           /* 37 CAL_M_IN */
    _UINT32_(0x00000000),                           /* 38 CAL_PH_IA */
    _UINT32_(0x00000000),                           /* 39 CAL_PH_VA */
    _UINT32_(0x00000000),                           /* 40 CAL_PH_IB */
    _UINT32_(0x00000000),                           /* 41 CAL_PH_VB */
    _UINT32_(0x00000000),                           /* 42 CAL_PH_IC */
    _UINT32_(0x00000000),                           /* 43 CAL_PH_VC */
    _UINT32_(0x00000000),                           /* 44 CAL_PH_IN */
    _UINT32_(DRV_METROLOGY_CONF_WAVEFORM),          /* 45 CAPTURE_CTRL */
    _UINT32_(DRV_METROLOGY_CAPTURE_BUF_SIZE),       /* 46 CAPTURE_BUFF_SIZE */
    _UINT32_(sCaptureBuffer),                       /* 47 CAPTURE_ADDR */
    _UINT32_(0x00000000),                           /* 48 RESERVED_C48 */
    _UINT32_(0x00000000),                           /* 49 RESERVED_C49 */
    _UINT32_(0x00000000),                           /* 50 RESERVED_C50 */
    _UINT32_(DRV_METROLOGY_CONF_ATS2023),           /* 51 ATSENSE_CTRL_20_23 */
    _UINT32_(DRV_METROLOGY_CONF_ATS2427),           /* 52 ATSENSE_CTRL_24_27 */
    _UINT32_(0x00000003),                           /* 53 ATSENSE_CTRL_28_2B: MSB_MODE=0,OSR=3 */
    _UINT32_(0x00000000),                           /* 54 RESERVED_C54 */
    _UINT32_(0x00000000),                           /* 55 POWER_OFFSET_P_A */
    _UINT32_(0x00000000),                           /* 56 POWER_OFFSET_P_B */
    _UINT32_(0x00000000),                           /* 57 POWER_OFFSET_P_C */
    _UINT32_(0x00000000),                           /* 58 POWER_OFFSET_Q_A */
    _UINT32_(0x00000000),                           /* 59 POWER_OFFSET_Q_B */
    _UINT32_(0x00000000)                            /* 60 POWER_OFFSET_Q_C */
};

static void _DRV_Metrology_copy (uintptr_t pDst, uintptr_t pSrc, size_t length)
{
    uint16_t size;

    /* Enable PMC clock */
    PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk | PMC_PCR_EN(1) | PMC_PCR_PID(ID_MEM2MEM0);

    /* Configure Transfer Size */
    if (!(length & 0x03)) 
    {
        MEM2MEM0_REGS->MEM2MEM_MR = MEM2MEM_MR_TSIZE_T_32BIT;
        size = length >> 2;
    } 
    else if (!(length & 0x01)) 
    {
        MEM2MEM0_REGS->MEM2MEM_MR = MEM2MEM_MR_TSIZE_T_16BIT;
        size = length >> 1;
    } 
    else 
    {
        MEM2MEM0_REGS->MEM2MEM_MR = MEM2MEM_MR_TSIZE_T_8BIT;
        size = length;
    }

    /* Prepare MEM2MEM transfer */
    MEM2MEM0_REGS->MEM2MEM_TPR = (uint32_t)pSrc;
    MEM2MEM0_REGS->MEM2MEM_TCR = size;
    MEM2MEM0_REGS->MEM2MEM_RPR = (uint32_t)pDst;
    MEM2MEM0_REGS->MEM2MEM_RCR = size;

    /* Start PDC transfer */
    MEM2MEM0_REGS->MEM2MEM_PTCR = (MEM2MEM_PTCR_RXTEN_Msk | MEM2MEM_PTCR_TXTEN_Msk);

    /* Wait until transfer done */
    while (0 == (MEM2MEM0_REGS->MEM2MEM_ISR & MEM2MEM_ISR_RXEND_Msk));

    /* Stop PDC transfer */
    MEM2MEM0_REGS->MEM2MEM_PTCR = (MEM2MEM_PTCR_RXTDIS_Msk | MEM2MEM_PTCR_TXTDIS_Msk);

    /* Disable PMC clock */
    PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk | PMC_PCR_EN(0) | PMC_PCR_PID(ID_MEM2MEM0);

}

void IPC1_Handler (void)
{
    uint32_t status = IPC1_REGS->IPC_ISR;
    status &= IPC1_REGS->IPC_IMR;

    if (status & DRV_METROLOGY_IPC_INIT_IRQ_MSK)
    {
        gDrvMetObj.state = DRV_METROLOGY_STATE_INIT_DSP;
    }

    if (status & DRV_METROLOGY_IPC_INTEGRATION_IRQ_MSK)
    {
        if (gDrvMetObj.metRegisters->MET_STATUS.STATUS == STATUS_STATUS_DSP_RUNNING)
        {
            /* Update Accumulators Data */
            _DRV_Metrology_copy((uintptr_t)&gDrvMetObj.metAccData, (uintptr_t)&gDrvMetObj.metRegisters->MET_ACCUMULATORS, sizeof(DRV_METROLOGY_ACCUMULATORS));
            /* Update Harmonics Data */
            _DRV_Metrology_copy((uintptr_t)&gDrvMetObj.metHarData, (uintptr_t)&gDrvMetObj.metRegisters->MET_HARMONICS, sizeof(DRV_METROLOGY_HARMONICS));
        }
    }

    IPC1_REGS->IPC_ICCR = status;

    /* Signal Metrology thread to update measurements for an integration period */
    OSAL_SEM_PostISR(&drvMetrologySemID);
}

static uint32_t _DRV_Metrology_GetVIRMS(uint64_t val, uint32_t k_x)
{
    double m;

    m = (double)(val);
    m = (m / RMS_DIV_Q);
    m = (m / gDrvMetObj.metRegisters->MET_STATUS.N);
    m = sqrt(m);
    m = m * k_x / RMS_DIV_G;
    m = m * 10000;

    return ((uint32_t)(m));
}

static uint32_t _DRV_Metrology_GetInxRMS(uint64_t val)
{
    double m;

    m = (double)(val);
    m = (m / RMS_DIV_Inx_Q);
    m = (m / gDrvMetObj.metRegisters->MET_STATUS.N);
    m = sqrt(m);
    m = m * 10000;

    return ((uint32_t)(m));
}

static uint32_t _DRV_Metrology_GetPQRMS(int64_t val, uint32_t k_ix, uint32_t k_vx)
{
    double m;

    if (val < 0)
    {
        m = (double)((~val) + 1);
    }
    else
    {
        m = (double)(val);
    }

    m = (m / RMS_DIV_Q);
    m = (m / gDrvMetObj.metRegisters->MET_STATUS.N);
    m = (m * k_ix * k_vx) / (RMS_DIV_G * RMS_DIV_G);
    m = m * 10;

    return ((uint32_t)(m));
}

static uint32_t _DRV_Metrology_CheckPQDir(int64_t val)
{
    if (val < 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static uint32_t _DRV_Metrology_CheckPQtDir(int64_t val_a, int64_t val_b, int64_t val_c)
{
    if ((val_a + val_b + val_c) < 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static uint32_t _DRV_Metrology_GetSRMS(int64_t pv, int64_t qv, uint32_t k_ix, uint32_t k_vx)
{
    double m, n;

    if (pv < 0)
    {
        m = (double)((~pv) + 1);
    }
    else
    {
        m = (double)(pv);
    }

    m = (m / RMS_DIV_Q);
    m = (m / gDrvMetObj.metRegisters->MET_STATUS.N);
    m = (m * k_ix * k_vx) / (RMS_DIV_G * RMS_DIV_G);
    m = m * 10;

    if (qv < 0)
    {
        n = (double)((~qv) + 1);
    }
    else
    {
        n = (double)(qv);
    }

    n = (n / RMS_DIV_Q);
    n = (n / gDrvMetObj.metRegisters->MET_STATUS.N);
    n = (n * k_ix * k_vx) / (RMS_DIV_G * RMS_DIV_G);
    n = n * 10;

    m = m * m;
    n = n * n;
    m = sqrt(m + n);

    return ((uint32_t)(m));
}

static uint32_t _DRV_Metrology_GetAngleRMS(int64_t p, int64_t q)
{
    double m;
    int32_t n;

    m = atan2(q, p);
    m = 180 * m;
    m = m * 100000;
    m = m / CONST_Pi;
    n = (int32_t)m;

    if (n < 0)
    {
        n = ~n + 1;
        n |= 0x80000000;
    }

    return ((uint32_t)(n));
}

static uint32_t _DRV_Metrology_GetPQEnergy(DRV_METROLOGY_ENERGY_TYPE id)
{
    double m, k;

    m = 0;
    k = 0;
    /* Calculated as absolute values */
    if (id == PENERGY)
    {
        if (gDrvMetObj.metAccData.P_A < 0)
        {
            m = (double)((~gDrvMetObj.metAccData.P_A) + 1);
        }
        else
        {
            m = (double)(gDrvMetObj.metAccData.P_A);
        }

        m = (m * gDrvMetObj.metRegisters->MET_CONTROL.K_IA * gDrvMetObj.metRegisters->MET_CONTROL.K_VA) / (RMS_DIV_G * RMS_DIV_G); /* m =m*k_v*k_i */
        m = (m / RMS_DIV_Q);            /* k =k/2^40 */
        k += (m / 4000);                /* k =k/fs */

        if (gDrvMetObj.metAccData.P_B < 0)
        {
            m = (double)((~gDrvMetObj.metAccData.P_B) + 1);
        }
        else
        {
            m = (double)(gDrvMetObj.metAccData.P_B);
        }

        m = (m * gDrvMetObj.metRegisters->MET_CONTROL.K_IB * gDrvMetObj.metRegisters->MET_CONTROL.K_VB) / (RMS_DIV_G * RMS_DIV_G); /* m =m*k_v*k_i */
        m = (m / RMS_DIV_Q);            /* k =k/2^40 */
        k += (m / 4000);                /* k =k/fs */

        if (gDrvMetObj.metAccData.P_C < 0)
        {
            m += (double)((~gDrvMetObj.metAccData.P_C) + 1);
        }
        else
        {
            m += (double)(gDrvMetObj.metAccData.P_C);
        }

        m = (m * gDrvMetObj.metRegisters->MET_CONTROL.K_IC * gDrvMetObj.metRegisters->MET_CONTROL.K_VC) / (RMS_DIV_G * RMS_DIV_G); /* m =m*k_v*k_i */
        m = (m / RMS_DIV_Q);            /* k =k/2^40 */
        k += (m / 4000);                /* k =k/fs */
    }
    else
    {
        /* reactive energy */
        if (gDrvMetObj.metAccData.Q_A < 0)
        {
            m = (double)((~gDrvMetObj.metAccData.Q_A) + 1);
        }
        else
        {
            m = (double)(gDrvMetObj.metAccData.Q_A);
        }

        m = (m * gDrvMetObj.metRegisters->MET_CONTROL.K_IA * gDrvMetObj.metRegisters->MET_CONTROL.K_VA) / (RMS_DIV_G * RMS_DIV_G); /* m =m*k_v*k_i */
        m = (m / RMS_DIV_Q);            /* k =k/2^40 */
        k += (m / 4000);                /* k =k/fs */

        if (gDrvMetObj.metAccData.Q_B < 0)
        {
            m = (double)((~gDrvMetObj.metAccData.Q_B) + 1);
        }
        else
        {
            m = (double)(gDrvMetObj.metAccData.Q_B);
        }

        m = (m * gDrvMetObj.metRegisters->MET_CONTROL.K_IB * gDrvMetObj.metRegisters->MET_CONTROL.K_VB) / (RMS_DIV_G * RMS_DIV_G); /* m =m*k_v*k_i */
        m = (m / RMS_DIV_Q);            /* k =k/2^40 */
        k += (m / 4000);                /* k =k/fs */

        if (gDrvMetObj.metAccData.Q_C < 0)
        {
            m = (double)((~gDrvMetObj.metAccData.Q_C) + 1);
        }
        else
        {
            m = (double)(gDrvMetObj.metAccData.Q_C);
        }

        m = (m * gDrvMetObj.metRegisters->MET_CONTROL.K_IC * gDrvMetObj.metRegisters->MET_CONTROL.K_VC) / (RMS_DIV_G * RMS_DIV_G); /* m =m*k_v*k_i */
        m = (m / RMS_DIV_Q);            /* k =k/2^40 */
        k += (m / 4000);                /* k =k/fs */
    }

    k = k / 3600;         /* xxxxxx (Wh/Varh) */
    k = k * 10000;        /* *10000 (kWh/kVarh) */

    return ((uint32_t)(k));  /* xxxx (kWh/kVarh) */
}

static void _DRV_Metrology_IpcInitialize (void)
{
    /* Clear interrupts */
    IPC1_REGS->IPC_ICCR = 0xFFFFFFFF;
    /* Enable interrupts */
    IPC1_REGS->IPC_IECR = DRV_METROLOGY_IPC_INIT_IRQ_MSK | DRV_METROLOGY_IPC_INTEGRATION_IRQ_MSK;
}

static uint32_t _DRV_Metrology_CorrectCalibrationAngle (uint32_t measured, double reference)
{
    int64_t measured_angle, correction_angle;
    uint64_t m;
        
    measured_angle = measured;
    if (measured & 0x80000000) 
    {
        measured_angle = 0x80000000 - measured_angle;
    }
    
    /* Improve accuracy */
    reference *= 100;
    correction_angle = measured_angle - (int64_t)reference;
    
    /* Correction angle should be between -180 and 180 degrees */
    while (correction_angle < (-18000000)) 
    {
        correction_angle += 36000000;
    }
    
    while (correction_angle > 18000000) 
    {
        correction_angle -= 36000000;
    }
    
    correction_angle = (correction_angle * 7158278827) / (gDrvMetObj.calibrationData.freq * 10000);
    correction_angle = (correction_angle + 50) / 100;
    m = (uint64_t)(abs(correction_angle));
    if (correction_angle < 0) 
    {
        m = (~m) + 1;
    }
    
    return (uint32_t)m;
}

static void _DRV_METROLOGY_UpdateMeasurements(void)
{
    uint32_t *afeRMS = NULL;
    uint32_t stateFlagReg;
    uint32_t freq;

    /* Get State Flag Register */
    stateFlagReg = gDrvMetObj.metRegisters->MET_STATUS.STATE_FLAG;
    freq = gDrvMetObj.metRegisters->MET_STATUS.FREQ;
            
    /* Update RMS values */
    afeRMS = gDrvMetObj.metAFEData.RMS;

    afeRMS[RMS_UA] = _DRV_Metrology_GetVIRMS(gDrvMetObj.metAccData.V_A, gDrvMetObj.metRegisters->MET_CONTROL.K_VA);
    afeRMS[RMS_UB] = _DRV_Metrology_GetVIRMS(gDrvMetObj.metAccData.V_B, gDrvMetObj.metRegisters->MET_CONTROL.K_VB);
    afeRMS[RMS_UC] = _DRV_Metrology_GetVIRMS(gDrvMetObj.metAccData.V_C, gDrvMetObj.metRegisters->MET_CONTROL.K_VC);

    afeRMS[RMS_IA] = _DRV_Metrology_GetVIRMS(gDrvMetObj.metAccData.I_A, gDrvMetObj.metRegisters->MET_CONTROL.K_IA);
    afeRMS[RMS_IB] = _DRV_Metrology_GetVIRMS(gDrvMetObj.metAccData.I_B, gDrvMetObj.metRegisters->MET_CONTROL.K_IB);
    afeRMS[RMS_IC] = _DRV_Metrology_GetVIRMS(gDrvMetObj.metAccData.I_C, gDrvMetObj.metRegisters->MET_CONTROL.K_IC);

    afeRMS[RMS_INI] = _DRV_Metrology_GetInxRMS(gDrvMetObj.metAccData.I_Ni);
    afeRMS[RMS_INM] = _DRV_Metrology_GetVIRMS(gDrvMetObj.metAccData.I_Nm, gDrvMetObj.metRegisters->MET_CONTROL.K_IN);
    afeRMS[RMS_INMI] = _DRV_Metrology_GetInxRMS(gDrvMetObj.metAccData.I_Nmi);

    afeRMS[RMS_PA]  = _DRV_Metrology_GetPQRMS(gDrvMetObj.metAccData.P_A, gDrvMetObj.metRegisters->MET_CONTROL.K_IA, gDrvMetObj.metRegisters->MET_CONTROL.K_VA);
    gDrvMetObj.metAFEData.afeEvents.paDir = _DRV_Metrology_CheckPQDir(gDrvMetObj.metAccData.P_A);
    afeRMS[RMS_PB]  = _DRV_Metrology_GetPQRMS(gDrvMetObj.metAccData.P_B, gDrvMetObj.metRegisters->MET_CONTROL.K_IB, gDrvMetObj.metRegisters->MET_CONTROL.K_VB);
    gDrvMetObj.metAFEData.afeEvents.pbDir = _DRV_Metrology_CheckPQDir(gDrvMetObj.metAccData.P_B);
    afeRMS[RMS_PC]  = _DRV_Metrology_GetPQRMS(gDrvMetObj.metAccData.P_C, gDrvMetObj.metRegisters->MET_CONTROL.K_IC, gDrvMetObj.metRegisters->MET_CONTROL.K_VC);
    gDrvMetObj.metAFEData.afeEvents.pcDir = _DRV_Metrology_CheckPQDir(gDrvMetObj.metAccData.P_C);

    afeRMS[RMS_QA]  = _DRV_Metrology_GetPQRMS(gDrvMetObj.metAccData.Q_A, gDrvMetObj.metRegisters->MET_CONTROL.K_IA, gDrvMetObj.metRegisters->MET_CONTROL.K_VA);
    gDrvMetObj.metAFEData.afeEvents.qaDir = _DRV_Metrology_CheckPQDir(gDrvMetObj.metAccData.Q_A);
    afeRMS[RMS_QB]  = _DRV_Metrology_GetPQRMS(gDrvMetObj.metAccData.Q_B, gDrvMetObj.metRegisters->MET_CONTROL.K_IB, gDrvMetObj.metRegisters->MET_CONTROL.K_VB);
    gDrvMetObj.metAFEData.afeEvents.qbDir = _DRV_Metrology_CheckPQDir(gDrvMetObj.metAccData.Q_B);
    afeRMS[RMS_QC]  = _DRV_Metrology_GetPQRMS(gDrvMetObj.metAccData.Q_C, gDrvMetObj.metRegisters->MET_CONTROL.K_IC, gDrvMetObj.metRegisters->MET_CONTROL.K_VC);
    gDrvMetObj.metAFEData.afeEvents.qcDir = _DRV_Metrology_CheckPQDir(gDrvMetObj.metAccData.Q_C);

    afeRMS[RMS_SA]  = _DRV_Metrology_GetSRMS(gDrvMetObj.metAccData.P_A, gDrvMetObj.metAccData.Q_A, gDrvMetObj.metRegisters->MET_CONTROL.K_IA, gDrvMetObj.metRegisters->MET_CONTROL.K_VA);
    afeRMS[RMS_SB]  = _DRV_Metrology_GetSRMS(gDrvMetObj.metAccData.P_B, gDrvMetObj.metAccData.Q_B, gDrvMetObj.metRegisters->MET_CONTROL.K_IB, gDrvMetObj.metRegisters->MET_CONTROL.K_VB);
    afeRMS[RMS_SC]  = _DRV_Metrology_GetSRMS(gDrvMetObj.metAccData.P_C, gDrvMetObj.metAccData.Q_C, gDrvMetObj.metRegisters->MET_CONTROL.K_IC, gDrvMetObj.metRegisters->MET_CONTROL.K_VC);

    afeRMS[RMS_PT]  = afeRMS[RMS_PA] + afeRMS[RMS_PB] + afeRMS[RMS_PC];
    gDrvMetObj.metAFEData.afeEvents.ptDir = _DRV_Metrology_CheckPQtDir(gDrvMetObj.metAccData.P_A, gDrvMetObj.metAccData.P_B, gDrvMetObj.metAccData.P_C);

    afeRMS[RMS_QT]  = afeRMS[RMS_QA] + afeRMS[RMS_QB] + afeRMS[RMS_QC];
    gDrvMetObj.metAFEData.afeEvents.qtDir = _DRV_Metrology_CheckPQtDir(gDrvMetObj.metAccData.Q_A, gDrvMetObj.metAccData.Q_B, gDrvMetObj.metAccData.Q_C);

    afeRMS[RMS_ST]  = afeRMS[RMS_SA] + afeRMS[RMS_SB] + afeRMS[RMS_SC];

    afeRMS[RMS_FREQ]  = (freq * 100) >> FREQ_Q;

    afeRMS[RMS_ANGLEA]  = _DRV_Metrology_GetAngleRMS(gDrvMetObj.metAccData.P_A, gDrvMetObj.metAccData.Q_A);
    afeRMS[RMS_ANGLEB]  = _DRV_Metrology_GetAngleRMS(gDrvMetObj.metAccData.P_B, gDrvMetObj.metAccData.Q_B);
    afeRMS[RMS_ANGLEC]  = _DRV_Metrology_GetAngleRMS(gDrvMetObj.metAccData.P_C, gDrvMetObj.metAccData.Q_C);
    afeRMS[RMS_ANGLEN]  = _DRV_Metrology_GetAngleRMS(gDrvMetObj.metAccData.P_N, gDrvMetObj.metAccData.Q_N);

    gDrvMetObj.metAFEData.energy += _DRV_Metrology_GetPQEnergy(PENERGY);
    
    /* Update Swell/Sag events */
    gDrvMetObj.metAFEData.afeEvents.sagA = stateFlagReg & STATUS_STATE_FLAG_SAG_DET_VA_Msk? 1 : 0;
    gDrvMetObj.metAFEData.afeEvents.sagB = stateFlagReg & STATUS_STATE_FLAG_SAG_DET_VB_Msk? 1 : 0;
    gDrvMetObj.metAFEData.afeEvents.sagC = stateFlagReg & STATUS_STATE_FLAG_SAG_DET_VC_Msk? 1 : 0;
    gDrvMetObj.metAFEData.afeEvents.swellA = stateFlagReg & STATUS_STATE_FLAG_SWELL_DET_VA_Msk? 1 : 0;
    gDrvMetObj.metAFEData.afeEvents.swellB = stateFlagReg & STATUS_STATE_FLAG_SWELL_DET_VB_Msk? 1 : 0;
    gDrvMetObj.metAFEData.afeEvents.swellC = stateFlagReg & STATUS_STATE_FLAG_SWELL_DET_VC_Msk? 1 : 0;

}

static bool _DRV_METROLOGY_UpdateCalibrationValues(void)
{
    DRV_METROLOGY_CALIBRATION * pCalibrationData;
    DRV_METROLOGY_ACCUMULATORS * pMetAccRegs;
    
    pCalibrationData = &gDrvMetObj.calibrationData;
    pMetAccRegs = &gDrvMetObj.metAccData;
    
    if (pCalibrationData->numIntegrationPeriods)
    {
        pCalibrationData->numIntegrationPeriods--;
        
        /* Update Accumulators */
        pCalibrationData->dspAccIa += pMetAccRegs->I_A;
        pCalibrationData->dspAccIb += pMetAccRegs->I_B;
        pCalibrationData->dspAccIc += pMetAccRegs->I_C;
        pCalibrationData->dspAccUa += pMetAccRegs->V_A;
        pCalibrationData->dspAccUb += pMetAccRegs->V_B;
        pCalibrationData->dspAccUc += pMetAccRegs->V_C;
        pCalibrationData->dspAccPa += pMetAccRegs->P_A_F;
        pCalibrationData->dspAccPb += pMetAccRegs->P_B_F;
        pCalibrationData->dspAccPc += pMetAccRegs->P_C_F;
        pCalibrationData->dspAccQa += pMetAccRegs->Q_A_F;
        pCalibrationData->dspAccQb += pMetAccRegs->Q_B_F;
        pCalibrationData->dspAccQc += pMetAccRegs->Q_C_F;
        
        return false;
    }
    else
    {
        DRV_METROLOGY_CONTROL * pMetControlRegs;
        uint64_t m;
        uint32_t k;
        
        pMetControlRegs = &gDrvMetObj.metRegisters->MET_CONTROL;
    
        /* The number of the required integration periods has been completed */
        /* Get Calibration Values */
        if ((pCalibrationData->references.lineId == PHASE_A) || (pCalibrationData->references.lineId == PHASE_T))
        {
            /* Calibration I RMS */
            pCalibrationData->dspAccIa >>= 2;
            k = _DRV_Metrology_GetVIRMS(pCalibrationData->dspAccIa, pMetControlRegs->K_IA);
            m = pCalibrationData->references.aimIA;
            m = m << CAL_VI_Q;
            pMetControlRegs->CAL_M_IA = (uint32_t)(m / k);
            if ((((m % k) * 10) / k) > 4) 
            {
                pMetControlRegs->CAL_M_IA += 1;
            }
            /* Calibration V RMS */
            pCalibrationData->dspAccUa >>= 2;
            k = _DRV_Metrology_GetVIRMS(pCalibrationData->dspAccUa, pMetControlRegs->K_VA);
            m = pCalibrationData->references.aimVA;
            m = m << CAL_VI_Q;
            pMetControlRegs->CAL_M_VA = (uint32_t)(m / k);
            if ((((m % k) * 10) / k) > 4) 
            {
                pMetControlRegs->CAL_M_VA += 1;
            }

            /* Calibration Phase */
            pCalibrationData->dspAccPa >>= 2;
            pCalibrationData->dspAccQa >>= 2;
            k = _DRV_Metrology_GetAngleRMS(pCalibrationData->dspAccPa, pCalibrationData->dspAccQa);
            pMetControlRegs->CAL_PH_IA = _DRV_Metrology_CorrectCalibrationAngle(k, pCalibrationData->references.angleA);
            
            pCalibrationData->result = true;
        }
        
        if ((pCalibrationData->references.lineId == PHASE_B) || (pCalibrationData->references.lineId == PHASE_T))
        {
            /* Calibration I RMS */
            pCalibrationData->dspAccIb >>= 2;
            k = _DRV_Metrology_GetVIRMS(pCalibrationData->dspAccIb, pMetControlRegs->K_IB);
            m = pCalibrationData->references.aimIB;
            m = m << CAL_VI_Q;
            pMetControlRegs->CAL_M_IB = (uint32_t)(m / k);
            if ((((m % k) * 10) / k) > 4) 
            {
                pMetControlRegs->CAL_M_IB += 1;
            }
            /* Calibration V RMS */
            pCalibrationData->dspAccUb >>= 2;
            k = _DRV_Metrology_GetVIRMS(pCalibrationData->dspAccUb, pMetControlRegs->K_VB);
            m = pCalibrationData->references.aimVB;
            m = m << CAL_VI_Q;
            pMetControlRegs->CAL_M_VB = (uint32_t)(m / k);
            if ((((m % k) * 10) / k) > 4) 
            {
                pMetControlRegs->CAL_M_VB += 1;
            }

            /* Calibration Phase */
            pCalibrationData->dspAccPb >>= 2;
            pCalibrationData->dspAccQb >>= 2;
            k = _DRV_Metrology_GetAngleRMS(pCalibrationData->dspAccPb, pCalibrationData->dspAccQb);
            pMetControlRegs->CAL_PH_IB = _DRV_Metrology_CorrectCalibrationAngle(k, pCalibrationData->references.angleB);
            
            pCalibrationData->result = true;
        }
        
        if ((pCalibrationData->references.lineId == PHASE_C) || (pCalibrationData->references.lineId == PHASE_T))
        {
            /* Calibration I RMS */
            pCalibrationData->dspAccIc >>= 2;
            k = _DRV_Metrology_GetVIRMS(pCalibrationData->dspAccIc, pMetControlRegs->K_IC);
            m = pCalibrationData->references.aimIC;
            m = m << CAL_VI_Q;
            pMetControlRegs->CAL_M_IC = (uint32_t)(m / k);
            if ((((m % k) * 10) / k) > 4) 
            {
                pMetControlRegs->CAL_M_IC += 1;
            }
            /* Calibration V RMS */
            pCalibrationData->dspAccUc >>= 2;
            k = _DRV_Metrology_GetVIRMS(pCalibrationData->dspAccUc, pMetControlRegs->K_VC);
            m = pCalibrationData->references.aimVC;
            m = m << CAL_VI_Q;
            pMetControlRegs->CAL_M_VC = (uint32_t)(m / k);
            if ((((m % k) * 10) / k) > 4) 
            {
                pMetControlRegs->CAL_M_VC += 1;
            }

            /* Calibration Phase */
            pCalibrationData->dspAccPc >>= 2;
            pCalibrationData->dspAccQc >>= 2;
            k = _DRV_Metrology_GetAngleRMS(pCalibrationData->dspAccPc, pCalibrationData->dspAccQc);
            pMetControlRegs->CAL_PH_IC = _DRV_Metrology_CorrectCalibrationAngle(k, pCalibrationData->references.angleC);
            
            pCalibrationData->result = true;
        }
        
        /* Restore FEATURE_CTRL0 after calibration */
        pMetControlRegs->FEATURE_CTRL0 = pCalibrationData->featureCtrl0Backup;
        
        /* Calibration has been completed */
        pCalibrationData->running = false;
        
        return true;
    }
}

static bool _DRV_METROLOGY_UpdateHarmonicAnalysisValues(void)
{
    uint32_t controlMReq;
    uint32_t statusMConf;

    controlMReq = (gDrvMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL1 & FEATURE_CTRL1_HARMONIC_m_REQ_Msk);
    controlMReq >>= FEATURE_CTRL1_HARMONIC_m_REQ_Pos;

    statusMConf = (gDrvMetObj.metRegisters->MET_STATUS.STATE_FLAG & STATUS_STATE_FLAG_HARMONIC_m_CONF_Msk);
    statusMConf >>= STATUS_STATE_FLAG_HARMONIC_m_CONF_Pos;

    if (controlMReq == statusMConf)
    {
        DRV_METROLOGY_HARMONICS_RMS *pHarmonicsRsp = gDrvMetObj.harmonicAnalysisData.pHarmonicAnalysisResponse;
        double sqrt2;
        uint64_t div;
        uint32_t *pHarReg;
        uint8_t index;
        uint32_t harTemp[12];

        pHarReg = (uint32_t *)&gDrvMetObj.metHarData;
        for (index = 0; index < 14; index++, pHarReg++) {
            if (*pHarReg > RMS_HARMONIC) {
                harTemp[index] = (~(*pHarReg)) + 1;
            } else {
                harTemp[index] = *pHarReg;
            }
        }

        sqrt2 = sqrt(2);
        div = gDrvMetObj.metRegisters->MET_STATUS.N << 6; /* format sQ25.6 */

        pHarmonicsRsp->Irms_A_m = (sqrt(pow((double)harTemp[0], 2) + pow((double)harTemp[7], 2)) * sqrt2) / div;
        pHarmonicsRsp->Irms_B_m = (sqrt(pow((double)harTemp[2], 2) + pow((double)harTemp[9], 2)) * sqrt2) / div;
        pHarmonicsRsp->Irms_C_m = (sqrt(pow((double)harTemp[4], 2) + pow((double)harTemp[11], 2)) * sqrt2) / div;
        pHarmonicsRsp->Irms_N_m = (sqrt(pow((double)harTemp[6], 2) + pow((double)harTemp[13], 2)) * sqrt2) / div;
        pHarmonicsRsp->Vrms_A_m = (sqrt(pow((double)harTemp[1], 2) + pow((double)harTemp[8], 2)) * sqrt2) / div;
        pHarmonicsRsp->Vrms_B_m = (sqrt(pow((double)harTemp[3], 2) + pow((double)harTemp[10], 2)) * sqrt2) / div;
        pHarmonicsRsp->Vrms_C_m = (sqrt(pow((double)harTemp[5], 2) + pow((double)harTemp[12], 2)) * sqrt2) / div;

        /* Disable Harmonic Analysis */
        gDrvMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL1 &= ~FEATURE_CTRL1_HARMONIC_EN_Msk;
        /* Clear Number of Harmonic for Analysis */
        gDrvMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL1 &= ~(FEATURE_CTRL1_HARMONIC_m_REQ_Msk);
        
        gDrvMetObj.harmonicAnalysisData.harmonicNum = statusMConf;
        gDrvMetObj.harmonicAnalysisData.running = false;

        return true;
    }
    else
    {
        return false;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Interface
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_METROLOGY_Initialize (SYS_MODULE_INIT * init, uint32_t resetCause)
{
    DRV_METROLOGY_INIT *metInit = (DRV_METROLOGY_INIT *)init;

    if ((gDrvMetObj.inUse == true) || (init == NULL))
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Create the Semaphore */
    if (OSAL_SEM_Create(&drvMetrologySemID, OSAL_SEM_TYPE_BINARY, 0, 0) == OSAL_RESULT_FALSE)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Disable IPC interrupts */
    SYS_INT_SourceDisable(IPC1_IRQn);

    if (resetCause != RSTC_SR_RSTTYP(RSTC_SR_RSTTYP_WDT0_RST_Val))
    {
        uint32_t tmp;

        /* Assert reset of the coprocessor and its peripherals */
        tmp = RSTC_REGS->RSTC_MR;
        tmp &= ~(RSTC_MR_CPROCEN_Msk | RSTC_MR_CPEREN_Msk);
        RSTC_REGS->RSTC_MR = RSTC_MR_KEY_PASSWD | tmp;
        
        /* Disable coprocessor Clocks */
        PMC_REGS->PMC_SCDR = PMC_SCDR_CPKEY_PASSWD | PMC_SCDR_CPCK_Msk;
        
        /* Disable coprocessor peripheral clocks */
        PMC_REGS->PMC_SCDR = PMC_SCDR_CPKEY_PASSWD | PMC_SCDR_CPBMCK_Msk;
        
        gDrvMetObj.binStartAddress = metInit->binStartAddress;
        gDrvMetObj.binSize = metInit->binEndAddress - metInit->binStartAddress;

        gDrvMetObj.status = SYS_STATUS_UNINITIALIZED;

        /* De-assert reset of the coprocessor peripherals */
        RSTC_REGS->RSTC_MR |= RSTC_MR_KEY_PASSWD | RSTC_MR_CPEREN_Msk;
        
        /* Enable coprocessor peripheral clocks */
        PMC_REGS->PMC_SCER = PMC_SCER_CPKEY_PASSWD | PMC_SCER_CPBMCK_Msk;
        
        /* Copy the Metrology bin file to SRAM1 */
        memcpy((uint32_t *)IRAM1_ADDR, (uint32_t *)gDrvMetObj.binStartAddress, gDrvMetObj.binSize);
    }

    /* Initialization of the interface with Metrology Lib */
    gDrvMetObj.metRegisters = (MET_REGISTERS *)metInit->regBaseAddress;
    gDrvMetObj.inUse = true;
    gDrvMetObj.integrationCallback = NULL;

    memset(&gDrvMetObj.metAccData, 0, sizeof(DRV_METROLOGY_ACCUMULATORS));
    memset(&gDrvMetObj.metHarData, 0, sizeof(DRV_METROLOGY_HARMONICS));
    memset(&gDrvMetObj.calibrationData, 0, sizeof(DRV_METROLOGY_CALIBRATION));
    memset(&gDrvMetObj.metAFEData, 0, sizeof(DRV_METROLOGY_AFE_DATA));

    /* Initialization of the Metrology object */
    gDrvMetObj.state = DRV_METROLOGY_STATE_HALT;
    gDrvMetObj.status = SYS_STATUS_READY;

    /* Configure IPC peripheral */
    _DRV_Metrology_IpcInitialize();

    return (SYS_MODULE_OBJ)&gDrvMetObj;
}

SYS_MODULE_OBJ DRV_METROLOGY_Reinitialize (SYS_MODULE_INIT * init)
{
    DRV_METROLOGY_INIT *metInit = (DRV_METROLOGY_INIT *)init;
    uint32_t tmp;

    if ((gDrvMetObj.inUse == false) || (init == NULL))
    {
        return SYS_MODULE_OBJ_INVALID;
    }
    
    /* Disable ICM : TBD -> icm_reset, disable ICM int, clear pending ICM */
    
    /* Disable IPC interrupts */
    SYS_INT_SourceDisable(IPC1_IRQn);

    /* Assert reset of the coprocessor and its peripherals */
    tmp = RSTC_REGS->RSTC_MR;
    tmp &= ~(RSTC_MR_CPROCEN_Msk | RSTC_MR_CPEREN_Msk);
    RSTC_REGS->RSTC_MR = RSTC_MR_KEY_PASSWD | tmp;

    /* Disable coprocessor Clocks */
    PMC_REGS->PMC_SCDR = PMC_SCDR_CPKEY_PASSWD | PMC_SCDR_CPCK_Msk;

    /* Disable coprocessor peripheral clocks */
    PMC_REGS->PMC_SCDR = PMC_SCDR_CPKEY_PASSWD | PMC_SCDR_CPBMCK_Msk;

    gDrvMetObj.binStartAddress = metInit->binStartAddress;
    gDrvMetObj.binSize = metInit->binEndAddress - metInit->binStartAddress;

    gDrvMetObj.status = SYS_STATUS_UNINITIALIZED;

    /* De-assert reset of the coprocessor peripherals */
    RSTC_REGS->RSTC_MR |= RSTC_MR_KEY_PASSWD | RSTC_MR_CPEREN_Msk;

    /* Enable coprocessor peripheral clocks */
    PMC_REGS->PMC_SCER = PMC_SCER_CPKEY_PASSWD | PMC_SCER_CPBMCK_Msk;

    /* Copy the Metrology bin file to SRAM1 */
    memcpy((uint32_t *)IRAM1_ADDR, (uint32_t *)gDrvMetObj.binStartAddress, gDrvMetObj.binSize);

    /* Initialization of the interface with Metrology Lib */
    gDrvMetObj.metRegisters = (MET_REGISTERS *)metInit->regBaseAddress;

    memset(&gDrvMetObj.metAccData, 0, sizeof(DRV_METROLOGY_ACCUMULATORS));
    memset(&gDrvMetObj.metHarData, 0, sizeof(DRV_METROLOGY_HARMONICS));
    memset(&gDrvMetObj.calibrationData, 0, sizeof(DRV_METROLOGY_CALIBRATION));
    memset(&gDrvMetObj.metAFEData, 0, sizeof(DRV_METROLOGY_AFE_DATA));

    /* Initialization of the Metrology object */
    gDrvMetObj.state = DRV_METROLOGY_STATE_HALT;
    gDrvMetObj.status = SYS_STATUS_READY;

    _DRV_Metrology_IpcInitialize();

    return (SYS_MODULE_OBJ)&gDrvMetObj;
}

DRV_METROLOGY_RESULT DRV_METROLOGY_Open (DRV_METROLOGY_START_MODE mode, DRV_METROLOGY_CONTROL * pConfiguration)
{
    if (gDrvMetObj.inUse == false)
    {
        return DRV_METROLOGY_ERROR;
    }

    if (gDrvMetObj.state != DRV_METROLOGY_STATE_HALT)
    {
        return DRV_METROLOGY_ERROR;
    }

    /* Enable IPC1 Interrupt Source */
    SYS_INT_SourceStatusClear(IPC1_IRQn);
    SYS_INT_SourceEnable(IPC1_IRQn);

    if (mode == DRV_METROLOGY_START_HARD)
    {
        /* De-assert the reset of the coprocessor (Core 1) */
        RSTC_REGS->RSTC_MR |= (RSTC_MR_KEY_PASSWD | RSTC_MR_CPROCEN_Msk);

        /* Enable the coprocessor clock (Core 1) */
        PMC_REGS->PMC_SCER = (PMC_SCER_CPKEY_PASSWD | PMC_SCER_CPCK_Msk);

        /* Wait IPC Init interrupt */
        while(gDrvMetObj.state != DRV_METROLOGY_STATE_INIT_DSP);

        /* Keep Metrology Lib in reset */
        gDrvMetObj.metRegisters->MET_CONTROL.STATE_CTRL = STATE_CTRL_STATE_CTRL_RESET_Val;
        
        if ((pConfiguration) && (pConfiguration->ATSENSE_CTRL_20_23 != 0))
        {
            /* Overwrite STATE CTRL register */
            pConfiguration->STATE_CTRL = STATE_CTRL_STATE_CTRL_RESET_Val;
            /* Load external Control values */
            memcpy(&gDrvMetObj.metRegisters->MET_CONTROL, pConfiguration, sizeof(DRV_METROLOGY_CONTROL));
        }
        else
        {
            /* Load default Control values */
            memcpy(&gDrvMetObj.metRegisters->MET_CONTROL, &gDrvMetControlDefault, sizeof(DRV_METROLOGY_CONTROL));
        }

        /* Set Metrology Lib state as Init */
        gDrvMetObj.metRegisters->MET_CONTROL.STATE_CTRL = STATE_CTRL_STATE_CTRL_INIT_Val;
    }

    return DRV_METROLOGY_SUCCESS;
}

DRV_METROLOGY_RESULT DRV_METROLOGY_Close (void)
{
    if (gDrvMetObj.inUse == false)
    {
        return DRV_METROLOGY_ERROR;
    }

    /* Disable IPC1 Interrupt Source */
    SYS_INT_SourceDisable(IPC1_IRQn);
    NVIC_ClearPendingIRQ(IPC1_IRQn);
    
    /* Update Driver state */
    gDrvMetObj.state = DRV_METROLOGY_STATE_HALT;
    gDrvMetObj.status = SYS_STATUS_BUSY;
    
    return DRV_METROLOGY_SUCCESS;

}

DRV_METROLOGY_RESULT DRV_METROLOGY_Start (void)
{
    if (gDrvMetObj.metRegisters->MET_STATUS.STATUS == STATUS_STATUS_READY)
    {
        /* Set Metrology Lib state as Run */
        gDrvMetObj.metRegisters->MET_CONTROL.STATE_CTRL = STATE_CTRL_STATE_CTRL_RUN_Val;

        return DRV_METROLOGY_SUCCESS;
    }

    return DRV_METROLOGY_ERROR;
}

DRV_METROLOGY_STATE DRV_METROLOGY_GetState (void)
{
    return (DRV_METROLOGY_STATE)gDrvMetObj.metRegisters->MET_STATUS.STATUS;
}

DRV_METROLOGY_RESULT DRV_METROLOGY_IntegrationCallbackRegister (
    DRV_METROLOGY_CALLBACK callback
)
{
    if (callback == NULL)
    {
        return DRV_METROLOGY_ERROR;
    }

    gDrvMetObj.integrationCallback = callback;
    return DRV_METROLOGY_SUCCESS;
}

DRV_METROLOGY_RESULT DRV_METROLOGY_CalibrationCallbackRegister (
    DRV_METROLOGY_CALIBRATION_CALLBACK callback
)
{
    if (callback == NULL)
    {
        return DRV_METROLOGY_ERROR;
    }

    gDrvMetObj.calibrationCallback = callback;
    return DRV_METROLOGY_SUCCESS;
}

DRV_METROLOGY_RESULT DRV_METROLOGY_HarmonicAnalysisCallbackRegister (
    DRV_METROLOGY_HARMONIC_ANALYSIS_CALLBACK callback
)
{
    if (callback == NULL)
    {
        return DRV_METROLOGY_ERROR;
    }

    gDrvMetObj.harmonicAnalysisCallback = callback;
    return DRV_METROLOGY_SUCCESS;
}

void DRV_METROLOGY_Tasks(SYS_MODULE_OBJ object)
{
    if (object == SYS_MODULE_OBJ_INVALID)
    {
        /* Invalid system object */
        return;
    }

    /* Wait for the metrology semaphore to get measurements at the end of the integration period. */
    OSAL_SEM_Pend(&drvMetrologySemID, OSAL_WAIT_FOREVER);

    /* Check if there is a calibration process running */
    if (gDrvMetObj.calibrationData.running)
    {
        if (_DRV_METROLOGY_UpdateCalibrationValues())
        {
            /* Launch calibration callback */
            if (gDrvMetObj.calibrationCallback)
            {
                gDrvMetObj.calibrationCallback(gDrvMetObj.calibrationData.result);
            }
        }
    }
    else
    {
        /* Update measurements from metrology library registers */
        _DRV_METROLOGY_UpdateMeasurements();

        /* Launch integration callback */
        if (gDrvMetObj.integrationCallback)
        {
            gDrvMetObj.integrationCallback();
        }
        
        /* Check if there is a harmonic analysis process running */
        if (gDrvMetObj.harmonicAnalysisData.running)
        {
            if (_DRV_METROLOGY_UpdateHarmonicAnalysisValues())
            {
                /* Launch calibration callback */
                if (gDrvMetObj.harmonicAnalysisCallback)
                {
                    gDrvMetObj.harmonicAnalysisCallback(gDrvMetObj.harmonicAnalysisData.harmonicNum);
                }
            }
        }
    }
}

DRV_METROLOGY_STATUS * DRV_METROLOGY_GetStatus (void)
{
    return &gDrvMetObj.metRegisters->MET_STATUS;
}

DRV_METROLOGY_CONTROL * DRV_METROLOGY_GetControl (void)
{
    return &gDrvMetObj.metRegisters->MET_CONTROL;
}

DRV_METROLOGY_CONTROL * DRV_METROLOGY_GetControlByDefault (void)
{
    return (DRV_METROLOGY_CONTROL *)&gDrvMetControlDefault;
}

DRV_METROLOGY_ACCUMULATORS * DRV_METROLOGY_GetAccData (void)
{
    return &gDrvMetObj.metAccData;
}

DRV_METROLOGY_HARMONICS * DRV_METROLOGY_GetHarData (void)
{
    return &gDrvMetObj.metHarData;
}

DRV_METROLOGY_CALIBRATION_REFS * DRV_METROLOGY_GetCalibrationReferences (void)
{
    return &gDrvMetObj.calibrationData.references;
}

void DRV_METROLOGY_SetControl (DRV_METROLOGY_CONTROL * pControl)
{
    uint32_t * pDstControl;
    uint32_t * pSrcControl;
    
    pDstControl = (uint32_t *)&gDrvMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL0;
    pSrcControl = (uint32_t *)&pControl->FEATURE_CTRL0;
    
    /* Keep State Control Register value */
    memcpy(pDstControl, pSrcControl, sizeof(DRV_METROLOGY_CONTROL) - sizeof(uint32_t));
    
}

uint32_t DRV_METROLOGY_GetEnergyValue(bool restartEnergy)
{
    uint32_t energy = gDrvMetObj.metAFEData.energy;

    if (restartEnergy)
    {
        gDrvMetObj.metAFEData.energy = 0;
    }

    return energy;
}

uint32_t DRV_METROLOGY_GetRMSValue(DRV_METROLOGY_RMS_TYPE type)
{
    int32_t value;
    
    value = gDrvMetObj.metAFEData.RMS[type];
    
    if (type >= RMS_ANGLEA)
    {
        if (value & 0x80000000) 
        {
            value &= 0x7FFFFFFF;
        }
        
        /* Absolute value should be between 0 and 180 degrees */
        while (value > 18000000) 
        {
            value -= 36000000;
        }
    }
    
    return (uint32_t)value;
}

DRV_METROLOGY_RMS_SIGN DRV_METROLOGY_GetRMSSign(DRV_METROLOGY_RMS_TYPE type)
{
    DRV_METROLOGY_RMS_SIGN sign;

    sign = RMS_SIGN_POSITIVE;

    if ((type == RMS_PA) && (gDrvMetObj.metAFEData.afeEvents.paDir))
    {
        sign = RMS_SIGN_NEGATIVE;
    }
    else if ((type == RMS_PB) && (gDrvMetObj.metAFEData.afeEvents.pbDir))
    {
        sign = RMS_SIGN_NEGATIVE;
    }
    else if ((type == RMS_PC) && (gDrvMetObj.metAFEData.afeEvents.pcDir))
    {
        sign = RMS_SIGN_NEGATIVE;
    }
    else if ((type == RMS_PT) && (gDrvMetObj.metAFEData.afeEvents.ptDir))
    {
        sign = RMS_SIGN_NEGATIVE;
    }
    else if ((type == RMS_QA) && (gDrvMetObj.metAFEData.afeEvents.qaDir))
    {
        sign = RMS_SIGN_NEGATIVE;
    }
    else if ((type == RMS_QB) && (gDrvMetObj.metAFEData.afeEvents.qbDir))
    {
        sign = RMS_SIGN_NEGATIVE;
    }
    else if ((type == RMS_QC) && (gDrvMetObj.metAFEData.afeEvents.qcDir))
    {
        sign = RMS_SIGN_NEGATIVE;
    }
    else if ((type == RMS_QT) && (gDrvMetObj.metAFEData.afeEvents.qtDir))
    {
        sign = RMS_SIGN_NEGATIVE;
    }
    else if ((type >= RMS_ANGLEA) && (gDrvMetObj.metAFEData.RMS[type] & 0x80000000))
    {
        sign = RMS_SIGN_NEGATIVE;
    }

    return sign;
}

void DRV_METROLOGY_SetConfiguration(DRV_METROLOGY_CONFIGURATION * config)
{
    uint64_t m;
    DRV_METROLOGY_CONTROL *pControl;
    uint32_t i = 0;
    uint32_t reg;
    
    /* Store Calibration parameters */
    gDrvMetObj.calibrationData.freq = config->freq;

    pControl = &gDrvMetObj.calibrationData.metControlConf;
    *pControl = gDrvMetObj.metRegisters->MET_CONTROL;

    m = 1000000000 / (config->mc);
    m = (m << GAIN_P_K_T_Q);
    m = m / 1000000;

    pControl->P_K_t = (uint32_t)m;
    pControl->Q_K_t = (uint32_t)m;
    pControl->I_K_t = (uint32_t)m;

    reg = pControl->METER_TYPE;
    reg &= ~(METER_TYPE_SENSOR_TYPE_I_A_Msk | METER_TYPE_SENSOR_TYPE_I_B_Msk | METER_TYPE_SENSOR_TYPE_I_C_Msk);
    reg |= METER_TYPE_SENSOR_TYPE_I_A(config->st) |
            METER_TYPE_SENSOR_TYPE_I_B(config->st) | METER_TYPE_SENSOR_TYPE_I_C(config->st);
    pControl->METER_TYPE = reg;

    reg = pControl->ATSENSE_CTRL_20_23;
    reg &= ~(ATSENSE_CTRL_20_23_I0_GAIN_Msk | ATSENSE_CTRL_20_23_I1_GAIN_Msk | ATSENSE_CTRL_20_23_I2_GAIN_Msk);
    reg |= ATSENSE_CTRL_20_23_I0_GAIN(config->gain) |
            ATSENSE_CTRL_20_23_I1_GAIN(config->gain) | ATSENSE_CTRL_20_23_I2_GAIN(config->gain);
    pControl->ATSENSE_CTRL_20_23 = reg;

    reg = pControl->ATSENSE_CTRL_24_27;
    reg &= ~(ATSENSE_CTRL_24_27_I3_GAIN_Msk);
    reg |= ATSENSE_CTRL_24_27_I3_GAIN(config->gain);
    pControl->ATSENSE_CTRL_24_27 = reg;

    if (config->st == SENSOR_CT)
    {
        double div;
        double res;
        
        m = config->tr * 1000000; /* improve accuracy * (ohm -> uohm) */
        div = config->rl * 1000000; /* improve accuracy * (ohm -> uohm) */
        div *= (1 << config->gain);
        res = m / div;
        res *= (1 << GAIN_VI_Q); /* format Q22.10 */
        i = (uint32_t) res;
    } 
    else if (config->st == SENSOR_ROGOWSKI) 
    {
        double res;
        double div;
        
        res = 1000000 / config->tr;
        div = 60 / config->freq; 
        res = res / ((1 << config->gain) * div);
        res *= (1 << GAIN_VI_Q); /* format Q22.10 */
        i = (uint32_t) res;
    } 
    else if (config->st == SENSOR_SHUNT) 
    {
        m = 1000000; /* (uOhm -> Ohm) */
        reg = (1 << config->gain) * config->rl;
        m = m / reg; /* (ohm -> mohm) */
        m = (m << GAIN_VI_Q); /* format Q22.10 */
        i = (uint32_t)(m);
    }

    pControl->K_IA = i;
    pControl->K_IB = i;
    pControl->K_IC = i;
    pControl->K_IN = i;

    i = (config->ku) << GAIN_VI_Q; /* format Q22.10 */
    pControl->K_VA = i;
    pControl->K_VB = i;
    pControl->K_VC = i;
    
    gDrvMetObj.metRegisters->MET_CONTROL = *pControl;

}

void DRV_METROLOGY_GetEventsData(DRV_METROLOGY_AFE_EVENTS * events)
{
    *events = gDrvMetObj.metAFEData.afeEvents;
}

void DRV_METROLOGY_StartCalibration(void)
{
    DRV_METROLOGY_CALIBRATION * pCalibrationData = &gDrvMetObj.calibrationData;
    
    if (!pCalibrationData->running)
    {
        DRV_METROLOGY_CONTROL * pMetControlRegs = &gDrvMetObj.metRegisters->MET_CONTROL;
    
        /* Set the number of integration periods to improve the accuracy of the calibration */
        pCalibrationData->numIntegrationPeriods = 4;
        
        /* Increase accuracy of references for calibrating procedure */
        pCalibrationData->references.aimIA *= 10000;
        pCalibrationData->references.aimVA *= 10000;
        pCalibrationData->references.angleA *= 1000;
        pCalibrationData->references.aimIB *= 10000;
        pCalibrationData->references.aimVB *= 10000;
        pCalibrationData->references.angleB *= 1000;
        pCalibrationData->references.aimIC *= 10000;
        pCalibrationData->references.aimVC *= 10000;
        pCalibrationData->references.angleC *= 1000;

        /* Save FEATURE_CTRL0 register value, to be restored after calibration */
        pCalibrationData->featureCtrl0Backup = pMetControlRegs->FEATURE_CTRL0;

        /* Init Accumulators */
        pCalibrationData->dspAccIa = 0;
        pCalibrationData->dspAccIb = 0;
        pCalibrationData->dspAccIc = 0;
        pCalibrationData->dspAccIn = 0;
        pCalibrationData->dspAccUa = 0;
        pCalibrationData->dspAccUb = 0;
        pCalibrationData->dspAccUc = 0;
        pCalibrationData->dspAccPa = 0;
        pCalibrationData->dspAccPb = 0;
        pCalibrationData->dspAccPc = 0;
        pCalibrationData->dspAccQa = 0;
        pCalibrationData->dspAccQb = 0;
        pCalibrationData->dspAccQc = 0;

        /* Initialize calibration registers to the default values */
        switch (pCalibrationData->references.lineId)
        {
            case PHASE_A:
                pMetControlRegs->FEATURE_CTRL0 = FEATURE_CTRL0_RZC_CHAN_SELECT(FEATURE_CTRL0_RZC_CHAN_SELECT_V1_Val) | 
                        FEATURE_CTRL0_SYNCH(FEATURE_CTRL0_SYNCH_A_Val) | FEATURE_CTRL0_PHASE_A_EN_Msk;
                break;

            case PHASE_B:
                pMetControlRegs->FEATURE_CTRL0 = FEATURE_CTRL0_RZC_CHAN_SELECT(FEATURE_CTRL0_RZC_CHAN_SELECT_V2_Val) | 
                        FEATURE_CTRL0_SYNCH(FEATURE_CTRL0_SYNCH_B_Val) | FEATURE_CTRL0_PHASE_B_EN_Msk;
                break;

            case PHASE_C:
                pMetControlRegs->FEATURE_CTRL0 = FEATURE_CTRL0_RZC_CHAN_SELECT(FEATURE_CTRL0_RZC_CHAN_SELECT_V3_Val) | 
                        FEATURE_CTRL0_SYNCH(FEATURE_CTRL0_SYNCH_C_Val) | FEATURE_CTRL0_PHASE_C_EN_Msk;
                break;

            case PHASE_T:
                pMetControlRegs->FEATURE_CTRL0 = FEATURE_CTRL0_RZC_CHAN_SELECT(FEATURE_CTRL0_RZC_CHAN_SELECT_V1_Val) | 
                        FEATURE_CTRL0_SYNCH(FEATURE_CTRL0_SYNCH_A_Val) | FEATURE_CTRL0_PHASE_A_EN_Msk |
                        FEATURE_CTRL0_PHASE_B_EN_Msk | FEATURE_CTRL0_PHASE_C_EN_Msk;
                break;

        }

        pCalibrationData->running = true;
        pCalibrationData->result = false;
    }
}

void DRV_METROLOGY_StartHarmonicAnalysis(uint8_t harmonicNum, DRV_METROLOGY_HARMONICS_RMS *pHarmonicResponse)
{
    if (!gDrvMetObj.harmonicAnalysisData.running)
    {
        gDrvMetObj.harmonicAnalysisData.running = true;

        /* Set Data pointer to store the Harmonic data result */
        gDrvMetObj.harmonicAnalysisData.pHarmonicAnalysisResponse = pHarmonicResponse;

        /* Set Number of Harmonic for Analysis */
        gDrvMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL1 &= ~(FEATURE_CTRL1_HARMONIC_m_REQ_Msk);
        gDrvMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL1 |= FEATURE_CTRL1_HARMONIC_m_REQ(harmonicNum);

        /* Enable Harmonic Analysis */
        gDrvMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL1 |= FEATURE_CTRL1_HARMONIC_EN_Msk;
    }
}

#ifdef __cplusplus // Provide C++ Compatibility
 }
#endif