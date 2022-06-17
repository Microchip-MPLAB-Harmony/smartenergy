/*******************************************************************************
  Interface definition of Metrology Driver.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_metrology.h

  Summary:
    Interface definition of the Metrology Driver.

  Description:
    This file defines the interface for the Metrology driver.
*******************************************************************************/

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

#ifndef DRV_METROLOGY_H    // Guards against multiple inclusion
#define DRV_METROLOGY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "driver/driver.h"
#include "system/system.h"
#include "device.h"
#include "configuration.h"
#include "drv_metrology_definitions.h"

#ifdef __cplusplus // Provide C++ Compatibility
    extern "C" {
#endif

#define MET_CAPTURE_BUF_SIZE     DRV_METROLOGY_CAPTURE_BUF_SIZE
        
// *****************************************************************************
/* Metrology Driver Result

  Summary:
    Result of a Metrology driver interface operation.

  Description:
    Identifies the result of certain Metrology driver operations.
*/

typedef enum
{
    // Operation completed with success.
    DRV_METROLOGY_SUCCESS,

    // Invalid handle or operation failed.
    DRV_METROLOGY_ERROR

} DRV_METROLOGY_RESULT;  


typedef enum
{
    // Operation completed with success.
    DRV_METROLOGY_START_SOFT,

    // Invalid handle or operation failed.
    DRV_METROLOGY_START_HARD

} DRV_METROLOGY_START_MODE;  




// *****************************************************************************
// *****************************************************************************
// Section: Interface
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_METROLOGY_Initialize ( const SYS_MODULE_INIT * const init, uint32_t resetValue  );
DRV_METROLOGY_RESULT DRV_METROLOGY_Open ( DRV_METROLOGY_START_MODE mode );
DRV_METROLOGY_RESULT DRV_METROLOGY_Start ( void );
DRV_METROLOGY_RESULT DRV_METROLOGY_IntegrationCallbackRegister ( DRV_METROLOGY_CALLBACK callback);
DRV_METROLOGY_STATE DRV_METROLOGY_GetState ( void );
MET_CONTROL * DRV_METROLOGY_GetControl ( void );
MET_ACCUMULATORS * DRV_METROLOGY_GetAccData ( void );
MET_HARMONICS * DRV_METROLOGY_GetHarData ( void );
void DRV_METROLOGY_SetControl ( MET_CONTROL * pControl );
void DRV_METROLOGY_UpdateMeasurements( void );

#ifdef __cplusplus // Provide C++ Compatibility
 }
#endif

#endif
