
// <editor-fold defaultstate="collapsed" desc="DRV_G3_MACRT Initialization Data">

/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 8.4 deviated once. Deviation record ID - H3_MISRAC_2012_R_8_4_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block deviate "MISRA C-2012 Rule 8.4" "H3_MISRAC_2012_R_8_4_DR_1"
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

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 8.4"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRA C-2012 deviation block end */

// </editor-fold>