/******************************************************************************
  DRV_G3_MACRT Library Interface Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    drv_g3_macrt.c

  Summary:
    G3 MAC RT Driver Library Interface implementation

  Description:
    The G3 MAC RT Library provides a interface to access the PLC external device.
    This file implements the G3 Mac Real Time Library interface.
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
#include "configuration.h"
#include "driver/plc/common/drv_plc_boot.h"
#include "driver/plc/g3MacRt/drv_g3_macrt.h"
#include "driver/plc/g3MacRt/drv_g3_macrt_local_comm.h"
#include "drv_g3_macrt_local.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* This is the driver instance object array. */
DRV_G3_MACRT_OBJ gDrvG3MacRtObj;

// *****************************************************************************
// *****************************************************************************
// Section: PLC Driver Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_G3_MACRT_Initialize(
    const SYS_MODULE_INDEX index,
    const SYS_MODULE_INIT * const init
)
{
    DRV_G3_MACRT_INIT* g3MacRtInit = (DRV_G3_MACRT_INIT *)init;

    /* Validate the request */
    if (index >= DRV_G3_MACRT_INSTANCES_NUMBER)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    if (gDrvG3MacRtObj.inUse == true)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    gDrvG3MacRtObj.status                = SYS_STATUS_UNINITIALIZED;

    gDrvG3MacRtObj.inUse                 = true;
    gDrvG3MacRtObj.nClients              = 0;

    gDrvG3MacRtObj.plcHal                = g3MacRtInit->plcHal;
    gDrvG3MacRtObj.nClientsMax           = g3MacRtInit->numClients;
    gDrvG3MacRtObj.plcProfile            = g3MacRtInit->plcProfile;
    gDrvG3MacRtObj.binSize               = g3MacRtInit->binEndAddress - g3MacRtInit->binStartAddress;
    gDrvG3MacRtObj.binStartAddress       = g3MacRtInit->binStartAddress;
    gDrvG3MacRtObj.secure                = g3MacRtInit->secure;
    
    /* Callbacks initialization */
    gDrvG3MacRtObj.bootDataCallback      = 0;
    gDrvG3MacRtObj.txCfmCallback         = 0;
    gDrvG3MacRtObj.dataIndCallback       = 0;
    gDrvG3MacRtObj.mlmeGetCfmcallback    = 0;
    gDrvG3MacRtObj.exceptionCallback     = 0;
    gDrvG3MacRtObj.snifferDataCallback   = 0;

    /* HAL init */
    gDrvG3MacRtObj.plcHal->init((DRV_PLC_PLIB_INTERFACE *)g3MacRtInit->plcHal->plcPlib);

    /* Update status */
    gDrvG3MacRtObj.status                = SYS_STATUS_BUSY;

    /* Return the object structure */
    return ( (SYS_MODULE_OBJ)index );

}

SYS_STATUS DRV_G3_MACRT_Status( const SYS_MODULE_INDEX index )
{
    /* Validate the request */
    if (index >= DRV_G3_MACRT_INSTANCES_NUMBER)
    {
        return SYS_STATUS_ERROR;
    }
    
    /* Return the driver status */
    return (gDrvG3MacRtObj.status);
}

DRV_HANDLE DRV_G3_MACRT_Open(
    const SYS_MODULE_INDEX index,
    const DRV_PLC_BOOT_DATA_CALLBACK callback
)
{
    DRV_PLC_BOOT_INFO bootInfo;
    
    /* Validate the request */
    if (index >= DRV_G3_MACRT_INSTANCES_NUMBER)
    {
        return DRV_HANDLE_INVALID;
    }

    if ((gDrvG3MacRtObj.status != SYS_STATUS_BUSY) || (gDrvG3MacRtObj.inUse == false) \
            || (gDrvG3MacRtObj.nClients >= gDrvG3MacRtObj.nClientsMax))
    {
        return DRV_HANDLE_INVALID;
    }
    
    /* Launch boot start process */  
    bootInfo.binSize = gDrvG3MacRtObj.binSize;
    bootInfo.binStartAddress = gDrvG3MacRtObj.binStartAddress;
    bootInfo.pendingLength = gDrvG3MacRtObj.binSize;
    bootInfo.pSrc = gDrvG3MacRtObj.binStartAddress;    
    bootInfo.secure = gDrvG3MacRtObj.secure;
    if (callback)
    {
        bootInfo.bootDataCallback = callback;
        bootInfo.contextBoot = index;
    }
    
    DRV_PLC_BOOT_Start(&bootInfo, gDrvG3MacRtObj.plcHal);

    gDrvG3MacRtObj.nClients++;

    return ((DRV_HANDLE)0);
}

void DRV_G3_MACRT_Close( const DRV_HANDLE handle )
{
    if ((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvG3MacRtObj.nClients--;
        gDrvG3MacRtObj.inUse = false;
    }
}

void DRV_G3_MACRT_TxCfmCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_TX_CFM_CALLBACK callback, 
    const uintptr_t context 
)
{
    if ((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvG3MacRtObj.txCfmCallback = callback;
        gDrvG3MacRtObj.contextTxCfm = context;
    }
}

void DRV_G3_MACRT_DataIndCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_DATA_IND_CALLBACK callback, 
    const uintptr_t context 
)
{
    if ((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvG3MacRtObj.dataIndCallback = callback;
        gDrvG3MacRtObj.contextDataInd = context;
    }
}

void DRV_G3_MACRT_MlmeGetCfmCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_MLME_GET_CFM_CALLBACK callback, 
    const uintptr_t context 
)
{
    if ((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvG3MacRtObj.mlmeGetCfmcallback = callback;
        gDrvG3MacRtObj.contextMlmeGetCfm = context;
    }
}

void DRV_G3_MACRT_ExceptionCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_EXCEPTION_CALLBACK callback, 
    const uintptr_t context 
)
{
    if ((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvG3MacRtObj.exceptionCallback = callback;
        gDrvG3MacRtObj.contextExc = context;
    }
}

void DRV_G3_MACRT_SnifferCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_SNIFFER_CALLBACK callback,
    const uint8_t *pSnifferData, 
    const uintptr_t context 
)
{
    if ((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvG3MacRtObj.snifferDataCallback = callback;
        gDrvG3MacRtObj.contextSniffer = context;
        gDrvG3MacRtObj.pDataSniffer = (uint8_t *)pSnifferData;
    }
}

void DRV_G3_MACRT_Tasks( SYS_MODULE_OBJ hSysObj )
{
    /* Validate the request */
    if (hSysObj >= DRV_G3_MACRT_INSTANCES_NUMBER)
    {
        return; // SYS_MODULE_OBJ_INVALID
    }
    
    if (gDrvG3MacRtObj.status == SYS_STATUS_READY)
    {
        /* Run G3 MAC RT communication task */
        DRV_G3_MACRT_Task();
    }
    else if (gDrvG3MacRtObj.status == SYS_STATUS_BUSY)
    {
        DRV_PLC_BOOT_STATUS state;
        
        /* Check bootloader process */
        state = DRV_PLC_BOOT_Status();
        if (state < DRV_PLC_BOOT_STATUS_READY)
        {
            DRV_PLC_BOOT_Tasks();
        } 
        else if (state == DRV_PLC_BOOT_STATUS_READY)
        {
            DRV_G3_MACRT_Init(&gDrvG3MacRtObj);
            gDrvG3MacRtObj.status = SYS_STATUS_READY;
            gDrvG3MacRtObj.state = DRV_G3_MACRT_STATE_IDLE;
        }
        else
        {
            gDrvG3MacRtObj.status = SYS_STATUS_ERROR;
            gDrvG3MacRtObj.state = DRV_G3_MACRT_STATE_ERROR;
        }
    } 
    else
    {
        /* SYS_STATUS_ERROR: Nothing to do */
    }
}

