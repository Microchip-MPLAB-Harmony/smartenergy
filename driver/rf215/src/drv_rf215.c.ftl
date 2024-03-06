/******************************************************************************
  RF215 Driver Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_rf215.c

  Summary:
    Source code for the RF215 driver implementation.

  Description:
    The RF215 driver provides a simple interface to manage the PHY layer of
    RF215 transceiver. This file contains the source code for the
    implementation of the RF215 driver.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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
#include "configuration.h"
#include "driver/rf215/drv_rf215.h"
#include "driver/rf215/drv_rf215_local.h"
#include "driver/rf215/hal/rf215_hal.h"
#include "driver/rf215/phy/rf215_phy.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* RF215 register values */
const RF215_REG_VALUES_OBJ rf215RegValues = {
    /* CLKO.OS: Disable clock output; CLKO.DRV: minimum driving */
    .RF_CLKO = (RF215_RF_CLKO_OS_OFF | RF215_RF_CLKO_DRV_2mA),

    /* Rradio IRQ Mask: Enable WAKEUP, TRXRDY, EDC and TRXERR interrupts */
    .RFn_IRQM = RF215_RFn_IRQ_WAKEUP | RF215_RFn_IRQ_TRXRDY | RF215_RFn_IRQ_TRXERR | RF215_RFn_IRQ_EDC,

    /* Transceiver Auxiliary Settings:
     * AVEN=1: Faster state transition, increasing TRXOFF power consumption
     * PAVC: 2.4V Power amplifier voltage control
     * AVEXT: Internal analog supply voltage is used
     * AGCMAP: Internal AGC, no external LNA used
     * EXTLNABYP: Bypass of external LNA not available */
    .RFn_AUXS = RF215_RFn_AUXS_AVEN_EN | RF215_RFn_AUXS_PAVC_2_4V,

    /* Transceiver Command values */
    .RFn_CMD = {
        .sleep = RF215_RFn_CMD_RF_SLEEP,
        .trxoff = RF215_RFn_CMD_RF_TRXOFF,
        .txprep = RF215_RFn_CMD_RF_TXPREP,
        .tx = RF215_RFn_CMD_RF_TX,
        .rx = RF215_RFn_CMD_RF_RX,
        .reset = RF215_RFn_CMD_RF_RESET
    },
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>

    /* Counter Configuration:
     * EN=1: Enable counter
     * RSTRXS=1: Reset counter at RX frame start event
     * RSTTXS=1: Reset counter at TX start event
     * CAPRXS=CAPTXS=0: Free-running mode */
    .BBCn_CNTC = RF215_BBCn_CNTC_EN | RF215_BBCn_CNTC_RSTRXS | RF215_BBCn_CNTC_RSTTXS
</#if>
};

static const DRV_RF215_FW_VERSION rf215FwVersion = {
    .major = 2,
    .minor = 0,
    .revision = 1,
    .day = 22,
    .month = 2,
    .year = 23
};

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data
// *****************************************************************************
// *****************************************************************************

/* RF215 Driver instance object */
static DRV_RF215_OBJ drvRf215Obj = {0};

/* Client object pool */
static DRV_RF215_CLIENT_OBJ drvRf215ClientPool[DRV_RF215_CLIENTS_NUMBER] = {0};

/* Transmission buffer object pool */
static DRV_RF215_TX_BUFFER_OBJ drvRf215TxBufPool[DRV_RF215_TX_BUFFERS_NUMBER] = {0};

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************

static inline uint32_t _DRV_RF215_MakeHandle (uint8_t index)
{
    /* Build handle and update token count. There is no issue if it overflows */
    uint32_t handle = ((uint32_t) (drvRf215Obj.tokenCount++) << 16) | index;

    return handle;
}

static DRV_RF215_CLIENT_OBJ* _DRV_RF215_DrvHandleValidate(DRV_HANDLE handle)
{
    uint8_t clientIndex;
    DRV_RF215_CLIENT_OBJ* clientObj;

    /* Extract the client index from the handle */
    clientIndex = handle & 0xFF;
    if (clientIndex > DRV_RF215_CLIENTS_NUMBER)
    {
        return NULL;
    }

    /* Obtain the object and check if it corresponds to the handle */
    clientObj = &drvRf215ClientPool[clientIndex];
    if ((clientObj->clientHandle != handle) || (clientObj->inUse == false))
    {
        clientObj = NULL;
    }

    return clientObj;
}

static void _DRV_RF215_Timeout(uintptr_t context)
{
    DRV_RF215_OBJ* dObj = (DRV_RF215_OBJ *)context;
    dObj->timeoutErr = true;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    /* Post semaphore to resume task */
    OSAL_SEM_PostISR(&dObj->semaphoreID);

</#if>
}

static void _DRV_RF215_ReadPNVN(uintptr_t context, void* pData<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, uint64_t time</#if>)
{
    DRV_RF215_OBJ* dObj = (DRV_RF215_OBJ *) context;
    uint8_t* pPN = pData;

<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    /* Post semaphore to resume task */
    OSAL_SEM_PostISR(&dObj->semaphoreID);

</#if>
    if ((pPN[0] != RF215_RF_PN_AT86RF215) || (pPN[1] != RF215_RF_VN_V3))
    {
        /* Wrong part number read */
        dObj->partNumErr = true;
        return;
    }

    /* Disable clock output by default (not used) */
    RF215_HAL_SpiWrite(RF215_RF_CLKO, (void *)&rf215RegValues.RF_CLKO, 1);

<#if DRV_RF215_TRX09_EN == false>
    /* RF09 TRX disabled: Switch it to sleep state to save power */
    RF215_HAL_SpiWrite(RF215_ADDR_RF09_CMD, (void *) &rf215RegValues.RFn_CMD.sleep, 1);

</#if>
<#if DRV_RF215_TRX24_EN == false>
    /* RF24 TRX disabled: Switch it to sleep state to save power */
    RF215_HAL_SpiWrite(RF215_ADDR_RF24_CMD, (void *) &rf215RegValues.RFn_CMD.sleep, 1);

</#if>
<#if DRV_RF215_TRX09_EN == true>
    /* RF09 TRX reset event */
    RF215_PHY_ExtIntEvent(RF215_TRX_RF09_IDX, RF215_RFn_IRQ_WAKEUP, 0);

</#if>
<#if DRV_RF215_TRX24_EN == true>
    /* RF24 TRX reset event */
    RF215_PHY_ExtIntEvent(RF215_TRX_RF24_IDX, RF215_RFn_IRQ_WAKEUP, 0);

</#if>
    /* RF215 Chip Reset handled correctly. The driver is ready */
    dObj->rfChipResetFlag = true;
}

static void _DRV_RF215_ReadIRQS(uintptr_t context, void* pData<#if DRV_RF215_TXRX_TIME_SUPPORT == true>, uint64_t time</#if>)
{
    DRV_RF215_OBJ* dObj = (DRV_RF215_OBJ *) context;
    uint8_t* pFlags = pData;
    uint8_t rf09IRQS = pFlags[0];
    uint8_t rf24IRQS = pFlags[1];
    uint8_t bbc0IRQS = pFlags[2];
<#if DRV_RF215_TRX24_EN == true>
    uint8_t bbc1IRQS = pFlags[3];
</#if>

    /* Check if object is not initialized or in error state */
    if (dObj->sysStatus <= SYS_STATUS_UNINITIALIZED)
    {
        return;
    }

    if ((dObj->sysStatus == SYS_STATUS_BUSY) && (dObj->rfChipResetFlag == false))
    {
        /* First interrupt after initialization: Chip Reset / Power-On-Reset
         * should be indicated */
<#if DRV_RF215_TRX24_EN == false>
        if (((rf09IRQS & rf24IRQS) != RF215_RFn_IRQ_WAKEUP) || (bbc0IRQS != 0))

<#else>
        if (((rf09IRQS & rf24IRQS) != RF215_RFn_IRQ_WAKEUP) || ((bbc0IRQS | bbc1IRQS) != 0))
</#if>
        {
            RF215_HAL_Deinitialize();
            dObj->irqsErr = true;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">

            /* Post semaphore to resume task */
            OSAL_SEM_PostISR(&dObj->semaphoreID);
</#if>
            return;
        }
    }
    else if (((rf09IRQS & 0xC0) != 0) || ((rf24IRQS & 0xC0) != 0))
    {
        /* 2 MSB bits of RFn_IRQS should be always 0 */
        dObj->irqsErr = true;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">

        /* Post semaphore to resume task */
        OSAL_SEM_PostISR(&dObj->semaphoreID);
</#if>
        return;
    }
<#if DRV_RF215_TRX24_EN == false>
    else if ((rf09IRQS | rf24IRQS | bbc0IRQS) == 0)
<#else>
    else if ((rf09IRQS | rf24IRQS | bbc0IRQS | bbc1IRQS) == 0)
</#if>
    {
        /* Count consecutive times reading RF215 flags as empty */
        if (dObj->irqsEmptyCount == 3)
        {
            dObj->irqsErr = true;
            dObj->irqsEmptyCount = 0;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">

            /* Post semaphore to resume task */
            OSAL_SEM_PostISR(&dObj->semaphoreID);
</#if>
            return;
        }
    }
    else
    {
        dObj->irqsEmptyCount = 0;
    }

    /* Check Chip Reset / Power-On-Reset (Wake-up Interrupt on both TRX) */
    if ((rf09IRQS & rf24IRQS & RF215_RFn_IRQ_WAKEUP) != 0)
    {
        /* Read Device Part Number and Version Number registers */
        uint8_t* pPN = &dObj->RF_PN;
        RF215_HAL_SpiRead(RF215_RF_PN, pPN, 2, _DRV_RF215_ReadPNVN, context);
        RF215_PHY_DeviceReset();
        return;
    }
<#if DRV_RF215_TRX09_EN == true>

    /* RF09 interrupts */
    RF215_PHY_ExtIntEvent(RF215_TRX_RF09_IDX, rf09IRQS, bbc0IRQS);
</#if>
<#if DRV_RF215_TRX24_EN == true>

    /* RF24 interrupts */
    RF215_PHY_ExtIntEvent(RF215_TRX_RF24_IDX, rf24IRQS, bbc1IRQS);
</#if>
}

// *****************************************************************************
// *****************************************************************************
// Section: RF215 Driver Local Functions
// *****************************************************************************
// *****************************************************************************

void DRV_RF215_ExtIntHandler(void)
{
    uint8_t* pFlags = &drvRf215Obj.RF09_IRQS;
    uintptr_t context = (uintptr_t) &drvRf215Obj;

    /* Check if object is not initialized or in error state */
    if (drvRf215Obj.sysStatus <= SYS_STATUS_UNINITIALIZED)
    {
        return;
    }

<#if DRV_RF215_TRX24_EN == false>
    /* Read IRQ Status registers (RF09_IRQS, RF24_IRQS, BBC0_IRQS) */
    RF215_HAL_SpiRead(RF215_RF09_IRQS, pFlags, 3, _DRV_RF215_ReadIRQS, context);
<#else>
    /* Read IRQS registers (RF09_IRQS, RF24_IRQS, BBC0_IRQS, BBC1_IRQS) */
    RF215_HAL_SpiRead(RF215_RF09_IRQS, pFlags, 4, _DRV_RF215_ReadIRQS, context);
</#if>
}

void DRV_RF215_NotifyRxInd(uint8_t trxIdx, DRV_RF215_RX_INDICATION_OBJ* ind)
{
    uint8_t clientIdx;
    for (clientIdx = 0; clientIdx < DRV_RF215_CLIENTS_NUMBER; clientIdx++)
    {
        DRV_RF215_CLIENT_OBJ* clientObj = &drvRf215ClientPool[clientIdx];

        /* Check if the client registered RX indication callback for this TRX */
        if ((clientObj->inUse == true) && (clientObj->trxIndex == trxIdx) &&
                (clientObj->rxIndCallback != NULL))
        {
            clientObj->rxIndCallback(ind, clientObj->rxIndContext);
        }
    }
}

<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
void DRV_RF215_AbortTxByRx(uint8_t trxIdx)
{
    /* Look for scheduled TX to cancel by RX */
    for (uint8_t bufIdx = 0; bufIdx < DRV_RF215_TX_BUFFERS_NUMBER; bufIdx++)
    {
        DRV_RF215_TX_BUFFER_OBJ* txBufObj = &drvRf215TxBufPool[bufIdx];
        if ((txBufObj->inUse == true) && (txBufObj->clientObj->trxIndex == trxIdx))
        {
            if (txBufObj->reqObj.cancelByRx == true)
            {
                RF215_PHY_SetTxCfm(txBufObj, RF215_TX_CANCEL_BY_RX);
            }
<#if DRV_RF215_CCA_CONTENTION_WINDOW == true>
            else if (RF215_PHY_CheckTxContentionWindow(txBufObj) == true)
            {
                RF215_PHY_SetTxCfm(txBufObj, RF215_TX_BUSY_RX);
            }
</#if>
        }
    }
}

</#if>
<#if DRV_RF215_FSK_EN == true && DRV_RF215_OFDM_EN == true>
void DRV_RF215_AbortTxByPhyConfig(uint8_t trxIdx)
{
    /* Look for scheduled TX to cancel by PHY configuration */
    for (uint8_t bufIdx = 0; bufIdx < DRV_RF215_TX_BUFFERS_NUMBER; bufIdx++)
    {
        DRV_RF215_TX_BUFFER_OBJ* txBufObj = &drvRf215TxBufPool[bufIdx];
        if ((txBufObj->inUse == true) && (txBufObj->cfmPending = false) && (txBufObj->clientObj->trxIndex == trxIdx))
        {
            RF215_PHY_SetTxCfm(txBufObj, RF215_TX_ABORTED);
        }
    }
}

</#if>
<#if DRV_RF215_FREQ_HOPPING_SUPPORT == true>
void DRV_RF215_NotifyChannelSwitch(uint8_t trxIdx, DRV_RF215_TX_RESULT result)
{
    uint8_t clientIdx;
    for (clientIdx = 0; clientIdx < DRV_RF215_CLIENTS_NUMBER; clientIdx++)
    {
        DRV_RF215_CLIENT_OBJ* clientObj = &drvRf215ClientPool[clientIdx];

        /* Check if the client registered set channel notification callback for
         * this TRX */
        if ((clientObj->inUse == true) && (clientObj->trxIndex == trxIdx) &&
                (clientObj->setChnCallback != NULL))
        {
            clientObj->setChnCallback(result, clientObj->setChnContext);
        }
    }
}

</#if>
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
void DRV_RF215_ResumeTask(void)
{
    /* Post semaphore to resume task */
    OSAL_SEM_PostISR(&drvRf215Obj.semaphoreID);
}

</#if>
// *****************************************************************************
// *****************************************************************************
// Section: RF215 Driver Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_RF215_Initialize (
    const SYS_MODULE_INDEX index,
    const SYS_MODULE_INIT * const init
)
{
    const DRV_RF215_INIT* rfPhyInit = (DRV_RF215_INIT *)init;
    DRV_RF215_PHY_BAND_OPM bandOpMode;
    uint16_t channelNum;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    OSAL_RESULT semResult;
</#if>

    /* Validate the instance index */
    if (index != DRV_RF215_INDEX_0)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Check if driver instance has already been initialized */
    if (drvRf215Obj.sysStatus > SYS_STATUS_UNINITIALIZED)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

<#if DRV_RF215_TRX09_EN == true>
    /* Initialize RF215 PHY (Sub-GHz transceiver) */
    bandOpMode = rfPhyInit->rf09PhyBandOpmIni;
    channelNum = rfPhyInit->rf09PhyChnNumIni;
    if (RF215_PHY_Initialize(RF215_TRX_RF09_IDX, bandOpMode, channelNum) == false)
    {
        /* Invalid PHY configuration (Sub-GHz transceiver) */
        return SYS_MODULE_OBJ_INVALID;
    }

</#if>
<#if DRV_RF215_TRX24_EN == true>
    /* Initialize RF215 PHY (2.4 GHz transceiver) */
    bandOpMode = rfPhyInit->rf24PhyBandOpmIni;
    channelNum = rfPhyInit->rf24PhyChnNumIni;
    if (RF215_PHY_Initialize(RF215_TRX_RF24_IDX, bandOpMode, channelNum) == false)
    {
        /* Invalid PHY configuration (2.4 GHz transceiver) */
        return SYS_MODULE_OBJ_INVALID;
    }

</#if>
    /* Initialize Hardware Abstraction Layer */
    RF215_HAL_Initialize(rfPhyInit);

    /* Reset RF device in the first task */
    drvRf215Obj.rfChipResetPending = true;
    drvRf215Obj.timeoutHandle = SYS_TIME_HANDLE_INVALID;

<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    /* Create semaphore. It is used to suspend and resume task. Initialized in
     * posted status */
    semResult = OSAL_SEM_Create(&drvRf215Obj.semaphoreID, OSAL_SEM_TYPE_BINARY, 1, 1);
    if ((semResult == OSAL_RESULT_SUCCESS) && (drvRf215Obj.semaphoreID != NULL))
    {
        /* Set busy status. Initialization will continue from interrupt */
        drvRf215Obj.sysStatus = SYS_STATUS_BUSY;
    }
    else
    {
        /* Error: Not enough memory to create semaphore */
        drvRf215Obj.sysStatus = SYS_STATUS_ERROR;
    }
<#else>
    /* Set busy status. Initialization will continue from interrupt */
    drvRf215Obj.sysStatus = SYS_STATUS_BUSY;
</#if>

    /* Zero initialization */
    drvRf215Obj.readyStatusCallback = NULL;
    drvRf215Obj.irqsEmptyCount = 0;
    drvRf215Obj.irqsErr = false;
    drvRf215Obj.partNumErr = false;
    drvRf215Obj.timeoutErr = false;
    drvRf215Obj.rfChipResetFlag = false;
    drvRf215Obj.readyStatusNotified = false;
    for (uint8_t idx = 0; idx < DRV_RF215_CLIENTS_NUMBER; idx++)
    {
        DRV_RF215_CLIENT_OBJ* clientObj = &drvRf215ClientPool[idx];
        clientObj->rxIndCallback = NULL;
        clientObj->txCfmCallback = NULL;
<#if DRV_RF215_FREQ_HOPPING_SUPPORT == true>
        clientObj->setChnCallback = NULL;
</#if>
        clientObj->inUse = false;
    }

    for (uint8_t idx = 0; idx < DRV_RF215_TX_BUFFERS_NUMBER; idx++)
    {
        drvRf215TxBufPool[idx].inUse = false;
    }

    return (SYS_MODULE_OBJ) index;
}

SYS_STATUS DRV_RF215_Status( SYS_MODULE_OBJ object )
{
    /* Validate instance object (returned from DRV_RF215_Initialize) */
    if (object != DRV_RF215_INDEX_0)
    {
        return SYS_STATUS_ERROR;
    }

    return drvRf215Obj.sysStatus;
}

void DRV_RF215_Tasks( SYS_MODULE_OBJ object )
{
    DRV_RF215_OBJ* dObj = &drvRf215Obj;

    /* Validate instance object (returned from DRV_RF215_Initialize) */
    if (object != DRV_RF215_INDEX_0)
    {
        return;
    }

<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    /* Suspend task until semaphore is posted */
    if (dObj->semaphoreID != NULL)
    {
        OSAL_SEM_Pend(&dObj->semaphoreID, OSAL_WAIT_FOREVER);
    }

</#if>
    switch (dObj->sysStatus)
    {
        case SYS_STATUS_BUSY:
        {
            if (dObj->rfChipResetPending == true)
            {
                /* Reset RF device */
                RF215_HAL_Reset();
                dObj->rfChipResetPending = false;
            }

            /* Hardware Abstraction Layer tasks */
            RF215_HAL_Tasks();

            if (dObj->timeoutHandle == SYS_TIME_HANDLE_INVALID)
            {
                /* Register initialization timeout callback */
                dObj->timeoutHandle = SYS_TIME_CallbackRegisterMS(
                        _DRV_RF215_Timeout, (uintptr_t) dObj, 5, SYS_TIME_SINGLE);
            }

            if ((dObj->timeoutErr == true) || (dObj->irqsErr == true) || (dObj->partNumErr == true))
            {
                RF215_HAL_Deinitialize();
                dObj->sysStatus = SYS_STATUS_ERROR;
            }
            else if (dObj->rfChipResetFlag == true)
            {
                SYS_TIME_TimerDestroy(dObj->timeoutHandle);
                dObj->rfChipResetFlag = false;
                dObj->sysStatus = SYS_STATUS_READY;
            }

            break;
        }

        case SYS_STATUS_READY:
        {
            uint8_t bufIdx;

            /* Ready status notification */
            if ((dObj->readyStatusCallback != NULL) && (dObj->readyStatusNotified == false))
            {
                dObj->readyStatusCallback(dObj->readyStatusContext, SYS_STATUS_READY);
                dObj->readyStatusNotified = true;
            }

            /* Hardware Abstraction Layer tasks */
            RF215_HAL_Tasks();

            /* Look for pending TX confirms */
            for (bufIdx = 0; bufIdx < DRV_RF215_TX_BUFFERS_NUMBER; bufIdx++)
            {
                DRV_RF215_TX_BUFFER_OBJ* txBufObj = &drvRf215TxBufPool[bufIdx];
                if ((txBufObj->inUse == true) && (txBufObj->cfmPending == true))
                {
                    bool reportCfm = false;
                    DRV_RF215_CLIENT_OBJ* clientObj = txBufObj->clientObj;

                    /* Critical region to avoid changes from interrupts */
                    RF215_HAL_EnterCritical();

                    /* Check flag again (it could be modified from interrupt) */
                    if (txBufObj->cfmPending == true)
                    {
                        /* Set TX buffer as free before reporting TX confirm */
                        txBufObj->inUse = false;
                        reportCfm = true;
                    }

                    /* Leave critical region. TX confirm ready to be notified */
                    RF215_HAL_LeaveCritical();

                    if ((reportCfm == true) && (clientObj->inUse == true) &&
                            (clientObj->txCfmCallback != NULL))
                    {
                        clientObj->txCfmCallback(txBufObj->txHandle,
                                &txBufObj->cfmObj, clientObj->txCfmContext);
                    }
                }
            }

            /* PHY tasks */
<#if DRV_RF215_TRX09_EN == true>
            RF215_PHY_Tasks(RF215_TRX_RF09_IDX);
</#if>
<#if DRV_RF215_TRX24_EN == true>
            RF215_PHY_Tasks(RF215_TRX_RF24_IDX);
</#if>
            break;
        }

        case SYS_STATUS_ERROR:
        {
            /* Error status notification */
            if ((dObj->readyStatusCallback != NULL) && (dObj->readyStatusNotified == false))
            {
                dObj->readyStatusCallback(dObj->readyStatusContext, SYS_STATUS_ERROR);
                dObj->readyStatusNotified = true;
            }
            break;
        }

        case SYS_STATUS_UNINITIALIZED:
        default:
        {
            break;
        }
    }
}

void DRV_RF215_ReadyStatusCallbackRegister (
    const SYS_MODULE_INDEX index,
    const DRV_RF215_READY_STATUS_CALLBACK callback,
    uintptr_t context
)
{
    /* Validate the instance index */
    if (index != DRV_RF215_INDEX_0)
    {
        return;
    }

    /* Register ready status callback */
    drvRf215Obj.readyStatusCallback = callback;
    drvRf215Obj.readyStatusContext = context;
    drvRf215Obj.readyStatusNotified = false;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">

    /* Post semaphore to resume task */
    if (drvRf215Obj.semaphoreID != NULL)
    {
        OSAL_SEM_Post(&drvRf215Obj.semaphoreID);
    }
</#if>
}

DRV_HANDLE DRV_RF215_Open (
    const SYS_MODULE_INDEX index,
    const DRV_RF215_TRX_ID trxID
)
{
    uint8_t clientIdx;
<#if DRV_RF215_TRX09_EN == true>
    uint8_t trxIdx = RF215_TRX_RF09_IDX;
<#else>
    uint8_t trxIdx = RF215_TRX_RF24_IDX;
</#if>

    /* Validate the transceiver identifier */
<#if DRV_RF215_TRX09_EN == true && DRV_RF215_TRX24_EN>
    if (trxID == RF215_TRX_ID_RF24)
    {
        trxIdx = RF215_TRX_RF24_IDX;
    }
    else if (trxID != RF215_TRX_ID_RF09)
    {
        /* Invalid transceiver */
        return DRV_HANDLE_INVALID;
    }
<#elseif DRV_RF215_TRX09_EN == true>
    if (trxID != RF215_TRX_ID_RF09)
    {
        /* Invalid transceiver */
        return DRV_HANDLE_INVALID;
    }
<#else>
    if (trxID != RF215_TRX_ID_RF24)
    {
        /* Invalid transceiver */
        return DRV_HANDLE_INVALID;
    }
</#if>

    /* Validate the instance index */
    if (index != DRV_RF215_INDEX_0)
    {
        return DRV_HANDLE_INVALID;
    }

    /* Check if driver instance is ready */
    if (drvRf215Obj.sysStatus < SYS_STATUS_READY)
    {
        return DRV_HANDLE_INVALID;
    }

    /* Look for a free client object in the pool */
    for (clientIdx = 0; clientIdx < DRV_RF215_CLIENTS_NUMBER; clientIdx++)
    {
        DRV_RF215_CLIENT_OBJ* clientObj = &drvRf215ClientPool[clientIdx];
        if (clientObj->inUse == false)
        {
            clientObj->inUse = true;
            clientObj->trxIndex = trxIdx;
            clientObj->rxIndCallback = NULL;
            clientObj->txCfmCallback = NULL;
            clientObj->clientHandle = _DRV_RF215_MakeHandle(clientIdx);
            return clientObj->clientHandle;
        }
    }

    /* Could not find a free client object */
    return DRV_HANDLE_INVALID;
}

void DRV_RF215_Close( const DRV_HANDLE drvHandle )
{
    DRV_RF215_CLIENT_OBJ* clientObj = _DRV_RF215_DrvHandleValidate(drvHandle);
    if (clientObj != NULL)
    {
        /* Set client object as free */
        clientObj->inUse = false;
    }
}

void DRV_RF215_RxIndCallbackRegister (
    DRV_HANDLE drvHandle,
    const DRV_RF215_RX_IND_CALLBACK callback,
    uintptr_t context
)
{
    DRV_RF215_CLIENT_OBJ* clientObj = _DRV_RF215_DrvHandleValidate(drvHandle);
    if (clientObj != NULL)
    {
        /* Register RX indication callback for this client */
        clientObj->rxIndCallback = callback;
        clientObj->rxIndContext = context;
    }
}

void DRV_RF215_TxCfmCallbackRegister (
    DRV_HANDLE drvHandle,
    const DRV_RF215_TX_CFM_CALLBACK callback,
    uintptr_t context
)
{
    DRV_RF215_CLIENT_OBJ* clientObj = _DRV_RF215_DrvHandleValidate(drvHandle);
    if (clientObj != NULL)
    {
        /* Register TX confirm callback for this client */
        clientObj->txCfmCallback = callback;
        clientObj->txCfmContext = context;
    }
}

<#if DRV_RF215_FREQ_HOPPING_SUPPORT == true>
void DRV_RF215_SetChannelCallbackRegister (
    DRV_HANDLE drvHandle,
    const DRV_RF215_SET_CHANNEL_CALLBACK callback,
    uintptr_t context
)
{
    DRV_RF215_CLIENT_OBJ* clientObj = _DRV_RF215_DrvHandleValidate(drvHandle);
    if (clientObj != NULL)
    {
        /* Register TX confirm callback for this client */
        clientObj->setChnCallback = callback;
        clientObj->setChnContext = context;
    }
}

</#if>
DRV_RF215_TX_HANDLE DRV_RF215_TxRequest (
    DRV_HANDLE drvHandle,
    DRV_RF215_TX_REQUEST_OBJ* reqObj,
    DRV_RF215_TX_RESULT* result
)
{
    uint8_t bufIdx;

    DRV_RF215_CLIENT_OBJ* clientObj = _DRV_RF215_DrvHandleValidate(drvHandle);
    if (clientObj == NULL)
    {
        *result = RF215_TX_INVALID_DRV_HANDLE;
        return DRV_RF215_TX_HANDLE_INVALID;
    }

    /* Look for a free TX buffer object in the pool */
    for (bufIdx = 0; bufIdx < DRV_RF215_TX_BUFFERS_NUMBER; bufIdx++)
    {
        DRV_RF215_TX_BUFFER_OBJ* txBufObj = &drvRf215TxBufPool[bufIdx];
        if (txBufObj->inUse == false)
        {
            /* Critical region to avoid conflicts */
            RF215_HAL_EnterCritical();

            /* Initialize RF215 driver's TX buffer. Copy PSDU. */
            txBufObj->clientObj = clientObj;
            txBufObj->reqObj = *reqObj;
            txBufObj->txHandle = _DRV_RF215_MakeHandle(bufIdx);
            txBufObj->inUse = true;
            txBufObj->cfmPending = false;
            memcpy(txBufObj->psdu, reqObj->psdu, reqObj->psduLen);

            /* Transmission request */
            *result = RF215_PHY_TxRequest(txBufObj);
            if (*result == RF215_TX_SUCCESS)
            {
                /* Transmission scheduled and buffer allocated (set as used).
                 * Result will be notified via callback. */
                RF215_HAL_LeaveCritical();
                return txBufObj->txHandle;
            }
            else
            {
                /* Transmission error. Buffer is not scheduled. */
                txBufObj->inUse = false;
                RF215_HAL_LeaveCritical();
                return DRV_HANDLE_INVALID;
            }
        }
    }

    /* Could not find a free TX buffer object */
    *result = RF215_TX_FULL_BUFFERS;
    return DRV_HANDLE_INVALID;
}

void DRV_RF215_TxCancel(DRV_HANDLE drvHandle, DRV_RF215_TX_HANDLE txHandle)
{
    DRV_RF215_CLIENT_OBJ* clientObj;
    DRV_RF215_TX_BUFFER_OBJ* txBufObj;
    uint8_t bufIdx;

    clientObj = _DRV_RF215_DrvHandleValidate(drvHandle);
    if (clientObj == NULL)
    {
        return;
    }

    /* Validate TX handle */
    bufIdx = txHandle & 0xFF;
    if (bufIdx > DRV_RF215_TX_BUFFERS_NUMBER)
    {
        return;
    }

    /* Obtain the TX buffer object */
    txBufObj = &drvRf215TxBufPool[bufIdx];
    if ((txBufObj->txHandle == txHandle) && (txBufObj->inUse == true))
    {
        /* Critical region to avoid changes from interrupts */
        RF215_HAL_EnterCritical();

        /* Check that TX has not finished */
        if (txBufObj->cfmPending == false)
        {
            RF215_PHY_TxCancel(txBufObj);
        }

        /* Leave critical region. TX confirm ready to be notified */
        RF215_HAL_LeaveCritical();
    }
}

<#if DRV_RF215_FREQ_HOPPING_SUPPORT == true>
void DRV_RF215_SetChannelRequest (
    DRV_HANDLE drvHandle,
    uint64_t timeCount,
    uint16_t channelNum,
    DRV_RF215_TX_TIME_MODE timeMode
)
{
    DRV_RF215_CLIENT_OBJ* clientObj = _DRV_RF215_DrvHandleValidate(drvHandle);
    if (clientObj == NULL)
    {
        return;
    }

    /* Set channel request */
    RF215_PHY_SetChannelRequest(clientObj->trxIndex, timeCount, channelNum, timeMode);
}

</#if>
uint8_t DRV_RF215_GetPibSize(DRV_RF215_PIB_ATTRIBUTE attr)
{
    uint8_t len;

    switch (attr)
    {
        case RF215_PIB_PHY_STATS_RESET:
        case RF215_PIB_DEVICE_RESET:
        case RF215_PIB_TRX_RESET:
        case RF215_PIB_TRX_SLEEP:
        case RF215_PIB_PHY_TX_CONTINUOUS:
            len = sizeof(uint8_t);
            break;

        case RF215_PIB_DEVICE_ID:
        case RF215_PIB_PHY_CHANNEL_NUM:
        case RF215_PIB_PHY_TX_PAY_SYMBOLS:
        case RF215_PIB_PHY_RX_PAY_SYMBOLS:
        case RF215_PIB_PHY_CCA_ED_DURATION:
        case RF215_PIB_PHY_TURNAROUND_TIME:
        case RF215_PIB_MAC_UNIT_BACKOFF_PERIOD:
            len = sizeof(uint16_t);
            break;

        case RF215_PIB_PHY_CHANNEL_FREQ_HZ:
        case RF215_PIB_PHY_TX_TOTAL:
        case RF215_PIB_PHY_TX_TOTAL_BYTES:
        case RF215_PIB_PHY_TX_ERR_TOTAL:
        case RF215_PIB_PHY_TX_ERR_BUSY_TX:
        case RF215_PIB_PHY_TX_ERR_BUSY_RX:
        case RF215_PIB_PHY_TX_ERR_BUSY_CHN:
        case RF215_PIB_PHY_TX_ERR_BAD_LEN:
        case RF215_PIB_PHY_TX_ERR_BAD_FORMAT:
        case RF215_PIB_PHY_TX_ERR_TIMEOUT:
        case RF215_PIB_PHY_TX_ERR_ABORTED:
        case RF215_PIB_PHY_TX_CFM_NOT_HANDLED:
        case RF215_PIB_PHY_RX_TOTAL:
        case RF215_PIB_PHY_RX_TOTAL_BYTES:
        case RF215_PIB_PHY_RX_ERR_TOTAL:
        case RF215_PIB_PHY_RX_ERR_FALSE_POSITIVE:
        case RF215_PIB_PHY_RX_ERR_BAD_LEN:
        case RF215_PIB_PHY_RX_ERR_BAD_FORMAT:
        case RF215_PIB_PHY_RX_ERR_BAD_FCS_PAY:
        case RF215_PIB_PHY_RX_ERR_ABORTED:
        case RF215_PIB_PHY_RX_OVERRIDE:
        case RF215_PIB_PHY_RX_IND_NOT_HANDLED:
            len = sizeof(uint32_t);
            break;

        case RF215_PIB_PHY_CCA_ED_THRESHOLD:
            len = sizeof(int8_t);
            break;

        case RF215_PIB_FW_VERSION:
            len = sizeof(DRV_RF215_FW_VERSION);
            break;

        case RF215_PIB_PHY_CONFIG:
            len = sizeof(DRV_RF215_PHY_CFG_OBJ);
            break;

        case RF215_PIB_PHY_BAND_OPERATING_MODE:
            len = sizeof(DRV_RF215_PHY_BAND_OPM);
            break;

        default:
            len = 0;
            break;
    }

    return len;
}

DRV_RF215_PIB_RESULT DRV_RF215_GetPib (
    DRV_HANDLE drvHandle,
    DRV_RF215_PIB_ATTRIBUTE attr,
    void* value
)
{
    DRV_RF215_CLIENT_OBJ* clientObj;

    clientObj = _DRV_RF215_DrvHandleValidate(drvHandle);
    if (clientObj == NULL)
    {
        return RF215_PIB_RESULT_INVALID_HANDLE;
    }

    switch (attr)
    {
        case RF215_PIB_DEVICE_RESET:
        case RF215_PIB_TRX_RESET:
        case RF215_PIB_PHY_STATS_RESET:
            return RF215_PIB_RESULT_WRITE_ONLY;

        case RF215_PIB_DEVICE_ID:
            *((uint16_t *) value) = 0x215;
            break;

        case RF215_PIB_FW_VERSION:
            memcpy(value, &rf215FwVersion, sizeof(rf215FwVersion));
            break;

        default:
            return RF215_PHY_GetPib(clientObj->trxIndex, attr, value);
    }

    return RF215_PIB_RESULT_SUCCESS;
}

DRV_RF215_PIB_RESULT DRV_RF215_SetPib (
    DRV_HANDLE drvHandle,
    DRV_RF215_PIB_ATTRIBUTE attr,
    void* value
)
{
    DRV_RF215_CLIENT_OBJ* clientObj;

    clientObj = _DRV_RF215_DrvHandleValidate(drvHandle);
    if (clientObj == NULL)
    {
        return RF215_PIB_RESULT_INVALID_HANDLE;
    }

    switch (attr)
    {
        case RF215_PIB_DEVICE_ID:
        case RF215_PIB_FW_VERSION:
        case RF215_PIB_PHY_CHANNEL_FREQ_HZ:
        case RF215_PIB_PHY_TURNAROUND_TIME:
        case RF215_PIB_PHY_TX_PAY_SYMBOLS:
        case RF215_PIB_PHY_RX_PAY_SYMBOLS:
        case RF215_PIB_PHY_TX_TOTAL:
        case RF215_PIB_PHY_TX_TOTAL_BYTES:
        case RF215_PIB_PHY_TX_ERR_TOTAL:
        case RF215_PIB_PHY_TX_ERR_BUSY_TX:
        case RF215_PIB_PHY_TX_ERR_BUSY_RX:
        case RF215_PIB_PHY_TX_ERR_BUSY_CHN:
        case RF215_PIB_PHY_TX_ERR_BAD_LEN:
        case RF215_PIB_PHY_TX_ERR_BAD_FORMAT:
        case RF215_PIB_PHY_TX_ERR_TIMEOUT:
        case RF215_PIB_PHY_TX_ERR_ABORTED:
        case RF215_PIB_PHY_TX_CFM_NOT_HANDLED:
        case RF215_PIB_PHY_RX_TOTAL:
        case RF215_PIB_PHY_RX_TOTAL_BYTES:
        case RF215_PIB_PHY_RX_ERR_TOTAL:
        case RF215_PIB_PHY_RX_ERR_FALSE_POSITIVE:
        case RF215_PIB_PHY_RX_ERR_BAD_LEN:
        case RF215_PIB_PHY_RX_ERR_BAD_FORMAT:
        case RF215_PIB_PHY_RX_ERR_BAD_FCS_PAY:
        case RF215_PIB_PHY_RX_ERR_ABORTED:
        case RF215_PIB_PHY_RX_OVERRIDE:
        case RF215_PIB_PHY_RX_IND_NOT_HANDLED:
        case RF215_PIB_MAC_UNIT_BACKOFF_PERIOD:
            return RF215_PIB_RESULT_READ_ONLY;

        case RF215_PIB_DEVICE_RESET:
            /* Critical region to avoid conflicts in PHY object data */
            RF215_HAL_EnterCritical();

            RF215_HAL_Reset();
<#if DRV_RF215_TRX09_EN == true && DRV_RF215_TRX24_EN>
            RF215_PHY_Reset(RF215_TRX_RF09_IDX);
            RF215_PHY_Reset(RF215_TRX_RF24_IDX);
<#else>
            RF215_PHY_Reset(clientObj->trxIndex);
</#if>

            /* Leave critical region */
            RF215_HAL_LeaveCritical();
            break;

        default:
            return RF215_PHY_SetPib(clientObj->trxIndex, attr, value);
    }

    return RF215_PIB_RESULT_SUCCESS;
}
