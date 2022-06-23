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
#include <math.h>
#include "system/int/sys_int.h"
#include "drv_metrology.h"
#include "drv_metrology_definitions.h"

#include "peripheral/pio/plib_pio.h"

#ifdef __cplusplus // Provide C++ Compatibility
    extern "C" {
#endif

/* This is the driver instance object array. */
DRV_METROLOGY_OBJ gDrvMetObj;
static CACHE_ALIGN uint32_t sCaptureBuffer[CACHE_ALIGNED_SIZE_GET(MET_CAPTURE_BUF_SIZE)]; 

const MET_CONTROL gDrvMetControlDefault = 
{
	STATE_CTRL_STATE_CTRL_RESET_Val,        /* 00 STATE_CTRL */
	_UINT32_(0x00000300),                   /* 01 FEATURE_CTRL0 [PIC32CXMTC_DB=_UINT32_(0x00000700);  PIC32CXMTSH_DB=_UINT32_(0x00000300)*/
	_UINT32_(0x00000000),                   /* 02 FEATURE_CTRL1 */
	_UINT32_(0x00000CCC),                   /* 03 METER_TYPE sensor_type =0 CT, 1 SHUNT, 2 ROGOWSKI */
	_UINT32_(0x00000000),                   /* 04 M M=50->50Hz M=60->60Hz */
	_UINT32_(0x00001130),                   /* 05 N_MAX 4400=0x1130 */
	_UINT32_(0x81009100),                   /* 06 PULSE0_CTRL: enable pulse, detent NET, Watt-hours, width=80mS=80*464=0x9100 */
	_UINT32_(0x81029100),                   /* 07 PULSE1_CTRL: enable pulse, detent NET, Var-hours, width=80mS=80*464=0x9100 */
	_UINT32_(0x10049100),                   /* 08 PULSE2_CTRL: disable pulse */
	_UINT32_(0x00500000),                   /* 09 P_K_T=(1000/3200)*2^24=52428800   mc=3200 imp/kWh */
	_UINT32_(0x00500000),                   /* 10 Q_K_T */
	_UINT32_(0x00500000),                   /* 11 I_K_T */
	_UINT32_(0x00002E9A),                   /* 12 CREEP_THR_P 2w	0x2E9A=[2/(50*3600)]*2^30 */
	_UINT32_(0x00002E9A),                   /* 13 CREEP_THR_Q 2var 0x2E9A=[2/(50*3600)]*2^30 */
	_UINT32_(0x0000212D),                   /* 14 CREEP_THR_I 5mA K_Ix=617.283 8493=(5/617.283?*2^20 */
	_UINT32_(0x00000000),                   /* 15 POWER_OFFSET_CTRL, Disable Power offset */
	_UINT32_(0x00000000),                   /* 16 POWER_OFFSET_P */
	_UINT32_(0x00000000),                   /* 17 POWER_OFFSET_Q */
	_UINT32_(0x05E84F61),                   /* 18 SWELL_THR_VA	[(220*114%)/1651]^2*2^32 */
	_UINT32_(0x05E84F61),                   /* 19 SWELL_THR_VB */
	_UINT32_(0x05E84F61),                   /* 20 SWELL_THR_VC */
	_UINT32_(0x01A2EC26),                   /* 21 SAG_THR_VA	[(220*60%)/1651]^2*2^32 */
	_UINT32_(0x01A2EC26),                   /* 22 SAG_THR_VB */
	_UINT32_(0x01A2EC26),                   /* 23 SAG_THR_VC */
	_UINT32_(0x0009A523),                   /* 24 K_IA	[Te=2000/3.24]*2^10 */
	_UINT32_(0x0019CC00),                   /* 25 K_VA	1651*2^10 */
	_UINT32_(0x0009A523),                   /* 26 K_IB */
	_UINT32_(0x0019CC00),                   /* 27 K_VB */
	_UINT32_(0x0009A523),                   /* 28 K_IC */
	_UINT32_(0x0019CC00),                   /* 29 K_VC */
	_UINT32_(0x0009A523),                   /* 30 K_IN */
	_UINT32_(0x20000000),                   /* 31 CAL_M_IA */
	_UINT32_(0x20000000),                   /* 32 CAL_M_VA */
	_UINT32_(0x20000000),                   /* 33 CAL_M_IB */
	_UINT32_(0x20000000),                   /* 34 CAL_M_VB */
	_UINT32_(0x20000000),                   /* 35 CAL_M_IC */
	_UINT32_(0x20000000),                   /* 36 CAL_M_VC */
	_UINT32_(0x20000000),                   /* 37 CAL_M_IN */
	_UINT32_(0x00000000),                   /* 38 CAL_PH_IA */
	_UINT32_(0x00000000),                   /* 39 CAL_PH_VA */
	_UINT32_(0x00000000),                   /* 40 CAL_PH_IB */
	_UINT32_(0x00000000),                   /* 41 CAL_PH_VB */
	_UINT32_(0x00000000),                   /* 42 CAL_PH_IC */
	_UINT32_(0x00000000),                   /* 43 CAL_PH_VC */
	_UINT32_(0x00000000),                   /* 44 CAL_PH_IN */
	_UINT32_(0x00000000),                   /* 45 CAPTURE_CTRL */
	_UINT32_(MET_CAPTURE_BUF_SIZE),         /* 46 CAPTURE_BUFF_SIZE */
	_UINT32_(0x00000000),                   /* 47 CAPTURE_ADDR */
	_UINT32_(0x00000000),                   /* 48 RESERVED_C48 */
	_UINT32_(0x00000000),                   /* 49 RESERVED_C49 */
	_UINT32_(0x00000000),                   /* 50 RESERVED_C50 */
	_UINT32_(0x01010103),                   /* 51 ATSENSE_CTRL_20_23: I2GAIN=0,I2ON=1,V1ON=1,I1GAIN=0,I1ON=1,I0GAIN=0,TEMP=1,I0ON=1 */
	_UINT32_(0x07010101),                   /* 52 ATSENSE_CTRL_24_27: ONLDO=1,ONREF=1,ONBIAS=1,V3ON=1,I3GAIN=0,I3ON=1,V2ON=1 */
	_UINT32_(0x00000003),                   /* 53 ATSENSE_CTRL_28_2B: MSB_MODE=0,OSR=3 */
	_UINT32_(0x00000000),                   /* 54 RESERVED_C54 */
	_UINT32_(0x00000000),                   /* 55 POWER_OFFSET_P_A */
	_UINT32_(0x00000000),                   /* 56 POWER_OFFSET_P_B */
	_UINT32_(0x00000000),                   /* 57 POWER_OFFSET_P_C */
	_UINT32_(0x00000000),                   /* 58 POWER_OFFSET_Q_A */
	_UINT32_(0x00000000),                   /* 59 POWER_OFFSET_Q_B */
	_UINT32_(0x00000000)                    /* 60 POWER_OFFSET_Q_C */
};

static void _DRV_Metrology_copy (uintptr_t pDst, uintptr_t pSrc, size_t length)
{
    uint16_t size;
    
    /* Enable PMC clock */
    PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk | PMC_PCR_EN(1) | PMC_PCR_PID(ID_MEM2MEM0);
    
    /* Configure Transfer Size */
	if (!(length & 0x03)) {
		MEM2MEM0_REGS->MEM2MEM_MR = MEM2MEM_MR_TSIZE_T_32BIT;
		size = length >> 2;
	} else if (!(length & 0x01)) {
		MEM2MEM0_REGS->MEM2MEM_MR = MEM2MEM_MR_TSIZE_T_16BIT;
		size = length >> 1;
	} else {
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
	while (0 == (MEM2MEM0_REGS->MEM2MEM_ISR & MEM2MEM_ISR_RXEND_Msk)) {
	}
	/* Stop PDC transfer */
	MEM2MEM0_REGS->MEM2MEM_PTCR = (MEM2MEM_PTCR_RXTDIS_Msk | MEM2MEM_PTCR_TXTDIS_Msk);

	/* Disable PMC clock */
    PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk | PMC_PCR_EN(0) | PMC_PCR_PID(ID_MEM2MEM0);
    
}

//uint32_t counterIPC = 0;
//uint32_t counterIPC20 = 0;
//uint32_t counterIPC0 = 0;
void IPC1_Handler (void)
{
    uint32_t status = IPC1_REGS->IPC_ISR & IPC1_REGS->IPC_IMR;
    
//    counterIPC++;
    if (status & DRV_METROLOGY_IPC_INIT_IRQ_MSK)
    {
//        counterIPC20++;
        gDrvMetObj.state = DRV_METROLOGY_STATE_INIT_DSP;
    }
    
    if (status & DRV_METROLOGY_IPC_INTEGRATION_IRQ_MSK)
    {
//        counterIPC0++;
        gDrvMetObj.newMetData = true;
        
        if (gDrvMetObj.metRegisters->MET_STATUS.STATUS == STATUS_STATUS_DSP_RUNNING)
        {
            /* Update Accumulators Data */
            _DRV_Metrology_copy((uintptr_t)&gDrvMetObj.metAccData, (uintptr_t)&gDrvMetObj.metRegisters->MET_ACCUMULATORS, sizeof(MET_ACCUMULATORS));
            /* Update Harmonics Data */
            _DRV_Metrology_copy((uintptr_t)&gDrvMetObj.metHarData, (uintptr_t)&gDrvMetObj.metRegisters->MET_HARMONICS, sizeof(MET_HARMONICS));
        }
        
        if (gDrvMetObj.newIntegrationCallback)
        {
            gDrvMetObj.newIntegrationCallback();
        }
    }
    
    IPC1_REGS->IPC_IDCR = status;
    IPC1_REGS->IPC_ICCR = status;
    IPC1_REGS->IPC_IECR = status;
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
	float m;
	int32_t n;

	m = atan2(q, p);
	m = 180 * m;
	m = m * 1000;
	m = m / CONST_Pi;
	n = (int32_t)m;
	if (n < 0) 
    {
		n = ~n + 1;
		n |= 0x80000000;
	}

	return ((uint32_t)(n));
}

static uint32_t _DRV_Metrology_GetPQEnergy(MET_ENERGY_TYPE id, MET_ENERGY_MODE mode)
{
	double m, k;

	m = 0;
	k = 0;
	if (id == PENERGY) 
    {
		/* active energy */
		if (mode == ABS) 
        {
			/* abs */
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
			k += (m / gDrvMetObj.metRegisters->MET_STATUS.N);
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
			k += (m / gDrvMetObj.metRegisters->MET_STATUS.N);
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
			k += (m / gDrvMetObj.metRegisters->MET_STATUS.N);
		} 
        else 
        {
			/* algebra */
		}
	}
    else 
    {
		 /* reactive energy */
		if (mode == ABS) 
        {
			/* abs */
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
			k += (m / gDrvMetObj.metRegisters->MET_STATUS.N);
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
			k += (m / gDrvMetObj.metRegisters->MET_STATUS.N);
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
			k += (m / gDrvMetObj.metRegisters->MET_STATUS.N);
		} 
        else 
        {
			/* algebra */
		}
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

// *****************************************************************************
// *****************************************************************************
// Section: Interface
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_METROLOGY_Initialize ( const SYS_MODULE_INIT * const init, uint32_t resetCause )
{
    DRV_METROLOGY_INIT *metInit = (DRV_METROLOGY_INIT *)init;
    if(gDrvMetObj.inUse == true)
    {
        return SYS_MODULE_OBJ_INVALID;
    }
    
    if (resetCause != RSTC_SR_RSTTYP(RSTC_SR_RSTTYP_WDT0_RST_Val))
    {
        uint32_t tmp;

        /* Asserts the reset of the co-processor (Core 1) */
        tmp = RSTC_REGS->RSTC_MR & ~RSTC_MR_CPROCEN_Msk;
        RSTC_REGS->RSTC_MR = RSTC_MR_KEY_PASSWD | tmp;

        gDrvMetObj.status = SYS_STATUS_UNINITIALIZED;

        gDrvMetObj.binStartAddress = metInit->binStartAddress;
        gDrvMetObj.binSize = metInit->binEndAddress - metInit->binStartAddress;

        /* Copy the Metrology bin file to SRAM1 */
        memcpy((uint32_t *)IRAM1_ADDR, (uint32_t *)gDrvMetObj.binStartAddress, gDrvMetObj.binSize);
    }
    
    /* Initialization of the interface with Metrology Lib */
    gDrvMetObj.metRegisters = (MET_REGISTERS *)metInit->regBaseAddress;
    
    memset(&gDrvMetObj.metAccData, 0, sizeof(MET_ACCUMULATORS));
    memset(&gDrvMetObj.metHarData, 0, sizeof(MET_HARMONICS));
    memset(&gDrvMetObj.metCalibration, 0, sizeof(MET_CALIBRATION));
    memset(&gDrvMetObj.metAFEData, 0, sizeof(MET_AFE_DATA));
    
    /* Initialization of the Metrology object */
    gDrvMetObj.inUse = true;
    gDrvMetObj.newMetData = false;
    gDrvMetObj.state = DRV_METROLOGY_STATE_HALT;
    gDrvMetObj.status = SYS_STATUS_READY;
    gDrvMetObj.newIntegrationCallback = NULL;
    
    /* Disable IPC interrupts */
    SYS_INT_SourceDisable(IPC1_IRQn);
    
    /* Configure IPC peripheral */
    _DRV_Metrology_IpcInitialize();
    
    return 0;
}

DRV_METROLOGY_RESULT DRV_METROLOGY_Open ( DRV_METROLOGY_START_MODE mode )
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
        /* Enable the co-processor clock (Core 1) */
        PMC_REGS->PMC_SCER = (PMC_SCER_CPKEY_PASSWD | PMC_SCER_CPCK_Msk);
        
        /* Deasserts the reset of the co-processor (Core 1) */
        RSTC_REGS->RSTC_MR |= (RSTC_MR_KEY_PASSWD | RSTC_MR_CPROCEN_Msk);
        
        /* Wait IPC Init interrupt */
        while(gDrvMetObj.state != DRV_METROLOGY_STATE_INIT_DSP);
        
        /* Keep Metrology Lib in reset */
        gDrvMetObj.metRegisters->MET_CONTROL.STATE_CTRL = STATE_CTRL_STATE_CTRL_RESET_Val;
        
        /* Load default Control values */
        memcpy(&gDrvMetObj.metRegisters->MET_CONTROL, &gDrvMetControlDefault, sizeof(MET_CONTROL));

        /* Initialize Capture Buffer */
        gDrvMetObj.metRegisters->MET_CONTROL.CAPTURE_ADDR = (uint32_t)sCaptureBuffer;
        gDrvMetObj.metRegisters->MET_CONTROL.CAPTURE_BUFF_SIZE = MET_CAPTURE_BUF_SIZE;

        /* Set Metrology Lib state as Init */
        gDrvMetObj.metRegisters->MET_CONTROL.STATE_CTRL = STATE_CTRL_STATE_CTRL_INIT_Val;
    }
    
    return DRV_METROLOGY_SUCCESS;
}

DRV_METROLOGY_RESULT DRV_METROLOGY_Start ( void )
{
    if (gDrvMetObj.metRegisters->MET_STATUS.STATUS == STATUS_STATUS_READY)
    {
        /* Set Metrology Lib state as Run */
        gDrvMetObj.metRegisters->MET_CONTROL.STATE_CTRL = STATE_CTRL_STATE_CTRL_RUN_Val;
        
        return DRV_METROLOGY_SUCCESS;
    }
    
    return DRV_METROLOGY_ERROR;
}

DRV_METROLOGY_STATE DRV_METROLOGY_GetState ( void )
{
    return (DRV_METROLOGY_STATE)gDrvMetObj.metRegisters->MET_STATUS.STATUS;
}

DRV_METROLOGY_RESULT DRV_METROLOGY_IntegrationCallbackRegister ( DRV_METROLOGY_CALLBACK callback)
{
    if (callback == NULL)
    {
        return DRV_METROLOGY_ERROR;
    }
    
    gDrvMetObj.newIntegrationCallback = callback;
    return DRV_METROLOGY_SUCCESS;
}

MET_CONTROL * DRV_METROLOGY_GetControl ( void )
{
    return &gDrvMetObj.metRegisters->MET_CONTROL;
}

MET_CONTROL * DRV_METROLOGY_GetControlByDefault ( void )
{
    return (MET_CONTROL *)&gDrvMetControlDefault;
}

MET_ACCUMULATORS * DRV_METROLOGY_GetAccData ( void )
{
    return &gDrvMetObj.metAccData;
}

MET_HARMONICS * DRV_METROLOGY_GetHarData ( void )
{
    return &gDrvMetObj.metHarData;
}

void DRV_METROLOGY_SetControl ( MET_CONTROL * pControl )
{
    MET_CONTROL * pDstControl;
    
    pDstControl = &gDrvMetObj.metRegisters->MET_CONTROL;
    
    pDstControl->FEATURE_CTRL0 = pControl->FEATURE_CTRL0;      
    pDstControl->FEATURE_CTRL1 = pControl->FEATURE_CTRL1;      
    pDstControl->METER_TYPE = pControl->METER_TYPE; 
    pDstControl->M = pControl->M;    
    pDstControl->N_MAX = pControl->N_MAX;  
    pDstControl->PULSE0_CTRL = pControl->PULSE0_CTRL;    
    pDstControl->PULSE1_CTRL = pControl->PULSE1_CTRL;    
    pDstControl->PULSE2_CTRL = pControl->PULSE2_CTRL;    
    pDstControl->P_K_t = pControl->P_K_t;  
    pDstControl->Q_K_t = pControl->Q_K_t;  
    pDstControl->I_K_t = pControl->I_K_t;  
    pDstControl->CREEP_THRESHOLD_P = pControl->CREEP_THRESHOLD_P;  
    pDstControl->CREEP_THRESHOLD_Q = pControl->CREEP_THRESHOLD_Q;  
    pDstControl->CREEP_THRESHOLD_I = pControl->CREEP_THRESHOLD_I;  
    pDstControl->POWER_OFFSET_CTRL = pControl->POWER_OFFSET_CTRL;  
    pDstControl->POWER_OFFSET_P = pControl->POWER_OFFSET_P;     
    pDstControl->POWER_OFFSET_Q = pControl->POWER_OFFSET_Q;     
    pDstControl->SWELL_THRESHOLD_VA = pControl->SWELL_THRESHOLD_VA; 
    pDstControl->SWELL_THRESHOLD_VB = pControl->SWELL_THRESHOLD_VB; 
    pDstControl->SWELL_THRESHOLD_VC = pControl->SWELL_THRESHOLD_VC; 
    pDstControl->SAG_THRESHOLD_VA = pControl->SAG_THRESHOLD_VA;   
    pDstControl->SAG_THRESHOLD_VB = pControl->SAG_THRESHOLD_VB;   
    pDstControl->SAG_THRESHOLD_VC = pControl->SAG_THRESHOLD_VC;   
    pDstControl->K_IA = pControl->K_IA; 
    pDstControl->K_VA = pControl->K_VA; 
    pDstControl->K_IB = pControl->K_IB; 
    pDstControl->K_VB = pControl->K_VB; 
    pDstControl->K_IC = pControl->K_IC; 
    pDstControl->K_VC = pControl->K_VC; 
    pDstControl->K_IN = pControl->K_IN; 
    pDstControl->CAL_M_IA = pControl->CAL_M_IA; 
    pDstControl->CAL_M_VA = pControl->CAL_M_VA; 
    pDstControl->CAL_M_IB = pControl->CAL_M_IB; 
    pDstControl->CAL_M_VB = pControl->CAL_M_VB; 
    pDstControl->CAL_M_IC = pControl->CAL_M_IC; 
    pDstControl->CAL_M_VC = pControl->CAL_M_VC; 
    pDstControl->CAL_M_IN = pControl->CAL_M_IN; 
    pDstControl->CAL_PH_IA = pControl->CAL_PH_IA;  
    pDstControl->CAL_PH_VA = pControl->CAL_PH_VA;  
    pDstControl->CAL_PH_IB = pControl->CAL_PH_IB;  
    pDstControl->CAL_PH_VB = pControl->CAL_PH_VB;  
    pDstControl->CAL_PH_IC = pControl->CAL_PH_IC;  
    pDstControl->CAL_PH_VC = pControl->CAL_PH_VC;  
    pDstControl->CAL_PH_IN = pControl->CAL_PH_IN;  
    pDstControl->CAPTURE_CTRL = pControl->CAPTURE_CTRL;       
    pDstControl->ATSENSE_CTRL_20_23 = pControl->ATSENSE_CTRL_20_23; 
    pDstControl->ATSENSE_CTRL_24_27 = pControl->ATSENSE_CTRL_24_27; 
    pDstControl->ATSENSE_CTRL_28_2B = pControl->ATSENSE_CTRL_28_2B; 
    pDstControl->POWER_OFFSET_P_A = pControl->POWER_OFFSET_P_A;   
    pDstControl->POWER_OFFSET_P_B = pControl->POWER_OFFSET_P_B;   
    pDstControl->POWER_OFFSET_P_C = pControl->POWER_OFFSET_P_C;   
    pDstControl->POWER_OFFSET_Q_A = pControl->POWER_OFFSET_Q_A;   
    pDstControl->POWER_OFFSET_Q_B = pControl->POWER_OFFSET_Q_B;   
    pDstControl->POWER_OFFSET_Q_C = pControl->POWER_OFFSET_Q_C;   
}

void DRV_METROLOGY_UpdateMeasurements( void )
{
    uint32_t *afeRMS = NULL;
    /* TBD : calibration ?????? */
    
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
    gDrvMetObj.metAFEData.afeEvents.BIT.paDir = _DRV_Metrology_CheckPQDir(gDrvMetObj.metAccData.P_A);
    afeRMS[RMS_PB]  = _DRV_Metrology_GetPQRMS(gDrvMetObj.metAccData.P_B, gDrvMetObj.metRegisters->MET_CONTROL.K_IB, gDrvMetObj.metRegisters->MET_CONTROL.K_VB);
    gDrvMetObj.metAFEData.afeEvents.BIT.pbDir = _DRV_Metrology_CheckPQDir(gDrvMetObj.metAccData.P_B);
    afeRMS[RMS_PC]  = _DRV_Metrology_GetPQRMS(gDrvMetObj.metAccData.P_C, gDrvMetObj.metRegisters->MET_CONTROL.K_IC, gDrvMetObj.metRegisters->MET_CONTROL.K_VC);
    gDrvMetObj.metAFEData.afeEvents.BIT.pcDir = _DRV_Metrology_CheckPQDir(gDrvMetObj.metAccData.P_C);
    
    afeRMS[RMS_QA]  = _DRV_Metrology_GetPQRMS(gDrvMetObj.metAccData.Q_A, gDrvMetObj.metRegisters->MET_CONTROL.K_IA, gDrvMetObj.metRegisters->MET_CONTROL.K_VA);
    gDrvMetObj.metAFEData.afeEvents.BIT.qaDir = _DRV_Metrology_CheckPQDir(gDrvMetObj.metAccData.Q_A);
    afeRMS[RMS_QB]  = _DRV_Metrology_GetPQRMS(gDrvMetObj.metAccData.Q_B, gDrvMetObj.metRegisters->MET_CONTROL.K_IB, gDrvMetObj.metRegisters->MET_CONTROL.K_VB);
    gDrvMetObj.metAFEData.afeEvents.BIT.qbDir = _DRV_Metrology_CheckPQDir(gDrvMetObj.metAccData.Q_B);
    afeRMS[RMS_QC]  = _DRV_Metrology_GetPQRMS(gDrvMetObj.metAccData.Q_C, gDrvMetObj.metRegisters->MET_CONTROL.K_IC, gDrvMetObj.metRegisters->MET_CONTROL.K_VC);
    gDrvMetObj.metAFEData.afeEvents.BIT.qcDir = _DRV_Metrology_CheckPQDir(gDrvMetObj.metAccData.Q_C);
    
    afeRMS[RMS_SA]  = _DRV_Metrology_GetSRMS(gDrvMetObj.metAccData.P_A, gDrvMetObj.metAccData.Q_A, gDrvMetObj.metRegisters->MET_CONTROL.K_IA, gDrvMetObj.metRegisters->MET_CONTROL.K_VA);
    afeRMS[RMS_SB]  = _DRV_Metrology_GetSRMS(gDrvMetObj.metAccData.P_B, gDrvMetObj.metAccData.Q_B, gDrvMetObj.metRegisters->MET_CONTROL.K_IB, gDrvMetObj.metRegisters->MET_CONTROL.K_VB);
    afeRMS[RMS_SC]  = _DRV_Metrology_GetSRMS(gDrvMetObj.metAccData.P_C, gDrvMetObj.metAccData.Q_C, gDrvMetObj.metRegisters->MET_CONTROL.K_IC, gDrvMetObj.metRegisters->MET_CONTROL.K_VC);
    
    afeRMS[RMS_PT]  = afeRMS[RMS_PA] + afeRMS[RMS_PB] + afeRMS[RMS_PC];
    gDrvMetObj.metAFEData.afeEvents.BIT.ptDir = _DRV_Metrology_CheckPQtDir(gDrvMetObj.metAccData.P_A, gDrvMetObj.metAccData.P_B, gDrvMetObj.metAccData.P_C);
    
    afeRMS[RMS_QT]  = afeRMS[RMS_QA] + afeRMS[RMS_QB] + afeRMS[RMS_QC];
    gDrvMetObj.metAFEData.afeEvents.BIT.qtDir = _DRV_Metrology_CheckPQtDir(gDrvMetObj.metAccData.Q_A, gDrvMetObj.metAccData.Q_B, gDrvMetObj.metAccData.Q_C);
    
    afeRMS[RMS_ST]  = afeRMS[RMS_SA] + afeRMS[RMS_SB] + afeRMS[RMS_SC];
    
    afeRMS[RMS_FREQ]  = (gDrvMetObj.metRegisters->MET_STATUS.FREQ * 100) >> FREQ_Q;
    
    afeRMS[RMS_ANGLEA]  = _DRV_Metrology_GetAngleRMS(gDrvMetObj.metAccData.P_A, gDrvMetObj.metAccData.Q_A);
    afeRMS[RMS_ANGLEB]  = _DRV_Metrology_GetAngleRMS(gDrvMetObj.metAccData.P_B, gDrvMetObj.metAccData.Q_B);
    afeRMS[RMS_ANGLEC]  = _DRV_Metrology_GetAngleRMS(gDrvMetObj.metAccData.P_C, gDrvMetObj.metAccData.Q_C);
    afeRMS[RMS_ANGLEN]  = _DRV_Metrology_GetAngleRMS(gDrvMetObj.metAccData.P_N, gDrvMetObj.metAccData.Q_N);
    
    gDrvMetObj.metAFEData.energy += _DRV_Metrology_GetPQEnergy(PENERGY, ABS);
    
}

uint32_t DRV_METROLOGY_GetEnergyValue( bool restartEnergy )
{
    uint32_t energy = gDrvMetObj.metAFEData.energy;
    
    if (restartEnergy)
    {
        gDrvMetObj.metAFEData.energy = 0;
    }
            
    return energy;
}

uint32_t DRV_METROLOGY_GetRMSValue( MET_RMS_TYPE type )
{
    return gDrvMetObj.metAFEData.RMS[type];
}


#ifdef __cplusplus // Provide C++ Compatibility
 }
#endif
