/******************************************************************************
  DRV_G3_MACRT_COMM Profile Layer

  Company:
    Microchip Technology Inc.

  File Name:
    drv_g3_macrt_comm.c

  Summary:
    G3 MAC RT Driver Communication Profile Layer

  Description:
    This file contains the source code for the implementation of the G3 Profile 
    Layer.
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
#include <stdbool.h>
#include "configuration.h"
#include "driver/g3MacRt/drv_plc_hal.h"
#include "driver/g3MacRt/drv_plc_boot.h"
#include "driver/g3MacRt/drv_g3_macrt.h"
#include "driver/g3MacRt/drv_g3_macrt_local.h"
#include "driver/g3MacRt/drv_g3_macrt_local_comm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* This is the driver instance object array. */
static DRV_G3_MACRT_OBJ *gG3MacRtObj = NULL;

/* Buffer definition to communicate with G3 MACRT device */
static uint8_t gG3Info[PLC_STATUS_LENGTH];
static uint8_t gG3TxData[DRV_G3_MACRT_DATA_MAX_SIZE];
static uint8_t gG3MlmeSetData[DRV_G3_MACRT_MLME_SET_SIZE];
static uint8_t gG3MlmeGetData[DRV_G3_MACRT_MLME_GET_SIZE];
static uint8_t gG3RxData[DRV_G3_MACRT_DATA_MAX_SIZE];
static uint8_t gG3TxCfmData[DRV_G3_MACRT_TX_CFM_SIZE];
static uint8_t gG3ToneMapData[DRV_G3_MACRT_TONEMAP_RSP_SIZE];
static uint8_t gG3RegData[DRV_G3_MACRT_REG_PKT_SIZE];

/* Local vars */
static MAC_RT_MOD_TYPE gG3LastModType;

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

uint16_t _DRV_G3_MACRT_COMM_GetDelayUs(MAC_RT_PHY_PIB id)
{
    uint16_t delay = 50;

    switch (id) 
    {
        case PHY_PARAM_TONE_MASK:
        delay = 600;
        break;

        case PHY_PARAM_PREDIST_COEF_TABLE_HI:
        case PHY_PARAM_PREDIST_COEF_TABLE_LO:
        delay = 250;
        break;

        case PHY_PARAM_PREDIST_COEF_TABLE_VLO:
        delay = 350;
        break;

        default:
        delay = 50;
        break;
    }

    return delay;
}

static bool _DRV_G3_MACRT_COMM_CheckComm(DRV_PLC_HAL_INFO *info)
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
            DRV_PLC_BOOT_Restart(false);
            if (gG3MacRtObj->exceptionCallback)
            {
                gG3MacRtObj->exceptionCallback(DRV_G3_MACRT_EXCEPTION_DEBUG, 
                        gG3MacRtObj->contextExc);
            }
        }
        else
        {
            /* PLC needs boot process to upload firmware */
            DRV_PLC_BOOT_Restart(true);
            if (gG3MacRtObj->exceptionCallback)
            {
                gG3MacRtObj->exceptionCallback(DRV_G3_MACRT_EXCEPTION_RESET, 
                        gG3MacRtObj->contextExc);
            }
        }
    }
    else
    {
        /* PLC needs boot process to upload firmware */
        DRV_PLC_BOOT_Restart(true);
        if (gG3MacRtObj->exceptionCallback)
        {
            gG3MacRtObj->exceptionCallback(
                    DRV_G3_MACRT_EXCEPTION_UNEXPECTED_KEY, 
                    gG3MacRtObj->contextExc
            );
        }
    }
    
    /* Check if there is any tx_cfm pending to be reported */
    if (gG3MacRtObj->state == DRV_G3_MACRT_STATE_WAITING_TX_CFM)
    {
        gG3MacRtObj->evResetTxCfm = true;
    }
    
    return false;
}

static void _DRV_G3_MACRT_COMM_SpiWriteCmd(DRV_G3_MACRT_MEM_ID id, uint8_t *pData, 
        uint16_t length)
{
    DRV_PLC_HAL_CMD halCmd;
    DRV_PLC_HAL_INFO halInfo;
    uint8_t failures = 0;
    
    /* Disable external interrupt from PLC */
    gG3MacRtObj->plcHal->enableExtInt(false);
    
    halCmd.cmd = DRV_PLC_HAL_CMD_WR;
    halCmd.memId = id;
    halCmd.length = length;
    halCmd.pData = pData;
    
    gG3MacRtObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    
    /* Check communication integrity */
    while(!_DRV_G3_MACRT_COMM_CheckComm(&halInfo))
    {
        failures++;
        if (failures == 2) {
            if (gG3MacRtObj->exceptionCallback)
            {
                gG3MacRtObj->exceptionCallback(
                        DRV_G3_MACRT_EXCEPTION_CRITICAL_ERROR, 
                        gG3MacRtObj->contextExc
                );
            }
            break;
        }
        gG3MacRtObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    }  
    
    /* Enable external interrupt from PLC */
    gG3MacRtObj->plcHal->enableExtInt(true); 
}

static void _DRV_G3_MACRT_COMM_SpiReadCmd(DRV_G3_MACRT_MEM_ID id, uint8_t *pData, 
        uint16_t length)
{
    DRV_PLC_HAL_CMD halCmd;
    DRV_PLC_HAL_INFO halInfo;
    uint8_t failures = 0;
    
    /* Disable external interrupt from PLC */
    gG3MacRtObj->plcHal->enableExtInt(false);
    
    halCmd.cmd = DRV_PLC_HAL_CMD_RD;
    halCmd.memId = id;
    halCmd.length = length;
    halCmd.pData = pData;
    
    gG3MacRtObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    
    /* Check communication integrity */
    while(!_DRV_G3_MACRT_COMM_CheckComm(&halInfo))
    {
        failures++;
        if (failures == 2) {
            if (gG3MacRtObj->exceptionCallback)
            {
                gG3MacRtObj->exceptionCallback(
                        DRV_G3_MACRT_EXCEPTION_CRITICAL_ERROR, 
                        gG3MacRtObj->contextExc
                );
            }
            break;
        }
        gG3MacRtObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    }    
    
    /* Enable external interrupt from PLC */
    gG3MacRtObj->plcHal->enableExtInt(true); 
}

static void _DRV_G3_MACRT_COMM_GetEventsInfo(DRV_G3_MACRT_EVENTS_OBJ *eventsObj)
{
    uint8_t *pData;
    DRV_PLC_HAL_CMD halCmd;
    DRV_PLC_HAL_INFO halInfo;
    uint8_t failures = 0;
    
    pData = gG3Info;    
    
    halCmd.cmd = DRV_PLC_HAL_CMD_RD;
    halCmd.memId = STATUS_ID;
    halCmd.length = PLC_STATUS_LENGTH;
    halCmd.pData = pData;
    
    gG3MacRtObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    
    /* Check communication integrity */
    while(!_DRV_G3_MACRT_COMM_CheckComm(&halInfo))
    {
        failures++;
        if (failures == 2) {
            if (gG3MacRtObj->exceptionCallback)
            {
                gG3MacRtObj->exceptionCallback(
                        DRV_G3_MACRT_EXCEPTION_CRITICAL_ERROR, 
                        gG3MacRtObj->contextExc
                );
            }
            break;
        }
        gG3MacRtObj->plcHal->sendWrRdCmd(&halCmd, &halInfo);
    }    
    
    /* Extract Events information */
    eventsObj->evTxCfm = (halInfo.flags & DRV_G3_MACRT_EV_TX_CFM_MASK)? 1:0;
    eventsObj->evDataInd = (halInfo.flags & DRV_G3_MACRT_EV_DATA_IND_MASK)? 1:0;
    eventsObj->evMlmeSetCfm = (halInfo.flags & DRV_G3_MACRT_EV_MLME_SET_CFM_MASK)? 1:0;
    eventsObj->evMlmeGetCfm = (halInfo.flags & DRV_G3_MACRT_EV_MLME_GET_CFM_MASK)? 1:0;
    eventsObj->evReg = (halInfo.flags & DRV_G3_MACRT_EV_REG_RSP_MASK)? 1:0;
    eventsObj->evToneMapRsp = (halInfo.flags & DRV_G3_MACRT_EV_TONMAP_RSP_MASK)? 1:0;
    eventsObj->evSniffer = (halInfo.flags & DRV_G3_MACRT_EV_SNIFFER_MASK)? 1:0;
    
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
// Section: DRV_PLC Common Interface Implementation
// *****************************************************************************
// *****************************************************************************
void DRV_G3_MACRT_Init(DRV_G3_MACRT_OBJ *g3MacRt)
{
    gG3MacRtObj = g3MacRt;
    
    /* Clear information about PLC events */
    gG3MacRtObj->evDataIndLength = 0;
    gG3MacRtObj->evMlmeGetCfm = false;
    gG3MacRtObj->evRegRspLength = 0;
    gG3MacRtObj->evResetTxCfm = false;
    gG3MacRtObj->evToneMapRsp = false;
    gG3MacRtObj->evTxCfm = false;
    
    /* Enable external interrupt from PLC */
    gG3MacRtObj->plcHal->enableExtInt(true);
}

void DRV_G3_MACRT_Task(void)
{
    /* Check event flags */
    if ((gG3MacRtObj->evTxCfm) || (gG3MacRtObj->evResetTxCfm))
    {
        MAC_RT_TX_CFM_OBJ *pTxCfmObj;
        MAC_RT_TX_CFM_OBJ txCfmObj;
    
        if (gG3MacRtObj->evResetTxCfm)
        {
            /* Reset event flag */
            gG3MacRtObj->evResetTxCfm = false;
            
            /* Fill Tx Cfm object in case of reset while transmission */
            pTxCfmObj = &txCfmObj;
            txCfmObj.status = MAC_RT_STATUS_DENIED;
            txCfmObj.modType = gG3LastModType;
            txCfmObj.updateTimestamp = false;
        } else {
            pTxCfmObj = (MAC_RT_TX_CFM_OBJ *)gG3TxCfmData;
        }
        
        /* Report to upper layer */
        if (gG3MacRtObj->txCfmCallback)
        {
            gG3MacRtObj->txCfmCallback(pTxCfmObj, gG3MacRtObj->contextTxCfm);
        }
        
        /* Update MAC RT state */
        gG3MacRtObj->state = DRV_G3_MACRT_STATE_IDLE;
        /* Reset event flag */
        gG3MacRtObj->evTxCfm = false;
    }
    
    if (gG3MacRtObj->evMlmeGetCfm)
    {
        MAC_RT_RX_PARAMETERS_OBJ *pRxParams;
        
        if (gG3MacRtObj->mlmeGetCfmcallback)
        {
            /* Report to upper layer */
            pRxParams = (MAC_RT_RX_PARAMETERS_OBJ *)gG3MlmeGetData;
            gG3MacRtObj->mlmeGetCfmcallback(pRxParams, gG3MacRtObj->contextMlmeGetCfm);
        }
        
        /* Reset event flag */
        gG3MacRtObj->evMlmeGetCfm = false;
    }
    
    if (gG3MacRtObj->evDataIndLength)
    {   
        if (gG3MacRtObj->dataIndCallback)
        {
            /* Report to upper layer */
            gG3MacRtObj->dataIndCallback(gG3RxData, gG3MacRtObj->evDataIndLength,
                    gG3MacRtObj->contextDataInd);
        }
        
        /* Reset event flag */
        gG3MacRtObj->evDataIndLength = 0;
    }
    
    if (gG3MacRtObj->evSniffer)
    {   
        if (gG3MacRtObj->snifferDataCallback)
        {
            /* Report to upper layer */
            gG3MacRtObj->snifferDataCallback(gG3MacRtObj->contextSniffer);
        }
        
        /* Reset event flag */
        gG3MacRtObj->evSniffer = false;
    }
}

void DRV_G3_MACRT_Send(const DRV_HANDLE handle, uint8_t *pData, uint16_t length)
{    
    if((handle != DRV_HANDLE_INVALID) && (handle == 0) && 
       (gG3MacRtObj->state == DRV_G3_MACRT_STATE_IDLE))
    {
        /* Check Length */
		if ((length > 0) && (length <= (MAC_RT_MAX_PAYLOAD_SIZE - 2))) {
            uint8_t *pTxData;
            
			/* Update PLC state: transmitting */
            gG3MacRtObj->state = DRV_G3_MACRT_STATE_TX;

            /* Build and Send TX data */
            pTxData = gG3TxData;
            *pTxData++= (uint8_t)(length >> 8);
            *pTxData++= (uint8_t)length;
            memcpy(pTxData, pData, length);
            _DRV_G3_MACRT_COMM_SpiWriteCmd(TX_REQ_ID, gG3TxData, length + 2);
		}
    }
}

void DRV_G3_MACRT_MlmeSet(const DRV_HANDLE handle, MAC_RT_TX_PARAMETERS_OBJ *pParameters)
{    
    if((handle != DRV_HANDLE_INVALID) && (handle == 0) && 
       (gG3MacRtObj->state == DRV_G3_MACRT_STATE_IDLE))
    {
        /* Send TX parameters */
        memcpy(gG3MlmeSetData, (uint8_t *)pParameters, sizeof(gG3MlmeSetData));
        
        _DRV_G3_MACRT_COMM_SpiWriteCmd(MLME_SET_ID, gG3MlmeSetData, 
                sizeof(gG3MlmeSetData));
    }
}

MAC_RT_STATUS DRV_G3_MACRT_PIBGet(const DRV_HANDLE handle, MAC_RT_PIB_OBJ *pibObj)
{    
    if ((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {        
        uint8_t *pDst;
        uint16_t waitCounter;
        uint16_t dummyValue;
        
        /* Check Length */
		if (pibObj->length > MAC_RT_PIB_MAX_VALUE_LENGTH) {
			/* Length error */
			return MAC_RT_STATUS_INVALID_PARAMETER;
		}
        
        /* Build command */
        pDst = gG3RegData;

        *pDst++ = MAC_RT_REG_CMD_RD;
        *pDst++ = (uint8_t)(pibObj->pib >> 24);
        *pDst++ = (uint8_t)(pibObj->pib >> 16);
        *pDst++ = (uint8_t)(pibObj->pib >> 8);
        *pDst++ = (uint8_t)(pibObj->pib);
        *pDst++ = (uint8_t)(pibObj->index >> 8);
        *pDst++ = (uint8_t)(pibObj->index);
        *pDst++ = (uint8_t)(pibObj->length);
        memcpy(pDst, pibObj->value, pibObj->length);
        pDst += pibObj->length;

        /* Send PIB information request */
        _DRV_G3_MACRT_COMM_SpiWriteCmd(REG_INFO_ID, gG3RegData, pDst - gG3RegData);
        
        /* Sync function: Wait to response from interrupt */
		waitCounter = 100;
		dummyValue = gG3MacRtObj->evRegRspLength;
		while (!dummyValue) {
			/* Wait for interrupt. The CPU is in sleep mode until an interrupt occurs. */
			__asm("WFI");
			if (!waitCounter--) {
				/* Error in get cmd */
				return MAC_RT_STATUS_TRANSACTION_OVERFLOW;
			}

			dummyValue = gG3MacRtObj->evRegRspLength;
		}

		/* Check Response Content */
		if (*gG3RegData != MAC_RT_STATUS_SUCCESS) {
			/* Not success process */
			return (MAC_RT_STATUS)*gG3RegData;
		}

        /* Copy Reg data in PIB object */
		memcpy(pibObj->value, gG3RegData + 8, gG3MacRtObj->evRegRspLength);
		pibObj->length = gG3MacRtObj->evRegRspLength;

		/* Reset event flag */
		gG3MacRtObj->evRegRspLength = 0;

        return MAC_RT_STATUS_SUCCESS;
    }
    
    return MAC_RT_STATUS_DENIED;
}

MAC_RT_STATUS DRV_G3_MACRT_PIBSet(const DRV_HANDLE handle, MAC_RT_PIB_OBJ *pibObj)
{    
    if ((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {        
        uint8_t *pDst;
        uint16_t waitCounter;
        uint16_t dummyValue;
        
        /* Check Length */
		if (pibObj->length > MAC_RT_PIB_MAX_VALUE_LENGTH) {
			/* Length error */
			return MAC_RT_STATUS_INVALID_PARAMETER;
		}
        
        /* Build command */
        pDst = gG3RegData;

        *pDst++ = MAC_RT_REG_CMD_WR;
        *pDst++ = (uint8_t)(pibObj->pib >> 24);
        *pDst++ = (uint8_t)(pibObj->pib >> 16);
        *pDst++ = (uint8_t)(pibObj->pib >> 8);
        *pDst++ = (uint8_t)(pibObj->pib);
        *pDst++ = (uint8_t)(pibObj->index >> 8);
        *pDst++ = (uint8_t)(pibObj->index);
        *pDst++ = (uint8_t)(pibObj->length);
        memcpy(pDst, pibObj->value, pibObj->length);
        pDst += pibObj->length;

        /* Send PIB information request */
        _DRV_G3_MACRT_COMM_SpiWriteCmd(REG_INFO_ID, gG3RegData, pDst - gG3RegData);
        
        /* Sync function: Wait to response from interrupt */
		waitCounter = 100;
		dummyValue = gG3MacRtObj->evRegRspLength;
		while (!dummyValue) {
			/* Wait for interrupt. The CPU is in sleep mode until an interrupt occurs. */
			__asm("WFI");
			if (!waitCounter--) {
				/* Error in get cmd */
				return MAC_RT_STATUS_TRANSACTION_OVERFLOW;
			}

			dummyValue = gG3MacRtObj->evRegRspLength;
		}

        /* Additional delay to ensure writing operation completion. */
        if (pibObj->pib == MAC_RT_PIB_MANUF_PHY_PARAM)
        {
            dummyValue = _DRV_G3_MACRT_COMM_GetDelayUs(pibObj->index);
            gG3MacRtObj->plcHal->delay(dummyValue);
        }

        return MAC_RT_STATUS_SUCCESS;
    }
    
    return MAC_RT_STATUS_DENIED;
}

void DRV_G3_MACRT_GetToneMapResponse(const DRV_HANDLE handle, 
        MAC_RT_TONE_MAP_RSP *pParameters)
{    
    if ((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {        
        uint16_t waitCounter;
        uint16_t dummyValue;
        
        /* Send Tone Map response request */
        _DRV_G3_MACRT_COMM_SpiWriteCmd(TONE_MAP_REQ_ID, gG3ToneMapData, 
                sizeof(gG3ToneMapData));
        
        /* Sync function: Wait to response from interrupt */
		waitCounter = 100;
		dummyValue = gG3MacRtObj->evToneMapRsp;
		while (!dummyValue) {
			/* Wait for interrupt. The CPU is in sleep mode until an interrupt occurs. */
			__asm("WFI");
			if (!waitCounter--) {
				/* Error in get cmd */
				return;
			}

			dummyValue = gG3MacRtObj->evToneMapRsp;
		}

        /* Copy Tone Map Response data in pParameters */
		memcpy(pParameters, gG3ToneMapData, sizeof(gG3ToneMapData));
    }
}

void DRV_G3_MACRT_SetCoordinator(const DRV_HANDLE handle)
{    
    if ((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {        
        uint8_t coordinator;
        
        coordinator = 1;
        /* Send Coordinator Capabilities Request */
        _DRV_G3_MACRT_COMM_SpiWriteCmd(SET_COORD_ID, &coordinator, 1);
    }
}

void DRV_G3_MACRT_SetSpec15Compliance(const DRV_HANDLE handle)
{    
    if ((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {        
        uint8_t spec15;
        
        spec15 = 1;
        /* Send G3 Specification 15 Request */
        _DRV_G3_MACRT_COMM_SpiWriteCmd(SET_SPEC15_ID, &spec15, 1);
    }
}

uint32_t DRV_G3_MACRT_GetTimerReference(const DRV_HANDLE handle)
{    
    uint32_t timerReference = 0;
    
    if ((handle != DRV_HANDLE_INVALID) && (handle == 0))
    {        
        /* Read G3 Internal Timer Reference */
        _DRV_G3_MACRT_COMM_SpiReadCmd(STATUS_ID, (uint8_t *)&timerReference, 4);
    }
    
    return timerReference;
}

void DRV_G3_MACRT_ExternalInterruptHandler(PIO_PIN pin, uintptr_t context)
{   
    /* Avoid warning */
    (void)context;

    if ((gG3MacRtObj) && (pin == (PIO_PIN)gG3MacRtObj->plcHal->plcPlib->extIntPin))
    {
        DRV_G3_MACRT_EVENTS_OBJ evObj;
        
        /* Time guard */
        gG3MacRtObj->plcHal->delay(20);
        
        /* Get PLC events information */
        _DRV_G3_MACRT_COMM_GetEventsInfo(&evObj);
        
        /* Check confirmation of the transmission event */
        if (evObj.evTxCfm)
        {
            _DRV_G3_MACRT_COMM_SpiReadCmd(TX_CFM_ID, gG3TxCfmData, 
                    DRV_G3_MACRT_TX_CFM_SIZE);
            /* update event flag */
            gG3MacRtObj->evTxCfm = true;
            /* Update PLC state: idle */
            gG3MacRtObj->state = DRV_G3_MACRT_STATE_IDLE;
        }
        
        /* Check received new parameters event */
        if (evObj.evMlmeGetCfm)
        {        
            _DRV_G3_MACRT_COMM_SpiReadCmd(MLME_SET_CMF_ID, gG3MlmeGetData, 
                    DRV_G3_MACRT_MLME_GET_SIZE);
            /* update event flag */
            gG3MacRtObj->evMlmeGetCfm = true;
        }
        
        /* Check received new data event */
        if (evObj.evDataInd)
        {       
            if ((evObj.rcvDataLength == 0) || 
                (evObj.rcvDataLength > DRV_G3_MACRT_DATA_MAX_SIZE))
            {
                evObj.rcvDataLength = 1;
            }
            _DRV_G3_MACRT_COMM_SpiReadCmd(DATA_IND_ID, gG3RxData, 
                    evObj.rcvDataLength);
            /* update event flag */
            gG3MacRtObj->evDataIndLength = evObj.rcvDataLength;
        }
        
        /* Check Tone Map Response event */
        if (evObj.evToneMapRsp)
        {        
            _DRV_G3_MACRT_COMM_SpiReadCmd(TONE_MAP_REQ_ID, gG3ToneMapData, 
                    DRV_G3_MACRT_TONEMAP_RSP_SIZE);
            /* update event flag */
            gG3MacRtObj->evToneMapRsp = true;
        }
        
        /* Check Register info event */
        if (evObj.evReg)
        {          
            if ((evObj.regRspLength == 0) || 
                (evObj.regRspLength > DRV_G3_MACRT_REG_PKT_SIZE))
            {
                evObj.regRspLength = 1;
            }
            _DRV_G3_MACRT_COMM_SpiReadCmd(REG_INFO_ID, gG3RegData, 
                    evObj.regRspLength + 8);
            /* update event flag */
            gG3MacRtObj->evRegRspLength = evObj.regRspLength;
        }
        
        /* Check Sniffer event */
        if (evObj.evSniffer)
        {        
            uint16_t snfLength;
            
            snfLength = sizeof(MAC_RT_SNIFFER_HEADER) + evObj.regRspLength;
            
            _DRV_G3_MACRT_COMM_SpiReadCmd(SNIFFER_ID, gG3MacRtObj->pDataSniffer, 
                    snfLength);
            /* update event flag */
            gG3MacRtObj->evSniffer = true;
        }
        
        /* Time guard */
        gG3MacRtObj->plcHal->delay(50);
    }
    
    /* PORTD Interrupt Status Clear */
    ((pio_registers_t*)PIO_PORT_D)->PIO_ISR;
}
