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
#include "srv_usi_definitions.h"

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
/* USI Service Handle

  Summary:
    Handle to an opened USI service.

  Description:
    This handle identifies the open instance of a USI service.  It must be
    passed to all other USI routines (except the initialization, deinitialization,
    or power routines) to identify the caller.

  Remarks:
    Every application or module that wants to use the USI service must first call
    the USI's open routine.  This is the only routine that is absolutely
    required for every service.

    If a USI service is unable to allow an additional module to use it, it must then
    return the special value SRV_USI_HANDLE_INVALID.  Callers should check the
    handle returned for this value to ensure this value was not returned before
    attempting to call any other USI routines using the handle.
*/

typedef uintptr_t SRV_USI_HANDLE;


// *****************************************************************************
/* Invalid USI Service Handle

 Summary:
    Invalid USI service.

 Description:
    If the USI service is unable to allow an additional module to use it, it must then
    return the special value SRV_USI_HANDLE_INVALID.  Callers should check the
    handle returned for this value to ensure this value was not returned before
    attempting to call any other USI routines using the handle.

 Remarks:
    None.
*/

#define SRV_USI_HANDLE_INVALID  (((SRV_USI_HANDLE) -1))

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

    const SRV_USI_USART_INTERFACE srvUSIUsartApi = {
      .open = (SRV_USI_USART_OPEN)DRV_USART_Open,
      .read = (SRV_USI_USART_READ)DRV_USART_ReadBufferAdd,
      .write = (SRV_USI_USART_WRITE)DRV_USART_WriteBufferAdd,
      .eventHandlerSet = (SRV_USI_USART_EVENT_HANDLER_SET)DRV_USART_BufferEventHandlerSet,
      .close = (SRV_USI_USART_CLOSE)DRV_USART_Close
    };

    const SRV_USI_INIT srvUSI0InitData =
    {
      .usiInterfaceApi = SRV_USI_USART_API,

      .usartApi = &srvUSIUsartApi,

      .cdcApi = SRV_USI_INVALID_API,

      .tcpApi = SRV_USI_INVALID_API,
    };

    SYS_MODULE_OBJ  objSrvUSI;

    objSrvUSI= SRV_USI_Initialize(SRV_USI_INDEX_0, (SYS_MODULE_INIT *)&srvUSI0InitData);
    if (objSrvUSI == SYS_MODULE_OBJ_INVALID)
    {
        // Handle error
    }
    </code>

  Remarks:
    This routine should normally only be called once during system
    initialization.
*/

SYS_MODULE_OBJ SRV_USI_Initialize(
    const SYS_MODULE_INDEX index,
    const SYS_MODULE_INIT * const init);

// *****************************************************************************
/* Function:
    SRV_USI_HANDLE SRV_USI_Open
    (
        const SYS_MODULE_INDEX index
    )

  Summary:
    Opens the specified USI service instance and returns a handle to it.

  Description:
    This routine opens the specified USI Service instance and provides a
    handle that must be provided to all other client-level operations to
    identify the caller and the instance of the driver.

  Precondition:
    Function SRV_USI_Initialize must have been called before calling this
    function.

  Parameters:
    index - Identifier for the object instance to be opened

  Returns:
    If successful, the routine returns a valid open-instance handle.

    If an error occurs, the return value is SRV_USI_HANDLE_INVALID. 
    Error can occur
    - if the client is trying to open the driver but driver has been already 
      opened.
    - if the driver peripheral instance being opened is not initialized or is
      invalid.
    - if the driver is not ready to be opened, typically when the initialize
      routine has not completed execution.

  Example:
    <code>
    SRV_USI_HANDLE handle;

    handle = SRV_USI_Open(SRV_USI_INDEX_0);
    if (handle == SRV_USI_HANDLE_INVALID)
    {
        // Unable to open USI instance
        // May be the driver is not initialized or the initialization
        // is not complete.
    }
    </code>

  Remarks:
    The handle returned is valid until the SRV_USI_Close routine is called.
    This routine will NEVER block waiting for hardware. If the requested intent
    flags are not supported, the routine will return SRV_USI_HANDLE_INVALID. 
*/

SRV_USI_HANDLE SRV_USI_Open( const SYS_MODULE_INDEX index );

// *****************************************************************************
/* Function:
    void SRV_USI_Close( SRV_USI_HANDLE handle )

  Summary:
    Closes an opened-instance of the USI Service.

  Description:
    This routine closes an opened-instance of the USI Service, invalidating the
    handle. Any buffers in the driver queue that were submitted by this client
    will be removed. A new handle must be obtained by calling SRV_USI_Open
    before the caller may use the driver again.

  Precondition:
    SRV_USI_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - A valid open-instance handle, returned from the driver's
    open routine

  Returns:
    None.

  Example:
    <code>
    // 'handle', returned from the SRV_USI_Open

    SRV_USI_Close(handle);

    </code>

  Remarks:
    None.
*/

void SRV_USI_Close( const SRV_USI_HANDLE handle );

// *****************************************************************************
/* Function:
    SYS_STATUS SRV_USI_Status( SYS_MODULE_OBJ object)

  Summary:
    Returns USI instance service status.

  Description:
    This function returns the current status of the System Time module.

  Precondition:
    None.

  Parameters:
    object - USI service object handle, returned from the
    SRV_USI_Initialize routine

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

SYS_STATUS SRV_USI_Status( SYS_MODULE_OBJ object);

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
    handle - A valid open-instance handle, returned from the driver's
    open routine
    protocol    - Identifier of the protocol in which callback function will be 
                  registered.
    callback    - Pointer to the function to be called.

  Returns:
    None. 

  Example:
    Given a callback function implementation matching the following prototype:
    <code>
    void MyG3SnifferCallback ( const SYS_MODULE_INDEX index, 
                      SRV_USI_PROTOCOL_ID protocol, SRV_USI_CALLBACK callback);
    </code>

    The following example call will register it, requesting a callback for G3 
    Sniffer application (SRV_USI_PROT_ID_SNIFF_G3).
    <code>
    //Give a SRV_USI_CALLBACK function "MyG3SnifferCallback",
    // 'handle', returned from the SRV_USI_Open
    SRV_USI_HANDLE handle = SRV_USI_CallbackRegisterUS(handle, 
                      SRV_USI_PROT_ID_SNIFF_G3, MyG3SnifferCallback);

    if (handle != SRV_USI_HANDLE_INVALID)
    {
          //Callback is registered successfully.
    }
    </code>
*/

void SRV_USI_CallbackRegister( 
    const SRV_USI_HANDLE handle,
    SRV_USI_PROTOCOL_ID protocol, 
    SRV_USI_CALLBACK callback);

// *****************************************************************************
/* Function:
  void SRV_USI_Register_Reception_buffer( const SRV_USI_HANDLE handle, 
        uint8_t *buffer, uint16_t size )

  Summary:
    Registers a data buffer to be used to store received messages.

  Description:
    Registers a data buffer to be used when a new message is received.
    USI service extracts the protocol information from the new message and leave
    clean data to application.

  Precondition:
    The SRV_USI_Initialize function should have been called before calling this
    function.

  Parameters:
    buffer    - Pointer to buffer to use in serial reception.
    size      - Size of the buffer in bytes.

  Returns:
    None. 

  Example:
    
    The following example call will register a data buffer for a given handler
    <code>
    static uint8_t MyUSIRxBuffer[512];
 
    // 'handle', returned from the SRV_USI_Open
    SRV_USI_Set_Reception_buffer( handle, MyUSIRxBuffer, sizeof(MyUSIRxBuffer));
 
    </code>
*/

void SRV_USI_Set_Transfer( 
    const SRV_USI_HANDLE handle, 
    void*   pTransmitData,
    size_t  txSize,
    void*   pReceiveData,
    size_t  rxSize);

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
