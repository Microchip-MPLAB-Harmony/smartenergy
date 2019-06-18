/*******************************************************************************
  DRV_PL360 Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360.h

  Summary:
    PL360 Library Interface header.

  Description:
    The PL360 Driver Library provides a interface to access the PL360 device.
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

#ifndef _DRV_PL360_H
#define _DRV_PL360_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdbool.h>
#include "driver/driver.h"
#include "system/system.h"
#include "drv_pl360_comm.h"
#include "drv_pl360_definitions.h"

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
/* DRV_PL360 Transfer Status

 Summary:
    Defines the data type for PL360 Driver transfer status.

 Description:
    This will be used to indicate the current transfer status of the
    PL360 driver operations.

 Remarks:
    None.
*/

typedef enum
{
    /* Transfer is being processed */
    DRV_PL360_TRANSFER_STATUS_BUSY,

    /* Transfer is successfully completed*/
    DRV_PL360_TRANSFER_STATUS_COMPLETED,

    /* Transfer had error or first transfer request is not made */
    DRV_PL360_TRANSFER_STATUS_ERROR

} DRV_PL360_TRANSFER_STATUS;     

/* DRV_PL360 Transfer Errors

 Summary:
    Defines the data type for PL360 Driver transfer errors.

 Description:
    This will be used to indicate the error of the last SPI transfer.

 Remarks:
    None.
*/

typedef enum
{
    /* SPI has detected an unexpected status, reset is recommended */
    DRV_PL360_EXCEPTION_UNEXPECTED_KEY,

    /* SPI critical error */
    DRV_PL360_EXCEPTION_CRITICAL_ERROR,

    /* Device has been reseted by Debugging tool */
    DRV_PL360_EXCEPTION_DEBUG,

    /* Device has been reseted */
    DRV_PL360_EXCEPTION_RESET

} DRV_PL360_EXCEPTION; 

// *****************************************************************************
/* PL360 Driver Transmission Confirm Event Handler Function Pointer

   Summary
    Pointer to a PL360 Driver Transmission Confirm Event handler function

   Description
    This data type defines the required function signature for the PL360 driver
    transmission confirm event handling callback function. A client must register 
    a pointer using the callback register function whose function signature 
    (parameter and return value types) match the types specified by this function 
    pointer in order to receive transfer related event calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    cfmObj -            Pointer to the object containing any data necessary to
                        identify the result of the last transmission.

    context -           Value identifying the context of the application that
                        registered the event handling function.

  Returns:
    None.

  Example:
    <code>
    void APP_MyTXCfmEventHandler( DRV_PL360_TRANSMISSION_CFM_OBJ *cfmObj,
                                   uintptr_t context )
    {
        MY_APP_DATA_STRUCT pAppData = (MY_APP_DATA_STRUCT) context;

        switch(cfmObj->result)
        {
            case DRV_PL360_TX_RESULT_PROCESS:
                // Transmission result: already in process
                break;   
            case DRV_PL360_TX_RESULT_SUCCESS:
                // Transmission result: end successfully
                break;   
            case DRV_PL360_TX_RESULT_INV_LENGTH:
                // Transmission result: invalid length error
                break;
            case DRV_PL360_TX_RESULT_BUSY_CH:
                // Transmission result: busy channel error
                break;    
            case DRV_PL360_TX_RESULT_BUSY_TX:
                // Transmission result: busy in transmission error
                break;    
            case DRV_PL360_TX_RESULT_BUSY_RX:
                // Transmission result: busy in reception error
                break;   
            case DRV_PL360_TX_RESULT_INV_SCHEME:
                // Transmission result: invalid modulation scheme error
                break; 
            case DRV_PL360_TX_RESULT_TIMEOUT:
                // Transmission result: timeout error
                break;   
            case DRV_PL360_TX_RESULT_INV_TONEMAP:
                // Transmission result: invalid tone map error
                break;
            case DRV_PL360_TX_RESULT_INV_MODE:
                // Transmission result: invalid G3 Mode error
                break;   
            case DRV_PL360_TX_RESULT_NO_TX:
                // Transmission result: No transmission ongoing
                break;   
        }
    }
    </code>

  Remarks:
    - If the result field is DRV_PL360_TX_RESULT_SUCCESS, it means that the data was
      transferred successfully.

    - Otherwise, it means that the data was not transferred successfully.

    - The context parameter contains the a handle to the client context,
      provided at the time the event handling function was registered using the
      DRV_PL360_DataCfmCallbackRegister function.  This context handle value is
      passed back to the client as the "context" parameter.  It can be any value
      necessary to identify the client context or instance (such as a pointer to
      the client's data) of the client that made the transfer add request.

    - The event handler function executes in task context of the peripheral.
      Hence it is recommended of the application to not perform process
      intensive or blocking operations with in this function.

*/

typedef void ( *DRV_PL360_DATA_CFM_CALLBACK )( DRV_PL360_TRANSMISSION_CFM_OBJ *cfmObj, uintptr_t context );

// *****************************************************************************
/* PL360 Driver Reception Event Handler Function Pointer

   Summary
    Pointer to a PL360 Driver Reception Event handler function

   Description
    This data type defines the required function signature for the PL360 driver
    reception event handling callback function. A client must register a pointer 
    using the callback register function whose function signature (parameter 
    and return value types) match the types specified by this function pointer 
    in order to receive transfer related event calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    indObj -            Pointer to the object containing any data necessary to
                        describe the new received message.

    context -           Value identifying the context of the application that
                        registered the event handling function.

  Returns:
    None.

  Example:
    <code>
    void APP_MyRxEventHandler( DRV_PL360_RECEPTION_OBJ *indObj,
                                   uintptr_t context )
    {
        MY_APP_DATA_STRUCT pAppData = (MY_APP_DATA_STRUCT) context;

        // Check length of the new message
        if (indObj->dataLength) {
          // extract data from received message
          memcpy(appData.pDataRx, indObj->pReceivedData, indObj->dataLength);
        }
    </code>

  Remarks:
    - Check DRV_PL360_RECEPTION_OBJ struct to identify other intereseting 
      parameters about reception such as LQI, Tonemap, Modulation scheme, etc.

    - The context parameter contains the a handle to the client context,
      provided at the time the event handling function was registered using the
      DRV_PL360_DataIndCallbackRegister function.  This context handle value is
      passed back to the client as the "context" parameter.  It can be any value
      necessary to identify the client context or instance (such as a pointer to
      the client's data) of the client that made the transfer add request.

    - The event handler function executes in task context of the peripheral.
      Hence it is recommended of the application to not perform process
      intensive or blocking operations with in this function.

*/

typedef void ( *DRV_PL360_DATA_IND_CALLBACK )( DRV_PL360_RECEPTION_OBJ *indObj, uintptr_t context );

// *****************************************************************************
/* PL360 Driver Exceptions Event Handler Function Pointer

   Summary
    Pointer to a PL360 Driver Exceptions Event handler function

   Description
    This data type defines the required function signature for the PL360 driver
    exceptions event handling callback function. A client must register a pointer 
    using the callback register function whose function signature (parameter 
    and return value types) match the types specified by this function pointer 
    in order to receive transfer related event calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    exception -         Value identifying the exception code which occurs during
                        SPI transfer.

    context -           Value identifying the context of the application that
                        registered the event handling function.

  Returns:
    None.

  Example:
    <code>
    void APP_MyExceptionEventHandler( DRV_PL360_EXCEPTION exception,
                                   uintptr_t context )
    {
        MY_APP_DATA_STRUCT pAppData = (MY_APP_DATA_STRUCT) context;

        switch (exceptionObj) 
        {
            case DRV_PL360_EXCEPTION_UNEXPECTED_KEY:
                // SPI has detected an unexpected status, reset is recommended
                break;

            case DRV_PL360_EXCEPTION_CRITICAL_ERROR:
                // SPI critical error in last transfer. Bootloader task has failured.
                break;

            case DRV_PL360_EXCEPTION_DEBUG:
                // PL360 device has been reseted by Debugging tool
                break;

            case DRV_PL360_EXCEPTION_RESET:
                // PL360 device has been reseted
                break;

            default:
                // SPI has detected an unexpected status, reset is recommended
        }

        appData.pl360_exception = true;
    }
    </code>

  Remarks:
    - The context parameter contains the a handle to the client context,
      provided at the time the event handling function was registered using the
      DRV_PL360_DataIndCallbackRegister function.  This context handle value is
      passed back to the client as the "context" parameter.  It can be any value
      necessary to identify the client context or instance (such as a pointer to
      the client's data) of the client that made the transfer add request.

    - The event handler function executes during the last SPI transaction of the 
      peripheral. Hence it is recommended of the application to not perform process
      intensive or blocking operations with in this function.

*/

typedef void ( *DRV_PL360_EXCEPTION_CALLBACK )( DRV_PL360_EXCEPTION exception, uintptr_t context );

// *****************************************************************************
/* PL360 Driver Boot Data Callback Function Pointer

   Summary
    Pointer to a PL360 Driver Boot Data Callback Function

   Description
    This data type defines the required function signature for the PL360 driver
    boot data callback function. A client must register a pointer 
    using the PL360 open function whose function signature (parameter 
    and return value types) match the types specified by this function pointer 
    in order to be notified when more data will be necessary while bootloader 
    process.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    address -           Address where new block of the binary data image is 
                        ready to be trasfered.

    length -            Length of the new block to be trasnfered. Maximum size 
                        is 634 bytes. If length is set to 0, it means that
                        binary data image transfer has finished and PL360 device
                        is ready to start up.

    context -           Value identifying the context of the application that
                        registered the event handling function. In this case, 
                        this value is fixed to the SYS_MODULE_INDEX passed in
                        open function.

  Returns:
    None.

  Example:
    <code>
    #define APP_BOOT_DATA_FRAG_SIZE    512  

    static uint32_t gBootDataNextAddr = 0x00450000;
    static uint16_t gBootDataPending = 64000;

    static void APP_PLCBootDataCb(uint32_t *address, uint16_t *length, uintptr_t context)
    {   
        // Avoid warnings
        (void)context;
        
        if (gBootDataPending)
        {
            *address = gBootDataNextAddr;
            
            gBootDataNextAddr += APP_BOOT_DATA_FRAG_SIZE;
            
            if (gBootDataPending > APP_BOOT_DATA_FRAG_SIZE)
            {
                *length = APP_BOOT_DATA_FRAG_SIZE;
                gBootDataPending -= APP_BOOT_DATA_FRAG_SIZE;
            }
            else
            {
                *length = APP_BOOT_DATA_FRAG_SIZE - gBootDataPending;
                gBootDataPending = 0;
            }
        }
        else
        {
            // End Boot Data Transfer 
            *length = 0;
        }    
    }

    DRV_HANDLE handle;

    handle = DRV_PL360_Open(DRV_PL360_INDEX_0, APP_PLCBootDataCb);
    if (handle == DRV_HANDLE_INVALID)
    {
        // Unable to open the driver
        // May be the driver is not initialized
    }

    </code>

  Remarks:
    - The context parameter contains the SYS_MODULE_INDEX provided at the time 
      the open function was called. This SYS_MODULE_INDEX value is passed back 
      to the client as the "context" parameter.  

    - Length parameter is used to notify that all binary image has been transfered
      to PL360 dirver and it should be ready to start up. This is indicated using
      length = 0;

    - The event handler function executes in task context of the peripheral.
      Hence it is recommended of the application to not perform process
      intensive or blocking operations with in this function.

*/

typedef void ( *DRV_PL360_BOOT_DATA_CALLBACK )( uint32_t *address, uint16_t *length, uintptr_t context );

// *****************************************************************************
// *****************************************************************************
// Section: DRV_PL360 Driver System Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ DRV_PL360_Initialize(
        const SYS_MODULE_INDEX index,
        const SYS_MODULE_INIT * const init
    )

  Summary:
    Initializes the PL360 instance for the specified driver index.

  Description:
    This routine initializes the PL360 device driver making it ready for
    clients to open and use. The initialization data is specified by the init
    parameter. It is a single instance driver, so this API should be called
    only once.

  Precondition:
    None.

  Parameters:
    index - Identifier for the instance to be initialized (single instance allowed)

    init  - Pointer to the init data structure containing any data necessary to
            initialize the driver.

  Returns:
    If successful, returns a valid handle to a driver instance object.
    Otherwise, returns SYS_MODULE_OBJ_INVALID.

  Example:
    <code>
    SYS_MODULE_OBJ   sysObjDrvPL360;

    DRV_PL360_PLIB_INTERFACE drvPL360Plib = {

        .spiPlibTransferSetup = (DRV_PL360_SPI_PLIB_TRANSFER_SETUP)SPI0_TransferSetup,
        .dmaChannelTx = SYS_DMA_CHANNEL_1,
        .dmaChannelRx  = SYS_DMA_CHANNEL_0,
        .spiAddressTx =  (void *)&(SPI0_REGS->SPI_TDR),
        .spiAddressRx  = (void *)&(SPI0_REGS->SPI_RDR),
        .spiClockFrequency = DRV_PL360_SPI_CLK,
        .ldoPin = DRV_PL360_LDO_EN_PIN, 
        .resetPin = DRV_PL360_RESET_PIN,
        .extIntPin = DRV_PL360_EXT_INT_PIN,
    };

    DRV_PL360_HAL_INTERFACE drvPL360HalAPI = {
        .pl360Plib = &drvPL360Plib,
        .init = (DRV_PL360_HAL_INIT)drv_pl360_hal_init,
        .setup = (DRV_PL360_HAL_SETUP)drv_pl360_hal_setup,
        .reset = (DRV_PL360_HAL_RESET)drv_pl360_hal_reset,
        .getCd = (DRV_PL360_HAL_GET_CD)drv_pl360_hal_get_cd,
        .enableExtInt = (DRV_PL360_HAL_ENABLE_EXT_INT)drv_pl360_hal_enable_interrupt,
        .delay = (DRV_PL360_HAL_DELAY)drv_pl360_hal_delay,
        .sendBootCmd = (DRV_PL360_HAL_SEND_BOOT_CMD)drv_pl360_hal_send_boot_cmd,
        .sendWrRdCmd = (DRV_PL360_HAL_SEND_WRRD_CMD)drv_pl360_hal_send_wrrd_cmd,
    };

    extern uint8_t pl360_bin_start;
    extern uint8_t pl360_bin_end;

    DRV_PL360_INIT drvPL360InitData = {
        .pl360Hal = &drvPL360HalAPI,
        .numClients = DRV_PL360_CLIENTS_NUMBER_IDX,  
        .plcProfile = DRV_PL360_PLC_PROFILE,
        .binStartAddress = (uint32_t)&pl360_bin_start,
        .binEndAddress = (uint32_t)&pl360_bin_end,
        .secure = DRV_PL360_SECURE,       
    };

    sysObjDrvPL3600 = DRV_PL360_Initialize(DRV_PL360_INDEX_0, (SYS_MODULE_INIT *)&drvPL3600InitData);
    // Register Callback function is mandatory to handle PL360 interruption 
    PIO_PinInterruptCallbackRegister(DRV_PL360_EXT_INT_PIN, DRV_PL360_ExternalInterruptHandler, sysObj.drvPL360);

    </code>

  Remarks:
    This routine must be called before any other DRV_PL360 routine is called.
    This routine should only be called once during system initialization.
    This routine will block for hardware access.
*/

SYS_MODULE_OBJ DRV_PL360_Initialize( const SYS_MODULE_INDEX index, const SYS_MODULE_INIT * const init);

// *****************************************************************************
/* Function:
    DRV_HANDLE DRV_PL360_Open
    (
        const SYS_MODULE_INDEX index,
        const DRV_PL360_BOOT_DATA_CALLBACK callback
    )

  Summary:
    Opens the specified PL360 driver instance and returns a handle to it.

  Description:
    This routine opens the specified PL360 driver instance and provides a
    handle that must be provided to all other client-level operations to
    identify the caller and the instance of the driver. The ioIntent
    parameter defines how the client interacts with this driver instance.

    This driver is a single client driver, so DRV_PL360_Open API should be
    called only once until driver is closed.

  Precondition:
    Function DRV_PL360_Initialize must have been called before calling this
    function.

  Parameters:
    index  -    Identifier for the object instance to be opened

    callback -  Boot Data Callback Function Pointer. In case of use NULL, 
                .binStartAddress and .binEndAddress fields must be configured 
                in initialization data DRV_PL360_INIT.

  Returns:
    If successful, the routine returns a valid open-instance handle (a number
    identifying both the caller and the module instance).

    If an error occurs, the return value is DRV_HANDLE_INVALID. Error can occur
    - if the  driver has been already opened once and in use.
    - if the driver peripheral instance being opened is not initialized or is
      invalid.

  Example:
    <code>
    DRV_HANDLE handle;

    handle = DRV_PL360_Open(DRV_PL360_INDEX_0, NULL);
    if (handle == DRV_HANDLE_INVALID)
    {
        // Unable to open the driver
        // May be the driver is not initialized
    }
    </code>

  Remarks:
    The handle returned is valid until the DRV_PL360_Close routine is called.
    This routine will NEVER block waiting for hardware.
*/
DRV_HANDLE DRV_PL360_Open(const SYS_MODULE_INDEX index, 
        const DRV_PL360_BOOT_DATA_CALLBACK callback);

// *****************************************************************************
/* Function:
    void DRV_PL360_Close( const DRV_Handle handle )

  Summary:
    Closes opened-instance of the PL360 driver.

  Description:
    This routine closes opened-instance of the PL360 driver, invalidating the
    handle. A new handle must be obtained by calling DRV_PL360_Open
    before the caller may use the driver again.

  Precondition:
    DRV_PL360_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle -    A valid open-instance handle, returned from the driver's
                open routine

  Returns:
    None.

  Example:
    <code>
    // 'handle', returned from the DRV_PL360_Open

    DRV_PL360_Close(handle);

    </code>

  Remarks:
    None.
*/
void DRV_PL360_Close(const DRV_HANDLE handle);

// *****************************************************************************
/* Function:
    void DRV_PL360_Send( DRV_Handle handle, DRV_PL360_TRANSMISSION_OBJ *transmitObj )

  Summary:
    Allows a client to transmit data through Power Line (PLC).

  Description:
    This routine sends a new data message through PLC using the PL360 driver.

  Precondition:
    DRV_PL360_Open must have been called to obtain a valid opened device handle.
    DRV_PL360_TRANSMISSION_OBJ must be configured before to send any data message.

  Parameters:
    handle -    A valid open-instance handle, returned from the driver's
                open routine

    transmitObj - Pointer to the object to transmit.

  Returns:
    None.

  Example:
    <code>
    // 'handle', returned from the DRV_PL360_Open
    
    DRV_PL360_TRANSMISSION_OBJ transmitObj;

    // It is mandatory to set all transmission parameters using DRV_PL360_TRANSMISSION_OBJ
    // Those parameters depends on the PLC profile in use.
    // Local function implemented in the user application
    _setupTransmissionParameters();
    
    memcpy(transmitObj.data, src_buf, data_len);
    transmitObj.length = data_len;

    // Rest of the paramenters in transmitObj were configured previously

    DRV_PL360_Send(handle, &transmitObj);

    </code>

  Remarks:
    None.
*/
void DRV_PL360_Send(const DRV_HANDLE handle, DRV_PL360_TRANSMISSION_OBJ *transmitObj);

// *****************************************************************************
/* Function:
    void DRV_PL360_PIBGet( const DRV_HANDLE handle, DRV_PL360_PIB_OBJ *pibObj )

  Summary:
    Allows a client to get information from PL360 device about PLC information base (PIB).

  Description:
    This routine get PLC data information from the PL360 device.

  Precondition:
    DRV_PL360_Open must have been called to obtain a valid opened device handle.
    DRV_PL360_PIB_OBJ must be configured before to get information.

  Parameters:
    handle -    A valid open-instance handle, returned from the driver's
                open routine

    pibObj -    Pointer to the PIB oobject to get.

  Returns:
    None.

  Example:
    <code>
    // 'handle', returned from the DRV_PL360_Open
    
    DRV_PL360_PIB_OBJ pibObj;
    uint32_t version;
    
    pibObj.pData = &version;
    pibObj.length = 4;
    pibObj.id = PL360_ID_VERSION_NUM;
    pibObj.sync = true;

    DRV_PL360_PIBGet(handle, &pibObj);

    </code>

  Remarks:
    None.
*/
bool DRV_PL360_PIBGet(const DRV_HANDLE handle, DRV_PL360_PIB_OBJ *pibObj);

// *****************************************************************************
/* Function:
    void DRV_PL360_PIBSet( const DRV_HANDLE handle, DRV_PL360_PIB_OBJ *pibObj )

  Summary:
    Allows a client to set information to PL360 device about PLC information base (PIB).

  Description:
    This routine set PLC data information to the PL360 device.

  Precondition:
    DRV_PL360_Open must have been called to obtain a valid opened device handle.
    DRV_PL360_PIB_OBJ must be configured before to set information.

  Parameters:
    handle -    A valid open-instance handle, returned from the driver's
                open routine

    pibObj -    Pointer to the PIB object to set.

  Returns:
    None.

  Example:
    <code>
    // 'handle', returned from the DRV_PL360_Open
    
    DRV_PL360_PIB_OBJ pibObj;
    uint8_t autoMode
    uint8_t impedance;
 
    // Disable AUTODETECT impedance mode
    autoMode = 0;
 
    // Set VLO impedance mode
    impedance = VLO_STATE; 
 
    pibObj.pData = &autoMode;
    pibObj.length = 1;
    pibObj.id = PL360_ID_CFG_AUTODETECT_IMPEDANCE;
    DRV_PL360_PIBSet(handle, &pibObj);
 
    pibObj.pData = &impedance;
    pibObj.length = 1;
    pibObj.id = PL360_ID_CFG_IMPEDANCE;
    DRV_PL360_PIBSet(handle, &pibObj);

    </code>

  Remarks:
    None.
*/
bool DRV_PL360_PIBSet(const DRV_HANDLE handle, DRV_PL360_PIB_OBJ *pibObj);

// *****************************************************************************
/* Function:
    void DRV_PL360_DataCfmCallbackRegister( 
        const DRV_HANDLE handle, 
        const DRV_PL360_DATA_CFM_CALLBACK callback, 
        const uintptr_t context 
    );

  Summary:
    Allows a client to identify a data confirm event handling function for the driver
    to call back when the requested transmission has finished.

  Description:
    This function allows a client to register a PL360 data confirm event handling 
    function with the driver to call back when a data confirmation PLC event occurs.

    The event handler should be set before the client submits any transmission
    requests that could generate events. The callback once set, persists
    until the client closes the driver or sets another callback (which
    could be a "NULL" pointer to indicate no callback).

  Precondition:
    DRV_PL360_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - A valid open-instance handle, returned from the driver's open routine.

    callback - Pointer to the callback function.

    context - The value of parameter will be passed back to the client unchanged, 
    when the callback function is called.  It can be used to identify any client 
    specific data object that identifies the instance of the client module (for example, 
    it may be a pointer to the client module's state structure).

  Returns:
    None.

  Example:
    <code>
     // Event is received when the transmission is finished
    void APP_PL360_Data_Cfm_callback(DRV_PL360_DATA_CFM_OBJ *cfmObj, uintptr_t context)
    {
        // The context handle was set to an application specific
        // object. It is now retrievable easily in the event handler.
        MY_APP_OBJ myAppObj = (MY_APP_OBJ *) context;

        if (cfmObj->result == DRV_PL360_TX_RESULT_PROCESS) {
          // This means the data was transferred successfully.
        } else {
          // Error handling here.
        }
    }
      
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    DRV_PL360_TRANSMISSION_OBJ myTransmissionObj;    

    // myHandle is the handle returned from DRV_PL360_Open API.

    // Client registers a data confirm callback with driver. This is done once

    DRV_PL360_DataCfmCallbackRegister( myHandle, APP_PL360_Data_Cfm_callback, (uintptr_t)&myAppObj );

    DRV_PL360_Send(myHandle, myTransmissionObj) == false);
   
    </code>

*/

void DRV_PL360_DataCfmCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_PL360_DATA_CFM_CALLBACK callback, 
    const uintptr_t context 
);

// *****************************************************************************
/* Function:
    void DRV_PL360_DataIndCallbackRegister( 
        const DRV_HANDLE handle, 
        const DRV_PL360_DATA_IND_CALLBACK callback, 
        const uintptr_t context 
    );

  Summary:
    Allows a client to identify a data indication event handling function for the driver
    to call back when a data reception has finished.

  Description:
    This function allows a client to register a PL360 data indication event handling 
    function with the driver to call back when a data reception PLC event occurs.

    The event handler should be set before the client submits any transmission
    requests that could generate events. The callback once set, persists
    until the client closes the driver or sets another callback (which
    could be a "NULL" pointer to indicate no callback).

  Precondition:
    DRV_PL360_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle   - A valid open-instance handle, returned from the driver's open routine.

    callback - Pointer to the callback function.

    context - The value of parameter will be passed back to the client unchanged, 
    when the callback function is called.  It can be used to identify any client 
    specific data object that identifies the instance of the client module (for example, 
    it may be a pointer to the client module's state structure).

  Returns:
    None.

  Example:
    <code>

    void APP_PL360_Data_Ind_callback(DRV_PL360_DATA_IND_OBJ *indObj, uintptr_t context)
    {
        // The context handle was set to an application specific
        // object. It is now retrievable easily in the event handler.
        MY_APP_OBJ myAppObj = (MY_APP_OBJ *) context;

        // Reception handling here.

    }
      
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    // myHandle is the handle returned from DRV_PL360_Open API.

    // Client registers a data confirm callback with driver. This is done once

    DRV_PL360_DataIndCallbackRegister( myHandle, APP_PL360_Data_Ind_callback, (uintptr_t)&myAppObj );

    // Event is received when PLC data is receiving.
    
    </code>

*/

void DRV_PL360_DataIndCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_PL360_DATA_IND_CALLBACK callback, 
    const uintptr_t context 
);

// *****************************************************************************
/* Function:
    void DRV_PL360_ExceptionCallbackRegister( 
        const DRV_HANDLE handle, 
        const DRV_PL360_EXCEPTION_CALLBACK callback, 
        const uintptr_t context 
    );

  Summary:
    Allows a client to identify an exception event handling function for the driver
    to call back when some error occurs through PL360 device communication.

  Description:
    This function allows a client to register a PL360 exception event handling 
    function with the driver to call back when a communication SPI error occurs.

    The event handler should be set before the client submits any transmission
    requests that could generate events. The callback once set, persists
    until the client closes the driver or sets another callback (which
    could be a "NULL" pointer to indicate no callback).

  Precondition:
    DRV_PL360_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - A valid open-instance handle, returned from the driver's open routine.

    callback - Pointer to the callback function.

    context - The value of parameter will be passed back to the client unchanged, 
    when the callback function is called.  It can be used to identify any client 
    specific data object that identifies the instance of the client module (for example, 
    it may be a pointer to the client module's state structure).

  Returns:
    None.

  Example:
    <code>

    void APP_PL360_Exception_callback(DRV_PL360_EXCEPTION_OBJ *exceptionObj, uintptr_t context)
    {
        // The context handle was set to an application specific
        // object. It is now retrievable easily in the event handler.
        MY_APP_OBJ myAppObj = (MY_APP_OBJ *) context;

        // Exception handling here.

    }
      
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    // myHandle is the handle returned from DRV_PL360_Open API.

    // Client registers a data confirm callback with driver. This is done once

    DRV_PL360_ExceptionCallbackRegister( myHandle, APP_PL360_Exception_callback, (uintptr_t)&myAppObj );

    // Event is received when PLC data is receiving.
    </code>

*/

void DRV_PL360_ExceptionCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_PL360_EXCEPTION_CALLBACK callback, 
    const uintptr_t context 
);

// *****************************************************************************
/* Function:
    void DRV_PL360_ExternalInterruptHandler( 
        const PIO_PIN pin, 
        const uintptr_t context 
    );

  Summary:
    Allows application to register callback for PL360 Interrupt pin.

  Description:
    This function allows a client to register a callback function to handle 
    PL360 interrupt.
    
  Precondition:
    DRV_PL360_Initialize must have been called to obtain a valid system object.

  Returns:
    None.

  Remarks:
    See plib_pio.h for more details.

  Example:
    <code>
      
    // Initialize PL360 Driver Instance
    sysObj.drvPL360 = DRV_PL360_Initialize(DRV_PL360_INDEX, (SYS_MODULE_INIT *)&drvPL360InitData);
    // Register Callback function to handle PL360 interruption
    PIO_PinInterruptCallbackRegister(DRV_PL360_EXT_INT_PIN, DRV_PL360_ExternalInterruptHandler, sysObj.drvPL360);

    </code>

*/

void DRV_PL360_ExternalInterruptHandler( PIO_PIN pin, uintptr_t context );

// *************************************************************************
/* Function:
    SYS_STATUS DRV_PL360_Status( const SYS_MODULE_INDEX index )

  Summary:
    Gets the current status of the PL360 driver module.

  Description:
    This routine provides the current status of the PL360 driver module.

  Preconditions:
    Function DRV_PL360_Initialize should have been called before calling
    this function.

  Parameters:
    index   -  Identifier for the instance used to initialize driver

  Returns:
    SYS_STATUS_READY - Indicates that the driver is ready and accept
                       requests for new operations.

    SYS_STATUS_UNINITIALIZED - Indicates the driver is not initialized.
  
    SYS_STATUS_ERROR - Indicates the driver is not initialized correctly.
  
    SYS_STATUS_BUSY - Indicates the driver is initializing.

  Example:
    <code>
    SYS_STATUS status;

    status = DRV_PL360_Status(DRV_PL360_INDEX);
    </code>

  Remarks:
    None.
*/

SYS_STATUS DRV_PL360_Status( const SYS_MODULE_INDEX index );

/***************************************************************************
  Function:
       void DRV_PL360_Tasks( SYS_MODULE_OBJ object )
    
  Summary:
    Maintains the driver's state machine.

  Description:
    This function is used to maintain the driver's internal state machine.

  Precondition:
    The DRV_PL360_Initialize routine must have been called for the
    specified PL360 driver instance.

  Parameters:
    - object -  Object handle for the specified driver instance (returned from
                DRV_PL360_Initialize)
  Returns:
    None
  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_PL360_Initialize
    
    while (true)
    {
        DRV_PL360_Tasks (object);
    
        // Do other tasks
    }
    </code>

  Remarks:
    - This function is normally not called directly by an application. It is
      called by the system's Tasks routine (SYS_Tasks)
    - This function will never block or access any resources that may cause
      it to block.                        
  ***************************************************************************/

void DRV_PL360_Tasks( SYS_MODULE_OBJ object );

#ifdef __cplusplus
}
#endif

#include "driver/pl360/src/drv_pl360_local.h"

#endif // #ifndef _DRV_PL360_H
/*******************************************************************************
 End of File
*/
