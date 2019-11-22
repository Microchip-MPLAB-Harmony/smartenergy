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

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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
#define BUFFER_SIZE               512    
    
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
    APP_PLC_STATE_IDLE=0,
    APP_PLC_STATE_INIT,
    APP_PLC_STATE_OPEN,
    APP_PLC_STATE_READ_CONFIG,
    APP_PLC_STATE_WRITE_CONFIG,
    APP_PLC_STATE_CHECK_CONFIG,
    APP_PLC_STATE_WAIT_CONFIG,
    APP_PLC_STATE_WAITING,
    APP_PLC_STATE_TX,
    APP_PLC_STATE_STOP_TX,
    APP_PLC_STATE_SET_BAND,
    APP_PLC_STATE_ERROR,

} APP_PLC_STATES;

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
    
    DRV_HANDLE drvPl360Handle;
    
    bool waitingTxCfm;
    
    bool couplingConfig;
    
    uint8_t couplingLevels;

} APP_PLC_DATA;

typedef struct
{    
    uint16_t configKey;
    
    uint32_t pl360PhyVersion;
    
    uint16_t pl360GainHigh[3];
    
    uint16_t pl360GainVeryLow[3];
    
    DRV_PLC_PHY_TRANSMISSION_OBJ pl360Tx;
    
    uint8_t pDataTx[BUFFER_SIZE];
    
	uint32_t txEndTime;
    
	uint8_t txAuto;
    
	uint8_t txImpedance;
    
    uint8_t channel;
    
    bool inTx;
    
    bool bin2InUse;

} APP_PLC_DATA_TX;

extern APP_PLC_DATA appPlc;
extern APP_PLC_DATA_TX appPlcTx;

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



#endif /* _APP_PLC_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

