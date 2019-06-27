// <editor-fold defaultstate="collapsed" desc="DRV_PL360_MACRT Initialization Data">

/* HAL Interface Initialization for PLC device */
DRV_PLC_PLIB_INTERFACE drvPLCPlib = {

    /* PLC SPI PLIB */
    .spiPlibTransferSetup = (DRV_PLC_SPI_PLIB_TRANSFER_SETUP)${DRV_G3_MACRT_PLIB}_TransferSetup,

    /* DMA Channel for Transmit */
    .dmaChannelTx = SYS_DMA_CHANNEL_${DRV_G3_MACRT_TX_DMA_CHANNEL?string},

    /* DMA Channel for Receive */
    .dmaChannelRx  = SYS_DMA_CHANNEL_${DRV_G3_MACRT_RX_DMA_CHANNEL?string},

    /* SPI Transmit Register */
    .spiAddressTx =  (void *)&(${DRV_G3_MACRT_PLIB?string}_REGS->SPI_TDR),

    /* SPI Receive Register */
    .spiAddressRx  = (void *)&(${DRV_G3_MACRT_PLIB?string}_REGS->SPI_RDR),
    
    /* SPI clock frequency */
    .spiClockFrequency = DRV_PLC_SPI_CLK,
    
    /* PL360 LDO Enable Pin */
    .ldoPin = DRV_PLC_LDO_EN_PIN, 
    
    /* PL360 Reset Pin */
    .resetPin = DRV_PLC_RESET_PIN,
       
    /* PL360 External Interrupt Pin */
    .extIntPin = DRV_PLC_EXT_INT_PIN,
};

/* HAL Interface Initialization for PL360 device */
DRV_PLC_HAL_INTERFACE drvPLCHalAPI = {

    /* PLC PLIB */
    .plcPlib = &drvPLCPlib,

    /* PLC HAL init */
    .init = (DRV_PLC_HAL_INIT)DRV_PLC_HAL_Init,

    /* PLC HAL setup */
    .setup = (DRV_PLC_HAL_SETUP)DRV_PLC_HAL_Setup,

    /* PLC device reset */
    .reset = (DRV_PLC_HAL_RESET)DRV_PLC_HAL_Reset,

    /* PLC Carrier Detect Status */
    .getCd = (DRV_PLC_HAL_GET_CD)DRV_PLC_HAL_GetCarrierDetect,

    /* PLC HAL Enable/Disable external interrupt */
    .enableExtInt = (DRV_PLC_HAL_ENABLE_EXT_INT)DRV_PLC_HAL_EnableInterrupts,

    /* PLC HAL delay function */
    .delay = (DRV_PLC_HAL_DELAY)DRV_PLC_HAL_Delay,

    /* PLC HAL Transfer Bootloader Command */
    .sendBootCmd = (DRV_PLC_HAL_SEND_BOOT_CMD)DRV_PLC_HAL_SendBootCmd,

    /* PLC HAL Transfer Write/Read Command */
    .sendWrRdCmd = (DRV_PLC_HAL_SEND_WRRD_CMD)DRV_PLC_HAL_SendWrRdCmd,
};

<#if DRV_G3_MACRT_EXTERNAL_ADDRESSING == false> 
/* PL360 Binary file addressing */
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
    .numClients = DRV_PL360_MACRT_CLIENTS_NUMBER_IDX,  

    /* G3 MAC RT PLC profile */
    .plcProfile = DRV_PL360_MACRT_PLC_PROFILE,

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