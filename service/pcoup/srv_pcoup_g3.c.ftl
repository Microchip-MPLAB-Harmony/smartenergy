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
    PLC PHY Cpupling service.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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
#include "driver/driver_common.h"
#include "service/pcoup/srv_pcoup.h"
<#if (drvPlcPhy)??>
#include "driver/plc/phy/drv_plc_phy_comm.h"
<#elseif (drvG3MacRt)??>
#include "driver/plc/g3MacRt/drv_g3_macrt_comm.h"
</#if>

// *****************************************************************************
/* PLC Coupling configuration data

  Summary:
    Holds PLC configuration data

  Description:
    This structure holds the PLC coupling configuration data.

  Remarks:
    Parameters are defined in srv_pcoup.h file
 */

static const SRV_PLC_PCOUP srvPlcCoup = {
  SRV_PCOUP_RMS_HIGH_TBL, SRV_PCOUP_RMS_VLOW_TBL,
  SRV_PCOUP_THRS_HIGH_TBL, SRV_PCOUP_THRS_VLOW_TBL,
  SRV_PCOUP_DACC_TBL, 
  SRV_PCOUP_GAIN_HIGH_TBL, SRV_PCOUP_GAIN_VLOW_TBL,
  SRV_PCOUP_MAX_NUM_TX_LEVELS, SRV_PCOUP_EQU_NUM_COEF << 1, SRV_PCOUP_LINE_DRV_CONF,
  SRV_PCOUP_PRED_HIGH_TBL, SRV_PCOUP_PRED_VLOW_TBL
};

<#if (SRV_PCOUP_G3_MAIN_BAND == "FCC" || SRV_PCOUP_G3_MAIN_BAND == "ARIB") && (SRV_PCOUP_G3_AUX_BAND != "None")>   
static const SRV_PLC_PCOUP srvPlcCoupAux = {
  SRV_PCOUP_AUX_RMS_HIGH_TBL, SRV_PCOUP_AUX_RMS_VLOW_TBL,
  SRV_PCOUP_AUX_THRS_HIGH_TBL, SRV_PCOUP_AUX_THRS_VLOW_TBL,
  SRV_PCOUP_AUX_DACC_TBL, 
  SRV_PCOUP_AUX_GAIN_HIGH_TBL, SRV_PCOUP_AUX_GAIN_VLOW_TBL,
  SRV_PCOUP_AUX_MAX_NUM_TX_LEVELS, SRV_PCOUP_EQU_NUM_COEF << 1, SRV_PCOUP_AUX_LINE_DRV_CONF,
  SRV_PCOUP_AUX_PRED_HIGH_TBL, SRV_PCOUP_AUX_PRED_VLOW_TBL
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
    return (SRV_PLC_PCOUP *)&srvPlcCoup;
  } 
  else 
  {
<#if (SRV_PCOUP_G3_MAIN_BAND == "FCC" || SRV_PCOUP_G3_MAIN_BAND == "ARIB") && (SRV_PCOUP_G3_AUX_BAND != "None")>    
    return (SRV_PLC_PCOUP *)&srvPlcCoupAux;
<#else>
    /* Auxiliary Branch has not been configured. Please review project configuration with MHC */
    return SYS_STATUS_UNINITIALIZED;
</#if>
  }
}

SRV_PLC_PCOUP_BRANCH SRV_PCOUP_Get_Default_Branch( void )
{
  return SRV_PCOUP_DEFAULT_BRANCH;
}

uint8_t SRV_PCOUP_Get_Phy_Band(SRV_PLC_PCOUP_BRANCH branch)
{
  if (branch == SRV_PLC_PCOUP_MAIN_BRANCH) 
  {
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
