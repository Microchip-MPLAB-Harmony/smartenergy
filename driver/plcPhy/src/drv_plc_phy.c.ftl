/******************************************************************************
  DRV_PLC_PHY Library Interface Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    drv_plc_phy.c

  Summary:
    PLC Driver Library Interface implementation

  Description:
    The PLC Library provides a interface to access the PLC external device.
    This file implements the PLC Library interface.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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
#include "driver/plc/phy/drv_plc_phy.h"
#include "driver/plc/phy/drv_plc_phy_local_comm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* This is the driver instance object array. */
DRV_PLC_PHY_OBJ gDrvPlcPhyObj<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal"> = {.semaphoreID = NULL}</#if>;

// *****************************************************************************
// *****************************************************************************
// Section: PLC Driver Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_PLC_PHY_Initialize(
    const SYS_MODULE_INDEX index,
    const SYS_MODULE_INIT * const init
)
{
    DRV_PLC_PHY_INIT* plcPhyInit = (DRV_PLC_PHY_INIT *)init;

    /* Validate the request */
    if(index >= DRV_PLC_PHY_INSTANCES_NUMBER)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    if(gDrvPlcPhyObj.inUse == true)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    gDrvPlcPhyObj.status                = SYS_STATUS_UNINITIALIZED;

    gDrvPlcPhyObj.inUse                 = true;
    gDrvPlcPhyObj.nClients              = 0;

    gDrvPlcPhyObj.plcHal                = plcPhyInit->plcHal;
    gDrvPlcPhyObj.nClientsMax           = plcPhyInit->numClients;
    gDrvPlcPhyObj.plcProfile            = plcPhyInit->plcProfile;
    gDrvPlcPhyObj.binSize               = plcPhyInit->binEndAddress - plcPhyInit->binStartAddress;
    gDrvPlcPhyObj.binStartAddress       = plcPhyInit->binStartAddress;
    gDrvPlcPhyObj.secure                = plcPhyInit->secure;
<#if DRV_PLC_SLEEP_MODE == true>   
    gDrvPlcPhyObj.sleep                 = false;
</#if>
    
    /* Callbacks initialization */
    gDrvPlcPhyObj.txCfmCallback         = 0;
    gDrvPlcPhyObj.dataIndCallback       = 0;
    gDrvPlcPhyObj.exceptionCallback     = 0;
    gDrvPlcPhyObj.bootDataCallback      = 0;
<#if DRV_PLC_SLEEP_MODE == true>     
    gDrvPlcPhyObj.sleepDisableCallback  = 0;
</#if>

    /* HAL init */
    gDrvPlcPhyObj.plcHal->init((DRV_PLC_PLIB_INTERFACE *)plcPhyInit->plcHal->plcPlib);

    /* Update status */
    gDrvPlcPhyObj.status                = SYS_STATUS_BUSY;

<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    if (gDrvPlcPhyObj.semaphoreID == NULL)
    {
        /* Create semaphore. It is used to suspend and resume task */
        OSAL_RESULT semResult = OSAL_SEM_Create(&gDrvPlcPhyObj.semaphoreID, OSAL_SEM_TYPE_BINARY, 0, 0);
        if ((semResult != OSAL_RESULT_TRUE) || (gDrvPlcPhyObj.semaphoreID == NULL))
        {
            /* Error: Not enough memory to create semaphore */
            gDrvPlcPhyObj.status = SYS_STATUS_ERROR;
        }
    }

</#if>
    /* Return the object structure */
    return ( (SYS_MODULE_OBJ)index );

}

SYS_STATUS DRV_PLC_PHY_Status( const SYS_MODULE_INDEX index )
{
    /* Avoid warning */
    (void)index;
    /* Return the driver status */
    return (gDrvPlcPhyObj.status);
}

DRV_HANDLE DRV_PLC_PHY_Open(
    const SYS_MODULE_INDEX index,
    const DRV_PLC_BOOT_DATA_CALLBACK callback
)
{
    DRV_PLC_BOOT_INFO bootInfo;
    
    /* Validate the request */
    if (index >= DRV_PLC_PHY_INSTANCES_NUMBER)
    {
        return DRV_HANDLE_INVALID;
    }

    if((gDrvPlcPhyObj.status != SYS_STATUS_BUSY) || (gDrvPlcPhyObj.inUse == false) \
            || (gDrvPlcPhyObj.nClients >= gDrvPlcPhyObj.nClientsMax))
    {
        return DRV_HANDLE_INVALID;
    }
    
    /* Launch boot start process */  
    bootInfo.binSize = gDrvPlcPhyObj.binSize;
    bootInfo.binStartAddress = gDrvPlcPhyObj.binStartAddress;
    bootInfo.pendingLength = gDrvPlcPhyObj.binSize;
    bootInfo.pSrc = gDrvPlcPhyObj.binStartAddress;    
    bootInfo.secure = gDrvPlcPhyObj.secure;
    if (callback)
    {
        bootInfo.bootDataCallback = callback;
        bootInfo.contextBoot = index;
    }
    else
    {
        bootInfo.bootDataCallback = NULL;
        bootInfo.contextBoot = 0;
    }
    
    DRV_PLC_BOOT_Start(&bootInfo, gDrvPlcPhyObj.plcHal);
    
    gDrvPlcPhyObj.nClients++;

<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    /* Post semaphore to resume task */
    if (gDrvPlcPhyObj.semaphoreID != NULL)
    {
        OSAL_SEM_Post(&gDrvPlcPhyObj.semaphoreID);
    }

</#if>
    return ((DRV_HANDLE)0);
}

void DRV_PLC_PHY_Close( const DRV_HANDLE handle )
{
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvPlcPhyObj.nClients--;
        gDrvPlcPhyObj.inUse = false;
        gDrvPlcPhyObj.status = SYS_STATUS_UNINITIALIZED;
    }
}

void DRV_PLC_PHY_TxCfmCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_PLC_PHY_TX_CFM_CALLBACK callback, 
    const uintptr_t context 
)
{
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvPlcPhyObj.txCfmCallback = callback;
        gDrvPlcPhyObj.contextCfm = context;
    }
}

void DRV_PLC_PHY_DataIndCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_PLC_PHY_DATA_IND_CALLBACK callback, 
    const uintptr_t context 
)
{
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvPlcPhyObj.dataIndCallback = callback;
        gDrvPlcPhyObj.contextInd = context;
    }
}

void DRV_PLC_PHY_ExceptionCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_PLC_PHY_EXCEPTION_CALLBACK callback, 
    const uintptr_t context 
)
{
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvPlcPhyObj.exceptionCallback = callback;
        gDrvPlcPhyObj.contextExc = context;
    }
}

void DRV_PLC_PHY_Tasks( SYS_MODULE_OBJ object )
{
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    /* Suspend task until semaphore is posted or timeout expires */
    if (gDrvPlcPhyObj.semaphoreID != NULL)
    {
        uint16_t waitMS = 1;

        /* If PLC device is running, wait forever. Otherwise, wait for 1 ms  */
        if (gDrvPlcPhyObj.status == SYS_STATUS_READY)
        {
            waitMS = OSAL_WAIT_FOREVER;
        }

        OSAL_SEM_Pend(&gDrvPlcPhyObj.semaphoreID, waitMS);
    }

</#if>
    if (gDrvPlcPhyObj.status == SYS_STATUS_READY)
    {
        /* Run PLC communication task */
        DRV_PLC_PHY_Task();
    }
    else if (gDrvPlcPhyObj.status == SYS_STATUS_BUSY)
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
            DRV_PLC_PHY_Init(&gDrvPlcPhyObj);
            gDrvPlcPhyObj.status = SYS_STATUS_READY;
            gDrvPlcPhyObj.state[0] = DRV_PLC_PHY_STATE_IDLE;
<#if (DRV_PLC_PROFILE == "PRIME")>
            gDrvPlcPhyObj.state[1] = DRV_PLC_PHY_STATE_IDLE;
</#if>
<#if DRV_PLC_SLEEP_MODE == true>

            if (gDrvPlcPhyObj.sleep && gDrvPlcPhyObj.sleepDisableCallback)
            {
                gDrvPlcPhyObj.sleep = false;
                gDrvPlcPhyObj.sleepDisableCallback(gDrvPlcPhyObj.contextSleep);
            }
</#if>            
        }
        else
        {
            gDrvPlcPhyObj.status = SYS_STATUS_ERROR;
            gDrvPlcPhyObj.state[0] = DRV_PLC_PHY_STATE_IDLE;
<#if (DRV_PLC_PROFILE == "PRIME")>
            gDrvPlcPhyObj.state[1] = DRV_PLC_PHY_STATE_IDLE;
</#if>
        }
    } 
    else
    {
        /* SYS_STATUS_ERROR: Nothing to do */
    }
}

<#if DRV_PLC_SLEEP_MODE == true> 
void DRV_PLC_PHY_SleepDisableCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_PLC_PHY_SLEEP_CALLBACK callback, 
    const uintptr_t context 
)
{
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        gDrvPlcPhyObj.sleepDisableCallback = callback;
        gDrvPlcPhyObj.contextSleep = context;
    }
}

void DRV_PLC_PHY_Sleep( const DRV_HANDLE handle, bool enable )
{
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        if (gDrvPlcPhyObj.sleep != enable)
        {
            if (enable)
            {
                /* Disable PLC interrupt */
                gDrvPlcPhyObj.plcHal->enableExtInt(false);
                /* Set Stand By pin */
                gDrvPlcPhyObj.plcHal->setStandBy(true);
                /* Set Sleep flag */
                gDrvPlcPhyObj.sleep = true;
            }
            else
            {
                /* Clear Stand By pin */
                gDrvPlcPhyObj.plcHal->setStandBy(false);
                
                /* Restart from Sleep mode */
                gDrvPlcPhyObj.status = SYS_STATUS_BUSY;
                DRV_PLC_BOOT_Restart(DRV_PLC_BOOT_RESTART_SLEEP);
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">

                /* Post semaphore to resume task */
                if (gDrvPlcPhyObj.semaphoreID != NULL)
                {
                    OSAL_SEM_Post(&gDrvPlcPhyObj.semaphoreID);
                }
</#if>
            }
        }
    }
}
</#if>    

<#if DRV_PLC_MODE == "PL460">
void DRV_PLC_PHY_EnableTX( const DRV_HANDLE handle, bool enable )
{
     if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        /* Set Tx Enable pin */
        gDrvPlcPhyObj.plcHal->setTxEnable(enable);
    }
}
</#if>   