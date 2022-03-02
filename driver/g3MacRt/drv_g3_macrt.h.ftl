/*******************************************************************************
  DRV_G3_MACRT_MACRT Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    drv_g3_macrt.h

  Summary:
    MAC RT PLC Library Interface header.

  Description:
    The G3 MAC RT Driver Library provides a interface to access the 
    PLC transceiver.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _DRV_G3_MACRT_H
#define _DRV_G3_MACRT_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdbool.h>
#include "driver/driver.h"
#include "system/system.h"
#include "drv_g3_macrt_comm.h"
#include "drv_g3_macrt_definitions.h"
#include "driver/plc/common/drv_plc_hal.h"
#include "driver/plc/common/drv_plc_boot.h"

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

/* DRV_PLC Transfer Errors

 Summary:
    Defines the data type for G3 MAC RT Driver transfer errors.

 Description:
    This will be used to indicate the error of the last SPI transfer.

 Remarks:
    None.
*/

typedef enum
{
    /* SPI has detected an unexpected status, reset is recommended */
    DRV_G3_MACRT_EXCEPTION_UNEXPECTED_KEY,

    /* SPI critical error */
    DRV_G3_MACRT_EXCEPTION_CRITICAL_ERROR,

    /* Device has been reseted by Debugging tool */
    DRV_G3_MACRT_EXCEPTION_DEBUG,

    /* Device has been reseted */
    DRV_G3_MACRT_EXCEPTION_RESET

} DRV_G3_MACRT_EXCEPTION; 

// *****************************************************************************
/* G3 MAC RT Driver Transmission Confirm Event Handler Function Pointer

   Summary
    Pointer to a MAC RT Driver Transmission Confirm Event handler function

   Description
    This data type defines the required function signature for the MAC RT driver
    transmission confirm event handling callback function. A client must register 
    a pointer using the callback register function whose function signature 
    (parameter and return value types) match the types specified by this function 
    pointer in order to receive transfer related event calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    cfmObj -            Pointer to the object containing any data necessary to
                        identify the result of the last transmission.

  Returns:
    None.

  Example:
    <code>
    void APP_MyTXCfmEventHandler( MAC_RT_TX_CFM_OBJ *cfmObj )
    {
        switch(cfmObj->status)
        {
            case MAC_RT_STATUS_SUCCESS:
                // Transmission result: already in process
                break;   
            case MAC_RT_STATUS_CHANNEL_ACCESS_FAILURE:
                // Transmission result: CSMA failure
                break;   
            case MAC_RT_STATUS_NO_ACK:
                // Transmission result: ACK failure
                break;
        }
    }
    </code>

  Remarks:
    - If the result field is MAC_RT_STATUS_SUCCESS, it means that the data was
      transferred successfully.

    - Otherwise, it means that the data was not transferred successfully.

*/

typedef void ( *DRV_G3_MACRT_TX_CFM_CALLBACK )( MAC_RT_TX_CFM_OBJ *cfmObj );

// *****************************************************************************
/* G3 MAC RT Driver Reception Event Handler Function Pointer

   Summary
    Pointer to a G3 MAC RT Driver Reception Event handler function

   Description
    This data type defines the required function signature for the MAC RT driver
    reception event handling callback function. A client must register a pointer 
    using the callback register function whose function signature (parameter 
    and return value types) match the types specified by this function pointer 
    in order to receive transfer related event calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    pData -             Pointer to the data content.
 
    length -            Length of the received data

  Returns:
    None.

  Example:
    <code>
    void APP_MyRxEventHandler( uint8_t *pData, uint16_t length )
    {
        // Check length of the new message
        if (length) {
          // extract data from received message
          memcpy(appData->dataRxBuffer, pData, length);
        }
    </code>

*/

typedef void ( *DRV_G3_MACRT_DATA_IND_CALLBACK )( uint8_t *pData, uint16_t length );

// *****************************************************************************
/* G3 MAC RT Driver Event Handler Function Pointer to get parameters from the
   last received message

   Summary
    Pointer to a G3 MAC RT Driver Event handler function to get parameters 
    from the last received message

   Description
    This data type defines the required function signature for the MAC RT driver
    reception event handling callback function. A client must register a pointer 
    using the callback register function whose function signature (parameter 
    and return value types) match the types specified by this function pointer 
    in order to receive transfer related event calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    pParameters -       Pointer to the characterization parameters of the last 
                        received message
 
  Returns:
    None.

  Example:
    <code>
    void APP_MyRxParamsHandler( MAC_RT_RX_PARAMETERS_OBJ *pParameters )
    {
        // extract all parameters from received message
        memcpy(appData->rxParams, (uint8_t *)pParameters, sizeof(MAC_RT_RX_PARAMETERS_OBJ));
        }
    </code>

*/

typedef void ( *DRV_G3_MACRT_RX_PARAMS_IND_CALLBACK )( MAC_RT_RX_PARAMETERS_OBJ *pParameters );

// *****************************************************************************
/* G3 MAC RT Sniffer Event Handler Function Pointer

   Summary
    Pointer to a G3 MAC RT Sniffer Reception Event handler function

   Description
    This data type defines the required function signature for the MAC RT driver
    sniffer event handling callback function. A client must register a pointer 
    using the callback register function whose function signature (parameter 
    and return value types) match the types specified by this function pointer 
    in order to receive transfer related event calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    pData -             Pointer to the data content.
 
    length -            Length of the received data

  Returns:
    None.

  Example:
    <code>
    void APP_MyMacRtMacSnifferHandler( uint8_t *pData, uint16_t length )
    {
        // Check length of the new PLC message
        if (length) {
          // extract data from received message
          memcpy(appData->dataMacRtMacSnifferBuffer, pData, length);
        }
    }
    </code>

*/

typedef void ( *DRV_G3_MACRT_MAC_SNIFFER_IND_CALLBACK )( uint8_t *pData, uint16_t length );

// *****************************************************************************
/* G3 MAC RT Communication Status Handler Function Pointer

   Summary
    Pointer to a G3 MAC RT Communication Status Event handler function

   Description
    This data type defines the required function signature for the MAC RT driver
    comm status event handling callback function. A client must register a pointer 
    using the callback register function whose function signature (parameter 
    and return value types) match the types specified by this function pointer 
    in order to receive transfer related event calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    pData -             Pointer to the data content.

  Returns:
    None.

  Example:
    <code>
    void APP_MyPLCMacRtCommStatusHandler( uint8_t *pData )
    {
        // Check length of the new PLC message
        if (length) {
          // extract data from received message
          memcpy(appData->dataMacCommStatusBuffer, pData, MAC_RT_COMM_STATUS_SIZE));
        }
    }
    </code>

*/

typedef void ( *DRV_G3_MACRT_COMM_STATUS_IND_CALLBACK )( uint8_t *pData );

// *****************************************************************************
/* G3 MAC RT PHY Sniffer Event Handler Function Pointer

   Summary
    Pointer to a G3 MAC RT PHY Sniffer Reception Event handler function

   Description
    This data type defines the required function signature for the MAC RT driver
    sniffer event handling callback function. A client must register a pointer 
    using the callback register function whose function signature (parameter 
    and return value types) match the types specified by this function pointer 
    in order to receive transfer related event calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    pData -             Pointer to the data content.
 
    length -            Length of the received data

  Returns:
    None.

  Example:
    <code>
    void APP_MyMacRtPhySnifferHandler( uint8_t *pData, uint16_t length )
    {
        // Check length of the new PLC message
        if (length) {
          // extract data from received message
          memcpy(appData->dataMacRtPhySnifferBuffer, pData, length);
        }
    }
    </code>

*/

typedef void ( *DRV_G3_MACRT_PHY_SNIFFER_IND_CALLBACK )( uint8_t *pData, uint16_t length );

// *****************************************************************************
/* G3 MAC RT Driver Exceptions Event Handler Function Pointer

   Summary
    Pointer to a G3 MAC RT Driver Exceptions Event handler function

   Description
    This data type defines the required function signature for the PLC driver
    exceptions event handling callback function. A client must register a pointer 
    using the callback register function whose function signature (parameter 
    and return value types) match the types specified by this function pointer 
    in order to receive transfer related event calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    exception -         Value identifying the exception code which occurs during
                        SPI transfer.

  Returns:
    None.

  Example:
    <code>
    void APP_MyExceptionEventHandler( DRV_G3_MACRT_EXCEPTION exception )
    {
        switch (exceptionObj) 
        {
            case DRV_G3_MACRT_EXCEPTION_UNEXPECTED_KEY:
                // SPI has detected an unexpected status, reset is recommended
                break;

            case DRV_G3_MACRT_EXCEPTION_CRITICAL_ERROR:
                // SPI critical error in last transfer. Bootloader task has failured.
                break;

            case DRV_G3_MACRT_EXCEPTION_DEBUG:
                // PLC transceiver has been reseted by Debugging tool
                break;

            case DRV_G3_MACRT_EXCEPTION_RESET:
                // PLC transceiver has been reseted
                break;

            default:
                // SPI has detected an unexpected status, reset is recommended
        }

        appData->macrt_exception = true;
    }
    </code>

*/

typedef void ( *DRV_G3_MACRT_EXCEPTION_CALLBACK )( DRV_G3_MACRT_EXCEPTION exception );

<#if DRV_PLC_SLEEP_MODE == true> 
// *****************************************************************************
/* G3 MAC RT Sleep Mode Disable Event Handler Function Pointer

   Summary
    Notifies when SLeep Mode is disabled and MAC RT driver is able to be used again.

   Description
    This data type defines the required function signature for the PLC driver
    sleep mode disable event handling callback function. A client must register 
	a pointer using the callback register function whose function signature 
	(parameter and return value types) match the types specified by this function 
	pointer in order to receive transfer related event calls back from the driver.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:

  Returns:
    None.

  Example:
    <code>
    void APP_MySleepDisableEventHandler( void )
    {
        // Do initial configuration of the application if needed
		
        appData.plc_sleep = false;
    }
    </code>

*/

typedef void ( *DRV_G3_MACRT_SLEEP_IND_CALLBACK )( void );

</#if>

// *****************************************************************************
// *****************************************************************************
// Section: DRV MAC RT Driver System Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ DRV_G3_MACRT_Initialize(
        const SYS_MODULE_INDEX index,
        const SYS_MODULE_INIT * const init
    )

  Summary:
    Initializes the MAC RT instance for the specified driver index.

  Description:
    This routine initializes the G3 MAC RT driver making it ready for
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
    SYS_MODULE_OBJ   sysObjDrvMACRT;

    DRV_PLC_PLIB_INTERFACE drvPLCPlib = {

        .spiPlibTransferSetup = (DRV_PLC_SPI_PLIB_TRANSFER_SETUP)SPI0_TransferSetup,
        .dmaChannelTx = SYS_DMA_CHANNEL_1,
        .dmaChannelRx  = SYS_DMA_CHANNEL_0,
        .spiAddressTx =  (void *)&(SPI0_REGS->SPI_TDR),
        .spiAddressRx  = (void *)&(SPI0_REGS->SPI_RDR),
        .spiClockFrequency = DRV_PLC_SPI_CLK,
        .ldoPin = DRV_PLC_LDO_EN_PIN, 
        .resetPin = DRV_PLC_RESET_PIN,
        .extIntPin = DRV_PLC_EXT_INT_PIN,
    };

    DRV_PLC_HAL_INTERFACE drvPLCHalAPI = {
        .plcPlib = &drvPLCPlib,
        .init = (DRV_PLC_HAL_INIT)drv_plc_hal_init,
        .setup = (DRV_PLC_HAL_SETUP)drv_plc_hal_setup,
        .reset = (DRV_PLC_HAL_RESET)drv_plc_hal_reset,
        .getCd = (DRV_PLC_HAL_GET_CD)drv_plc_hal_get_cd,
        .enableExtInt = (DRV_PLC_HAL_ENABLE_EXT_INT)drv_plc_hal_enable_interrupt,
        .delay = (DRV_PLC_HAL_DELAY)drv_plc_hal_delay,
        .sendBootCmd = (DRV_PLC_HAL_SEND_BOOT_CMD)drv_plc_hal_send_boot_cmd,
        .sendWrRdCmd = (DRV_PLC_HAL_SEND_WRRD_CMD)drv_plc_hal_send_wrrd_cmd,
    };

    extern uint8_t g3_macrt_bin_start;
    extern uint8_t g3_macrt_bin_end;

    DRV_G3_MACRT_INIT drvPLCMacRtInitData = {
        .plcHal = &drvPLCHalAPI,
        .numClients = DRV_G3_MACRT_CLIENTS_NUMBER_IDX,  
        .plcProfile = DRV_G3_MACRT_PLC_PROFILE,
        .binStartAddress = (uint32_t)&g3_macrt_bin_start,
        .binEndAddress = (uint32_t)&g3_macrt_bin_end,
        .secure = DRV_G3_MACRT_SECURE,       
    };

    sysObjDrvMACRT = DRV_G3_MACRT_Initialize(DRV_G3_MACRT_INDEX_0, (SYS_MODULE_INIT *)&drvPLCMacRtInitData);
    // Register Callback function is mandatory to handle PLC interruption 
    PIO_PinInterruptCallbackRegister(DRV_G3_MACRT_EXT_INT_PIN, DRV_G3_MACRT_ExternalInterruptHandler, sysObjDrvMACRT);

    </code>

  Remarks:
    This routine must be called before any other DRV_G3_MACRT routine is called.
    This routine should only be called once during system initialization.
    This routine will block for hardware access.
*/

SYS_MODULE_OBJ DRV_G3_MACRT_Initialize( const SYS_MODULE_INDEX index, const SYS_MODULE_INIT * const init);

// *****************************************************************************
/* Function:
    DRV_HANDLE DRV_G3_MACRT_Open
    (
        const SYS_MODULE_INDEX index,
        const DRV_PLC_BOOT_DATA_CALLBACK callback
    )

  Summary:
    Opens the specified MAC RT driver instance and returns a handle to it.

  Description:
    This routine opens the specified MAC RT driver instance and provides a
    handle that must be provided to all other client-level operations to
    identify the caller and the instance of the driver. 

    This driver is a single client driver, so DRV_G3_MACRT_Open API should be
    called only once until driver is closed.

  Precondition:
    Function DRV_G3_MACRT_Initialize must have been called before calling this
    function.

  Parameters:
    index  -    Identifier for the object instance to be opened

    callback -  Boot Data Callback Function Pointer. In case of use NULL, 
                .binStartAddress and .binEndAddress fields must be configured 
                in initialization data DRV_G3_MACRT_INIT.

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

    handle = DRV_G3_MACRT_Open(DRV_G3_MACRT_INDEX_0, NULL);
    if (handle == DRV_HANDLE_INVALID)
    {
        // Unable to open the driver
        // May be the driver is not initialized
    }
    </code>

  Remarks:
    The handle returned is valid until the DRV_G3_MACRT_Close routine is called.
    This routine will NEVER block waiting for hardware.
*/
DRV_HANDLE DRV_G3_MACRT_Open(const SYS_MODULE_INDEX index, 
        const DRV_PLC_BOOT_DATA_CALLBACK callback);

// *****************************************************************************
/* Function:
    void DRV_G3_MACRT_Close( const DRV_HANDLE handle )

  Summary:
    Closes opened-instance of the MAC RT driver.

  Description:
    This routine closes opened-instance of the MAC RT driver, invalidating the
    handle. A new handle must be obtained by calling DRV_G3_MACRT_Open
    before the caller may use the driver again.

  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle -    A valid open-instance handle, returned from the driver's
                open routine

  Returns:
    None.

  Example:
    <code>
    // 'handle', returned from the DRV_G3_MACRT_Open

    DRV_G3_MACRT_Close(handle);

    </code>

  Remarks:
    None.
*/
void DRV_G3_MACRT_Close(const DRV_HANDLE handle);

// *****************************************************************************
/* Function:
    void DRV_G3_MACRT_TxRequest( const DRV_HANDLE handle, uint8_t *pData, uint16_t length )

  Summary:
    Allows a client to transmit MAC RT data through Power Line (PLC).

  Description:
    This routine sends a new data message through PLC using the MAC RT driver.

  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device handle.
    DRV_G3_MACRT_MlmeSet must have been called before to send any data message.

  Parameters:
    handle -    A valid open-instance handle, returned from the driver's
                open routine

    pData -     Pointer to the data to transmit.

    length -    Length of the data to transmit in bytes.


  Returns:
    None.

  Example:
    <code>
    // 'handle', returned from the DRV_G3_MACRT_Open
    
    // It is mandatory to set all transmission parameters using DRV_G3_MACRT_MlmeSet
    // Those parameters depends on the PLC profile in use.
    // Local function implemented in the user application
    _setupTransmissionParameters();

    DRV_G3_MACRT_TxRequest(handle, appData.pData, appData.length);

    </code>

  Remarks:
    None.
*/
void DRV_G3_MACRT_TxRequest(const DRV_HANDLE handle, uint8_t *pData, uint16_t length);

// *****************************************************************************
/* Function:
    void DRV_G3_MACRT_PIBGet( const DRV_HANDLE handle, MAC_RT_PIB_OBJ *pibObj )

  Summary:
    Allows a client to get information from PLC transceiver about MAC RT and PHY
    information base (PIB).

  Description:
    This routine get MAC RT and PHY data information from the PLC transceiver.

  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device handle.
    MAC_RT_PIB_OBJ must be configured before to get information.

  Parameters:
    handle -    A valid open-instance handle, returned from the driver's
                open routine

    pibObj -    Pointer to the PIB object to get.

  Returns:
    None.

  Example:
    <code>
    // 'handle', returned from the DRV_G3_MACRT_Open
    
    MAC_RT_PIB_OBJ pibObj;
    uint32_t phyVersion;
    uint8_t macRtVersion[6];
    
    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_VERSION;
    pibObj.pData = &phyVersion;
    pibObj.length = 4;

    DRV_G3_MACRT_PIBGet(handle, &pibObj);
    
    pibObj.pib = MAC_RT_PIB_MANUF_MAC_RT_INTERNAL_VERSION;
    pibObj.index = 0;
    pibObj.pData = &macRtVersion;
    pibObj.length = 6;

    DRV_G3_MACRT_PIBGet(handle, &pibObj);

    </code>

  Remarks:
    None.
*/
MAC_RT_STATUS DRV_G3_MACRT_PIBGet(const DRV_HANDLE handle, MAC_RT_PIB_OBJ *pibObj);

// *****************************************************************************
/* Function:
    void DRV_G3_MACRT_PIBSet( const DRV_HANDLE handle, MAC_RT_PIB_OBJ *pibObj )

  Summary:
    Allows a client to set information to PLC transceiver about MAC RT and PHY 
    information base (PIB).

  Description:
    This routine set MAC RT and PHY data information to the PLC transceiver.

  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device handle.
    MAC_RT_PIB_OBJ must be configured before to set information.

  Parameters:
    handle -    A valid open-instance handle, returned from the driver's
                open routine

    pibObj -    Pointer to the PIB object to set.

  Returns:
    Mac Rt Status.

  Example:
    <code>
    // 'handle', returned from the DRV_G3_MACRT_Open
    
    MAC_RT_PIB_OBJ pibObj;
    uint8_t autoMode
    uint8_t impedance;
    uint8_t forcedRobo;
 
    // Disable AUTODETECT impedance mode
    autoMode = 0;
 
    // Set VLO impedance mode
    impedance = VLO_STATE; 
    
    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_CFG_AUTODETECT_BRANCH; 
    pibObj.pData = &autoMode;
    pibObj.length = 1;
    DRV_G3_MACRT_PIBSet(handle, &pibObj);
 
    pibObj.pib = MAC_RT_PIB_MANUF_PHY_PARAM;
    pibObj.index = PHY_PARAM_CFG_IMPEDANCE; 
    pibObj.pData = &impedance;
    pibObj.length = 1;
    DRV_G3_MACRT_PIBSet(handle, &pibObj);
 
    // Forces Modulation Type in every transmitted frame (1 - Force BPSK_ROBO)
    forcedRobo = 1; 
 
    pibObj.pib = MAC_RT_PIB_MANUF_FORCED_MOD_TYPE;
    pibObj.index = 0; 
    pibObj.pData = &forcedRobo;
    pibObj.length = 1;
    DRV_G3_MACRT_PIBSet(handle, &pibObj);

    </code>

  Remarks:
    None.
*/
MAC_RT_STATUS DRV_G3_MACRT_PIBSet(const DRV_HANDLE handle, MAC_RT_PIB_OBJ *pibObj);

// *****************************************************************************
/* Function:
    uint32_t DRV_G3_MACRT_GetTimerReference( const DRV_HANDLE handle )

  Summary:
    Get the internal timer reference from PLC transceiver in microseconds

  Description:
    PLC transceiver has an internal 32 bits counter which is used as internal
    time reference for all time calculations in PHY layer. This counter is 
    internally configured to be increased each microsecond. This function let 
    use this counter as accurate time reference to upper layers.
    
  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device 
    handle.

  Parameters:
    handle -    A valid open-instance handle, returned from the driver's
                open routine

  Returns:
    Internal timer reference from PLC transceiver in microseconds.

  Example:
    <code>
    // 'handle', returned from the DRV_G3_MACRT_Open
    uint32_t macRtTimerRef;

    macRtTimerRef = DRV_G3_MACRT_GetTimerReference(handle);

    </code>

  Remarks:
    None.
*/
uint32_t DRV_G3_MACRT_GetTimerReference(const DRV_HANDLE handle);

// *****************************************************************************
/* Function:
    void DRV_G3_MACRT_DataCfmCallbackRegister( 
        const DRV_HANDLE handle, 
        const DRV_G3_MACRT_TX_CFM_CALLBACK callback
    );

  Summary:
    Allows a client to identify a data confirm event handling function for the driver
    to call back when the requested transmission has finished.

  Description:
    This function allows a client to register a PLC data confirm event handling 
    function with the driver to call back when a data confirmation PLC event occurs.

    The event handler should be set before the client submits any transmission
    requests that could generate events. The callback once set, persists
    until the client closes the driver or sets another callback (which
    could be a "NULL" pointer to indicate no callback).

  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - A valid open-instance handle, returned from the driver's open routine.

    callback - Pointer to the callback function.

  Returns:
    None.

  Example:
    <code>
     // Event is received when the transmission is finished
    void APP_PLC_Tx_Cfm_callback(MAC_RT_TX_CFM_OBJ *cfmObj)
    {
        if (cfmObj->result == MAC_RT_STATUS_SUCCESS) {
          // This means the data was transferred successfully.
        } else {
          // Error handling here.
        }
    }
      
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    // myHandle is the handle returned from DRV_G3_MACRT_Open API.

    // Client registers a TX confirm callback with driver. This is done once

    DRV_G3_MACRT_TxCfmCallbackRegister( myAppObj.myHandle, APP_PLC_Tx_Cfm_callback );

    DRV_G3_MACRT_TxRequest(myAppObj.myHandle, myAppObj.pData, myAppObj.dataLength);
   
    </code>

*/

void DRV_G3_MACRT_TxCfmCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_TX_CFM_CALLBACK callback
);

// *****************************************************************************
/* Function:
    void DRV_G3_MACRT_DataIndCallbackRegister( 
        const DRV_HANDLE handle, 
        const DRV_G3_MACRT_DATA_IND_CALLBACK callback
    );

  Summary:
    Allows a client to identify a data indication event handling function for the driver
    to call back when a data reception has finished.

  Description:
    This function allows a client to register a MAC RT data indication event handling 
    function with the driver to call back when a data reception PLC event occurs.

    The event handler should be set before the client submits any transmission
    requests that could generate events. The callback once set, persists
    until the client closes the driver or sets another callback (which
    could be a "NULL" pointer to indicate no callback).

  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle   - A valid open-instance handle, returned from the driver's open routine.

    callback - Pointer to the callback function.

  Returns:
    None.

  Example:
    <code>

    void APP_PLC_Data_Ind_callback(uint8_t *pData, uint16_t length)
    {
        // Reception handling here.
        if (length)
        {
            memcpy(myAppObj->pDataRcv, pData, length);
            myAppObj->lengthDataRcv = length;
        }

    }
      
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    // myHandle is the handle returned from DRV_G3_MACRT_Open API.

    // Client registers a data confirm callback with driver. This is done once

    DRV_G3_MACRT_DataIndCallbackRegister( myAppObj.myHandle, APP_PLC_Data_Ind_callback );

    // Event is received when PLC data is receiving.
    
    </code>

*/

void DRV_G3_MACRT_DataIndCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_DATA_IND_CALLBACK callback
);

// *****************************************************************************
/* Function:
    void DRV_G3_MACRT_RxParamsIndCallbackRegister( 
        const DRV_HANDLE handle, 
        const DRV_G3_MACRT_RX_PARAMS_IND_CALLBACK callback
    );

  Summary:
    Allows a client to identify a parameters event handling function 
    for the driver to call back when a new data message is received.

  Description:
    This function allows a client to register a MAC RT Rx Params confirm event 
    handling function with the driver to call back when a new data message is 
    received.

    The event handler should be set as soon as possible in order to be notified 
    when new message is received. The callback once set, persists
    until the client closes the driver or sets another callback (which
    could be a "NULL" pointer to indicate no callback).

  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle   - A valid open-instance handle, returned from the driver's open routine.

    callback - Pointer to the callback function.

  Returns:
    None.

  Example:
    <code>

    void APP_PLC_Params_Ind_callback(MAC_RT_RX_PARAMETERS_OBJ *pParameters)
    {
        // Rx Parameters handling here.
    }
      
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    // myHandle is the handle returned from DRV_G3_MACRT_Open API.

    // Client registers a data confirm callback with driver. This is done once

    DRV_G3_MACRT_RxParamsIndCallbackRegister( myAppObj.myHandle, APP_PLC_Params_Ind_callback );

    // Event is received when PLC data is receiving.
    
    </code>

*/

void DRV_G3_MACRT_RxParamsIndCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_RX_PARAMS_IND_CALLBACK callback
);

// *****************************************************************************
/* Function:
    void DRV_G3_MACRT_MacSnifferCallbackRegister( 
        const DRV_HANDLE handle, 
        const DRV_G3_MACRT_MAC_SNIFFER_CALLBACK callback
		uint8_t* pDataBuffer
    );

  Summary:
    Allows enable G3 MAC RT Sniffer capabilities and set the data buffer 
    in which the content of the sniffer packet will be stored when a new PLC
    message is received.

  Description:
    This function allows a client to register a G3 MAC RT Sniffer event handling 
    function with the driver to call back when either a data reception event or 
    confirmation of the last transmission event occurs.
    The content of the sniffer data is compliant with MCHP PLC Sniffer tool 
    provided by Microchip.

    The callback once set, persists until the client closes the driver or sets 
    another callback (which could be a "NULL" pointer to indicate no callback).

  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle   - A valid open-instance handle, returned from the driver's open routine.

    callback - Pointer to the callback function.
	
	pDataBuffer - Pointer to buffer where sniffer message is stored. Data buffer should be 
	defined as MAC_RT_PHY_DATA_MAX_SIZE	to avoid memory overlaps.

  Returns:
    None.

  Example:
    <code>

    void APP_G3_MAC_RT_Mac_Sniffer_callback(uint8_t *pData, uint16_t length)
    {
        if (length)
        {
            // Mac Sniffer data handling here.
        }
    }
      
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    // myHandle is the handle returned from DRV_G3_MACRT_Open API.

    // Client registers a sniffer callback with MAC RT driver.

    DRV_G3_MACRT_MacSnifferCallbackRegister( myAppObj.myHandle, APP_G3_MAC_RT_Mac_Sniffer_callback,
		myAppObj.pMacSnfBuffer);

    // Event is received when PLC data is receiving.
    
    </code>

*/

void DRV_G3_MACRT_MacSnifferCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_MAC_SNIFFER_IND_CALLBACK callback,
	uint8_t* pDataBuffer
);

// *****************************************************************************
/* Function:
    void DRV_G3_MACRT_CommStatusCallbackRegister( 
        const DRV_HANDLE handle, 
        const DRV_G3_MACRT_COMM_STATUS_IND_CALLBACK callback
    );

  Summary:
    Allows enable G3 MAC RT Comm Status capabilities and set the data buffer 
    in which the content of the status packet will be stored.

  Description:
    This function allows a client to register a G3 MAC RT Comm Status event handling 
    function with the driver to call back when either a data reception event or 
    confirmation of the last transmission event occurs.

    The callback once set, persists until the client closes the driver or sets 
    another callback (which could be a "NULL" pointer to indicate no callback).

  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle   - A valid open-instance handle, returned from the driver's open routine.

    callback - Pointer to the callback function.

  Returns:
    None.

  Example:
    <code>

    void APP_G3_MAC_RT_Comm_Status_callback(uint8_t *pData)
    {
        // Extract Comm Status Data from data packet.
        
    }
      
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    // myHandle is the handle returned from DRV_G3_MACRT_Open API.

    // Client registers a Comm Status callback with MAC RT driver.

    DRV_G3_MACRT_CommStatusCallbackRegister( myAppObj.myHandle, APP_G3_MAC_RT_Comm_Status_callback );
    
    </code>

*/

void DRV_G3_MACRT_CommStatusCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_COMM_STATUS_IND_CALLBACK callback
);

// *****************************************************************************
/* Function:
    void DRV_G3_MACRT_PhySnifferCallbackRegister( 
        const DRV_HANDLE handle, 
        const DRV_G3_MACRT_PHY_SNIFFER_CALLBACK callback,
		uint8_t* pDataBuffer
    );

  Summary:
    Allows enable G3 MAC RT PHY Sniffer capabilities and set the data buffer 
    in which the content of the sniffer packet will be stored when a new PLC
    message is received.

  Description:
    This function allows a client to register a G3 MAC RT PHY Sniffer event handling 
    function with the driver to call back when either a data reception event or 
    confirmation of the last transmission event occurs.
    The content of the sniffer data is compliant with MCHP PLC Sniffer tool 
    provided by Microchip.

    The callback once set, persists until the client closes the driver or sets 
    another callback (which could be a "NULL" pointer to indicate no callback).

  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle   - A valid open-instance handle, returned from the driver's open routine.

    callback - Pointer to the callback function.
	
	pDataBuffer - Pointer to buffer where sniffer message is stored. Data buffer should be 
	defined as MAC_RT_DATA_MAX_SIZE	to avoid memory overlaps.

  Returns:
    None.

  Example:
    <code>

    void APP_G3_MAC_RT_Phy_Sniffer_callback(uint8_t *pData, uint16_t length)
    {
        if (length)
        {
            // Phy Sniffer data handling here.
        }
    }
      
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    // myHandle is the handle returned from DRV_G3_MACRT_Open API.

    // Client registers a Phy sniffer callback with MAC RT driver.

    DRV_G3_MACRT_PhySnifferCallbackRegister( myAppObj.myHandle, APP_G3_MAC_RT_Phy_Sniffer_callback,
		myAppObj.pPhySnfBuffer);

    // Event is received when PLC data is receiving.
    
    </code>

*/

void DRV_G3_MACRT_PhySnifferCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_PHY_SNIFFER_IND_CALLBACK callback,
	uint8_t* pDataBuffer
);

// *****************************************************************************
/* Function:
    void DRV_G3_MACRT_ExceptionCallbackRegister( 
        const DRV_HANDLE handle, 
        const DRV_G3_MACRT_EXCEPTION_CALLBACK callback,
    );

  Summary:
    Allows a client to identify an exception event handling function for the driver
    to call back when some error occurs through PLC transceiver communication.

  Description:
    This function allows a client to register a PLC exception event handling 
    function with the driver to call back when a communication SPI error occurs.

    The event handler should be set before the client submits any transmission
    requests that could generate events. The callback once set, persists
    until the client closes the driver or sets another callback (which
    could be a "NULL" pointer to indicate no callback).

  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - A valid open-instance handle, returned from the driver's open routine.

    callback - Pointer to the callback function.

  Returns:
    None.

  Example:
    <code>

    void APP_PLC_Exception_callback(DRV_G3_MACRT_EXCEPTION_OBJ *exceptionObj)
    {
        // Exception handling here.

    }
      
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    // myHandle is the handle returned from DRV_G3_MACRT_Open API.

    // Client registers a data confirm callback with driver. This is done once

    DRV_G3_MACRT_ExceptionCallbackRegister( myAppObj.myHandle, APP_PLC_Exception_callback );

    // Event is received when PLC data is receiving.
    </code>

*/

void DRV_G3_MACRT_ExceptionCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_EXCEPTION_CALLBACK callback
);

// *****************************************************************************
/* Function:
    void DRV_G3_MACRT_ExternalInterruptHandler( 
        const PIO_PIN pin, 
        const uintptr_t context 
    );

  Summary:
    Allows application to register callback for PLC Interrupt pin.

  Description:
    This function allows a client to register a callback function to handle 
    MAC RT interrupt.
    
  Precondition:
    DRV_G3_MACRT_Initialize must have been called to obtain a valid system object.

  Returns:
    None.

  Remarks:
    See plib_pio.h for more details.

  Example:
    <code>
      
    // Initialize G3 MAC RT Driver Instance
    sysObj.drvPLCMacRt = DRV_G3_MACRT_Initialize(DRV_G3_MACRT_INDEX, (SYS_MODULE_INIT *)&drvPLCMacRtInitData);
    // Register Callback function to handle PLC interruption
    PIO_PinInterruptCallbackRegister(DRV_G3_MACRT_EXT_INT_PIN, DRV_G3_MACRT_ExternalInterruptHandler, sysObj.drvPLCMacRt);

    </code>

*/

void DRV_G3_MACRT_ExternalInterruptHandler( PIO_PIN pin, uintptr_t context );

// *************************************************************************
/* Function:
    SYS_STATUS DRV_G3_MACRT_Status( const SYS_MODULE_INDEX index )

  Summary:
    Gets the current status of the PLC driver module.

  Description:
    This routine provides the current status of the PLC driver module.

  Preconditions:
    Function DRV_G3_MACRT_Initialize should have been called before calling
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

    status = DRV_G3_MACRT_Status(DRV_G3_MACRT_INDEX);
    </code>

  Remarks:
    None.
*/

SYS_STATUS DRV_G3_MACRT_Status( const SYS_MODULE_INDEX index );

//***************************************************************************
/*  Function:
       void DRV_G3_MACRT_Tasks( SYS_MODULE_OBJ object )
    
  Summary:
    Maintains the driver's state machine.

  Description:
    This function is used to maintain the driver's internal state machine.

  Precondition:
    The DRV_G3_MACRT_Initialize routine must have been called for the
    specified PLC driver instance.

  Parameters:
    - object -  Object handle for the specified driver instance (returned from
                DRV_G3_MACRT_Initialize)
  Returns:
    None
  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_G3_MACRT_Initialize
    
    while (true)
    {
        DRV_G3_MACRT_Tasks (object);
    
        // Do other tasks
    }
    </code>

  Remarks:
    - This function is normally not called directly by an application. It is
      called by the system's Tasks routine (SYS_Tasks)
    - This function will never block or access any resources that may cause
      it to block.                        
  */

void DRV_G3_MACRT_Tasks( SYS_MODULE_OBJ object );

<#if DRV_PLC_SLEEP_MODE == true> 
// *****************************************************************************
/* Function:
	void DRV_G3_MACRT_SleepDisableCallbackRegister( 
	    const DRV_HANDLE handle, 
	    const DRV_G3_MACRT_SLEEP_CALLBACK callback
    );

  Summary:
    Allows a client to identify an sleep mode disable event handling function 
	for the driver to call back when the PLC driver is active again.

  Description:
    This function allows a client to register a PLC sleep mode disable event handling 
    function with the driver to call back when sleep mode is disabled and PLC driver 
	has been restarted.

    The event handler should be set before the client submits any transmission
    requests that could generate events. The callback once set, persists
    until the client closes the driver or sets another callback (which
    could be a "NULL" pointer to indicate no callback).

  Precondition:
    DRV_G3_MACRT_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - A valid open-instance handle, returned from the driver's open routine.

    callback - Pointer to the callback function.

  Returns:
    None.

  Example:
    <code>

    void APP_PLC_SleepModeDisableCb( void )
    {
        // Sleep Disable handling here.

    }
      
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    // myHandle is the handle returned from DRV_G3_MACRT_Open API.

    // Client registers a data confirm callback with driver. This is done once

    DRV_G3_MACRT_SleepDisableCallbackRegister( myAppObj.myHandle, APP_PLC_SleepModeDisableCb );

    // Event is received when PLC device comes out of sleep mode and becomes active again.
    </code>

*/

void DRV_G3_MACRT_SleepIndCallbackRegister( 
    const DRV_HANDLE handle, 
    const DRV_G3_MACRT_SLEEP_IND_CALLBACK callback
);

//***************************************************************************
/*
  Function:
       void DRV_G3_MACRT_Sleep( const DRV_HANDLE handle, bool enable )
    
  Summary:
    PLC Sleep mode management.

  Description:
    This function disables PLC interrupts before going to sleep, and will leave
    them disabled upon return. If there is any PLC transmission in progress, 
    sleep mode will be enabled after the notification of the TX confirmation.

  Precondition:
    The DRV_G3_MACRT_Initialize routine must have been called for the
    specified PLC driver instance.

  Parameters:
    - object -  Object handle for the specified driver instance (returned from
                DRV_G3_MACRT_Initialize)
    - enable -  Set True to enter in sleep mode. Set False to exit from sleep mode.
  Returns:
    None.
  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_G3_MACRT_Initialize
    
    while (true)
    {
      if (sleep_condition)
      {
        DRV_G3_MACRT_Sleep (object, true);
      }
    }
    </code>

  Remarks:
    - This function will never block or access any resources that may cause
      it to block.                        
  */

void DRV_G3_MACRT_Sleep( const DRV_HANDLE handle, bool enable );

</#if> 

<#if DRV_PLC_MODE == "PL460">
//***************************************************************************
/*
  Function:
       void DRV_G3_MACRT_Enable_TX( const DRV_HANDLE handle, bool enable )
    
  Summary:
    Enable PLC transmission.

  Description:
    This function allows a client to enable or disable the PLC tranmission.
    If there is any transmission on going, it will be cancelled and notified
    through the TX confirmation callback. 
    
  Precondition:
    The DRV_G3_MACRT_Initialize routine must have been called for the
    specified PLC driver instance.

  Parameters:
    - object -  Object handle for the specified driver instance (returned from
                DRV_G3_MACRT_Initialize)
    - enable -  Set True to enable PLC tranmission. Set False to disable it.
  Returns:
    None.
  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_G3_MACRT_Initialize
    
    while (true)
    {
        ...
        if (cancel_tx)
        {
            DRV_G3_MACRT_Enable_TX (object, false);
        }
        else
        {
            DRV_G3_MACRT_Enable_TX (object, true);
        }
        ...
    }
    </code>
                   
  */

void DRV_G3_MACRT_Enable_TX( const DRV_HANDLE handle, bool enable );

</#if> 
#ifdef __cplusplus
}
#endif

#include "driver/plc/g3MacRt/drv_g3_macrt_local.h"

#endif // #ifndef _DRV_G3_MACRT_H
/*******************************************************************************
 End of File
*/
