/*******************************************************************************
  Implementation of Phy Sniffer Serialization.

  Company:
    Microchip Technology Inc.

  File Name:
    srv_psniffer.c

  Summary:
    Phy Sniffer serialization service used by Microchip PLC Sniffer Tool.

  Description:
    The Phy Sniffer serialization provides a service to format messages
    through serial connection in order to communicate with PLC Sniffer Tool 
    provided by Microchip.
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
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "stddef.h"
#include "string.h"
#include "srv_psniffer.h"
#include "driver/plc/phy/drv_plc_phy_comm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
static DRV_PLC_PHY_TRANSMISSION_OBJ srvPsnifferLastTxObj[2];
static uint8_t srvPsnifferLastTxData[512][2];
static uint16_t srvPsnifferLastRxPayloadSymbols;
static uint16_t srvPsnifferLastTxPayloadSymbols;
static uint8_t srvPsnifferChannel = 1;

// *****************************************************************************
// *****************************************************************************
// Section: SRV_PSNIFFER Interface Routines
// *****************************************************************************
// *****************************************************************************
static uint32_t SRV_PSNIFFER_GetMessageDuration(DRV_PLC_PHY_FRAME_TYPE frameType, uint8_t symbols)
{
    uint32_t duration;

    if (frameType == FRAME_TYPE_A) {
        duration = PSNIFFER_P13_PREAMBLE_US + PSNIFFER_P13_HEADER_US + (symbols *  PSNIFFER_SYMBOL_US);
    } else if (frameType == FRAME_TYPE_B) {
        duration = PSNIFFER_PP_PREAMBLE_US + PSNIFFER_PP_HEADER_US + (symbols * PSNIFFER_SYMBOL_US);
    } else if (frameType == FRAME_TYPE_BC) {
        duration = PSNIFFER_P13_PREAMBLE_US + PSNIFFER_P13_HEADER_US + PSNIFFER_PP_PREAMBLE_US + PSNIFFER_PP_HEADER_US +
                (symbols * PSNIFFER_SYMBOL_US);
    } else {
        duration = 0;
    }

    return duration;
}

SRV_PSNIFFER_COMMAND SRV_PSNIFFER_GetCommand(uint8_t* pData)
{
    /* Extract Command */
    return (SRV_PSNIFFER_COMMAND)*pData;
}

void SRV_PSNIFFER_SetTxMessage(DRV_PLC_PHY_TRANSMISSION_OBJ* pTxObj)
{
    DRV_PLC_PHY_TRANSMISSION_OBJ* pLastTxObj;
    
    /* Use internal buffer to store TX messages to use it as a received message when TX_CFM arrives */
    pLastTxObj = (DRV_PLC_PHY_TRANSMISSION_OBJ *)&srvPsnifferLastTxObj[pTxObj->bufferId];
    memcpy((uint8_t *)pLastTxObj, (uint8_t *)pTxObj, sizeof(DRV_PLC_PHY_TRANSMISSION_OBJ));
    
    pLastTxObj->pTransmitData = (uint8_t*)&srvPsnifferLastTxData[pTxObj->bufferId];
    memcpy(pLastTxObj->pTransmitData, pTxObj->pTransmitData, pTxObj->dataLength);
}

void SRV_PSNIFFER_SetRxPayloadSymbols(uint16_t payloadSym)
{
    srvPsnifferLastRxPayloadSymbols = payloadSym;
}

void SRV_PSNIFFER_SetTxPayloadSymbols(uint16_t payloadSym)
{
    srvPsnifferLastTxPayloadSymbols = payloadSym;
}

void SRV_PSNIFFER_SetPLCChannel(uint8_t channel)
{
    if ((channel > 0) && (channel < 15))
    {
        srvPsnifferChannel = channel;
    }
}

size_t SRV_PSNIFFER_SerialRxMessage(uint8_t* pDataDst, DRV_PLC_PHY_RECEPTION_OBJ* pDataSrc)
{
    uint8_t* pData;
    uint32_t timeIni, timeEnd;
    uint8_t snr;
    
    pData = pDataDst;
    
    /* Insert Message type */
    if (pDataSrc->frameType == FRAME_TYPE_A)
    {
        *pData++ = PSNIFFER_MSG_TYPE_A; 
    }
    else if (pDataSrc->frameType == FRAME_TYPE_B)
    {
        *pData++ = PSNIFFER_MSG_TYPE_B;         
    }
    else if (pDataSrc->frameType == FRAME_TYPE_BC)
    {
        *pData++ = PSNIFFER_MSG_TYPE_BC;        
    }
    else
    {
        return 0;
    }    

    /* Insert version identifiers */
    *pData++ = PSNIFFER_VERSION; 
    *pData++ = PSNIFFER_PROFILE;     
    /* Fill data */
    *pData++ = (uint8_t)pDataSrc->scheme; 
    *pData++ = (uint8_t)srvPsnifferLastRxPayloadSymbols;     
    /* SNR = (QT/(3 * 4)) + 1 */
    /* QT = CINR_MIN + 12 */
    snr = ((pDataSrc->cinrMin + 12) / 12) + 1;
    if (snr > 7) {
        snr = 7;
    }
    *pData++ = snr;
    /* Added 2 extra to make round instead trunk */
    *pData++ = (pDataSrc->cinrAvg + 12 + 2) / 4; 
    *pData++ = srvPsnifferChannel;
    *pData++ = (uint8_t)pDataSrc->cinrMin;
    *pData++ = (uint8_t)pDataSrc->berSoftAvg;
    *pData++ = (uint8_t)pDataSrc->berSoftMax;
    /* padding (reserved bytes) */
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    /* Adapt RX TIME to 10us base */
    timeIni = pDataSrc->time;
    timeEnd = timeIni + SRV_PSNIFFER_GetMessageDuration(pDataSrc->frameType, srvPsnifferLastRxPayloadSymbols);
    *pData++ = (uint8_t)(timeIni >> 24);
    *pData++ = (uint8_t)(timeIni >> 16);
    *pData++ = (uint8_t)(timeIni >> 8);
    *pData++ = (uint8_t)(timeIni);
    *pData++ = (uint8_t)(timeEnd >> 24);
    *pData++ = (uint8_t)(timeEnd >> 16);
    *pData++ = (uint8_t)(timeEnd >> 8);
    *pData++ = (uint8_t)(timeEnd);
    *pData++ = 0;
    *pData++ = pDataSrc->rssiAvg;
    /* mac_enable not supported */
    *pData++ = 0;
    /* Length in bytes */
    *pData++ = (uint8_t)(pDataSrc->dataLength >> 8);
    *pData++ = (uint8_t)(pDataSrc->dataLength);
    /* PDU */
    memcpy(pData, pDataSrc->pReceivedData, pDataSrc->dataLength);
    pData += pDataSrc->dataLength;
    
    return (pData - pDataDst);    
}

size_t SRV_PSNIFFER_SerialCfmMessage(uint8_t* pDataDst, DRV_PLC_PHY_TRANSMISSION_CFM_OBJ* pCfmObj)
{    
    DRV_PLC_PHY_TRANSMISSION_OBJ* pTxObj;
    uint8_t* pData;
    uint32_t timeIni, timeEnd;
    
    if (pCfmObj->result != DRV_PLC_PHY_TX_RESULT_SUCCESS)
    {
        /* Error in transmission: No report */
        return 0;
    }

    pTxObj = (DRV_PLC_PHY_TRANSMISSION_OBJ*)&srvPsnifferLastTxObj[pCfmObj->bufferId];
    
    pData = pDataDst;
    
    /* Insert Message type */
    if (pTxObj->frameType == FRAME_TYPE_A)
    {
        *pData++ = PSNIFFER_MSG_TYPE_A; 
    }
    else if (pTxObj->frameType == FRAME_TYPE_B)
    {
        *pData++ = PSNIFFER_MSG_TYPE_B;         
    }
    else if (pTxObj->frameType == FRAME_TYPE_BC)
    {
        *pData++ = PSNIFFER_MSG_TYPE_BC;        
    }
    else
    {
        return 0;
    }    

    /* Insert version identifiers */
    *pData++ = PSNIFFER_VERSION; 
    *pData++ = PSNIFFER_PROFILE;     
    /* Fill data */
    *pData++ = (uint8_t)pTxObj->scheme; 
    *pData++ = (uint8_t)srvPsnifferLastTxPayloadSymbols; 
    *pData++ = 7;               /* fix SNR */
    *pData++ = 60;              /* fix EX_SNR */
    *pData++ = srvPsnifferChannel;
    *pData++ = 255;
    *pData++ = 0;
    *pData++ = 0;
    /* padding (reserved bytes) */
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    /* Adapt RX TIME to 10us base */
    timeIni = pCfmObj->time;
    timeEnd = timeIni + SRV_PSNIFFER_GetMessageDuration(pCfmObj->frameType, srvPsnifferLastTxPayloadSymbols);
    *pData++ = (uint8_t)(timeIni >> 24);
    *pData++ = (uint8_t)(timeIni >> 16);
    *pData++ = (uint8_t)(timeIni >> 8);
    *pData++ = (uint8_t)(timeIni);
    *pData++ = (uint8_t)(timeEnd >> 24);
    *pData++ = (uint8_t)(timeEnd >> 16);
    *pData++ = (uint8_t)(timeEnd >> 8);
    *pData++ = (uint8_t)(timeEnd);
    *pData++ = 0;
    *pData++ = 140;             /* fix RSSI */
    /* mac_enable not supported */
    *pData++ = 0;
    /* Length in bytes */
    *pData++ = (uint8_t)(pTxObj->dataLength >> 8);
    *pData++ = (uint8_t)(pTxObj->dataLength);
    /* PDU */
    memcpy(pData, pTxObj->pTransmitData, pTxObj->dataLength);
    pData += pTxObj->dataLength;
    
    return (pData - pDataDst);    
}
