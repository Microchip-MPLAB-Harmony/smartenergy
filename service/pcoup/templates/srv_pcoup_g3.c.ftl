/*******************************************************************************
  PLC PHY Coupling Service Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    srv_pcoup.c

  Summary:
    PLC PHY Coupling service implementation.

  Description:
    This file contains the source code for the implementation of the
    PLC PHY Coupling service. It helps to configure the PLC PHY Coupling
    parameters through PLC Driver PIB interface.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2024, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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

<#if (drvG3MacRt)??>
#include <string.h>
</#if>
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

static const uint16_t srvPlcCoupPredistCoefHigh[SRV_PCOUP_EQU_NUM_COEF] = SRV_PCOUP_PRED_HIGH_TBL;
static const uint16_t srvPlcCoupPredistCoefVLow[SRV_PCOUP_EQU_NUM_COEF] = SRV_PCOUP_PRED_VLOW_TBL;

<#if (SRV_PCOUP_G3_MAIN_BAND == "FCC" || SRV_PCOUP_G3_MAIN_BAND == "ARIB") && (SRV_PCOUP_G3_AUX_BAND != "None")>
static const uint16_t srvPlcCoupAuxPredistCoefHigh[SRV_PCOUP_AUX_EQU_NUM_COEF] = SRV_PCOUP_AUX_PRED_HIGH_TBL;
static const uint16_t srvPlcCoupAuxPredistCoefVLow[SRV_PCOUP_AUX_EQU_NUM_COEF] = SRV_PCOUP_AUX_PRED_VLOW_TBL;
</#if>

/* PLC PHY Coupling data

  Summary:
    PLC PHY Coupling data.

  Description:
    This structure(s) contains all the data required to set the PLC PHY
    Coupling parameters, for each transmission branch.

  Remarks:
    Values are defined in srv_pcoup.h file
 */

static const SRV_PLC_PCOUP_DATA srvPlcCoup = {
  SRV_PCOUP_RMS_HIGH_TBL, SRV_PCOUP_RMS_VLOW_TBL,
  SRV_PCOUP_THRS_HIGH_TBL, SRV_PCOUP_THRS_VLOW_TBL,
  SRV_PCOUP_DACC_TBL,
  srvPlcCoupPredistCoefHigh, srvPlcCoupPredistCoefVLow,
  SRV_PCOUP_GAIN_HIGH_TBL, SRV_PCOUP_GAIN_VLOW_TBL,
  SRV_PCOUP_NUM_TX_LEVELS, SRV_PCOUP_EQU_NUM_COEF << 1,
  SRV_PCOUP_LINE_DRV_CONF
};

<#if (SRV_PCOUP_G3_MAIN_BAND == "FCC" || SRV_PCOUP_G3_MAIN_BAND == "ARIB") && (SRV_PCOUP_G3_AUX_BAND != "None")>
static const SRV_PLC_PCOUP_DATA srvPlcCoupAux = {
  SRV_PCOUP_AUX_RMS_HIGH_TBL, SRV_PCOUP_AUX_RMS_VLOW_TBL,
  SRV_PCOUP_AUX_THRS_HIGH_TBL, SRV_PCOUP_AUX_THRS_VLOW_TBL,
  SRV_PCOUP_AUX_DACC_TBL,
  srvPlcCoupAuxPredistCoefHigh, srvPlcCoupAuxPredistCoefVLow,
  SRV_PCOUP_AUX_GAIN_HIGH_TBL, SRV_PCOUP_AUX_GAIN_VLOW_TBL,
  SRV_PCOUP_AUX_NUM_TX_LEVELS, SRV_PCOUP_AUX_EQU_NUM_COEF << 1,
  SRV_PCOUP_AUX_LINE_DRV_CONF
};
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: PLC PHY Coupling Service Interface Implementation
// *****************************************************************************
// *****************************************************************************

SRV_PLC_PCOUP_DATA * SRV_PCOUP_Get_Config(SRV_PLC_PCOUP_BRANCH branch)
{
    /* MISRA C-2012 deviation block start */
<#if (SRV_PCOUP_G3_MAIN_BAND == "FCC" || SRV_PCOUP_G3_MAIN_BAND == "ARIB") && (SRV_PCOUP_G3_AUX_BAND != "None")>
    /* MISRA C-2012 Rule 11.8 deviated twice. Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */
<#else>
    /* MISRA C-2012 Rule 11.8 deviated once. Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */
</#if>
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    </#if>
    #pragma coverity compliance block deviate "MISRA C-2012 Rule 11.8" "H3_MISRAC_2012_R_11_8_DR_1"
</#if>

    if (branch == SRV_PLC_PCOUP_MAIN_BRANCH)
    {
        /* PLC PHY Coupling parameters for Main transmission branch */
        return (SRV_PLC_PCOUP_DATA *)&srvPlcCoup;
    }
<#if (SRV_PCOUP_G3_MAIN_BAND == "FCC" || SRV_PCOUP_G3_MAIN_BAND == "ARIB") && (SRV_PCOUP_G3_AUX_BAND != "None")>

    if (branch == SRV_PLC_PCOUP_AUXILIARY_BRANCH)
    {
        /* PLC PHY Coupling parameters for Auxiliary transmission branch */
        return (SRV_PLC_PCOUP_DATA *)&srvPlcCoupAux;
    }
</#if>

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    #pragma coverity compliance end_block "MISRA C-2012 Rule 11.8"
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic pop
    </#if>
</#if>
    /* MISRA C-2012 deviation block end */

    /* Transmission branch not recognized */
    return NULL;
}

bool SRV_PCOUP_Set_Config(DRV_HANDLE handle, SRV_PLC_PCOUP_BRANCH branch)
{
    SRV_PLC_PCOUP_DATA *pCoupValues;
    bool result, resultOut;
<#if (drvPlcPhy)??>
    DRV_PLC_PHY_PIB_OBJ pibObj;
<#elseif (drvG3MacRt)??>
    MAC_RT_PIB_OBJ pibObj;
</#if>

    /* Get PLC PHY Coupling parameters for the desired transmission branch */
    pCoupValues = SRV_PCOUP_Get_Config(branch);

    if (pCoupValues == NULL)
    {
        /* Transmission branch not recognized */
        return false;
    }

    /* Set PLC PHY Coupling parameters */
<#if (drvPlcPhy)??>
    pibObj.id = PLC_ID_IC_DRIVER_CFG;
    pibObj.length = 1;
    pibObj.pData = &pCoupValues->lineDrvConf;
    result = DRV_PLC_PHY_PIBSet(handle, &pibObj);

    pibObj.id = PLC_ID_NUM_TX_LEVELS;
    pibObj.pData = &pCoupValues->numTxLevels;
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

    pibObj.id = PLC_ID_DACC_TABLE_CFG;
    pibObj.length = (uint16_t)sizeof(pCoupValues->daccTable);
    pibObj.pData = (uint8_t *)pCoupValues->daccTable;
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
    /* MISRA C-2012 Rule 11.8 deviated twice. Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    </#if>
    #pragma coverity compliance block deviate "MISRA C-2012 Rule 11.8" "H3_MISRAC_2012_R_11_8_DR_1"
</#if>

    pibObj.id = PLC_ID_PREDIST_COEF_TABLE_HI;
    pibObj.length = pCoupValues->equSize;
    pibObj.pData = (uint8_t *)pCoupValues->equHigh;
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

    pibObj.id = PLC_ID_PREDIST_COEF_TABLE_VLO;
    pibObj.pData = (uint8_t *)pCoupValues->equVlow;
    resultOut = DRV_PLC_PHY_PIBSet(handle, &pibObj);
    result = result && resultOut;

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    #pragma coverity compliance end_block "MISRA C-2012 Rule 11.8"
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic pop
    </#if>
</#if>
    /* MISRA C-2012 deviation block end */
<#elseif (drvG3MacRt)??>
    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = (uint16_t)PHY_PIB_PLC_IC_DRIVER_CFG;
    pibObj.length = 1;
    pibObj.pData[0] = pCoupValues->lineDrvConf;
    result = (DRV_G3_MACRT_PIBSet(handle, &pibObj) == MAC_RT_STATUS_SUCCESS);

    pibObj.index = (uint16_t)PHY_PIB_NUM_TX_LEVELS;
    pibObj.pData[0] = pCoupValues->numTxLevels;
    resultOut = (DRV_G3_MACRT_PIBSet(handle, &pibObj) == MAC_RT_STATUS_SUCCESS);
    result = result && resultOut;

    pibObj.index = (uint16_t)PHY_PIB_DACC_TABLE_CFG;
    pibObj.length = (uint8_t)sizeof(pCoupValues->daccTable);
    (void) memcpy(pibObj.pData, (uint8_t *)pCoupValues->daccTable, pibObj.length);
    resultOut = (DRV_G3_MACRT_PIBSet(handle, &pibObj) == MAC_RT_STATUS_SUCCESS);
    result = result && resultOut;

    pibObj.index = (uint16_t)PHY_PIB_MAX_RMS_TABLE_HI;
    pibObj.length = (uint8_t)sizeof(pCoupValues->rmsHigh);
    (void) memcpy(pibObj.pData, (uint8_t *)pCoupValues->rmsHigh, pibObj.length);
    resultOut = (DRV_G3_MACRT_PIBSet(handle, &pibObj) == MAC_RT_STATUS_SUCCESS);
    result = result && resultOut;

    pibObj.index = (uint16_t)PHY_PIB_MAX_RMS_TABLE_VLO;
    (void) memcpy(pibObj.pData, (uint8_t *)pCoupValues->rmsVLow, pibObj.length);
    resultOut = (DRV_G3_MACRT_PIBSet(handle, &pibObj) == MAC_RT_STATUS_SUCCESS);
    result = result && resultOut;

    pibObj.index = (uint16_t)PHY_PIB_THRESHOLDS_TABLE_HI;
    pibObj.length = (uint8_t)sizeof(pCoupValues->thrsHigh);
    (void) memcpy(pibObj.pData, (uint8_t *)pCoupValues->thrsHigh, pibObj.length);
    resultOut = (DRV_G3_MACRT_PIBSet(handle, &pibObj) == MAC_RT_STATUS_SUCCESS);
    result = result && resultOut;

    pibObj.index = (uint16_t)PHY_PIB_THRESHOLDS_TABLE_VLO;
    (void) memcpy(pibObj.pData, (uint8_t *)pCoupValues->thrsVLow, pibObj.length);
    resultOut = (DRV_G3_MACRT_PIBSet(handle, &pibObj) == MAC_RT_STATUS_SUCCESS);
    result = result && resultOut;

    pibObj.index = (uint16_t)PHY_PIB_GAIN_TABLE_HI;
    pibObj.length = (uint8_t)sizeof(pCoupValues->gainHigh);
    (void) memcpy(pibObj.pData, (uint8_t *)pCoupValues->gainHigh, pibObj.length);
    resultOut = (DRV_G3_MACRT_PIBSet(handle, &pibObj) == MAC_RT_STATUS_SUCCESS);
    result = result && resultOut;

    pibObj.index = (uint16_t)PHY_PIB_GAIN_TABLE_VLO;
    (void) memcpy(pibObj.pData, (uint8_t *)pCoupValues->gainVLow, pibObj.length);
    resultOut = (DRV_G3_MACRT_PIBSet(handle, &pibObj) == MAC_RT_STATUS_SUCCESS);
    result = result && resultOut;

    /* MISRA C-2012 deviation block start */
    /* MISRA C-2012 Rule 11.8 deviated twice. Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    </#if>
    #pragma coverity compliance block deviate "MISRA C-2012 Rule 11.8" "H3_MISRAC_2012_R_11_8_DR_1"
</#if>

    pibObj.index = (uint16_t)PHY_PIB_PREDIST_COEF_TABLE_HI;
    pibObj.length = pCoupValues->equSize;
    (void) memcpy(pibObj.pData, (uint8_t *)pCoupValues->equHigh, pibObj.length);
    resultOut = (DRV_G3_MACRT_PIBSet(handle, &pibObj) == MAC_RT_STATUS_SUCCESS);
    result = result && resultOut;

    pibObj.index = (uint16_t)PHY_PIB_PREDIST_COEF_TABLE_VLO;
    (void) memcpy(pibObj.pData, (uint8_t *)pCoupValues->equVlow, pibObj.length);
    resultOut = (DRV_G3_MACRT_PIBSet(handle, &pibObj) == MAC_RT_STATUS_SUCCESS);
    result = result && resultOut;

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    #pragma coverity compliance end_block "MISRA C-2012 Rule 11.8"
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic pop
    </#if>
</#if>
    /* MISRA C-2012 deviation block end */
</#if>

    return result;
}

SRV_PLC_PCOUP_BRANCH SRV_PCOUP_Get_Default_Branch( void )
{
    return SRV_PCOUP_DEFAULT_BRANCH;
}

uint8_t SRV_PCOUP_Get_Phy_Band(SRV_PLC_PCOUP_BRANCH branch)
{
    if (branch == SRV_PLC_PCOUP_MAIN_BRANCH)
    {
        /* PHY band for Main transmission branch */
<#if (SRV_PCOUP_G3_MAIN_BAND == "CEN-A")>
        return (uint8_t)G3_CEN_A;
<#elseif (SRV_PCOUP_G3_MAIN_BAND == "CEN-B")>
       return (uint8_t)G3_CEN_B;
<#elseif (SRV_PCOUP_G3_MAIN_BAND == "FCC")>
       return (uint8_t)G3_FCC;
<#else>
       return (uint8_t)G3_ARIB;
</#if>
   }
<#if (SRV_PCOUP_G3_MAIN_BAND == "FCC" || SRV_PCOUP_G3_MAIN_BAND == "ARIB") && (SRV_PCOUP_G3_AUX_BAND != "None")>

    if (branch == SRV_PLC_PCOUP_AUXILIARY_BRANCH)
    {
        /* PHY band for Main Auxiliary branch */
  <#if (SRV_PCOUP_G3_AUX_BAND == "CEN-A")>
        return (uint8_t)G3_CEN_A;
  <#else>
        return (uint8_t)G3_CEN_B;
  </#if>
    }
</#if>

    /* Transmission branch not recognized */
    return (uint8_t)G3_INVALID;
}
