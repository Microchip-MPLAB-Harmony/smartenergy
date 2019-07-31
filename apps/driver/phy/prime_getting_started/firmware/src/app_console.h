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

/* String shown at Application initialization. */
#define APP_CONSOLE_STRING_HEADER "\r\n-- MCHP PRIME PHY Getting Started " \
    "Application --\r\n -- Compiled: "__DATE__ " "__TIME__ " --\r\n"

/* Buffer size for console print (APP_CONSOLE_Print()) */
#define APP_CONSOLE_PRINT_BUFFER_SIZE 512

/* Buffer size for console input */
#define APP_CONSOLE_READ_BUFFER_SIZE  (SYS_CONSOLE_UART_RD_QUEUE_DEPTH_IDX0 - 1)

/* Macro to compute round(a / b) using integer arithmetic */
#define DIV_ROUND(a, b)               (((a) + (b >> 1)) / (b))

/* Time in milliseconds for status led blink rate. */
#define LED_BLINK_RATE_MS             500

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

    /* Application waiting for initialization of PLC application. */
    APP_CONSOLE_STATE_WAIT_PLC,

    /* Application waiting for commands from console. */
    APP_CONSOLE_STATE_WAIT_COMMAND,

    /* The user is typing a message to transmit. */
    APP_CONSOLE_STATE_TYPING,

    /* Application waiting for input (Configuration Menu). */
    APP_CONSOLE_STATE_CONFIG_MENU,

    /* Application waiting for input (Modulation Configuration Menu). */
    APP_CONSOLE_STATE_CONFIG_MOD,

    /* Application waiting for input (Channel Configuration Menu). */
    APP_CONSOLE_STATE_CONFIG_CHANNEL,

    /* Application waiting for message to be transmitted. */
    APP_CONSOLE_STATE_TRANSMITTING,

} APP_CONSOLE_STATES;

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
    APP_CONSOLE_STATES state;

    /* Index for transmission data buffer */
    uint16_t txDataIndex;

    /* Index to next input character to be received */
    uint8_t charRcvIndex;

    /* Index to next input character to be processed */
    uint8_t charProcessedIndex;

    /* Flag to indicate if SYS_CONSOLE is busy for write */
    volatile bool consolePrintBusy;

    /* Console echo enabled/disabled */
    bool echoOn;

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
// Section: Console Interface Definition
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


// *****************************************************************************
/* Function:
    void APP_CONSOLE_Print(const char *format, ...)

  Summary:
    Formats and prints a message with a variable number of arguments to the
    console.

  Description:
    This function formats and prints a message with a variable number of
    arguments to the console.

  Precondition:
    APP_CONSOLE_Initialize must have returned a valid object handle.

  Parameters:
    format          - Pointer to a buffer containing the format string for
                      the message to be displayed.
    ...             - Zero or more optional parameters to be formated as
                      defined by the format string.

  Returns:
    None.

  Example:
    <code>
    // In source code
    int result;

    result = SomeOperation();
    if (result > MAX_VALUE)
    {
        APP_CONSOLE_Print("Result of %d exceeds max value\r\n", result);
    }
    </code>

  Remarks:
    The format string and arguments follow the printf convention.
*/

void APP_CONSOLE_Print(const char *format, ...);

void APP_CONSOLE_HandleRxMsgCrcOk(uint8_t *pData, uint16_t dataLen,
        DRV_PLC_PHY_SCH scheme, uint8_t rssi, uint8_t cinrAvg);

void APP_CONSOLE_HandleRxMsgCrcBad(void);



#endif /* _APP_CONSOLE_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

