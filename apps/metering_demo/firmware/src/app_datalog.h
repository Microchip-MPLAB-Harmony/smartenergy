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
    APP_DATALOG_MOUNT_WAIT = 0,

    /* The app formats the disk. */
    APP_DATALOG_FORMAT_DISK,

    /* The app opens the file */
    APP_DATALOG_OPEN_FILE,

    /* The app writes data to the file */
    APP_DATALOG_WRITE_TO_FILE,

    /* The app performs a file sync operation. */
    APP_DATALOG_FLUSH_FILE,

    /* The app checks the file status */
    APP_DATALOG_READ_FILE_STAT,

    /* The app checks the file size */
    APP_DATALOG_READ_FILE_SIZE,

    /* The app does a file seek to the end of the file. */
    APP_DATALOG_DO_FILE_SEEK,

    /* The app checks for EOF */
    APP_DATALOG_CHECK_EOF,

    /* The app does another file seek, to move file pointer to the beginning of
     * the file. */
    APP_DATALOG_DO_ANOTHER_FILE_SEEK,

    /* The app reads and verifies the written data. */
    APP_DATALOG_READ_FILE_CONTENT,

    /* The app closes the file. */
    APP_DATALOG_CLOSE_FILE,

    /* The app unmounts the disk. */
    APP_DATALOG_UNMOUNT_DISK,

    /* The app idles */
    APP_DATALOG_IDLE,

    /* An app error has occurred */
    APP_DATALOG_ERROR

} APP_DATALOG_STATES;


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
    
    SYS_FS_HANDLE fileHandle;
    
    SYS_FS_FSTAT fileStatus;
    
    long fileSize;

    bool diskMounted;

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

void APP_DATALOG_Initialize ( void );


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

void APP_DATALOG_Tasks( void );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_DATALOG_H */

/*******************************************************************************
 End of File
 */

