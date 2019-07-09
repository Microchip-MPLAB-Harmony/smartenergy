/*******************************************************************************
  DRV_PLC Hardware Abstraction Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_plc_hal.h

  Summary:
    PLC Driver Hardware Abstraction Layer Header File

  Description:
    The PLC Library provides a Hardware Abstraction Layer.
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

#ifndef _DRV_PLC_HAL_H
#define _DRV_PLC_HAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdbool.h>
#include "drv_g3_macrt_definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// ***************************************************************************** 
#define DRV_PLC_HAL_CMD_POS                   15
#define DRV_PLC_HAL_CMD_RD                    (0 << DRV_PLC_HAL_CMD_POS)
#define DRV_PLC_HAL_CMD_WR                    (1 << DRV_PLC_HAL_CMD_POS)

#define DRV_PLC_HAL_LEN_MASK                  0x7FFF
        
/* SPI Key MASK */
#define DRV_PLC_HAL_KEY_MASK                  0xFFFE
/* SPI Key when bootloader is running in PLC device */
#define DRV_PLC_HAL_KEY_BOOT                  (0x5634 & DRV_PLC_HAL_KEY_MASK)
/* SPI Key when PLC firmware is running in PLC device */
#define DRV_PLC_HAL_KEY_CORTEX                (0x1122 & DRV_PLC_HAL_KEY_MASK)
        
#define DRV_PLC_HAL_KEY(b0, b1)               ((((uint16_t)b1 << 8) + b0) & DRV_PLC_HAL_KEY_MASK)
#define DRV_PLC_HAL_FLAGS_BOOT(b0, b2, b3)    ((((uint32_t)b3) << 8) + ((uint32_t)b2) + ((uint32_t)(b0 & 0x01) << 16))
#define DRV_PLC_HAL_FLAGS_CORTEX(b2, b3)      ((((uint32_t)b3) << 8) + ((uint32_t)b2))        

/* User rest flag in bootloader key*/
#define DRV_PLC_HAL_FLAG_RST_USER             0x00010000
/* Cortex(debugger) rest flag in bootloader key*/
#define DRV_PLC_HAL_FLAG_RST_CORTEX           0x00008000
/* Watch Dog flag in bootloader key */
#define DRV_PLC_HAL_FLAG_RST_WDOG             0x00004000
/* Power-ON reset is indicated when the three flags are 0, mask will be used to detect it*/
#define DRV_PLC_HAL_FLAG_RST_PON              0x0001C000  

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
        
// *****************************************************************************
/* DRV_PLC_HAL Command object

  Summary:
    Object used to hanlde any SPI transfer.

  Description:
    None.

  Remarks:
    None.
*/        

typedef struct
{
    uint8_t *pData;
    uint16_t length;
    uint16_t memId;
    uint16_t cmd;
}DRV_PLC_HAL_CMD;

// *****************************************************************************
/* DRV_PLC_HAL header Information

  Summary:
    Object used to keep track of a SPI transfer with PLC device.

  Description:
    None.

  Remarks:
    None.
*/    

typedef struct
{
    uint32_t key;
    uint32_t flags;
}DRV_PLC_HAL_INFO;

// *****************************************************************************
// *****************************************************************************
// Section: DRV_PLC_HAL Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

void DRV_PLC_HAL_Init(DRV_PLC_PLIB_INTERFACE *plcPlib);
void DRV_PLC_HAL_Reset(void);
void DRV_PLC_HAL_Setup(bool set16Bits);
bool DRV_PLC_HAL_GetCarrierDetect(void);
void DRV_PLC_HAL_EnableInterrupts(bool enable);
void DRV_PLC_HAL_Delay(uint64_t delayUs);
void DRV_PLC_HAL_SendBootCmd(uint16_t cmd, uint32_t address, uint32_t dataLength, uint8_t *pDataWr, uint8_t *pDataRd);
void DRV_PLC_HAL_SendWrRdCmd(DRV_PLC_HAL_CMD *pCmd, DRV_PLC_HAL_INFO *pInfo);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _DRV_PLC_HAL_H
/*******************************************************************************
 End of File
*/
