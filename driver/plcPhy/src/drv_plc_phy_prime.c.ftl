/******************************************************************************
  DRV_PLC_PHY PRIME Profile Layer

  Company:
    Microchip Technology Inc.

  File Name:
    drv_plc_phy_comm.c

  Summary:
    PLC Driver PRIME Profile Layer

  Description:
    This file contains the source code for the implementation of the PRIME
    Profile Layer.
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
#include <string.h>
#include "configuration.h"
#include "system/system.h"
#include "driver/plc/phy/drv_plc_phy.h"
#include "driver/plc/common/drv_plc_hal.h"
#include "driver/plc/common/drv_plc_boot.h"
#include "driver/plc/phy/drv_plc_phy_local_comm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global data
// *****************************************************************************
// *****************************************************************************

/* This is the driver instance object array. */
static DRV_PLC_PHY_OBJ *gPlcPhyObj;

/* Buffer definition to communicate with PLC */
static CACHE_ALIGN uint8_t sDataInfo[CACHE_ALIGNED_SIZE_GET(PLC_STATUS_LENGTH)];
static CACHE_ALIGN uint8_t sDataTx[CACHE_ALIGNED_SIZE_GET((PLC_TX_PAR_SIZE + PLC_DATA_PKT_SIZE))];
static CACHE_ALIGN uint8_t sDataRxPar[CACHE_ALIGNED_SIZE_GET(PLC_RX_PAR_SIZE)];
static CACHE_ALIGN uint8_t sDataRxDat[CACHE_ALIGNED_SIZE_GET(PLC_DATA_PKT_SIZE)];
static CACHE_ALIGN uint8_t sDataTxCfm[2][CACHE_ALIGNED_SIZE_GET(PLC_CMF_PKT_SIZE)];
static CACHE_ALIGN uint8_t sDataReg[CACHE_ALIGNED_SIZE_GET(PLC_REG_PKT_SIZE)];

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

static uint32_t _DRV_PLC_PHY_COMM_GetPibBaseAddress(DRV_PLC_PHY_ID id)
{
    uint32_t addr;

    addr = 0;

    if (id & DRV_PLC_PHY_REG_ADC_MASK) {
        addr = (uint32_t)DRV_PLC_PHY_REG_ADC_BASE;
    } else if (id & DRV_PLC_PHY_REG_DAC_MASK) {
        addr = (uint32_t)DRV_PLC_PHY_REG_DAC_BASE;
    } else if (id & DRV_PLC_PHY_FUSES_MASK) {
        addr = (uint32_t)DRV_PLC_PHY_FUSES_BASE;
    } else if ((id & DRV_PLC_PHY_REG_MASK) && (id < PLC_ID_END_ID)) {
        addr = (uint32_t)DRV_PLC_PHY_REG_BASE;
    }

    return addr;
}

uint16_t _DRV_PLC_PHY_COMM_GetDelayUs(DRV_PLC_PHY_ID id)
{
    uint16_t delay = 50;

    if ((id & DRV_PLC_PHY_REG_MASK) && (id < PLC_ID_END_ID)) 
    {
        switch (id) 
        {
            case PLC_ID_CHANNEL_CFG:
            delay = 5500;
            break;

            case PLC_ID_PREDIST_COEF_TABLE_HI:
            case PLC_ID_PREDIST_COEF_TABLE_LO:
            case PLC_ID_PREDIST_COEF_TABLE_HI_2:
            case PLC_ID_PREDIST_COEF_TABLE_LO_2:
            delay = 1000;
            break;

            case PLC_ID_PREDIST_COEF_TABLE_VLO:
            case PLC_ID_PREDIST_COEF_TABLE_VLO_2:
            delay = 2000;
            break;

            default:
            delay = 50;
            break;
        }
    }

    return delay;
}

static size_t _DRV_PLC_PHY_COMM_TxStringify(DRV_PLC_PHY_TRANSMISSION_OBJ *pSrc)
{
    uint8_t *pDst;
    size_t size;
    
    pDst = sDataTx;
    
    *pDst++ = (uint8_t)pSrc->time;
    *pDst++ = (uint8_t)(pSrc->time >> 8);
    *pDst++ = (uint8_t)(pSrc->time >> 16);
    *pDst++ = (uint8_t)(pSrc->time >> 24);
    *pDst++ = (uint8_t)pSrc->dataLength;
    *pDst++ = (uint8_t)(pSrc->dataLength >> 8);    
    *pDst++ = pSrc->attenuation;
    *pDst++ = pSrc->scheme;
    *pDst++ = pSrc->forced;
    *pDst++ = pSrc->frameType;
    *pDst++ = pSrc->mode;
    *pDst++ = pSrc->bufferId;

    if (pSrc->dataLength > PLC_DATA_PKT_SIZE)
    {
        pSrc->dataLength = PLC_DATA_PKT_SIZE;
    }

    memcpy(pDst, pSrc->pTransmitData, pSrc->dataLength);
    pDst += pSrc->dataLength;

    size = (uint16_t)(pDst - sDataTx);
    
    return size;
    
}

static void _DRV_PLC_PHY_COMM_TxCfmEvent(DRV_PLC_PHY_TRANSMISSION_CFM_OBJ *pCfmObj, uint8_t id)
{
    uint8_t *pSrc;
    
    pSrc = sDataTxCfm[id];
    
    pCfmObj->rmsCalc = (uint32_t)*pSrc++;
    pCfmObj->rmsCalc += (uint32_t)*pSrc++ << 8;
    pCfmObj->rmsCalc += (uint32_t)*pSrc++ << 16;
    pCfmObj->rmsCalc += (uint32_t)*pSrc++ << 24;

    pCfmObj->time = (uint32_t)*pSrc++;
    pCfmObj->time += (uint32_t)*pSrc++ << 8;
    pCfmObj->time += (uint32_t)*pSrc++ << 16;
    pCfmObj->time += (uint32_t)*pSrc++ << 24;

    pCfmObj->frameType = (DRV_PLC_PHY_FRAME_TYPE)*pSrc++;
    pCfmObj->result = (DRV_PLC_PHY_TX_RESULT)*pSrc++;
    pCfmObj->bufferId = (DRV_PLC_PHY_BUFFER_ID)*pSrc;
}

static void _DRV_PLC_PHY_COMM_RxEvent(DRV_PLC_PHY_RECEPTION_OBJ *pRxObj)
{
    uint8_t *pSrc;
    
    pSrc = sDataRxPar;
    
    /* Parse parameters of reception event */
    pRxObj->evmHeaderAcum = (uint32_t)*pSrc++;
    pRxObj->evmHeaderAcum += (uint32_t)*pSrc++ << 8;
    pRxObj->evmHeaderAcum += (uint32_t)*pSrc++ << 16;
    pRxObj->evmHeaderAcum += (uint32_t)*pSrc++ << 24;
    pRxObj->evmPayloadAcum = (uint32_t)*pSrc++;
    pRxObj->evmPayloadAcum += (uint32_t)*pSrc++ << 8;
    pRxObj->evmPayloadAcum += (uint32_t)*pSrc++ << 16;
    pRxObj->evmPayloadAcum += (uint32_t)*pSrc++ << 24;
    pRxObj->time = (uint32_t)*pSrc++;
    pRxObj->time += (uint32_t)*pSrc++ << 8;
    pRxObj->time += (uint32_t)*pSrc++ << 16;
    pRxObj->time += (uint32_t)*pSrc++ << 24;
    pRxObj->evmHeader = (uint16_t)*pSrc++;
    pRxObj->evmHeader += (uint16_t)*pSrc++ << 8;
    pRxObj->evmPayload = (uint16_t)*pSrc++;
    pRxObj->evmPayload += (uint16_t)*pSrc++ << 8;
    pRxObj->dataLength = (uint16_t)*pSrc++;
    pRxObj->dataLength += (uint16_t)*pSrc++ << 8;
    pRxObj->scheme = (DRV_PLC_PHY_SCH)*pSrc++;
    pRxObj->frameType = (DRV_PLC_PHY_FRAME_TYPE)*pSrc++;
    pRxObj->headerType = (DRV_PLC_PHY_HEADER)*pSrc++;
    pRxObj->rssiAvg = *pSrc++;
    pRxObj->cinrAvg = *pSrc++;
    pRxObj->cinrMin = *pSrc++;
    pRxObj->berSoftAvg = *pSrc++;
    pRxObj->berSoftMax = *pSrc++;
    pRxObj->narBandPercent = *pSrc++;
    pRxObj->impNoisePercent = *pSrc++;

    if (pRxObj->dataLength > PLC_DATA_PKT_SIZE)
    {
        pRxObj->dataLength = PLC_DATA_PKT_SIZE;
    }
    
    /* Set data content pointer */
    pRxObj->pReceivedData = sDataRxDat;
}

static bool _DRV_PLC_PHY_COMM_CheckComm(DRV_PLC_HAL_INFO *info)
{
    if (info->key == DRV_PLC_HAL_KEY_CORTEX)
    {
        /* Communication correct */
        return true;
    }
    else if (info->key == DRV_PLC_HAL_KEY_BOOT)
    {
        /* Communication Error : Check reset value */
        if (info->flags & DRV_PLC_HAL_FLAG_RST_WDOG)   
        {
            /* Debugger is connected */
            DRV_PLC_BOOT_Restart(DRV_PLC_BOOT_RESTART_SOFT);
            if (gPlcPhyObj->exceptionCallback)
            {
                gPlcPhyObj->exceptionCallback(DRV_PLC_PHY_EXCEPTION_DEBUG, gPlcPhyObj->contextExc);
            }
        }
        else
        {
            /* PLC needs boot process to upload firmware */
            DRV_PLC_BOOT_Restart(DRV_PLC_BOOT_RESTART_HARD);
            if (gPlcPhyObj->exceptionCallback)
            {
                gPlcPhyObj->exceptionCallback(DRV_PLC_PHY_EXCEPTION_RESET, gPlcPhyObj->contextExc);
            }
            
            /* Update Driver Status */
            gPlcPhyObj->status = SYS_STATUS_BUSY;
        }

        /* Check if there is any tx_cfm pending to be reported */
        if ((gPlcPhyObj->state[0] == DRV_PLC_PHY_STATE_WAITING_TX_CFM) ||
                (gPlcPhyObj->state[1] == DRV_PLC_PHY_STATE_WAITING_TX_CFM))
        {
            gPlcPhyObj->evResetTxCfm = true;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">

            /* Post semaphore to resume task */
            OSAL_SEM_PostISR(&gPlcPhyObj->semaphoreID);
</#if>
        }

        return true;
    }
    else
    {
        /* PLC needs boot process to upload firmware */
        DRV_PLC_BOOT_Restart(DRV_PLC_BOOT_RESTART_HARD);
        if (gPlcPhyObj->exceptionCallback)
        {
            gPlcPhyObj->exceptionCallback(DRV_PLC_PHY_EXCEPTION_UNEXPECTED_KEY, gPlcPhyObj->contextExc);
        }
            
        /* Update Driver Status */
        gPlcPhyObj->status = SYS_STATUS_ERROR;

        return false;
    }
}

static void _DRV_PLC_PHY_COMM_SpiWriteCmd(DRV_PLC_PHY_MEM_ID id, uint8_t *pData, uint16_t length)
{
    DRV_PLC_HAL_CMD halCmd;
    DRV_PLC_HAL_INFO halInfo;
    uint8_t failures = 0;
    
    /* Disable external interrupt from PLC */
    gPlcPhyObj->plcHal->enableExtInt(false);
    
    halCmd.cmd = DRV_PLC_HAL_CMD_WR;
    halCmd.memId = id;
    halCmd.length = length;
    halCmd.pData = pData;
    
    gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    
    /* Check communication integrity */
    while(!_DRV_PLC_PHY_COMM_CheckComm(&halInfo))
    {
        failures++;
        if (failures == 2) {
            if (gPlcPhyObj->exceptionCallback)
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

static void _DRV_PLC_PHY_COMM_SpiReadCmd(DRV_PLC_PHY_MEM_ID id, uint8_t *pData, uint16_t length)
{
    DRV_PLC_HAL_CMD halCmd;
    DRV_PLC_HAL_INFO halInfo;
    uint8_t failures = 0;
    
    /* Disable external interrupt from PLC */
    gPlcPhyObj->plcHal->enableExtInt(false);
    
    halCmd.cmd = DRV_PLC_HAL_CMD_RD;
    halCmd.memId = id;
    halCmd.length = length;
    halCmd.pData = pData;
    
    gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    
    /* Check communication integrity */
    while(!_DRV_PLC_PHY_COMM_CheckComm(&halInfo))
    {
        failures++;
        if (failures == 2) {
            if (gPlcPhyObj->exceptionCallback)
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

static void _DRV_PLC_PHY_COMM_GetEventsInfo(DRV_PLC_PHY_EVENTS_OBJ *eventsObj)
{
    uint8_t *pData;
    DRV_PLC_HAL_CMD halCmd;
    DRV_PLC_HAL_INFO halInfo;
    uint8_t failures = 0;
    
    pData = sDataInfo;    
    
    halCmd.cmd = DRV_PLC_HAL_CMD_RD;
    halCmd.memId = STATUS_ID;
    halCmd.length = PLC_STATUS_LENGTH;
    halCmd.pData = pData;
    
    gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    
    /* Check communication integrity */
    while(!_DRV_PLC_PHY_COMM_CheckComm(&halInfo))
    {
        failures++;
        if (failures == 2) {
            if (gPlcPhyObj->exceptionCallback)
            {
                gPlcPhyObj->exceptionCallback(DRV_PLC_PHY_EXCEPTION_CRITICAL_ERROR, gPlcPhyObj->contextExc);
            }
            break;
        }
        gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    }    
    
    /* Extract Events information */
    eventsObj->evCfm[0] = (halInfo.flags & DRV_PLC_PHY_EV_FLAG_TX0_CFM_MASK)? 1:0;
    eventsObj->evCfm[1] = (halInfo.flags & DRV_PLC_PHY_EV_FLAG_TX1_CFM_MASK)? 1:0;
    eventsObj->evRxDat = (halInfo.flags & DRV_PLC_PHY_EV_FLAG_RX_DAT_MASK)? 1:0;
    eventsObj->evRxPar = (halInfo.flags & DRV_PLC_PHY_EV_FLAG_RX_PAR_MASK)? 1:0;
    eventsObj->evReg = (halInfo.flags & DRV_PLC_PHY_EV_FLAG_REG_MASK)? 1:0;
    
    /* Extract Timer info */
    eventsObj->timerRef = ((uint32_t)*pData++);
    eventsObj->timerRef += ((uint32_t)*pData++) << 8;
    eventsObj->timerRef += ((uint32_t)*pData++) << 16;
    eventsObj->timerRef += ((uint32_t)*pData++) << 24;

    /* Extract Lengths info */
    eventsObj->rcvDataLength = ((uint32_t)*pData++);
    eventsObj->rcvDataLength += ((uint32_t)*pData++) << 8;
    eventsObj->regRspLength = ((uint32_t)*pData++);
    eventsObj->regRspLength += ((uint32_t)*pData++) << 8;
}

// *****************************************************************************
// *****************************************************************************
// Section: DRV_PLC_PHY Common Interface Implementation
// *****************************************************************************
// *****************************************************************************
void DRV_PLC_PHY_Init(DRV_PLC_PHY_OBJ *pl360)
{
    gPlcPhyObj = pl360;
    
    /* Clear information about PLC events */
    gPlcPhyObj->evTxCfm[0] = false;
    gPlcPhyObj->evTxCfm[1] = false;
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
    for (uint8_t idx = 0; idx < 2; idx++)
    {
        if ((gPlcPhyObj->evTxCfm[idx]) || (gPlcPhyObj->evResetTxCfm))
        {
            DRV_PLC_PHY_TRANSMISSION_CFM_OBJ cfmObj;

            /* Reset event flag */
            gPlcPhyObj->evTxCfm[idx] = false;

            if (gPlcPhyObj->evResetTxCfm)
            {
                gPlcPhyObj->evResetTxCfm = false;
                gPlcPhyObj->state[idx] = DRV_PLC_PHY_STATE_IDLE;

                cfmObj.bufferId = (DRV_PLC_PHY_BUFFER_ID)idx;
                cfmObj.rmsCalc = 0;
                cfmObj.time = 0;
                cfmObj.result = DRV_PLC_PHY_TX_RESULT_NO_TX;
            } else {
                _DRV_PLC_PHY_COMM_TxCfmEvent(&cfmObj, idx);
            }
            
            if (gPlcPhyObj->txCfmCallback)
            {
                /* Report to upper layer */
                gPlcPhyObj->txCfmCallback(&cfmObj, gPlcPhyObj->contextCfm);
            }
        }
    }
    
    if (gPlcPhyObj->evRxPar && gPlcPhyObj->evRxDat)
    {
        DRV_PLC_PHY_RECEPTION_OBJ rxObj;

        /* Reset event flags */
        gPlcPhyObj->evRxPar = false;
        gPlcPhyObj->evRxDat = false;

        _DRV_PLC_PHY_COMM_RxEvent(&rxObj);
        if (gPlcPhyObj->dataIndCallback)
        {
            /* Report to upper layer */
            gPlcPhyObj->dataIndCallback(&rxObj, gPlcPhyObj->contextInd);
        }
    }
}

void DRV_PLC_PHY_TxRequest(const DRV_HANDLE handle, DRV_PLC_PHY_TRANSMISSION_OBJ *transmitObj)
{    
    DRV_PLC_PHY_TRANSMISSION_CFM_OBJ cfmObj;
    uint8_t bufIdx = (uint8_t) transmitObj->bufferId;

    if (bufIdx > TX_BUFFER_1)
    {
        /* Invalid buffer. */
        if (gPlcPhyObj->txCfmCallback)
        {
            cfmObj.rmsCalc = 0;
            cfmObj.time = 0;
            cfmObj.result = DRV_PLC_PHY_TX_RESULT_INV_BUFFER;
            /* Report to upper layer */
            gPlcPhyObj->txCfmCallback(&cfmObj, gPlcPhyObj->contextCfm);
        }
        
        return;
    }

<#if DRV_PLC_SLEEP_MODE == true>             
    if (gPlcPhyObj->sleep)
    {
        /* Do not transmit in SLeep Mode. */
        if (gPlcPhyObj->txCfmCallback)
        {
            cfmObj.rmsCalc = 0;
            cfmObj.time = 0;
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
        if (gPlcPhyObj->txCfmCallback)
        {
            cfmObj.rmsCalc = 0;
            cfmObj.time = 0;
            cfmObj.result = DRV_PLC_PHY_TX_RESULT_HIGH_TEMP_110;
            /* Report to upper layer */
            gPlcPhyObj->txCfmCallback(&cfmObj, gPlcPhyObj->contextCfm);
        }
        
        return;
    }

</#if>  
    if((handle != DRV_HANDLE_INVALID) && (handle == 0) &&
            ((gPlcPhyObj->state[bufIdx] == DRV_PLC_PHY_STATE_IDLE) || ((transmitObj->mode & TX_MODE_CANCEL) != 0)))
    {
        size_t size;
        
        size = _DRV_PLC_PHY_COMM_TxStringify(transmitObj);
        
        if (size)
        {
            if ((transmitObj->mode & TX_MODE_CANCEL) == 0)
            {
                /* Update PLC state: transmitting */
                gPlcPhyObj->state[bufIdx] = DRV_PLC_PHY_STATE_TX;
            }
            
            /* Send TX message */
            if (bufIdx == TX_BUFFER_0)
            {
                _DRV_PLC_PHY_COMM_SpiWriteCmd(TX0_PAR_ID, sDataTx, size);
            }
            else
            {
                _DRV_PLC_PHY_COMM_SpiWriteCmd(TX1_PAR_ID, sDataTx, size);
            }
            
            /* Update PLC state: waiting confirmation */
            gPlcPhyObj->state[bufIdx] = DRV_PLC_PHY_STATE_WAITING_TX_CFM;
        }
        else
        {
            /* Notify DRV_PLC_PHY_TX_RESULT_INV_LENGTH */
            if (gPlcPhyObj->txCfmCallback)
            {
                cfmObj.rmsCalc = 0;
                cfmObj.time = 0;
                cfmObj.result = DRV_PLC_PHY_TX_RESULT_INV_LENGTH;
                /* Report to upper layer */
                gPlcPhyObj->txCfmCallback(&cfmObj, gPlcPhyObj->contextCfm);
            }
        }
    }
    else
    {
        if (gPlcPhyObj->txCfmCallback)
        {
            /* Notify DRV_PLC_PHY_TX_RESULT_NO_TX */
            cfmObj.rmsCalc = 0;
            cfmObj.time = 0;
            cfmObj.result = DRV_PLC_PHY_TX_RESULT_NO_TX;
            /* Report to upper layer */
            gPlcPhyObj->txCfmCallback(&cfmObj, gPlcPhyObj->contextCfm);
        }
    }
}

bool DRV_PLC_PHY_PIBGet(const DRV_HANDLE handle, DRV_PLC_PHY_PIB_OBJ *pibObj)
{    
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
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
            _DRV_PLC_PHY_COMM_SpiReadCmd(STATUS_ID, pibObj->pData, pibObj->length);
            return true;
        }
        else if (pibObj->id & DRV_PLC_PHY_REG_ID_MASK)
        {
            uint8_t *pDst;
            uint32_t address;
            uint32_t offset;
            uint16_t secureCnt;
            uint16_t cmdLength;

            offset = (uint32_t)(pibObj->id & DRV_PLC_PHY_REG_OFFSET_MASK);

            /* Get address offset */
            address = _DRV_PLC_PHY_COMM_GetPibBaseAddress(pibObj->id);
            if (address == 0)
            {
                return false;
            }
            address += offset;
            
            /* Set CMD and length */
            cmdLength = DRV_PLC_PHY_CMD_READ | (pibObj->length & DRV_PLC_PHY_REG_LEN_MASK);

            /* Build command */
            pDst = sDataReg;

            *pDst++ = (uint8_t)(address >> 24);
            *pDst++ = (uint8_t)(address >> 16);
            *pDst++ = (uint8_t)(address >> 8);
            *pDst++ = (uint8_t)(address);
            *pDst++ = (uint8_t)(cmdLength >> 8);
            *pDst++ = (uint8_t)(cmdLength);

            /* Send PIB information request */
            _DRV_PLC_PHY_COMM_SpiWriteCmd(REG_INFO_ID, sDataReg, 8);

            /* Wait to the response : Check length of the register response */
            secureCnt = 0xFFFF;
            while (!gPlcPhyObj->evRegRspLength) {
                if (!secureCnt--) {
                    /* Didn't came the expected response */
                    return false;
                }
            }

            /* copy Register info in data pointer */
            memcpy(pibObj->pData, sDataReg, pibObj->length);
            /* Reset length of the register response */
            gPlcPhyObj->evRegRspLength = 0;

            return true;
        } 
        else 
        {
            uint32_t value;
            
            /* Get HOST information */
            switch(pibObj->id)
            {
                case PLC_ID_HOST_DESCRIPTION_ID:
                    memcpy(pibObj->pData, DRV_PLC_PHY_HOST_DESC, 10);
                    break;
                    
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
                    return false;
            }
            
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool DRV_PLC_PHY_PIBSet(const DRV_HANDLE handle, DRV_PLC_PHY_PIB_OBJ *pibObj)
{    
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
<#if DRV_PLC_SLEEP_MODE == true>             
        if (gPlcPhyObj->sleep)
        {
            return false;
        }

</#if>  
        if (pibObj->id & DRV_PLC_PHY_REG_ID_MASK)
        {
            uint8_t *pDst;
            uint8_t *pSrc;
            uint32_t address;
            uint32_t offset;
            uint16_t delay;
            uint16_t cmdLength;

            offset = (uint32_t)(pibObj->id & DRV_PLC_PHY_REG_OFFSET_MASK);

            /* Get base address */
            address = _DRV_PLC_PHY_COMM_GetPibBaseAddress(pibObj->id);
            if (address == 0)
            {
                return false;
            }
            address += offset;
            
            /* Set CMD and length */
            cmdLength = DRV_PLC_PHY_CMD_WRITE | (pibObj->length & DRV_PLC_PHY_REG_LEN_MASK);

            /* Build command */
            pDst = sDataReg;

            *pDst++ = (uint8_t)(address >> 24);
            *pDst++ = (uint8_t)(address >> 16);
            *pDst++ = (uint8_t)(address >> 8);
            *pDst++ = (uint8_t)(address);
            *pDst++ = (uint8_t)(cmdLength >> 8);
            *pDst++ = (uint8_t)(cmdLength);
            
            pSrc = pibObj->pData;
            if (pibObj->length == 4) {
                *pDst++ = *pSrc++;
                *pDst++ = *pSrc++;
                *pDst++ = *pSrc++;
                *pDst++ = *pSrc++;
            } else if (pibObj->length == 2) {
                *pDst++ = *pSrc++;
                *pDst++ = *pSrc++;
            } else {
                memcpy(pDst, pSrc, pibObj->length);
                pDst += pibObj->length;
            }

            /* Send PIB information request */
            _DRV_PLC_PHY_COMM_SpiWriteCmd(REG_INFO_ID, sDataReg, pDst - sDataReg);

            /* Guard delay to ensure writing operation completion. */
            delay = _DRV_PLC_PHY_COMM_GetDelayUs(pibObj->id);
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
    if (gPlcPhyObj)
<#else>
    if ((gPlcPhyObj) && (pin == (PIO_PIN)gPlcPhyObj->plcHal->plcPlib->extIntPin))
</#if>
    {
        DRV_PLC_PHY_EVENTS_OBJ evObj;
        
        /* Time guard */
        gPlcPhyObj->plcHal->delay(20);
        
        /* Get PLC events information */
        _DRV_PLC_PHY_COMM_GetEventsInfo(&evObj);
        
        /* Check confirmation of the transmission event */
        if (evObj.evCfm[0])
        {
            _DRV_PLC_PHY_COMM_SpiReadCmd(TX0_CFM_ID, sDataTxCfm[0], PLC_CMF_PKT_SIZE);
            /* update event flag */
            gPlcPhyObj->evTxCfm[0] = true;
            /* Update PLC state: idle */
            gPlcPhyObj->state[0] = DRV_PLC_PHY_STATE_IDLE;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
            /* Post semaphore to resume task */
            OSAL_SEM_PostISR(&gPlcPhyObj->semaphoreID);
</#if>
        }

        if (evObj.evCfm[1])
        {
            _DRV_PLC_PHY_COMM_SpiReadCmd(TX1_CFM_ID, sDataTxCfm[1], PLC_CMF_PKT_SIZE);
            /* update event flag */
            gPlcPhyObj->evTxCfm[1] = true;
            /* Update PLC state: idle */
            gPlcPhyObj->state[1] = DRV_PLC_PHY_STATE_IDLE;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
            /* Post semaphore to resume task */
            OSAL_SEM_PostISR(&gPlcPhyObj->semaphoreID);
</#if>
        }
        
        /* Check received new data event (First event in RX) */
        if (evObj.evRxDat)
        {        
            _DRV_PLC_PHY_COMM_SpiReadCmd(RX_DAT_ID, sDataRxDat, evObj.rcvDataLength);
            /* update event flag */
            gPlcPhyObj->evRxDat = true;
        }
        
        /* Check received new parameters event (Second event in RX) */
        if (evObj.evRxPar)
        {
            _DRV_PLC_PHY_COMM_SpiReadCmd(RX_PAR_ID, sDataRxPar, PLC_RX_PAR_SIZE - 4);
            /* update event flag */
            gPlcPhyObj->evRxPar = true;
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
            /* Post semaphore to resume task */
            OSAL_SEM_PostISR(&gPlcPhyObj->semaphoreID);
</#if>
        }
        
        /* Check register info event */
        if (evObj.evReg)
        {     
            _DRV_PLC_PHY_COMM_SpiReadCmd(REG_INFO_ID, sDataReg, evObj.regRspLength);
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
