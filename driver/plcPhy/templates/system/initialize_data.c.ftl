
// <editor-fold defaultstate="collapsed" desc="DRV_PLC_PHY Initialization Data">

/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 8.4 deviated once. Deviation record ID - H3_MISRAC_2012_R_8_4_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block deviate "MISRA C-2012 Rule 8.4" "H3_MISRAC_2012_R_8_4_DR_1"
</#if>

/* PLC Driver Initialization Data */
DRV_PLC_PHY_INIT drvPlcPhyInitData = {

    /* SPI PLIB API  interface*/
    .plcHal = &drvPLCHalAPI,

    /* PLC PHY Number of clients */
    .numClients = DRV_PLC_PHY_CLIENTS_NUMBER_IDX,  

    /* PLC PHY profile */
    .plcProfile = DRV_PLC_PHY_PROFILE,
 <#if DRV_PLC_BIN_STATIC_ADDRESSING == true>
 
    /* PLC Binary start address */
    .binStartAddress = DRV_PLC_BIN_START_ADDRESS,
    
    /* PLC Binary end address */
    .binEndAddress = DRV_PLC_BIN_START_ADDRESS + DRV_PLC_BIN_SIZE - 1,
<#else>
  <#if ((DRV_PLC_BAND_IN_USE >= 6) && (DRV_PLC_G3_BAND_AUX_ACTIVE == true))>
 
    /* PLC Binary start address */
    .binStartAddress = (uint32_t)&plc_phy_bin2_start,
    
    /* PLC Binary end address */
    .binEndAddress = (uint32_t)&plc_phy_bin2_end,

  <#else>
 
    /* PLC Binary start address */
    .binStartAddress = (uint32_t)&plc_phy_bin_start,
    
    /* PLC Binary end address */
    .binEndAddress = (uint32_t)&plc_phy_bin_end,
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