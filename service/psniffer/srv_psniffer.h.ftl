/*******************************************************************************
  Phy Sniffer Serialization header file.

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

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#ifndef SRV_PSNIFFER_H    // Guards against multiple inclusion
#define SRV_PSNIFFER_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "system/system.h"
#include "driver/pl360/drv_pl360_comm.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Macro definitions
// *****************************************************************************
// *****************************************************************************
<#if SRV_PSNF_PLC_PROFILE == "0">
#define PSNIFFER_TONEMAP_SIZE     1
#define PSNIFFER_SUBBANDS_SIZE    6   
#define PSNIFFER_CARRIERS_SIZE    36        
#define PSNIFFER_RS_2_BLOCKS      0
#define PSNIFFER_VERSION          0x02
#define PSNIFFER_PROFILE          0x12
</#if>
<#if SRV_PSNF_PLC_PROFILE == "1">
#define PSNIFFER_TONEMAP_SIZE     1
#define PSNIFFER_SUBBANDS_SIZE    4   
#define PSNIFFER_CARRIERS_SIZE    16        
#define PSNIFFER_RS_2_BLOCKS      0
#define PSNIFFER_VERSION          0x02
#define PSNIFFER_PROFILE          0x12
</#if>
<#if SRV_PSNF_PLC_PROFILE == "2">
#define PSNIFFER_TONEMAP_SIZE     3
#define PSNIFFER_SUBBANDS_SIZE    24   
#define PSNIFFER_CARRIERS_SIZE    72        
#define PSNIFFER_RS_2_BLOCKS      1
#define PSNIFFER_VERSION          0x02
#define PSNIFFER_PROFILE          0x12
</#if>
<#if SRV_PSNF_PLC_PROFILE == "4">
#define PSNIFFER_VERSION          0x14
#define PSNIFFER_PROFILE          0x11
#define PSNIFFER_MSG_TYPE_A       0x20
#define PSNIFFER_MSG_TYPE_B       0x21
#define PSNIFFER_MSG_TYPE_BC      0x22
#define PSNIFFER_P13_PREAMBLE_US  2048L
#define PSNIFFER_P13_HEADER_US    4480L
#define PSNIFFER_SYMBOL_US        2240L
#define PSNIFFER_PP_PREAMBLE_US   (PSNIFFER_P13_PREAMBLE_US << 2)
#define PSNIFFER_PP_HEADER_US     (PSNIFFER_SYMBOL_US << 2)
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/* PLC Phy Sniffer Tool command

  Summary:
    PLC Sniffer Commands enumeration

  Description:
    This enumeration defines the PLC commands used by PLC Phy Sniffer Tool
    provided by Microchip.
*/
typedef enum
{
  /* Receive new PLC message */
  SRV_PSNIFFER_CMD_RECEIVE_MSG = 0,       
  /* Set Tone Mask request */ 
  SRV_PSNIFFER_CMD_SET_TONE_MASK,      
  /* Set PLC Channel */    
  SRV_PSNIFFER_CMD_SET_CHANNEL,           
} SRV_PSNIFFER_COMMAND;      

// *****************************************************************************
// *****************************************************************************
// Section: SRV_PSNIFFER Interface Routines
// *****************************************************************************
// *****************************************************************************

SRV_PSNIFFER_COMMAND SRV_PSNIFFER_GetCommand(uint8_t* pData);
size_t SRV_PSNIFFER_SerialRxMessage(uint8_t* pDataDst, DRV_PL360_RECEPTION_OBJ* pDataSrc);
size_t SRV_PSNIFFER_SerialCfmMessage(uint8_t* pDataDst, DRV_PL360_TRANSMISSION_CFM_OBJ* pDataCfm);
void SRV_PSNIFFER_SetTxMessage(DRV_PL360_TRANSMISSION_OBJ* pDataDst);
void SRV_PSNIFFER_SetRxPayloadSymbols(uint16_t payloadSym);
void SRV_PSNIFFER_SetTxPayloadSymbols(uint16_t payloadSym);
<#if SRV_PSNF_PLC_PROFILE == "4">
void SRV_PSNIFFER_SetPLCChannel(uint8_t channel);
<#else>
void SRV_PSNIFFER_ConvertToneMask(uint8_t* pToneMaskDst, uint8_t* pToneMaskSrc);
</#if>

#endif //SRV_PSNIFFER_H
