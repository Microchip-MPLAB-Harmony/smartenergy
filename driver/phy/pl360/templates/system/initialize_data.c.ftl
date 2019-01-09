// <editor-fold defaultstate="collapsed" desc="DRV_PL360 Initialization Data">

/* HAL Interface Initialization for PL360 Driver */
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
    
    /* PL360 LDO Enable Pin */
    .ldoPin = DRV_PL360_LDO_EN_PIN, 
    
    /* PL360 Reset Pin */
    .resetPin = DRV_PL360_RESET_PIN,
       
    /* PL360 External Interrupt Pin */
    .extIntPin = DRV_PL360_EXT_INT_PIN,
};

/* HAL Interface Initialization for PL360 Driver */
DRV_PL360_HAL_INTERFACE drvPL360HalAPI = {

    /* PL360 PLIB */
    .pl360Plib = &drvPL360Plib,

    /* PL360 HAL init */
    .init = (DRV_PL360_HAL_INIT)drv_pl360_hal_init,

    /* PL360 HAL setup */
    .setup = (DRV_PL360_HAL_SETUP)drv_pl360_hal_setup,

    /* PL360 PL360 reset */
    .reset = (DRV_PL360_HAL_RESET)drv_pl360_hal_reset,

    /* PL360 HAL Get Carrier Detect or PLC Line Status */
    .getCd = (DRV_PL360_HAL_GET_CD)drv_pl360_hal_get_cd,

    /* PL360 HAL Enable/Disable external interrupt */
    .enableExtInt = (DRV_PL360_HAL_ENABLE_EXT_INT)drv_pl360_hal_enable_interrupt,

    /* PL360 HAL delay function */
    .delay = (DRV_PL360_HAL_DELAY)drv_pl360_hal_delay,

    /* PL360 HAL Transfer Bootloader Command */
    .sendBootCmd = (DRV_PL360_HAL_SEND_BOOT_CMD)drv_pl360_hal_send_boot_cmd,

    /* PL360 HAL Transfer Write/Read Command */
    .sendWrRdCmd = (DRV_PL360_HAL_SEND_WRRD_CMD)drv_pl360_hal_send_wrrd_cmd,
};

/* PL360 Driver Initialization Data */
DRV_PL360_INIT drvPL360InitData =
{
    /* SPI PLIB API  interface*/
    .pl360Hal = &drvPL360HalAPI,

    /* PL360 Number of clients */
    .numClients = DRV_PL360_CLIENTS_NUMBER_IDX,  

    /* PL360 PLC profile */
    .plcProfile = DRV_PL360_PLC_PROFILE,
 <#if DRV_PL360_BIN_STATIC_ADDRESSING == true>   
 
    /* Size (in Bytes) of the PLC binary file */
    .binSize = DRV_PL360_BIN_SIZE,

    /* Address where PLC binary file is located */
    .binStartAddress = DRV_PL360_BIN_ADDRESS, 
</#if>  

    /* Secure Mode */
    .secure = DRV_PL360_SECURE,
    
};

// </editor-fold>