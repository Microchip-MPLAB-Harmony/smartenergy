/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_console.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_CONSOLE_Initialize" and "APP_CONSOLE_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_CONSOLE_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_CONSOLE_H
#define _APP_CONSOLE_H

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
#include "app_events.h"

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

/* Default password for Console commands */
#define APP_CONSOLE_DEFAULT_PWD                   "PIC"
/* Max time (in ms) to wait for datalog to be ready.
 * Once time expires, console app continues without storaga capabilities */
#define CONSOLE_MAX_WAIT_MS_UNTIL_DATALOG_READY   2000

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
    APP_CONSOLE_STATE_INIT=0,
    APP_CONSOLE_STATE_IDLE,
    APP_CONSOLE_STATE_WAIT_STORAGE_READY,
    APP_CONSOLE_STATE_READ_STORAGE,
    APP_CONSOLE_STATE_READ_STORAGE_ERROR,
    APP_CONSOLE_STATE_DATALOG_NOT_READY,
    APP_CONSOLE_STATE_READ_CONTROL_REG,
    APP_CONSOLE_STATE_WRITE_CONTROL_REG,
    APP_CONSOLE_STATE_READ_ALL_CONTROL_REGS,
    APP_CONSOLE_STATE_READ_ACCUM_REG,
    APP_CONSOLE_STATE_READ_ALL_ACCUM_REGS,
    APP_CONSOLE_STATE_READ_STATUS_REG,
    APP_CONSOLE_STATE_READ_ALL_STATUS_REGS,
    APP_CONSOLE_STATE_READ_HARMONICS_REG,
    APP_CONSOLE_STATE_READ_ALL_HARMONICS_REGS,
    APP_CONSOLE_STATE_READ_METER_ID,
    APP_CONSOLE_STATE_READ_TOU,
    APP_CONSOLE_STATE_READ_RTC,
    APP_CONSOLE_STATE_PRINT_HRR,
    APP_CONSOLE_STATE_PRINT_MONTHLY_ENERGY,
    APP_CONSOLE_STATE_PRINT_EVENT,
    APP_CONSOLE_STATE_PRINT_MAX_DEMAND,
    APP_CONSOLE_STATE_PRINT_VOLTAGE,
    APP_CONSOLE_STATE_PRINT_CURRENT,
    APP_CONSOLE_STATE_PRINT_ACTIVE_POWER,
    APP_CONSOLE_STATE_PRINT_REACTIVE_POWER,
    APP_CONSOLE_STATE_PRINT_APARENT_POWER,
    APP_CONSOLE_STATE_PRINT_FREQUENCY,
    APP_CONSOLE_STATE_PRINT_ANGLE,
    APP_CONSOLE_STATE_PRINT_WAVEFORM_DATA,
} APP_CONSOLE_STATES;


#define APP_CONSOLE_MAX_REGS   64

typedef struct
{
    uint32_t value;
    uint8_t index;
} APP_CONSOLE_REG;

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
    APP_CONSOLE_STATES state;
    uint8_t ctrlRegToRead;
    uint8_t accumRegToRead;
    uint8_t statusRegToRead;
    uint8_t harRegToRead;
    uint32_t *rawData;
    size_t rawDataLen;
    APP_CONSOLE_REG regsToModify[APP_CONSOLE_MAX_REGS];
    bool dataValid;
    struct tm timeRequest;
    struct tm sysTime;
    APP_EVENTS_EVENT_ID eventIdRequest;
    uint8_t eventLastTimeRequest;
    uint32_t currentWaitForDatalogReady;
} APP_CONSOLE_DATA;

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
    void APP_CONSOLE_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_CONSOLE_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_CONSOLE_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_CONSOLE_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_CONSOLE_Tasks ( void )

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
    APP_CONSOLE_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_CONSOLE_Tasks( void );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_CONSOLE_H */

/*******************************************************************************
 End of File
 */

