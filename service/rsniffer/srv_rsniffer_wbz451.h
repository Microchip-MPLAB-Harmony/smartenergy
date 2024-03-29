/*******************************************************************************
  RF PHY Sniffer Serialization Header File

  Company:
    Microchip Technology Inc.

  File Name:
    srv_rsniffer.h

  Summary:
    RF PHY sniffer serialization interface header file.

  Description:
    The RF PHY sniffer serialization provides a service to format messages
    through serial connection in order to communicate with Hybrid Sniffer Tool
    provided by Microchip. This file provides the interface definition for the
    RF PHY sniffer serialization.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2024, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
//DOM-IGNORE-END

#ifndef SRV_RSNIFFER_H    // Guards against multiple inclusion
#define SRV_RSNIFFER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
/* PAR RF PHY Data information

  Summary:
    Defines the data necessary to build the serial message when receiving RF data.

  Description:
    This data type defines the data needed by RF Sniffer to be serialized.

  Remarks:
    None.
*/

typedef struct
{
    /* Time counter when the packet has been received/transmitted */
    uint64_t timeIniCount;

    /* Packet duration */
    uint32_t durationCount;

    /* Pointer to RF PHY data payload */
    uint8_t *pData;

    /* Number of symbols of the payload */
    uint16_t paySymbols;

    /* Payload length in bytes */
    uint16_t payloadLen;

    /* RSSI value in dBm */
    int8_t rssi;

} SRV_RSNIFFER_PHY_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: RF PHY Sniffer Serialization Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SRV_RSNIFFER_SerialRxMessage ( SRV_RSNIFFER_PHY_DATA *rxData, size_t *pMsgLen )

  Summary:
    Serializes a received RF frame along with its parameters.

  Description:
    This function takes an object containing a RF frame and its related
    parameters and serializes it in a buffer for further transmission through
    serial interface.

  Precondition:
    None.

  Parameters:
    rxData     - Pointer to RF reception data containing the frame and
                 parameters
    pMsgLen    - Pointer to sniffer message length in bytes (output)

  Returns:
    Pointer to sniffer message to be sent through serial interface.

  Example:
    <code>


    </code>

  Remarks:
    None.
*/

uint8_t* SRV_RSNIFFER_SerialRxMessage ( SRV_RSNIFFER_PHY_DATA *rxData, size_t *pMsgLen );

// *****************************************************************************
/* Function:
    uint8_t* SRV_RSNIFFER_SerialCfmMessage ( SRV_RSNIFFER_PHY_DATA *txData, size_t *pMsgLen )

  Summary:
    Serializes a transmitted RF frame along with its parameters.

  Description:
    This function serializes the RF transmitted packet in a buffer for further transmission
    through serial interface.

  Precondition:
    None.

  Parameters:
    rxData     - Pointer to RF transmission data containing the frame and
                 parameters
    pMsgLen    - Pointer to sniffer message length in bytes (output)

  Returns:
    Pointer to sniffer message to be sent through serial interface.

  Example:
    <code>


    </code>

  Remarks:
    None.
*/

uint8_t* SRV_RSNIFFER_SerialCfmMessage ( SRV_RSNIFFER_PHY_DATA *txData, size_t *pMsgLen );

#endif //SRV_RSNIFFER_H
