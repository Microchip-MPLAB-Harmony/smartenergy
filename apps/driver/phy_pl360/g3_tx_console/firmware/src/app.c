/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

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

#include <string.h>
#include "app.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#define DRV_PL360_INDEX_0   0

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

#define APP_TX_DELAY_US           1000000 /* 1 second */

#define div_ceil(a, b)            (((a) + (b) - 1) / (b))

//static bool _rxCheckCrc(DRV_PL360_RECEPTION_OBJ *indObj);

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void Timer1_Callback (uintptr_t context)
{
    /* RX Led off */
    LED_Set();
}

void APP_ReadCallback(uintptr_t context)
{
    /* ErrorGet clears errors */
    if(USART1_ErrorGet() == USART_ERROR_NONE)
    {
        /* Update reception data */
        appData.numCharToReceive = 0;
    }
}

static void APP_ExceptionCb(DRV_PL360_EXCEPTION exceptionObj, uintptr_t context )
{
    /* Avoid warning */
    (void)context;
    
    switch (exceptionObj) 
    {
        case DRV_PL360_EXCEPTION_UNEXPECTED_KEY:
            printf("Exception: DRV_PL360_EXCEPTION_UNEXPECTED_KEY]\r\n");
            break;

        case DRV_PL360_EXCEPTION_CRITICAL_ERROR:
            printf("Exception: DRV_PL360_EXCEPTION_CRITICAL_ERROR\r\n");
            break;

        case DRV_PL360_EXCEPTION_RESET:
            printf("Exception: DRV_PL360_EXCEPTION_RESET\r\n");
            break;

        case DRV_PL360_EXCEPTION_DEBUG:
            printf("Exception: DRV_PL360_EXCEPTION_DEBUG\r\n");
            break;

        default:
            printf("Exception: UNKNOWN\r\n");
	}

	appData.pl360_exception = true;
}

static void APP_DataCfmCb(DRV_PL360_TRANSMISSION_CFM_OBJ *cfmObj, uintptr_t context )
{
    /* Avoid warning */
    (void)context;
    
    /* Handle result of transmission : Show it through Console */
    switch(cfmObj->result)
    {
        case DRV_PL360_TX_RESULT_PROCESS:
            printf("...DRV_PL360_TX_RESULT_PROCESS\r\n");
            break;   
        case DRV_PL360_TX_RESULT_SUCCESS:
            printf("...DRV_PL360_TX_RESULT_PROCESS\r\n");
            break;   
        case DRV_PL360_TX_RESULT_INV_LENGTH:
            printf("...DRV_PL360_TX_RESULT_INV_LENGTH\r\n");
            break;
        case DRV_PL360_TX_RESULT_BUSY_CH:
            printf("...DRV_PL360_TX_RESULT_BUSY_CH\r\n");
            break;    
        case DRV_PL360_TX_RESULT_BUSY_TX:
            printf("...DRV_PL360_TX_RESULT_BUSY_TX\r\n");
            break;    
        case DRV_PL360_TX_RESULT_BUSY_RX:
            printf("...DRV_PL360_TX_RESULT_BUSY_RX\r\n");
            break;   
        case DRV_PL360_TX_RESULT_INV_SCHEME:
            printf("...DRV_PL360_TX_RESULT_INV_SCHEME\r\n");
            break; 
        case DRV_PL360_TX_RESULT_TIMEOUT:
            printf("...DRV_PL360_TX_RESULT_TIMEOUT\r\n");
            break;   
        case DRV_PL360_TX_RESULT_INV_TONEMAP:
            printf("...DRV_PL360_TX_RESULT_INV_TONEMAP\r\n");
            break;
        case DRV_PL360_TX_RESULT_INV_MODE:
            printf("...DRV_PL360_TX_RESULT_INV_MODE\r\n");
            break;   
        case DRV_PL360_TX_RESULT_NO_TX:
            printf("...DRV_PL360_TX_RESULT_NO_TX\r\n");
            break;   
    }
    
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

static uint32_t APP_ReadSerialChar(char *rcvChar, uint8_t size)
{    
    appData.numCharToReceive = size;
    
    if(USART1_Read((void*)rcvChar, size))
    {
        /* Success */
        return 0;
    }
    else
    {
        /* No data is available or errors */
        appData.state = APP_STATE_ERROR;
        return 1;
    }
}

static bool APP_SetAttenuationLevel(char *level)
{
	uint16_t us_level;
    bool res = false;

    us_level = (*level++ - 0x30) * 10;
    us_level += (*level - 0x30);
    
    if (us_level < 32) {
        appData.pl360Tx.attenuation = us_level;
//        save_config(PHY_APP_CMD_MENU_START_MODE);
        res = true;
    }
    
    return res;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_IDLE;
    appData.mode = APP_MODE_TRANSMISION;
    
    /* Init Timer handler */
    appData.tmr1Handle = SYS_TIME_HANDLE_INVALID;
    
    /* Update state machine */
    appData.state = APP_STATE_INIT; 
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            /* Open PL360 driver */
            appData.drvPl360Handle = DRV_PL360_Open(DRV_PL360_INDEX_0, DRV_IO_INTENT_READWRITE);
            
            if (appData.drvPl360Handle != DRV_HANDLE_INVALID)
            {
                appData.state = APP_STATE_CONFIG;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }
            break;
        }
            
        case APP_STATE_CONFIG:
        {
            /* Check PL360 device */
            if (DRV_PL360_Status(DRV_PL360_INDEX_0) == SYS_STATUS_READY)
            {
                /* Configure PL360 callbacks */
                DRV_PL360_ExceptionCallbackRegister(appData.drvPl360Handle, APP_ExceptionCb, DRV_PL360_INDEX_0);
                DRV_PL360_DataCfmCallbackRegister(appData.drvPl360Handle, APP_DataCfmCb, DRV_PL360_INDEX_0);
                
                /* Configure USART Callback */
                USART1_ReadCallbackRegister(APP_ReadCallback, 0);
                
                /* Waiting Console command */
                appData.state = APP_STATE_SHOW_MENU;
            }                
            break;
        }

        case APP_STATE_SHOW_MENU:
        {
            /* Show console interface */
            printf(MENU_HEADER);

            /* Waiting Console command */
            appData.state = APP_STATE_CONSOLE;
            
            /* Launch initial Reception job */
            APP_ReadSerialChar(appData.pReceivedChar, 1);
            
            break;
        }
            
        case APP_STATE_CONSOLE:
        {
            if (appData.numCharToReceive == 0)
            {
                switch(*appData.pReceivedChar)
                {
                    case '0':
                        printf("\r\nEnter attenuation level using 2 digits [00..31] : ");
                        appData.state = APP_STATE_SET_ATT_LEVEL;
                        APP_ReadSerialChar(appData.pReceivedChar, 2);
                        break;

                    case '1':
                        appData.state = APP_STATE_SET_SCHEME;
                        break;

                    case '2':
                        appData.state = APP_STATE_SET_TIME_PERIOD;
                        break;

                    case '3':
                        appData.state = APP_STATE_SET_DATA;
                        break;

                    case '4':
                        appData.state = APP_STATE_SET_TONE_MAP;
                        break;

                    case '5':
                        appData.state = APP_STATE_SET_PREEMPHASIS;
                        break;

                    case '6':
                        appData.state = APP_STATE_SET_BRANCH_MODE;
                        break;

                    case '7':
                        appData.state = APP_STATE_SET_OUTPUT_SIGNAL;
                        break;

                    case 'v':
                    case 'V':
                        appData.state = APP_STATE_VIEW_CONFIG;
                        break;

                    case 'e':
                    case 'E':
                        appData.state = APP_STATE_TX;
                        break;

                    case 'x':
                    case 'X':
                        appData.state = APP_STATE_STOP_TX;
                        break;

                    default:
                        break;

                }
            }
               
            break;
        }

        case APP_STATE_SET_ATT_LEVEL:
        {
            if (appData.numCharToReceive == 0)
            {
                if (APP_SetAttenuationLevel(appData.pReceivedChar))
                {
                    printf("\r\nSet Attenuation level = %u\r\n", 
                            (unsigned int)appData.pl360Tx.attenuation);
                    appData.state = APP_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: Attenuation level not permitted [0..31]. "
                            "Try again.\n\r");
                    APP_ReadSerialChar(appData.pReceivedChar, 2);
                }
            }  
            break;
        }

        case APP_STATE_SET_SCHEME:
        {
            break;
        }

        case APP_STATE_SET_TIME_PERIOD:
        {
            break;
        }

        case APP_STATE_SET_DATA:
        {
            break;
        }

        case APP_STATE_SET_TONE_MAP:
        {
            break;
        }

        case APP_STATE_SET_PREEMPHASIS:
        {
            break;
        }

        case APP_STATE_SET_BRANCH_MODE:
        {
            break;
        }

        case APP_STATE_SET_OUTPUT_SIGNAL:
        {
            break;
        }

        case APP_STATE_VIEW_CONFIG:
        {
            break;
        }

        case APP_STATE_TX:
        {
            break;
        }

        case APP_STATE_STOP_TX:
        {
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


/*******************************************************************************
 End of File
 */
