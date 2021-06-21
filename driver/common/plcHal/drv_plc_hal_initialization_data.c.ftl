// <editor-fold defaultstate="collapsed" desc="DRV_PLC_HAL Initialization Data">

/* HAL Interface Initialization for PLC transceiver */
DRV_PLC_PLIB_INTERFACE drvPLCPlib = {

 <#if DRV_PLC_TX_RX_DMA == true> 
    /* SPI Transfer Setup */
    .spiPlibTransferSetup = (DRV_PLC_SPI_PLIB_TRANSFER_SETUP)${.vars["${DRV_PLC_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_TransferSetup,

    /* DMA Channel for Transmit */
    .dmaChannelTx = SYS_DMA_CHANNEL_${DRV_PLC_TX_DMA_CHANNEL?string},

    /* DMA Channel for Receive */
    .dmaChannelRx  = SYS_DMA_CHANNEL_${DRV_PLC_RX_DMA_CHANNEL?string},

    /* SPI Transmit Register */
    .spiAddressTx =  (void *)&(${.vars["${DRV_PLC_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_REGS->SPI_TDR),

    /* SPI Receive Register */
    .spiAddressRx  = (void *)&(${.vars["${DRV_PLC_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_REGS->SPI_RDR),

    /* SPI MR register address. */
    .spiMR  = (void *)&(${.vars["${DRV_PLC_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_REGS->SPI_MR),

    /* SPI CSR register address. */
    .spiCSR  = (void *)&(${.vars["${DRV_PLC_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_REGS->SPI_CSR),
<#else>
    /* SPI Transfer Setup */
    .spiPlibTransferSetup = (DRV_PLC_SPI_PLIB_TRANSFER_SETUP)${.vars["${DRV_PLC_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_TransferSetup,

    /* SPI Write/Read */
    .spiWriteRead = ${.vars["${DRV_PLC_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_WriteRead,
    
    /* SPI Is Busy */
    .spiIsBusy = ${.vars["${DRV_PLC_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_IsBusy,
</#if>
    
    /* SPI clock frequency */
    .spiClockFrequency = DRV_PLC_SPI_CLK,
    
    /* PLC LDO Enable Pin */
    .ldoPin = DRV_PLC_LDO_EN_PIN, 
    
    /* PLC Reset Pin */
    .resetPin = DRV_PLC_RESET_PIN,
       
    /* PLC External Interrupt Pin */
    .extIntPin = DRV_PLC_EXT_INT_PIN,

    /* PLC External Interrupt Pin */
    .cdPin = DRV_PLC_CD_PIN,

<#if DRV_PLC_MODE == "PL460">       
    /* PLC StandBy Pin */
    .stByPin = DRV_PLC_STBY_PIN,

    /* PLC External Interrupt Pin */
    .thMonPin = DRV_PLC_THMON_PIN,
    
</#if>
};

/* HAL Interface Initialization for PLC transceiver */
DRV_PLC_HAL_INTERFACE drvPLCHalAPI = {

    /* PLC PLIB */
    .plcPlib = &drvPLCPlib,

    /* PLC HAL init */
    .init = (DRV_PLC_HAL_INIT)DRV_PLC_HAL_Init,

    /* PLC HAL setup */
    .setup = (DRV_PLC_HAL_SETUP)DRV_PLC_HAL_Setup,

    /* PLC transceiver reset */
    .reset = (DRV_PLC_HAL_RESET)DRV_PLC_HAL_Reset,

<#if DRV_PLC_MODE == "PL460">       
    /* PLC low power management */
    .standBy = (DRV_PLC_HAL_STBY)DRV_PLC_HAL_StandBy,    
<#else>
    /* PLC low power management */
    .standBy = NULL,
</#if>

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

// </editor-fold>