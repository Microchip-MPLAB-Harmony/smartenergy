/******************************************************************************
  DRV_PL360 Bootloader Layer

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360_boot.c

  Summary:
    PL360 Driver Bootloader Layer

  Description:
    The PL360 Library provides a Bootloader Layer.
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
#include "configuration.h"
#include "driver/pl360/src/drv_pl360_boot.h"
#include "driver/pl360/src/drv_pl360_local.h"
#include "driver/pl360/src/drv_pl360_local_comm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global objects
// *****************************************************************************
// *****************************************************************************

static DRV_PL360_HAL_INTERFACE *sDrvPL360HalObj;
static DRV_PL360_BOOT_INFO sDrvPL360BootInfo = {0};

#define MAX_FRAG_SIZE      512

// *****************************************************************************
// *****************************************************************************
// Section: DRV_PL360_BOOT Local Functions
// *****************************************************************************
// *****************************************************************************
#include <string.h>
uint8_t dbgData[MAX_FRAG_SIZE];
static void _firmware_upload_task(void)
{
    uint8_t *pData;
    uint32_t progAddr;
    uint16_t fragSize;
    uint8_t padding;
    
    pData = (uint8_t *)sDrvPL360BootInfo.pSrc;
    progAddr = sDrvPL360BootInfo.pDst;

    if (sDrvPL360BootInfo.pendingLength > MAX_FRAG_SIZE) {
        fragSize = MAX_FRAG_SIZE;
        padding = 0;
    } else {
        fragSize = sDrvPL360BootInfo.pendingLength;
        padding = fragSize % 4;
        fragSize += padding;
    }

    /* Write fragment data */
    sDrvPL360HalObj->sendBootCmd(PL360_DRV_BOOT_CMD_WRITE_BUF, progAddr, fragSize, pData, NULL);

    /* Update counters */
    sDrvPL360BootInfo.pendingLength -= (fragSize - padding);
    pData += fragSize;
    progAddr += fragSize;
    sDrvPL360BootInfo.pSrc = (uint32_t)pData;
    sDrvPL360BootInfo.pDst = progAddr;

}

static void _enable_boot_cmd(void)
{
    uint32_t reg_value;
    uint8_t cmd_value[4];
    
    /* Reset PL360 device */
    sDrvPL360HalObj->reset();
    
    /* Configure 8 bits transfer */
    sDrvPL360HalObj->setup(false);
    
    /* Enable Write operation in bootloader */
    cmd_value[3] = (uint8_t)(PL360_DRV_BOOT_WRITE_KEY >> 24);
    cmd_value[2] = (uint8_t)(PL360_DRV_BOOT_WRITE_KEY >> 16);
    cmd_value[1] = (uint8_t)(PL360_DRV_BOOT_WRITE_KEY >> 8);
    cmd_value[0] = (uint8_t)(PL360_DRV_BOOT_WRITE_KEY);
    sDrvPL360HalObj->sendBootCmd(PL360_DRV_BOOT_CMD_ENABLE_WRITE, 0, 4, cmd_value, NULL);

    cmd_value[3] = (uint8_t)(PL360_DRV_BOOT_WRITE_KEY >> 8);
    cmd_value[2] = (uint8_t)(PL360_DRV_BOOT_WRITE_KEY);
    cmd_value[1] = (uint8_t)(PL360_DRV_BOOT_WRITE_KEY >> 24);
    cmd_value[0] = (uint8_t)(PL360_DRV_BOOT_WRITE_KEY >> 16);
    sDrvPL360HalObj->sendBootCmd(PL360_DRV_BOOT_CMD_ENABLE_WRITE, 0, 4, cmd_value, NULL);

    /* System configuration */
    reg_value = PL360_MISCR_CPUWAIT | PL360_MISCR_PPM_CALIB_OFF | PL360_MISCR_MEM_96_96_CFG | PL360_MISCR_EN_ACCESS_ERROR | PL360_MISCR_SET_GPIO_12_ZC;
    cmd_value[3] = (uint8_t)(reg_value >> 24);
    cmd_value[2] = (uint8_t)(reg_value >> 16);
    cmd_value[1] = (uint8_t)(reg_value >> 8);
    cmd_value[0] = (uint8_t)(reg_value);
    sDrvPL360HalObj->sendBootCmd(PL360_DRV_BOOT_CMD_WRITE_WORD, PL360_MISCR, 4, cmd_value, NULL);
}

static void _disable_boot_cmd(void)
{
    uint32_t reg_value;
    uint8_t cmd_value[4];
    
    /* Configure 8 bits transfer */
    sDrvPL360HalObj->setup(false);
    
    /* Disable CPU Wait */
    reg_value = PL360_MISCR_PPM_CALIB_OFF | PL360_MISCR_MEM_96_96_CFG | PL360_MISCR_EN_ACCESS_ERROR | PL360_MISCR_SET_GPIO_12_ZC;
    cmd_value[3] = (uint8_t)(reg_value >> 24);
    cmd_value[2] = (uint8_t)(reg_value >> 16);
    cmd_value[1] = (uint8_t)(reg_value >> 8);
    cmd_value[0] = (uint8_t)(reg_value);
    sDrvPL360HalObj->sendBootCmd(PL360_DRV_BOOT_CMD_WRITE_WORD, PL360_MISCR, 4, cmd_value, NULL);

    /* Disable Bootloader */
    sDrvPL360HalObj->sendBootCmd(PL360_DRV_BOOT_CMD_DIS_SPI_CLK_CTRL, 0, 0, NULL, NULL);
    
    /* Wait to PL360 startup (10ms) */
    sDrvPL360HalObj->delay(10000);
    
    /* Configure 16 bits transfer */
    sDrvPL360HalObj->setup(true);
}

static bool _check_firmware(void)
{
    DRV_PL360_HAL_CMD halCmd;
    DRV_PL360_HAL_INFO halInfo;
    uint8_t status[PL360_STATUS_LENGTH];
    
    halCmd.cmd = DRV_PL360_HAL_CMD_RD;
    halCmd.memId = STATUS_ID;
    halCmd.length = PL360_STATUS_LENGTH;
    halCmd.pData = status;    
    
    sDrvPL360HalObj->sendWrRdCmd(&halCmd, &halInfo);

    if (halInfo.key == DRV_PL360_HAL_KEY_CORTEX)
    {
        return true;
    }
    
    return false;
}

// *****************************************************************************
// *****************************************************************************
// Section: DRV_PL360_BOOT Global Functions
// *****************************************************************************
// *****************************************************************************

void drv_pl360_boot_start(void *pl360Drv)
{
    DRV_PL360_OBJ *pl360DrvObj = (DRV_PL360_OBJ *)pl360Drv;
    
 #ifdef DRV_PL360_BIN_ADDRESS
    sDrvPL360BootInfo.binSize = pl360DrvObj->binSize;
    sDrvPL360BootInfo.binStartAddress = pl360DrvObj->binStartAddress;
    sDrvPL360BootInfo.pendingLength = pl360DrvObj->binSize;
    sDrvPL360BootInfo.pSrc = pl360DrvObj->binStartAddress;
#else
    extern uint8_t pl360_bin_start;
    extern uint8_t pl360_bin_end;

    sDrvPL360BootInfo.binStartAddress = (int)&pl360_bin_start;
    sDrvPL360BootInfo.binSize = (int)&pl360_bin_end - (int)&pl360_bin_start;
    sDrvPL360BootInfo.pendingLength = sDrvPL360BootInfo.binSize;
    sDrvPL360BootInfo.pSrc = sDrvPL360BootInfo.binStartAddress;    
#endif
    
    sDrvPL360HalObj = pl360DrvObj->pl360Hal;
    sDrvPL360BootInfo.secure = pl360DrvObj->secure;
    sDrvPL360BootInfo.pDst = PL360_DRV_BOOT_PROGRAM_ADDR;

    _enable_boot_cmd();
    
    if (sDrvPL360BootInfo.secure) {
        /* Secure Mode : Set secure configuration and send encrypted binary file */
        /* TBD */
    }    
    
    sDrvPL360BootInfo.status = DRV_PL360_BOOT_STATUS_PROCESING;
}

DRV_PL360_BOOT_STATUS drv_pl360_boot_status( void )
{
    return sDrvPL360BootInfo.status;
}

void drv_pl360_boot_tasks( void )
{
    if (sDrvPL360BootInfo.status == DRV_PL360_BOOT_STATUS_PROCESING)
    {
        _firmware_upload_task();
        if (sDrvPL360BootInfo.pendingLength == 0)
        {
            /* Complete firmware upload */
            sDrvPL360BootInfo.status = DRV_PL360_BOOT_STATUS_SWITCHING;
        }
    }
    else if (sDrvPL360BootInfo.status == DRV_PL360_BOOT_STATUS_SWITCHING)
    {
        _disable_boot_cmd();
        sDrvPL360BootInfo.status = DRV_PL360_BOOT_STATUS_VALIDATING;
    }
    else if (sDrvPL360BootInfo.status == DRV_PL360_BOOT_STATUS_VALIDATING)
    {
        
        /* Check firmware */
        if (_check_firmware())
        {
//            /* Configure 16 bits transfer */
//            sDrvPL360HalObj->setup(true);
            /* Update boot status */
            sDrvPL360BootInfo.status = DRV_PL360_BOOT_STATUS_READY;
        }
        else
        {
            /* Update boot status */
            sDrvPL360BootInfo.status = DRV_PL360_BOOT_STATUS_ERROR;
        }
    }
}

void drv_pl360_boot_restart(bool update)
{
    if (!update)
    {
        /* Configure 8 bits transfer */
        sDrvPL360HalObj->setup(false);
        
        /* Disable Bootloader */
        sDrvPL360HalObj->sendBootCmd(PL360_DRV_BOOT_CMD_DIS_SPI_CLK_CTRL, 0, 0, NULL, NULL);

        /* Wait to PL360 startup (5ms) */
        sDrvPL360HalObj->delay(5000);

        /* Configure 16 bits transfer */
        sDrvPL360HalObj->setup(true);
    }
    else
    {
        /* Update binary file */
        sDrvPL360BootInfo.pendingLength = sDrvPL360BootInfo.binSize;
        sDrvPL360BootInfo.pSrc = sDrvPL360BootInfo.binStartAddress;
        sDrvPL360BootInfo.pDst = PL360_DRV_BOOT_PROGRAM_ADDR;

        _enable_boot_cmd();
        
        while(sDrvPL360BootInfo.pendingLength)
        {
            _firmware_upload_task();
        }
        
        _disable_boot_cmd();
        
        /* Check firmware */
        if (_check_firmware())
        {
            sDrvPL360BootInfo.status = DRV_PL360_BOOT_STATUS_READY;
        }
        else
        {
            sDrvPL360BootInfo.status = DRV_PL360_BOOT_STATUS_ERROR;
        }
    }
}
