
// <editor-fold defaultstate="collapsed" desc="DRV_METROLOGY Initialization Data">

/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 8.4 deviated once. Deviation record ID - H3_MISRAC_2012_R_8_4_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block deviate "MISRA C-2012 Rule 8.4" "H3_MISRAC_2012_R_8_4_DR_1"
</#if>

/* Metrology Driver Initialization Data */
DRV_METROLOGY_INIT drvMetrologyInitData = {

    /* MET bin destination address */
    .regBaseAddress = DRV_METROLOGY_REG_BASE_ADDRESS,

    /* MET Binary start address */
    .binStartAddress = (uint32_t)&met_bin_start,
    
    /* MET Binary end address */
    .binEndAddress = (uint32_t)&met_bin_end,
    
};

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 8.4"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRA C-2012 deviation block end */

// </editor-fold>