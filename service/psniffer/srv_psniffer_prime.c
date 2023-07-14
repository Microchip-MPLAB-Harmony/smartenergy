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
static uint8_t srvPsnifferLastTxData[2][512];
static uint8_t srvPsnifferLastRxPayloadSymbols;
static uint8_t srvPsnifferLastTxPayloadSymbols;
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
    pLastTxObj = &srvPsnifferLastTxObj[pTxObj->bufferId];
    *pLastTxObj = *pTxObj;

    pLastTxObj->pTransmitData = srvPsnifferLastTxData[pTxObj->bufferId];
    (void) memcpy(pLastTxObj->pTransmitData, pTxObj->pTransmitData, pTxObj->dataLength);
}

void SRV_PSNIFFER_SetRxPayloadSymbols(uint16_t payloadSym)
{
    srvPsnifferLastRxPayloadSymbols = (uint8_t)payloadSym;
}

void SRV_PSNIFFER_SetTxPayloadSymbols(uint16_t payloadSym)
{
    srvPsnifferLastTxPayloadSymbols = (uint8_t)payloadSym;
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
    ptrdiff_t size;
    uint8_t* pData;
    uint32_t timeIni, timeEnd;
    int16_t snr;
    
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
    *pData++ = srvPsnifferLastRxPayloadSymbols;
    /* SNR = (QT/(3 * 4)) + 1 */
    /* QT = CINR_MIN + 12 */
    snr = ((int16_t)pDataSrc->cinrMin + 12 - 40) / 12;
    if (snr > 7)
    {
        *pData++ = 7;
    }
    else if (snr < 0)
    {
        *pData++ = 0;
    }
    else
    {
        *pData++ = (uint8_t)snr;
    }

    /* Added 2 extra to make round instead trunk */
    snr = ((int16_t)pDataSrc->cinrAvg + 12 + 2 - 40) / 4;
    if (snr < 0)
    {
        *pData++ = 0;
    }
    else
    {
        *pData++ = (uint8_t)snr;
    }

    *pData++ = srvPsnifferChannel;
    *pData++ = pDataSrc->cinrMin;
    *pData++ = pDataSrc->berSoftAvg;
    *pData++ = pDataSrc->berSoftMax;
    /* padding (reserved bytes) */
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    *pData++ = 0;
    timeIni = pDataSrc->timeIni;
    timeEnd = timeIni + SRV_PSNIFFER_GetMessageDuration(pDataSrc->frameType, srvPsnifferLastRxPayloadSymbols);
    *pData++ = (uint8_t)(timeIni >> 24);
    *pData++ = (uint8_t)(timeIni >> 16);
    *pData++ = (uint8_t)(timeIni >> 8);
    *pData++ = (uint8_t)timeIni;
    *pData++ = (uint8_t)(timeEnd >> 24);
    *pData++ = (uint8_t)(timeEnd >> 16);
    *pData++ = (uint8_t)(timeEnd >> 8);
    *pData++ = (uint8_t)timeEnd;
    *pData++ = 0;
    *pData++ = pDataSrc->rssiAvg;
    /* mac_enable not supported */
    *pData++ = 0;
    /* Length in bytes */
    *pData++ = (uint8_t)(pDataSrc->dataLength >> 8);
    *pData++ = (uint8_t)pDataSrc->dataLength;
    /* PDU */
    (void) memcpy(pData, pDataSrc->pReceivedData, pDataSrc->dataLength);
    pData += pDataSrc->dataLength;

    size = pData - pDataDst;
    return (size_t)size;
}

size_t SRV_PSNIFFER_SerialCfmMessage(uint8_t* pDataDst, DRV_PLC_PHY_TRANSMISSION_CFM_OBJ* pCfmObj)
{
    ptrdiff_t size;
    DRV_PLC_PHY_TRANSMISSION_OBJ* pTxObj;
    uint8_t* pData;
    uint32_t timeIni, timeEnd;
    
    if (pCfmObj->result != DRV_PLC_PHY_TX_RESULT_SUCCESS)
    {
        /* Error in transmission: No report */
        return 0;
    }

    pTxObj = &srvPsnifferLastTxObj[pCfmObj->bufferId];
    
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
    *pData++ = srvPsnifferLastTxPayloadSymbols;
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
    timeIni = pCfmObj->timeIni;
    timeEnd = timeIni + SRV_PSNIFFER_GetMessageDuration(pCfmObj->frameType, srvPsnifferLastTxPayloadSymbols);
    *pData++ = (uint8_t)(timeIni >> 24);
    *pData++ = (uint8_t)(timeIni >> 16);
    *pData++ = (uint8_t)(timeIni >> 8);
    *pData++ = (uint8_t)timeIni;
    *pData++ = (uint8_t)(timeEnd >> 24);
    *pData++ = (uint8_t)(timeEnd >> 16);
    *pData++ = (uint8_t)(timeEnd >> 8);
    *pData++ = (uint8_t)timeEnd;
    *pData++ = 0;
    *pData++ = 140;             /* fix RSSI */
    /* mac_enable not supported */
    *pData++ = 0;
    /* Length in bytes */
    *pData++ = (uint8_t)(pTxObj->dataLength >> 8);
    *pData++ = (uint8_t)pTxObj->dataLength;
    /* PDU */
    (void) memcpy(pData, pTxObj->pTransmitData, pTxObj->dataLength);
    pData += pTxObj->dataLength;

    size = pData - pDataDst;
    return (size_t)size;
}
