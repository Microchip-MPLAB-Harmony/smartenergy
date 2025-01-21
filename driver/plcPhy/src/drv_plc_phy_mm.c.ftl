/******************************************************************************
  DRV_PLC_PHY Meters&More Profile Layer

  Company:
    Microchip Technology Inc.

  File Name:
    drv_plc_phy_comm.c

  Summary:
    PLC Driver Meters&More Profile Layer

  Description:
    This file contains the source code for the implementation of the Meters&More
    Profile Layer.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2024, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
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
static CACHE_ALIGN uint8_t sDataTx[CACHE_ALIGNED_SIZE_GET(PLC_TX_PAR_SIZE + PLC_DATA_PKT_SIZE)];
static CACHE_ALIGN uint8_t sDataRxPar[CACHE_ALIGNED_SIZE_GET(PLC_RX_PAR_SIZE)];
static CACHE_ALIGN uint8_t sDataRxDat[CACHE_ALIGNED_SIZE_GET(PLC_DATA_PKT_SIZE)];
static CACHE_ALIGN uint8_t sDataTxCfm[CACHE_ALIGNED_SIZE_GET(PLC_CMF_PKT_SIZE)];
static CACHE_ALIGN uint8_t sDataReg[CACHE_ALIGNED_SIZE_GET(PLC_REG_PKT_SIZE)];

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

static uint32_t lDRV_PLC_PHY_COMM_GetPibBaseAddress(DRV_PLC_PHY_ID id)
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

static uint16_t lDRV_PLC_PHY_COMM_GetDelayUs(DRV_PLC_PHY_ID id)
{
    return 50;
}

static size_t lDRV_PLC_PHY_COMM_TxStringify(DRV_PLC_PHY_TRANSMISSION_OBJ *pSrc)
{
    uint8_t *pDst;
    ptrdiff_t size;

    pDst = sDataTx;

    *pDst++ = (uint8_t)pSrc->timeIni;
    *pDst++ = (uint8_t)(pSrc->timeIni >> 8);
    *pDst++ = (uint8_t)(pSrc->timeIni >> 16);
    *pDst++ = (uint8_t)(pSrc->timeIni >> 24);
    *pDst++ = (uint8_t)pSrc->dataLength;
    *pDst++ = (uint8_t)(pSrc->dataLength >> 8);
    *pDst++ = pSrc->mode;
    *pDst++ = pSrc->attenuation;
    *pDst++ = pSrc->nbFrame;

    if (pSrc->dataLength > PLC_DATA_PKT_SIZE)
    {
        pSrc->dataLength = PLC_DATA_PKT_SIZE;
    }
    
    (void) memcpy(pDst, pSrc->pTransmitData, pSrc->dataLength);
    pDst += pSrc->dataLength;

    size = pDst - sDataTx;

    return (size_t)size;

}

static void lDRV_PLC_PHY_COMM_TxCfmEvent(DRV_PLC_PHY_TRANSMISSION_CFM_OBJ *pCfmObj)
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

static void lDRV_PLC_PHY_COMM_RxEvent(DRV_PLC_PHY_RECEPTION_OBJ *pRxObj)
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

    pRxObj->dataLength = (uint16_t)*pSrc++;
    pRxObj->dataLength += (uint16_t)*pSrc++ << 8;
    if (pRxObj->dataLength > PLC_DATA_PKT_SIZE) {
        pRxObj->dataLength = PLC_DATA_PKT_SIZE;
    }

    u16Aux = (uint16_t)*pSrc++;
    u16Aux += (uint16_t)*pSrc++ << 8;
    pRxObj->snrHeader = (int16_t)u16Aux;
    u16Aux = (uint16_t)*pSrc++;
    u16Aux += (uint16_t)*pSrc++ << 8;
    pRxObj->snrPayload = (int16_t)u16Aux;

    pRxObj->nbRx = *pSrc++;
    pRxObj->lqi = *pSrc++;
    pRxObj->rssi = *pSrc++;
    pRxObj->crcOk = *pSrc++;

    /* Set data content pointer */
    pRxObj->pReceivedData = sDataRxDat;
}

static bool lDRV_PLC_PHY_COMM_CheckComm(DRV_PLC_HAL_INFO *info)
{
    bool result = false;

    if (info->key == DRV_PLC_HAL_KEY_CORTEX)
    {
        /* Communication correct */
        result = true;
        gPlcPhyObj->consecutiveSpiErrors = 0;
    }
    else if (info->key == DRV_PLC_HAL_KEY_BOOT)
    {
        /* Communication Error : Check reset value */
        if ((info->flags & DRV_PLC_HAL_FLAG_RST_WDOG) != 0U)
        {
            /* Debugger is connected */
            result = true;
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
        }
    }
    else
    {
        /* PLC needs boot process to upload firmware */
        DRV_PLC_BOOT_Restart(DRV_PLC_BOOT_RESTART_HARD);
        if (gPlcPhyObj->exceptionCallback != NULL)
        {
            gPlcPhyObj->exceptionCallback(DRV_PLC_PHY_EXCEPTION_UNEXPECTED_KEY, gPlcPhyObj->contextExc);
        }
    }

    if (false == result)
    {
        /* Firmware is uploaded 2 times as maximum */
        gPlcPhyObj->consecutiveSpiErrors++;
        if (gPlcPhyObj->consecutiveSpiErrors <= 2)
        {
            /* Update Driver Status */
            gPlcPhyObj->status = SYS_STATUS_BUSY;
        }
        else
        {
            /* Update Driver Status */
            gPlcPhyObj->status = SYS_STATUS_ERROR;
            if (gPlcPhyObj->exceptionCallback != NULL)
            {
                gPlcPhyObj->exceptionCallback(DRV_PLC_PHY_EXCEPTION_CRITICAL_ERROR, gPlcPhyObj->contextExc);
            }
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
    }

    return result;
}

static void lDRV_PLC_PHY_COMM_SpiWriteCmd(DRV_PLC_PHY_MEM_ID id, uint8_t *pData, uint16_t length)
{
    DRV_PLC_HAL_CMD halCmd;
    DRV_PLC_HAL_INFO halInfo;

    /* Disable external interrupt from PLC */
    gPlcPhyObj->plcHal->enableExtInt(false);

    halCmd.cmd = DRV_PLC_HAL_CMD_WR;
    halCmd.memId = (uint16_t)id;
    halCmd.length = length;
    halCmd.pData = pData;

    gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);

    /* Check communication integrity */
    if (lDRV_PLC_PHY_COMM_CheckComm(&halInfo))
    {
        /* Enable external interrupt from PLC */
        gPlcPhyObj->plcHal->enableExtInt(true);
    }
}

static void lDRV_PLC_PHY_COMM_SpiReadCmd(DRV_PLC_PHY_MEM_ID id, uint8_t *pData, uint16_t length)
{
    DRV_PLC_HAL_CMD halCmd;
    DRV_PLC_HAL_INFO halInfo;

    /* Disable external interrupt from PLC */
    gPlcPhyObj->plcHal->enableExtInt(false);

    halCmd.cmd = DRV_PLC_HAL_CMD_RD;
    halCmd.memId = (uint16_t)id;
    halCmd.length = length;
    halCmd.pData = pData;

    gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);

    /* Check communication integrity */
    if (lDRV_PLC_PHY_COMM_CheckComm(&halInfo))
    {
        /* Enable external interrupt from PLC */
        gPlcPhyObj->plcHal->enableExtInt(true);
    }
}

static void lDRV_PLC_PHY_COMM_GetEventsInfo(DRV_PLC_PHY_EVENTS_OBJ *eventsObj)
{
    uint8_t *pData;
    DRV_PLC_HAL_CMD halCmd;
    DRV_PLC_HAL_INFO halInfo;

    pData = sDataInfo;

    halCmd.cmd = DRV_PLC_HAL_CMD_RD;
    halCmd.memId = (uint16_t)STATUS_ID;
    halCmd.length = PLC_STATUS_LENGTH;
    halCmd.pData = pData;

    gPlcPhyObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);

    /* Check communication integrity */
    if (!lDRV_PLC_PHY_COMM_CheckComm(&halInfo))
    {
        /* Disable external interrupt from PLC */
        gPlcPhyObj->plcHal->enableExtInt(false);
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
            lDRV_PLC_PHY_COMM_TxCfmEvent(&cfmObj);
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

        lDRV_PLC_PHY_COMM_RxEvent(&rxObj);
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
        size_t size;

        size = lDRV_PLC_PHY_COMM_TxStringify(transmitObj);

        if ((transmitObj->mode & TX_MODE_CANCEL) == 0U)
        {
            /* Update PLC state: waiting confirmation */
            gPlcPhyObj->state[0] = DRV_PLC_PHY_STATE_WAITING_TX_CFM;
        }

        /* Send TX message */
        lDRV_PLC_PHY_COMM_SpiWriteCmd(TX_PAR_ID, sDataTx, (uint16_t)size);

        /* Time guard */
        gPlcPhyObj->plcHal->delay(20);
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
            lDRV_PLC_PHY_COMM_SpiReadCmd(STATUS_ID, pibObj->pData, pibObj->length);
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
            address = lDRV_PLC_PHY_COMM_GetPibBaseAddress(pibObj->id);
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
            lDRV_PLC_PHY_COMM_SpiWriteCmd(REG_INFO_ID, sDataReg, 8U);

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
            address = lDRV_PLC_PHY_COMM_GetPibBaseAddress(pibObj->id);
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
            lDRV_PLC_PHY_COMM_SpiWriteCmd(REG_INFO_ID, sDataReg, 6U + pibObj->length);

            /* Guard delay to ensure writing operation completion. */
            delay = lDRV_PLC_PHY_COMM_GetDelayUs(pibObj->id);
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
        lDRV_PLC_PHY_COMM_GetEventsInfo(&evObj);

        /* Check confirmation of the transmission event */
        if (evObj.evCfm)
        {
            lDRV_PLC_PHY_COMM_SpiReadCmd(TX_CFM_ID, sDataTxCfm, (uint16_t)PLC_CMF_PKT_SIZE);
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
            lDRV_PLC_PHY_COMM_SpiReadCmd(RX_DAT_ID, sDataRxDat, evObj.rcvDataLength);
            /* update event flag */
            gPlcPhyObj->evRxDat = true;
        }

        /* Check received new parameters event (Second event in RX) */
        if (evObj.evRxPar)
        {
            lDRV_PLC_PHY_COMM_SpiReadCmd(RX_PAR_ID, sDataRxPar, (uint16_t)PLC_RX_PAR_SIZE - 4U);
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
            lDRV_PLC_PHY_COMM_SpiReadCmd(REG_INFO_ID, sDataReg, evObj.regRspLength);
            /* update event flag */
            gPlcPhyObj->evRegRspLength = evObj.regRspLength;
        }

        /* Time guard */
        gPlcPhyObj->plcHal->delay(20);
    }
<#if SPI_PLIB?lower_case[0..*6] != "sercom">

    /* PORT Interrupt Status Clear */
<#if (PLC_PIO_ID??) && (PLC_PIO_ID == "11264")>
    (&(PIO0_REGS->PIO_GROUP[DRV_PLC_EXT_INT_PIO_PORT]))->PIO_ISR;
<#else>
    ((pio_registers_t*)DRV_PLC_EXT_INT_PIO_PORT)->PIO_ISR;
</#if>
</#if>
}
