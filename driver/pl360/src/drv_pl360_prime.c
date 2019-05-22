/******************************************************************************
  DRV_PL360 PRIME Profile Layer

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360_comm.c

  Summary:
    PL360 Driver PRIME Profile Layer

  Description:
    This file contains the source code for the implementation of the PRIME
    Profile Layer.
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
#include "configuration.h"
#include "driver/pl360/drv_pl360.h"
#include "driver/pl360/drv_pl360_hal.h"
#include "driver/pl360/src/drv_pl360_local_comm.h"
#include "driver/pl360/src/drv_pl360_boot.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global data
// *****************************************************************************
// *****************************************************************************

/* This is the driver instance object array. */
static DRV_PL360_OBJ *gPl360Obj;

/* Buffer definition to communicate with PL360 */
static uint8_t sDataInfo[PL360_STATUS_LENGTH];
static uint8_t sDataTx[PL360_TX_PAR_SIZE + PL360_DATA_PKT_SIZE];
static uint8_t sDataRxPar[PL360_RX_PAR_SIZE];
static uint8_t sDataRxDat[PL360_DATA_PKT_SIZE];
static uint8_t sDataTxCfm[2][PL360_CMF_PKT_SIZE];
static uint8_t sDataReg[PL360_REG_PKT_SIZE];

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
static uint32_t _DRV_PL360_COMM_GetPibBaseAddress(DRV_PL360_ID id)
{
    uint32_t addr;

    addr = 0;

    if (id & DRV_PL360_REG_ADC_MASK) {
        addr = (uint32_t)DRV_PL360_REG_ADC_BASE;
    } else if (id & DRV_PL360_REG_DAC_MASK) {
        addr = (uint32_t)DRV_PL360_REG_DAC_BASE;
    } else if (id & DRV_PL360_FUSES_MASK) {
        addr = (uint32_t)DRV_PL360_FUSES_BASE;
    } else if ((id & DRV_PL360_REG_MASK) && (id < PL360_ID_END_ID)) {
        addr = (uint32_t)DRV_PL360_REG_BASE;
    }

    return addr;
}

uint16_t _DRV_PL360_COMM_GetDelayUs(DRV_PL360_ID id)
{
    uint16_t delay = 50;

    if ((id & DRV_PL360_REG_MASK) && (id < PL360_ID_END_ID)) 
    {
        switch (id) 
        {
            case PL360_ID_CHANNEL_CFG:
            delay = 100;
            break;

            case PL360_ID_PREDIST_COEF_TABLE_HI:
            case PL360_ID_PREDIST_COEF_TABLE_LO:
            delay = 1000;
            break;

            case PL360_ID_PREDIST_COEF_TABLE_VLO:
            delay = 2000;
            break;

            default:
            delay = 50;
            break;
        }
    }

    return delay;
}

static size_t _DRV_PL360_COMM_TxStringify(DRV_PL360_TRANSMISSION_OBJ *pSrc)
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

    if (pSrc->dataLength > PL360_DATA_PKT_SIZE)
    {
        pSrc->dataLength = PL360_DATA_PKT_SIZE;
    }

    memcpy(pDst, pSrc->pTransmitData, pSrc->dataLength);
    pDst += pSrc->dataLength;

    size = (uint16_t)(pDst - sDataTx);
    
    return size;
    
}

static void _DRV_PL360_COMM_TxCfmEvent(DRV_PL360_TRANSMISSION_CFM_OBJ *pCfmObj, uint8_t id)
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

    pCfmObj->frameType = (DRV_PL360_FRAME_TYPE)*pSrc++;
    pCfmObj->result = (DRV_PL360_TX_RESULT)*pSrc++;
    pCfmObj->bufferId = (DRV_PL360_BUFFER_ID)*pSrc;
}

static void _DRV_PL360_COMM_RxEvent(DRV_PL360_RECEPTION_OBJ *pRxObj)
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
    pRxObj->scheme = (DRV_PL360_SCH)*pSrc++;
    pRxObj->frameType = (DRV_PL360_FRAME_TYPE)*pSrc++;
    pRxObj->headerType = (DRV_PL360_HEADER)*pSrc++;
    pRxObj->rssiAvg = *pSrc++;
    pRxObj->cinrAvg = *pSrc++;
    pRxObj->cinrMin = *pSrc++;
    pRxObj->berSoftAvg = *pSrc++;
    pRxObj->berSoftMax = *pSrc++;
    pRxObj->narBandPercent = *pSrc++;
    pRxObj->impNoisePercent = *pSrc++;

    if (pRxObj->dataLength > PL360_DATA_PKT_SIZE)
    {
        pRxObj->dataLength = PL360_DATA_PKT_SIZE;
    }
    
    /* Set data content pointer */
    pRxObj->pReceivedData = sDataRxDat;
}

static bool _DRV_PL360_COMM_CheckComm(DRV_PL360_HAL_INFO *info)
{
    if (info->key == DRV_PL360_HAL_KEY_CORTEX)
    {
        /* Communication correct */
        return true;
    }
    else if (info->key == DRV_PL360_HAL_KEY_BOOT)
    {
        /* Communication Error : Check reset value */
        if (info->flags & DRV_PL360_HAL_FLAG_RST_WDOG)   
        {
            /* Debugger is connected */
            DRV_PL360_BOOT_Restart(false);
            if (gPl360Obj->exceptionCallback)
            {
                gPl360Obj->exceptionCallback(DRV_PL360_EXCEPTION_DEBUG, gPl360Obj->context);
            }
        }
        else
        {
            /* PL360 needs boot process to upload firmware */
            DRV_PL360_BOOT_Restart(true);
            if (gPl360Obj->exceptionCallback)
            {
                gPl360Obj->exceptionCallback(DRV_PL360_EXCEPTION_RESET, gPl360Obj->context);
            }
        }
    }
    else
    {
        /* PL360 needs boot process to upload firmware */
        DRV_PL360_BOOT_Restart(true);
        if (gPl360Obj->exceptionCallback)
        {
            gPl360Obj->exceptionCallback(DRV_PL360_EXCEPTION_UNEXPECTED_KEY, gPl360Obj->context);
        }
    }
    
    /* Check if there is any tx_cfm pending to be reported */
    if (gPl360Obj->state == DRV_PL360_STATE_WAITING_TX_CFM)
    {
        gPl360Obj->evResetTxCfm = true;
    }
    
    return false;
}

static void _DRV_PL360_COMM_SpiWriteCmd(DRV_PL360_MEM_ID id, uint8_t *pData, uint16_t length)
{
    DRV_PL360_HAL_CMD halCmd;
    DRV_PL360_HAL_INFO halInfo;
    uint8_t failures = 0;
    
    /* Disable external interrupt from PL360 */
    gPl360Obj->pl360Hal->enableExtInt(false);
    
    halCmd.cmd = DRV_PL360_HAL_CMD_WR;
    halCmd.memId = id;
    halCmd.length = length;
    halCmd.pData = pData;
    
    gPl360Obj->pl360Hal->sendWrRdCmd(&halCmd, &halInfo);
    
    /* Check communication integrity */
    while(!_DRV_PL360_COMM_CheckComm(&halInfo))
    {
        failures++;
        if (failures == 2) {
            if (gPl360Obj->exceptionCallback)
            {
                gPl360Obj->exceptionCallback(DRV_PL360_EXCEPTION_CRITICAL_ERROR, gPl360Obj->context);
            }
            break;
        }
        gPl360Obj->pl360Hal->sendWrRdCmd(&halCmd, &halInfo);
    }  
    
    /* Enable external interrupt from PL360 */
    gPl360Obj->pl360Hal->enableExtInt(true); 
}

static void _DRV_PL360_COMM_SpiReadCmd(DRV_PL360_MEM_ID id, uint8_t *pData, uint16_t length)
{
    DRV_PL360_HAL_CMD halCmd;
    DRV_PL360_HAL_INFO halInfo;
    uint8_t failures = 0;
    
    /* Disable external interrupt from PL360 */
    gPl360Obj->pl360Hal->enableExtInt(false);
    
    halCmd.cmd = DRV_PL360_HAL_CMD_RD;
    halCmd.memId = id;
    halCmd.length = length;
    halCmd.pData = pData;
    
    gPl360Obj->pl360Hal->sendWrRdCmd(&halCmd, &halInfo);
    
    /* Check communication integrity */
    while(!_DRV_PL360_COMM_CheckComm(&halInfo))
    {
        failures++;
        if (failures == 2) {
            if (gPl360Obj->exceptionCallback)
            {
                gPl360Obj->exceptionCallback(DRV_PL360_EXCEPTION_CRITICAL_ERROR, gPl360Obj->context);
            }
            break;
        }
        gPl360Obj->pl360Hal->sendWrRdCmd(&halCmd, &halInfo);
    }    
    
    /* Enable external interrupt from PL360 */
    gPl360Obj->pl360Hal->enableExtInt(true); 
}

static void _DRV_PL360_COMM_GetEventsInfo(DRV_PL360_EVENTS_OBJ *eventsObj)
{
    uint8_t *pData;
    DRV_PL360_HAL_CMD halCmd;
    DRV_PL360_HAL_INFO halInfo;
    uint8_t failures = 0;
    
    pData = sDataInfo;    
    
    halCmd.cmd = DRV_PL360_HAL_CMD_RD;
    halCmd.memId = STATUS_ID;
    halCmd.length = PL360_STATUS_LENGTH;
    halCmd.pData = pData;
    
    gPl360Obj->pl360Hal->sendWrRdCmd(&halCmd, &halInfo);
    
    /* Check communication integrity */
    while(!_DRV_PL360_COMM_CheckComm(&halInfo))
    {
        failures++;
        if (failures == 2) {
            if (gPl360Obj->exceptionCallback)
            {
                gPl360Obj->exceptionCallback(DRV_PL360_EXCEPTION_CRITICAL_ERROR, gPl360Obj->context);
            }
            break;
        }
        gPl360Obj->pl360Hal->sendWrRdCmd(&halCmd, &halInfo);
    }    
    
    /* Extract Events information */
    eventsObj->evCfm[0] = (halInfo.flags & DRV_PL360_EV_FLAG_TX0_CFM_MASK)? 1:0;
    eventsObj->evCfm[1] = (halInfo.flags & DRV_PL360_EV_FLAG_TX1_CFM_MASK)? 1:0;
    eventsObj->evRxDat = (halInfo.flags & DRV_PL360_EV_FLAG_RX_DAT_MASK)? 1:0;
    eventsObj->evRxPar = (halInfo.flags & DRV_PL360_EV_FLAG_RX_PAR_MASK)? 1:0;
    eventsObj->evReg = (halInfo.flags & DRV_PL360_EV_FLAG_REG_MASK)? 1:0;
    
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
// Section: DRV_PL360 Common Interface Implementation
// *****************************************************************************
// *****************************************************************************
void DRV_PL360_Init(DRV_PL360_OBJ *pl360)
{
    gPl360Obj = pl360;
    
    /* Clear information about PL360 events */
    gPl360Obj->evTxCfm[0] = false;
    gPl360Obj->evTxCfm[1] = false;
    gPl360Obj->evRxPar = false;
    gPl360Obj->evRxDat = false;
    gPl360Obj->evRegRspLength = 0;
    gPl360Obj->evResetTxCfm = false;
    
    /* Enable external interrupt from PL360 */
    gPl360Obj->pl360Hal->enableExtInt(true);
}

void DRV_PL360_Task(void)
{
    /* Check event flags */
    for (uint8_t idx = 0; idx < 2; idx++)
    {
        if (gPl360Obj->evTxCfm[idx])
        {
            DRV_PL360_TRANSMISSION_CFM_OBJ cfmObj;
            
            if (gPl360Obj->evResetTxCfm)
            {
				gPl360Obj->evResetTxCfm = false;
            	gPl360Obj->state = DRV_PL360_STATE_IDLE;
				
                cfmObj.bufferId = (DRV_PL360_BUFFER_ID)idx;
                cfmObj.rmsCalc = 0;
                cfmObj.time = 0;
                cfmObj.result = DRV_PL360_TX_RESULT_NO_TX;
            } else {
                _DRV_PL360_COMM_TxCfmEvent(&cfmObj, idx);            
            }
            
            if (gPl360Obj->dataCfmCallback)
            {
                /* Report to upper layer */
                gPl360Obj->dataCfmCallback(&cfmObj, gPl360Obj->context);
            }
            
            /* Reset event flag */
            gPl360Obj->evTxCfm[idx] = false;
        }
    }
    
    if (gPl360Obj->evRxPar && gPl360Obj->evRxDat)
    {
        DRV_PL360_RECEPTION_OBJ rxObj;
        
        _DRV_PL360_COMM_RxEvent(&rxObj);
        if (gPl360Obj->dataIndCallback)
        {
            /* Report to upper layer */
            gPl360Obj->dataIndCallback(&rxObj, gPl360Obj->context);
        }
        
        /* Reset event flags */
        gPl360Obj->evRxPar = false;
        gPl360Obj->evRxDat = false;
    }
}

void DRV_PL360_Send(const DRV_HANDLE handle, DRV_PL360_TRANSMISSION_OBJ *transmitObj)
{    
    if((handle != DRV_HANDLE_INVALID) && (handle == 0) && (gPl360Obj->state == DRV_PL360_STATE_IDLE))
    {
        size_t size;
        
        size = _DRV_PL360_COMM_TxStringify(transmitObj);
        
        if (size)
        {
            /* Update PL360 state: transmitting */
            gPl360Obj->state = DRV_PL360_STATE_TX;
            
            /* Send TX message */
            if (transmitObj->bufferId == TX_BUFFER_0)
            {
                _DRV_PL360_COMM_SpiWriteCmd(TX0_PAR_ID, sDataTx, size);
            }
            else
            {
                _DRV_PL360_COMM_SpiWriteCmd(TX1_PAR_ID, sDataTx, size);
            }
            
                           
            /* Update PL360 state: waiting confirmation */
            gPl360Obj->state = DRV_PL360_STATE_WAITING_TX_CFM;
        }
    }
}

bool DRV_PL360_PIBGet(const DRV_HANDLE handle, DRV_PL360_PIB_OBJ *pibObj)
{    
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        if (pibObj->id == PL360_ID_TIME_REF_ID)
        {
            /* Send PIB information request */
            _DRV_PL360_COMM_SpiReadCmd(STATUS_ID, pibObj->pData, pibObj->length);
            return true;
        }
        else if (pibObj->id & DRV_PL360_REG_ID_MASK)
        {
            uint8_t *pDst;
            uint32_t address;
            uint32_t offset;
            uint16_t secureCnt;
            uint16_t cmdLength;

            offset = (uint32_t)(pibObj->id & DRV_PL360_REG_OFFSET_MASK);

            /* Get address offset */
            address = _DRV_PL360_COMM_GetPibBaseAddress(pibObj->id);
            if (address == 0)
            {
                return false;
            }
            address += offset;
            
            /* Set CMD and length */
            cmdLength = DRV_PL360_CMD_READ | (pibObj->length & DRV_PL360_REG_LEN_MASK);

            /* Build command */
            pDst = sDataReg;

            *pDst++ = (uint8_t)(address >> 24);
            *pDst++ = (uint8_t)(address >> 16);
            *pDst++ = (uint8_t)(address >> 8);
            *pDst++ = (uint8_t)(address);
            *pDst++ = (uint8_t)(cmdLength >> 8);
            *pDst++ = (uint8_t)(cmdLength);

            /* Send PIB information request */
            _DRV_PL360_COMM_SpiWriteCmd(REG_INFO_ID, sDataReg, 8);

            /* Wait to the response : Check length of the register response */
            secureCnt = 0xFFFF;
            while (!gPl360Obj->evRegRspLength) {
                if (!secureCnt--) {
                    /* Didn't came the expected response */
                    return false;
                }
            }

            /* copy Register info in data pointer */
            memcpy(pibObj->pData, sDataReg, pibObj->length);
            /* Reset length of the register response */
            gPl360Obj->evRegRspLength = 0;

            return true;
        } 
        else 
        {
            uint32_t value;
            
            /* Get HOST information */
            switch(pibObj->id)
            {
                case PL360_ID_HOST_DESCRIPTION_ID:
                    memcpy(pibObj->pData, DRV_PL360_HOST_DESC, 10);
                    break;
                    
                case PL360_ID_HOST_MODEL_ID:
                    value = DRV_PL360_HOST_MODEL;
                    memcpy(pibObj->pData, (uint8_t*)&value, 2);
                    break;
                    
                case PL360_ID_HOST_PHY_ID:
                    value = DRV_PL360_HOST_PHY;
                    memcpy(pibObj->pData, (uint8_t*)&value, 4);
                    break;
                    
                case PL360_ID_HOST_PRODUCT_ID:
                    value = DRV_PL360_HOST_PRODUCT;
                    memcpy(pibObj->pData, (uint8_t*)&value, 2);
                    break;
                    
                case PL360_ID_HOST_VERSION_ID:
                    value = DRV_PL360_HOST_VERSION;
                    memcpy(pibObj->pData, (uint8_t*)&value, 4);
                    break;
                    
                case PL360_ID_HOST_BAND_ID:
                    value = DRV_PL360_HOST_BAND;
                    memcpy(pibObj->pData, (uint8_t*)&value, 1);
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

bool DRV_PL360_PIBSet(const DRV_HANDLE handle, DRV_PL360_PIB_OBJ *pibObj)
{    
    if((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {
        if (pibObj->id & DRV_PL360_REG_ID_MASK)
        {
            uint8_t *pDst;
            uint8_t *pSrc;
            uint32_t address;
            uint32_t offset;
            uint16_t delay;
            uint16_t cmdLength;

            offset = (uint32_t)(pibObj->id & DRV_PL360_REG_OFFSET_MASK);

            /* Get base address */
            address = _DRV_PL360_COMM_GetPibBaseAddress(pibObj->id);
            if (address == 0)
            {
                return false;
            }
            address += offset;
            
            /* Set CMD and length */
            cmdLength = DRV_PL360_CMD_WRITE | (pibObj->length & DRV_PL360_REG_LEN_MASK);

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
            _DRV_PL360_COMM_SpiWriteCmd(REG_INFO_ID, sDataReg, pDst - sDataReg);

            /* Guard delay to ensure writing operation completion. */
            delay = _DRV_PL360_COMM_GetDelayUs(pibObj->id);
            gPl360Obj->pl360Hal->delay(delay);

            return true;
        }
    }
    
    return false;
}

void DRV_PL360_ExternalInterruptHandler( PIO_PIN pin, uintptr_t context )
{    
    /* Avoid warning */
    (void)context;

    if ((gPl360Obj) && (pin == (PIO_PIN)gPl360Obj->pl360Hal->pl360Plib->extIntPin))
    {
        DRV_PL360_EVENTS_OBJ evObj;
        
        /* Time guard */
        gPl360Obj->pl360Hal->delay(20);
        
        /* Get PL360 events information */
        _DRV_PL360_COMM_GetEventsInfo(&evObj);
        
        /* Check confirmation of the transmission event */
        if (evObj.evCfm[0])
        {
            _DRV_PL360_COMM_SpiReadCmd(TX0_CFM_ID, sDataTxCfm[0], PL360_CMF_PKT_SIZE);
            /* update event flag */
            gPl360Obj->evTxCfm[0] = true;
            /* Update PL360 state: idle */
            gPl360Obj->state = DRV_PL360_STATE_IDLE;
        }

        if (evObj.evCfm[1])
        {
            _DRV_PL360_COMM_SpiReadCmd(TX1_CFM_ID, sDataTxCfm[1], PL360_CMF_PKT_SIZE);
            /* update event flag */
            gPl360Obj->evTxCfm[1] = true;
            /* Update PL360 state: idle */
            gPl360Obj->state = DRV_PL360_STATE_IDLE;
        }
        
        /* Check received new parameters event (First event in RX) */
        if (evObj.evRxDat)
        {        
            _DRV_PL360_COMM_SpiReadCmd(RX_DAT_ID, sDataRxDat, evObj.rcvDataLength);
            /* update event flag */
            gPl360Obj->evRxDat = true;
        }
        
        /* Check received new data event (Second event in RX) */
        if (evObj.evRxPar)
        {
            _DRV_PL360_COMM_SpiReadCmd(RX_PAR_ID, sDataRxPar, PL360_RX_PAR_SIZE - 4);
            /* update event flag */
            gPl360Obj->evRxPar = true;
        }
        
        /* Check register info event */
        if (evObj.evReg)
        {     
            _DRV_PL360_COMM_SpiReadCmd(REG_INFO_ID, sDataReg, evObj.regRspLength);
            /* update event flag */
            gPl360Obj->evRegRspLength = evObj.regRspLength;
        }
        
        /* Time guard */
        gPl360Obj->pl360Hal->delay(50);
    }
    
    /* PORTD Interrupt Status Clear */
    ((pio_registers_t*)PIO_PORT_D)->PIO_ISR;
}
