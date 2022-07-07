/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_display.c

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

#include "app_display.h"
#include "definitions.h"
#include "config/pic32cxmtsh_db/driver/display/cl010.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Initial value of the loop timer

  Summary:
    Initial value for the loop timer.

  Description:
    This is the initial value of the loop timer.
*/

#define APP_DISPLAY_TIMER_LOOP_INIT          2

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_DISPLAY_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DISPLAY_DATA CACHE_ALIGN app_displayData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
static void APP_DISPLAY_ScrollUp_Callback ( PIO_PIN pin, uintptr_t context)
{
    app_displayData.scrup_pressed = true;
}

static void APP_DISPLAY_ScrollDown_Callback ( PIO_PIN pin, uintptr_t context)
{
    app_displayData.scrdown_pressed = true;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    Init LCD module.
 */

static status_code_t APP_DISPLAY_InitLCD(void)
{
    status_code_t status;

    /* Initialize the CL010 LCD glass component */
    status = cl010_init();
    if (status != STATUS_OK) 
    {
        return STATUS_ERR_BUSY;
    }

    /* Show all symbols */
    cl010_show_all();

    /* Show icons */
    cl010_show_icon(CL010_ICON_MICROCHIP);
    cl010_show_icon(CL010_ICON_LCD);

    return STATUS_OK;
}

/*******************************************************************************
  Function:
    Timer in seconds for changing the info in loop mode.
 */

static void APP_DISPLAY_SetTimerLoop(uint32_t time_sec)
{
    if (time_sec) 
    {
        /* Set the new time for loop mode */
	 app_displayData.reload_display_time = time_sec;
    }
}

/*******************************************************************************
  Function:
    Add display info to the display loop.
 */

static void APP_DISPLAY_AddLoopInfo(APP_DISPLAY_INFO info)
{
    if (
            (info < APP_DISPLAY_MAX_TYPE)                      && 
            (app_displayData.loop_max < APP_DISPLAY_MAX_TYPE)
        ) 
    {
        /* Add info to display */
	app_displayData.loop_info[app_displayData.loop_max++] = info;
    }
}

/*******************************************************************************
  Function:
    Change the information in the display.
 */

static void APP_DISPLAY_ChangeInfo(void)
{
    if (app_displayData.loop_max <= APP_DISPLAY_MAX_TYPE) 
    {
        app_displayData.display_info = app_displayData.loop_info[app_displayData.loop_idx];

        if ((app_displayData.direction & APP_DISPLAY_FORWARD) == APP_DISPLAY_FORWARD) 
        {
            if (++app_displayData.loop_idx >= app_displayData.loop_max) 
            {
                app_displayData.loop_idx = 0;
            }
        } 
        else if ((app_displayData.direction & APP_DISPLAY_BACKWARD) == APP_DISPLAY_BACKWARD) 
        {
            if (--app_displayData.loop_idx >= app_displayData.loop_max) 
            {
                app_displayData.loop_idx = app_displayData.loop_max - 1;
            }
        }

        /* Reload time */
        app_displayData.display_time = app_displayData.reload_display_time;
    }
}

/*******************************************************************************
  Function:
    Process information for the display.
 */

static void APP_DISPLAY_Process(void)
{
//    uint64_t total;
    uint64_t upd_symbols = 1;
//    uint32_t temp, temp_dec;
    uint8_t buff1[9];

    if (app_displayData.display_info != 0xFF) 
    {
        cl010_clear_all();
    }

    /* Init internal buffer */
    memset(buff1, 0, sizeof(buff1));

    switch ( app_displayData.display_info )
    {
        case APP_DISPLAY_TOTAL_ENERGY:
        {
        
        }
        
        case APP_DISPLAY_TOU1_ENERGY:
        {
        
        }
         
        case APP_DISPLAY_TOU2_ENERGY:
        {
        
        }
          
        case APP_DISPLAY_TOU3_ENERGY:
        {
        
        }
        
        case APP_DISPLAY_TOU4_ENERGY:
        {
        
        }
        
        case APP_DISPLAY_VA_RMS:
        {
        
        }
        
        case APP_DISPLAY_VB_RMS:
        {
        
        }
      
        case APP_DISPLAY_VC_RMS:
        {
        
        }
        
        case APP_DISPLAY_IA_RMS:
        {
        
        }
        
        case APP_DISPLAY_IB_RMS:
        {
        
        }
        
        case APP_DISPLAY_IC_RMS:
        {
        
        }
        
        case APP_DISPLAY_RTC_TIME:
        {
        
        }
        
        case APP_DISPLAY_RTC_DATE:
        {
        
        }
        
        case APP_DISPLAY_TOTAL_MAX_DEMAND:
        {
        
        }
        
        case APP_DISPLAY_TOU1_MAX_DEMAND:
        {
        
        }
        
        case APP_DISPLAY_TOU2_MAX_DEMAND:
        {
        
        }
        
        case APP_DISPLAY_TOU3_MAX_DEMAND:
        {
        
        }
        
        case APP_DISPLAY_TOU4_MAX_DEMAND:
        {
        
        }
        
        case APP_DISPLAY_APP_INFO:
        {
            cl010_show_numeric_string(CL010_LINE_UP, app_displayData.app_info);
            upd_symbols = 0;
        }
        
        case APP_DISPLAY_BOARD_ID:
        {
        
        }
        
        case APP_DISPLAY_VERSION:
        {
        
        }
        
        default:
            break;
    }
    
    if (upd_symbols) {
        
    }

}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_DISPLAY_Initialize ( void )

  Remarks:
    See prototype in app_display.h.
 */

void APP_DISPLAY_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_displayData.state = APP_DISPLAY_STATE_INIT;

    /* No buttons pressed */
    app_displayData.scrdown_pressed = false;
    app_displayData.scrup_pressed = false;
     
    /* Init LCD */
    APP_DISPLAY_InitLCD();
    
    /* Set startup display information */
    app_displayData.loop_max = 0;
    app_displayData.loop_idx = 0;

    /* Clear flag to disable application message */
    app_displayData.app_info_en = false;
    memset(app_displayData.app_info, 0, 8);

    /* Set display loop direction by default */
    app_displayData.direction = APP_DISPLAY_FORWARD;

    /* Init display info */
    app_displayData.display_info = (APP_DISPLAY_INFO)0xFF;
    
    /* Initial counter of cycles */
    app_displayData.cycle_counter = 0;
    
    /* Set display time */
    app_displayData.display_time = APP_DISPLAY_TIMER_LOOP_INIT;
    
    /* Configure display timer loop */
    APP_DISPLAY_SetTimerLoop(3);
    
    /* Configure display measurements */
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_BOARD_ID);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_VERSION);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_TOTAL_ENERGY);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_TOU1_ENERGY);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_TOU2_ENERGY);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_TOU3_ENERGY);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_TOU4_ENERGY);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_RTC_TIME);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_RTC_DATE);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_VA_RMS);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_VB_RMS);
//    #if BOARD==PIC32CXMTC_DB
//    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_VC_RMS);
//    #endif
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_IA_RMS);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_IB_RMS);
//    #if BOARD==PIC32CXMTC_DB
//    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_IC_RMS);
//    #endif
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_TOTAL_MAX_DEMAND);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_TOU1_MAX_DEMAND);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_TOU2_MAX_DEMAND);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_TOU3_MAX_DEMAND);
    APP_DISPLAY_AddLoopInfo(APP_DISPLAY_TOU4_MAX_DEMAND);
}


/******************************************************************************
  Function:
    void APP_DISPLAY_Tasks ( void )

  Remarks:
    See prototype in app_display.h.
 */

void APP_DISPLAY_Tasks ( void )
{
    switch ( app_displayData.state )
    {
        case APP_DISPLAY_STATE_INIT:
        {
            PIO_PinInterruptCallbackRegister(SWITCH_SCRUP_PIN, 
                    APP_DISPLAY_ScrollUp_Callback, (uintptr_t)NULL);
            
            PIO_PinInterruptCallbackRegister(SWITCH_SCRDOWN_PIN, 
                    APP_DISPLAY_ScrollDown_Callback, (uintptr_t)NULL);
            
            PIO_PinInterruptEnable(SWITCH_SCRUP_PIN);
            PIO_PinInterruptEnable(SWITCH_SCRDOWN_PIN);
            
            app_displayData.state = APP_DISPLAY_STATE_SERVICE_TASKS;
            break;
        }

        case APP_DISPLAY_STATE_SERVICE_TASKS:
        {
            ++app_displayData.cycle_counter;
            /* Task configured to run every 250ms, so 4 cycles are 1 second */
            if (app_displayData.cycle_counter >= 4) 
            {
                /* Restart counter */
                app_displayData.cycle_counter = 0;
                 
                --app_displayData.display_time;
                /* Show information in display */
                if (app_displayData.display_time != 0)
                {
                    APP_DISPLAY_Process();
                } 
                else 
                {
                    /* Change the information if no button has been pressed */
                    if (
                            (!app_displayData.scrup_pressed)     && 
                            (!app_displayData.scrdown_pressed)
                        )
                    {
                        APP_DISPLAY_ChangeInfo();    
                    }
                }
            }
            
            /* If any button has been pressed, change the information */
            if (app_displayData.scrup_pressed)
            {
                LED_RED_Set();
                app_displayData.scrup_pressed = false;
                app_displayData.direction = APP_DISPLAY_FORWARD;
                APP_DISPLAY_ChangeInfo();
            }
            
            if (app_displayData.scrdown_pressed)
            {
                LED_RED_Clear();
                app_displayData.scrdown_pressed = false;
                app_displayData.direction = APP_DISPLAY_BACKWARD;
                APP_DISPLAY_ChangeInfo();
            }

            break;
        }


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

/******************************************************************************
  Function:
    void APP_DISPLAY_SetAppInfo(const char *msg, uint8_t len)

  Remarks:
    See prototype in app_display.h.
 */

void APP_DISPLAY_SetAppInfo(const char *msg, uint8_t len)
{
    if (len > sizeof(app_displayData.app_info)) 
    {
	len = sizeof(app_displayData.app_info);
    }
    
    memcpy(app_displayData.app_info, msg, len);

    if (!app_displayData.app_info_en) 
    {
	/* Set enable flag */
	app_displayData.app_info_en = true;
	/* Add APP display info */
	APP_DISPLAY_AddLoopInfo(APP_DISPLAY_APP_INFO);
    }

    /* Change display information and reload time */
    app_displayData.display_time = app_displayData.reload_display_time;
    app_displayData.display_info = APP_DISPLAY_APP_INFO;
}

/*******************************************************************************
 End of File
 */
