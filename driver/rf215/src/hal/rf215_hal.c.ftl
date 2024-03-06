/******************************************************************************
  RF215 Driver Hardware Abstraction Layer Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    rf215_hal.c

  Summary:
    Source code for the RF215 Driver Hardware Abstraction Layer implementation.

  Description:
    The RF215 driver HAL (Hardware Abstraction Layer) manages the hardware
    peripherals used by the RF215 driver. This file contains the source code
    for the implementation of the RF215 driver HAL.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "configuration.h"
#include "system/ports/sys_ports.h"
<#if core.DMA_ENABLE?has_content>
#include "system/dma/sys_dma.h"
</#if>
<#if core.DATA_CACHE_ENABLE?? && core.DATA_CACHE_ENABLE == true >
#include "system/cache/sys_cache.h"
</#if>
#include "driver/rf215/drv_rf215_local.h"
#include "driver/rf215/hal/rf215_hal.h"

<#if core.PIO_ENABLE?has_content>
    <#assign PIO_PLIB = "PIO">
<#elseif core.GPIO_ENABLE?has_content>
    <#assign PIO_PLIB = "GPIO">
<#else>
    <#assign PIO_PLIB = "">
</#if>
// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

/* Reset pin pulse width.
 * From RF215 datasheet (Table 10-3): Min tRST = 625 ns */
#define RF215_RST_PULSE_US            7

/* SPI DMA buffer size */
#define RF215_SPI_BUF_SIZE            (RF215_SPI_CMD_SIZE + DRV_RF215_MAX_PSDU_LEN)

/* SPI transfer pool size: Maximum number of SPI transfers that can be queued */
<#if DRV_RF215_TRX09_EN == true && DRV_RF215_TRX24_EN == true>
#define RF215_SPI_TRANSFER_POOL_SIZE  35
<#else>
#define RF215_SPI_TRANSFER_POOL_SIZE  20
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data
// *****************************************************************************
// *****************************************************************************

/* RF215 HAL object */
static RF215_HAL_OBJ rf215HalObj = {0};

/* SPI transfer pool */
static RF215_SPI_TRANSFER_OBJ halSpiTransferPool[RF215_SPI_TRANSFER_POOL_SIZE] = {0};

/* DMA buffers for SPI transmit and receive */
<#if core.DATA_CACHE_ENABLE?? && core.DATA_CACHE_ENABLE == true >
static CACHE_ALIGN uint8_t halSpiTxData[CACHE_ALIGNED_SIZE_GET(RF215_SPI_BUF_SIZE)];
static CACHE_ALIGN uint8_t halSpiRxData[CACHE_ALIGNED_SIZE_GET(RF215_SPI_BUF_SIZE)];
<#else>
static uint8_t halSpiTxData[RF215_SPI_BUF_SIZE];
static uint8_t halSpiRxData[RF215_SPI_BUF_SIZE];
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************

static inline void _RF215_HAL_ExtIntDisable()
{
    ${PIO_PLIB}_PinInterruptDisable((PIO_PIN) DRV_RF215_EXT_INT_PIN);
    rf215HalObj.extIntDisableCount++;
}

static inline void _RF215_HAL_ExtIntEnable()
{
    if (rf215HalObj.extIntDisableCount > 0)
    {
        rf215HalObj.extIntDisableCount--;
    }

    if (rf215HalObj.extIntDisableCount == 0)
    {
        ${PIO_PLIB}_PinInterruptEnable((PIO_PIN) DRV_RF215_EXT_INT_PIN);
    }
}

static inline bool _RF215_HAL_DisableIntSources(<#if DRV_RF215_TXRX_TIME_SUPPORT == true>bool* timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, bool* plcExtIntStatus</#if></#if>)
{
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
<#if (drvPlcPhy)?? || (drvG3MacRt)??>
    *plcExtIntStatus = SYS_INT_SourceDisable(rf215HalObj.plcExtIntSource);
</#if>
    *timeIntStatus = SYS_INT_SourceDisable(rf215HalObj.sysTimeIntSource);
</#if>
    return SYS_INT_SourceDisable(rf215HalObj.dmaIntSource);
}

static inline void _RF215_HAL_RestoreIntSources(bool dmaIntStatus<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, bool timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, bool plcExtIntStatus</#if></#if>)
{
    SYS_INT_SourceRestore(rf215HalObj.dmaIntSource, dmaIntStatus);
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    SYS_INT_SourceRestore(rf215HalObj.sysTimeIntSource, timeIntStatus);
<#if (drvPlcPhy)?? || (drvG3MacRt)??>
    SYS_INT_SourceRestore(rf215HalObj.plcExtIntSource, plcExtIntStatus);
</#if>
</#if>
}

static void _RF215_HAL_SpiTransferStart (
    RF215_SPI_TRANSFER_MODE mode,
    uint16_t regAddr,
    void* pData,
    size_t size
)
{
    size_t transferSize<#if core.DATA_CACHE_ENABLE?? && core.DATA_CACHE_ENABLE == true>, txCleanCacheSize</#if>;
    uint16_t cmd;
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    bool intStatus;
<#elseif (DRV_RF215_PLIB == "SRV_SPISPLIT") && ((drvPlcPhy)?? || (drvG3MacRt)??)>
    bool plcExtIntStatus;
</#if>
    uint8_t* pTxData = halSpiTxData;
    RF215_HAL_OBJ* hObj = &rf215HalObj;

    /* Build 16 bits corresponding to COMMAND
     * COMMAND[15:14] = MODE[1:0]; COMMAND[13:0] = ADDRESS[13:0] */
    cmd = regAddr | (uint16_t)mode;
    transferSize = size + RF215_SPI_CMD_SIZE;

    /* Write COMMAND to SPI transmit buffer (MSB first) */
    *pTxData++ = (uint8_t)(cmd >> 8);
    *pTxData++ = (uint8_t)(cmd & 0xFF);

    if (mode == RF215_SPI_WRITE)
    {
        /* Copy data to SPI transmit buffer */
        memcpy(pTxData, pData, size);
<#if core.DATA_CACHE_ENABLE?? && core.DATA_CACHE_ENABLE == true >
        /* All SPI transmit data cache must be cleaned */
        txCleanCacheSize = transferSize;
</#if>
    }
<#if core.DATA_CACHE_ENABLE?? && core.DATA_CACHE_ENABLE == true >
    else
    {
        /* Only COMMAND SPI transmit data cache needs to be cleaned */
        txCleanCacheSize = RF215_SPI_CMD_SIZE;
    }
</#if>

<#if DRV_RF215_PLIB == "SRV_SPISPLIT">
<#if (DRV_RF215_TXRX_TIME_SUPPORT == false) && ((drvPlcPhy)?? || (drvG3MacRt)??)>
    /* Disable PLC external interrupt to avoid SPI transfer from PLC driver */
    plcExtIntStatus = SYS_INT_SourceDisable(hObj->plcExtIntSource);

</#if>
    /* Wait if there is SPI transfer in progress */
    while(hObj->spiPlibIsBusy() == true);
    while(SYS_DMA_ChannelIsBusy(DRV_RF215_SPI_RX_DMA_CH) == true);

<#if DRV_RF215_SPI_NUM_CSR != 0>
    /* Set chip select (shared SPI) */
    hObj->spiPlibSetChipSelect(DRV_RF215_SPI_CHIP_SELECT);

</#if>
<#if core.DMA_ENABLE?has_content>
    /* Set 8-bit data width in DMA channels (shared SPI) */
    SYS_DMA_DataWidthSetup(DRV_RF215_SPI_TX_DMA_CH, SYS_DMA_WIDTH_8_BIT);
    SYS_DMA_DataWidthSetup(DRV_RF215_SPI_RX_DMA_CH, SYS_DMA_WIDTH_8_BIT);

</#if>
</#if>
<#if core.DMA_ENABLE?has_content>
    /* Configure DMA channel for SPI receive */
    SYS_DMA_ChannelTransfer(DRV_RF215_SPI_RX_DMA_CH, hObj->spiRxAddr, halSpiRxData, transferSize);

</#if>
<#if core.DATA_CACHE_ENABLE?? && core.DATA_CACHE_ENABLE == true >
	/* Clean cache to push SPI transmit buffer to the main memory */
    SYS_CACHE_CleanDCache_by_Addr((uint32_t *) halSpiTxData, txCleanCacheSize);

</#if>
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    /* Disable all interrupts for a while to avoid delays between SPI transfer
     * and SYS_TIME counter read */
    intStatus = SYS_INT_Disable();

</#if>
<#if core.DMA_ENABLE?has_content>
    /* Configure DMA channel for SPI transmit. This triggers the SPI transfer */
    SYS_DMA_ChannelTransfer(DRV_RF215_SPI_TX_DMA_CH, halSpiTxData, hObj->spiTxAddr, transferSize);
<#else>
    /* Write and read through SPI. This triggers the SPI transfer */
    hObj->spiPlibWriteRead(halSpiTxData, transferSize, halSpiRxData, transferSize);
</#if>
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>

    /* Read SYS_TIME counter just after SPI transfer is launched */
    hObj->sysTimeTransfer = SYS_TIME_Counter64Get();
    SYS_INT_Restore(intStatus);
</#if>
<#if DRV_RF215_PLIB == "SRV_SPISPLIT">
<#if (DRV_RF215_TXRX_TIME_SUPPORT == false) && ((drvPlcPhy)?? || (drvG3MacRt)??)>

    /* Restore PLC external interrupt status */
    SYS_INT_SourceRestore(hObj->plcExtIntSource, plcExtIntStatus);
</#if>

    /* Set DMA transfer in progress */
    hObj->dmaTransferInProgress = true;
</#if>
<#if core.DATA_CACHE_ENABLE?? && core.DATA_CACHE_ENABLE == true >

    if (mode == RF215_SPI_READ)
    {
        /* Invalidate SPI receive buffer (force CPU to load from main memory) */
        SYS_CACHE_InvalidateDCache_by_Addr((uint32_t *) halSpiRxData, transferSize);
    }
</#if>
}

static void _RF215_HAL_SpiTransfer (
    RF215_SPI_TRANSFER_MODE mode,
    uint16_t regAddr,
    void* pData,
    size_t size,
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    bool fromTasks,
</#if>
    RF215_SPI_TRANSFER_CALLBACK callback,
    uintptr_t context
)
{
    bool dmaIntStatus<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, plcExtIntStatus</#if></#if>;
    RF215_SPI_TRANSFER_OBJ* transfer;
    RF215_SPI_TRANSFER_OBJ* transferPoolEdge;

    /* Critical region to avoid conflict in SPI transfer queue */
    dmaIntStatus = _RF215_HAL_DisableIntSources(<#if DRV_RF215_TXRX_TIME_SUPPORT == true>&timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, &plcExtIntStatus</#if></#if>);
    _RF215_HAL_ExtIntDisable();

    /* Look for a free transfer object in the pool */
    transferPoolEdge = &halSpiTransferPool[RF215_SPI_TRANSFER_POOL_SIZE];
    for (transfer = halSpiTransferPool; transfer < transferPoolEdge; transfer++)
    {
        if (transfer->inUse == false)
        {
            /* Copy transfer parameters */
            transfer->next = NULL;
            transfer->pData = pData;
            transfer->callback = callback;
            transfer->context = context;
            transfer->size = size;
            transfer->mode = mode;
            transfer->regAddr = regAddr;
            transfer->inUse = true;
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
            transfer->fromTasks = fromTasks;
</#if>

            if (rf215HalObj.spiQueueFirst == NULL)
            {
                /* No SPI transfers in the queue */
                rf215HalObj.spiQueueFirst = transfer;
                rf215HalObj.spiQueueLast = transfer;
            }
            else
            {
                /* Add SPI transfer to the queue */
                rf215HalObj.spiQueueLast->next = transfer;
                rf215HalObj.spiQueueLast = transfer;
            }

            break;
        }
    }

    /* External interrupt kept disabled until SPI transfer finishes */
    if (rf215HalObj.spiQueueFirst == transfer)
    {
        /* This transfer is the first in the queue so it can be started */
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
        if (fromTasks == false)
        {
            _RF215_HAL_SpiTransferStart(mode, regAddr, pData, size);
        }
        else
        {
            rf215HalObj.spiTransferFromTasks = true;
        }
<#else>
        _RF215_HAL_SpiTransferStart(mode, regAddr, pData, size);
</#if>
    }

    /* Leave critical region */
    _RF215_HAL_RestoreIntSources(dmaIntStatus<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, plcExtIntStatus</#if></#if>);
}

static void _RF215_HAL_SpiTransferFinished(RF215_SPI_TRANSFER_OBJ* transfer)
{
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    uint64_t callbackTime;
</#if>
    uintptr_t callbackContext;
    void* callbackData;
    RF215_SPI_TRANSFER_OBJ* next;
    RF215_SPI_TRANSFER_CALLBACK callback;

    if (transfer->mode == RF215_SPI_READ)
    {
        /* Copy SPI received data to buffer from upper layer */
        memcpy(transfer->pData, &halSpiRxData[RF215_SPI_CMD_SIZE], transfer->size);
    }

    /* Copy needed data to local variables */
    callback = transfer->callback;
    callbackContext = transfer->context;
    callbackData = transfer->pData;
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    callbackTime = rf215HalObj.sysTimeTransfer;
</#if>

    /* The transfer object can now be freed */
    transfer->inUse = false;
<#if DRV_RF215_PLIB == "SRV_SPISPLIT">
    rf215HalObj.dmaTransferInProgress = false;
</#if>

    /* Check next transfer */
    next = transfer->next;
    if (next != NULL)
    {
        /* Move queue start to next transfer */
        rf215HalObj.spiQueueFirst = next;
    }
    else
    {
        /* No more SPI transfers in the queue */
        rf215HalObj.spiQueueFirst = NULL;
        rf215HalObj.spiQueueLast = NULL;
    }

    if (next != NULL)
    {
        /* Start next SPI transfer */
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
        if (next->fromTasks == false)
        {
            _RF215_HAL_SpiTransferStart(next->mode, next->regAddr, next->pData, next->size);
        }
        else
        {
            rf215HalObj.spiTransferFromTasks = true;
        }
<#else>
        _RF215_HAL_SpiTransferStart(next->mode, next->regAddr, next->pData, next->size);
</#if>
    }

    if (callback != NULL)
    {
        /* Notify upper layer via callback */
        callback(callbackContext, callbackData<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, callbackTime</#if>);
    }

    /* External interrupt can now be enabled */
    _RF215_HAL_ExtIntEnable();
}

<#if core.DMA_ENABLE?has_content>
static void _RF215_HAL_SpiDmaHandler(SYS_DMA_TRANSFER_EVENT ev, uintptr_t ctxt)
{
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    bool dmaIntStatus, timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, plcExtIntStatus</#if>;
</#if>
    RF215_SPI_TRANSFER_OBJ* transfer = rf215HalObj.spiQueueFirst;
    SYS_DMA_CHANNEL dmaChannel = (SYS_DMA_CHANNEL) ctxt;
    bool restartTransfer = false;

    if (transfer == NULL)
    {
        /* Empty SPI transfer queue, probably because of RF215_HAL_Reset */
        return;
    }

    /* Critical region to avoid conflicts */
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    dmaIntStatus = _RF215_HAL_DisableIntSources(&timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, &plcExtIntStatus</#if>);
</#if>
    _RF215_HAL_ExtIntDisable();

<#if DRV_RF215_PLIB == "SRV_SPISPLIT">
    if ((rf215HalObj.dmaTransferInProgress == false) || (rf215HalObj.spiPlibIsBusy() == true))
    {
        /* DMA transfer not in progress or still in progress (shared SPI) */
        _RF215_HAL_ExtIntEnable();
        _RF215_HAL_RestoreIntSources(dmaIntStatus, timeIntStatus, plcExtIntStatus);
        return;
    }

</#if>
    if (ev == SYS_DMA_TRANSFER_ERROR)
    {
        if (dmaChannel == DRV_RF215_SPI_TX_DMA_CH)
        {
            /* Set DMA error flag */
            rf215HalObj.dmaTxError = true;
        }
        else /* DRV_RF215_SPI_RX_DMA_CH */
        {
            /* Restart SPI transfer */
            restartTransfer = true;
        }
    }
    else /* SYS_DMA_TRANSFER_COMPLETE */
    {
        if (dmaChannel == DRV_RF215_SPI_TX_DMA_CH)
        {
            /* Clear DMA error flag */
            rf215HalObj.dmaTxError = false;
        }
        else /* DRV_RF215_SPI_RX_DMA_CH */
        {
            if (rf215HalObj.dmaTxError == true)
            {
                /* Restart SPI transfer */
                restartTransfer = true;
            }
            else
            {
                /* SPI transfer finished successfully */
                _RF215_HAL_SpiTransferFinished(transfer);
            }
        }
    }

    if (restartTransfer == true)
    {
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
        if (transfer->fromTasks == false)
        {
            _RF215_HAL_SpiTransferStart(transfer->mode, transfer->regAddr, transfer->pData, transfer->size);
        }
        else
        {
            rf215HalObj.spiTransferFromTasks = true;
        }
<#else>
        _RF215_HAL_SpiTransferStart(transfer->mode, transfer->regAddr, transfer->pData, transfer->size);
</#if>
    }

    /* Leave critical region */
    _RF215_HAL_ExtIntEnable();
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    _RF215_HAL_RestoreIntSources(dmaIntStatus, timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, plcExtIntStatus</#if>);
</#if>
}
<#else>
static void _RF215_HAL_SpiDmaHandler(uintptr_t ctxt)
{
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    bool dmaIntStatus, timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, plcExtIntStatus</#if>;
</#if>
    RF215_SPI_TRANSFER_OBJ* transfer = rf215HalObj.spiQueueFirst;

    if (transfer == NULL)
    {
        /* Empty SPI transfer queue, probably because of RF215_HAL_Reset */
        return;
    }

    /* Critical region to avoid conflicts */
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    dmaIntStatus = _RF215_HAL_DisableIntSources(&timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, &plcExtIntStatus</#if>);
</#if>
    _RF215_HAL_ExtIntDisable();

<#if DRV_RF215_PLIB == "SRV_SPISPLIT">
    if ((rf215HalObj.dmaTransferInProgress == false) || (rf215HalObj.spiPlibIsBusy() == true))
    {
        /* DMA transfer not in progress or still in progress (shared SPI) */
        _RF215_HAL_ExtIntEnable();
        _RF215_HAL_RestoreIntSources(dmaIntStatus, timeIntStatus, plcExtIntStatus);
        return;
    }

</#if>
    /* SPI transfer finished successfully */
    _RF215_HAL_SpiTransferFinished(transfer);

    /* Leave critical region */
    _RF215_HAL_ExtIntEnable();
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    _RF215_HAL_RestoreIntSources(dmaIntStatus, timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, plcExtIntStatus</#if>);
</#if>
}
</#if>

static void _RF215_HAL_ExtIntHandler(PIO_PIN pin, uintptr_t context)
{
    /* Check if external interrupt is still active */
    if (SYS_PORT_PinRead((SYS_PORT_PIN)pin) == true)
    {
        DRV_RF215_ExtIntHandler();
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: RF215 Driver HAL Interface Implementation
// *****************************************************************************
// *****************************************************************************

<#if DRV_RF215_PLIB == "SRV_SPISPLIT">
<#--  Connected to SPI PLIB through SPI Splitter  -->
    <#assign SPI_PLIB = DRV_RF215_PLIB_SPISPLIT>
<#else>
<#--  Connected directly to SPI PLIB  -->
    <#assign SPI_PLIB = DRV_RF215_PLIB>
</#if>
void RF215_HAL_Initialize(const DRV_RF215_INIT * const init)
{
<#if DRV_RF215_SPI_NUM_CSR != 0>
    /* Configure SPI Chip Select behavior: Clear CSAAT/CSNAAT bits */
<#if SPI_PLIB?starts_with("FLEXCOM")>
    *(init->spiCSRegAddress) &= ~(FLEX_SPI_CSR_CSAAT_Msk | FLEX_SPI_CSR_CSNAAT_Msk);
<#else>
    *(init->spiCSRegAddress) &= ~(SPI_CSR_CSAAT_Msk | SPI_CSR_CSNAAT_Msk);
</#if>

</#if>
    /* Store interrupt sources */
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    rf215HalObj.sysTimeIntSource = init->sysTimeIntSource;
</#if>
    rf215HalObj.dmaIntSource = init->dmaIntSource;
<#if (drvPlcPhy)?? || (drvG3MacRt)??>
    rf215HalObj.plcExtIntSource = init->plcExtIntSource;
</#if>

<#if (DRV_RF215_PLIB == "SRV_SPISPLIT") || (core.DMA_ENABLE?has_content == false)>
    /* Store pointers to SPI PLIB functions */
</#if>
<#if (DRV_RF215_PLIB == "SRV_SPISPLIT") || (core.DMA_ENABLE?has_content == false)>
    rf215HalObj.spiPlibIsBusy = init->spiPlibIsBusy;
</#if>
<#if (DRV_RF215_PLIB == "SRV_SPISPLIT") && (DRV_RF215_SPI_NUM_CSR != 0)>
    rf215HalObj.spiPlibSetChipSelect = init->spiPlibSetChipSelect;
</#if>
<#if core.DMA_ENABLE?has_content == false>
    rf215HalObj.spiPlibWriteRead = init->spiPlibWriteRead;
</#if>
<#if (DRV_RF215_PLIB == "SRV_SPISPLIT") || (core.DMA_ENABLE?has_content == false)>

</#if>
<#if core.DMA_ENABLE?has_content>
    /* Store addresses of SPI transmit and receive registers */
    rf215HalObj.spiTxAddr = init->spiTransmitAddress;
    rf215HalObj.spiRxAddr = init->spiReceiveAddress;

    /* Register callback for SPI Transmit and Receive DMA */
    SYS_DMA_ChannelCallbackRegister(DRV_RF215_SPI_RX_DMA_CH,
            _RF215_HAL_SpiDmaHandler, DRV_RF215_SPI_RX_DMA_CH);
    SYS_DMA_ChannelCallbackRegister(DRV_RF215_SPI_TX_DMA_CH,
            _RF215_HAL_SpiDmaHandler, DRV_RF215_SPI_TX_DMA_CH);
<#else>
    /* Register callback for SPI transfer finished */
    init->spiPlibSetCallback(_RF215_HAL_SpiDmaHandler, 0);
</#if>

    /* Register callback for external interrupt pin */
    ${PIO_PLIB}_PinInterruptCallbackRegister((${PIO_PLIB}_PIN) DRV_RF215_EXT_INT_PIN,
            _RF215_HAL_ExtIntHandler, 0);

    /* Pin interrupt is disabled at initialization */
    rf215HalObj.extIntDisableCount = 1;
    rf215HalObj.firstReset = true;

    /* Zero initialization */
    rf215HalObj.spiQueueFirst = NULL;
    rf215HalObj.spiQueueLast = NULL;
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    rf215HalObj.spiTransferFromTasks = false;
</#if>
<#if core.DMA_ENABLE?has_content>
    rf215HalObj.dmaTxError = false;
</#if>
<#if DRV_RF215_PLIB == "SRV_SPISPLIT">
    rf215HalObj.dmaTransferInProgress = false;
</#if>
<#if DRV_RF215_USE_LED_RX == true>
    rf215HalObj.ledRxOnCount = 0;
</#if>
<#if DRV_RF215_USE_LED_TX == true>
    rf215HalObj.ledTxOnCount = 0;
</#if>
    for (uint8_t idx = 0; idx < RF215_SPI_TRANSFER_POOL_SIZE; idx++)
    {
        halSpiTransferPool[idx].inUse = false;
    }
}

void RF215_HAL_Deinitialize()
{
    bool dmaIntStatus<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, plcExtIntStatus</#if></#if>;

    /* Critical region to avoid conflict in SPI transfer queue */
    dmaIntStatus = _RF215_HAL_DisableIntSources(<#if DRV_RF215_TXRX_TIME_SUPPORT == true>&timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, &plcExtIntStatus</#if></#if>);
    _RF215_HAL_ExtIntDisable();

    /* Push reset pin */
    SYS_PORT_PinClear(DRV_RF215_RESET_PIN);

    /* Clear SPI transfer pool */
    rf215HalObj.spiQueueFirst = NULL;
    rf215HalObj.spiQueueLast = NULL;
    for (uint8_t idx = 0; idx < RF215_SPI_TRANSFER_POOL_SIZE; idx++)
    {
        halSpiTransferPool[idx].inUse = false;
    }

    /* Leave critical region. External interrupt disabled */
    _RF215_HAL_RestoreIntSources(dmaIntStatus<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, plcExtIntStatus</#if></#if>);
}

void RF215_HAL_Tasks()
{
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    if (rf215HalObj.spiTransferFromTasks == true)
    {
        RF215_SPI_TRANSFER_OBJ* transfer = rf215HalObj.spiQueueFirst;

        if (transfer != NULL)
        {
            bool dmaIntStatus<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, plcExtIntStatus</#if></#if>;

            /* Critical region to avoid new SPI transfers */
            dmaIntStatus = _RF215_HAL_DisableIntSources(<#if DRV_RF215_TXRX_TIME_SUPPORT == true>&timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, &plcExtIntStatus</#if></#if>);

            /* Start SPI transfer from tasks */
            _RF215_HAL_SpiTransferStart(transfer->mode, transfer->regAddr, transfer->pData, transfer->size);

            /* Leave critical region */
            _RF215_HAL_RestoreIntSources(dmaIntStatus<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, plcExtIntStatus</#if></#if>);
        }

        rf215HalObj.spiTransferFromTasks = false;
    }
</#if>
}

void RF215_HAL_Reset()
{
    SYS_TIME_HANDLE timeHandle;
    bool dmaIntStatus<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, plcExtIntStatus</#if></#if>;

    /* Critical region to avoid interrupts during reset */
    dmaIntStatus = _RF215_HAL_DisableIntSources(<#if DRV_RF215_TXRX_TIME_SUPPORT == true>&timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, &plcExtIntStatus</#if></#if>);
    if (rf215HalObj.firstReset == false)
    {
        _RF215_HAL_ExtIntDisable();
    }

    if (rf215HalObj.spiQueueFirst != NULL)
    {
<#if core.DMA_ENABLE?has_content>
        /* Abort SPI transfer in progress */
        SYS_DMA_ChannelDisable(DRV_RF215_SPI_TX_DMA_CH);
        SYS_DMA_ChannelDisable(DRV_RF215_SPI_RX_DMA_CH);
<#else>
        /* Wait to SPI transfer to finish */
        while(rf215HalObj.spiPlibIsBusy() == true);
</#if>
    }

    /* Push reset pin */
    SYS_PORT_PinClear(DRV_RF215_RESET_PIN);

    /* Clear SPI transfer pool. Pending SPI transfers aborted */
    rf215HalObj.spiQueueFirst = NULL;
    rf215HalObj.spiQueueLast = NULL;
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    rf215HalObj.spiTransferFromTasks = false;
</#if>
    rf215HalObj.firstReset = false;
    for (uint8_t idx = 0; idx < RF215_SPI_TRANSFER_POOL_SIZE; idx++)
    {
        halSpiTransferPool[idx].inUse = false;
    }

<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    /* Perform reset pulse delay (SYS_TIME interrupt has to be enabled) */
    SYS_INT_SourceEnable(rf215HalObj.sysTimeIntSource);
<#else>
    /* Perform reset pulse delay */
</#if>
    if (SYS_TIME_DelayUS(RF215_RST_PULSE_US, &timeHandle) == SYS_TIME_SUCCESS)
    {
        while (SYS_TIME_DelayIsComplete(timeHandle) == false);
    }

<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    /* Disable again SYS_TIME interrupt */
    SYS_INT_SourceDisable(rf215HalObj.sysTimeIntSource);

</#if>
    /* Release reset pin and enable/restore interrupts */
    SYS_PORT_PinSet(DRV_RF215_RESET_PIN);
    _RF215_HAL_ExtIntEnable();
    _RF215_HAL_RestoreIntSources(dmaIntStatus<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, timeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, plcExtIntStatus</#if></#if>);
}

bool RF215_HAL_SpiLock()
{
    /* Disable interrupts to avoid new SPI transfers */
    RF215_HAL_OBJ* hObj = &rf215HalObj;
    hObj->dmaIntStatus = _RF215_HAL_DisableIntSources(<#if DRV_RF215_TXRX_TIME_SUPPORT == true>&hObj->sysTimeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, &hObj->plcExtIntStatus</#if></#if>);
    _RF215_HAL_ExtIntDisable();

    if (hObj->spiQueueFirst == NULL)
    {
<#if DRV_RF215_PLIB == "SRV_SPISPLIT">
        /* Check SPI status */
        return !hObj->spiPlibIsBusy();
<#else>
        /* SPI is free */
        return true;
</#if>
    }
    else
    {
        /* SPI is busy */
        return false;
    }
}

void RF215_HAL_SpiUnlock()
{
    /* Restore interrupts */
    RF215_HAL_OBJ* hObj = &rf215HalObj;
    _RF215_HAL_ExtIntEnable();
    _RF215_HAL_RestoreIntSources(hObj->dmaIntStatus<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, hObj->sysTimeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, hObj->plcExtIntStatus</#if></#if>);
}

void RF215_HAL_EnterCritical()
{
<#if (DRV_RF215_TXRX_TIME_SUPPORT == false) && ((drvPlcPhy)?? || (drvG3MacRt)??)>
    bool intStatus = SYS_INT_Disable();

</#if>
    /* Critical region: Disable interrupts to avoid conflicts
     * External interrupt not disabled because it just makes one SPI transfer
     * and there is no other static variable update */
    RF215_HAL_OBJ* hObj = &rf215HalObj;
    hObj->dmaIntStatus = _RF215_HAL_DisableIntSources(<#if DRV_RF215_TXRX_TIME_SUPPORT == true>&hObj->sysTimeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, &hObj->plcExtIntStatus</#if></#if>);
<#if (DRV_RF215_TXRX_TIME_SUPPORT == false) && ((drvPlcPhy)?? || (drvG3MacRt)??)>

    SYS_INT_Restore(intStatus);
</#if>
}

void RF215_HAL_LeaveCritical()
{
<#if (DRV_RF215_TXRX_TIME_SUPPORT == false) && ((drvPlcPhy)?? || (drvG3MacRt)??)>
    bool intStatus = SYS_INT_Disable();

</#if>
    /* Leave critical region: Restore interrupts */
    RF215_HAL_OBJ* hObj = &rf215HalObj;
    _RF215_HAL_RestoreIntSources(hObj->dmaIntStatus<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, hObj->sysTimeIntStatus<#if (drvPlcPhy)?? || (drvG3MacRt)??>, hObj->plcExtIntStatus</#if></#if>);
<#if (DRV_RF215_TXRX_TIME_SUPPORT == false) && ((drvPlcPhy)?? || (drvG3MacRt)??)>

    SYS_INT_Restore(intStatus);
</#if>
}

void RF215_HAL_SpiRead (
    uint16_t addr,
    void* pData,
    size_t size,
    RF215_SPI_TRANSFER_CALLBACK cb,
    uintptr_t context
)
{
    _RF215_HAL_SpiTransfer(RF215_SPI_READ, addr, pData, size<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, false</#if>, cb, context);
}

<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
void RF215_HAL_SpiReadFromTasks (
    uint16_t addr,
    void* pData,
    size_t size,
    RF215_SPI_TRANSFER_CALLBACK cb,
    uintptr_t context
)
{
    _RF215_HAL_SpiTransfer(RF215_SPI_READ, addr, pData, size, true, cb, context);
}

</#if>
void RF215_HAL_SpiWrite (
    uint16_t addr,
    void* pData,
    size_t size
)
{
    _RF215_HAL_SpiTransfer(RF215_SPI_WRITE, addr, pData, size<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, false</#if>, NULL, 0);
}

void RF215_HAL_SpiWriteUpdate (
    uint16_t addr,
    uint8_t* pDataNew,
    uint8_t* pDataOld,
    size_t size
)
{
    uint8_t* pDataWrite = pDataOld;
    size_t sizeWrite = 0;
    size_t sizeSame = 0;
    uint16_t addrWrite = addr;

    for (uint8_t idx = 0; idx < size; idx++)
    {
        if (pDataNew[idx] != pDataOld[idx])
        {
            /* Different byte, needs to be written. Update static array to store
             * register values */
            pDataOld[idx] = pDataNew[idx];

            /* Split the SPI transfer only if it is worth: 3 consecutive bytes
             * not updated (2 SPI header bytes) */
            if (sizeSame > RF215_SPI_CMD_SIZE)
            {
                RF215_HAL_SpiWrite(addrWrite, pDataWrite, sizeWrite);
                sizeWrite = 0;
                sizeSame = 0;
            }

            /* Check start of SPI transfer block */
            if (sizeWrite == 0)
            {
                addrWrite = addr + idx;
                pDataWrite = &pDataOld[idx];
            }

            /* Update write counters */
            sizeWrite += (sizeSame + 1);
            sizeSame = 0;
        }
        else if (sizeWrite != 0)
        {
            /* Same byte and SPI transfer block started */
            sizeSame += 1;
        }
    }

    if (sizeWrite != 0)
    {
        /* Send the last SPI transfer */
        RF215_HAL_SpiWrite(addrWrite, pDataWrite, sizeWrite);
    }
}

<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
size_t RF215_HAL_GetSpiQueueSize()
{
    size_t queueSize = 0;
    RF215_SPI_TRANSFER_OBJ* transfer = rf215HalObj.spiQueueFirst;

    while (transfer != NULL)
    {
        queueSize += transfer->size + RF215_SPI_CMD_SIZE;
        transfer = transfer->next;
    }

    return queueSize;
}

</#if>
<#if DRV_RF215_USE_LED_RX == true>
void RF215_HAL_LedRx(bool on)
{
    if (on == true)
    {
        SYS_PORT_PinSet(DRV_RF215_LED_RX_PIN);
        rf215HalObj.ledRxOnCount++;
    }
    else
    {
        if (rf215HalObj.ledRxOnCount > 0)
        {
            rf215HalObj.ledRxOnCount--;
        }

        if (rf215HalObj.ledRxOnCount == 0)
        {
            SYS_PORT_PinClear(DRV_RF215_LED_RX_PIN);
        }
    }
}

</#if>
<#if DRV_RF215_USE_LED_TX == true>
void RF215_HAL_LedTx(bool on)
{
    if (on == true)
    {
        SYS_PORT_PinSet(DRV_RF215_LED_TX_PIN);
        rf215HalObj.ledTxOnCount++;
    }
    else
    {
        if (rf215HalObj.ledTxOnCount > 0)
        {
            rf215HalObj.ledTxOnCount--;
        }

        if (rf215HalObj.ledTxOnCount == 0)
        {
            SYS_PORT_PinClear(DRV_RF215_LED_TX_PIN);
        }
    }
}

</#if>