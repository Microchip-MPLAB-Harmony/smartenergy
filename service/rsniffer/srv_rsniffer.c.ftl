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
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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
#include <string.h>
#include "srv_rsniffer.h"
#include "configuration.h"
<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
#include "system/time/sys_time.h"
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Macro definitions
// *****************************************************************************
// *****************************************************************************

<#if SRV_RSNF_PROTOCOL == "G3">
/* G3 sniffer identifiers and version */
#define RSNIFFER_PHY_CMD_RECEIVE_MSG                  0x00
#define RSNIFFER_VERSION                              0x02
#define RSNIFFER_RF215_G3                             0x14
#define RSNIFFER_RF215_G3_EXTENDED                    0x34
#define RSNIFFER_RF215_G3_SIMULATOR                   0xD4

/* G3 sniffer modulation types (PLC modulation types + RF PHY types) */
#define RSNIFFER_MOD_TYPE_PLC_BPSK                    0x00
#define RSNIFFER_MOD_TYPE_PLC_QPSK                    0x01
#define RSNIFFER_MOD_TYPE_PLC_8PSK                    0x02
#define RSNIFFER_MOD_TYPE_PLC_16QAM                   0x03
#define RSNIFFER_MOD_TYPE_PLC_BPSK_ROBO               0x04
#define RSNIFFER_MOD_TYPE_RF_FSK50                    0x05
#define RSNIFFER_MOD_TYPE_RF_FSK100                   0x06
#define RSNIFFER_MOD_TYPE_RF_FSK150                   0x07
#define RSNIFFER_MOD_TYPE_RF_FSK200                   0x08
#define RSNIFFER_MOD_TYPE_RF_FSK300                   0x09
#define RSNIFFER_MOD_TYPE_RF_FSK400                   0x0A
#define RSNIFFER_MOD_TYPE_RF_4FSK50                   0x0B
#define RSNIFFER_MOD_TYPE_RF_4FSK100                  0x0C
#define RSNIFFER_MOD_TYPE_RF_4FSK150                  0x0D
#define RSNIFFER_MOD_TYPE_RF_4FSK200                  0x0E
#define RSNIFFER_MOD_TYPE_RF_4FSK300                  0x0F
#define RSNIFFER_MOD_TYPE_RF_4FSK400                  0x10
#define RSNIFFER_MOD_TYPE_RF_OFDM1                    0x11
#define RSNIFFER_MOD_TYPE_RF_OFDM2                    0x12
#define RSNIFFER_MOD_TYPE_RF_OFDM3                    0x13
#define RSNIFFER_MOD_TYPE_RF_OFDM4                    0x14

/* G3 sniffer modulation schemes (PLC modulation schemes + RF modulations) */
#define RSNIFFER_MOD_SCHEME_PLC_DIF                   0x00
#define RSNIFFER_MOD_SCHEME_PLC_COH                   0x01
#define RSNIFFER_MOD_SCHEME_RF_FSK_FEC_OFF            0x02
#define RSNIFFER_MOD_SCHEME_RF_FSK_FEC_ON             0x03
#define RSNIFFER_MOD_SCHEME_RF_OFDM_MCS0              0x04
#define RSNIFFER_MOD_SCHEME_RF_OFDM_MCS1              0x05
#define RSNIFFER_MOD_SCHEME_RF_OFDM_MCS2              0x06
#define RSNIFFER_MOD_SCHEME_RF_OFDM_MCS3              0x07
#define RSNIFFER_MOD_SCHEME_RF_OFDM_MCS4              0x08
#define RSNIFFER_MOD_SCHEME_RF_OFDM_MCS5              0x09
#define RSNIFFER_MOD_SCHEME_RF_OFDM_MCS6              0x0A

/* Buffer sizes */
#define RSNIFFER_MSG_HEADER_SIZE                      25
#define RSNIFFER_MSG_MAX_SIZE                         (DRV_RF215_MAX_PSDU_LEN + RSNIFFER_MSG_HEADER_SIZE)
<#else>
/* PRIME sniffer identifiers and version */
#define RSNIFFER_RF215_PRIME                          0x13
#define RSNIFFER_RF215_PRIME_EXTENDED                 0x33
#define RSNIFFER_RF215_PRIME_SIMULATOR                0xD3
#define RSNIFFER_VERSION                              0x14

/* PRIME sniffer message types */
#define RSNIFFER_PHY_MESSAGE_TYPE_A                   0x20
#define RSNIFFER_PHY_MESSAGE_TYPE_B                   0x21
#define RSNIFFER_PHY_MESSAGE_TYPE_BC                  0x22
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_FSK50            0x23
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_FSK100           0x24
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_FSK150           0x25
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_FSK200           0x26
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_FSK300           0x27
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_FSK400           0x28
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_4FSK50           0x29
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_4FSK100          0x2A
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_4FSK150          0x2B
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_4FSK200          0x2C
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_4FSK300          0x2D
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_4FSK400          0x2E
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_OFDM1            0x2F
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_OFDM2            0x30
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_OFDM3            0x31
#define RSNIFFER_PHY_MESSAGE_TYPE_RF_OFDM4            0x32

/* PRIME sniffer PHY modulations */
#define RSNIFFER_PHY_MESSAGE_MOD_DBPSK                0x00
#define RSNIFFER_PHY_MESSAGE_MOD_DQPSK                0x01
#define RSNIFFER_PHY_MESSAGE_MOD_D8PSK                0x02
#define RSNIFFER_PHY_MESSAGE_MOD_DBPSK_C              0x04
#define RSNIFFER_PHY_MESSAGE_MOD_DQPSK_C              0x05
#define RSNIFFER_PHY_MESSAGE_MOD_D8PSK_C              0x06
#define RSNIFFER_PHY_MESSAGE_MOD_R_DBPSK              0x0C
#define RSNIFFER_PHY_MESSAGE_MOD_R_DQPSK              0x0D
#define RSNIFFER_PHY_MESSAGE_MOD_R_DQPSK              0x0D
#define RSNIFFER_PHY_MESSAGE_MOD_RF_FSK_FEC_OFF       0x10
#define RSNIFFER_PHY_MESSAGE_MOD_RF_FSK_FEC_ON        0x11
#define RSNIFFER_PHY_MESSAGE_MOD_RF_OFDM_MCS0         0x12
#define RSNIFFER_PHY_MESSAGE_MOD_RF_OFDM_MCS1         0x13
#define RSNIFFER_PHY_MESSAGE_MOD_RF_OFDM_MCS2         0x14
#define RSNIFFER_PHY_MESSAGE_MOD_RF_OFDM_MCS3         0x15
#define RSNIFFER_PHY_MESSAGE_MOD_RF_OFDM_MCS4         0x16
#define RSNIFFER_PHY_MESSAGE_MOD_RF_OFDM_MCS5         0x17
#define RSNIFFER_PHY_MESSAGE_MOD_RF_OFDM_MCS6         0x18

/* Buffer sizes */
#define RSNIFFER_MSG_HEADER_SIZE                      32
#define RSNIFFER_MSG_MAX_SIZE                         (DRV_RF215_MAX_PSDU_LEN + RSNIFFER_MSG_HEADER_SIZE)
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data
// *****************************************************************************
// *****************************************************************************

<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
static uint64_t srvRsnifferPrevSysTime = 0;
static uint32_t srvRsnifferPrevTimeUS = 0;
</#if>
static uint8_t srvRsnifferRxMsg[RSNIFFER_MSG_MAX_SIZE];
static uint8_t srvRsnifferTxMsg[DRV_RF215_TX_BUFFERS_NUMBER][RSNIFFER_MSG_MAX_SIZE];

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************

<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    <#assign PHY_TYPE_INDENT = "        ">
<#else>
    <#assign PHY_TYPE_INDENT = "    ">
</#if>
<#if SRV_RSNF_PROTOCOL == "G3">
static uint8_t _SRV_RSNIFFER_ModType(DRV_RF215_PHY_CFG_OBJ* pPhyCfgObj)
{
    uint8_t modType;

<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    if (pPhyCfgObj->phyType == PHY_TYPE_FSK)
    {
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true>
${PHY_TYPE_INDENT}modType = (uint8_t) pPhyCfgObj->phyTypeCfg.fsk.symRate;
${PHY_TYPE_INDENT}if (pPhyCfgObj->phyTypeCfg.fsk.modOrd == FSK_MOD_ORD_2FSK)
${PHY_TYPE_INDENT}{
${PHY_TYPE_INDENT}    modType += RSNIFFER_MOD_TYPE_RF_FSK50;
${PHY_TYPE_INDENT}}
${PHY_TYPE_INDENT}else /* FSK_MOD_ORD_4FSK */
${PHY_TYPE_INDENT}{
${PHY_TYPE_INDENT}    modType += RSNIFFER_MOD_TYPE_RF_4FSK50;
${PHY_TYPE_INDENT}}
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
    else /* PHY_TYPE_OFDM */
    {
</#if>
<#if drvRf215.DRV_RF215_OFDM_EN == true>
${PHY_TYPE_INDENT}modType = (uint8_t) pPhyCfgObj->phyTypeCfg.ofdm.opt + RSNIFFER_MOD_TYPE_RF_OFDM1;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
</#if>

    return modType;
}
<#else>
static uint8_t _SRV_RSNIFFER_FrameType(DRV_RF215_PHY_CFG_OBJ* pPhyCfgObj)
{
    uint8_t frameType;

<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    if (pPhyCfgObj->phyType == PHY_TYPE_FSK)
    {
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true>
${PHY_TYPE_INDENT}frameType = (uint8_t) pPhyCfgObj->phyTypeCfg.fsk.symRate;
${PHY_TYPE_INDENT}if (pPhyCfgObj->phyTypeCfg.fsk.modOrd == FSK_MOD_ORD_2FSK)
${PHY_TYPE_INDENT}{
${PHY_TYPE_INDENT}    frameType += RSNIFFER_PHY_MESSAGE_TYPE_RF_FSK50;
${PHY_TYPE_INDENT}}
${PHY_TYPE_INDENT}else /* FSK_MOD_ORD_4FSK */
${PHY_TYPE_INDENT}{
${PHY_TYPE_INDENT}    frameType += RSNIFFER_PHY_MESSAGE_TYPE_RF_4FSK50;
${PHY_TYPE_INDENT}}
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
    else /* PHY_TYPE_OFDM */
    {
</#if>
<#if drvRf215.DRV_RF215_OFDM_EN == true>
${PHY_TYPE_INDENT}frameType = (uint8_t) pPhyCfgObj->phyTypeCfg.ofdm.opt + RSNIFFER_PHY_MESSAGE_TYPE_RF_OFDM1;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
</#if>

    return frameType;
}
</#if>

<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
static uint32_t _SRV_RSNIFFER_SysTimeToUS(uint64_t sysTime)
{
    uint64_t sysTimeDiff;
    uint32_t sysTimeDiffNumHigh, sysTimeDiffRemaining;
    uint32_t timeUS = srvRsnifferPrevTimeUS;

    /* Difference between current and previous system time */
    sysTimeDiff = sysTime - srvRsnifferPrevSysTime;
    sysTimeDiffNumHigh = (uint32_t) (sysTimeDiff / 0x10000000);
    sysTimeDiffRemaining = (uint32_t) (sysTimeDiff % 0x10000000);

    /* Convert system time to microseconds and add to previous time */
    timeUS += (SYS_TIME_CountToUS(0x10000000) * sysTimeDiffNumHigh);
    timeUS += SYS_TIME_CountToUS(sysTimeDiffRemaining);

    /* Store times for next computation */
    srvRsnifferPrevSysTime = sysTime;
    srvRsnifferPrevTimeUS = timeUS;

    return timeUS;
}

</#if>
// *****************************************************************************
// *****************************************************************************
// Section: RF PHY Sniffer Serialization Interface Implementation
// *****************************************************************************
// *****************************************************************************

uint8_t* SRV_RSNIFFER_SerialRxMessage (
    DRV_RF215_RX_INDICATION_OBJ* pIndObj,
    DRV_RF215_PHY_CFG_OBJ* pPhyCfgObj,
    uint16_t paySymbols,
<#if SRV_RSNF_PROTOCOL == "PRIME">
    uint16_t channel,
</#if>
    size_t* pMsgLen
)
{
    uint32_t timeIni, timeEnd;
    uint16_t psduLen;
    int16_t rssi;

<#if SRV_RSNF_PROTOCOL == "G3">
    /* Receive message command */
    srvRsnifferRxMsg[0] = RSNIFFER_PHY_CMD_RECEIVE_MSG;

    /* Sniffer version and sniffer type */
    srvRsnifferRxMsg[1] = RSNIFFER_VERSION;
    srvRsnifferRxMsg[2] = RSNIFFER_RF215_G3;

    /* Modulation scheme */
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    if (pPhyCfgObj->phyType == PHY_TYPE_FSK)
    {
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true>
${PHY_TYPE_INDENT}srvRsnifferRxMsg[3] = (uint8_t) pIndObj->modScheme + RSNIFFER_MOD_SCHEME_RF_FSK_FEC_OFF;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
    else /* PHY_TYPE_OFDM */
    {
</#if>
<#if drvRf215.DRV_RF215_OFDM_EN == true>
${PHY_TYPE_INDENT}srvRsnifferRxMsg[3] = (uint8_t) pIndObj->modScheme + RSNIFFER_MOD_SCHEME_RF_OFDM_MCS0;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
</#if>

    /* Modulation type depending on RF PHY configuration */
    srvRsnifferRxMsg[4] = _SRV_RSNIFFER_ModType(pPhyCfgObj);

    /* Correct FCS flag */
    srvRsnifferRxMsg[5] = (uint8_t) pIndObj->fcsOk;

    /* Number of payload symbols */
    srvRsnifferRxMsg[7] = (uint8_t) (paySymbols >> 8);
    srvRsnifferRxMsg[8] = (uint8_t) (paySymbols);

    /* Initial and end time of RX frame */
<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
    timeIni = _SRV_RSNIFFER_SysTimeToUS(pIndObj->timeIniCount);
<#else>
    timeIni = 0;
</#if>
    srvRsnifferRxMsg[11] = (uint8_t) (timeIni >> 24);
    srvRsnifferRxMsg[12] = (uint8_t) (timeIni >> 16);
    srvRsnifferRxMsg[13] = (uint8_t) (timeIni >> 8);
    srvRsnifferRxMsg[14] = (uint8_t) (timeIni);
    timeEnd = timeIni + SYS_TIME_CountToUS(pIndObj->ppduDurationCount);
    srvRsnifferRxMsg[15] = (uint8_t) (timeEnd >> 24);
    srvRsnifferRxMsg[16] = (uint8_t) (timeEnd >> 16);
    srvRsnifferRxMsg[17] = (uint8_t) (timeEnd >> 8);
    srvRsnifferRxMsg[18] = (uint8_t) (timeEnd);

    /* RSSI */
    rssi = (int16_t) pIndObj->rssiDBm;
    srvRsnifferRxMsg[19] = (uint8_t) (rssi >> 8);
    srvRsnifferRxMsg[20] = (uint8_t) (rssi);

    /* Data PSDU length (including G3-RF FCS) */
    psduLen = pIndObj->psduLen;
    srvRsnifferRxMsg[23] = (uint8_t) (psduLen >> 8);
    srvRsnifferRxMsg[24] = (uint8_t) (psduLen);
<#else>
    /* Frame type depending on RF PHY configuration */
    srvRsnifferRxMsg[0] = _SRV_RSNIFFER_FrameType(pPhyCfgObj);

    /* Sniffer version and sniffer type */
    srvRsnifferRxMsg[1] = RSNIFFER_VERSION;
    srvRsnifferRxMsg[2] = RSNIFFER_RF215_PRIME;

    /* Frame modulation */
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    if (pPhyCfgObj->phyType == PHY_TYPE_FSK)
    {
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true>
${PHY_TYPE_INDENT}srvRsnifferRxMsg[3] = (uint8_t) pIndObj->modScheme + RSNIFFER_PHY_MESSAGE_MOD_RF_FSK_FEC_OFF;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
    else /* PHY_TYPE_OFDM */
    {
</#if>
<#if drvRf215.DRV_RF215_OFDM_EN == true>
${PHY_TYPE_INDENT}srvRsnifferRxMsg[3] = (uint8_t) pIndObj->modScheme + RSNIFFER_PHY_MESSAGE_MOD_RF_OFDM_MCS0;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
</#if>

    /* Number of payload symbols */
    srvRsnifferRxMsg[4] = (uint8_t) (paySymbols >> 8);
    srvRsnifferRxMsg[5] = (uint8_t) (paySymbols);

    /* Channel */
    srvRsnifferRxMsg[6] = (uint8_t) (channel >> 8);
    srvRsnifferRxMsg[7] = (uint8_t) (channel);

    /* Initial and end time of RX frame */
<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
    timeIni = _SRV_RSNIFFER_SysTimeToUS(pIndObj->timeIniCount);
<#else>
    timeIni = 0;
</#if>
    srvRsnifferRxMsg[19] = (uint8_t) (timeIni >> 24);
    srvRsnifferRxMsg[20] = (uint8_t) (timeIni >> 16);
    srvRsnifferRxMsg[21] = (uint8_t) (timeIni >> 8);
    srvRsnifferRxMsg[22] = (uint8_t) (timeIni);
    timeEnd = timeIni + SYS_TIME_CountToUS(pIndObj->ppduDurationCount);
    srvRsnifferRxMsg[23] = (uint8_t) (timeEnd >> 24);
    srvRsnifferRxMsg[24] = (uint8_t) (timeEnd >> 16);
    srvRsnifferRxMsg[25] = (uint8_t) (timeEnd >> 8);
    srvRsnifferRxMsg[26] = (uint8_t) (timeEnd);

    /* RSSI */
    rssi = (int16_t) pIndObj->rssiDBm;
    srvRsnifferRxMsg[27] = (uint8_t) (rssi >> 8);
    srvRsnifferRxMsg[28] = (uint8_t) (rssi);

    /* mac_enable not supported */
    srvRsnifferRxMsg[29] = 0;

    /* Data PSDU length (including PRIME CRC) */
    psduLen = pIndObj->psduLen;
    srvRsnifferRxMsg[30] = (uint8_t) (psduLen >> 8);
    srvRsnifferRxMsg[31] = (uint8_t) (psduLen);
</#if>

    /* Copy PSDU */
    memcpy(srvRsnifferRxMsg + RSNIFFER_MSG_HEADER_SIZE, pIndObj->psdu, psduLen);

    *pMsgLen = (size_t) psduLen + RSNIFFER_MSG_HEADER_SIZE;
    return srvRsnifferRxMsg;
}

void SRV_RSNIFFER_SetTxMessage (
    DRV_RF215_TX_REQUEST_OBJ* pReqObj,
    DRV_RF215_TX_HANDLE txHandle
)
{
    uint8_t* pMsgDest;
    uint16_t psduLen;
    int16_t rssi;
    uint8_t txBufIndex;

    if (txHandle == DRV_RF215_TX_HANDLE_INVALID)
    {
        return;
    }

    /* Get buffer index from TX handle */
    txBufIndex = (uint8_t) (txHandle & 0xFF);
    pMsgDest = srvRsnifferTxMsg[txBufIndex];

<#if SRV_RSNF_PROTOCOL == "G3">
    /* Modulation scheme */
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    if (pPhyCfgObj->phyType == PHY_TYPE_FSK)
    {
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true>
${PHY_TYPE_INDENT}pMsgDest[3] = (uint8_t) pReqObj->modScheme + RSNIFFER_MOD_SCHEME_RF_FSK_FEC_OFF;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
    else /* PHY_TYPE_OFDM */
    {
</#if>
<#if drvRf215.DRV_RF215_OFDM_EN == true>
${PHY_TYPE_INDENT}pMsgDest[3] = (uint8_t) pReqObj->modScheme + RSNIFFER_MOD_SCHEME_RF_OFDM_MCS0;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
</#if>

    /* RSSI */
    rssi = 14 - (int16_t) pReqObj->txPwrAtt;
    pMsgDest[19] = (uint8_t) (rssi >> 8);
    pMsgDest[20] = (uint8_t) (rssi);

    /* Data PSDU length (including G3-RF FCS) */
    psduLen = pReqObj->psduLen;
    pMsgDest[23] = (uint8_t) (psduLen >> 8);
    pMsgDest[24] = (uint8_t) (psduLen);
<#else>
    /* Frame modulation */
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    if (pPhyCfgObj->phyType == PHY_TYPE_FSK)
    {
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true>
${PHY_TYPE_INDENT}pMsgDest[3] = (uint8_t) pReqObj->modScheme + RSNIFFER_PHY_MESSAGE_MOD_RF_FSK_FEC_OFF;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
    else /* PHY_TYPE_OFDM */
    {
</#if>
<#if drvRf215.DRV_RF215_OFDM_EN == true>
${PHY_TYPE_INDENT}pMsgDest[3] = (uint8_t) pReqObj->modScheme + RSNIFFER_PHY_MESSAGE_MOD_RF_OFDM_MCS0;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
</#if>

<#if drvRf215.DRV_RF215_FREQ_HOPPING_SUPPORT == true>
    /* Channel */
    pMsgDest[6] = (uint8_t) (pReqObj->channelNum >> 8);
    pMsgDest[7] = (uint8_t) (pReqObj->channelNum);

</#if>
    /* RSSI */
    rssi = 14 - (int16_t) pReqObj->txPwrAtt;
    pMsgDest[27] = (uint8_t) (rssi >> 8);
    pMsgDest[28] = (uint8_t) (rssi);

    /* mac_enable not supported */
    pMsgDest[29] = 0;

    /* Data PSDU length (including PRIME CRC) */
    psduLen = pReqObj->psduLen;
    pMsgDest[30] = (uint8_t) (psduLen >> 8);
    pMsgDest[31] = (uint8_t) (psduLen);
</#if>

<#if drvRf215.DRV_RF215_FCS_MODE != "0">
    /* Copy PHY data message (without FCS) */
    memcpy(pMsgDest + RSNIFFER_MSG_HEADER_SIZE, pReqObj->psdu, psduLen - DRV_RF215_FCS_LEN);

    /* FCS set to 0 (automatically computed) */
    memset(pMsgDest + RSNIFFER_MSG_HEADER_SIZE + psduLen, 0, DRV_RF215_FCS_LEN);
<#else>
    /* Copy PHY data message */
    memcpy(pMsgDest + RSNIFFER_MSG_HEADER_SIZE, pReqObj->psdu, psduLen);
</#if>
}

uint8_t* SRV_RSNIFFER_SerialCfmMessage (
    DRV_RF215_TX_CONFIRM_OBJ* pCfmObj,
    DRV_RF215_TX_HANDLE txHandle,
    DRV_RF215_PHY_CFG_OBJ* pPhyCfgObj,
    uint16_t paySymbols,
<#if SRV_RSNF_PROTOCOL == "PRIME">
    uint16_t channel,
</#if>
    size_t* pMsgLen
)
{
    uint8_t* pMsgDest;
    uint32_t timeIni, timeEnd;
    uint16_t psduLen;
    uint8_t txBufIndex;

    if (pCfmObj->txResult != RF215_TX_SUCCESS)
    {
        /* Error in transmission: No report */
        *pMsgLen = 0;
        return NULL;
    }

    /* Get buffer index from TX handle */
    txBufIndex = (uint8_t) (txHandle & 0xFF);
    pMsgDest = srvRsnifferTxMsg[txBufIndex];

<#if SRV_RSNF_PROTOCOL == "G3">
    /* Receive message command */
    pMsgDest[0] = RSNIFFER_PHY_CMD_RECEIVE_MSG;

    /* Sniffer version and sniffer type */
    pMsgDest[1] = RSNIFFER_VERSION;
    pMsgDest[2] = RSNIFFER_RF215_G3;

    /* Modulation type depending on RF PHY configuration */
    pMsgDest[4] = _SRV_RSNIFFER_ModType(pPhyCfgObj);

    /* Correct FCS flag */
    pMsgDest[5] = (uint8_t) true;

    /* Number of payload symbols */
    pMsgDest[7] = (uint8_t) (paySymbols >> 8);
    pMsgDest[8] = (uint8_t) (paySymbols);

    /* Initial and end time of RX frame */
<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
    timeIni = _SRV_RSNIFFER_SysTimeToUS(pCfmObj->timeIniCount);
<#else>
    timeIni = 0;
</#if>
    pMsgDest[11] = (uint8_t) (timeIni >> 24);
    pMsgDest[12] = (uint8_t) (timeIni >> 16);
    pMsgDest[13] = (uint8_t) (timeIni >> 8);
    pMsgDest[14] = (uint8_t) (timeIni);
    timeEnd = timeIni + SYS_TIME_CountToUS(pCfmObj->ppduDurationCount);
    pMsgDest[15] = (uint8_t) (timeEnd >> 24);
    pMsgDest[16] = (uint8_t) (timeEnd >> 16);
    pMsgDest[17] = (uint8_t) (timeEnd >> 8);
    pMsgDest[18] = (uint8_t) (timeEnd);

    psduLen = (uint16_t) (pMsgDest[23] << 8) + pMsgDest[24];
<#else>
    /* Frame type depending on RF PHY configuration */
    pMsgDest[0] = _SRV_RSNIFFER_FrameType(pPhyCfgObj);

    /* Sniffer version and sniffer type */
    pMsgDest[1] = RSNIFFER_VERSION;
    pMsgDest[2] = RSNIFFER_RF215_PRIME;

    /* Number of payload symbols */
    pMsgDest[4] = (uint8_t) (paySymbols >> 8);
    pMsgDest[5] = (uint8_t) (paySymbols);

<#if drvRf215.DRV_RF215_FREQ_HOPPING_SUPPORT == false>
    /* Channel */
    pMsgDest[6] = (uint8_t) (channel >> 8);
    pMsgDest[7] = (uint8_t) (channel);

</#if>
    /* Initial and end time of RX frame */
<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
    timeIni = _SRV_RSNIFFER_SysTimeToUS(pCfmObj->timeIniCount);
<#else>
    timeIni = 0;
</#if>
    pMsgDest[19] = (uint8_t) (timeIni >> 24);
    pMsgDest[20] = (uint8_t) (timeIni >> 16);
    pMsgDest[21] = (uint8_t) (timeIni >> 8);
    pMsgDest[22] = (uint8_t) (timeIni);
    timeEnd = timeIni + SYS_TIME_CountToUS(pCfmObj->ppduDurationCount);
    pMsgDest[23] = (uint8_t) (timeEnd >> 24);
    pMsgDest[24] = (uint8_t) (timeEnd >> 16);
    pMsgDest[25] = (uint8_t) (timeEnd >> 8);
    pMsgDest[26] = (uint8_t) (timeEnd);

    psduLen = (uint16_t) (pMsgDest[30] << 8) + pMsgDest[31];
</#if>
    *pMsgLen = (size_t) psduLen + RSNIFFER_MSG_HEADER_SIZE;
    return pMsgDest;
}
