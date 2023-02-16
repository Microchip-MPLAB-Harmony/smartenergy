// <editor-fold defaultstate="collapsed" desc="DRV_RF215 Initialization Data">

<#if DRV_RF215_PLIB == "SRV_SPISPLIT">
<#--  Connected to SPI PLIB through SPI Splitter  -->
    <#assign SPI_PLIB = DRV_RF215_PLIB_SPISPLIT>
<#else>
<#--  Connected directly to SPI PLIB  -->
    <#assign SPI_PLIB = DRV_RF215_PLIB>
</#if>
/* RF215 Driver Initialization Data */
const DRV_RF215_INIT drvRf215InitData = {
<#if DRV_RF215_SPI_NUM_CSR != 0>
    /* SPI chip select register address used for SPI configuration */
<#if SPI_PLIB?starts_with("FLEXCOM")>
    .spiCSRegAddress = (uint32_t *)&(${SPI_PLIB}_REGS->FLEX_SPI_CSR[DRV_RF215_CSR_INDEX]),
<#else>
    .spiCSRegAddress = (uint32_t *)&(${SPI_PLIB}_REGS->SPI_CSR[DRV_RF215_CSR_INDEX]),
</#if>

</#if>
<#if core.DMA_ENABLE?has_content>
    /* SPI Transmit Register */
    .spiTransmitAddress = (const void *)${.vars["${SPI_PLIB?lower_case}"].TRANSMIT_DATA_REGISTER},

    /* SPI Receive Register */
    .spiReceiveAddress = (const void *)${.vars["${SPI_PLIB?lower_case}"].RECEIVE_DATA_REGISTER},

</#if>
<#if (DRV_RF215_PLIB == "SRV_SPISPLIT") || (core.DMA_ENABLE?has_content == false)>
    /* Pointer to SPI PLIB is busy function */
<#if SPI_PLIB?starts_with("FLEXCOM")>
    .spiPlibIsBusy = ${SPI_PLIB}_SPI_IsTransmitterBusy,
<#else>
    .spiPlibIsBusy = ${SPI_PLIB}_IsTransmitterBusy,
</#if>

</#if>
<#if (DRV_RF215_PLIB == "SRV_SPISPLIT") && (DRV_RF215_SPI_NUM_CSR != 0)>
    /* Pointer to SPI PLIB chip select function */
<#if SPI_PLIB?starts_with("FLEXCOM")>
    .spiPlibSetChipSelect = ${SPI_PLIB}_SPI_ChipSelectSetup,
<#else>
    .spiPlibSetChipSelect = ${SPI_PLIB}_ChipSelectSetup,
</#if>

</#if>
<#if core.DMA_ENABLE?has_content == false>
    /* Pointer to SPI Write and Read function */
<#if SPI_PLIB?starts_with("FLEXCOM")>
    .spiPlibWriteRead = ${SPI_PLIB}_SPI_WriteRead,
<#else>
    .spiPlibWriteRead = ${SPI_PLIB}_WriteRead,
</#if>

    /* Pointer to SPI Register Callback function */
<#if SPI_PLIB?starts_with("FLEXCOM")>
    .spiPlibSetCallback = ${SPI_PLIB}_SPI_CallbackRegister,
<#else>
    .spiPlibSetCallback = ${SPI_PLIB}_CallbackRegister,
</#if>

</#if>
    /* Interrupt source ID for DMA */
<#if core.DMA_ENABLE?has_content>
    .dmaIntSource = ${core.DMA_INSTANCE_NAME}_IRQn,
<#else>
    .dmaIntSource = ${SPI_PLIB}_IRQn,
</#if>

<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    /* Interrupt source ID for SYS_TIME */
    .sysTimeIntSource = ${.vars["${sys_time.SYS_TIME_PLIB?lower_case}"].IRQ_ENUM_NAME},

</#if>
<#if (drvPlcPhy)??>
    /* Interrupt source ID for PLC external interrupt */
    .plcExtIntSource = PIO${drvPlcPhy.DRV_PLC_EXT_INT_PIN[14]}_IRQn,

<#elseif (drvG3MacRt)??>
    /* Interrupt source ID for PLC external interrupt */
    .plcExtIntSource = PIO${drvG3MacRt.DRV_PLC_EXT_INT_PIN[14]}_IRQn,

</#if>
<#if DRV_RF215_TRX09_EN == true>
<#if DRV_RF215_TRX09_PHY_TYPE == "FSK">
    <#assign DRV_RF215_TRX09_MOD_OPM_STR = "OPM">
<#elseif DRV_RF215_TRX09_PHY_TYPE == "OFDM">
    <#assign DRV_RF215_TRX09_MOD_OPM_STR = "OPT">
</#if>
    /* Initial PHY frequency band and operating mode for Sub-GHz transceiver */
    .rf09PhyBandOpmIni = SUN_${DRV_RF215_TRX09_PHY_TYPE}_BAND_${DRV_RF215_TRX09_PHY_BAND}_${DRV_RF215_TRX09_MOD_OPM_STR}${DRV_RF215_TRX09_PHY_OPM},

    /* Initial PHY frequency channel number for Sub-GHz transceiver */
    .rf09PhyChnNumIni = ${DRV_RF215_TRX09_PHY_CHANNEL},

</#if>
<#if DRV_RF215_TRX24_EN == true>
<#if DRV_RF215_TRX24_PHY_TYPE == "FSK">
    <#assign DRV_RF215_TRX24_MOD_OPM_STR = "OPM">
<#elseif DRV_RF215_TRX24_PHY_TYPE == "OFDM">
    <#assign DRV_RF215_TRX24_MOD_OPM_STR = "OPT">
</#if>
    /* Initial PHY frequency band and operating mode for Sub-GHz transceiver */
    .rf24PhyBandOpmIni = SUN_${DRV_RF215_TRX24_PHY_TYPE}_BAND_${DRV_RF215_TRX24_PHY_BAND}_${DRV_RF215_TRX24_MOD_OPM_STR}${DRV_RF215_TRX24_PHY_OPM},

    /* Initial PHY frequency channel number for Sub-GHz transceiver */
    .rf24PhyChnNumIni = ${DRV_RF215_TRX24_PHY_CHANNEL},

</#if>
};

// </editor-fold>