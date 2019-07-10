/*******************************************************************************
  Phy layer serialization service used by Microchip PLC Tools

  Company:
    Microchip Technology Inc.

  File Name:
    srv_pserial.c

  Summary:
    Phy layer serialization service used by Microchip PLC Tools.

  Description:
    The Phy layer serialization provides a service to format messages
    through serial connection in order to communicate with PLC Tools provided
    by Microchip.
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
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "stddef.h"
#include "string.h"
#include "srv_pserial.h"
#include "driver/plc/phy/drv_plc_phy_comm.h"

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

static void _SRV_SERIAL_memcpyRev(uint8_t *pDataDst, uint8_t *pDataSrc, size_t length)
{
    uint8_t *pMemDst, *pMemSrc;
    uint16_t indexRev;

    if (length <= 4) {
        pMemDst = pDataDst + length - 1;
        pMemSrc = pDataSrc;
        for (indexRev = 0; indexRev < length; indexRev++) {
            *pMemDst-- = (uint8_t)*pMemSrc++;
        }
    } else {
        memcpy(pDataDst, pDataSrc, length);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: SRV_PSERIAL Interface Routines
// *****************************************************************************
// *****************************************************************************

SRV_PSERIAL_COMMAND SRV_PSERIAL_GetCommand(uint8_t* pData) 
{
    /* Extract Command */
    return (SRV_PSERIAL_COMMAND)*pData;
}

void SRV_PSERIAL_ParseGetPIB(DRV_PLC_PHY_PIB_OBJ* pDataDst, uint8_t* pDataSrc)
{
    /* Skip command */
    pDataSrc++;
    /* Extract parameters of PIB */
    pDataDst->id = (DRV_PLC_PHY_ID)(((uint16_t)*pDataSrc++) << 8);
    pDataDst->id += (DRV_PLC_PHY_ID)((uint16_t)*pDataSrc++);
    pDataDst->length = *pDataSrc;    
}

size_t SRV_PSERIAL_SerialGetPIB(uint8_t* pDataDst, DRV_PLC_PHY_PIB_OBJ* pDataSrc)
{
    uint8_t* pData;
    
    pData = pDataDst;
    
    /* Insert command */
    *pData++ = SRV_PSERIAL_CMD_PHY_GET_CFG_RSP;
    
    /* Serialize parameters of PIB */
    *pData++ = (uint8_t)((pDataSrc->id) >> 8);
    *pData++ = (uint8_t)(pDataSrc->id);
    *pData++ = (uint8_t)(pDataSrc->length);
    
    if (pDataSrc->length > 4) {
        memcpy(pData, pDataSrc->pData, pDataSrc->length);
    } else {
        _SRV_SERIAL_memcpyRev(pData, pDataSrc->pData, pDataSrc->length);
    }
    
    pData += pDataSrc->length;
    
    return (pData - pDataDst);    
}

void SRV_PSERIAL_ParseSetPIB(DRV_PLC_PHY_PIB_OBJ* pDataDst, uint8_t* pDataSrc)
{
    /* Skip command */
    pDataSrc++;
    /* Extract parameters of PIB */
    pDataDst->id = (DRV_PLC_PHY_ID)(((uint16_t)*pDataSrc++) << 8);
    pDataDst->id += (DRV_PLC_PHY_ID)((uint16_t)*pDataSrc++);
    pDataDst->length = *pDataSrc++;    
    _SRV_SERIAL_memcpyRev(pDataDst->pData, pDataSrc, pDataDst->length);
}

size_t SRV_PSERIAL_SerialSetPIB(uint8_t* pDataDst, DRV_PLC_PHY_PIB_OBJ* pDataSrc)
{
    uint8_t* pData;
    
    pData = pDataDst;
    
    /* Insert command */
    *pData++ = SRV_PSERIAL_CMD_PHY_SET_CFG_RSP;

    /* Serialize parameters of PIB */
    *pData++ = (uint8_t)((pDataSrc->id) >> 8);
    *pData++ = (uint8_t)(pDataSrc->id);
    *pData++ = (uint8_t)(pDataSrc->length);
    *pData++ = true;
    
    return (pData - pDataDst);    
}

void SRV_PSERIAL_ParseTxMessage(DRV_PLC_PHY_TRANSMISSION_OBJ* pDataDst, uint8_t* pDataSrc)
{    
    /* Skip command */
    pDataSrc++;

    /* Extract parameters of transmission */
    pDataDst->bufferId = (DRV_PLC_PHY_BUFFER_ID)*pDataSrc++;
    pDataDst->attenuation = *pDataSrc++;
    pDataDst->frameType = (DRV_PLC_PHY_FRAME_TYPE)(*pDataSrc++);
    pDataDst->scheme = (DRV_PLC_PHY_SCH)(*pDataSrc++);
    pDataDst->mode = *pDataSrc++;
    pDataDst->time = ((uint32_t)*pDataSrc++) << 24;
    pDataDst->time += ((uint32_t)*pDataSrc++) << 16;
    pDataDst->time += ((uint32_t)*pDataSrc++) << 8;
    pDataDst->time += (uint32_t)*pDataSrc++;
    pDataDst->forced += (uint32_t)*pDataSrc++;
    pDataDst->dataLength = ((uint16_t)*pDataSrc++) << 8;
    pDataDst->dataLength += (uint16_t)*pDataSrc++;
    
    if (pDataDst->frameType == FRAME_TYPE_A) 
    {
        pDataDst->pTransmitData[0] = 0xAA;
        pDataDst->pTransmitData[1] = 0xAA;
        pDataDst->pTransmitData[2] = 0xAA;
        pDataDst->dataLength += 3;
        /* copy data */
        memcpy(pDataDst->pTransmitData + 3, pDataSrc, pDataDst->dataLength);
    }
    else
    {
        /* copy data */
        memcpy(pDataDst->pTransmitData, pDataSrc, pDataDst->dataLength);
    }
}

size_t SRV_PSERIAL_SerialRxMessage(uint8_t* pDataDst, DRV_PLC_PHY_RECEPTION_OBJ* pDataSrc)
{
    uint8_t* pData;
    
    pData = pDataDst;
    
    /* Insert command */
    *pData++ = SRV_PSERIAL_CMD_PHY_RECEIVE_MSG;  

    /* Serialize parameters of PIB */
    *pData++ = (uint8_t)(pDataSrc->frameType);
    *pData++ = (uint8_t)(pDataSrc->scheme);
    *pData++ = (uint8_t)(pDataSrc->headerType);
    *pData++ = (uint8_t)(pDataSrc->berSoftAvg);
    *pData++ = (uint8_t)(pDataSrc->berSoftMax);
    *pData++ = (uint8_t)(pDataSrc->cinrMin);
    *pData++ = (uint8_t)(pDataSrc->cinrAvg);
    *pData++ = (uint8_t)(pDataSrc->rssiAvg);
    *pData++ = (uint8_t)((pDataSrc->evmHeader) >> 8);
    *pData++ = (uint8_t)(pDataSrc->evmHeader);
    *pData++ = (uint8_t)((pDataSrc->evmPayload) >> 8);
    *pData++ = (uint8_t)(pDataSrc->evmPayload);
    *pData++ = (uint8_t)((pDataSrc->evmHeaderAcum) >> 24);
    *pData++ = (uint8_t)((pDataSrc->evmHeaderAcum) >> 16);
    *pData++ = (uint8_t)((pDataSrc->evmHeaderAcum) >> 8);
    *pData++ = (uint8_t)(pDataSrc->evmHeaderAcum);
    *pData++ = (uint8_t)((pDataSrc->evmPayloadAcum) >> 24);
    *pData++ = (uint8_t)((pDataSrc->evmPayloadAcum) >> 16);
    *pData++ = (uint8_t)((pDataSrc->evmPayloadAcum) >> 8);
    *pData++ = (uint8_t)(pDataSrc->evmPayloadAcum);
    *pData++ = (uint8_t)((pDataSrc->time) >> 24);
    *pData++ = (uint8_t)((pDataSrc->time) >> 16);
    *pData++ = (uint8_t)((pDataSrc->time) >> 8);
    *pData++ = (uint8_t)(pDataSrc->time);
    *pData++ = (uint8_t)(pDataSrc->narBandPercent);
    *pData++ = (uint8_t)(pDataSrc->impNoisePercent);
    
    if (pDataSrc->frameType == FRAME_TYPE_A) 
    {
        /* remove Generic Data Frame Header */
        pDataSrc->dataLength -= 3;
        *pData++ = (uint8_t)((pDataSrc->dataLength) >> 8);
        *pData++ = (uint8_t)(pDataSrc->dataLength);
        /* copy data */
        memcpy(pData, pDataSrc->pReceivedData + 3, pDataSrc->dataLength);
    }
    else
    {
        /* copy data */
        *pData++ = (uint8_t)((pDataSrc->dataLength) >> 8);
        *pData++ = (uint8_t)(pDataSrc->dataLength);
        memcpy(pData, pDataSrc->pReceivedData + 3, pDataSrc->dataLength);
    }
    
    pData += pDataSrc->dataLength;
    
    return (pData - pDataDst);    
}

size_t SRV_PSERIAL_SerialCfmMessage(uint8_t* pDataDst, DRV_PLC_PHY_TRANSMISSION_CFM_OBJ* pDataSrc)
{
    uint8_t* pData;
    
    pData = pDataDst;
    
    /* Insert command */
    *pData++ = SRV_PSERIAL_CMD_PHY_SEND_MSG_RSP;
    
    /* Serialize parameters of Confirm Message */
    *pData++ = (uint8_t)(pDataSrc->bufferId);
    *pData++ = (uint8_t)(pDataSrc->result);
    *pData++ = (uint8_t)((pDataSrc->rmsCalc) >> 24);
    *pData++ = (uint8_t)((pDataSrc->rmsCalc) >> 16);
    *pData++ = (uint8_t)((pDataSrc->rmsCalc) >> 8);
    *pData++ = (uint8_t)(pDataSrc->rmsCalc);
    *pData++ = (uint8_t)((pDataSrc->time) >> 24);
    *pData++ = (uint8_t)((pDataSrc->time) >> 16);
    *pData++ = (uint8_t)((pDataSrc->time) >> 8);
    *pData++ = (uint8_t)(pDataSrc->time);
      
    return (pData - pDataDst);    
}
