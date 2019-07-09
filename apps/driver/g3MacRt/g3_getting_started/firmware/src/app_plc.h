/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_plc.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_PLC_Initialize" and "APP_PLC_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_PLC_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_PLC_H
#define _APP_PLC_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
#define DATA_SIZE               512    
    
// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    /* Application's state machine's initial state. */
    APP_PLC_STATE_INIT=0,
    APP_PLC_STATE_OPEN,
    APP_PLC_STATE_TX_CONFIG,
    APP_PLC_STATE_WAITING,
    APP_PLC_STATE_TX,
    APP_PLC_STATE_SET_MODULATION,
    APP_PLC_STATE_ERROR,

} APP_PLC_STATES;

// *****************************************************************************
/* PLC Coupling configuration data

  Summary:
    Holds PLC configuration data

  Description:
    This structure holds the PLC coupling configuration data.

  Remarks: 
    The maximum number of levels is 8.
 */    
typedef struct {
	uint32_t maxRMSHigh[8];
	uint32_t maxRMSVeryLow[8];
	uint32_t thresholdHigh[16];
	uint32_t thresholdVeryLow[16];
	uint32_t daccConfig[17];
	uint16_t predistorsionHigh[72];
	uint16_t predistorsionVeryLow[72];
	uint16_t gainHigh[3];
	uint16_t gainVeryLow[3];
	uint8_t numTxLevels;
} APP_PLC_COUPLING_DATA;

// *****************************************************************************
/* G3 MAC Frame Control Field

  Summary:
    Holds G3 MAC Frame Control Field

  Description:
   Frame control field is specified in G3-PLC Alliance - G3 PLC specifications
   document.

  Remarks:
   
 */

typedef struct __attribute__((packed, aligned(1)))
{
  uint16_t frameType : 3;
  uint16_t securityEnabled : 1;
  uint16_t framePending : 1;
  uint16_t ackRequest : 1;
  uint16_t panIdCompression : 1;
  uint16_t reserved : 3;
  uint16_t destAddressingMode : 2;
  uint16_t frameVersion : 2;
  uint16_t srcAddressingMode : 2;
}APP_G3_MAC_FRAME_CONTROL;

// *****************************************************************************
/* G3 MAC RT Data Frame

  Summary:
    Holds G3 MAC RT Data Frame application data

  Description:
   The content of the data buffer to use with G3 MAC RT driver should be compliant
   with fields defined in the MAC Header (MHR) specified in IEEE 802.15.4, 
   clause 7.2. The aim to this structure is to help to customer to use specific
   DATA frames to exchange data messages between G3 MAC RT applications.

  Remarks:
   Destination addressing mode is fixed to short mode and source addressing mode 
   is fixed to extended mode.
 */

typedef struct __attribute__((packed, aligned(1)))
{
  APP_G3_MAC_FRAME_CONTROL frameControl;
  uint8_t sequenceNumber;
  uint16_t panId;
  uint16_t destinationAddress;
  uint16_t sourceAddress;
}APP_G3_MAC_HEADER;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_PLC_STATES state;
    
    DRV_HANDLE drvG3MacRtHandle;
    
    uint32_t phyVersion;
    
    bool waitingTxCfm;
    
    bool couplingConfig;
    
    bool inTx;
    
    bool bin2InUse;
    
    MAC_RT_TX_PARAMETERS_OBJ g3MacRtTxParameters;
    
    MAC_RT_RX_PARAMETERS_OBJ g3MacRtRxParameters;
    
    MAC_RT_TX_CFM_OBJ g3MacRtTxCfm;
    
    uint8_t pPlcTxData[DATA_SIZE];
    
    uint8_t pPlcRxData[DATA_SIZE];
    
    uint8_t *pG3MacRtTxPayload;
    
    uint8_t *pG3MacRtRxPayload;
    
    uint16_t sourceAddress;
    
    uint16_t destinationAddress;
    
    uint8_t sequenceNumber;
    
} APP_PLC_DATA;

extern APP_PLC_DATA appPlc;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

/*******************************************************************************
  Function:
    void APP_PLC_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_PLC_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_PLC_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_PLC_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_PLC_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_PLC_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_PLC_Tasks( void );


void APP_PLC_SendMessage( uint8_t *pData, uint16_t length );



#endif /* _APP_PLC_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

