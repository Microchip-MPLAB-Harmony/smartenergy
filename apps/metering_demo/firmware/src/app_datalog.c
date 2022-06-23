/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_datalog.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app_datalog.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#define DATALOG_MOUNT_NAME          SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0
#define DATALOG_DEVICE_NAME         SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0
#define DATALOG_TST_FILE_NAME       "test_log.txt"

#define TEST_LOG_LEN                10


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_DATALOG_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATALOG_DATA CACHE_ALIGN app_datalogData;

QueueHandle_t CACHE_ALIGN app_datalogQueue = NULL;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
static void APP_DATALOG_SysFSEventHandler(SYS_FS_EVENT event,void* eventData,uintptr_t context)
{
    switch(event)
    {
        // If the event is mount then check which media has been mounted
        case SYS_FS_EVENT_MOUNT:
            if(strcmp((const char *)eventData, DATALOG_MOUNT_NAME) == 0)
            {
                app_datalogData.diskMounted = true;
            }

            break;

        case SYS_FS_EVENT_MOUNT_WITH_NO_FILESYSTEM:
        {
            if(strcmp((const char *)eventData, DATALOG_MOUNT_NAME) == 0)
            {
                app_datalogData.diskFormatRequired = true;
            }
            break;
        }

        // If the event is unmount then check which media has been unmount
        case SYS_FS_EVENT_UNMOUNT:
            if(strcmp((const char *)eventData, DATALOG_MOUNT_NAME) == 0)
            {
                app_datalogData.diskMounted = false;
                app_datalogData.diskFormatRequired = false;

                if (app_datalogData.state == APP_DATALOG_IDLE)
                {
                    app_datalogData.state = APP_DATALOG_MOUNT_WAIT;
                }
                else
                {
                    app_datalogData.state = APP_DATALOG_ERROR;
                }
            }

            break;

        case SYS_FS_EVENT_ERROR:
        default:
            break;
    }
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_DATALOG_Initialize ( void )

  Remarks:
    See prototype in app_datalog.h.
 */

void APP_DATALOG_Initialize ( void )
{
    // Place the App state machine in its initial state.
    app_datalogData.state = APP_DATALOG_MOUNT_WAIT;
    
    // Set the handling function of the File System
    SYS_FS_EventHandlerSet(APP_DATALOG_SysFSEventHandler,(uintptr_t)NULL);
    
    // Create a queue capable of containing 5 unsigned long values.
    app_datalogQueue = xQueueCreate( 5, sizeof( struct tm ) );

    if( app_datalogQueue == NULL )
    {
        // Queue was not created and must not be used.
        app_datalogData.state = APP_DATALOG_ERROR;
        return;
    }
}

/******************************************************************************
  Function:
    void APP_DATALOG_Tasks ( void )

  Remarks:
    See prototype in app_datalog.h.
 */

void APP_DATALOG_Tasks ( void )
{
    struct tm sysTime = { 0 };
    char logData[TEST_LOG_LEN] = { 0 };
    
    // Check the application's current state.
    switch ( app_datalogData.state )
    {
        case APP_DATALOG_MOUNT_WAIT:
        {
            if (app_datalogData.diskFormatRequired == true)
            {
                // Mount was successful. Format the disk.
                app_datalogData.state = APP_DATALOG_FORMAT_DISK;
            }
            else if (app_datalogData.diskMounted == true)
            {
                // Open file for testing
                app_datalogData.fileHandle = SYS_FS_FileOpen(DATALOG_MOUNT_NAME"/"DATALOG_TST_FILE_NAME,
                                                       (SYS_FS_FILE_OPEN_APPEND));
                
                if(app_datalogData.fileHandle == SYS_FS_HANDLE_INVALID)
                {
                    /* Could not open the file. Error out*/
                    app_datalogData.state = APP_DATALOG_ERROR;
                    break;
                }
                
                // Mount was successful. Open the file.
                app_datalogData.state = APP_DATALOG_IDLE;
            }
            
            break;
        }
        
        case APP_DATALOG_FORMAT_DISK:
        {
            SYS_CONSOLE_PRINT("APP_DATALOG_FORMAT_DISK!!!\r\n\r\n");
            app_datalogData.state = APP_DATALOG_ERROR;
            
            break;
        }

        case APP_DATALOG_IDLE:
        {
            // Wait messages in queue
            if (xQueueReceive(app_datalogQueue, &sysTime, portMAX_DELAY))
            {
                sprintf(logData, "[%02d:%02d:%02d]", sysTime.tm_hour, 
                        sysTime.tm_min, sysTime.tm_sec);
                
                if(SYS_FS_FilePrintf(app_datalogData.fileHandle, "%s\r\n", logData)
                                    == SYS_FS_RES_FAILURE)
                {
                    
                    /* There was an error while reading the file error out. */
                    SYS_CONSOLE_PRINT("Write Test File ERROR!!!\r\n\r\n");
                    app_datalogData.state = APP_DATALOG_ERROR;
                    
                }
                else
                {
                    if (SYS_FS_FileSync(app_datalogData.fileHandle) != 0)
                    {
                        /* Could not flush the contents of the file. Error out. */
                        SYS_CONSOLE_PRINT("Sync File ERROR!!!\r\n\r\n");
                        app_datalogData.state = APP_DATALOG_ERROR;
                        break;
                    }
                    
                    /* The test was successful. Lets idle. */
                    SYS_CONSOLE_PRINT("Write Test File Done!!! \r\n\r\n");
                }
            }
            break;
        }

        case APP_DATALOG_ERROR:
        {
            SYS_CONSOLE_PRINT("APP_DATALOG_ERROR!!!\r\n\r\n");
            
            break;
        }

        // The default state should never be executed.
        default:
        {
            // TODO: Handle error in application's state machine.
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
