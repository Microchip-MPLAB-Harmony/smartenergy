/*******************************************************************************
  RF215 Driver Hardware Abstraction Layer Header File

  Company:
    Microchip Technology Inc.

  File Name:
    rf215_hal.h

  Summary:
    RF215 Driver Hardware Abstraction Layer Header File

  Description:
    The RF215 driver HAL (Hardware Abstraction Layer) manages the hardware
    peripherals used by the RF215 driver. This file provides the interface
    definition for the RF215 driver HAL.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2024, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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
//DOM-IGNORE-END

#ifndef RF215_HAL_H
#define RF215_HAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
#include "driver/rf215/drv_rf215_definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

/* RF215 SPI COMMAND definition (Table 4-4 in RF215 datasheet).
 * COMMAND is 16 bits, containing MODE (read / write) and ADDRESS.
 * COMMAND[15:14] = MODE[1:0]; COMMAND[13:0] = ADDRESS[13:0] */
#define RF215_SPI_CMD_MODE_POS        14U
#define RF215_SPI_CMD_MODE_READ       (0U << RF215_SPI_CMD_MODE_POS)
#define RF215_SPI_CMD_MODE_WRITE      (2U << RF215_SPI_CMD_MODE_POS)
#define RF215_SPI_CMD_SIZE            2U

<#if DRV_RF215_PLIB == "SRV_SPISPLIT">
<#--  Connected to SPI PLIB through SPI Splitter  -->
    <#assign SPI_PLIB = DRV_RF215_PLIB_SPISPLIT>
<#else>
<#--  Connected directly to SPI PLIB  -->
    <#assign SPI_PLIB = DRV_RF215_PLIB>
</#if>
<#if DRV_RF215_SPI_NUM_CSR != 0>
  <#--  PLIB with multiple CSR/NPCS. Configurable delay between transfers  -->
  <#if SPI_PLIB?contains("FLEXCOM")>
    <#assign PLIB_SPI_CSR = "${SPI_PLIB?lower_case}.FLEXCOM_SPI_CSR${DRV_RF215_SPI_NPCS}">
    <#assign PLIB_SPI_MASTER_CLOCK = "core.${SPI_PLIB}_CLOCK_FREQUENCY">
  <#else>
    <#assign PLIB_SPI_CSR = "${SPI_PLIB?lower_case}.SPI_CSR${DRV_RF215_SPI_NPCS}">
    <#assign PLIB_SPI_MASTER_CLOCK = "${SPI_PLIB?lower_case}.SPI_MASTER_CLOCK">
  </#if>
  <#assign PLIB_SPI_CSR_SCBR = "${PLIB_SPI_CSR}_SCBR_VALUE">
  <#assign PLIB_SPI_CSR_DLYBCT = "${PLIB_SPI_CSR}_DLYBCT">
  <#assign PLIB_SPI_BYTE_CYCLES = ((PLIB_SPI_CSR_SCBR?eval * 8) + (PLIB_SPI_CSR_DLYBCT?eval * 32))>
<#else>
  <#if SPI_PLIB?contains("USART")>
    <#assign PLIB_SPI_MASTER_CLOCK = "${SPI_PLIB?lower_case}.USART_CLOCK_FREQ">
    <#assign PLIB_SPI_BYTE_CYCLES = (.vars["${SPI_PLIB?lower_case}"].USART_SPI_BRG_VALUE * 8)>
  <#elseif SPI_PLIB?contains("SERCOM")>
    <#assign PLIB_SPI_MASTER_CLOCK = "core.${SPI_PLIB}_CORE_CLOCK_FREQUENCY">
    <#assign PLIB_SPI_BYTE_CYCLES = ((.vars["${SPI_PLIB?lower_case}"].SPI_BAUD_REG_VALUE + 1) * 2 * 8)>
  <#else>
    <#assign PLIB_SPI_MASTER_CLOCK = "core.${SPI_PLIB}_CLOCK_FREQUENCY">
    <#assign PLIB_SPI_BYTE_CYCLES = ((.vars["${SPI_PLIB?lower_case}"].SPI_BRG_VALUE + 1) * 2 * 8)>
  </#if>
</#if>
/* SPI transfer duration for one byte in us with 5 decimal bits (equivalent to
   cycles of 32MHz, which is the frequency of RF215 counter) */
#define RF215_SPI_BYTE_DURATION_US_Q5 ${(((PLIB_SPI_BYTE_CYCLES * 1000000 * 32) / PLIB_SPI_MASTER_CLOCK?eval)?round)?int}U

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* RF215 HAL SPI Transfer Handler Function Pointer

  Summary
    Pointer to a RF215 HAL SPI transfer handler function, called back when SPI
    transfer finishes.
*/

typedef void ( *RF215_SPI_TRANSFER_CALLBACK )( uintptr_t context, void* pData<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, uint64_t time</#if> );

// *****************************************************************************
/* RF215 HAL SPI Transfer Mode

  Summary:
    Available SPI transfer modes in RF215 HAL.
*/

typedef enum
{
    RF215_SPI_READ  = RF215_SPI_CMD_MODE_READ,
    RF215_SPI_WRITE = RF215_SPI_CMD_MODE_WRITE,

} RF215_SPI_TRANSFER_MODE;

// *****************************************************************************
/* RF215 Driver HAL SPI Transfer object

  Summary:
    Object used to keep any data required for a SPI transfer.
*/

typedef struct RF215_SPI_TRANSFER_OBJ_tag
{
    struct RF215_SPI_TRANSFER_OBJ_tag* next;
    void*                              pData;
    RF215_SPI_TRANSFER_CALLBACK        callback;
    uintptr_t                          context;
    size_t                             size;
    RF215_SPI_TRANSFER_MODE            mode;
    uint16_t                           regAddr;
    bool                               inUse;
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    bool                               fromTasks;
</#if>
} RF215_SPI_TRANSFER_OBJ;

// *****************************************************************************
/* RF215 Driver HAL Instance Object

  Summary:
    Object used to keep any data required for an instance of the RF215 driver
    HAL.
*/

typedef struct
{
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    /* SYS_TIME counter captured when SPI transfer is launched */
    uint64_t                        sysTimeTransfer;

</#if>
    /* Pointer to first element of SPI transfer queue */
    RF215_SPI_TRANSFER_OBJ*         spiQueueFirst;

    /* Pointer to last element of SPI transfer queue */
    RF215_SPI_TRANSFER_OBJ*         spiQueueLast;

<#if (DRV_RF215_PLIB == "SRV_SPISPLIT") || (core.DMA_ENABLE?has_content == false)>
    /* Pointer to SPI PLIB is busy funcition */
    DRV_RF215_PLIB_SPI_IS_BUSY      spiPlibIsBusy;

</#if>
<#if (DRV_RF215_PLIB == "SRV_SPISPLIT") && (DRV_RF215_SPI_NUM_CSR != 0)>
    /* Pointer to SPI PLIB chip select funcition */
    DRV_RF215_PLIB_SPI_SET_CS       spiPlibSetChipSelect;

</#if>
<#if core.DMA_ENABLE?has_content>
    /* SPI transmit register address used for DMA operation */
    const void*                     spiTxAddr;

    /* SPI receive register address used for DMA operation */
    const void*                     spiRxAddr;

    /* Flag to indicate error in DMA for SPI transmit */
    bool                            dmaTxError;
<#else>
    /* Pointer to SPI Write and Read function */
    DRV_RF215_PLIB_SPI_WRITE_READ   spiPlibWriteRead;
</#if>

<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    /* Interrupt source ID for the SYS_TIME interrupt */
    INT_SOURCE                      sysTimeIntSource;

    /* Status of the SYS_TIME interrupt */
    bool                            sysTimeIntStatus;

</#if>
    /* Interrupt source ID for the DMA interrupt */
    INT_SOURCE                      dmaIntSource;

    /* Status of the DMA interrupt */
    bool                            dmaIntStatus;

<#if (drvPlcPhy)?? || (drvG3MacRt)??>
    /* Interrupt source ID for PLC external interrupt */
    INT_SOURCE                      plcExtIntSource;

    /* Status of the PLC external interrupt */
    bool                            plcExtIntStatus;

</#if>
    /* First reset flag */
    bool                            firstReset;

<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    /* SPI transfer from tasks flag */
    bool                            spiTransferFromTasks;

</#if>
    /* External interrupt disable counter */
    uint8_t                         extIntDisableCount;

<#if DRV_RF215_PLIB == "SRV_SPISPLIT">
    /* DMA transfer in progress flag */
    bool                            dmaTransferInProgress;

</#if>
<#if DRV_RF215_USE_LED_RX == true>
    /* LED RX counter ON */
    uint8_t                         ledRxOnCount;

</#if>
<#if DRV_RF215_USE_LED_TX == true>
    /* LED TX counter ON */
    uint8_t                         ledTxOnCount;

</#if>
} RF215_HAL_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: RF215 Driver HAL Interface
// *****************************************************************************
// *****************************************************************************

void RF215_HAL_Initialize(const DRV_RF215_INIT * const init);

void RF215_HAL_Deinitialize(void);

void RF215_HAL_Reset(void);

<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
void RF215_HAL_Tasks(void);

bool RF215_HAL_SpiLock(void);

void RF215_HAL_SpiUnlock(void);

</#if>
void RF215_HAL_EnterCritical(void);

void RF215_HAL_LeaveCritical(void);

void RF215_HAL_SpiRead (
    uint16_t addr,
    void* pData,
    size_t size,
    RF215_SPI_TRANSFER_CALLBACK cb,
    uintptr_t context
);

<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
void RF215_HAL_SpiReadFromTasks (
    uint16_t addr,
    void* pData,
    size_t size,
    RF215_SPI_TRANSFER_CALLBACK cb,
    uintptr_t context
);

</#if>
void RF215_HAL_SpiWrite (
    uint16_t addr,
    void* pData,
    size_t size
);

void RF215_HAL_SpiWriteUpdate (
    uint16_t addr,
    uint8_t* pDataNew,
    uint8_t* pDataOld,
    size_t size
);

<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
size_t RF215_HAL_GetSpiQueueSize(void);

</#if>
<#if DRV_RF215_USE_LED_RX == true>
void RF215_HAL_LedRx(bool on);

</#if>
<#if DRV_RF215_USE_LED_TX == true>
void RF215_HAL_LedTx(bool on);

</#if>
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // #ifndef RF215_HAL_H
