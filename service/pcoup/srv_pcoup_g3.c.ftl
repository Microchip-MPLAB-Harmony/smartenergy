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

<#if (drvG3MacRt)??>
#include <string.h>
</#if>
#include "configuration.h"
#include "service/pcoup/srv_pcoup.h"

// *****************************************************************************
/* PLC PHY Tx Coupling Equalization Data

  Summary:
    Holds the Tx equalization coefficients tables.

  Description:
    Predistorsion applies specific gain for each carrier, which can be used to 
    compensate the hardware coupling filter frequency response, equalizing the 
    PLC transmission frequency response.

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

/* PLC PHY Tx Coupling Settings Data

  Summary:
    Holds the data required to set the PLC PHY Coupling parameters.

  Description:
    This structure holds the data required to set the PLC Tx Coupling PHY 
    parameters.

  Remarks:
    Values are defined in srv_pcoup.h file
 */

static const SRV_PLC_PCOUP srvPlcCoup = {
  SRV_PCOUP_RMS_HIGH_TBL, SRV_PCOUP_RMS_VLOW_TBL,
  SRV_PCOUP_THRS_HIGH_TBL, SRV_PCOUP_THRS_VLOW_TBL,
  SRV_PCOUP_DACC_TBL,
  srvPlcCoupPredistCoefHigh, srvPlcCoupPredistCoefVLow,
  SRV_PCOUP_GAIN_HIGH_TBL, SRV_PCOUP_GAIN_VLOW_TBL,
  SRV_PCOUP_NUM_TX_LEVELS, SRV_PCOUP_EQU_NUM_COEF << 1,
  SRV_PCOUP_LINE_DRV_CONF
};

<#if (SRV_PCOUP_G3_MAIN_BAND == "FCC" || SRV_PCOUP_G3_MAIN_BAND == "ARIB") && (SRV_PCOUP_G3_AUX_BAND != "None")>
static const SRV_PLC_PCOUP srvPlcCoupAux = {
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
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
SRV_PLC_PCOUP * SRV_PCOUP_Get_Config(SRV_PLC_PCOUP_BRANCH branch)
{
  if (branch == SRV_PLC_PCOUP_MAIN_BRANCH) 
  {
    /* PLC Tx Coupling PHY parameters for Main transmission branch */
    return (SRV_PLC_PCOUP *)&srvPlcCoup;
  }
<#if (SRV_PCOUP_G3_MAIN_BAND == "FCC" || SRV_PCOUP_G3_MAIN_BAND == "ARIB") && (SRV_PCOUP_G3_AUX_BAND != "None")>
  else if (branch == SRV_PLC_PCOUP_AUXILIARY_BRANCH)
  {
    /* PLC Tx Coupling PHY parameters for Auxiliary transmission branch */
    return (SRV_PLC_PCOUP *)&srvPlcCoupAux;
  }
</#if>

  /* Transmission branch not recognized */
  return NULL;
}

bool SRV_PCOUP_Set_Config(DRV_HANDLE handle, SRV_PLC_PCOUP_BRANCH branch)
{
  SRV_PLC_PCOUP *pCoupValues;
<#if (drvPlcPhy)??>
  DRV_PLC_PHY_PIB_OBJ pibObj;
  bool result;  
<#elseif (drvG3MacRt)??>
  MAC_RT_PIB_OBJ pibObj;
  MAC_RT_STATUS result;
</#if>

  /* Get PLC Tx Coupling PHY parameters for the desired transmission branch */
  pCoupValues = SRV_PCOUP_Get_Config(branch);

  if (pCoupValues == NULL)
  {
    /* Transmission branch not recognized */
    return false;
  }

  /* Set PLC Tx Coupling PHY parameters */
<#if (drvPlcPhy)??>  
  pibObj.id = PLC_ID_IC_DRIVER_CFG;
  pibObj.length = 1;
  pibObj.pData = &pCoupValues->lineDrvConf;
  result = DRV_PLC_PHY_PIBSet(handle, &pibObj);

  pibObj.id = PLC_ID_NUM_TX_LEVELS;
  pibObj.pData = &pCoupValues->numTxLevels;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

  pibObj.id = PLC_ID_DACC_TABLE_CFG;
  pibObj.length = sizeof(pCoupValues->daccTable);
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
  pibObj.length = pCoupValues->equSize;
  pibObj.pData = (uint8_t *)pCoupValues->equHigh;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

  pibObj.id = PLC_ID_PREDIST_COEF_TABLE_VLO;
  pibObj.pData = (uint8_t *)pCoupValues->equVlow;
  result &= DRV_PLC_PHY_PIBSet(handle, &pibObj);

  return result;
<#elseif (drvG3MacRt)??>
  pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
  pibObj.index = PHY_PIB_PLC_IC_DRIVER_CFG;
  pibObj.length = 1;
  pibObj.pData[0] = pCoupValues->lineDrvConf;
  result = DRV_G3_MACRT_PIBSet(handle, &pibObj);

  pibObj.index = PHY_PIB_NUM_TX_LEVELS;
  pibObj.pData[0] = pCoupValues->numTxLevels;
  result |= DRV_G3_MACRT_PIBSet(handle, &pibObj);

  pibObj.index = PHY_PIB_DACC_TABLE_CFG;
  pibObj.length = sizeof(pCoupValues->daccTable);
  memcpy(pibObj.pData, pCoupValues->daccTable, pibObj.length);
  result |= DRV_G3_MACRT_PIBSet(handle, &pibObj);  

  pibObj.index = PHY_PIB_MAX_RMS_TABLE_HI;
  pibObj.length = sizeof(pCoupValues->rmsHigh);
  memcpy(pibObj.pData, pCoupValues->rmsHigh, pibObj.length);
  result |= DRV_G3_MACRT_PIBSet(handle, &pibObj);

  pibObj.index = PHY_PIB_MAX_RMS_TABLE_VLO;
  memcpy(pibObj.pData, pCoupValues->rmsVLow, pibObj.length);
  result |= DRV_G3_MACRT_PIBSet(handle, &pibObj);

  pibObj.index = PHY_PIB_THRESHOLDS_TABLE_HI;
  pibObj.length = sizeof(pCoupValues->thrsHigh);
  memcpy(pibObj.pData, pCoupValues->thrsHigh, pibObj.length);
  result |= DRV_G3_MACRT_PIBSet(handle, &pibObj);

  pibObj.index = PHY_PIB_THRESHOLDS_TABLE_VLO;
  memcpy(pibObj.pData, pCoupValues->thrsVLow, pibObj.length);
  result |= DRV_G3_MACRT_PIBSet(handle, &pibObj);

  pibObj.index = PHY_PIB_GAIN_TABLE_HI;
  pibObj.length = sizeof(pCoupValues->gainHigh);
  memcpy(pibObj.pData, pCoupValues->gainHigh, pibObj.length);
  result |= DRV_G3_MACRT_PIBSet(handle, &pibObj);

  pibObj.index = PHY_PIB_GAIN_TABLE_VLO;
  memcpy(pibObj.pData, pCoupValues->gainVLow, pibObj.length);
  result |= DRV_G3_MACRT_PIBSet(handle, &pibObj);

  pibObj.index = PHY_PIB_PREDIST_COEF_TABLE_HI;
  pibObj.length = pCoupValues->equSize;
  memcpy(pibObj.pData, pCoupValues->equHigh, pibObj.length);
  result |= DRV_G3_MACRT_PIBSet(handle, &pibObj);

  pibObj.index = PHY_PIB_PREDIST_COEF_TABLE_VLO;
  memcpy(pibObj.pData, pCoupValues->equVlow, pibObj.length);
  result |= DRV_G3_MACRT_PIBSet(handle, &pibObj);

  return (bool)(result == MAC_RT_STATUS_SUCCESS);
</#if>
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
    return G3_CEN_A;
<#elseif (SRV_PCOUP_G3_MAIN_BAND == "CEN-B")>
    return G3_CEN_B;
<#elseif (SRV_PCOUP_G3_MAIN_BAND == "FCC")>
    return G3_FCC;
<#else>
    return G3_ARIB;
</#if>
  } 
  else 
  {
    /* PHY band for Auxiliary transmission branch */
<#if (SRV_PCOUP_G3_AUX_BAND != "None")>    
  <#if (SRV_PCOUP_G3_AUX_BAND == "CEN-A")>
    return G3_CEN_A;
  <#else>
    return G3_CEN_B;
  </#if>
<#else>
    return G3_INVALID;
</#if>
  }
}
