/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATE" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_H
#define _APP_H

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
#define APP_SERIAL_DATA_BUFFER_SIZE   512
#define APP_PLC_DATA_BUFFER_SIZE      512
#define APP_PLC_PIB_BUFFER_SIZE       256
    
#define LED_BLINK_RATE_MS             500
#define LED_BLINK_PLC_MSG_MS          100
    
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
    APP_STATE_IDLE=0,
    APP_STATE_INIT,
    APP_STATE_REGISTER,
    APP_STATE_CONFIG_PL360,
    APP_STATE_SEND_PLC_MSG,
    APP_STATE_SEND_USI_MSG,
    APP_STATE_READY,
    APP_STATE_ERROR

} APP_STATE;

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
	uint16_t predistorsionHigh[DRV_PL360_NUM_CARRIERS];
	uint16_t predistorsionVeryLow[DRV_PL360_NUM_CARRIERS];
	uint16_t gainHigh[3];
	uint16_t gainVeryLow[3];
	uint8_t numTxLevels;
} APP_PLC_COUPLING_DATA;

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
    APP_STATE state;
    
    /* TODO: Define any additional data used by the application. */
    SYS_TIME_HANDLE tmr1Handle; 
    
    SYS_TIME_HANDLE tmr2Handle;
    
    DRV_HANDLE drvPl360Handle;
    
    SRV_USI_HANDLE srvUSIHandle;
    
    bool pl360_exception;
    
    uint32_t pl360_err_unexpected;
    
    uint32_t pl360_err_critical;
    
    uint32_t pl360_err_reset;
    
    uint32_t pl360_err_unknow;
    
    uint8_t pPLCDataTx[APP_PLC_DATA_BUFFER_SIZE];
    
    uint8_t pPLCDataRx[APP_PLC_DATA_BUFFER_SIZE];
    
    uint8_t pPLCDataPIB[APP_PLC_PIB_BUFFER_SIZE];
    
    uint8_t pSerialData[APP_SERIAL_DATA_BUFFER_SIZE];
    
    DRV_PL360_TRANSMISSION_OBJ plcTxObj;
    
    DRV_PL360_TRANSMISSION_CFM_OBJ plcTxCfmObj;
    
    DRV_PL360_RECEPTION_OBJ plcRxObj;
    
    DRV_PL360_PIB_OBJ plcPIB;
    
    bool plcConfigureCoupling;

} APP_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

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
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks( void );



#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

