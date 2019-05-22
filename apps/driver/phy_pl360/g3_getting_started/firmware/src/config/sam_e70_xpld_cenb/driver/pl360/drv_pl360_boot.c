/******************************************************************************
  DRV_PL360 Bootloader Layer

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360_boot.c

  Summary:
    PL360 Driver Bootloader Layer

  Description:
    This file contains the source code for the implementation of the bootloader
    of PL360 device.
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

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "driver/pl360/drv_pl360_hal.h"
#include "driver/pl360/src/drv_pl360_boot.h"
#include "driver/pl360/src/drv_pl360_local_comm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* This is the pointer to refer Hardware Abstraction Layer (HAL) object. */
static DRV_PL360_HAL_INTERFACE *sDrvPL360HalObj;

/* This is the driver instance object array. */
static DRV_PL360_BOOT_INFO sDrvPL360BootInfo = {0};

/* This is the maximum size of the fragments to handle the upload task of binary
 file to PL360 device */
#define MAX_FRAG_SIZE      512

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
static void _DRV_PL360_BOOT_FirmwareUploadTask(void)
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

static void _DRV_PL360_BOOT_EnableBootCmd(void)
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

static void _DRV_PL360_BOOT_DisableBootCmd(void)
{
    uint32_t reg_value;
    uint8_t cmd_value[4];
    
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

static bool _DRV_PL360_BOOT_CheckFirmware(void)
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
// Section: DRV_PL360_BOOT Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

void DRV_PL360_BOOT_Start(void *pl360Drv)
{
    DRV_PL360_OBJ *pl360DrvObj = (DRV_PL360_OBJ *)pl360Drv;
    
    sDrvPL360BootInfo.binSize = pl360DrvObj->binSize;
    sDrvPL360BootInfo.binStartAddress = pl360DrvObj->binStartAddress;
    sDrvPL360BootInfo.pendingLength = pl360DrvObj->binSize;
    sDrvPL360BootInfo.pSrc = pl360DrvObj->binStartAddress;    
    sDrvPL360HalObj = pl360DrvObj->pl360Hal;
    sDrvPL360BootInfo.secure = pl360DrvObj->secure;
    sDrvPL360BootInfo.pDst = PL360_DRV_BOOT_PROGRAM_ADDR;

    _DRV_PL360_BOOT_EnableBootCmd();
    
    if (sDrvPL360BootInfo.secure) {
        /* Secure Mode : Set secure configuration and send encrypted binary file */
        /* TBD */
    }    
    
    sDrvPL360BootInfo.status = DRV_PL360_BOOT_STATUS_PROCESING;
}

DRV_PL360_BOOT_STATUS DRV_PL360_BOOT_Status( void )
{
    return sDrvPL360BootInfo.status;
}

void DRV_PL360_BOOT_Tasks( void )
{
    if (sDrvPL360BootInfo.status == DRV_PL360_BOOT_STATUS_PROCESING)
    {
        _DRV_PL360_BOOT_FirmwareUploadTask();
        if (sDrvPL360BootInfo.pendingLength == 0)
        {
            /* Complete firmware upload */
            sDrvPL360BootInfo.status = DRV_PL360_BOOT_STATUS_SWITCHING;
        }
    }
    else if (sDrvPL360BootInfo.status == DRV_PL360_BOOT_STATUS_SWITCHING)
    {
        _DRV_PL360_BOOT_DisableBootCmd();
        sDrvPL360BootInfo.status = DRV_PL360_BOOT_STATUS_VALIDATING;
    }
    else if (sDrvPL360BootInfo.status == DRV_PL360_BOOT_STATUS_VALIDATING)
    {
        
        /* Check firmware */
        if (_DRV_PL360_BOOT_CheckFirmware())
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

void DRV_PL360_BOOT_Restart(bool update)
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

        _DRV_PL360_BOOT_EnableBootCmd();
        
        while(sDrvPL360BootInfo.pendingLength)
        {
            _DRV_PL360_BOOT_FirmwareUploadTask();
        }
        
        _DRV_PL360_BOOT_DisableBootCmd();
        
        /* Check firmware */
        if (_DRV_PL360_BOOT_CheckFirmware())
        {
            sDrvPL360BootInfo.status = DRV_PL360_BOOT_STATUS_READY;
        }
        else
        {
            sDrvPL360BootInfo.status = DRV_PL360_BOOT_STATUS_ERROR;
        }
    }
}
