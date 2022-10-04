<#--
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
-->
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

    /* PLC HAL delay function */
    .delay = (DRV_PLC_HAL_DELAY)DRV_PLC_HAL_Delay,

    /* PLC HAL Transfer Bootloader Command */
    .sendBootCmd = (DRV_PLC_HAL_SEND_BOOT_CMD)DRV_PLC_HAL_SendBootCmd,

    /* PLC HAL Transfer Write/Read Command */
    .sendWrRdCmd = (DRV_PLC_HAL_SEND_WRRD_CMD)DRV_PLC_HAL_SendWrRdCmd,
};

// </editor-fold>