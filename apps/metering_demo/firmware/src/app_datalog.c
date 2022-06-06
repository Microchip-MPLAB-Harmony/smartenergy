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
#define DATALOG_MOUNT_NAME          SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0
#define DATALOG_DEVICE_NAME         SYS_FS_MEDIA_IDX1_DEVICE_NAME_VOLUME_IDX0
#define DATALOG_TST_FILE_NAME       "test_log.txt"

#define TEST_LOG_LEN                10

#define BUILD_TIME_HOUR     ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0')
#define BUILD_TIME_MIN      ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0')
#define BUILD_TIME_SEC      ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0')


#define BUILD_DATE_YEAR     ((__DATE__[7] - '0') * 1000 + \
                             (__DATE__[8] - '0') * 100 + \
                             (__DATE__[9] - '0') * 10  + \
                             (__DATE__[10] - '0'))

#define BUILD_DATE_DAY      (((__DATE__[4] == 0x20)? 0:(__DATE__[4] - '0') * 10) + \
                             (__DATE__[5] - '0'))

const uint8_t Month_DayTable[12] = {0x31, 0x28, 0x31, 0x30, 0x31, 0x30, 0x31, 0x31, 0x30, 0x31, 0x30, 0x31};
const char Months_DateTable[12][3] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const uint8_t Day_weekTableD[12] = {6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

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
uint32_t int_cnt = 0;
uint32_t last_sec = 0;
void RTC_callback(uint32_t int_cause, uintptr_t context)
{
    if (int_cause & RTC_SR_SEC_Msk)
    {
        if (app_datalogData.state == APP_DATALOG_IDLE)
        {
            struct tm rtcTime;
            LED_RED_Toggle();
            RTC_TimeGet(&rtcTime);
            int_cnt++;
            if ((rtcTime.tm_sec == 0) || ((rtcTime.tm_sec % 10) == 0))
            {
                if (last_sec != rtcTime.tm_sec)
                {
                    // Write on Test File (Send to Queue)
                    xQueueSendFromISR(app_datalogQueue, &rtcTime, NULL);
                    last_sec = rtcTime.tm_sec;
                }
            }
        }
    }
}

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
    struct tm sysTime;
    char dateBuff[12];
	uint16_t y, d;
	uint8_t mon, uc_idx;
	uint8_t a, m;
    
    // Place the App state machine in its initial state.
    app_datalogData.state = APP_DATALOG_MOUNT_WAIT;
    
    // Set the handling function of the File System
    SYS_FS_EventHandlerSet(APP_DATALOG_SysFSEventHandler,(uintptr_t)NULL);
    
    // Create a queue capable of containing 10 unsigned long values.
    app_datalogQueue = xQueueCreate( 5, sizeof( struct tm ) );

    if( app_datalogQueue == NULL )
    {
        // Queue was not created and must not be used.
        app_datalogData.state = APP_DATALOG_ERROR;
        return;
    }
    
    // RTC Configuration
    // Get Time info from Build Time
    sysTime.tm_hour = BUILD_TIME_HOUR;
    sysTime.tm_min = BUILD_TIME_MIN;
    sysTime.tm_sec = BUILD_TIME_SEC;
    sysTime.tm_year = BUILD_DATE_YEAR - 1900;
    sysTime.tm_mday = BUILD_DATE_DAY;
    
    sprintf(dateBuff, "%s", __DATE__);
    sysTime.tm_mon = 0;
	for (uc_idx = 0; uc_idx < 12; uc_idx++) {
		if (memcmp(&Months_DateTable[uc_idx], dateBuff, 3) == 0) {
			sysTime.tm_mon = uc_idx;
			break;
		}
	}
    
	// Get day of the week
    mon = sysTime.tm_mon + 1;
	a = (14 - mon) / 12;
	y = BUILD_DATE_YEAR - ((14 - mon) / 12);
	m = mon + (12 * a) - 2;
	d = sysTime.tm_mday + y + y/4 - y/100 + y/400 + ((31*m)/12);
	sysTime.tm_wday = (d % 7) - 1;
    
    // Set RTC Time to current system time.
    RTC_TimeSet(&sysTime);
    RTC_CallbackRegister(RTC_callback, 0);
    //RTC_InterruptEnable(RTC_INT_SECOND);

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
                    SYS_CONSOLE_PRINT("Write Test File Done!!! - %u\r\n\r\n", int_cnt);
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
