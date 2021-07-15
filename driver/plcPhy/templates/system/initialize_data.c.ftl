// <editor-fold defaultstate="collapsed" desc="DRV_PLC_PHY Initialization Data">

<#if DRV_PLC_BIN_STATIC_ADDRESSING == false> 
/* PLC Binary file addressing */
extern uint8_t plc_phy_bin_start;
extern uint8_t plc_phy_bin_end;
<#if (DRV_PLC_PROFILE == "G3")>
extern uint8_t plc_phy_bin2_start;
extern uint8_t plc_phy_bin2_end;
</#if>

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
    .binStartAddress = 0,
    
    /* PLC Binary end address */
    .binEndAddress = 0,
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

// </editor-fold>