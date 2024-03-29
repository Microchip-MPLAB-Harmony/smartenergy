/******************************************************************************
  RF PHY Sniffer Serialization Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    srv_rsniffer.c

  Summary:
    Source code for the RF PHY sniffer serialization implementation.

  Description:
    The RF PHY sniffer serialization provides a service to format messages
    through serial connection in order to communicate with Hybrid Sniffer Tool
    provided by Microchip. This file contains the source code for the
    implementation of the RF PHY sniffer serialization.

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
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "srv_rsniffer.h"
#include "configuration.h"
#include "system/time/sys_time.h"
#include "stack_config.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macro definitions
// *****************************************************************************
// *****************************************************************************

/* G3 sniffer identifiers and version */
#define RSNIFFER_PHY_CMD_RECEIVE_MSG                  0x00U
#define RSNIFFER_VERSION                              0x02U
#define RSNIFFER_RF215_G3                             0x14U

/* G3 sniffer modulation types (PLC modulation types + RF PHY types) */
#define RSNIFFER_MOD_TYPE_RF_OQPSK                    0x80U

/* G3 sniffer modulation schemes (PLC modulation schemes + RF modulations) */
#define RSNIFFER_MOD_SCHEME_RF_OQPSK                  0x80U

/* Buffer sizes */
#define RSNIFFER_MSG_HEADER_SIZE                      25U
#define RSNIFFER_MSG_MAX_SIZE                         (LARGE_BUFFER_SIZE + RSNIFFER_MSG_HEADER_SIZE)

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data
// *****************************************************************************
// *****************************************************************************

static uint64_t srvRsnifferPrevSysTime = 0;
static uint32_t srvRsnifferPrevTimeUS = 0;
static uint8_t srvRsnifferRxMsg[RSNIFFER_MSG_MAX_SIZE];

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************
static uint32_t lSRV_RSNIFFER_SysTimeToUS(uint64_t sysTime)
{
    uint64_t sysTimeDiff;
    uint32_t sysTimeDiffNumHigh, sysTimeDiffRemaining;
    uint32_t timeUS = srvRsnifferPrevTimeUS;

    /* Difference between current and previous system time */
    sysTimeDiff = sysTime - srvRsnifferPrevSysTime;
    sysTimeDiffNumHigh = (uint32_t) (sysTimeDiff / 0x10000000UL);
    sysTimeDiffRemaining = (uint32_t) (sysTimeDiff % 0x10000000UL);

    /* Convert system time to microseconds and add to previous time */
    timeUS += (SYS_TIME_CountToUS(0x10000000UL) * sysTimeDiffNumHigh);
    timeUS += SYS_TIME_CountToUS(sysTimeDiffRemaining);

    /* Store times for next computation */
    srvRsnifferPrevSysTime = sysTime;
    srvRsnifferPrevTimeUS = timeUS;

    return timeUS;
}

static uint8_t* lSRV_RSNIFFER_SerialMessage (SRV_RSNIFFER_PHY_DATA *pData, size_t *pMsgLen)
{
    uint32_t timeIni, timeEnd;
    int16_t rssi;

    /* Receive message command */
    srvRsnifferRxMsg[0] = RSNIFFER_PHY_CMD_RECEIVE_MSG;

    /* Sniffer version and sniffer type */
    srvRsnifferRxMsg[1] = RSNIFFER_VERSION;
    srvRsnifferRxMsg[2] = RSNIFFER_RF215_G3;

    /* Modulation scheme */
    srvRsnifferRxMsg[3] = RSNIFFER_MOD_SCHEME_RF_OQPSK;

    /* Modulation type depending on RF PHY configuration */
    srvRsnifferRxMsg[4] = RSNIFFER_MOD_TYPE_RF_OQPSK;

    /* Correct FCS flag */
    srvRsnifferRxMsg[5] = (uint8_t) true;

    /* Number of payload symbols */
    srvRsnifferRxMsg[7] = (uint8_t)(pData->paySymbols >> 8);
    srvRsnifferRxMsg[8] = (uint8_t)pData->paySymbols;

    /* Initial and end time of RX frame */
    timeIni = lSRV_RSNIFFER_SysTimeToUS(pData->timeIniCount);
    srvRsnifferRxMsg[11] = (uint8_t)(timeIni >> 24);
    srvRsnifferRxMsg[12] = (uint8_t)(timeIni >> 16);
    srvRsnifferRxMsg[13] = (uint8_t)(timeIni >> 8);
    srvRsnifferRxMsg[14] = (uint8_t)timeIni;
    timeEnd = timeIni + SYS_TIME_CountToUS(pData->durationCount);
    srvRsnifferRxMsg[15] = (uint8_t)(timeEnd >> 24);
    srvRsnifferRxMsg[16] = (uint8_t)(timeEnd >> 16);
    srvRsnifferRxMsg[17] = (uint8_t)(timeEnd >> 8);
    srvRsnifferRxMsg[18] = (uint8_t)timeEnd;

    /* RSSI */
    rssi = (int16_t)pData->rssi;
    srvRsnifferRxMsg[19] = (uint8_t)((uint16_t)rssi >> 8);
    srvRsnifferRxMsg[20] = (uint8_t)rssi;

    /* Data payload length */
    srvRsnifferRxMsg[23] = (uint8_t)(pData->payloadLen >> 8);
    srvRsnifferRxMsg[24] = (uint8_t)pData->payloadLen;

    /* Copy payload */
    (void)memcpy(srvRsnifferRxMsg + RSNIFFER_MSG_HEADER_SIZE, pData->pData, pData->payloadLen);

    *pMsgLen = (size_t)pData->payloadLen + RSNIFFER_MSG_HEADER_SIZE;
    return srvRsnifferRxMsg;
}

// *****************************************************************************
// *****************************************************************************
// Section: RF PHY Sniffer Serialization Interface Implementation
// *****************************************************************************
// *****************************************************************************

uint8_t* SRV_RSNIFFER_SerialRxMessage ( SRV_RSNIFFER_PHY_DATA *rxData, size_t *pMsgLen )
{
    return lSRV_RSNIFFER_SerialMessage(rxData, pMsgLen);
}

uint8_t* SRV_RSNIFFER_SerialCfmMessage ( SRV_RSNIFFER_PHY_DATA *txData, size_t *pMsgLen )
{
    return lSRV_RSNIFFER_SerialMessage(txData, pMsgLen);
}
