/*******************************************************************************
  CRC service used by PLC components Header File

  Company:
    Microchip Technology Inc.

  File Name:
    srv_pcrc.h

  Summary:
    CRC service used by PLC components Implementation.

  Description:
    The CRC wrapper provides a simple interface to manage the CRC needs
    for PLC components. This file implements the CRC core interface routines 
    for PLC. 

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

#ifndef SRV_PCRC_H    // Guards against multiple inclusion
#define SRV_PCRC_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section lists the other files that are included in this file.
*/

#include <stdint.h>
#include <stdbool.h>
#include "system/system.h"
#include "service/usi/srv_usi_definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef enum 
{
    PCRC_HT_GENERIC = 0,
    PCRC_HT_PROMOTION = 1,
    PCRC_HT_BEACON = 2,
    PCRC_HT_USI = 3
}PCRC_HEADER_TYPE;

typedef enum 
{
    PCRC_CRC8 = 0,
    PCRC_CRC16 = 1,
    PCRC_CRC32 = 2,
    PCRC_NOCRC = 3
}PCRC_CRC_TYPE;

#define PCRC_INVALID     0xFFFFFFFF
        
uint32_t SRV_PCRC_GetValue(uint8_t *pData, size_t length,
        PCRC_HEADER_TYPE hdrType, PCRC_CRC_TYPE crcType);

void SRV_PCRC_SetInitialValue(uint32_t value);

void SRV_PCRC_SetSNAValue (uint8_t* sna);

#endif //SRV_PCRC_H
