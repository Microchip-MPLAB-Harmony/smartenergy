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
#define SERIAL_BUFFER_SIZE        512
#define LED_RX_OFF_RATE_MS        100    
    
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
    APP_CONSOLE_STATE_IDLE=0,
    APP_CONSOLE_STATE_INIT,
    APP_CONSOLE_STATE_WAIT_PLC,
    APP_CONSOLE_STATE_CONFIG_MENU,
    APP_CONSOLE_STATE_CONSOLE,
    APP_CONSOLE_STATE_SET_SRC_ADDRESS,
    APP_CONSOLE_STATE_SET_DST_ADDRESS,
    APP_CONSOLE_STATE_SET_MODULATION,
    APP_CONSOLE_STATE_PLC_SEND,
    APP_CONSOLE_STATE_ERROR,

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
    SYS_TIME_HANDLE tmr1Handle;   
    
    APP_CONSOLE_STATES state;

    char pTrasmitChar[SERIAL_BUFFER_SIZE];

    char pReceivedChar[SERIAL_BUFFER_SIZE];

    char* pNextChar;
    
    uint8_t numCharToReceive;
    
    size_t dataLength;
    
    bool echoOn;
    
    bool newEcho;

} APP_CONSOLE_DATA;

extern APP_CONSOLE_DATA appConsole;

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
#define STRING_EOL    "\r"
#define STRING_HEADER "\r\n-- MCHP G3 MAC RT Getting Started Application --\r\n" \
	"-- Compiled: "__DATE__ " "__TIME__ " --\r\n"

#define MENU_HEADER "\n\r-- Menu Configuration --------------\n\r" \
	"0: Select attenuation Level\n\r" \
	"1: Select Modulation/Scheme\n\r" \
	"2: Select time period between messages to transmit(us.)\n\r" \
	"3: Select Data to transmit\n\r" \
	"4: Select TX tone Map\n\r" \
	"5: Select TX preemphasis\n\r" \
	"6: Select Branch Mode\n\r" \
	"7: Set/Clear Force No Output Signal\n\r" \
	"8: Select PLC band (only for multiband boards)\n\r" \
	"v: View TX configuration values\n\r" \
	"e: Execute transmission application\n\r" \
	"otherwise: Display this main menu\n\n\r"

#define MENU_SCHEME "\n\r-- Modulation Scheme --------------\r\n" \
	"0: DBPSK\n\r" \
	"1: DQPSK\n\r" \
	"2: D8PSK\n\r" \
	"3: Differential Robust\n\r" \
	"4: Coherent BPSK\n\r" \
	"5: Coherent QPSK\n\r" \
	"6: Coherent 8PSK\n\r" \
	"7: Coherent Robust\n\r" 

#define MENU_MODE "\n\r-- Transmission Mode --------------\r\n"	\
	"0: Immediate and Not Forced\n\r" \
	"1: Immediate and Forced\n\r" \
	"2: Delayed and Not Forced\n\r"	\
	"3: Delayed and Forced\n\r"

#define MENU_DATA_MODE "\n\r-- Select Data Mode --------------\r\n" \
	"0: Random Data\n\r" \
	"1: Fixed Data\n\r"

#define MENU_BRANCH_MODE "\n\r-- Select Branch Mode --------------\r\n"	\
	"0: Autodetect\n\r" \
	"1: High Impedance\n\r"	\
	"2: Very Low Impedance\n\r"

#define MENU_NO_OUTPUT "\n\r-- Force No Output Signal --------------\r\n"	\
	"0: Clear\n\r" \
	"1: Set\n\r"

#define MENU_MULTIBAND "\n\r-- Select PLC band --------------\r\n"	\
	"0: CEN A\n\r" \
	"1: FCC\n\r"

#define MENU_CONSOLE "\n\rPHY-Console>"

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

#endif /* _APP_CONSOLE_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

