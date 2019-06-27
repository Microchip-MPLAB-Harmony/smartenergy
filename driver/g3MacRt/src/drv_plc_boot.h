/*******************************************************************************
  DRV_PL360 Bootloader Interface Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360_boot.h

  Summary:
    PL360 Driver Bootloader Interface Header File

  Description:
    The PL360 bootloader provides a simple interface to handle the upload task
    of the firmware on PL360 device. This file provides the interface definition
    for the PL360 bootloader driver.
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

#ifndef _DRV_PL360_BOOT_H
#define _DRV_PL360_BOOT_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
#include <stdio.h>
#include <stdbool.h>
#include "driver/pl360MacRt/src/drv_pl360_macrt_local.h"

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

#define PL360_DRV_BOOT_CMD_DIS_SPI_CTRL      0xA55A

#define PL360_DRV_BOOT_CMD_DIS_SPI_CLK_CTRL  0xA66A

#define PL360_DRV_BOOT_CMD_ENABLE_WRITE      0xDE05

#define PL360_DRV_BOOT_PROGRAM_ADDR          0x00000000

#define PL360_DRV_BOOT_WRITE_KEY             0x5345ACBA

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* DRV_PL360_BOOT Command set

  Summary:
    Enumeration listing the DRV_PL360_BOOT commands.

  Description:
    This enumeration defines the commands used to interact with the DRV_PL360_BOOT
    of PL360 device.

  Remarks:
    None
*/

typedef enum
{
    /* Write Word (32 bits) */
    PL360_DRV_BOOT_CMD_WRITE_WORD         = 0x0000,

    /* Write buffer */
    PL360_DRV_BOOT_CMD_WRITE_BUF          = 0x0001,

    /* Read buffer */
    PL360_DRV_BOOT_CMD_READ_BUF           = 0x0002,

    /* Read Word (32 bits) */
    PL360_DRV_BOOT_CMD_READ_WORD          = 0x0003,

    /* Set number of decryption packets */
    PL360_DRV_BOOT_SET_DEC_NUM_PKTS       = 0x0004,

    /* Set decryption initial vector */
    PL360_DRV_BOOT_SET_DEC_INIT_VECT      = 0x0005,

    /* Set decryption signature */
    PL360_DRV_BOOT_SET_DEC_SIGN           = 0x0006,

    /* Set 128 bits fuses value */
    PL360_DRV_BOOT_SET_128_FUSES_VALUE    = 0x0007,

    /* Write buffer register to tamper value at KEY_ENC_FUSES */
    PL360_DRV_BOOT_SET_ENC_FUSES          = 0x0008,

    /* Write buffer register to tamper value at KEY_TAG_FUSES */
    PL360_DRV_BOOT_SET_TAG_FUSES          = 0x0009,

    /* Write buffer register to tamper value at CONTROL_FUSES */
    PL360_DRV_BOOT_SET_CTRL_FUSES         = 0x000B,

    /* Blown desired fuses */
    PL360_DRV_BOOT_BLOWN_FUSES            = 0x000C,

    /* Read KEY_ENC_FUSES to tamper register */
    PL360_DRV_BOOT_GET_ENC_FUSES          = 0x000D,

    /* Read KEY_TAG_FUSES to tamper register */
    PL360_DRV_BOOT_GET_TAG_FUSES          = 0x000E,

    /* Read CONTROL_FUSES to tamper register */
    PL360_DRV_BOOT_GET_CTRL_FUSES         = 0x0010,

    /* Read tamper register */
    PL360_DRV_BOOT_READ_TAMPER_REG        = 0x0011,

    /* Read bootloader status */
    PL360_DRV_BOOT_READ_BOOT_STATUS       = 0x0012,

    /* Start Decryption */
    PL360_DRV_BOOT_START_DECRYPT          = 0x0013,

    /* Stop Window */
    PL360_DRV_BOOT_STOP_WINDOW            = 0x0014

} DRV_PL360_BOOT_CMD;

typedef enum
{
    DRV_PL360_BOOT_STATUS_IDLE = 0,
    DRV_PL360_BOOT_STATUS_PROCESING,
    DRV_PL360_BOOT_STATUS_SWITCHING,
    DRV_PL360_BOOT_STATUS_VALIDATING,
    DRV_PL360_BOOT_STATUS_READY,
    DRV_PL360_BOOT_STATUS_ERROR,
} DRV_PL360_BOOT_STATUS;

typedef struct
{
    uint32_t                   binSize;
    uint32_t                   binStartAddress;
    bool                       secure;   
    DRV_PL360_BOOT_STATUS      status;
    uint32_t                   pendingLength;
    uint32_t                   pSrc;
    uint32_t                   pDst;
    uint16_t                   secNumPackets;
    uint8_t                    secIV[16];
    uint8_t                    secSN[16];    
} DRV_PL360_BOOT_INFO;

// *****************************************************************************
// *****************************************************************************
// Section: DRV_PL360_BOOT Software API definition
// *****************************************************************************
// *****************************************************************************

/* -------- Register Definition --------------------------------------------- */
#define PL360_MISCR                               (0x400E1800U) /**< (MISC) Miscelaneous Register */
#define PL360_RSTR                                (0x400E1804U) /**< (RSTR) Reset Register */
#define PL360_SR                                  (0x400E1808U) /**< (SR) Status Register */

/* -------- PL360_MISCR : Miscelaneous Register ----------------------------- */
#define PL360_MISCR_CPUWAIT                       (0x1u << 0)   /**< (PL360_MISCR) Cortex M7 Hold */
#define PL360_MISCR_PPM_CALIB_ON                  (0x1u << 8)   /**< (PL360_MISCR) PPM Calibration On */
#define PL360_MISCR_PPM_CALIB_OFF                 (0x0u << 8)   /**< (PL360_MISCR) PPM Calibration Off */
#define PL360_MISCR_MEM_128_64_CFG                (0x0u << 16)  /**< (PL360_MISCR) Memory configuration: 128kB ITCM - 64kB DTCM */
#define PL360_MISCR_MEM_96_96_CFG                 (0x1u << 16)  /**< (PL360_MISCR) Memory configuration: 96kB ITCM - 96kB DTCM */
#define PL360_MISCR_EN_ACCESS_ERROR               (0x1u << 24)  /**< (PL360_MISCR) Access Errors from CM7 enable */
#define PL360_MISCR_SET_GPIO_12_ZC                (0x0u << 25)  /**< (PL360_MISCR) Change GPIO ZeroCross: ZC by GPIO_12 */
#define PL360_MISCR_SET_GPIO_2_ZC                 (0x1u << 25)  /**< (PL360_MISCR) Change GPIO ZeroCross: ZC by GPIO_2 */
#define PL360_MISCR_SIGN_FAIL                     (0x1u << 26)  /**< (PL360_MISCR) Check fail in Signature check */
  
/* -------- PL360_RSTR : Reset Register ------------------------------------- */
#define PL360_RSTR_EN_PROC_RESET                  (0x1u << 0)   /**< (PL360_RSTR) Enable Processor Reset */
#define PL360_RSTR_EN_PER_RESET                   (0x1u << 1)   /**< (PL360_RSTR) Enable Peripheral Reset */
#define PL360_RSTR_EN_PERSLCK_RESET               (0x1u << 2)   /**< (PL360_RSTR) Enable Peripheral SLCK Reset */
#define PL360_RSTR_EN_LOCK_PLL                    (0x1u << 3)   /**< (PL360_RSTR) Enable Lock PLL */
#define PL360_RSTR_VALUE_CONST_RST                (0x24u << 16) /**< (PL360_RSTR) Number of cycles on active reset */

/* -------- PL360_SR : Status Register -------------------------------------- */
#define PL360_SR_WDT_RESET                        (0x1u << 0)   /**< (PL360_RSTR) Watchdog Reset */
#define PL360_SR_CM7_RESET                        (0x1u << 1)   /**< (PL360_RSTR) Cortex-M7 Reset */
#define PL360_SR_USR_RESET                        (0x1u << 2)   /**< (PL360_RSTR) User Reset */

/* -------- PL360_Fuses : Fuses Control-------------------------------------- */
#define PL360_FUSES_ENCRNOTPLAIN                  (0x1u << 0)   /**< (PL360_Fuses) Set to enable secure mode */
#define PL360_FUSES_READ_AES_KEY                  (0x1u << 1)   /**< (PL360_Fuses) Set to disable AES key fuses reading.*/
#define PL360_FUSES_WRITE_AES_KEY                 (0x1u << 2)   /**< (PL360_Fuses) Set to disable AES key fuses writing.*/
#define PL360_FUSES_READ_CONTROL                  (0x1u << 5)   /**< (PL360_Fuses) Set to disable fuses control reading. */
#define PL360_FUSES_WRITE_CONTROL                 (0x1u << 6)   /**< (PL360_Fuses) Set to disable fuses control writing. */
#define PL360_FUSES_READ_RAM                      (0x1u << 7)   /**< (PL360_Fuses) Set to disable ram reading. */
#define PL360_FUSES_KEY_MCHP_RDY                  (0x1u << 9)   /**< (PL360_Fuses) Set to disable bootloader in master mode */
#define PL360_FUSES_SIG_IV_NB                     (0x1u << 10)  /**< (PL360_Fuses) Set to force IV + NB in Signature */
#define PL360_FUSES_DISABLE_DBG                   (0x1u << 16)  /**< (PL360_Fuses) Set to disable Debug access */
#define PL360_FUSES_DBG_MSSC                      (0x1u << 17)  /**< (PL360_Fuses) Set to Debug access depending on MSSC register */

/* -------- PL360_Boot_status : Boot status --------------------------------- */
#define PL360_FUSES_BOOT_ST_FUSES_ACT             (0x1u << 0)   /**< (PL360_Boot_status) Fuses Blown active */
#define PL360_FUSES_BOOT_ST_AES_ACT               (0x1u << 1)   /**< (PL360_Boot_status) AES active */
#define PL360_FUSES_BOOT_ST_SIGN_OK               (0x1u << 2)   /**< (PL360_Boot_status) Signature OK */

// *****************************************************************************
// *****************************************************************************
// Section: DRV_PL360_BOOT Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

void DRV_PL360_BOOT_Start(void *pl360Drv);
void DRV_PL360_BOOT_Tasks( void );
DRV_PL360_BOOT_STATUS DRV_PL360_BOOT_Status( void );
void DRV_PL360_BOOT_Restart(bool update);

/* Provide C++ Compatibility */
#ifdef __cplusplus

    }

#endif

#endif // #ifndef _DRV_PL360_BOOT_H
/*******************************************************************************
 End of File
*/
