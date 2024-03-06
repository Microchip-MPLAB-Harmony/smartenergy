/******************************************************************************
  DRV_PLC_PHY G3 Profile Layer

  Company:
    Microchip Technology Inc.

  File Name:
    drv_plc_phy_comm.c

  Summary:
    PLC Driver G3 Profile Layer

  Description:
    This file contains the source code for the implementation of the G3 Profile 
    Layer.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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
#include "system/system.h"
#include "driver/plc/phy/drv_plc_phy.h"
#include "driver/plc/common/drv_plc_hal.h"
#include "driver/plc/common/drv_plc_boot.h"
#include "driver/plc/phy/drv_plc_phy_local_comm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* This is the driver instance object array. */
static DRV_PLC_PHY_OBJ *gPlcPhyObj;

/* Buffer definition to communicate with PLC */
static CACHE_ALIGN uint8_t sDataInfo[CACHE_ALIGNED_SIZE_GET(PLC_STATUS_LENGTH)];
static CACHE_ALIGN uint8_t sDataTxPar[CACHE_ALIGNED_SIZE_GET(PLC_TX_PAR_SIZE)];
static CACHE_ALIGN uint8_t sDataRxPar[CACHE_ALIGNED_SIZE_GET(PLC_RX_PAR_SIZE)];
static CACHE_ALIGN uint8_t sDataRxDat[CACHE_ALIGNED_SIZE_GET(PLC_DATA_PKT_SIZE)];
static CACHE_ALIGN uint8_t sDataTxCfm[CACHE_ALIGNED_SIZE_GET(PLC_CMF_PKT_SIZE)];
static CACHE_ALIGN uint8_t sDataReg[CACHE_ALIGNED_SIZE_GET(PLC_REG_PKT_SIZE)];

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

static uint32_t DRV_PLC_PHY_COMM_GetPibBaseAddress(DRV_PLC_PHY_ID id)
{
    uint32_t addr;

    addr = 0;

    if (((uint16_t)id & DRV_PLC_PHY_REG_ADC_MASK) != 0U)
    {
        addr = (uint32_t)DRV_PLC_PHY_REG_ADC_BASE;
    }
    else if (((uint16_t)id & DRV_PLC_PHY_REG_DAC_MASK) != 0U)
    {
        addr = (uint32_t)DRV_PLC_PHY_REG_DAC_BASE;
    }
    else if (((uint16_t)id & DRV_PLC_PHY_FUSES_MASK) != 0U)
    {
        addr = (uint32_t)DRV_PLC_PHY_FUSES_BASE;
    }
    else if ((((uint16_t)id & DRV_PLC_PHY_REG_MASK) != 0U) && (id < PLC_ID_END_ID))
    {
        addr = (uint32_t)DRV_PLC_PHY_REG_BASE;
    }
    else
    {
        addr = 0;
    }

    return addr;
}

static uint16_t DRV_PLC_PHY_COMM_GetDelayUs(DRV_PLC_PHY_ID id)
{
    uint16_t delay = 50;

    if ((((uint16_t)id & DRV_PLC_PHY_REG_MASK) != 0U) && (id < PLC_ID_END_ID))
    {
        switch (id) 
        {
            case PLC_ID_TONE_MASK:
            delay = 600;
            break;

            case PLC_ID_PREDIST_COEF_TABLE_HI:
            case PLC_ID_PREDIST_COEF_TABLE_LO:
            delay = 250;
            break;

            case PLC_ID_PREDIST_COEF_TABLE_VLO:
            delay = 350;
            break;

            default:
            delay = 50;
            break;
        }
    }

    return delay;
}

static size_t DRV_PLC_PHY_COMM_TxStringify(DRV_PLC_PHY_TRANSMISSION_OBJ *pSrc)
{
    uint8_t *pDst;
    ptrdiff_t size;
    
    pDst = sDataTxPar;
    
    *pDst++ = (uint8_t)pSrc->timeIni;
    *pDst++ = (uint8_t)(pSrc->timeIni >> 8);
    *pDst++ = (uint8_t)(pSrc->timeIni >> 16);
    *pDst++ = (uint8_t)(pSrc->timeIni >> 24);
    *pDst++ = (uint8_t)pSrc->dataLength;
    *pDst++ = (uint8_t)(pSrc->dataLength >> 8);
    (void) memcpy(pDst, pSrc->preemphasis, sizeof(pSrc->preemphasis));
    pDst += sizeof(pSrc->preemphasis);
    (void) memcpy(pDst, pSrc->toneMap, sizeof(pSrc->toneMap));
    pDst += sizeof(pSrc->toneMap);
    *pDst++ = pSrc->mode;
    *pDst++ = pSrc->attenuation;
    *pDst++ = (uint8_t)pSrc->modType;
    *pDst++ = (uint8_t)pSrc->modScheme;
    *pDst++ = pSrc->pdc;
    *pDst++ = pSrc->rs2Blocks;
    *pDst++ = (uint8_t)pSrc->delimiterType;

    size = pDst - sDataTxPar;
    
    return (size_t)size;
    
}

static void DRV_PLC_PHY_COMM_TxCfmEvent(DRV_PLC_PHY_TRANSMISSION_CFM_OBJ *pCfmObj)
{
    uint8_t *pSrc;
    
    pSrc = sDataTxCfm;
    
    pCfmObj->rmsCalc = (uint32_t)*pSrc++;
    pCfmObj->rmsCalc += (uint32_t)*pSrc++ << 8;
    pCfmObj->rmsCalc += (uint32_t)*pSrc++ << 16;
    pCfmObj->rmsCalc += (uint32_t)*pSrc++ << 24;

    pCfmObj->timeEnd = (uint32_t)*pSrc++;
    pCfmObj->timeEnd += (uint32_t)*pSrc++ << 8;
    pCfmObj->timeEnd += (uint32_t)*pSrc++ << 16;
    pCfmObj->timeEnd += (uint32_t)*pSrc++ << 24;

    pCfmObj->result = (DRV_PLC_PHY_TX_RESULT)*pSrc;
}

static void DRV_PLC_PHY_COMM_RxEvent(DRV_PLC_PHY_RECEPTION_OBJ *pRxObj)
{
    uint8_t *pSrc;
    uint16_t u16Aux;
    
    pSrc = sDataRxPar;
    
    /* Parse parameters of reception event */
    pRxObj->timeEnd = (uint32_t)*pSrc++;
    pRxObj->timeEnd += (uint32_t)*pSrc++ << 8;
    pRxObj->timeEnd += (uint32_t)*pSrc++ << 16;
    pRxObj->timeEnd += (uint32_t)*pSrc++ << 24;

    pRxObj->frameDuration = (uint32_t)*pSrc++;
    pRxObj->frameDuration += (uint32_t)*pSrc++ << 8;
    pRxObj->frameDuration += (uint32_t)*pSrc++ << 16;
    pRxObj->frameDuration += (uint32_t)*pSrc++ << 24;

    pRxObj->rssi = (uint16_t)*pSrc++;
    pRxObj->rssi += (uint16_t)*pSrc++ << 8;
    
    pRxObj->dataLength = (uint16_t)*pSrc++;
    pRxObj->dataLength += (uint16_t)*pSrc++ << 8;
    if (pRxObj->dataLength > PLC_DATA_PKT_SIZE) {
        pRxObj->dataLength = PLC_DATA_PKT_SIZE;
    }

    pRxObj->zctDiff = *pSrc++;
    pRxObj->rsCorrectedErrors = *pSrc++;
    pRxObj->modType = (DRV_PLC_PHY_MOD_TYPE)*pSrc++;
    pRxObj->modScheme = (DRV_PLC_PHY_MOD_SCHEME)*pSrc++;

    pRxObj->agcFactor = (uint32_t)*pSrc++;
    pRxObj->agcFactor += (uint32_t)*pSrc++ << 8;
    pRxObj->agcFactor += (uint32_t)*pSrc++ << 16;
    pRxObj->agcFactor += (uint32_t)*pSrc++ << 24;
    pRxObj->agcFine = (uint16_t)*pSrc++;
    pRxObj->agcFine += (uint16_t)*pSrc++ << 8;
    u16Aux = (uint16_t)*pSrc++;
    u16Aux += (uint16_t)*pSrc++ << 8;
    pRxObj->agcOffsetMeas = (int16_t)u16Aux;
    pRxObj->agcActive = *pSrc++;
    pRxObj->agcPgaValue = *pSrc++;
    u16Aux = (uint16_t)*pSrc++;
    u16Aux += (uint16_t)*pSrc++ << 8;
    pRxObj->snrFch = (int16_t)u16Aux;
    u16Aux = (uint16_t)*pSrc++;
    u16Aux += (uint16_t)*pSrc++ << 8;
    pRxObj->snrPay = (int16_t)u16Aux;
    pRxObj->payloadCorruptedCarriers = (uint16_t)*pSrc++;
    pRxObj->payloadCorruptedCarriers += (uint16_t)*pSrc++ << 8;
    pRxObj->payloadNoisedSymbols = (uint16_t)*pSrc++;
    pRxObj->payloadNoisedSymbols += (uint16_t)*pSrc++ << 8;
    pRxObj->payloadSnrWorstCarrier = *pSrc++;
    pRxObj->payloadSnrWorstSymbol = *pSrc++;
    pRxObj->payloadSnrImpulsive = *pSrc++;
    pRxObj->payloadSnrBand = *pSrc++;
    pRxObj->payloadSnrBackground = *pSrc++;
    pRxObj->lqi = *pSrc++;

    pRxObj->delimiterType = (DRV_PLC_PHY_DEL_TYPE)*pSrc++;
    pRxObj->crcOk = *pSrc++;
    (void) memcpy(pRxObj->toneMap, pSrc, TONE_MAP_SIZE_MAX);
    pSrc += sizeof(pRxObj->toneMap);
    (void) memcpy(pRxObj->carrierSnr, pSrc, PROTOCOL_CARRIERS_MAX);

    /* Set data content pointer */
    pRxObj->pReceivedData = sDataRxDat;
}

static bool DRV_PLC_PHY_COMM_CheckComm(DRV_PLC_HAL_INFO *info)
{
    if (info->key == DRV_PLC_HAL_KEY_CORTEX)
    {
        /* Communication correct */
        return true;
    }
    else if (info->key == DRV_PLC_HAL_KEY_BOOT)
    {
        /* Communication Error : Check reset value */
        if ((info->flags & DRV_PLC_HAL_FLAG_RST_WDOG) != 0U)
        {
            /* Debugger is connected */
            DRV_PLC_BOOT_Restart(DRV_PLC_BOOT_RESTART_SOFT);
            if (gPlcPhyObj->exceptionCallback != NULL)
            {
                gPlcPhyObj->exceptionCallback(DRV_PLC_PHY_EXCEPTION_DEBUG, gPlcPhyObj->contextExc);
            }
        }
        else
        {
            /* PLC needs boot process to upload firmware */
            DRV_PLC_BOOT_Restart(DRV_PLC_BOOT_RESTART_HARD);
            if (gPlcPhyObj->exceptionCallback != NULL)
            {
                gPlcPhyObj->exceptionCallback(DRV_PLC_PHY_EXCEPTION_RESET, gPlcPhyObj->contextExc);
            }
            
            /* Update Driver Status */
            gPlcPhyObj->status = SYS_STATUS_BUSY;
        }

        /* Check if there is any tx_cfm pending to be reported */
        if (gPlcPhyObj->state[0] == DRV_PLC_PHY_STATE_WAITING_TX_CFM)
        {
            gPlcPhyObj->evResetTxCfm = true;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">

            /* Post semaphore to resume task */
            (void) OSAL_SEM_PostISR(&gPlcPhyObj->semaphoreID);
</#if>
        }

        return true;
    }
    else
    {
        /* PLC needs boot process to upload firmware */
        DRV_PLC_BOOT_Restart(DRV_PLC_BOOT_RESTART_HARD);
        if (gPlcPhyObj->exceptionCallback != NULL)
        {
            gPlcPhyObj->exceptionCallback(DRV_PLC_PHY_EXCEPTION_UNEXPECTED_KEY, gPlcPhyObj->contextExc);
        }
            
        /* Update Driver Status */
        gPlcPhyObj->status = SYS_STATUS_ERROR;

        return false;
    }
}

static void DRV_PLC_PHY_COMM_SpiWriteCmd(DRV_PLC_PHY_MEM_ID id, uint8_t *pData, uint16_t length)
{
    DRV_PLC_HAL_CMD halCmd;
    DRV_PLC_HAL_INFO halInfo;
    uint8_t failures = 0;
    
    /* Disable external interrupt from PLC */
    gPlcPhyObj->plcHal->enableExtInt(false);
    
    halCmd.cmd = DRV_PLC_HAL_CMD_WR;
    halCmd.memId = (uint16_t)id;
    halCmd.length = length;
    halCmd.pData = pData;
    
    gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    
    /* Check communication integrity */
    while(!DRV_PLC_PHY_COMM_CheckComm(&halInfo))
    {
        failures++;
        if (failures == 2U) {
            if (gPlcPhyObj->exceptionCallback != NULL)
            {
                gPlcPhyObj->exceptionCallback(DRV_PLC_PHY_EXCEPTION_CRITICAL_ERROR, gPlcPhyObj->contextExc);
            }
            break;
        }
        gPlcPhyObj->plcHal->reset();
        gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    }  
    
    /* Enable external interrupt from PLC */
    gPlcPhyObj->plcHal->enableExtInt(true); 
}

static void DRV_PLC_PHY_COMM_SpiReadCmd(DRV_PLC_PHY_MEM_ID id, uint8_t *pData, uint16_t length)
{
    DRV_PLC_HAL_CMD halCmd;
    DRV_PLC_HAL_INFO halInfo;
    uint8_t failures = 0;
    
    /* Disable external interrupt from PLC */
    gPlcPhyObj->plcHal->enableExtInt(false);
    
    halCmd.cmd = DRV_PLC_HAL_CMD_RD;
    halCmd.memId = (uint16_t)id;
    halCmd.length = length;
    halCmd.pData = pData;
    
    gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    
    /* Check communication integrity */
    while(!DRV_PLC_PHY_COMM_CheckComm(&halInfo))
    {
        failures++;
        if (failures == 2U) {
            if (gPlcPhyObj->exceptionCallback != NULL)
            {
                gPlcPhyObj->exceptionCallback(DRV_PLC_PHY_EXCEPTION_CRITICAL_ERROR, gPlcPhyObj->contextExc);
            }
            break;
        }
        gPlcPhyObj->plcHal->reset();
        gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    }    
    
    /* Enable external interrupt from PLC */
    gPlcPhyObj->plcHal->enableExtInt(true); 
}

static void DRV_PLC_PHY_COMM_GetEventsInfo(DRV_PLC_PHY_EVENTS_OBJ *eventsObj)
{
    uint8_t *pData;
    DRV_PLC_HAL_CMD halCmd;
    DRV_PLC_HAL_INFO halInfo;
    uint8_t failures = 0;
    
    pData = sDataInfo;
    
    halCmd.cmd = DRV_PLC_HAL_CMD_RD;
    halCmd.memId = (uint16_t)STATUS_ID;
    halCmd.length = PLC_STATUS_LENGTH;
    halCmd.pData = pData;
    
    gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    
    /* Check communication integrity */
    while(!DRV_PLC_PHY_COMM_CheckComm(&halInfo))
    {
        failures++;
        if (failures == 2U) {
            if (gPlcPhyObj->exceptionCallback != NULL)
            {
                gPlcPhyObj->exceptionCallback(DRV_PLC_PHY_EXCEPTION_CRITICAL_ERROR, gPlcPhyObj->contextExc);
            }
            break;
        }
        gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    }    
    
    /* Extract Events information */
    eventsObj->evCfm = ((halInfo.flags & DRV_PLC_PHY_EV_FLAG_TX_CFM_MASK) != 0U);
    eventsObj->evRxDat = ((halInfo.flags & DRV_PLC_PHY_EV_FLAG_RX_DAT_MASK) != 0U);
    eventsObj->evRxPar = ((halInfo.flags & DRV_PLC_PHY_EV_FLAG_RX_PAR_MASK) != 0U);
    eventsObj->evReg = ((halInfo.flags & DRV_PLC_PHY_EV_FLAG_REG_MASK) != 0U);
    
    /* Extract Timer info */
    eventsObj->timerRef = ((uint32_t)*pData++);
    eventsObj->timerRef += ((uint32_t)*pData++) << 8;
    eventsObj->timerRef += ((uint32_t)*pData++) << 16;
    eventsObj->timerRef += ((uint32_t)*pData++) << 24;

    /* Extract Lengths info */
    eventsObj->rcvDataLength = ((uint16_t)*pData++);
    eventsObj->rcvDataLength += ((uint16_t)*pData++) << 8;
    eventsObj->regRspLength = ((uint16_t)*pData++);
    eventsObj->regRspLength += ((uint16_t)*pData++) << 8;
}

// *****************************************************************************
// *****************************************************************************
// Section: DRV_PLC_PHY Common Interface Implementation
// *****************************************************************************
// *****************************************************************************
void DRV_PLC_PHY_Init(DRV_PLC_PHY_OBJ *plcPhyObj)
{
    gPlcPhyObj = plcPhyObj;
    
    /* Clear information about PLC events */
    gPlcPhyObj->evTxCfm[0] = false;
    gPlcPhyObj->evRxPar = false;
    gPlcPhyObj->evRxDat = false;
    gPlcPhyObj->evRegRspLength = 0;
    gPlcPhyObj->evResetTxCfm = false;
    
    /* Enable external interrupt from PLC */
    gPlcPhyObj->plcHal->enableExtInt(true);
}

void DRV_PLC_PHY_Task(void)
{
<#if DRV_PLC_SLEEP_MODE == true>
    if (gPlcPhyObj->sleep)
    {
        return;
    }

</#if>
    /* Check event flags */
    if ((gPlcPhyObj->evTxCfm[0]) || (gPlcPhyObj->evResetTxCfm))
    {
        DRV_PLC_PHY_TRANSMISSION_CFM_OBJ cfmObj;

        /* Reset event flag */
        gPlcPhyObj->evTxCfm[0] = false;

        if (gPlcPhyObj->evResetTxCfm)
        {
            gPlcPhyObj->evResetTxCfm = false;
            gPlcPhyObj->state[0] = DRV_PLC_PHY_STATE_IDLE;
            
            cfmObj.rmsCalc = 0;
            cfmObj.timeEnd = 0;
            cfmObj.result = DRV_PLC_PHY_TX_RESULT_NO_TX;
        } else {
            DRV_PLC_PHY_COMM_TxCfmEvent(&cfmObj);
        }
        
        if (gPlcPhyObj->txCfmCallback != NULL)
        {
            /* Report to upper layer */
            gPlcPhyObj->txCfmCallback(&cfmObj, gPlcPhyObj->contextCfm);
        }
    }
    
    if (gPlcPhyObj->evRxPar && gPlcPhyObj->evRxDat)
    {
        DRV_PLC_PHY_RECEPTION_OBJ rxObj;

        /* Reset event flags */
        gPlcPhyObj->evRxPar = false;
        gPlcPhyObj->evRxDat = false;

        DRV_PLC_PHY_COMM_RxEvent(&rxObj);
        if (gPlcPhyObj->dataIndCallback != NULL)
        {
            /* Report to upper layer */
            gPlcPhyObj->dataIndCallback(&rxObj, gPlcPhyObj->contextInd);
        }
    }
}

void DRV_PLC_PHY_TxRequest(const DRV_HANDLE handle, DRV_PLC_PHY_TRANSMISSION_OBJ *transmitObj)
{    
    DRV_PLC_PHY_TRANSMISSION_CFM_OBJ cfmObj;

<#if DRV_PLC_SLEEP_MODE == true>
    if (gPlcPhyObj->sleep)
    {
        /* Do not transmit in SLeep Mode. */
        if (gPlcPhyObj->txCfmCallback != NULL)
        {
            cfmObj.rmsCalc = 0;
            cfmObj.timeEnd = 0;
            cfmObj.result = DRV_PLC_PHY_TX_RESULT_NO_TX;
            /* Report to upper layer */
            gPlcPhyObj->txCfmCallback(&cfmObj, gPlcPhyObj->contextCfm);
        }
        
        return;
    }

</#if>
<#if DRV_PLC_MODE == "PL460" && DRV_PLC_THERMAL_MONITOR == true>
    if (gPlcPhyObj->plcHal->getThermalMonitor()) 
    {
        /* Check thermal warning (>110ºC). Do not transmit and report High Temperature warning. */
        if (gPlcPhyObj->txCfmCallback != NULL)
        {
            cfmObj.rmsCalc = 0;
            cfmObj.timeEnd = 0;
            cfmObj.result = DRV_PLC_PHY_TX_RESULT_HIGH_TEMP_110;
            /* Report to upper layer */
            gPlcPhyObj->txCfmCallback(&cfmObj, gPlcPhyObj->contextCfm);
        }
        
        return;
    }

</#if>
    if((handle != DRV_HANDLE_INVALID) && (handle == 0U) &&
            ((gPlcPhyObj->state[0] == DRV_PLC_PHY_STATE_IDLE) || ((transmitObj->mode & TX_MODE_CANCEL) != 0U)))
    {
        size_t size_params;
        
        size_params = DRV_PLC_PHY_COMM_TxStringify(transmitObj);
        
        if (size_params > 0U)
        {
            if ((transmitObj->mode & TX_MODE_CANCEL) == 0U)
            {
                /* Update PLC state: transmitting */
                gPlcPhyObj->state[0] = DRV_PLC_PHY_STATE_TX;
            }
            
            /* Send TX parameters */
            DRV_PLC_PHY_COMM_SpiWriteCmd(TX_PAR_ID, sDataTxPar, (uint16_t)size_params);
            
            /* Waiting CFM to avoid soon error responses */
            gPlcPhyObj->plcHal->delay(200);
            
            /* Send TX data */
            if (gPlcPhyObj->state[0] == DRV_PLC_PHY_STATE_TX)
            {
                /* Send TX data content */
                DRV_PLC_PHY_COMM_SpiWriteCmd(TX_DAT_ID, transmitObj->pTransmitData, transmitObj->dataLength);
            
                /* Update PLC state: waiting confirmation */
                gPlcPhyObj->state[0] = DRV_PLC_PHY_STATE_WAITING_TX_CFM;
            }
        }
        else
        {
            /* Notify DRV_PLC_PHY_TX_RESULT_INV_LENGTH */
            if (gPlcPhyObj->txCfmCallback != NULL)
            {
                cfmObj.rmsCalc = 0;
                cfmObj.timeEnd = 0;
                cfmObj.result = DRV_PLC_PHY_TX_RESULT_INV_LENGTH;
                /* Report to upper layer */
                gPlcPhyObj->txCfmCallback(&cfmObj, gPlcPhyObj->contextCfm);
            }
        }
    }
    else
    {
        if (gPlcPhyObj->txCfmCallback != NULL)
        {
            /* Notify DRV_PLC_PHY_TX_RESULT_NO_TX */
            cfmObj.rmsCalc = 0;
            cfmObj.timeEnd = 0;
            cfmObj.result = DRV_PLC_PHY_TX_RESULT_NO_TX;
            /* Report to upper layer */
            gPlcPhyObj->txCfmCallback(&cfmObj, gPlcPhyObj->contextCfm);
        }
    }
}

bool DRV_PLC_PHY_PIBGet(const DRV_HANDLE handle, DRV_PLC_PHY_PIB_OBJ *pibObj)
{    
    if((handle != DRV_HANDLE_INVALID) && (handle == 0U))
    {
<#if DRV_PLC_SLEEP_MODE == true>
        if (gPlcPhyObj->sleep)
        {
            return false;
        }

</#if>
        if (pibObj->id == PLC_ID_TIME_REF_ID)
        {
            /* Send PIB information request */
            DRV_PLC_PHY_COMM_SpiReadCmd(STATUS_ID, pibObj->pData, pibObj->length);
            return true;
        }
        else if (((uint16_t)pibObj->id & DRV_PLC_PHY_REG_ID_MASK) != 0U)
        {
            uint8_t *pDst;
            uint32_t address;
            uint16_t offset;
            uint16_t secureCnt;
            uint16_t cmdLength;

            offset = (uint16_t)pibObj->id & DRV_PLC_PHY_REG_OFFSET_MASK;

            /* Get address offset */
            address = DRV_PLC_PHY_COMM_GetPibBaseAddress(pibObj->id);
            if (address == 0U)
            {
                return false;
            }
            address += offset;
            
            /* Set CMD and length */
            cmdLength = (uint16_t)DRV_PLC_PHY_CMD_READ | (pibObj->length & DRV_PLC_PHY_REG_LEN_MASK);

            /* Build command */
            pDst = sDataReg;

            *pDst++ = (uint8_t)(address >> 24);
            *pDst++ = (uint8_t)(address >> 16);
            *pDst++ = (uint8_t)(address >> 8);
            *pDst++ = (uint8_t)(address);
            *pDst++ = (uint8_t)(cmdLength >> 8);
            *pDst++ = (uint8_t)(cmdLength);

            /* Send PIB information request */
            DRV_PLC_PHY_COMM_SpiWriteCmd(REG_INFO_ID, sDataReg, 8U);

            /* Wait to the response : Check length of the register response */
            secureCnt = 0xFFFF;
            while (gPlcPhyObj->evRegRspLength == 0U)
            {
                if ((secureCnt--) == 0U)
                {
                    /* Didn't came the expected response */
                    return false;
                }
            }

            /* copy Register info in data pointer */
            (void) memcpy(pibObj->pData, sDataReg, pibObj->length);
            /* Reset length of the register response */
            gPlcPhyObj->evRegRspLength = 0;

            return true;
        } 
        else
        {
            uint32_t value;
            bool result = true;
            
            /* Get HOST information */
            switch(pibObj->id)
            {
                case PLC_ID_HOST_DESCRIPTION_ID:
                {
                    const char *hostDesc = DRV_PLC_PHY_HOST_DESC;
                    (void) memcpy((void *)pibObj->pData, (const void *)hostDesc, strlen(DRV_PLC_PHY_HOST_DESC));
                    break;
                }
                    
                case PLC_ID_HOST_MODEL_ID:
                    value = DRV_PLC_PHY_HOST_MODEL;
                    pibObj->pData[0] = (uint8_t)value;
                    pibObj->pData[1] = (uint8_t)(value >> 8);
                    break;
                    
                case PLC_ID_HOST_PHY_ID:
                    value = DRV_PLC_PHY_HOST_PHY;
                    pibObj->pData[0] = (uint8_t)value;
                    pibObj->pData[1] = (uint8_t)(value >> 8);
                    pibObj->pData[2] = (uint8_t)(value >> 16);
                    pibObj->pData[3] = (uint8_t)(value >> 24);
                    break;
                    
                case PLC_ID_HOST_PRODUCT_ID:
                    value = DRV_PLC_PHY_HOST_PRODUCT;
                    pibObj->pData[0] = (uint8_t)value;
                    pibObj->pData[1] = (uint8_t)(value >> 8);
                    break;
                    
                case PLC_ID_HOST_VERSION_ID:
                    value = DRV_PLC_PHY_HOST_VERSION;
                    pibObj->pData[0] = (uint8_t)value;
                    pibObj->pData[1] = (uint8_t)(value >> 8);
                    pibObj->pData[2] = (uint8_t)(value >> 16);
                    pibObj->pData[3] = (uint8_t)(value >> 24);
                    break;
                    
                case PLC_ID_HOST_BAND_ID:
                    value = DRV_PLC_PHY_HOST_BAND;
                    pibObj->pData[0] = (uint8_t)value;
                    break;
                    
                default:
                    result = false;
                    break;
            }
            
            return result;
        }
    }
    else
    {
        return false;
    }
}

bool DRV_PLC_PHY_PIBSet(const DRV_HANDLE handle, DRV_PLC_PHY_PIB_OBJ *pibObj)
{    
    if((handle != DRV_HANDLE_INVALID) && (handle == 0U))
    {
<#if DRV_PLC_SLEEP_MODE == true>
        if (gPlcPhyObj->sleep)
        {
            return false;
        }

</#if>
        if (((uint16_t)pibObj->id & DRV_PLC_PHY_REG_ID_MASK) != 0U)
        {
            uint8_t *pDst;
            uint8_t *pSrc;
            uint32_t address;
            uint16_t offset;
            uint16_t delay;
            uint16_t cmdLength;

            offset = (uint16_t)pibObj->id & DRV_PLC_PHY_REG_OFFSET_MASK;

            /* Get base address */
            address = DRV_PLC_PHY_COMM_GetPibBaseAddress(pibObj->id);
            if (address == 0U)
            {
                return false;
            }
            address += offset;
            
            /* Set CMD and length */
            cmdLength = (uint16_t)DRV_PLC_PHY_CMD_WRITE | (pibObj->length & DRV_PLC_PHY_REG_LEN_MASK);

            /* Build command */
            pDst = sDataReg;

            *pDst++ = (uint8_t)(address >> 24);
            *pDst++ = (uint8_t)(address >> 16);
            *pDst++ = (uint8_t)(address >> 8);
            *pDst++ = (uint8_t)(address);
            *pDst++ = (uint8_t)(cmdLength >> 8);
            *pDst++ = (uint8_t)(cmdLength);
            
            pSrc = pibObj->pData;
            if (pibObj->length == 4U)
            {
                *pDst++ = *pSrc++;
                *pDst++ = *pSrc++;
                *pDst++ = *pSrc++;
                *pDst++ = *pSrc++;
            }
            else if (pibObj->length == 2U)
            {
                *pDst++ = *pSrc++;
                *pDst++ = *pSrc++;
            }
            else
            {
                (void) memcpy(pDst, pSrc, pibObj->length);
            }

            /* Send PIB information request */
            DRV_PLC_PHY_COMM_SpiWriteCmd(REG_INFO_ID, sDataReg, 6U + pibObj->length);

            /* Guard delay to ensure writing operation completion. */
            delay = DRV_PLC_PHY_COMM_GetDelayUs(pibObj->id);
            gPlcPhyObj->plcHal->delay(delay);

            return true;
        }
    }
    
    return false;
}

<#if DRV_PLC_PLIB == "SRV_SPISPLIT">
<#--  Connected to SPI PLIB through SPI Splitter  -->
    <#assign SPI_PLIB = DRV_PLC_PLIB_SPISPLIT>
<#else>
<#--  Connected directly to SPI PLIB  -->
    <#assign SPI_PLIB = DRV_PLC_PLIB>
</#if>
<#if SPI_PLIB?lower_case[0..*6] == "sercom">
void DRV_PLC_PHY_ExternalInterruptHandler(uintptr_t context)
<#else>
void DRV_PLC_PHY_ExternalInterruptHandler(PIO_PIN pin, uintptr_t context)
</#if>
{
    /* Avoid warning */
    (void)context;

<#if SPI_PLIB?lower_case[0..*6] == "sercom">
    if (gPlcPhyObj != NULL)
<#else>
    if ((gPlcPhyObj != NULL) && (pin == (PIO_PIN)gPlcPhyObj->plcHal->plcPlib->extIntPin))
</#if>
    {
        DRV_PLC_PHY_EVENTS_OBJ evObj;
        
        /* Time guard */
        gPlcPhyObj->plcHal->delay(20);
        
        /* Get PLC events information */
        DRV_PLC_PHY_COMM_GetEventsInfo(&evObj);
        
        /* Check confirmation of the transmission event */
        if (evObj.evCfm)
        {
            DRV_PLC_PHY_COMM_SpiReadCmd(TX_CFM_ID, sDataTxCfm, (uint16_t)PLC_CMF_PKT_SIZE);
            /* update event flag */
            gPlcPhyObj->evTxCfm[0] = true;
            /* Update PLC state: idle */
            gPlcPhyObj->state[0] = DRV_PLC_PHY_STATE_IDLE;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
            /* Post semaphore to resume task */
            (void) OSAL_SEM_PostISR(&gPlcPhyObj->semaphoreID);
</#if>
        }
        
        /* Check received new data event (First event in RX) */
        if (evObj.evRxDat)
        {        
            DRV_PLC_PHY_COMM_SpiReadCmd(RX_DAT_ID, sDataRxDat, evObj.rcvDataLength);
            /* update event flag */
            gPlcPhyObj->evRxDat = true;
        }
        
        /* Check received new parameters event (Second event in RX) */
        if (evObj.evRxPar)
        {
            DRV_PLC_PHY_COMM_SpiReadCmd(RX_PAR_ID, sDataRxPar, (uint16_t)PLC_RX_PAR_SIZE - 4U);
            /* update event flag */
            gPlcPhyObj->evRxPar = true;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
            /* Post semaphore to resume task */
            (void) OSAL_SEM_PostISR(&gPlcPhyObj->semaphoreID);
</#if>
        }
        
        /* Check register info event */
        if (evObj.evReg)
        {     
            DRV_PLC_PHY_COMM_SpiReadCmd(REG_INFO_ID, sDataReg, evObj.regRspLength);
            /* update event flag */
            gPlcPhyObj->evRegRspLength = evObj.regRspLength;
        }
        
        /* Time guard */
        gPlcPhyObj->plcHal->delay(50);
    }
<#if SPI_PLIB?lower_case[0..*6] != "sercom">
    
    /* PORT Interrupt Status Clear */
<#if (PLC_PIO_ID??) && (PLC_PIO_ID == 11264)>
    (&(PIO0_REGS->PIO_GROUP[DRV_PLC_EXT_INT_PIO_PORT]))->PIO_ISR;
<#else>
    ((pio_registers_t*)DRV_PLC_EXT_INT_PIO_PORT)->PIO_ISR;
</#if>
</#if>
}
