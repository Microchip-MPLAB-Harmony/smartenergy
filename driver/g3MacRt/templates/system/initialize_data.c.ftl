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

    /* G3 MAC RT Number of clients */
    .numClients = DRV_G3_MACRT_CLIENTS_NUMBER_IDX,  

    /* G3 MAC RT Specification Compliance */
    .plcSpecification = DRV_G3_MACRT_SPEC_COMPLIANCE,
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