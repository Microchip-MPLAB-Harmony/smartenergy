/*******************************************************************************
  USI Service Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    srv_usi_local.h

  Summary:
    USI Service Local Data Structures

  Description:
    Service Local Data Structures
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

#ifndef SRV_USI_LOCAL_H
#define SRV_USI_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "service/usi/srv_usi.h"
#include "service/usi/srv_usi_definitions.h"
#include "osal/osal.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USI Callback Index

  Summary:
    Index to identify callback position in USI callback object.

  Description:
    None.

  Remarks:
    None.
*/

typedef uint8_t SRV_USI_CALLBACK_INDEX;


// *****************************************************************************
/* Invalid USI Callback Index

 Summary:
    Invalid USI Callback Index.

 Description:
    None.

 Remarks:
    None.
*/

#define SRV_USI_CALLBACK_INDEX_INVALID  (((SRV_USI_CALLBACK_INDEX) -1))

// *****************************************************************************
/* USI Callback Object

  Summary:
    Array of callback functions associated to every USI protocol.

  Description:
    There are 10 different USI protocols.

  Remarks:
    None.
*/

typedef SRV_USI_CALLBACK SRV_USI_CALLBACK_OBJ[10];

// *****************************************************************************
/* USI Service Instance Object

  Summary:
    Object used to keep any data required for each instance of the USI Service.

  Description:
    None.

  Remarks:
    None.
*/

typedef struct
{
    /* Flag to indicate this object is in use */
    bool                                     inUse;

    /* The status of the service */
    SYS_STATUS                               status;

    /* Identifies the USI instance */
    SYS_MODULE_INDEX                         usiIndex;

    /* Identifies the interface with peripherals */
    SRV_USI_INTERFACE_API                    usiInterfaceApi;

    /* Identifies the USI interface API with peripherals */
    void*                                    usiApi;

    /* Identifies the Driver used for USI interface API */
    DRV_HANDLE                               apiDriver;

    /* Pointer to the application read buffer */
    void*                                    readBuffer;

    /* Maximum number of bytes of the application buffer to receive serial data */
    size_t                                   readSizeMax;

    /* Pointer to the application write buffer */
    void*                                    writeBuffer;

    /* Maximum number of bytes of the application buffer to send serial data */
    size_t                                   writeSizeMax;  

    /* Pointer to the internal read buffer */
    void*                                    readInternalBuffer;

    /* Maximum number of bytes of the internal buffer to receive serial data */
    size_t                                   readInternalSizeMax;

    /* Identifies the USI callback object */
    SRV_USI_CALLBACK*                        callback;

} SRV_USI_OBJ;

#endif //#ifndef SRV_USI_LOCAL_H
