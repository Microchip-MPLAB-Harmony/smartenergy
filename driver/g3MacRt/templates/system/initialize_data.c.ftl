// <editor-fold defaultstate="collapsed" desc="DRV_G3_MACRT Initialization Data">

<#if DRV_G3_MACRT_EXTERNAL_ADDRESSING == false> 
/* PLC Binary file addressing */
extern uint8_t g3_mac_rt_bin_start;
extern uint8_t g3_mac_rt_bin_end;
<#if DRV_G3_MACRT_PLC_PROFILE == "4">
extern uint8_t g3_mac_rt_bin2_start;
extern uint8_t g3_mac_rt_bin2_end;
</#if>

</#if>
/* G3 MAC RT Driver Initialization Data */
DRV_G3_MACRT_INIT drvG3MacRtInitData = {

    /* SPI PLIB API interface*/
    .plcHal = &drvPLCHalAPI,

    /* G3 MAC RT Number of clients */
    .numClients = DRV_G3_MACRT_CLIENTS_NUMBER_IDX,  

    /* G3 MAC RT PLC profile */
    .plcProfile = DRV_G3_MACRT_PLC_PROFILE,

    /* G3 MAC RT Specification Compliance */
    .plcSpecification = DRV_G3_MACRT_SPEC_COMPLIANCE,
 <#if DRV_G3_MACRT_EXTERNAL_ADDRESSING == true>   
 
    /* PLC Binary start address */
    .binStartAddress = 0,
    
    /* PLC Binary end address */
    .binEndAddress = 0,
<#else>
 
    /* PLC Binary start address */
    .binStartAddress = (uint32_t)&g3_mac_rt_bin_start,
    
    /* PLC Binary end address */
    .binEndAddress = (uint32_t)&g3_mac_rt_bin_start,
</#if>  

    /* Secure Mode */
    .secure = DRV_PLC_SECURE,
    
};

// </editor-fold>