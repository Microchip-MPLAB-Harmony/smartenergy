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

#define DATALOG_TASK_DELAY_MS_UNTIL_FILE_SYSTEM_READY   100
#define DATALOG_TASK_DELAY_MS_BETWEEN_STATES            10


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

/* Define a queue to signal the Datalog Tasks to store data */
QueueHandle_t appDatalogQueueID = NULL;

/* Buffer to get a string name from User IDs */
static char *userToString[APP_DATALOG_USER_NUM] = {"metrology", "calibration", "tou", "rtc", "console", "events", "energy", "demand"};

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
static void APP_DATALOG_SysFSEventHandler(SYS_FS_EVENT event, void* eventData, uintptr_t context)
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

                if (app_datalogData.state == APP_DATALOG_STATE_READY)
                {
                    app_datalogData.state = APP_DATALOG_STATE_MOUNT_WAIT;
                }
                else
                {
                    app_datalogData.state = APP_DATALOG_STATE_ERROR;
                }
            }

            break;

        case SYS_FS_EVENT_ERROR:
        default:
            break;
    }
}

static void APP_DATALOG_GetFileNameByDate(APP_DATALOG_USER userId, APP_DATALOG_DATE *date, char *fileName)
{
    // If date not provided or invalid, buld filename with userId only
    if (date == NULL)
    {
        sprintf(fileName, "%s/%s", userToString[userId], userToString[userId]);
    }
    else if ((date->year == APP_DATALOG_INVALID_YEAR) || (date->month == APP_DATALOG_INVALID_MONTH))
    {
        sprintf(fileName, "%s/%s", userToString[userId], userToString[userId]);
    }
    else
    {
        // Build name using date
        sprintf(fileName, "%s/%02d%02d", userToString[userId], date->year, date->month);
    }
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    APP_DATALOG_STATES APP_DATALOG_GetStatus(void)

  Remarks:
    See prototype in app_datalog.h.
 */

APP_DATALOG_STATES APP_DATALOG_GetStatus(void)
{
    return app_datalogData.state;
}

/*******************************************************************************
  Function:
    bool APP_DATALOG_FileExists(APP_DATALOG_USER userId, APP_DATALOG_DATE *date)

  Remarks:
    See prototype in app_datalog.h.
 */

bool APP_DATALOG_FileExists(APP_DATALOG_USER userId, APP_DATALOG_DATE *date)
{
    SYS_FS_HANDLE fileHandle;

    // If Filesystem not yet ready, return false
    if (app_datalogData.state <= APP_DATALOG_STATE_CHECK_DIRECTORIES)
    {
        return false;
    }

    // Get file name from parameters
    APP_DATALOG_GetFileNameByDate(userId, date, app_datalogData.fileName);

    // Check whether file exist trying to open it
    fileHandle = SYS_FS_FileOpen(app_datalogData.fileName, (SYS_FS_FILE_OPEN_READ));

    if (fileHandle != SYS_FS_HANDLE_INVALID)
    {
        // File exists, close it and return true
        SYS_FS_FileClose(fileHandle);
        return true;
    }
    else
    {
        // File does not exist, return false
        return false;
    }
}

/*******************************************************************************
  Function:
    void APP_DATALOG_ClearData(APP_DATALOG_USER userId)

  Remarks:
    See prototype in app_datalog.h.
 */

void APP_DATALOG_ClearData(APP_DATALOG_USER userId)
{
    SYS_FS_HANDLE dirHandle;
    char dirName[16];
    SYS_FS_FSTAT stat;
    bool eod = false;
    char filePath[32];

    // Get directory name
    sprintf(dirName, "%s", userToString[userId]);
    //Open it
    dirHandle = SYS_FS_DirOpen(dirName);

    if (dirHandle != SYS_FS_HANDLE_INVALID)
    {
        while (!eod)
        {
            // Directory open is successful
            if(SYS_FS_DirRead(dirHandle, &stat) == SYS_FS_RES_FAILURE)
            {
                // Directory read failed.
                eod = true;
            }
            else
            {
                // Directory read succeeded.
                if (stat.fname[0] == '\0')
                {
                    // Reached the end of the directory.
                    eod = true;
                }
                else
                {
                    // Remove next foud file
                    sprintf(filePath, "%s/%s", dirName, stat.fname);
                    SYS_FS_FileDirectoryRemove(filePath);
                }
            }
        }
    }
}

/*******************************************************************************
  Function:
    void APP_DATALOG_Initialize ( void )

  Remarks:
    See prototype in app_datalog.h.
 */

void APP_DATALOG_Initialize ( void )
{
    // Place the App state machine in its initial state.
    app_datalogData.state = APP_DATALOG_STATE_MOUNT_WAIT;

    // Set the handling function of the File System
    SYS_FS_EventHandlerSet(APP_DATALOG_SysFSEventHandler,(uintptr_t)NULL);

    // Create a queue capable of containing 10 queue data elements.
    appDatalogQueueID = xQueueCreate(10, sizeof(APP_DATALOG_QUEUE_DATA));

    if (appDatalogQueueID == NULL)
    {
        // Queue was not created and must not be used.
        app_datalogData.state = APP_DATALOG_STATE_ERROR;
        return;
    }
}

/******************************************************************************
  Function:
    void APP_DATALOG_Tasks ( void )

  Remarks:
    See prototype in app_datalog.h.
 */

void APP_DATALOG_Tasks(void)
{
    // Check the application's current state.
    switch (app_datalogData.state)
    {
        case APP_DATALOG_STATE_MOUNT_WAIT:
        {
            if (app_datalogData.diskFormatRequired == true)
            {
                // Mount was successful. Format the disk.
                app_datalogData.state = APP_DATALOG_STATE_FORMAT_DISK;
            }
            else if (app_datalogData.diskMounted == true)
            {
                // Set current drive to access files directly without path
                SYS_FS_CurrentDriveSet(DATALOG_MOUNT_NAME);

                // Mount was successful. Check directories.
                app_datalogData.state = APP_DATALOG_STATE_CHECK_DIRECTORIES;
            }

            // Yield to other tasks
            vTaskDelay(DATALOG_TASK_DELAY_MS_UNTIL_FILE_SYSTEM_READY / portTICK_PERIOD_MS);

            break;
        }

        case APP_DATALOG_STATE_CHECK_DIRECTORIES:
        {
            uint8_t idx;
            SYS_FS_HANDLE dirHandle;

            // Check whether directories exist, otherwise create them
            for (idx = 0; idx < APP_DATALOG_USER_NUM; idx++)
            {
                dirHandle = SYS_FS_DirOpen(userToString[idx]);

                if (dirHandle != SYS_FS_HANDLE_INVALID)
                {
                    // Directory exists, close it
                    SYS_FS_DirClose(dirHandle);
                }
                else
                {
                    // Directory does not exist, create it
                    SYS_FS_DirectoryMake(userToString[idx]);
                }
            }
            // Ready to accept accesses
            app_datalogData.state = APP_DATALOG_STATE_READY;

            // Yield to other tasks
            vTaskDelay(DATALOG_TASK_DELAY_MS_BETWEEN_STATES / portTICK_PERIOD_MS);

            break;
        }

        case APP_DATALOG_STATE_FORMAT_DISK:
        {
            SYS_CONSOLE_PRINT("APP_DATALOG_STATE_FORMAT_DISK!!!\r\n\r\n");
            app_datalogData.state = APP_DATALOG_STATE_ERROR;

            break;
        }

        case APP_DATALOG_STATE_READY:
        {
            // Wait messages in queue
            if (xQueueReceive(appDatalogQueueID, &app_datalogData.newQueueData, portMAX_DELAY))
            {
                // Get file name
                if ((app_datalogData.newQueueData.date.year == APP_DATALOG_INVALID_YEAR) ||
                    (app_datalogData.newQueueData.date.month == APP_DATALOG_INVALID_MONTH))
                {
                    APP_DATALOG_GetFileNameByDate(app_datalogData.newQueueData.userId, NULL, app_datalogData.fileName);
                }
                else
                {
                    APP_DATALOG_GetFileNameByDate(app_datalogData.newQueueData.userId, &app_datalogData.newQueueData.date, app_datalogData.fileName);
                }

                // Check Read/Write operation
                if (app_datalogData.newQueueData.operation == APP_DATALOG_READ)
                {
                    // Go to Read state
                    app_datalogData.state = APP_DATALOG_STATE_READ_FROM_FILE;
                }
                else if ((app_datalogData.newQueueData.operation == APP_DATALOG_APPEND) ||
                         (app_datalogData.newQueueData.operation == APP_DATALOG_WRITE))
                {
                    // Go to Write state
                    app_datalogData.state = APP_DATALOG_STATE_WRITE_TO_FILE;
                }
            }

            // Yield to other tasks
            vTaskDelay(DATALOG_TASK_DELAY_MS_BETWEEN_STATES / portTICK_PERIOD_MS);

            break;
        }

        case APP_DATALOG_STATE_READ_FROM_FILE:
        {
            if (app_datalogData.newQueueData.operation == APP_DATALOG_READ)
            {
                // Read operation
                app_datalogData.fileHandle = SYS_FS_FileOpen(app_datalogData.fileName, (SYS_FS_FILE_OPEN_READ));
                if(app_datalogData.fileHandle != SYS_FS_HANDLE_INVALID)
                {
                    // File open succeeded. Read Data.
                    if (SYS_FS_FileRead(app_datalogData.fileHandle, app_datalogData.newQueueData.pData, app_datalogData.newQueueData.dataLen) != -1)
                    {
                        // Read success
                        app_datalogData.result = APP_DATALOG_RESULT_SUCCESS;
                    }
                    else
                    {
                        // Read error
                        app_datalogData.result = APP_DATALOG_RESULT_ERROR;
                    }
                    // Go to Close state
                    app_datalogData.state = APP_DATALOG_STATE_CLOSE_FILE;
                }
                else
                {
                    // File open failed.
                    app_datalogData.result = APP_DATALOG_RESULT_ERROR;
                    // Go to report state
                    app_datalogData.state = APP_DATALOG_STATE_REPORT_RESULT;
                }
            }

            // Yield to other tasks
            vTaskDelay(DATALOG_TASK_DELAY_MS_BETWEEN_STATES / portTICK_PERIOD_MS);

            break;
        }

        case APP_DATALOG_STATE_WRITE_TO_FILE:
        {
            if (app_datalogData.newQueueData.operation == APP_DATALOG_APPEND)
            {
                // Append operation
                app_datalogData.fileHandle = SYS_FS_FileOpen(app_datalogData.fileName, (SYS_FS_FILE_OPEN_APPEND));
                if(app_datalogData.fileHandle != SYS_FS_HANDLE_INVALID)
                {
                    // File open succeeded. Append Data.
                    if (SYS_FS_FileWrite(app_datalogData.fileHandle, app_datalogData.newQueueData.pData, app_datalogData.newQueueData.dataLen) != -1)
                    {
                        // Write success
                        app_datalogData.result = APP_DATALOG_RESULT_SUCCESS;
                    }
                    else
                    {
                        // Write error
                        app_datalogData.result = APP_DATALOG_RESULT_ERROR;
                    }
                    // Go to Close state
                    app_datalogData.state = APP_DATALOG_STATE_CLOSE_FILE;
                }
                else
                {
                    // File open failed.
                    app_datalogData.result = APP_DATALOG_RESULT_ERROR;
                    // Go to report state
                    app_datalogData.state = APP_DATALOG_STATE_REPORT_RESULT;
                }
            }
            else if (app_datalogData.newQueueData.operation == APP_DATALOG_WRITE)
            {
                // Write operation
                app_datalogData.fileHandle = SYS_FS_FileOpen(app_datalogData.fileName, (SYS_FS_FILE_OPEN_WRITE));
                if(app_datalogData.fileHandle != SYS_FS_HANDLE_INVALID)
                {
                    // File open succeeded. Write Data.
                    if (SYS_FS_FileWrite(app_datalogData.fileHandle, app_datalogData.newQueueData.pData, app_datalogData.newQueueData.dataLen) != -1)
                    {
                        // Write success
                        app_datalogData.result = APP_DATALOG_RESULT_SUCCESS;
                    }
                    else
                    {
                        // Write error
                        app_datalogData.result = APP_DATALOG_RESULT_ERROR;
                    }
                    // Go to Close state
                    app_datalogData.state = APP_DATALOG_STATE_CLOSE_FILE;
                }
                else
                {
                    // File open failed.
                    app_datalogData.result = APP_DATALOG_RESULT_ERROR;
                    // Go to report state
                    app_datalogData.state = APP_DATALOG_STATE_REPORT_RESULT;
                }
            }

            // Yield to other tasks
            vTaskDelay(DATALOG_TASK_DELAY_MS_BETWEEN_STATES / portTICK_PERIOD_MS);

            break;
        }

        case APP_DATALOG_STATE_CLOSE_FILE:
        {
            // Close current file
            if (SYS_FS_FileClose(app_datalogData.fileHandle) == SYS_FS_RES_SUCCESS)
            {
                // Go to report state
                app_datalogData.state = APP_DATALOG_STATE_REPORT_RESULT;
            }
            else
            {
                // Go to Error state
                app_datalogData.state = APP_DATALOG_STATE_ERROR;
            }

            // Yield to other tasks
            vTaskDelay(DATALOG_TASK_DELAY_MS_BETWEEN_STATES / portTICK_PERIOD_MS);

            break;
        }

        case APP_DATALOG_STATE_REPORT_RESULT:
        {
            // Invoke callback with result from operation
            if (app_datalogData.newQueueData.endCallback != NULL)
            {
                app_datalogData.newQueueData.endCallback(app_datalogData.result);
            }
            // Go back to Ready state
            app_datalogData.state = APP_DATALOG_STATE_READY;

            // Yield to other tasks
            vTaskDelay(DATALOG_TASK_DELAY_MS_BETWEEN_STATES / portTICK_PERIOD_MS);

            break;
        }

        case APP_DATALOG_STATE_ERROR:
        {
            SYS_CONSOLE_PRINT("APP_DATALOG_STATE_ERROR!!!\r\n\r\n");

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
