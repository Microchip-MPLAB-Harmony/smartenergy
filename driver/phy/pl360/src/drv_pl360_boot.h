/*******************************************************************************
  DRV_PL360 Bootloader Layer

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360_boot.c

  Summary:
    PL360 Driver Platform Abstraction Layer

  Description:
    The PL360 Library provides a Platform Abstraction Layer.
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

#ifndef _DRV_PL360_BOOT_H
#define _DRV_PL360_BOOT_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdbool.h>
#include "driver/phy/pl360/src/drv_pl360_local.h"

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
/*  The following data type definitions are used by the functions in this
    interface and should be considered part it.
*/
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/** Bootloader command: Disable SPI control to bootloader */
#define PL360_DRV_BOOT_CMD_DIS_SPI_CTRL       0xA55A
/** Bootloader command: Disable SPI control to bootloader and disable Boot_CLK */
#define PL360_DRV_BOOT_CMD_DIS_SPI_CLK_CTRL   0xA66A
/** Bootloader Address of CPUWAIT */
#define PL360_DRV_BOOT_CMD_ENABLE_WRITE       0xDE05
/** Bootloader Address for writing program */
#define PL360_DRV_BOOT_PROGRAM_ADDR           0x00000000
/** Bootloader Passwords for enable writing  */
#define PL360_DRV_BOOT_WRITE_KEY              0x5345ACBA

/** Bootloader command: Write Word (32 bits) */
#define PL360_DRV_BOOT_CMD_WRITE_WORD         0x0000
/** Bootloader command: Write buffer */
#define PL360_DRV_BOOT_CMD_WRITE_BUF          0x0001
/** Bootloader command: Read buffer */
#define PL360_DRV_BOOT_CMD_READ_BUF           0x0002
/** Bootloader command: Read Word (32 bits) */
#define PL360_DRV_BOOT_CMD_READ_WORD          0x0003
/** Bootloader command: Set number of decryption packets */
#define PL360_DRV_BOOT_SET_DEC_NUM_PKTS       0x0004
/** Bootloader command: Set decryption initial vector */
#define PL360_DRV_BOOT_SET_DEC_INIT_VECT      0x0005
/** Bootloader command: Set decryption signature */
#define PL360_DRV_BOOT_SET_DEC_SIGN           0x0006
/** Bootloader command: Set 128 bits fuses value */
#define PL360_DRV_BOOT_SET_128_FUSES_VALUE    0x0007
/** Bootloader command: Write buffer register to tamper value at KEY_ENC_FUSES */
#define PL360_DRV_BOOT_SET_ENC_FUSES          0x0008
/** Bootloader command: Write buffer register to tamper value at KEY_TAG_FUSES */
#define PL360_DRV_BOOT_SET_TAG_FUSES          0x0009
/** Bootloader command: Write buffer register to tamper value at CONTROL_FUSES */
#define PL360_DRV_BOOT_SET_CTRL_FUSES         0x000B
/** Bootloader command: Blown desired fuses */
#define PL360_DRV_BOOT_BLOWN_FUSES            0x000C
/** Bootloader command: Read KEY_ENC_FUSES to tamper register */
#define PL360_DRV_BOOT_GET_ENC_FUSES          0x000D
/** Bootloader command: Read KEY_TAG_FUSES to tamper register */
#define PL360_DRV_BOOT_GET_TAG_FUSES          0x000E
/** Bootloader command: Read CONTROL_FUSES to tamper register */
#define PL360_DRV_BOOT_GET_CTRL_FUSES         0x0010
/** Bootloader command: Read tamper register */
#define PL360_DRV_BOOT_READ_TAMPER_REG        0x0011
/** Bootloader command: Read bootloader status */
#define PL360_DRV_BOOT_READ_BOOT_STATUS       0x0012
/** Bootloader command: Start Decryption */
#define PL360_DRV_BOOT_START_DECRYPT          0x0013
/** Bootloader command: Stop Window */
#define PL360_DRV_BOOT_STOP_WINDOW            0x0014

/* -------- Register Definition -------- */
#define PL360_MISCR                               (0x400E1800U) /**< \brief (MISC) Miscelaneous Register */
#define PL360_RSTR                                (0x400E1804U) /**< \brief (RSTR) Reset Register */
#define PL360_SR                                  (0x400E1808U) /**< \brief (SR) Status Register */

/* -------- PL360_MISCR : Miscelaneous Register -------- */
#define PL360_MISCR_CPUWAIT                       (0x1u << 0) /**< \brief (PL360_MISCR) Cortex M7 Hold */
#define PL360_MISCR_PPM_CALIB_ON                  (0x1u << 8) /**< \brief (PL360_MISCR) PPM Calibration On */
#define PL360_MISCR_PPM_CALIB_OFF                 (0x0u << 8) /**< \brief (PL360_MISCR) PPM Calibration Off */
#define PL360_MISCR_MEM_128_64_CFG                (0x0u << 16) /**< \brief (PL360_MISCR) Memory configuration: 128kB ITCM - 64kB DTCM */
#define PL360_MISCR_MEM_96_96_CFG                 (0x1u << 16) /**< \brief (PL360_MISCR) Memory configuration: 96kB ITCM - 96kB DTCM */
#define PL360_MISCR_EN_ACCESS_ERROR               (0x1u << 24) /**< \brief (PL360_MISCR) Access Errors from CM7 enable */
#define PL360_MISCR_SET_GPIO_12_ZC                (0x0u << 25) /**< \brief (PL360_MISCR) Change GPIO ZeroCross: ZC by GPIO_12 */
#define PL360_MISCR_SET_GPIO_2_ZC                 (0x1u << 25) /**< \brief (PL360_MISCR) Change GPIO ZeroCross: ZC by GPIO_2 */
#define PL360_MISCR_SIGN_FAIL                     (0x1u << 26) /**< \brief (PL360_MISCR) Check fail in Signature check */

/* -------- PL360_RSTR : Reset Register -------- */
#define PL360_RSTR_EN_PROC_RESET                  (0x1u << 0) /**< \brief (PL360_RSTR) Enable Processor Reset */
#define PL360_RSTR_EN_PER_RESET                   (0x1u << 1) /**< \brief (PL360_RSTR) Enable Peripheral Reset */
#define PL360_RSTR_EN_PERSLCK_RESET               (0x1u << 2) /**< \brief (PL360_RSTR) Enable Peripheral SLCK Reset */
#define PL360_RSTR_EN_LOCK_PLL                    (0x1u << 3) /**< \brief (PL360_RSTR) Enable Lock PLL */
#define PL360_RSTR_VALUE_CONST_RST                (0x24u << 16) /**< \brief (PL360_RSTR) Number of cycles on active reset */

/* -------- PL360_SR : Status Register -------- */
#define PL360_SR_WDT_RESET                        (0x1u << 0) /**< \brief (PL360_RSTR) Watchdog Reset */
#define PL360_SR_CM7_RESET                        (0x1u << 1) /**< \brief (PL360_RSTR) Cortex-M7 Reset */
#define PL360_SR_USR_RESET                        (0x1u << 2) /**< \brief (PL360_RSTR) User Reset */

/* -------- PL360_Fuses : Fuses Control---- --- */
#define PL360_FUSES_ENCRNOTPLAIN                  (0x1u << 0) /**< \brief (PL360_Fuses) Set to enable secure mode */
#define PL360_FUSES_READ_AES_KEY                  (0x1u << 1) /**< \brief (PL360_Fuses) Set to disable AES key fuses reading.*/
#define PL360_FUSES_WRITE_AES_KEY                 (0x1u << 2) /**< \brief (PL360_Fuses) Set to disable AES key fuses writing.*/
#define PL360_FUSES_READ_CONTROL                  (0x1u << 5) /**< \brief (PL360_Fuses) Set to disable fuses control reading. */
#define PL360_FUSES_WRITE_CONTROL                 (0x1u << 6) /**< \brief (PL360_Fuses) Set to disable fuses control writing. */
#define PL360_FUSES_READ_RAM                      (0x1u << 7) /**< \brief (PL360_Fuses) Set to disable ram reading. */
#define PL360_FUSES_KEY_MCHP_RDY                  (0x1u << 9) /**< \brief (PL360_Fuses) Set to disable bootloader in master mode */
#define PL360_FUSES_SIG_IV_NB                     (0x1u << 10) /**< \brief (PL360_Fuses) Set to force IV + NB in Signature */
#define PL360_FUSES_DISABLE_DBG                   (0x1u << 16) /**< \brief (PL360_Fuses) Set to disable Debug access */
#define PL360_FUSES_DBG_MSSC                      (0x1u << 17) /**< \brief (PL360_Fuses) Set to Debug access depending on MSSC register */

/* -------- PL360_Boot_status : Boot status ----------- */
#define PL360_FUSES_BOOT_ST_FUSES_ACT             (0x1u << 0) /**< \brief (PL360_Boot_status) Fuses Blown active */
#define PL360_FUSES_BOOT_ST_AES_ACT               (0x1u << 1) /**< \brief (PL360_Boot_status) AES active */
#define PL360_FUSES_BOOT_ST_SIGN_OK               (0x1u << 2) /**< \brief (PL360_Boot_status) Signature OK */

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
} DRV_PL360_BOOT_INFO;

void drv_pl360_boot_start(void *pl360Drv);
void drv_pl360_boot_tasks( void );
DRV_PL360_BOOT_STATUS drv_pl360_boot_status( void );
void drv_pl360_boot_restart(bool update);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _DRV_PL360_BOOT_H
/*******************************************************************************
 End of File
*/
