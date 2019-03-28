/*******************************************************************************
  USI Service Library Interface Header File

  Company
    Microchip Technology Inc.

  File Name
    srv_usi.h

  Summary
    Universal Serial Interface (USI) service library interface.

  Description
    This file defines the interface to the USI service library.  This
    library provides a function that packs and sends each message via 
    the serial link to the external application. For serial receptions 
    from the serial link, the USI provides a function that unpacks the 
    received message and passes it to upper layers. 

  Remarks:
    USI interface is used by all PLC Tools provided by Microchip.
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

#ifndef SRV_USI_H    // Guards against multiple inclusion
#define SRV_USI_H


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
/* USI Service Serial Protocol Identifiers

  Summary:
    Defines the identifier to use for each USI serial protocol

  Description:
    This data type defines the identifier required to specify the
    serial protocol to use by USI service.

  Remarks:
    Do not rely on the underlying type as it may change in different versions.
*/

typedef enum
{
    SRV_USI_PROT_ID_MNGP_PRIME                = 0x00,
    SRV_USI_PROT_ID_MNGP_PRIME_GETQRY         = 0x00,
    SRV_USI_PROT_ID_MNGP_PRIME_GETRSP         = 0x01,
    SRV_USI_PROT_ID_MNGP_PRIME_SET            = 0x02,
    SRV_USI_PROT_ID_MNGP_PRIME_RESET          = 0x03,
    SRV_USI_PROT_ID_MNGP_PRIME_REBOOT         = 0x04,
    SRV_USI_PROT_ID_MNGP_PRIME_FU             = 0x05,
    SRV_USI_PROT_ID_MNGP_PRIME_GETQRY_EN      = 0x06,
    SRV_USI_PROT_ID_MNGP_PRIME_GETRSP_EN      = 0x07,
    SRV_USI_PROT_ID_SNIF_PRIME                = 0x13,
    SRV_USI_PROT_ID_PHY_SERIAL_PRIME          = 0x1F,
    SRV_USI_PROT_ID_PHY                       = 0x22,
    SRV_USI_PROT_ID_SNIFF_G3                  = 0x23,
    SRV_USI_PROT_ID_MAC_G3                    = 0x24,
    SRV_USI_PROT_ID_ADP_G3                    = 0x25,
    SRV_USI_PROT_ID_COORD_G3                  = 0x26,
    SRV_USI_PROT_ID_PHY_MICROPLC              = 0x27,
    SRV_USI_PROT_ID_PRIME_API                 = 0x30,
    SRV_USI_PROT_ID_INVALID                   = 0xFF,
}SRV_USI_PROTOCOL_ID;

// *****************************************************************************
/* USI Driver Error

  Summary:
    Defines the error values

  Description:
    This data type defines the error values for the errors occured during transfer.
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


// *****************************************************************************
/* Function:
    void ( * SRV_USI_CALLBACK ) ( uintptr_t context, uint8_t *data, uint16_t length )

   Summary:
    Pointer to a USI service callback function.

   Description:
    This data type defines a pointer to a USI service callback function, thus
    defining the function signature.  Callback functions should be registered by
    clients of the USI service the SRV_USI_Callback shortcut function.

   Precondition:
    The USI service must have been initialized using the srv_USI_Initialize
    function before attempting to register a USI service callback function.

   Parameters:
    context    - A context value, returned untouched to the client when the
                 callback occurs.  It can be used to identify the instance of
                 the client who registered the callback.
    data       - A pointer to data message to send
    length     - Length of the data message to send
    

   Returns:
    None.

  Example:
    <code>
    //"MY_APP_DATA_STRUCT" is a user defined data structure with "isMessagePending" as
    //one of the structure members.
    void MyCallback ( uintptr_t context, uint8_t *data, uint16_t length )
    {
        MY_APP_DATA_STRUCT* pAppData = (MY_APP_DATA_STRUCT *) context;
        if (pAppData != NULL)
        {
            if ((length > 0) && (pAppData->isMessagePending)) {
              pAppData->isMessagePending = false;  
            } 
        }
    }
    </code>

  Remarks:
    None.
*/

typedef void ( * SRV_USI_CALLBACK ) ( uintptr_t context, uint8_t *data, uint16_t length );


// *****************************************************************************
// *****************************************************************************
// Section: Service Interface Functions
// *****************************************************************************
// *****************************************************************************
/*  Service interface functions are called by system code to initialize the
    module and maintain proper operation of it.
*/

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ SRV_USI_Initialize ( const SYS_MODULE_INDEX index,
                                         const SYS_MODULE_INIT * const init )

   Summary:
        Initializes the USI System module.

   Description:
        This function initializes the instance of the USI System module.

   Parameters:
       index    - Index for the instance to be initialized

       init     - Pointer to a data structure containing data necessary to
                  initialize the module.

   Returns:
        If successful, returns a valid handle to an object. Otherwise, it
        returns SYS_MODULE_OBJ_INVALID.

   Example:
        <code>

        const SRV_USI_PLIB_INTERFACE srvUSIPlibAPI = {
            ?????????????????????????????????????????????????????????????? TBD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        };

        const SRV_USI_INIT srvUSIInitData =
        {
            .USIPlib = &srvUSIPlibAPI,
            .numClients = SRV_USI_CLIENTS_NUMBER_IDX0,
            .baudrate = SRV_USI_BAUDRATE_IDX0,
        };

        SYS_MODULE_OBJ  objSrvUSI;

        objSrvUSI = SRV_USI_Initialize(SRV_USI_INDEX_0, (SYS_MODULE_INIT *)&srvUSIInitData);
        if (objSrvUSI == SYS_MODULE_OBJ_INVALID)
        {
            // Handle error
        }
        </code>

  Remarks:
        This routine should normally only be called once during system
        initialization.
*/

SYS_MODULE_OBJ SRV_USI_Initialize( const SYS_MODULE_INDEX index, const SYS_MODULE_INIT * const init );


// *****************************************************************************
/* Function:
   void SRV_USI_Deinitialize ( const SYS_MODULE_INDEX index )

  Summary:
       Deinitializes the specific module instance of the USI Service module

  Description:
       This function deinitializes the specific module instance disabling its
       operation (and any hardware for driver modules). Resets all of the internal
       data structures and fields for the specified instance to the default settings.

  Precondition:
       The SRV_USI_Initialize function should have been called before calling
       this function.

  Parameters:
       index    - Index for the instance to be deinitialized

  Returns:
       None.

  Example:
        <code>

        SRV_USI_Deinitialize (SRV_USI_INDEX_0);

        if (SRV_USI_Status (SRV_USI_INDEX_0) != SYS_STATUS_UNINITIALIZED)
        {
            // Check again later if you need to know
            // when the USI instance is De-initialized.
        }
        </code>

  Remarks:
       Once the Initialize operation has been called, the De-initialize
       operation must be called before the Initialize operation can be called
       again.
*/

void  SRV_USI_Deinitialize ( const SYS_MODULE_INDEX index );


// *****************************************************************************
/* Function:
       SYS_STATUS SRV_USI_Status ( const SYS_MODULE_INDEX index )

  Summary:
      Returns USI instance service status.

  Description:
       This function returns the current status of the System Time module.

  Precondition:
       None.

  Parameters:
       index    - Index for the instance to get the status

  Returns:
       SYS_STATUS_UNINITIALIZED - Indicates that the USI instance is not initialized.

       SYS_STATUS_READY - Indicates that the USI instance initialization is
                             complete and it ready to be used.

  Example:
       <code>
       if (SRV_USI_Status (SRV_USI_INDEX_0) == SYS_STATUS_READY)
       {
           // USI service is initialized and ready to accept new requests.
       }
       </code>

  Remarks:
       None.
  */

SYS_STATUS SRV_USI_Status ( const SYS_MODULE_INDEX index );

// *****************************************************************************
// *****************************************************************************
// Section:  USI Service Callback Interface Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SRV_USI_HANDLE SRV_USI_CallbackRegister ( const SYS_MODULE_INDEX index, 
                        SRV_USI_PROTOCOL_ID protocol, SRV_USI_CALLBACK callback )

Summary:
    Registers a function to be called back when a new message is received and it
    belongs to the specified protocol.

Description:
    Registers a function with it to be called back when a new message is received.
    USI service extracts the protocol information from the new message and, if a
    callback function is registered, it calls to it.

Precondition:
    The SRV_USI_Initialize function should have been called before calling this
    function.

Parameters:
    index       - Index for the instance to set the callback function
    protocol    - Identifier of the protocol in which callback function will be 
                  registered.
    callback    - Pointer to the function to be called.

Returns:
    SRV_USI_HANDLE - A valid object handle if the call succeeds.
                      SRV_USI_HANDLE_INVALID if it fails.

Example:
  Given a callback function implementation matching the following prototype:
  <code>
  void MyG3SnifferCallback ( const SYS_MODULE_INDEX index, 
                    SRV_USI_PROTOCOL_ID protocol, SRV_USI_CALLBACK callback);
  </code>

  The following example call will register it, requesting a callback for G3 Sniffer
  application (SRV_USI_PROT_ID_SNIFF_G3)
  <code>
  //Give a SRV_USI_CALLBACK function "MyG3SnifferCallback",
  SRV_USI_HANDLE handle = SRV_USI_CallbackRegisterUS(SRV_USI_INDEX_0, SRV_USI_PROT_ID_SNIFF_G3, 
                                                     MyG3SnifferCallback);
  if (handle != SRV_USI_HANDLE_INVALID)
  {
        //Callback is registered successfully.
  }
  </code>
*/

SRV_USI_HANDLE SRV_USI_CallbackRegister ( const SYS_MODULE_INDEX index, SRV_USI_PROTOCOL_ID protocol, 
                                          SRV_USI_CALLBACK callback );




/***************************************************************************
  Function:
       void SRV_USI_Tasks( const SYS_MODULE_INDEX index )
    
  Summary:
    Maintains the USI's state machine.

  Description:
    This function is used to maintain the USI's internal state machine and
    generate callback functions.

  Precondition:
    The SRV_USI_Initialize routine must have been called for the specified 
    USI service instance.

  Parameters:
    index       - Index for the instance to set the callback function
  Returns:
    None
  Example:
    <code>
    
    while (true)
    {
        SRV_USI_Tasks (SRV_USI_INDEX_0);
    
        // Do other tasks
    }
    </code>

  Remarks:
    - This function is normally not called directly by an application. It is
      called by the system's Tasks routine (SYS_Tasks)
    - This function will never block or access any resources that may cause
      it to block.                        
  ***************************************************************************/

void SRV_USI_Tasks( const SYS_MODULE_INDEX index );

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif //SRV_USI_H
