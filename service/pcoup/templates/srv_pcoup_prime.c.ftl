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
/*
Copyright (C) 2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
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
static const SRV_PLC_PCOUP_CHANNEL_DATA * const srvPlcCoupChnData[16] = {
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
// Section: PLC PHY Coupling Service Interface Implementation
// *****************************************************************************
// *****************************************************************************

DRV_PLC_PHY_CHANNEL SRV_PCOUP_GetDefaultChannel( void )
{
    return SRV_PCOUP_DEFAULT_CHANNEL;
}

SRV_PLC_PCOUP_CHANNEL_DATA * SRV_PCOUP_GetChannelConfig(DRV_PLC_PHY_CHANNEL channel)
{
    if ((channel >= CHN1) && (channel <= CHN7_CHN8))
    {
        /* MISRA C-2012 deviation block start */
        /* MISRA C-2012 Rule 11.8 deviated once. Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    <#if core.COMPILER_CHOICE == "XC32">
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    </#if>
        #pragma coverity compliance block deviate "MISRA C-2012 Rule 11.8" "H3_MISRAC_2012_R_11_8_DR_1"
</#if>

        return (SRV_PLC_PCOUP_CHANNEL_DATA *)srvPlcCoupChnData[channel];

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
        #pragma coverity compliance end_block "MISRA C-2012 Rule 11.8"
    <#if core.COMPILER_CHOICE == "XC32">
        #pragma GCC diagnostic pop
    </#if>
</#if>
        /* MISRA C-2012 deviation block end */
    }

    /* Channel not recognized */
    return NULL;
}

bool SRV_PCOUP_SetChannelConfig(DRV_HANDLE handle, DRV_PLC_PHY_CHANNEL channel)
{
    SRV_PLC_PCOUP_CHANNEL_DATA *pCoupValues;
    DRV_PLC_PHY_PIB_OBJ pibObj;
    bool result, resultOut;

    /* Get PLC PHY Coupling parameters for the desired transmission channel */
    pCoupValues = SRV_PCOUP_GetChannelConfig(channel);

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
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

    pibObj.id = PLC_ID_MAX_RMS_TABLE_HI;
    pibObj.length = (uint16_t)sizeof(pCoupValues->rmsHigh);
    pibObj.pData = (uint8_t *)pCoupValues->rmsHigh;
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

    pibObj.id = PLC_ID_MAX_RMS_TABLE_VLO;
    pibObj.pData = (uint8_t *)pCoupValues->rmsVLow;
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

    pibObj.id = PLC_ID_THRESHOLDS_TABLE_HI;
    pibObj.length = (uint16_t)sizeof(pCoupValues->thrsHigh);
    pibObj.pData = (uint8_t *)pCoupValues->thrsHigh;
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

    pibObj.id = PLC_ID_THRESHOLDS_TABLE_VLO;
    pibObj.pData = (uint8_t *)pCoupValues->thrsVLow;
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

    pibObj.id = PLC_ID_GAIN_TABLE_HI;
    pibObj.length = (uint16_t)sizeof(pCoupValues->gainHigh);
    pibObj.pData = (uint8_t *)pCoupValues->gainHigh;
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

    pibObj.id = PLC_ID_GAIN_TABLE_VLO;
    pibObj.pData = (uint8_t *)pCoupValues->gainVLow;
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

    /* MISRA C-2012 deviation block start */
<#if (SRV_PCOUP_PRIME_CHANNELS_SELECTED >= 256) >
    /* MISRA C-2012 Rule 11.8 deviated 5 times. Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */
<#else>
    /* MISRA C-2012 Rule 11.8 deviated 3 times. Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */
</#if>
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    </#if>
    #pragma coverity compliance block deviate "MISRA C-2012 Rule 11.8" "H3_MISRAC_2012_R_11_8_DR_1"
</#if>

    pibObj.id = PLC_ID_DACC_TABLE_CFG;
    pibObj.length = 17U << 2;
    pibObj.pData = (uint8_t *)pCoupValues->daccTable;
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

    pibObj.id = PLC_ID_PREDIST_COEF_TABLE_HI;
    pibObj.length = SRV_PCOUP_EQU_NUM_COEF_CHN << 1;
    pibObj.pData = (uint8_t *)pCoupValues->equHigh;
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

    pibObj.id = PLC_ID_PREDIST_COEF_TABLE_VLO;
    pibObj.pData = (uint8_t *)pCoupValues->equVlow;
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

<#if (SRV_PCOUP_PRIME_CHANNELS_SELECTED >= 256) >
    if (channel >= CHN1_CHN2)
    {
        pibObj.id = PLC_ID_PREDIST_COEF_TABLE_HI_2;
        pibObj.pData = (uint8_t *)(&pCoupValues->equHigh[SRV_PCOUP_EQU_NUM_COEF_CHN]);
        resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
        result = result && resultOut;

        pibObj.id = PLC_ID_PREDIST_COEF_TABLE_VLO_2;
        pibObj.pData = (uint8_t *)(&pCoupValues->equVlow[SRV_PCOUP_EQU_NUM_COEF_CHN]);
        resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
        result = result && resultOut;
    }

</#if>
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    #pragma coverity compliance end_block "MISRA C-2012 Rule 11.8"
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic pop
    </#if>
</#if>
    /* MISRA C-2012 deviation block end */

    return result;
}

uint16_t SRV_PCOUP_GetChannelList(void)
{
  return (uint16_t)SRV_PCOUP_CHANNEL_LIST;
}

DRV_PLC_PHY_CHANNEL SRV_PCOUP_GetChannelImpedanceDetection(void)
{
  DRV_PLC_PHY_CHANNEL channel = (DRV_PLC_PHY_CHANNEL)SRV_PCOUP_CHANNEL_IMP_DET;

  return channel;
}
