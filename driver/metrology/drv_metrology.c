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
#include "system/int/sys_int.h"
#include "drv_metrology.h"
#include "drv_metrology_definitions.h"

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

void IPC1_handler (void)
{
    uint32_t status = IPC1_REGS->IPC_ISR & IPC1_REGS->IPC_IMR;
    
    if (status & DRV_METROLOGY_IPC_INIT_IRQ_MSK)
    {
        gDrvMetObj.state = DRV_METROLOGY_STATE_INIT_DSP;
    }
    
    if (status & DRV_METROLOGY_IPC_INTEGRATION_IRQ_MSK)
    {
        gDrvMetObj.newMetData = true;
        
        if (gDrvMetObj.newIntegrationCallback)
        {
            gDrvMetObj.newIntegrationCallback();
        }
    }
    
    IPC1_REGS->IPC_ICCR = status;
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
    uint32_t *pul_dst = (uint32_t *)IRAM1_ADDR;
    uint32_t *pul_src = (uint32_t *)gDrvMetObj.binStartAddress;

    if(gDrvMetObj.inUse == true)
    {
        return SYS_MODULE_OBJ_INVALID;
    }
    
    if (resetCause != RSTC_SR_RSTTYP(RSTC_SR_RSTTYP_WDT0_RST_Val))
    {
        uint32_t tmp;
        
        /* Keep Coprocessor (Core 1) on reset */
        tmp = RSTC_REGS->RSTC_MR & ~RSTC_MR_CPROCEN_Msk;
        RSTC_REGS->RSTC_MR = RSTC_MR_KEY_PASSWD | tmp;

        gDrvMetObj.status = SYS_STATUS_UNINITIALIZED;

        gDrvMetObj.binStartAddress = metInit->binStartAddress;
        gDrvMetObj.binSize = metInit->binEndAddress - metInit->binStartAddress;

        /* Copy the Metrology bin file to SRAM1 */
        for (tmp = 0; tmp < gDrvMetObj.binSize; tmp += 4, pul_src++, pul_dst++) {
            *pul_dst = *pul_src;
        }
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
    
    if (gDrvMetObj.state == DRV_METROLOGY_STATE_HALT)
    {
        return DRV_METROLOGY_ERROR;
    }
    
    /* Enable IPC1 Interrupt Source */
    SYS_INT_SourceEnable(IPC1_IRQn);
    
    if (mode == DRV_METROLOGY_START_HARD)
    {
        /* Enable Coprocessor (Core 1) */
        uint32_t reg = RSTC_REGS->RSTC_MR;
        RSTC_REGS->RSTC_MR = RSTC_MR_KEY_PASSWD | reg | RSTC_MR_CPROCEN_Msk;

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
    if (gDrvMetObj.metRegisters->MET_STATUS.STATUS != STATUS_STATUS_READY)
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


#ifdef __cplusplus // Provide C++ Compatibility
 }
#endif
