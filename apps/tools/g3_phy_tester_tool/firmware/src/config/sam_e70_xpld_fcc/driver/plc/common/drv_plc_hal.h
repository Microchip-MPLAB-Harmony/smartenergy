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
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _DRV_PLC_HAL_H
#define _DRV_PLC_HAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdbool.h>
#include "system/ports/sys_ports.h"
#include "system/dma/sys_dma.h"

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
#define DRV_PLC_HAL_CMD_POS                   15
#define DRV_PLC_HAL_CMD_RD                    (0 << DRV_PLC_HAL_CMD_POS)
#define DRV_PLC_HAL_CMD_WR                    (1 << DRV_PLC_HAL_CMD_POS)

#define DRV_PLC_HAL_LEN_MASK                  0x7FFF
        
/* SPI Key MASK */
#define DRV_PLC_HAL_KEY_MASK                  0xFFFE
/* SPI Key when bootloader is running in PLC transceiver */
#define DRV_PLC_HAL_KEY_BOOT                  (0x5634 & DRV_PLC_HAL_KEY_MASK)
/* SPI Key when PLC firmware is running in PLC transceiver */
#define DRV_PLC_HAL_KEY_CORTEX                (0x1122 & DRV_PLC_HAL_KEY_MASK)
        
#define DRV_PLC_HAL_KEY(b0, b1)               ((((uint16_t)b1 << 8) + b0) & DRV_PLC_HAL_KEY_MASK)
#define DRV_PLC_HAL_FLAGS_BOOT(b0, b2, b3)    ((((uint32_t)b3) << 8) + ((uint32_t)b2) + ((uint32_t)(b0 & 0x01) << 16))
#define DRV_PLC_HAL_FLAGS_CORTEX(b2, b3)      ((((uint32_t)b3) << 8) + ((uint32_t)b2))        

/* User rest flag in bootloader key*/
#define DRV_PLC_HAL_FLAG_RST_USER             0x00010000
/* Cortex(debugger) rest flag in bootloader key*/
#define DRV_PLC_HAL_FLAG_RST_CORTEX           0x00008000
/* Watch Dog flag in bootloader key */
#define DRV_PLC_HAL_FLAG_RST_WDOG             0x00004000
/* Power-ON reset is indicated when the three flags are 0, mask will be used to detect it*/
#define DRV_PLC_HAL_FLAG_RST_PON              0x0001C000  

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef bool (* DRV_PLC_SPI_PLIB_TRANSFER_SETUP)(uintptr_t, uint32_t);

 typedef enum
{
    DRV_PLC_SPI_CLOCK_PHASE_TRAILING_EDGE = 0 << SPI_CSR_NCPHA_Pos,
    DRV_PLC_SPI_CLOCK_PHASE_LEADING_EDGE = 1 << SPI_CSR_NCPHA_Pos,

    /* Force the compiler to reserve 32-bit space for each enum value */
    DRV_PLC_SPI_CLOCK_PHASE_INVALID = 0xFFFFFFFF

}DRV_PLC_SPI_CLOCK_PHASE;

typedef enum
{
    DRV_PLC_SPI_CLOCK_POLARITY_IDLE_LOW = 0 << SPI_CSR_CPOL_Pos,
    DRV_PLC_SPI_CLOCK_POLARITY_IDLE_HIGH = 1 << SPI_CSR_CPOL_Pos,

    /* Force the compiler to reserve 32-bit space for each enum value */
    DRV_PLC_SPI_CLOCK_POLARITY_INVALID = 0xFFFFFFFF

}DRV_PLC_SPI_CLOCK_POLARITY;

typedef enum
{
    DRV_PLC_SPI_DATA_BITS_8 = SPI_CSR_BITS_8_BIT_Val << SPI_CSR_BITS_Pos,
    DRV_PLC_SPI_DATA_BITS_9 = SPI_CSR_BITS_9_BIT_Val << SPI_CSR_BITS_Pos,
    DRV_PLC_SPI_DATA_BITS_10 = SPI_CSR_BITS_10_BIT_Val << SPI_CSR_BITS_Pos,
    DRV_PLC_SPI_DATA_BITS_11 = SPI_CSR_BITS_11_BIT_Val << SPI_CSR_BITS_Pos,
    DRV_PLC_SPI_DATA_BITS_12 = SPI_CSR_BITS_12_BIT_Val << SPI_CSR_BITS_Pos,
    DRV_PLC_SPI_DATA_BITS_13 = SPI_CSR_BITS_13_BIT_Val << SPI_CSR_BITS_Pos,
    DRV_PLC_SPI_DATA_BITS_14 = SPI_CSR_BITS_14_BIT_Val << SPI_CSR_BITS_Pos,
    DRV_PLC_SPI_DATA_BITS_15 = SPI_CSR_BITS_15_BIT_Val << SPI_CSR_BITS_Pos,
    DRV_PLC_SPI_DATA_BITS_16 = SPI_CSR_BITS_16_BIT_Val << SPI_CSR_BITS_Pos,

    /* Force the compiler to reserve 32-bit space for each enum value */
    DRV_PLC_SPI_DATA_BITS_INVALID = 0xFFFFFFFF

}DRV_PLC_SPI_DATA_BITS;

typedef struct
{
    uint32_t    clockFrequency;
    DRV_PLC_SPI_CLOCK_PHASE clockPhase;
    DRV_PLC_SPI_CLOCK_POLARITY clockPolarity;
    DRV_PLC_SPI_DATA_BITS   dataBits;

}DRV_PLC_SPI_TRANSFER_SETUP;

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

    /* SPI transmit DMA channel. */
    SYS_DMA_CHANNEL                        dmaChannelTx;

    /* SPI receive DMA channel. */
    SYS_DMA_CHANNEL                        dmaChannelRx;

    /* SPI transmit register address used for DMA operation. */
    void                                   *spiAddressTx;

    /* SPI receive register address used for DMA operation. */
    void                                   *spiAddressRx;

    /* SPI clock frequency */
    uint32_t                               spiClockFrequency;

    /* PLC LDO enable pin */
    SYS_PORT_PIN                           ldoPin;

    /* PLC reset pin */
    SYS_PORT_PIN                           resetPin;

    /* PLC external interrupt pin */
    SYS_PORT_PIN                           extIntPin;

} DRV_PLC_PLIB_INTERFACE;

// *****************************************************************************

typedef void (* DRV_PLC_HAL_INIT)(DRV_PLC_PLIB_INTERFACE*);

typedef void (* DRV_PLC_HAL_SETUP)(bool);

typedef void (* DRV_PLC_HAL_RESET)(void);

typedef bool (* DRV_PLC_HAL_GET_CD)(void);

typedef void (* DRV_PLC_HAL_ENABLE_EXT_INT)(bool);

typedef void (* DRV_PLC_HAL_DELAY)(uint64_t);

typedef void (* DRV_PLC_HAL_SEND_BOOT_CMD)(uint16_t, uint32_t, uint32_t, void*, void*);

typedef void (* DRV_PLC_HAL_SEND_WRRD_CMD)(void*, void*);

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

    /* PLC HAL Get Carrier Detect or PLC Line Status */
    DRV_PLC_HAL_GET_CD                       getCd;

    /* PLC HAL Enable/Disable external interrupt */
    DRV_PLC_HAL_ENABLE_EXT_INT               enableExtInt;

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
    uint32_t key;
    uint32_t flags;
}DRV_PLC_HAL_INFO;

// *****************************************************************************
// *****************************************************************************
// Section: DRV_PLC_HAL Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

void DRV_PLC_HAL_Init(DRV_PLC_PLIB_INTERFACE *plcPlib);
void DRV_PLC_HAL_Reset(void);
void DRV_PLC_HAL_Setup(bool set16Bits);
bool DRV_PLC_HAL_GetCarrierDetect(void);
void DRV_PLC_HAL_EnableInterrupts(bool enable);
void DRV_PLC_HAL_Delay(uint64_t delayUs);
void DRV_PLC_HAL_SendBootCmd(uint16_t cmd, uint32_t address, uint32_t dataLength, uint8_t *pDataWr, uint8_t *pDataRd);
void DRV_PLC_HAL_SendWrRdCmd(DRV_PLC_HAL_CMD *pCmd, DRV_PLC_HAL_INFO *pInfo);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _DRV_PLC_HAL_H
/*******************************************************************************
 End of File
*/
