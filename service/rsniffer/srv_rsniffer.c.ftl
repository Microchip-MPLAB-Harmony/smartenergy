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
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
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
#define RSNIFFER_PHY_CMD_RECEIVE_MSG                  0x00U
#define RSNIFFER_VERSION                              0x02U
#define RSNIFFER_RF215_G3                             0x14U
#define RSNIFFER_RF215_G3_EXTENDED                    0x34U
#define RSNIFFER_RF215_G3_SIMULATOR                   0xD4U

/* G3 sniffer modulation types (PLC modulation types + RF PHY types) */
#define RSNIFFER_MODTYPE_PLC_BPSK                     0x00U
#define RSNIFFER_MODTYPE_PLC_QPSK                     0x01U
#define RSNIFFER_MODTYPE_PLC_8PSK                     0x02U
#define RSNIFFER_MODTYPE_PLC_16QAM                    0x03U
#define RSNIFFER_MODTYPE_PLC_BPSK_ROBO                0x04U
#define RSNIFFER_MODTYPE_RF_FSK50                     0x05U
#define RSNIFFER_MODTYPE_RF_FSK100                    0x06U
#define RSNIFFER_MODTYPE_RF_FSK150                    0x07U
#define RSNIFFER_MODTYPE_RF_FSK200                    0x08U
#define RSNIFFER_MODTYPE_RF_FSK300                    0x09U
#define RSNIFFER_MODTYPE_RF_FSK400                    0x0AU
#define RSNIFFER_MODTYPE_RF_4FSK50                    0x0BU
#define RSNIFFER_MODTYPE_RF_4FSK100                   0x0CU
#define RSNIFFER_MODTYPE_RF_4FSK150                   0x0DU
#define RSNIFFER_MODTYPE_RF_4FSK200                   0x0EU
#define RSNIFFER_MODTYPE_RF_4FSK300                   0x0FU
#define RSNIFFER_MODTYPE_RF_4FSK400                   0x10U
#define RSNIFFER_MODTYPE_RF_OFDM1                     0x11U
#define RSNIFFER_MODTYPE_RF_OFDM2                     0x12U
#define RSNIFFER_MODTYPE_RF_OFDM3                     0x13U
#define RSNIFFER_MODTYPE_RF_OFDM4                     0x14U

/* G3 sniffer modulation schemes (PLC modulation schemes + RF modulations) */
#define RSNIFFER_MODSCHEME_PLC_DIF                    0x00U
#define RSNIFFER_MODSCHEME_PLC_COH                    0x01U
#define RSNIFFER_MODSCHEME_RF_FSK_FECOFF              0x02U
#define RSNIFFER_MODSCHEME_RF_FSK_FECON               0x03U
#define RSNIFFER_MODSCHEME_RF_OFDM_MCS0               0x04U
#define RSNIFFER_MODSCHEME_RF_OFDM_MCS1               0x05U
#define RSNIFFER_MODSCHEME_RF_OFDM_MCS2               0x06U
#define RSNIFFER_MODSCHEME_RF_OFDM_MCS3               0x07U
#define RSNIFFER_MODSCHEME_RF_OFDM_MCS4               0x08U
#define RSNIFFER_MODSCHEME_RF_OFDM_MCS5               0x09U
#define RSNIFFER_MODSCHEME_RF_OFDM_MCS6               0x0AU

/* Buffer sizes */
#define RSNIFFER_MSG_HEADER_SIZE                      25U
#define RSNIFFER_MSG_MAX_SIZE                         (DRV_RF215_MAX_PSDU_LEN + RSNIFFER_MSG_HEADER_SIZE)
<#else>
/* PRIME sniffer identifiers and version */
#define RSNIFFER_RF215_PRIME                          0x13U
#define RSNIFFER_RF215_PRIME_EXTENDED                 0x33U
#define RSNIFFER_RF215_PRIME_SIMULATOR                0xD3U
#define RSNIFFER_VERSION                              0x14U

/* PRIME sniffer message types */
#define RSNIFFER_TYPE_A                               0x20U
#define RSNIFFER_TYPE_B                               0x21U
#define RSNIFFER_TYPE_BC                              0x22U
#define RSNIFFER_TYPE_RF_FSK50                        0x23U
#define RSNIFFER_TYPE_RF_FSK100                       0x24U
#define RSNIFFER_TYPE_RF_FSK150                       0x25U
#define RSNIFFER_TYPE_RF_FSK200                       0x26U
#define RSNIFFER_TYPE_RF_FSK300                       0x27U
#define RSNIFFER_TYPE_RF_FSK400                       0x28U
#define RSNIFFER_TYPE_RF_4FSK50                       0x29U
#define RSNIFFER_TYPE_RF_4FSK100                      0x2AU
#define RSNIFFER_TYPE_RF_4FSK150                      0x2BU
#define RSNIFFER_TYPE_RF_4FSK200                      0x2CU
#define RSNIFFER_TYPE_RF_4FSK300                      0x2DU
#define RSNIFFER_TYPE_RF_4FSK400                      0x2EU
#define RSNIFFER_TYPE_RF_OFDM1                        0x2FU
#define RSNIFFER_TYPE_RF_OFDM2                        0x30U
#define RSNIFFER_TYPE_RF_OFDM3                        0x31U
#define RSNIFFER_TYPE_RF_OFDM4                        0x32U

/* PRIME sniffer PHY modulations */
#define RSNIFFER_MOD_DBPSK                            0x00U
#define RSNIFFER_MOD_DQPSK                            0x01U
#define RSNIFFER_MOD_D8PSK                            0x02U
#define RSNIFFER_MOD_DBPSK_C                          0x04U
#define RSNIFFER_MOD_DQPSK_C                          0x05U
#define RSNIFFER_MOD_D8PSK_C                          0x06U
#define RSNIFFER_MOD_R_DBPSK                          0x0CU
#define RSNIFFER_MOD_R_DQPSK                          0x0DU
#define RSNIFFER_MOD_R_DQPSK                          0x0DU
#define RSNIFFER_MOD_RF_FSK_FEC_OFF                   0x10U
#define RSNIFFER_MOD_RF_FSK_FEC_ON                    0x11U
#define RSNIFFER_MOD_RF_OFDM_MCS0                     0x12U
#define RSNIFFER_MOD_RF_OFDM_MCS1                     0x13U
#define RSNIFFER_MOD_RF_OFDM_MCS2                     0x14U
#define RSNIFFER_MOD_RF_OFDM_MCS3                     0x15U
#define RSNIFFER_MOD_RF_OFDM_MCS4                     0x16U
#define RSNIFFER_MOD_RF_OFDM_MCS5                     0x17U
#define RSNIFFER_MOD_RF_OFDM_MCS6                     0x18U

/* Buffer sizes */
#define RSNIFFER_MSG_HEADER_SIZE                      32U
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
static uint8_t lSRV_RSNIFFER_ModType(DRV_RF215_PHY_CFG_OBJ* pPhyCfgObj)
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
${PHY_TYPE_INDENT}    modType += RSNIFFER_MODTYPE_RF_FSK50;
${PHY_TYPE_INDENT}}
${PHY_TYPE_INDENT}else /* FSK_MOD_ORD_4FSK */
${PHY_TYPE_INDENT}{
${PHY_TYPE_INDENT}    modType += RSNIFFER_MODTYPE_RF_4FSK50;
${PHY_TYPE_INDENT}}
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
    else /* PHY_TYPE_OFDM */
    {
</#if>
<#if drvRf215.DRV_RF215_OFDM_EN == true>
${PHY_TYPE_INDENT}modType = (uint8_t) pPhyCfgObj->phyTypeCfg.ofdm.opt + RSNIFFER_MODTYPE_RF_OFDM1;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
</#if>

    return modType;
}
<#else>
static uint8_t SRV_RSNIFFER_FrameType(DRV_RF215_PHY_CFG_OBJ* pPhyCfgObj)
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
${PHY_TYPE_INDENT}    frameType += RSNIFFER_TYPE_RF_FSK50;
${PHY_TYPE_INDENT}}
${PHY_TYPE_INDENT}else /* FSK_MOD_ORD_4FSK */
${PHY_TYPE_INDENT}{
${PHY_TYPE_INDENT}    frameType += RSNIFFER_TYPE_RF_4FSK50;
${PHY_TYPE_INDENT}}
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
    else /* PHY_TYPE_OFDM */
    {
</#if>
<#if drvRf215.DRV_RF215_OFDM_EN == true>
${PHY_TYPE_INDENT}frameType = (uint8_t) pPhyCfgObj->phyTypeCfg.ofdm.opt + RSNIFFER_TYPE_RF_OFDM1;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
</#if>

    return frameType;
}
</#if>

<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
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

</#if>
// *****************************************************************************
// *****************************************************************************
// Section: RF PHY Sniffer Serialization Interface Implementation
// *****************************************************************************
// *****************************************************************************

SRV_RSNIFFER_COMMAND SRV_RSNIFFER_GetCommand(uint8_t* pDataSrc)
{
    /* Extract Command */
    return (SRV_RSNIFFER_COMMAND)*pDataSrc;
}

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
${PHY_TYPE_INDENT}srvRsnifferRxMsg[3] = (uint8_t) pIndObj->modScheme + RSNIFFER_MODSCHEME_RF_FSK_FECOFF;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
    else /* PHY_TYPE_OFDM */
    {
</#if>
<#if drvRf215.DRV_RF215_OFDM_EN == true>
${PHY_TYPE_INDENT}srvRsnifferRxMsg[3] = (uint8_t) pIndObj->modScheme + RSNIFFER_MODSCHEME_RF_OFDM_MCS0;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
</#if>

    /* Modulation type depending on RF PHY configuration */
    srvRsnifferRxMsg[4] = lSRV_RSNIFFER_ModType(pPhyCfgObj);

    /* Correct FCS flag */
    srvRsnifferRxMsg[5] = (uint8_t) pIndObj->fcsOk;

    /* Number of payload symbols */
    srvRsnifferRxMsg[7] = (uint8_t) (paySymbols >> 8);
    srvRsnifferRxMsg[8] = (uint8_t) paySymbols;

    /* Initial and end time of RX frame */
<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
    timeIni = lSRV_RSNIFFER_SysTimeToUS(pIndObj->timeIniCount);
<#else>
    timeIni = 0;
</#if>
    srvRsnifferRxMsg[11] = (uint8_t) (timeIni >> 24);
    srvRsnifferRxMsg[12] = (uint8_t) (timeIni >> 16);
    srvRsnifferRxMsg[13] = (uint8_t) (timeIni >> 8);
    srvRsnifferRxMsg[14] = (uint8_t) timeIni;
    timeEnd = timeIni + SYS_TIME_CountToUS(pIndObj->ppduDurationCount);
    srvRsnifferRxMsg[15] = (uint8_t) (timeEnd >> 24);
    srvRsnifferRxMsg[16] = (uint8_t) (timeEnd >> 16);
    srvRsnifferRxMsg[17] = (uint8_t) (timeEnd >> 8);
    srvRsnifferRxMsg[18] = (uint8_t) timeEnd;

    /* RSSI */
    rssi = (int16_t) pIndObj->rssiDBm;
    srvRsnifferRxMsg[19] = (uint8_t) ((uint16_t) rssi >> 8);
    srvRsnifferRxMsg[20] = (uint8_t) (rssi);

    /* Data PSDU length (including G3-RF FCS) */
    psduLen = pIndObj->psduLen;
    srvRsnifferRxMsg[23] = (uint8_t) (psduLen >> 8);
    srvRsnifferRxMsg[24] = (uint8_t) psduLen;
<#else>
    /* Frame type depending on RF PHY configuration */
    srvRsnifferRxMsg[0] = SRV_RSNIFFER_FrameType(pPhyCfgObj);

    /* Sniffer version and sniffer type */
    srvRsnifferRxMsg[1] = RSNIFFER_VERSION;
    srvRsnifferRxMsg[2] = RSNIFFER_RF215_PRIME;

    /* Frame modulation */
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    if (pPhyCfgObj->phyType == PHY_TYPE_FSK)
    {
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true>
${PHY_TYPE_INDENT}srvRsnifferRxMsg[3] = (uint8_t) pIndObj->modScheme + RSNIFFER_MOD_RF_FSK_FEC_OFF;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
    else /* PHY_TYPE_OFDM */
    {
</#if>
<#if drvRf215.DRV_RF215_OFDM_EN == true>
${PHY_TYPE_INDENT}srvRsnifferRxMsg[3] = (uint8_t) pIndObj->modScheme + RSNIFFER_MOD_RF_OFDM_MCS0;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
</#if>

    /* Number of payload symbols */
    srvRsnifferRxMsg[4] = (uint8_t) (paySymbols >> 8);
    srvRsnifferRxMsg[5] = (uint8_t) paySymbols;

    /* Channel */
    srvRsnifferRxMsg[6] = (uint8_t) (channel >> 8);
    srvRsnifferRxMsg[7] = (uint8_t) channel;

    /* Initial and end time of RX frame */
<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
    timeIni = lSRV_RSNIFFER_SysTimeToUS(pIndObj->timeIniCount);
<#else>
    timeIni = 0;
</#if>
    srvRsnifferRxMsg[19] = (uint8_t) (timeIni >> 24);
    srvRsnifferRxMsg[20] = (uint8_t) (timeIni >> 16);
    srvRsnifferRxMsg[21] = (uint8_t) (timeIni >> 8);
    srvRsnifferRxMsg[22] = (uint8_t) timeIni;
    timeEnd = timeIni + SYS_TIME_CountToUS(pIndObj->ppduDurationCount);
    srvRsnifferRxMsg[23] = (uint8_t) (timeEnd >> 24);
    srvRsnifferRxMsg[24] = (uint8_t) (timeEnd >> 16);
    srvRsnifferRxMsg[25] = (uint8_t) (timeEnd >> 8);
    srvRsnifferRxMsg[26] = (uint8_t) timeEnd;

    /* RSSI */
    rssi = (int16_t) pIndObj->rssiDBm;
    srvRsnifferRxMsg[27] = (uint8_t) ((uint16_t) rssi >> 8);
    srvRsnifferRxMsg[28] = (uint8_t) (rssi);

    /* mac_enable not supported */
    srvRsnifferRxMsg[29] = 0;

    /* Data PSDU length (including PRIME CRC) */
    psduLen = pIndObj->psduLen;
    srvRsnifferRxMsg[30] = (uint8_t) (psduLen >> 8);
    srvRsnifferRxMsg[31] = (uint8_t) psduLen;
</#if>

    /* Copy PSDU */
    (void) memcpy(srvRsnifferRxMsg + RSNIFFER_MSG_HEADER_SIZE, pIndObj->psdu, psduLen);

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
    txBufIndex = (uint8_t) txHandle;
    pMsgDest = srvRsnifferTxMsg[txBufIndex];

<#if SRV_RSNF_PROTOCOL == "G3">
    /* Modulation scheme */
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    if (pPhyCfgObj->phyType == PHY_TYPE_FSK)
    {
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true>
${PHY_TYPE_INDENT}pMsgDest[3] = (uint8_t) pReqObj->modScheme + RSNIFFER_MODSCHEME_RF_FSK_FECOFF;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
    else /* PHY_TYPE_OFDM */
    {
</#if>
<#if drvRf215.DRV_RF215_OFDM_EN == true>
${PHY_TYPE_INDENT}pMsgDest[3] = (uint8_t) pReqObj->modScheme + RSNIFFER_MODSCHEME_RF_OFDM_MCS0;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
</#if>

    /* RSSI */
    rssi = 14 - (int16_t) pReqObj->txPwrAtt;
    pMsgDest[19] = (uint8_t) ((uint16_t) rssi >> 8);
    pMsgDest[20] = (uint8_t) rssi;

    /* Data PSDU length (including G3-RF FCS) */
    psduLen = pReqObj->psduLen;
    pMsgDest[23] = (uint8_t) (psduLen >> 8);
    pMsgDest[24] = (uint8_t) psduLen;
<#else>
    /* Frame modulation */
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    if (pPhyCfgObj->phyType == PHY_TYPE_FSK)
    {
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true>
${PHY_TYPE_INDENT}pMsgDest[3] = (uint8_t) pReqObj->modScheme + RSNIFFER_MOD_RF_FSK_FEC_OFF;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
    else /* PHY_TYPE_OFDM */
    {
</#if>
<#if drvRf215.DRV_RF215_OFDM_EN == true>
${PHY_TYPE_INDENT}pMsgDest[3] = (uint8_t) pReqObj->modScheme + RSNIFFER_MOD_RF_OFDM_MCS0;
</#if>
<#if drvRf215.DRV_RF215_FSK_EN == true && drvRf215.DRV_RF215_OFDM_EN == true>
    }
</#if>

<#if drvRf215.DRV_RF215_FREQ_HOPPING_SUPPORT == true>
    /* Channel */
    pMsgDest[6] = (uint8_t) (pReqObj->channelNum >> 8);
    pMsgDest[7] = (uint8_t) pReqObj->channelNum;

</#if>
    /* RSSI */
    rssi = 14 - (int16_t) pReqObj->txPwrAtt;
    pMsgDest[27] = (uint8_t) ((uint16_t) rssi >> 8);
    pMsgDest[28] = (uint8_t) rssi;

    /* mac_enable not supported */
    pMsgDest[29] = 0;

    /* Data PSDU length (including PRIME CRC) */
    psduLen = pReqObj->psduLen;
    pMsgDest[30] = (uint8_t) (psduLen >> 8);
    pMsgDest[31] = (uint8_t) psduLen;
</#if>

<#if drvRf215.DRV_RF215_FCS_MODE != "0">
    /* Copy PHY data message (without FCS) */
    (void) memcpy(pMsgDest + RSNIFFER_MSG_HEADER_SIZE, pReqObj->psdu, (size_t) psduLen - DRV_RF215_FCS_LEN);

    /* FCS set to 0 (automatically computed) */
    (void) memset(pMsgDest + RSNIFFER_MSG_HEADER_SIZE + psduLen, 0, DRV_RF215_FCS_LEN);
<#else>
    /* Copy PHY data message */
    (void) memcpy(pMsgDest + RSNIFFER_MSG_HEADER_SIZE, pReqObj->psdu, psduLen);
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
    txBufIndex = (uint8_t) txHandle;
    pMsgDest = srvRsnifferTxMsg[txBufIndex];

<#if SRV_RSNF_PROTOCOL == "G3">
    /* Receive message command */
    pMsgDest[0] = RSNIFFER_PHY_CMD_RECEIVE_MSG;

    /* Sniffer version and sniffer type */
    pMsgDest[1] = RSNIFFER_VERSION;
    pMsgDest[2] = RSNIFFER_RF215_G3;

    /* Modulation type depending on RF PHY configuration */
    pMsgDest[4] = lSRV_RSNIFFER_ModType(pPhyCfgObj);

    /* Correct FCS flag */
    pMsgDest[5] = (uint8_t) true;

    /* Number of payload symbols */
    pMsgDest[7] = (uint8_t) (paySymbols >> 8);
    pMsgDest[8] = (uint8_t) (paySymbols);

    /* Initial and end time of RX frame */
<#if drvRf215.DRV_RF215_TXRX_TIME_SUPPORT == true>
    timeIni = lSRV_RSNIFFER_SysTimeToUS(pCfmObj->timeIniCount);
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

    psduLen = ((uint16_t) pMsgDest[23] << 8) + pMsgDest[24];
<#else>
    /* Frame type depending on RF PHY configuration */
    pMsgDest[0] = SRV_RSNIFFER_FrameType(pPhyCfgObj);

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
    timeIni = lSRV_RSNIFFER_SysTimeToUS(pCfmObj->timeIniCount);
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

    psduLen = ((uint16_t) pMsgDest[30] << 8) + pMsgDest[31];
</#if>
    *pMsgLen = (size_t) psduLen + RSNIFFER_MSG_HEADER_SIZE;
    return pMsgDest;
}

void SRV_RSNIFFER_ParseConfigCommand (
    uint8_t* pDataSrc,
    uint16_t* pBandOpMode,
    uint16_t* pChannel
)
{
    /* Extract Band and Operating Mode */
    *pBandOpMode = ((uint16_t) pDataSrc[1] << 8) + pDataSrc[2];

    /* Extract Channel */
    *pChannel = ((uint16_t) pDataSrc[3] << 8) + pDataSrc[4];
}

