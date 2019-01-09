/******************************************************************************
  DRV_PL360 Library Interface Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360.c

  Summary:
    PL360 Driver Library Interface implementation

  Description:
    The PL360 Library provides a interface to access the PL360 external device.
    This file implements the PL360 Library interface.
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
#include "definitions.h"
#include "driver/phy/pl360/drv_pl360.h"
#include "driver/phy/pl360/drv_pl360_comm.h"
#include "driver/phy/pl360/src/drv_pl360_boot.h"
#include "driver/phy/pl360/src/drv_pl360_local_comm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global objects
// *****************************************************************************
// *****************************************************************************

/* This is the driver instance object array. */
DRV_PL360_OBJ gDrvPL360Obj;

// *****************************************************************************
// *****************************************************************************
// Section: DRV_PL360 Driver Global Functions
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_PL360_Initialize(
    const SYS_MODULE_INDEX drvIndex,
    const SYS_MODULE_INIT * const init
)
{
    DRV_PL360_INIT* pl360Init = (DRV_PL360_INIT *)init;

    /* Validate the request */
    if(drvIndex >= DRV_PL360_INSTANCES_NUMBER)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    if(gDrvPL360Obj.inUse == true)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    gDrvPL360Obj.status                = SYS_STATUS_UNINITIALIZED;

    gDrvPL360Obj.inUse                 = true;
    gDrvPL360Obj.nClients              = 0;

    gDrvPL360Obj.pl360Hal              = pl360Init->pl360Hal;
    gDrvPL360Obj.nClientsMax           = pl360Init->numClients;
    gDrvPL360Obj.plcProfile            = pl360Init->plcProfile;
    gDrvPL360Obj.binSize               = pl360Init->binSize;
    gDrvPL360Obj.binStartAddress       = pl360Init->binStartAddress;
    gDrvPL360Obj.secure                = pl360Init->secure;
    
    /* Callbacks initialization */
    gDrvPL360Obj.dataCfmCallback       = 0;
    gDrvPL360Obj.dataIndCallback       = 0;
    gDrvPL360Obj.exceptionCallback     = 0;

    /* HAL init */
    gDrvPL360Obj.pl360Hal->init((DRV_PL360_PLIB_INTERFACE *)pl360Init->pl360Hal->pl360Plib);

    /* Update the status */
    gDrvPL360Obj.status                = SYS_STATUS_BUSY;

    /* Return the object structure */
    return ( (SYS_MODULE_OBJ)drvIndex );

}

SYS_STATUS DRV_PL360_Status( const SYS_MODULE_INDEX drvIndex )
{
    /* Return the driver status */
    return (gDrvPL360Obj.status);
}

DRV_HANDLE DRV_PL360_Open(
    const SYS_MODULE_INDEX drvIndex,
    const DRV_IO_INTENT ioIntent
)
{
    /* Validate the request */
    if (drvIndex >= DRV_PL360_INSTANCES_NUMBER)
    {
        return DRV_HANDLE_INVALID;
    }

    if((gDrvPL360Obj.status != SYS_STATUS_BUSY) || (gDrvPL360Obj.inUse == false) \
            || (gDrvPL360Obj.nClients >= gDrvPL360Obj.nClientsMax))
    {
        return DRV_HANDLE_INVALID;
    }
    
    /* Launch boot start process */  
    drv_pl360_boot_start(&gDrvPL360Obj);

    gDrvPL360Obj.nClients++;

    return ((DRV_HANDLE)0);
}

void DRV_PL360_Close( const DRV_HANDLE handle )
{
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvPL360Obj.nClients--;
    }
}

void DRV_PL360_DataCfmCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_PL360_DATA_CFM_CALLBACK callback, 
    const uintptr_t context 
)
{
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvPL360Obj.dataCfmCallback = callback;
        gDrvPL360Obj.context = context;
    }
}

void DRV_PL360_DataIndCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_PL360_DATA_IND_CALLBACK callback, 
    const uintptr_t context 
)
{
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvPL360Obj.dataIndCallback = callback;
        gDrvPL360Obj.context = context;
    }
}

void DRV_PL360_ExceptionCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_PL360_EXCEPTION_CALLBACK callback, 
    const uintptr_t context 
)
{
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvPL360Obj.exceptionCallback = callback;
        gDrvPL360Obj.context = context;
    }
}

void DRV_PL360_Tasks( SYS_MODULE_OBJ hSysObj )
{
    if (gDrvPL360Obj.status == SYS_STATUS_READY)
    {
        /* Run PL360 communication task */
        drv_pl360_comm_task();
    }
    else if (gDrvPL360Obj.status == SYS_STATUS_BUSY)
    {
        DRV_PL360_BOOT_STATUS state;
        
        /* Check bootloader process */
        state = drv_pl360_boot_status();
        if (state < DRV_PL360_BOOT_STATUS_READY)
        {
            drv_pl360_boot_tasks();
        } 
        else if (state == DRV_PL360_BOOT_STATUS_READY)
        {
            drv_pl360_comm_init(&gDrvPL360Obj);
            gDrvPL360Obj.status = SYS_STATUS_READY;
            gDrvPL360Obj.state = DRV_PL360_STATE_IDLE;
        }
        else
        {
            gDrvPL360Obj.status = SYS_STATUS_ERROR;
            gDrvPL360Obj.state = DRV_PL360_STATE_ERROR;
        }
    } 
    else
    {
        /* SYS_STATUS_ERROR: Nothing to do */
    }
}

