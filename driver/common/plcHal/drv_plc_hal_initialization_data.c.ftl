<#--
/*
Copyright (C) 2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
-->

// <editor-fold defaultstate="collapsed" desc="_on_reset() critical function">
<#assign SUPPORTED_BOARDS = [
    "PIC32CXMTSH Development Board",
    "PIC32CXMTG Evaluation Kit",
    "SAM E70 Xplained Ultra",
    "WBZ451 Curiosity",
    "SAM D20 Xplained Pro"
]>
<#assign BOARD_FIND = "">


/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 8.4 deviated once. Deviation record ID - H3_MISRAC_2012_R_8_4_DR_1 */
/* MISRA C-2012 Rule 21.2 deviated once. Deviation record ID - H3_MISRAC_2012_R_21_2_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block deviate "MISRA C-2012 Rule 8.4" "H3_MISRAC_2012_R_8_4_DR_1"
#pragma coverity compliance block deviate "MISRA C-2012 Rule 21.2" "H3_MISRAC_2012_R_21_2_DR_1"
</#if>

/* This routine must initialize the PL460 control pins as soon as possible */
/* after a power up reset to avoid risks on starting up PL460 device when */
/* pull up resistors are configured by default */
void _on_reset(void)
{
<#list SUPPORTED_BOARDS as BOARD>
    <#assign BSP_BOARD_NAME = "BSP_${BOARD}"?replace(" ", "_")>
    <#if .vars[BSP_BOARD_NAME]??>
        <#assign BOARD_FIND = BSP_BOARD_NAME>
        <#if BOARD?matches("PIC32CXMTSH Development Board")>
            <#lt>   /* Enable co-processor bus clock  */
            <#lt>   PMC_REGS->PMC_SCER = (PMC_SCER_CPKEY_PASSWD | PMC_SCER_CPBMCK_Msk);
            <#lt>   /* Coprocessor Peripheral Enable */
            <#lt>   RSTC_REGS->RSTC_MR |= (RSTC_MR_KEY_PASSWD | RSTC_MR_CPEREN_Msk);
            <#lt>
            <#lt>   /* Program PMC_CPU_CKR.CPPRES and wait for PMC_SR.CPMCKRDY to be set   */
            <#lt>   uint32_t reg = (PMC_REGS->PMC_CPU_CKR & ~PMC_CPU_CKR_CPPRES_Msk);
            <#lt>   reg |= PMC_CPU_CKR_CPPRES_CLK_2;
            <#lt>   PMC_REGS->PMC_CPU_CKR = reg;
            <#lt>
            <#lt>   PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk | PMC_PCR_EN_Msk | PMC_PCR_PID(ID_PIOA);
            <#lt>   while((PMC_REGS->PMC_CSR0 & PMC_CSR0_PID17_Msk) == 0U)
            <#lt>   {
            <#lt>       /* Wait for clock to be initialized */
            <#lt>   }
            <#lt>
            <#lt>   /* Disable STBY Pin */
            <#lt>   SYS_PORT_PinOutputEnable(SYS_PORT_PIN_PA16);
            <#lt>   SYS_PORT_PinClear(SYS_PORT_PIN_PA16);
            <#lt>
            <#lt>   while ((PMC_REGS->PMC_SR & PMC_SR_CPMCKRDY_Msk) != PMC_SR_CPMCKRDY_Msk)
            <#lt>   {
            <#lt>       /* Wait for status CPMCKRDY */
            <#lt>   }
            <#lt>
            <#lt>   PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk | PMC_PCR_EN_Msk | PMC_PCR_PID(ID_PIOD);
            <#lt>   while((PMC_REGS->PMC_CSR2 & PMC_CSR2_PID85_Msk) == 0U)
            <#lt>   {
            <#lt>       /* Wait for clock to be initialized */
            <#lt>   }
            <#lt>
            <#lt>   /* Enable Reset Pin */
            <#lt>   SYS_PORT_PinOutputEnable(DRV_PLC_RESET_PIN);
            <#lt>   SYS_PORT_PinClear(DRV_PLC_RESET_PIN);
        </#if>
        <#if BOARD?matches("PIC32CXMTG Evaluation Kit")>
            <#lt>    PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk | PMC_PCR_EN_Msk | PMC_PCR_PID(ID_PIOA);
            <#lt>    while((PMC_REGS->PMC_CSR0 & PMC_CSR0_PID17_Msk) == 0U)
            <#lt>    {
            <#lt>        /* Wait for clock to be initialized */
            <#lt>    }
            <#lt>
            <#lt>    /* Disable STBY Pin */
            <#lt>    SYS_PORT_PinOutputEnable(SYS_PORT_PIN_PA0);
            <#lt>    SYS_PORT_PinClear(SYS_PORT_PIN_PA0);
        </#if>
        <#if BOARD?matches("SAM E70 Xplained Ultra")>
            <#lt>    /* Enables PIOA and PIOC */
            <#lt>    PMC_REGS->PMC_PCER0 = PMC_PCER0_PID10_Msk | PMC_PCER0_PID12_Msk;
            <#lt>
            <#lt>    /* Enable Reset Pin */
            <#lt>    SYS_PORT_PinOutputEnable(DRV_PLC_RESET_PIN);
            <#lt>    SYS_PORT_PinClear(DRV_PLC_RESET_PIN);
            <#lt>    /* Disable STBY Pin */
            <#lt>    SYS_PORT_PinOutputEnable(SYS_PORT_PIN_PA3);
            <#lt>    SYS_PORT_PinClear(SYS_PORT_PIN_PA3);
            <#lt>    /* Disable LDO Pin */
            <#lt>    SYS_PORT_PinOutputEnable(DRV_PLC_LDO_EN_PIN);
            <#lt>    SYS_PORT_PinClear(DRV_PLC_LDO_EN_PIN);
        </#if>
        <#if BOARD?matches("WBZ451 Curiosity")>
            <#lt>    __asm volatile ("NOP");
        </#if>
        <#if BOARD?matches("SAM D20 Xplained Pro")>
            <#lt>    /* Disable STBY Pin */
            <#lt>    SYS_PORT_PinOutputEnable(SYS_PORT_PIN_PA08);
            <#lt>    SYS_PORT_PinClear(SYS_PORT_PIN_PA08);
            <#lt>    /* Enable Reset Pin */
            <#lt>    SYS_PORT_PinOutputEnable(DRV_PLC_RESET_PIN);
            <#lt>    SYS_PORT_PinClear(DRV_PLC_RESET_PIN);
            <#lt>    /* Disable LDO Pin */
            <#lt>    SYS_PORT_PinOutputEnable(DRV_PLC_LDO_EN_PIN);
            <#lt>    SYS_PORT_PinClear(DRV_PLC_LDO_EN_PIN);
        </#if>
    </#if>
</#list>
<#if BOARD_FIND == "">
    #warning Board not supported. Please, review Pin configuration to initialize the PL460-EK GPIOs (RESET, STBY and LDO_EN).
    <#lt>    __asm volatile ("NOP");
    <#lt>    /* Enable Reset Pin */
    <#lt>    // SYS_PORT_PinOutputEnable(DRV_PLC_RESET_PIN);
    <#lt>    // SYS_PORT_PinClear(DRV_PLC_RESET_PIN);
    <#lt>    /* Disable STBY Pin */
    <#lt>    // SYS_PORT_PinOutputEnable(SYS_PORT_PIN_PA08);
    <#lt>    // SYS_PORT_PinClear(SYS_PORT_PIN_PA08);
    <#lt>    /* Disable LDO Pin */
    <#lt>    // SYS_PORT_PinOutputEnable(DRV_PLC_LDO_EN_PIN);
    <#lt>    // SYS_PORT_PinClear(DRV_PLC_LDO_EN_PIN);
</#if>
}

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 8.4"
#pragma coverity compliance end_block "MISRA C-2012 Rule 21.2"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRA C-2012 deviation block end */

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="DRV_PLC_HAL Initialization Data">
<#if DRV_PLC_PLIB == "SRV_SPISPLIT">
<#--  Connected to SPI PLIB through SPI Splitter  -->
    <#assign SPI_PLIB = DRV_PLC_PLIB_SPISPLIT>
<#else>
<#--  Connected directly to SPI PLIB  -->
    <#assign SPI_PLIB = DRV_PLC_PLIB>
</#if>
<#if SPI_PLIB?lower_case[0..*6] == "sercom">
    <#assign SPI_PREFFIX = "SPI">
<#elseif SPI_PLIB?lower_case[0..*7] == "flexcom">
    <#assign SPI_PREFFIX = "FLEX_SPI">
<#elseif SPI_PLIB?lower_case[0..*3] == "spi">
    <#assign SPI_PREFFIX = "SPI">
</#if>
/* HAL Interface Initialization for PLC transceiver */
static DRV_PLC_PLIB_INTERFACE drvPLCPlib = {

    /* SPI Transfer Setup */
    .spiPlibTransferSetup = (DRV_PLC_SPI_PLIB_TRANSFER_SETUP)${.vars["${SPI_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_TransferSetup,

<#if DRV_PLC_PLIB == "SRV_SPISPLIT">
    /* SPI Is Busy */
    .spiIsBusy = ${.vars["${SPI_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_IsTransmitterBusy,

<#elseif core.DMA_ENABLE?has_content == false>
    /* SPI Is Busy */
<#if core.CoreArchitecture == "CORTEX-M0PLUS">
    .spiIsBusy = ${.vars["${SPI_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_IsBusy,
<#else>
    .spiIsBusy = ${.vars["${SPI_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_IsTransmitterBusy,
</#if>

</#if>
<#if (DRV_PLC_PLIB == "SRV_SPISPLIT") && (DRV_PLC_SPI_NUM_CSR != 0)>
    /* SPI Set Chip Select */
    .spiSetChipSelect = ${.vars["${SPI_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_ChipSelectSetup,

</#if>
<#if core.DMA_ENABLE?has_content>
    /* DMA Channel for Transmit */
    .dmaChannelTx = SYS_DMA_CHANNEL_${DRV_PLC_TX_DMA_CHANNEL?string},

    /* DMA Channel for Receive */
    .dmaChannelRx = SYS_DMA_CHANNEL_${DRV_PLC_RX_DMA_CHANNEL?string},

<#if SPI_PLIB?lower_case[0..*6] == "sercom">
    /* SPI Transmit Register */
    .spiAddressTx = (void *)&(${SPI_PLIB}_REGS->SPIM.SERCOM_DATA),

    /* SPI Receive Register */
    .spiAddressRx  = (void *)&(${SPI_PLIB}_REGS->SPIM.SERCOM_DATA),

<#else>
    /* SPI Transmit Register */
    .spiAddressTx = (void *)&(${.vars["${SPI_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_REGS->SPI_TDR),

    /* SPI Receive Register */
    .spiAddressRx  = (void *)&(${.vars["${SPI_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_REGS->SPI_RDR),

</#if>
<#else>
    /* SPI Write/Read */
    .spiWriteRead = ${.vars["${SPI_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_WriteRead,

</#if>
<#if SPI_PLIB?lower_case[0..*6] == "sercom">
    /* SPI Chip select pin */
    .spiCSPin = DRV_PLC_SPI_CS_PIN,

</#if>
    /* SPI clock frequency */
    .spiClockFrequency = DRV_PLC_SPI_CLK,

    /* PLC LDO Enable Pin */
    .ldoPin = DRV_PLC_LDO_EN_PIN,

    /* PLC Reset Pin */
    .resetPin = DRV_PLC_RESET_PIN,

    /* PLC External Interrupt Pin */
    .extIntPin = DRV_PLC_EXT_INT_PIN,

    /* PLC External Interrupt Pio */
    .extIntPio = DRV_PLC_EXT_INT_PIO,

<#if DRV_PLC_MODE == "PL460">
    /* PLC TX Enable Pin */
    .txEnablePin = DRV_PLC_TX_ENABLE_PIN,

</#if>
<#if DRV_PLC_SLEEP_MODE == true>
    /* PLC StandBy Pin */
    .stByPin = DRV_PLC_STBY_PIN,

</#if>
<#if DRV_PLC_MODE == "PL460" && DRV_PLC_THERMAL_MONITOR == true>
    /* PLC External Interrupt Pin */
    .thMonPin = DRV_PLC_THMON_PIN,

</#if>
<#if (DRV_PLC_PLIB == "SRV_SPISPLIT") && (drvRf215)??>
    /* Interrupt source ID for RF external interrupt */
    .rfExtIntSource = PIO${drvRf215.DRV_RF215_EXT_INT_PIN[14]}_IRQn,

    /* Interrupt source ID for DMA */
    .dmaIntSource = ${core.DMA_INSTANCE_NAME}_IRQn,

<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
    /* Interrupt source ID for SYS_TIME */
    .sysTimeIntSource = ${.vars["${sys_time.SYS_TIME_PLIB?lower_case}"].IRQ_ENUM_NAME},

</#if>
</#if>
};

/* HAL Interface Initialization for PLC transceiver */
static DRV_PLC_HAL_INTERFACE drvPLCHalAPI = {

    /* PLC PLIB */
    .plcPlib = &drvPLCPlib,

    /* PLC HAL init */
    .init = (DRV_PLC_HAL_INIT)DRV_PLC_HAL_Init,

    /* PLC HAL setup */
    .setup = (DRV_PLC_HAL_SETUP)DRV_PLC_HAL_Setup,

    /* PLC transceiver reset */
    .reset = (DRV_PLC_HAL_RESET)DRV_PLC_HAL_Reset,

<#if DRV_PLC_SLEEP_MODE == true>
    /* PLC Set StandBy Mode */
    .setStandBy = (DRV_PLC_HAL_SET_STBY)DRV_PLC_HAL_SetStandBy,

</#if>
<#if DRV_PLC_MODE == "PL460" && DRV_PLC_THERMAL_MONITOR == true>
    /* PLC Get Thermal Monitor value */
    .getThermalMonitor = (DRV_PLC_HAL_GET_THMON)DRV_PLC_HAL_GetThermalMonitor,

</#if>
<#if DRV_PLC_MODE == "PL460">
    /* PLC Set TX Enable Pin */
    .setTxEnable = (DRV_PLC_HAL_SET_TXENABLE)DRV_PLC_HAL_SetTxEnable,

</#if>
    /* PLC HAL Enable/Disable external interrupt */
    .enableExtInt = (DRV_PLC_HAL_ENABLE_EXT_INT)DRV_PLC_HAL_EnableInterrupts,

    /* PLC HAL Enable/Disable external interrupt */
    .getPinLevel = (DRV_PLC_HAL_GET_PIN_LEVEL)DRV_PLC_HAL_GetPinLevel,

    /* PLC HAL delay function */
    .delay = (DRV_PLC_HAL_DELAY)DRV_PLC_HAL_Delay,

    /* PLC HAL Transfer Bootloader Command */
    .sendBootCmd = (DRV_PLC_HAL_SEND_BOOT_CMD)DRV_PLC_HAL_SendBootCmd,

    /* PLC HAL Transfer Write/Read Command */
    .sendWrRdCmd = (DRV_PLC_HAL_SEND_WRRD_CMD)DRV_PLC_HAL_SendWrRdCmd,
};

// </editor-fold>