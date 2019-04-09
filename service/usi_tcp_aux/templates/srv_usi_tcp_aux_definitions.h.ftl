/*******************************************************************************
  USI Service Definitions Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usi_definitions.h

  Summary:
    USI Service Definitions Header File

  Description:
    This file provides implementation-specific definitions for the USI
    service's system interface.
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

#ifndef DRV_USI_DEFINITIONS_H
#define DRV_USI_DEFINITIONS_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include "stddef.h"
#include "system/system_module.h"

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

// *****************************************************************************
/* USI Service Error

  Summary:
    Defines the error values

  Description:
    This data type defines the error values for the errors occured during transfer.

  Remarks:
    None.
*/

typedef enum
{
    /* Error in protocol identifier */
    SRV_USI_PROTOCOL_ERROR,

    /* Protocol is not registered */
    SRV_USI_PROTOCOL_NOT_REGISTERED,

    /* Error in the format of message */
    SRV_USI_FORMAT_ERROR,

    /* Error in PLIB communication */
    SRV_USI_PLIB_ERROR,

    /* Invalid operation */
    SRV_USI_INVALID

} SRV_USI_ERROR;


typedef void (* DRV_USI_PLIB_CALLBACK)( uintptr_t );

typedef bool (* DRV_USI_PLIB_READ)( uint16_t, uint8_t *, uint32_t );

typedef bool (* DRV_USI_PLIB_WRITE)( uint16_t, uint8_t *, uint32_t );

typedef DRV_USI_ERROR (* DRV_USI_PLIB_ERROR_GET)( void );

typedef void (* DRV_USI_PLIB_CALLBACK_REGISTER)(DRV_USI_PLIB_CALLBACK, uintptr_t);


// *****************************************************************************
/* USI Driver PLib Interface Data

  Summary:
    Defines the data required to initialize the USI service PLib Interface.

  Description:
    This data type defines the data required to initialize the USI service PLib
    Interface.

  Remarks:
    None.
*/

typedef struct
{
    /* USI PLib read API */
    DRV_USI_PLIB_READ                           read;

    /* USI PLib write API */
    DRV_USI_PLIB_WRITE                          write;

    /* USI PLib transfer */
    DRV_USI_PLIB_ERROR_GET                      errorGet;

    /* USI PLib callback register API */
    DRV_USI_PLIB_CALLBACK_REGISTER              callbackRegister;

} DRV_USI_PLIB_INTERFACE;

// *****************************************************************************
/* USI Driver Initialization Data

  Summary:
    Defines the data required to initialize the USI service

  Description:
    This data type defines the data required to initialize or the USI service.
    If the service is built statically, the members of this data structure are
    statically over-ridden by static override definitions in the
    configuration.h file.

  Remarks:
    None.
*/

typedef struct
{
    /* Identifies the PLIB API set to be used by the service to access the
     * peripheral. */
    const DRV_USI_PLIB_INTERFACE*           usiPlib;

    /* Memory Pool for Client Objects */
    uintptr_t                               clientObjPool;

    /* Number of clients */
    uint32_t                                numClients;
<#if DRV_USI_MODE == "Asynchronous">

    /* Size of buffer objects queue */
    uint32_t                                transferObjPoolSize;

</#if>

} DRV_USI_INIT;

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // #ifndef DRV_USI_DEFINITIONS_H
