/******************************************************************************
  DRV_PL360 Hardware Abstraction Layer

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360_hal.c

  Summary:
    PL360 Driver Hardware Abstraction Layer

  Description:
    The PL360 Library provides a Hardware Abstraction Layer.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "configuration.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global objects
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: DRV_PL360_HAL Local Functions
// *****************************************************************************
// *****************************************************************************
/** SPI Header size. */
#define HAL_SPI_HEADER_SIZE      4
/** SPI Max Msg_Data size. */
#define HAL_SPI_MSG_DATA_SIZE    512
/** SPI Max Msg_Data size. */
#define HAL_SPI_MSG_PARAMS_SIZE  118   /* Worst case = 118: sizeof(rx_msg_t) [G3] */
/** PDC buffer us_size. */
#define HAL_SPI_BUFFER_SIZE      (HAL_SPI_HEADER_SIZE + HAL_SPI_MSG_DATA_SIZE + HAL_SPI_MSG_PARAMS_SIZE)

/* PDC Receive buffer */
static uint8_t sRxSpiData[HAL_SPI_BUFFER_SIZE];
/* PDC Transmission buffer */
static uint8_t sTxSpiData[HAL_SPI_BUFFER_SIZE];

/* Static pointer to PLIB interface used to handle PL360 */
static DRV_PL360_PLIB_INTERFACE *sPl360Plib;

void _delay(uint64_t n)
{
    (void)n;
    
    __asm (
        "loop: DMB  \n"
        "SUBS R0, R0, #1  \n"
        "BNE.N loop         "
    );
}

// *****************************************************************************
// *****************************************************************************
// Section: DRV_PL360_HAL Global Functions
// *****************************************************************************
// *****************************************************************************
void drv_pl360_hal_init(DRV_PL360_PLIB_INTERFACE *pl360Plib)
{
    sPl360Plib = pl360Plib;   
    
    /* Enable External Interrupt Source */
    SYS_INT_SourceEnable(DRV_PL360_EXT_INT_SRC);
    /* Disable External Pin Interrupt */
    PIO_PinInterruptDisable(DRV_PL360_EXT_INT_PIN);
}

void drv_pl360_hal_setup(bool set16Bits)
{
    SPI_TRANSFER_SETUP spiPlibSetup;
        
    if (set16Bits) 
    {
        spiPlibSetup.dataBits = SPI_DATA_BITS_16;
        SYS_DMA_DataWidthSetup(sPl360Plib->dmaChannelTx, SYS_DMA_WIDTH_16_BIT);
        SYS_DMA_DataWidthSetup(sPl360Plib->dmaChannelRx, SYS_DMA_WIDTH_16_BIT);
    }
    else
    {
        spiPlibSetup.dataBits = SPI_DATA_BITS_8;
        SYS_DMA_DataWidthSetup(sPl360Plib->dmaChannelTx, SYS_DMA_WIDTH_8_BIT);
        SYS_DMA_DataWidthSetup(sPl360Plib->dmaChannelRx, SYS_DMA_WIDTH_8_BIT);
    }
    
    /* Configure SPI PLIB for Bootloader */
    spiPlibSetup.clockFrequency = 8000000;
    spiPlibSetup.clockPhase = SPI_CLOCK_PHASE_LEADING_EDGE;
    spiPlibSetup.clockPolarity = SPI_CLOCK_POLARITY_IDLE_LOW;    
    sPl360Plib->spiPlibTransferSetup((uintptr_t)&spiPlibSetup, 0);
    
    /* Configure DMA for Bootloader */
    SYS_DMA_AddressingModeSetup(sPl360Plib->dmaChannelTx, SYS_DMA_SOURCE_ADDRESSING_MODE_INCREMENTED, SYS_DMA_DESTINATION_ADDRESSING_MODE_FIXED);
    SYS_DMA_AddressingModeSetup(sPl360Plib->dmaChannelRx, SYS_DMA_SOURCE_ADDRESSING_MODE_FIXED, SYS_DMA_DESTINATION_ADDRESSING_MODE_INCREMENTED);
    
}

void drv_pl360_hal_reset(void)
{
    /* Disable LDO pin */
    SYS_PORT_PinClear(sPl360Plib->ldoPin);
    /* Enable Reset Pin */
    SYS_PORT_PinClear(sPl360Plib->resetPin);

    /* Wait to PL360 startup (500us) */
    drv_pl360_hal_delay(500);

    /* Enable LDO pin */
    SYS_PORT_PinSet(sPl360Plib->ldoPin);

    /* Wait to PL360 LDO enable (500us) */
    drv_pl360_hal_delay(500);

    /* Disable Reset pin */
    SYS_PORT_PinSet(sPl360Plib->resetPin);

    /* Wait to PL360 startup (500us) */
    drv_pl360_hal_delay(500);
}

bool drv_pl360_hal_get_cd(void)
{
    return false;
}

void drv_pl360_hal_delay(uint64_t delayUs)
{    
    _delay((delayUs * 300000000 + (uint64_t)(5.932e6 - 1ul)) / (uint64_t)5.932e6);
}

void drv_pl360_hal_enable_interrupt(bool enable)
{
    if (enable)
    {
        SYS_INT_SourceStatusClear(DRV_PL360_EXT_INT_SRC);
        PIO_PinInterruptEnable(DRV_PL360_EXT_INT_PIN);
    }
    else
    {
        PIO_PinInterruptDisable(DRV_PL360_EXT_INT_PIN);
    }
}

bool drv_pl360_hal_send_boot_cmd(uint16_t cmd, uint32_t addr, uint32_t dataLength, uint8_t *pDataWr, uint8_t *pDataRd)
{
    uint8_t *pTxData;
    size_t size;
    
    while(SYS_DMA_ChannelIsBusy(sPl360Plib->dmaChannelTx));
    while(SYS_DMA_ChannelIsBusy(sPl360Plib->dmaChannelRx));
    
    pTxData = sTxSpiData;
    
    /* Build command */
    memcpy(pTxData, &addr, 4);
    pTxData += 4;
    memcpy(pTxData, &cmd, 2);
    pTxData += 2;
    if (dataLength)
    {
        if (dataLength > HAL_SPI_BUFFER_SIZE)
        {
            dataLength = HAL_SPI_BUFFER_SIZE;
        }
        
        if (pDataWr) {
            memcpy(pTxData, pDataWr, dataLength);
        }
        else{
            /* Insert dummy data */
            memset(pTxData, 0, dataLength);
        }
        
        pTxData += dataLength;
    }
    /* Get length of DMA transaction in bytes */
    size = pTxData - sTxSpiData;
       
    if (DATA_CACHE_ENABLED == true)
    {
        /* Invalidate cache lines having received buffer before using it
         * to load the latest data in the actual memory to the cache */
        DCACHE_CLEAN_BY_ADDR((uint32_t *)sTxSpiData, HAL_SPI_BUFFER_SIZE);
        DCACHE_INVALIDATE_BY_ADDR((uint32_t *)sRxSpiData, HAL_SPI_BUFFER_SIZE);
    }
    
    SYS_DMA_ChannelTransfer (sPl360Plib->dmaChannelRx, (const void *)sPl360Plib->spiAddressRx, (const void *)sRxSpiData, size);
    SYS_DMA_ChannelTransfer (sPl360Plib->dmaChannelTx, (const void *)sTxSpiData, (const void *)sPl360Plib->spiAddressTx, size);
    
    if (pDataRd) {
        while(SYS_DMA_ChannelIsBusy(sPl360Plib->dmaChannelRx));
        
        /* Update data received */
        memcpy(pDataRd, &sRxSpiData[6], dataLength);
    }
    
    return true;
}

bool drv_pl360_hal_send_wrrd_cmd(DRV_PL360_HAL_CMD *pCmd, DRV_PL360_HAL_INFO *pInfo)
{
    uint8_t *pTxData;
    size_t cmdSize;
    size_t dataLength;
    
    while(SYS_DMA_ChannelIsBusy(sPl360Plib->dmaChannelTx));
    while(SYS_DMA_ChannelIsBusy(sPl360Plib->dmaChannelRx));
    
    pTxData = sTxSpiData;
    
    dataLength = ((pCmd->length + 1) >> 1) & 0x7FFF;
    
    /* Protect length */
    if ((dataLength == 0) || (dataLength > (HAL_SPI_MSG_DATA_SIZE + HAL_SPI_MSG_PARAMS_SIZE)))
    {
        return false;
    }
    
    /* Join CMD and Length */
    dataLength |= pCmd->cmd;
    
    /* Build command */
    /* Address */
    *pTxData++ = (uint8_t)(pCmd->memId);
    *pTxData++ = (uint8_t)(pCmd->memId >> 8);
    *pTxData++ = (uint8_t)(dataLength);
    *pTxData++ = (uint8_t)(dataLength >> 8);

    if (pCmd->cmd == DRV_PL360_HAL_CMD_WR) {
        /* Fill with transmission data */
        memcpy(pTxData, pCmd->pData, pCmd->length);
    } else {
        /* Fill with dummy data */
        memset(pTxData, 0, pCmd->length);
    }

    pTxData += pCmd->length;

    cmdSize = pTxData - sTxSpiData;
    
    if (cmdSize % 2) {
        *pTxData++ = 0;
        cmdSize++;
    }
       
    if (DATA_CACHE_ENABLED == true)
    {
        /* Invalidate cache lines having received buffer before using it
         * to load the latest data in the actual memory to the cache */
        DCACHE_CLEAN_BY_ADDR((uint32_t *)sTxSpiData, HAL_SPI_BUFFER_SIZE);
        DCACHE_INVALIDATE_BY_ADDR((uint32_t *)sRxSpiData, HAL_SPI_BUFFER_SIZE);
    }
    
    SYS_DMA_ChannelTransfer (sPl360Plib->dmaChannelRx, (const void *)sPl360Plib->spiAddressRx, (const void *)sRxSpiData, cmdSize >> 1);
    SYS_DMA_ChannelTransfer (sPl360Plib->dmaChannelTx, (const void *)sTxSpiData, (const void *)sPl360Plib->spiAddressTx, cmdSize >> 1);
    
    if (pCmd->cmd == DRV_PL360_HAL_CMD_RD) {
        while(SYS_DMA_ChannelIsBusy(sPl360Plib->dmaChannelTx));
        while(SYS_DMA_ChannelIsBusy(sPl360Plib->dmaChannelRx));
        
        /* Update data received */
        memcpy(pCmd->pData, &sRxSpiData[4], pCmd->length);
    }
    
    /* Get HAL info */
    pInfo->key = DRV_PL360_HAL_KEY(sRxSpiData[0], sRxSpiData[1]);
    if (pInfo->key == DRV_PL360_HAL_KEY_CORTEX)
    {
        pInfo->flags = DRV_PL360_HAL_FLAGS_CORTEX(sRxSpiData[2], sRxSpiData[3]);
    } 
    else if (pInfo->key == DRV_PL360_HAL_KEY_BOOT)
    {
        pInfo->flags = DRV_PL360_HAL_FLAGS_BOOT(sRxSpiData[0], sRxSpiData[2], sRxSpiData[3]);
    } 
    else 
    {
        pInfo->flags = 0;
    }
    
    return true;
}
