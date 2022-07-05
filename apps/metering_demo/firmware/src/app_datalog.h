/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_datalog.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_DATALOG_Initialize" and "APP_DATALOG_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_DATALOG_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_DATALOG_H
#define _APP_DATALOG_H

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
    /* Wait for disk Mount */
    APP_DATALOG_STATE_MOUNT_WAIT = 0,

    /* The app formats the disk. */
    APP_DATALOG_STATE_FORMAT_DISK,

    /* The app checks directories for storage. */
    APP_DATALOG_STATE_CHECK_DIRECTORIES,

    /* The app reads data from file */
    APP_DATALOG_STATE_READ_FROM_FILE,

    /* The app writes data to a file from the beginning */
    APP_DATALOG_STATE_WRITE_TO_FILE,

    /* The app closes the file. */
    APP_DATALOG_STATE_CLOSE_FILE,

    /* The app reports the result from file operation. */
    APP_DATALOG_STATE_REPORT_RESULT,

    /* The app is ready to accept requests */
    APP_DATALOG_STATE_READY,

    /* An app error has occurred */
    APP_DATALOG_STATE_ERROR

} APP_DATALOG_STATES;


// *****************************************************************************
/* Application Datalog Users

  Summary:
    Potential Datalog Users enumeration

  Description:
    This enumeration defines the different users of the datalog service in
    order to tag the store queue elements by their requester.
*/

typedef enum
{
    /* Metrology data */
    APP_DATALOG_USER_METROLOGY = 0,

    /* TOU data */
    APP_DATALOG_USER_TOU,

    /* RTC data */
    APP_DATALOG_USER_RTC,

    /* Console data */
    APP_DATALOG_USER_CONSOLE,

    /* Energy data */
    APP_DATALOG_USER_ENERGY,

    /* Events data */
    APP_DATALOG_USER_EVENTS,

    /* Demand data */
    APP_DATALOG_USER_DEMAND,

    /* Last value used to get number of Users */
    APP_DATALOG_USER_NUM

} APP_DATALOG_USER;


// *****************************************************************************
/* Application Datalog Available Operations

  Summary:
    List of Operations to perform by Datalog

*/

typedef enum
{
    /* Read operation */
    APP_DATALOG_READ = 0,

    /* Append operation */
    APP_DATALOG_APPEND,

    /* Read operation */
    APP_DATALOG_WRITE

} APP_DATALOG_OPERATION;


// *****************************************************************************
/* Application Datalog Possible results

  Summary:
    List of possible results for Datalog Operations

*/

typedef enum
{
    /* Result Success */
    APP_DATALOG_RESULT_SUCCESS = 0,

    /* Result Error */
    APP_DATALOG_RESULT_ERROR

} APP_DATALOG_RESULT;


// *****************************************************************************
/* Callback to report Datalog Operation end and its result

*/
typedef void (*APP_DATALOG_END_CALLBACK)(APP_DATALOG_RESULT result);


// *****************************************************************************
/* Application Queue Data

  Summary:
    Defines data to store in the Datalog Queue

  Description:
    This structure will be used by any module willing to use the Datalog service
    to indicate data to be stored.

  Remarks:
    None.
 */

typedef struct
{
    // Datalog User ID
    APP_DATALOG_USER userId;

    // Read/ReadPrevious/Write/Append operation
    APP_DATALOG_OPERATION operation;

    // Callback to be invoked at the end of Datalog operation
    APP_DATALOG_END_CALLBACK endCallback;

    // Date Time to index the proper file to read/write
    struct tm sysTime;

    // Length of data to be read/written
    uint16_t dataLen;

    // Pointer to data to be written or to store read data
    uint8_t *pData;

} APP_DATALOG_QUEUE_DATA;


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
    APP_DATALOG_STATES state;

    /* Element from Queue */
    APP_DATALOG_QUEUE_DATA newQueueData;

    /* Current File Handle */
    SYS_FS_HANDLE fileHandle;

    /* Current File Name */
    char fileName[32];

    /* Result from file operation */
    APP_DATALOG_RESULT result;

    /* Flag to indicate whether disk is mounted */
    bool diskMounted;

    /* Flag to indicate whether disk needs format */
    bool diskFormatRequired;

} APP_DATALOG_DATA;

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
    void APP_DATALOG_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_DATALOG_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_DATALOG_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_DATALOG_Initialize(void);


/*******************************************************************************
  Function:
    void APP_DATALOG_Tasks ( void )

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
    APP_DATALOG_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_DATALOG_Tasks(void);


/*******************************************************************************
  Function:
    APP_DATALOG_STATES APP_DATALOG_GetStatus(void)

  Summary:
    Provides current state of Datalog module.

  Description:
    This routine provides the current state of Datalog module.
    Available states are defined by APP_DATALOG_STATES Enum.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Datalog module state as defined in APP_DATALOG_STATES Enum.

  Example:
    <code>
    if (APP_DATALOG_GetStatus() == APP_DATALOG_STATE_READY)
      // Store last metrology data
    }
    </code>

  Remarks:
    None.
 */

APP_DATALOG_STATES APP_DATALOG_GetStatus(void);


/*******************************************************************************
  Function:
    bool APP_DATALOG_FileExists(APP_DATALOG_USER userId, struct tm sysTime)

  Summary:
    Checks whether a file exists.

  Description:
    This routine returns whether a file exists in synchronous mode,
    filename is obtained given its parameters.

  Precondition:
    None.

  Parameters:
    userId - ID of module to which file is associated to.
    sysTime - DateTime structure to derive filename from.

  Returns:
    True if file exists. Otherwise False.

  Example:
    <code>
    if (APP_DATALOG_FileExists(APP_DATALOG_USER_DEMAND, dateTime))
      // Put element in Datalog queue for file read/write operation
    }
    </code>

  Remarks:
    None.
 */

bool APP_DATALOG_FileExists(APP_DATALOG_USER userId, struct tm sysTime);


/*******************************************************************************
  Function:
    void APP_DATALOG_ClearData(APP_DATALOG_USER userId)

  Summary:
    Clears all stored data for a given user (module).

  Description:
    This routine clears all data for a given user id.
    As users ids have their own directory, it clear all files in it.

  Precondition:
    None.

  Parameters:
    userId - ID of module to clear data.

  Returns:
    None.

  Example:
    <code>
    // Clear all stored energy data
    APP_DATALOG_ClearData(APP_DATALOG_USER_ENERGY)
    </code>

  Remarks:
    None.
 */

void APP_DATALOG_ClearData(APP_DATALOG_USER userId);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_DATALOG_H */

/*******************************************************************************
 End of File
 */

