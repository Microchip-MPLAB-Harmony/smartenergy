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
#include "driver/pl360/drv_pl360_comm.h"

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

#define PSERIAL_TONEMAP_SIZE     1
#define PSERIAL_SUBBANDS_SIZE    6   
#define PSERIAL_CARRIERS_SIZE    36        
#define PSERIAL_RS_2_BLOCKS      0
       
void SRV_PSERIAL_ParseGetPIB(DRV_PL360_PIB_OBJ* pDataDst, uint8_t* pDataSrc);
size_t SRV_PSERIAL_SerialGetPIB(uint8_t* pDataDst, DRV_PL360_PIB_OBJ* pDataSrc);
void SRV_PSERIAL_ParseSetPIB(DRV_PL360_PIB_OBJ* pDataDst, uint8_t* pDataSrc);
size_t SRV_PSERIAL_SerialSetPIB(uint8_t* pDataDst, DRV_PL360_PIB_OBJ* pDataSrc);
void SRV_PSERIAL_ParseTxMessage(DRV_PL360_TRANSMISSION_OBJ* pDataDst, uint8_t* pDataSrc);
size_t SRV_PSERIAL_SerialRxMessage(uint8_t* pDataDst, DRV_PL360_RECEPTION_OBJ* pDataSrc);
size_t SRV_PSERIAL_SerialCfmMessage(uint8_t* pDataDst, DRV_PL360_TRANSMISSION_CFM_OBJ* pDataSrc);

#endif //SRV_PCRC_H
