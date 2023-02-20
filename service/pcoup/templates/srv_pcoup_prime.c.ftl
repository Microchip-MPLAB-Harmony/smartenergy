/*******************************************************************************
  PLC Phy Coupling Service Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    srv_pcoup.c

  Summary:
    PLC PHY Coupling service implementation.

  Description:
    This file contains the source code for the implementation of the
    PLC PHY Cpupling service. It helps to configure the PLC PHY Coupling 
    parameters through PLC Driver PIB interface.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "service/pcoup/srv_pcoup.h"

// *****************************************************************************
/* PLC PHY Coupling equalization data

  Summary:
    Holds the Tx equalization coefficients tables.

  Description:
    Pre-distorsion applies specific gain factor for each carrier, compensating 
    the frequency response of the external analog filter, and equalizing the 
    the transmitted signal.

  Remarks:
    Values are defined in srv_pcoup.h file. Different values for HIGH and VLOW 
    modes
 */

<#if (SRV_PCOUP_PRIME_CHN1 == true)>
static const uint16_t srvPlcCoupPredistCoefChn1High[SRV_PCOUP_EQU_NUM_COEF_CHN] = SRV_PCOUP_PRED_CHN1_HIGH_TBL;
static const uint16_t srvPlcCoupPredistCoefChn1Low[SRV_PCOUP_EQU_NUM_COEF_CHN] = SRV_PCOUP_PRED_CHN1_VLOW_TBL;

</#if>
<#if ((SRV_PCOUP_PRIME_CHN2 == true) || (SRV_PCOUP_PRIME_CHN3 == true) || (SRV_PCOUP_PRIME_CHN4 == true) || (SRV_PCOUP_PRIME_CHN5 == true) || (SRV_PCOUP_PRIME_CHN6 == true) || (SRV_PCOUP_PRIME_CHN7 == true) || (SRV_PCOUP_PRIME_CHN8 == true))>
static const uint16_t srvPlcCoupPredistCoefDummy[SRV_PCOUP_EQU_NUM_COEF_CHN] = SRV_PCOUP_PRED_NOT_USED;

</#if>
<#if (SRV_PCOUP_PRIME_CHANNELS_SELECTED >= 256) >
static const uint16_t srvPlcCoupPredist2ChnCoefDummy[SRV_PCOUP_EQU_NUM_COEF_2_CHN] = SRV_PCOUP_PRED_2CHN_NOT_USED;

</#if>
/* Configuration values of internal DACC peripheral */
<#if (SRV_PCOUP_PRIME_CHN1 == true)>
static const uint32_t srvPlcCoupDaccTableCenA[17] = SRV_PCOUP_DACC_CENA_TBL;

</#if>
<#if ((SRV_PCOUP_PRIME_CHN2 == true) || (SRV_PCOUP_PRIME_CHN3 == true) || (SRV_PCOUP_PRIME_CHN4 == true) || (SRV_PCOUP_PRIME_CHN5 == true) || (SRV_PCOUP_PRIME_CHN6 == true) || (SRV_PCOUP_PRIME_CHN7 == true) || (SRV_PCOUP_PRIME_CHN8 == true))>
static const uint32_t srvPlcCoupDaccTableFcc[17] = SRV_PCOUP_DACC_FCC_TBL;

</#if>
<#if (SRV_PCOUP_PRIME_CHANNELS_SELECTED >= 256) >
static const uint32_t srvPlcCoupDaccTable2Chn[17] = SRV_PCOUP_DACC_2CHN_TBL;

</#if>

/* PLC PHY Coupling data

  Summary:
    PLC PHY Coupling data.

  Description:
    This structure contains all the data required to set the PLC PHY Coupling 
    parameters, for each PRIME channel.

  Remarks:
    Values are defined in srv_pcoup.h file
 */

<#if (SRV_PCOUP_PRIME_CHN1 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn1Data = {
  SRV_PCOUP_CHN1_RMS_HIGH_TBL, SRV_PCOUP_CHN1_RMS_VLOW_TBL,
  SRV_PCOUP_CHN1_THRS_HIGH_TBL, SRV_PCOUP_CHN1_THRS_VLOW_TBL,
  srvPlcCoupDaccTableCenA,
  srvPlcCoupPredistCoefChn1High, srvPlcCoupPredistCoefChn1Low,
  SRV_PCOUP_CHN1_GAIN_HIGH_TBL, SRV_PCOUP_CHN1_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN1_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN1_LINE_DRV_CONF
  
};

</#if>
<#if (SRV_PCOUP_PRIME_CHN2 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn2Data = {
  SRV_PCOUP_CHN2_RMS_HIGH_TBL, SRV_PCOUP_CHN2_RMS_VLOW_TBL,
  SRV_PCOUP_CHN2_THRS_HIGH_TBL, SRV_PCOUP_CHN2_THRS_VLOW_TBL,
  srvPlcCoupDaccTableFcc,
  srvPlcCoupPredistCoefDummy, srvPlcCoupPredistCoefDummy,
  SRV_PCOUP_CHN2_GAIN_HIGH_TBL, SRV_PCOUP_CHN2_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN2_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN2_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_CHN3 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn3Data = {
  SRV_PCOUP_CHN3_RMS_HIGH_TBL, SRV_PCOUP_CHN3_RMS_VLOW_TBL,
  SRV_PCOUP_CHN3_THRS_HIGH_TBL, SRV_PCOUP_CHN3_THRS_VLOW_TBL,
  srvPlcCoupDaccTableFcc,
  srvPlcCoupPredistCoefDummy, srvPlcCoupPredistCoefDummy,
  SRV_PCOUP_CHN3_GAIN_HIGH_TBL, SRV_PCOUP_CHN3_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN3_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN3_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_CHN4 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn4Data = {
  SRV_PCOUP_CHN4_RMS_HIGH_TBL, SRV_PCOUP_CHN4_RMS_VLOW_TBL,
  SRV_PCOUP_CHN4_THRS_HIGH_TBL, SRV_PCOUP_CHN4_THRS_VLOW_TBL,
  srvPlcCoupDaccTableFcc,
  srvPlcCoupPredistCoefDummy, srvPlcCoupPredistCoefDummy,
  SRV_PCOUP_CHN4_GAIN_HIGH_TBL, SRV_PCOUP_CHN4_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN4_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN4_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_CHN5 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn5Data = {
  SRV_PCOUP_CHN5_RMS_HIGH_TBL, SRV_PCOUP_CHN5_RMS_VLOW_TBL,
  SRV_PCOUP_CHN5_THRS_HIGH_TBL, SRV_PCOUP_CHN5_THRS_VLOW_TBL,
  srvPlcCoupDaccTableFcc,
  srvPlcCoupPredistCoefDummy, srvPlcCoupPredistCoefDummy,
  SRV_PCOUP_CHN5_GAIN_HIGH_TBL, SRV_PCOUP_CHN5_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN5_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN5_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_CHN6 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn6Data = {
  SRV_PCOUP_CHN6_RMS_HIGH_TBL, SRV_PCOUP_CHN6_RMS_VLOW_TBL,
  SRV_PCOUP_CHN6_THRS_HIGH_TBL, SRV_PCOUP_CHN6_THRS_VLOW_TBL,
  srvPlcCoupDaccTableFcc,
  srvPlcCoupPredistCoefDummy, srvPlcCoupPredistCoefDummy,
  SRV_PCOUP_CHN6_GAIN_HIGH_TBL, SRV_PCOUP_CHN6_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN6_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN6_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_CHN7 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn7Data = {
  SRV_PCOUP_CHN7_RMS_HIGH_TBL, SRV_PCOUP_CHN7_RMS_VLOW_TBL,
  SRV_PCOUP_CHN7_THRS_HIGH_TBL, SRV_PCOUP_CHN7_THRS_VLOW_TBL,
  srvPlcCoupDaccTableFcc,
  srvPlcCoupPredistCoefDummy, srvPlcCoupPredistCoefDummy,
  SRV_PCOUP_CHN7_GAIN_HIGH_TBL, SRV_PCOUP_CHN7_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN7_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN7_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_CHN8 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn8Data = {
  SRV_PCOUP_CHN8_RMS_HIGH_TBL, SRV_PCOUP_CHN8_RMS_VLOW_TBL,
  SRV_PCOUP_CHN8_THRS_HIGH_TBL, SRV_PCOUP_CHN8_THRS_VLOW_TBL,
  srvPlcCoupDaccTableFcc,
  srvPlcCoupPredistCoefDummy, srvPlcCoupPredistCoefDummy,
  SRV_PCOUP_CHN8_GAIN_HIGH_TBL, SRV_PCOUP_CHN8_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN8_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN8_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_2CHN12 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn12Data = {
  SRV_PCOUP_CHN12_RMS_HIGH_TBL, SRV_PCOUP_CHN12_RMS_VLOW_TBL,
  SRV_PCOUP_CHN12_THRS_HIGH_TBL, SRV_PCOUP_CHN12_THRS_VLOW_TBL,
  srvPlcCoupDaccTable2Chn,
  srvPlcCoupPredist2ChnCoefDummy, srvPlcCoupPredist2ChnCoefDummy,
  SRV_PCOUP_CHN12_GAIN_HIGH_TBL, SRV_PCOUP_CHN12_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN12_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN12_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_2CHN23 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn23Data = {
  SRV_PCOUP_CHN23_RMS_HIGH_TBL, SRV_PCOUP_CHN23_RMS_VLOW_TBL,
  SRV_PCOUP_CHN23_THRS_HIGH_TBL, SRV_PCOUP_CHN23_THRS_VLOW_TBL,
  srvPlcCoupDaccTable2Chn,
  srvPlcCoupPredist2ChnCoefDummy, srvPlcCoupPredist2ChnCoefDummy,
  SRV_PCOUP_CHN23_GAIN_HIGH_TBL, SRV_PCOUP_CHN23_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN23_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN23_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_2CHN34 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn34Data = {
  SRV_PCOUP_CHN34_RMS_HIGH_TBL, SRV_PCOUP_CHN34_RMS_VLOW_TBL,
  SRV_PCOUP_CHN34_THRS_HIGH_TBL, SRV_PCOUP_CHN34_THRS_VLOW_TBL,
  srvPlcCoupDaccTable2Chn,
  srvPlcCoupPredist2ChnCoefDummy, srvPlcCoupPredist2ChnCoefDummy,
  SRV_PCOUP_CHN34_GAIN_HIGH_TBL, SRV_PCOUP_CHN34_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN34_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN34_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_2CHN45 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn45Data = {
  SRV_PCOUP_CHN45_RMS_HIGH_TBL, SRV_PCOUP_CHN45_RMS_VLOW_TBL,
  SRV_PCOUP_CHN45_THRS_HIGH_TBL, SRV_PCOUP_CHN45_THRS_VLOW_TBL,
  srvPlcCoupDaccTable2Chn,
  srvPlcCoupPredist2ChnCoefDummy, srvPlcCoupPredist2ChnCoefDummy,
  SRV_PCOUP_CHN45_GAIN_HIGH_TBL, SRV_PCOUP_CHN45_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN45_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN45_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_2CHN56 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn56Data = {
  SRV_PCOUP_CHN56_RMS_HIGH_TBL, SRV_PCOUP_CHN56_RMS_VLOW_TBL,
  SRV_PCOUP_CHN56_THRS_HIGH_TBL, SRV_PCOUP_CHN56_THRS_VLOW_TBL,
  srvPlcCoupDaccTable2Chn,
  srvPlcCoupPredist2ChnCoefDummy, srvPlcCoupPredist2ChnCoefDummy,
  SRV_PCOUP_CHN56_GAIN_HIGH_TBL, SRV_PCOUP_CHN56_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN56_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN56_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_2CHN67 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn67Data = {
  SRV_PCOUP_CHN67_RMS_HIGH_TBL, SRV_PCOUP_CHN67_RMS_VLOW_TBL,
  SRV_PCOUP_CHN67_THRS_HIGH_TBL, SRV_PCOUP_CHN67_THRS_VLOW_TBL,
  srvPlcCoupDaccTable2Chn,
  srvPlcCoupPredist2ChnCoefDummy, srvPlcCoupPredist2ChnCoefDummy,
  SRV_PCOUP_CHN67_GAIN_HIGH_TBL, SRV_PCOUP_CHN67_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN67_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN67_LINE_DRV_CONF
};

</#if>
<#if (SRV_PCOUP_PRIME_2CHN78 == true)>
static const SRV_PLC_PCOUP_CHANNEL_DATA srvPlcCoupChn78Data = {
  SRV_PCOUP_CHN78_RMS_HIGH_TBL, SRV_PCOUP_CHN78_RMS_VLOW_TBL,
  SRV_PCOUP_CHN78_THRS_HIGH_TBL, SRV_PCOUP_CHN78_THRS_VLOW_TBL,
  srvPlcCoupDaccTable2Chn,
  srvPlcCoupPredist2ChnCoefDummy, srvPlcCoupPredist2ChnCoefDummy,
  SRV_PCOUP_CHN78_GAIN_HIGH_TBL, SRV_PCOUP_CHN78_GAIN_VLOW_TBL,
  SRV_PCOUP_CHN78_MAX_NUM_TX_LEVELS, SRV_PCOUP_CHN78_LINE_DRV_CONF
};

</#if>
static const SRV_PLC_PCOUP_CHANNEL_DATA * srvPlcCoupChnData[16] = {
    NULL,
<#if (SRV_PCOUP_PRIME_CHN1 == true)>
    &srvPlcCoupChn1Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_CHN2 == true)>
    &srvPlcCoupChn2Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_CHN3 == true)>
    &srvPlcCoupChn3Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_CHN4 == true)>
    &srvPlcCoupChn4Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_CHN5 == true)>
    &srvPlcCoupChn5Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_CHN6 == true)>
    &srvPlcCoupChn6Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_CHN7 == true)>
    &srvPlcCoupChn7Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_CHN8 == true)>
    &srvPlcCoupChn8Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_2CHN12 == true)>
    &srvPlcCoupChn12Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_2CHN23 == true)>
    &srvPlcCoupChn23Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_2CHN34 == true)>
    &srvPlcCoupChn34Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_2CHN45 == true)>
    &srvPlcCoupChn45Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_2CHN56 == true)>
    &srvPlcCoupChn56Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_2CHN67 == true)>
    &srvPlcCoupChn67Data,
<#else>
    NULL,
</#if>
<#if (SRV_PCOUP_PRIME_2CHN78 == true)>
    &srvPlcCoupChn78Data
<#else>
    NULL
</#if>
};

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

DRV_PLC_PHY_CHANNEL SRV_PCOUP_Get_Default_Channel( void )
{
  return SRV_PCOUP_DEFAULT_CHANNEL;
}

SRV_PLC_PCOUP_CHANNEL_DATA * SRV_PCOUP_Get_Channel_Config(DRV_PLC_PHY_CHANNEL channel)
{
    if ((channel >= CHN1) && (channel <= CHN7_CHN8))
    {
        return (SRV_PLC_PCOUP_CHANNEL_DATA *)srvPlcCoupChnData[channel];
    }
    else
    {
        return NULL;
    }
}

bool SRV_PCOUP_Set_Channel_Config(DRV_HANDLE handle, DRV_PLC_PHY_CHANNEL channel)
{
  SRV_PLC_PCOUP_CHANNEL_DATA *pCoupValues;
  DRV_PLC_PHY_PIB_OBJ pibObj;
  bool result;  

  /* Get PLC PHY Coupling parameters for the desired transmission channel */
  pCoupValues = SRV_PCOUP_Get_Channel_Config(channel);

  if (pCoupValues == NULL)
  {
    /* Transmission channel not recognized */
    return false;
  }

  /* Set PLC PHY Coupling parameters */
  pibObj.id = PLC_ID_IC_DRIVER_CFG;
  pibObj.length = 1;
  pibObj.pData = &pCoupValues->lineDrvConf;
  result = DRV_PLC_PHY_PIBSet(handle, &pibObj);

  pibObj.id = PLC_ID_NUM_TX_LEVELS;
  pibObj.pData = &pCoupValues->numTxLevels;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

  pibObj.id = PLC_ID_DACC_TABLE_CFG;
  pibObj.length = 17 << 2;
  pibObj.pData = (uint8_t *)pCoupValues->daccTable;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);  

  pibObj.id = PLC_ID_MAX_RMS_TABLE_HI;
  pibObj.length = sizeof(pCoupValues->rmsHigh);
  pibObj.pData = (uint8_t *)pCoupValues->rmsHigh;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

  pibObj.id = PLC_ID_MAX_RMS_TABLE_VLO;
  pibObj.pData = (uint8_t *)pCoupValues->rmsVLow;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

  pibObj.id = PLC_ID_THRESHOLDS_TABLE_HI;
  pibObj.length = sizeof(pCoupValues->thrsHigh);
  pibObj.pData = (uint8_t *)pCoupValues->thrsHigh;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

  pibObj.id = PLC_ID_THRESHOLDS_TABLE_VLO;
  pibObj.pData = (uint8_t *)pCoupValues->thrsVLow;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

  pibObj.id = PLC_ID_GAIN_TABLE_HI;
  pibObj.length = sizeof(pCoupValues->gainHigh);
  pibObj.pData = (uint8_t *)pCoupValues->gainHigh;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

  pibObj.id = PLC_ID_GAIN_TABLE_VLO;
  pibObj.pData = (uint8_t *)pCoupValues->gainVLow;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

  pibObj.id = PLC_ID_PREDIST_COEF_TABLE_HI;
  pibObj.length = SRV_PCOUP_EQU_NUM_COEF_CHN << 1;
  pibObj.pData = (uint8_t *)pCoupValues->equHigh;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

  pibObj.id = PLC_ID_PREDIST_COEF_TABLE_VLO;
  pibObj.pData = (uint8_t *)pCoupValues->equVlow;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

<#if (SRV_PCOUP_PRIME_CHANNELS_SELECTED >= 256) >
  if (channel >= CHN1_CHN2)
  {
    pibObj.id = PLC_ID_PREDIST_COEF_TABLE_HI_2;
    pibObj.pData = (uint8_t *)(&pCoupValues->equHigh[SRV_PCOUP_EQU_NUM_COEF_CHN]);
    result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

    pibObj.id = PLC_ID_PREDIST_COEF_TABLE_VLO_2;
    pibObj.pData = (uint8_t *)(&pCoupValues->equVlow[SRV_PCOUP_EQU_NUM_COEF_CHN]);
    result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);
  }

</#if>
  return result;
}
