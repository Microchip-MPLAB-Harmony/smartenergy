/*******************************************************************************
  PL360 Driver Definitions Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360_definitions.h

  Summary:
    PL360 Driver Definitions Header File

  Description:
    This file provides implementation-specific definitions for the PL360
    driver's system interface.
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

#ifndef DRV_PL360_DEFINITIONS_H
#define DRV_PL360_DEFINITIONS_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <device.h>
#include "system/ports/sys_ports.h"
#include "system/dma/sys_dma.h"


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef bool (* DRV_PL360_SPI_PLIB_TRANSFER_SETUP)(uintptr_t, uint32_t);

// *****************************************************************************
/* PL360 Driver PLIB Interface Data

  Summary:
    Defines the data required to initialize the PL360 driver PLIB Interface.

  Description:
    This data type defines the data required to initialize the PL360 driver
    PLIB Interface.

  Remarks:
    None.
*/

typedef struct
{
    /* PL360 SPI PLIB Transfer Setup */
    DRV_PL360_SPI_PLIB_TRANSFER_SETUP      spiPlibTransferSetup;

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

    /* PL360 LDO enable pin */
    SYS_PORT_PIN                           ldoPin;

    /* PL360 reset pin */
    SYS_PORT_PIN                           resetPin;

    /* PL360 external interrupt pin */
    SYS_PORT_PIN                           extIntPin;

} DRV_PL360_PLIB_INTERFACE;

// *****************************************************************************

typedef void (* DRV_PL360_HAL_INIT)(DRV_PL360_PLIB_INTERFACE*);

typedef void (* DRV_PL360_HAL_SETUP)(bool);

typedef void (* DRV_PL360_HAL_RESET)(void);

typedef bool (* DRV_PL360_HAL_GET_CD)(void);

typedef void (* DRV_PL360_HAL_ENABLE_EXT_INT)(bool);

typedef void (* DRV_PL360_HAL_DELAY)(uint64_t);

typedef void (* DRV_PL360_HAL_SEND_BOOT_CMD)(uint16_t, uint32_t, uint32_t, void*, void*);

typedef void (* DRV_PL360_HAL_SEND_WRRD_CMD)(void*, void*);

// *****************************************************************************
/* PL360 Driver HAL Interface Data

  Summary:
    Defines the data required to initialize the PL360 driver HAL Interface.

  Description:
    This data type defines the data required to initialize the PL360 driver
    HAL Interface.

  Remarks:
    None.
*/

typedef struct
{
    /* PL360 PLIB Interface */
    DRV_PL360_PLIB_INTERFACE                   *pl360Plib;

    /* PL360 HAL init */
    DRV_PL360_HAL_INIT                         init;

    /* PL360 HAL setup */
    DRV_PL360_HAL_SETUP                        setup;

    /* PL360 HAL reset device */
    DRV_PL360_HAL_RESET                        reset;

    /* PL360 HAL Get Carrier Detect or PLC Line Status */
    DRV_PL360_HAL_GET_CD                       getCd;

    /* PL360 HAL Enable/Disable external interrupt */
    DRV_PL360_HAL_ENABLE_EXT_INT               enableExtInt;

    /* PL360 HAL delay function */
    DRV_PL360_HAL_DELAY                        delay;

    /* PL360 HAL Transfer Bootloader Command */
    DRV_PL360_HAL_SEND_BOOT_CMD                sendBootCmd;

    /* PL360 HAL Transfer Write/Read Command */
    DRV_PL360_HAL_SEND_WRRD_CMD                sendWrRdCmd;

} DRV_PL360_HAL_INTERFACE;

// *****************************************************************************
/* PL360 Driver Initialization Data

  Summary:
    Defines the data required to initialize the PL360 driver

  Description:
    This data type defines the data required to initialize or the PL360 driver.

  Remarks:
    None.
*/

typedef struct
{
    /* Identifies the HAL API set to be used by the driver to access
     * peripherals. */
    DRV_PL360_HAL_INTERFACE        *pl360Hal;

    /* Number of clients */
    size_t                          numClients;

    /* PLC Profile */
    uint8_t                         plcProfile;

    /* Specification Compliance */
    uint8_t                         plcSpecification;

    /* Start Address where PLC binary file is located */
    uint32_t                        binStartAddress;

    /* End Address where PLC binary file is located */
    uint32_t                        binEndAddress;

    /* Secure mode */
    bool                            secure;

} DRV_PL360_MACRT_INIT;


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // #ifndef DRV_PL360_DEFINITIONS_H

/*******************************************************************************
 End of File
*/
