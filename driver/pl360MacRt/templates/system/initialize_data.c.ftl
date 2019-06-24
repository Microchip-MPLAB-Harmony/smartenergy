// <editor-fold defaultstate="collapsed" desc="DRV_PL360_MACRT Initialization Data">

/* HAL Interface Initialization for PL360 device */
DRV_PL360_PLIB_INTERFACE drvPL360Plib = {

    /* PL360 SPI PLIB */
    .spiPlibTransferSetup = (DRV_PL360_SPI_PLIB_TRANSFER_SETUP)${DRV_PL360_PLIB}_TransferSetup,

    /* DMA Channel for Transmit */
    .dmaChannelTx = SYS_DMA_CHANNEL_${DRV_PL360_TX_DMA_CHANNEL?string},

    /* DMA Channel for Receive */
    .dmaChannelRx  = SYS_DMA_CHANNEL_${DRV_PL360_RX_DMA_CHANNEL?string},

    /* SPI Transmit Register */
    .spiAddressTx =  (void *)&(${DRV_PL360_PLIB?string}_REGS->SPI_TDR),

    /* SPI Receive Register */
    .spiAddressRx  = (void *)&(${DRV_PL360_PLIB?string}_REGS->SPI_RDR),
    
    /* SPI clock frequency */
    .spiClockFrequency = DRV_PL360_SPI_CLK,
    
    /* PL360 LDO Enable Pin */
    .ldoPin = DRV_PL360_LDO_EN_PIN, 
    
    /* PL360 Reset Pin */
    .resetPin = DRV_PL360_RESET_PIN,
       
    /* PL360 External Interrupt Pin */
    .extIntPin = DRV_PL360_EXT_INT_PIN,
};

/* HAL Interface Initialization for PL360 device */
DRV_PL360_HAL_INTERFACE drvPL360HalAPI = {

    /* PL360 PLIB */
    .pl360Plib = &drvPL360Plib,

    /* PL360 HAL init */
    .init = (DRV_PL360_HAL_INIT)DRV_PL360_HAL_Init,

    /* PL360 HAL setup */
    .setup = (DRV_PL360_HAL_SETUP)DRV_PL360_HAL_Setup,

    /* PL360 PL360 reset */
    .reset = (DRV_PL360_HAL_RESET)DRV_PL360_HAL_Reset,

    /* PL360 HAL Get Carrier Detect or PLC Line Status */
    .getCd = (DRV_PL360_HAL_GET_CD)DRV_PL360_HAL_GetCarrierDetect,

    /* PL360 HAL Enable/Disable external interrupt */
    .enableExtInt = (DRV_PL360_HAL_ENABLE_EXT_INT)DRV_PL360_HAL_EnableInterrupts,

    /* PL360 HAL delay function */
    .delay = (DRV_PL360_HAL_DELAY)DRV_PL360_HAL_Delay,

    /* PL360 HAL Transfer Bootloader Command */
    .sendBootCmd = (DRV_PL360_HAL_SEND_BOOT_CMD)DRV_PL360_HAL_SendBootCmd,

    /* PL360 HAL Transfer Write/Read Command */
    .sendWrRdCmd = (DRV_PL360_HAL_SEND_WRRD_CMD)DRV_PL360_HAL_SendWrRdCmd,
};

<#if DRV_PL360_EXTERNAL_ADDRESSING == false> 
/* PL360 Binary file addressing */
extern uint8_t pl360_mac_rt_bin_start;
extern uint8_t pl360_mac_rt_bin_start;
<#if DRV_PL360_MACRT_PLC_PROFILE == "5">
extern uint8_t pl360_mac_rt_bin2_start;
extern uint8_t pl360_mac_rt_bin2_start;
</#if>

</#if>
/* PL360 MAC RT Driver Initialization Data */
DRV_PL360_MACRT_INIT drvPL360MacRtInitData = {

    /* SPI PLIB API  interface*/
    .pl360Hal = &drvPL360HalAPI,

    /* PL360 Number of clients */
    .numClients = DRV_PL360_MACRT_CLIENTS_NUMBER_IDX,  

    /* PL360 MAC RT PLC profile */
    .plcProfile = DRV_PL360_MACRT_PLC_PROFILE,
 <#if DRV_PL360_EXTERNAL_ADDRESSING == true>   
 
    /* PLC Binary start address */
    .binStartAddress = 0,
    
    /* PLC Binary end address */
    .binEndAddress = 0,
<#else>
 
    /* PLC Binary start address */
    .binStartAddress = (uint32_t)&pl360_mac_rt_bin_start,
    
    /* PLC Binary end address */
    .binEndAddress = (uint32_t)&pl360_mac_rt_bin_start,
</#if>  

    /* Secure Mode */
    .secure = DRV_PL360_SECURE,
    
};

// </editor-fold>