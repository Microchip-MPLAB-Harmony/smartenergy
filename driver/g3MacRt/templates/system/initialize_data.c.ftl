<#--
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
-->
// <editor-fold defaultstate="collapsed" desc="DRV_G3_MACRT Initialization Data">

<#if DRV_PLC_BIN_STATIC_ADDRESSING == false> 
/* PLC MAC RT Binary file addressing */
extern uint8_t g3_mac_rt_bin_start;
extern uint8_t g3_mac_rt_bin_end;
extern uint8_t g3_mac_rt_bin2_start;
extern uint8_t g3_mac_rt_bin2_end;
</#if>

/* G3 MAC RT Driver Initialization Data */
DRV_G3_MACRT_INIT drvG3MacRtInitData = {

    /* SPI PLIB API interface*/
    .plcHal = &drvPLCHalAPI,
<#if DRV_PLC_BIN_STATIC_ADDRESSING == true> 
 
    /* PLC Binary start address */
    .binStartAddress = 0,
    
    /* PLC Binary end address */
    .binEndAddress = 0,
<#else>
  <#if ((DRV_PLC_BAND_IN_USE >= 5) && (DRV_PLC_G3_BAND_AUX_ACTIVE == true))>
 
    /* PLC MAC RT Binary start address */
    .binStartAddress = (uint32_t)&g3_mac_rt_bin2_start,
    
    /* PLC MAC RT Binary end address */
    .binEndAddress = (uint32_t)&g3_mac_rt_bin2_end,

  <#else>
 
    /* PLC MAC RT Binary start address */
    .binStartAddress = (uint32_t)&g3_mac_rt_bin_start,
    
    /* PLC MAC RT Binary end address */
    .binEndAddress = (uint32_t)&g3_mac_rt_bin_end,
  </#if>
</#if>  

    /* Secure Mode */
    .secure = DRV_PLC_SECURE,
    
};

// </editor-fold>