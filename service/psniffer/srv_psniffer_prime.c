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
/*
Copyright (C) 2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

static uint32_t lSRV_PSNIFFER_GetMessageDuration(DRV_PLC_PHY_FRAME_TYPE frameType, uint8_t symbols)
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

// *****************************************************************************
// *****************************************************************************
// Section: SRV_PSNIFFER Interface Routines
// *****************************************************************************
// *****************************************************************************

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
    if ((channel > 0U) && (channel < 15U))
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
    timeEnd = timeIni + lSRV_PSNIFFER_GetMessageDuration(pDataSrc->frameType, srvPsnifferLastRxPayloadSymbols);
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
    timeEnd = timeIni + lSRV_PSNIFFER_GetMessageDuration(pCfmObj->frameType, srvPsnifferLastTxPayloadSymbols);
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
