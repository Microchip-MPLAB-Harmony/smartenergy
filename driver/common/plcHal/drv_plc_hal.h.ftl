/*******************************************************************************
  DRV_PLC Hardware Abstraction Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_plc_hal.h

  Summary:
    PLC Driver Hardware Abstraction Layer Header File

  Description:
    The PLC Library provides a Hardware Abstraction Layer.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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

#ifndef DRV_PLC_HAL_H
#define DRV_PLC_HAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdbool.h>
#include "system/ports/sys_ports.h"
<#if core.DMA_ENABLE?has_content>
#include "system/dma/sys_dma.h"
</#if>
<#if DRV_PLC_PLIB == "SRV_SPISPLIT">
<#--  Connected to SPI PLIB through SPI Splitter  -->
<#if (drvRf215)??>
#include "system/int/sys_int.h"
</#if>
    <#assign SPI_PLIB = DRV_PLC_PLIB_SPISPLIT>
<#else>
<#--  Connected directly to SPI PLIB  -->
    <#assign SPI_PLIB = DRV_PLC_PLIB>
</#if>
<#if SPI_PLIB?lower_case[0..*6] == "sercom">
    <#assign SPI_PREFFIX = "SPI">
#include "peripheral/sercom/spi_master/plib_sercom_spi_master_common.h"
#include "peripheral/eic/plib_eic.h"
<#elseif SPI_PLIB?lower_case[0..*7] == "flexcom">
    <#assign SPI_PREFFIX = "FLEXCOM_SPI">
#include "peripheral/flexcom/spi/master/plib_flexcom_spi_master_common.h"
<#elseif SPI_PLIB?lower_case[0..*3] == "spi">
    <#assign SPI_PREFFIX = "SPI">
#include "peripheral/spi/spi_master/plib_spi_master_common.h"
</#if>

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
#define DRV_PLC_HAL_CPU_CLOCK_FREQ            ${core.CPU_CLOCK_FREQUENCY}

#define DRV_PLC_HAL_CMD_POS                   15
#define DRV_PLC_HAL_CMD_RD                    ((uint16_t)0U << DRV_PLC_HAL_CMD_POS)
#define DRV_PLC_HAL_CMD_WR                    ((uint16_t)1U << DRV_PLC_HAL_CMD_POS)

#define DRV_PLC_HAL_LEN_MASK                  0x7FFF
        
/* SPI Key MASK */
#define DRV_PLC_HAL_KEY_MASK                  0xFFFEU
/* SPI Key when bootloader is running in PLC transceiver */
#define DRV_PLC_HAL_KEY_BOOT                  (0x5634U & DRV_PLC_HAL_KEY_MASK)
/* SPI Key when PLC firmware is running in PLC transceiver */
#define DRV_PLC_HAL_KEY_CORTEX                (0x${DRV_PLC_CORE_KEY}U & DRV_PLC_HAL_KEY_MASK)
        
#define DRV_PLC_HAL_KEY(b0, b1)               ((((uint16_t)(b1) << 8) + (b0)) & DRV_PLC_HAL_KEY_MASK)
#define DRV_PLC_HAL_FLAGS_BOOT(b0, b2, b3)    ((((uint32_t)(b3)) << 8) + ((uint32_t)(b2)) + (((uint32_t)(b0) & 0x01UL) << 16))
#define DRV_PLC_HAL_FLAGS_CORTEX(b2, b3)      ((((uint32_t)(b3)) << 8) + ((uint32_t)(b2)))        

/* User rest flag in bootloader key*/
#define DRV_PLC_HAL_FLAG_RST_USER             0x00010000UL
/* Cortex(debugger) rest flag in bootloader key*/
#define DRV_PLC_HAL_FLAG_RST_CORTEX           0x00008000UL
/* Watch Dog flag in bootloader key */
#define DRV_PLC_HAL_FLAG_RST_WDOG             0x00004000UL
/* Power-ON reset is indicated when the three flags are 0, mask will be used to detect it*/
#define DRV_PLC_HAL_FLAG_RST_PON              0x0001C000UL

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef bool (* DRV_PLC_SPI_PLIB_TRANSFER_SETUP)(uintptr_t setup, uint32_t spiSourceClock);
<#if core.DMA_ENABLE?has_content == false>
typedef bool (* DRV_PLC_SPI_WRITE_READ)( void * pTransmitData, size_t txSize, 
        void * pReceiveData, size_t rxSize );
</#if>
<#if (core.DMA_ENABLE?has_content == false) || (DRV_PLC_PLIB == "SRV_SPISPLIT")>
typedef bool (* DRV_PLC_SPI_ISBUSY)( void );
</#if>
<#if (DRV_PLC_PLIB == "SRV_SPISPLIT") && (DRV_PLC_SPI_NUM_CSR != 0)>
typedef void (* DRV_PLC_SPI_SET_CS)( ${SPI_PREFFIX}_CHIP_SELECT chipSelect );
</#if>

typedef enum
{
    DRV_PLC_SPI_CLOCK_PHASE_TRAILING_EDGE = ${SPI_PREFFIX}_CLOCK_PHASE_TRAILING_EDGE,
    DRV_PLC_SPI_CLOCK_PHASE_LEADING_EDGE = ${SPI_PREFFIX}_CLOCK_PHASE_LEADING_EDGE,

    /* Force the compiler to reserve 32-bit space for each enum value */
    DRV_PLC_SPI_CLOCK_PHASE_INVALID = ${SPI_PREFFIX}_CLOCK_PHASE_INVALID

} DRV_PLC_SPI_CLOCK_PHASE;

/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 5.2 deviated once.  Deviation record ID - H3_MISRAC_2012_R_5_2_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block deviate "MISRA C-2012 Rule 5.2" "H3_MISRAC_2012_R_5_2_DR_1"
</#if>

typedef enum
{
    DRV_PLC_SPI_CLOCK_POLARITY_IDLE_LOW = ${SPI_PREFFIX}_CLOCK_POLARITY_IDLE_LOW,
    DRV_PLC_SPI_CLOCK_POLARITY_IDLE_HIGH = ${SPI_PREFFIX}_CLOCK_POLARITY_IDLE_HIGH,

    /* Force the compiler to reserve 32-bit space for each enum value */
    DRV_PLC_SPI_CLOCK_POLARITY_INVALID = ${SPI_PREFFIX}_CLOCK_POLARITY_INVALID

} DRV_PLC_SPI_CLOCK_POLARITY;

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 5.2"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRA C-2012 deviation block end */

typedef enum
{
<#if SPI_PLIB?lower_case[0..*6] == "sercom">
    DRV_PLC_SPI_DATA_BITS_8 = ${SPI_PREFFIX}_DATA_BITS_8,
    DRV_PLC_SPI_DATA_BITS_9 = ${SPI_PREFFIX}_DATA_BITS_9,
<#else>
    DRV_PLC_SPI_DATA_BITS_8 = ${SPI_PREFFIX}_DATA_BITS_8,
    DRV_PLC_SPI_DATA_BITS_9 = ${SPI_PREFFIX}_DATA_BITS_9, 
    DRV_PLC_SPI_DATA_BITS_10 = ${SPI_PREFFIX}_DATA_BITS_10,
    DRV_PLC_SPI_DATA_BITS_11 = ${SPI_PREFFIX}_DATA_BITS_11,
    DRV_PLC_SPI_DATA_BITS_12 = ${SPI_PREFFIX}_DATA_BITS_12,
    DRV_PLC_SPI_DATA_BITS_13 = ${SPI_PREFFIX}_DATA_BITS_13,
    DRV_PLC_SPI_DATA_BITS_14 = ${SPI_PREFFIX}_DATA_BITS_14,
    DRV_PLC_SPI_DATA_BITS_15 = ${SPI_PREFFIX}_DATA_BITS_15,
    DRV_PLC_SPI_DATA_BITS_16 = ${SPI_PREFFIX}_DATA_BITS_16,
</#if>

    /* Force the compiler to reserve 32-bit space for each enum value */
    DRV_PLC_SPI_DATA_BITS_INVALID = ${SPI_PREFFIX}_DATA_BITS_INVALID

} DRV_PLC_SPI_DATA_BITS;

typedef struct
{
    uint32_t    clockFrequency;
    DRV_PLC_SPI_CLOCK_PHASE clockPhase;
    DRV_PLC_SPI_CLOCK_POLARITY clockPolarity;
    DRV_PLC_SPI_DATA_BITS   dataBits;

} DRV_PLC_SPI_TRANSFER_SETUP;

// *****************************************************************************
/* PLC Driver PLIB Interface Data

  Summary:
    Defines the data required to initialize the PLC driver PLIB Interface.

  Description:
    This data type defines the data required to initialize the PLC driver
    PLIB Interface.

  Remarks:
    None.
*/

typedef struct
{  
    /* PLC SPI PLIB Transfer Setup */
    DRV_PLC_SPI_PLIB_TRANSFER_SETUP        spiPlibTransferSetup;

<#if (core.DMA_ENABLE?has_content == false) || (DRV_PLC_PLIB == "SRV_SPISPLIT")>
    /* SPI Is Busy */
    DRV_PLC_SPI_ISBUSY                     spiIsBusy;

</#if>
<#if (DRV_PLC_PLIB == "SRV_SPISPLIT") && (DRV_PLC_SPI_NUM_CSR != 0)>
    /* SPI Set Chip Select */
    DRV_PLC_SPI_SET_CS                     spiSetChipSelect;

</#if>
<#if core.DMA_ENABLE?has_content>
    /* SPI transmit DMA channel. */
    SYS_DMA_CHANNEL                        dmaChannelTx;

    /* SPI receive DMA channel. */
    SYS_DMA_CHANNEL                        dmaChannelRx;

    /* SPI transmit register address used for DMA operation. */
    void                                   *spiAddressTx;

    /* SPI receive register address used for DMA operation. */
    void                                   *spiAddressRx;

<#else>
    /* SPI Write/Read */
    DRV_PLC_SPI_WRITE_READ                 spiWriteRead;

</#if>
<#if SPI_PLIB?lower_case[0..*6] == "sercom">
    /* SPI Chip select pin */
    SYS_PORT_PIN                           spiCSPin;
    
</#if>
    /* SPI clock frequency */
    uint32_t                               spiClockFrequency;

    /* PLC LDO enable pin */
    SYS_PORT_PIN                           ldoPin;

    /* PLC reset pin */
    SYS_PORT_PIN                           resetPin;

<#if SPI_PLIB?lower_case[0..*6] == "sercom">
    EIC_PIN                                extIntPin;
<#else>
    /* PLC external interrupt pin */
    SYS_PORT_PIN                           extIntPin;
</#if>

    /* PLC external interrupt pio */
    SYS_PORT_PIN                           extIntPio;

<#if DRV_PLC_MODE == "PL460"> 
    /* PLC Tx Enable pin */
    SYS_PORT_PIN                           txEnablePin;

</#if>
<#if DRV_PLC_SLEEP_MODE == true>
    /* PLC StandBy Pin */
    SYS_PORT_PIN                           stByPin;

</#if>
<#if DRV_PLC_MODE == "PL460" && DRV_PLC_THERMAL_MONITOR == true>   
    /* PLC Thermal Monitor pin */
    SYS_PORT_PIN                           thMonPin;

</#if>
<#if (DRV_PLC_PLIB == "SRV_SPISPLIT") && (drvRf215)??>
    /* Interrupt source ID for RF external interrupt */
    INT_SOURCE                             rfExtIntSource;

    /* Interrupt source ID for DMA */
    INT_SOURCE                             dmaIntSource;

<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
    /* Interrupt source ID for SYS_TIME */
    INT_SOURCE                             sysTimeIntSource;

</#if>
</#if>
} DRV_PLC_PLIB_INTERFACE;

// *****************************************************************************

typedef void (* DRV_PLC_HAL_INIT)(DRV_PLC_PLIB_INTERFACE *plcPlib);

typedef void (* DRV_PLC_HAL_SETUP)(bool set16Bits);

typedef void (* DRV_PLC_HAL_RESET)(void);

<#if DRV_PLC_SLEEP_MODE == true>
typedef void (* DRV_PLC_HAL_SET_STBY)(bool enable);

</#if>
<#if DRV_PLC_MODE == "PL460" && DRV_PLC_THERMAL_MONITOR == true>
typedef bool (* DRV_PLC_HAL_GET_THMON)(void);

</#if>
<#if DRV_PLC_MODE == "PL460">
typedef void (* DRV_PLC_HAL_SET_TXENABLE)(bool enable);

</#if>
typedef void (* DRV_PLC_HAL_ENABLE_EXT_INT)(bool enable);

typedef bool (* DRV_PLC_HAL_GET_PIN_LEVEL)(SYS_PORT_PIN pin);

typedef void (* DRV_PLC_HAL_DELAY)(uint32_t delay);

typedef void (* DRV_PLC_HAL_SEND_BOOT_CMD)(uint16_t cmd, uint32_t addr, uint32_t dataLength, void *pDataWr, void *pDataRd);

typedef void (* DRV_PLC_HAL_SEND_WRRD_CMD)(void *pCmd, void *pInfo);

// *****************************************************************************
/* PLC Driver HAL Interface Data

  Summary:
    Defines the data required to initialize the PLC driver HAL Interface.

  Description:
    This data type defines the data required to initialize the PLC driver
    HAL Interface.

  Remarks:
    None.
*/

typedef struct
{
    /* PLC PLIB Interface */
    DRV_PLC_PLIB_INTERFACE                   *plcPlib;

    /* PLC HAL init */
    DRV_PLC_HAL_INIT                         init;

    /* PLC HAL setup */
    DRV_PLC_HAL_SETUP                        setup;

    /* PLC HAL reset device */
    DRV_PLC_HAL_RESET                        reset;

<#if DRV_PLC_SLEEP_MODE == true>        
    /* PLC low power management */
    DRV_PLC_HAL_SET_STBY                     setStandBy;

</#if>
<#if DRV_PLC_MODE == "PL460" && DRV_PLC_THERMAL_MONITOR == true>
    /* PLC Temperature Monitor */
    DRV_PLC_HAL_GET_THMON                    getThermalMonitor;

</#if>
<#if DRV_PLC_MODE == "PL460">
    /* PLC HAL Set Tx Enable pin */
    DRV_PLC_HAL_SET_TXENABLE                 setTxEnable;

</#if>
    /* PLC HAL Enable/Disable external interrupt */
    DRV_PLC_HAL_ENABLE_EXT_INT               enableExtInt;

    /* PLC HAL Get Pin level */
    DRV_PLC_HAL_GET_PIN_LEVEL                getPinLevel;

    /* PLC HAL delay function */
    DRV_PLC_HAL_DELAY                        delay;

    /* PLC HAL Transfer Bootloader Command */
    DRV_PLC_HAL_SEND_BOOT_CMD                sendBootCmd;

    /* PLC HAL Transfer Write/Read Command */
    DRV_PLC_HAL_SEND_WRRD_CMD                sendWrRdCmd;

} DRV_PLC_HAL_INTERFACE;

// *****************************************************************************
/* DRV_PLC_HAL Command object

  Summary:
    Object used to handle any SPI transfer.

  Description:
    None.

  Remarks:
    None.
*/        

typedef struct
{
    uint8_t *pData;
    uint16_t length;
    uint16_t memId;
    uint16_t cmd;
}DRV_PLC_HAL_CMD;

// *****************************************************************************
/* DRV_PLC_HAL header Information

  Summary:
    Object used to keep track of a SPI transfer with PLC transceiver.

  Description:
    None.

  Remarks:
    None.
*/    

typedef struct
{
    uint32_t flags;
    uint16_t key;
}DRV_PLC_HAL_INFO;

// *****************************************************************************
// *****************************************************************************
// Section: DRV_PLC_HAL Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

void DRV_PLC_HAL_Init(DRV_PLC_PLIB_INTERFACE *plcPlib);
void DRV_PLC_HAL_Reset(void);
<#if DRV_PLC_SLEEP_MODE == true>
void DRV_PLC_HAL_SetStandBy(bool enable);
</#if>
<#if DRV_PLC_MODE == "PL460" && DRV_PLC_THERMAL_MONITOR == true> 
bool DRV_PLC_HAL_GetThermalMonitor(void);
</#if>
void DRV_PLC_HAL_Setup(bool set16Bits);
<#if DRV_PLC_MODE == "PL460">       
void DRV_PLC_HAL_SetTxEnable(bool enable);
</#if>
void DRV_PLC_HAL_EnableInterrupts(bool enable);
bool DRV_PLC_HAL_GetPinLevel(SYS_PORT_PIN pin);
void DRV_PLC_HAL_Delay(uint32_t delayUs);
void DRV_PLC_HAL_SendBootCmd(uint16_t cmd, uint32_t addr, uint32_t dataLength, uint8_t *pDataWr, uint8_t *pDataRd);
void DRV_PLC_HAL_SendWrRdCmd(DRV_PLC_HAL_CMD *pCmd, DRV_PLC_HAL_INFO *pInfo);

#ifdef __cplusplus
}
#endif

#endif // #ifndef DRV_PLC_HAL_H
/*******************************************************************************
 End of File
*/
