/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_console.c

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
#include <math.h>
#include "definitions.h"
#include "app_plc.h"
#include "app_nvm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
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

APP_CONSOLE_DATA CACHE_ALIGN appConsole;

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

void APP_CONSOLE_ReadCallback(uintptr_t context)
{
    /* ErrorGet clears errors */
    if(USART1_ErrorGet() == USART_ERROR_NONE)
    {
        /* Update reception data */
        appConsole.pNextChar++;
        appConsole.numCharToReceive = 0;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

static uint32_t APP_CONSOLE_ReadSerialChar(uint8_t size, bool restart)
{
    if (restart)
    {
        appConsole.pNextChar = appConsole.pReceivedChar;
    }
    
    appConsole.numCharToReceive = size;
    
    if(USART1_Read((void*)appConsole.pNextChar, size))
    {
        /* Success */
        return 0;
    }
    else
    {
        /* No data is available or errors */
        appConsole.state = APP_CONSOLE_STATE_ERROR;
        return 1;
    }
}

static bool APP_CONSOLE_SetBuffer(char *buffer)
{
    bool res = true;
    
	switch (*buffer) 
    {
		case '0':
			appPlcTx.pl360Tx.bufferId = 0;
			break;

		case '1':
			appPlcTx.pl360Tx.bufferId = 1;
			break;

		default:
			res = false;
	}
    
    return res;
}

static bool APP_CONSOLE_SetAttenuationLevel(char *level)
{
	uint16_t attLevel;
    bool res = false;

    attLevel = (*level++ - 0x30) * 10;
    attLevel += (*level - 0x30);
    
    if (attLevel < 32) {
        appPlcTx.pl360Tx.attenuation = attLevel;
//        save_config(PHY_APP_CONSOLE_CMD_MENU_START_MODE);
        res = true;
    }
    
    return res;
}

static bool APP_CONSOLE_SetScheme(char *scheme)
{
    bool res = true;
    
	switch (*scheme) 
    {
		case '0':
			appPlcTx.pl360Tx.scheme = SCHEME_DBPSK;
			break;

		case '1':
			appPlcTx.pl360Tx.scheme = SCHEME_DQPSK;
			break;

		case '2':
			appPlcTx.pl360Tx.scheme = SCHEME_D8PSK;
			break;

		case '3':
			appPlcTx.pl360Tx.scheme = SCHEME_DBPSK_C;
			break;

		case '4':
			appPlcTx.pl360Tx.scheme = SCHEME_DQPSK_C;
			break;

		case '5':
			appPlcTx.pl360Tx.scheme = SCHEME_D8PSK_C;
			break;

		case '6':
			appPlcTx.pl360Tx.scheme = SCHEME_R_DBPSK;
			break;

		case '7':
			appPlcTx.pl360Tx.scheme = SCHEME_R_DQPSK;
			break;

		default:
			res = false;
	}
    
    return res;
}

static bool APP_CONSOLE_SetForcedMode(char *mode)
{
    bool res = true;
    
	switch (*mode) 
    {
		case '0':
			appPlcTx.pl360Tx.forced = 0;
			break;

		case '1':
			appPlcTx.pl360Tx.forced = 1;
			break;

		default:
			res = false;
	}
    
    return res;
}

static bool APP_CONSOLE_SetFrameType(char *type)
{
    bool res = true;
    
	switch (*type) 
    {
		case '0':
			appPlcTx.pl360Tx.frameType = FRAME_TYPE_A;
			break;

		case '1':
			appPlcTx.pl360Tx.frameType = FRAME_TYPE_B;
			break;

		case '2':
			appPlcTx.pl360Tx.frameType = FRAME_TYPE_BC;
			break;

		default:
			res = false;
	}
    
    return res;
}

static bool APP_CONSOLE_SetTransmissionPeriod(char *pTime, size_t length)
{
    uint8_t index;
    uint8_t tmpValue;
    bool res = false;
    
    appPlcTx.pl360Tx.time = 0;

    for(index = length - 1; index > 0; index--)
    {
        if ((*pTime >= '0') && (*pTime <= '9')) {
				tmpValue = (*pTime - 0x30);
                appPlcTx.pl360Tx.time += (uint32_t)pow(10, index - 1) * tmpValue;
                pTime++;
                
                res = true;                
        }
        else
        {
            res = false;
            break;
        }
    }
    
    return res;
}

static bool APP_CONSOLE_SetDataLength(char *pDataLength, size_t length)
{
    uint8_t index;
    uint8_t tmpValue;
    bool res = false;
    
    appPlcTx.pl360Tx.dataLength = 0;

    for(index = length; index > 0; index--)
    {
        if ((*pDataLength >= '0') && (*pDataLength <= '9')) {
				tmpValue = (*pDataLength - 0x30);
                appPlcTx.pl360Tx.dataLength += (uint32_t)pow(10, index - 1) * tmpValue;
                pDataLength++;
                
                res = true;                
        }
        else
        {
            res = false;
            break;
        }
    }
    
    return res;
}

static bool APP_CONSOLE_SetDataMode(char *mode)
{
    size_t length;
    uint8_t* pData;
    uint32_t dataValue;
    bool res = true;
    
    length = appPlcTx.pl360Tx.dataLength;
    appPlcTx.pl360Tx.pTransmitData = appPlcTx.pDataTx;
    pData = appPlcTx.pl360Tx.pTransmitData;
    
	switch (*mode) 
    {
		case '0':
            /* Random mode */
            length >>= 2;
            length++;
            while(length--)
            {
                dataValue = TRNG_ReadData();
                *pData++ = (uint8_t)dataValue;
                *pData++ = (uint8_t)(dataValue >> 8);
                *pData++ = (uint8_t)(dataValue >> 16);
                *pData++ = (uint8_t)(dataValue >> 24);
            }
			break;

		case '1':
            /* Fixed mode */
            dataValue = 0x30;
			while(length--)
            {
                *pData++ = (uint8_t)dataValue++;
                if (dataValue == 0x3A) {
                    dataValue = 0x30;
                }
            }
			break;
            
		default:
			res = false;
	}
    
    return res;
}

static bool APP_CONSOLE_SetBranchMode(char *mode)
{
    bool res = true;
    
	switch (*mode) 
    {
		case '0':
			appPlcTx.txAuto = 1;
			appPlcTx.txImpedance = HI_STATE;
			break;

		case '1':
			appPlcTx.txAuto = 0;
			appPlcTx.txImpedance = HI_STATE;
			break;

		case '2':
			appPlcTx.txAuto = 0;
			appPlcTx.txImpedance = VLO_STATE;
			break;

		default:
			res = false;
	}
    
    return res;
}

static bool APP_CONSOLE_SetOutputMode(char *mode)
{
    bool res = true;
    
	switch (*mode) 
    {
		case '0':
			appPlcTx.txForceNoOutput = 0;
			break;

		case '1':
			appPlcTx.txForceNoOutput = 1;
			break;

		default:
			res = false;
	}
    
    return res;
}

static bool APP_CONSOLE_SetChannel(char *channel)
{
    uint8_t chn;
    
    chn = *(uint8_t *)channel - 0x30;
    
    if ((chn > 0) && (chn < 9))
    {
        appPlcTx.channel = chn;
        return true;
    }
    
    return false;
}

static void APP_CONSOLE_ShowConfiguration(void)
{    
    printf("\n\r-- Configuration Info --------------\r\n");
    printf("-I- PHY Version: 0x%08X\n\r", (unsigned int)appPlcTx.pl360PhyVersion);
    printf("-I- Buffer: %u\n\r", (unsigned int)appPlcTx.pl360Tx.bufferId);
    printf("-I- TX Attenuation: %u\n\r", (unsigned int)appPlcTx.pl360Tx.attenuation);
    switch (appPlcTx.pl360Tx.scheme) 
    {
        case SCHEME_DBPSK:
            printf("-I- Modulation Scheme: DBPSK \n\r");
		break;

        case SCHEME_DQPSK:
            printf("-I- Modulation Scheme: DQPSK \n\r");
		break;

        case SCHEME_D8PSK:
            printf("-I- Modulation Scheme: D8PSK \n\r");
		break;

        case SCHEME_DBPSK_C:
            printf("-I- Modulation Scheme: DBPSK_C \n\r");
		break;

        case SCHEME_DQPSK_C:
            printf("-I- Modulation Scheme: DQPSK_C \n\r");
		break;

        case SCHEME_D8PSK_C:
            printf("-I- Modulation Scheme: D8PSK_C \n\r");
		break;

        case SCHEME_R_DBPSK:
            printf("-I- Modulation Scheme: R_DBPSK \n\r");
		break;

        case SCHEME_R_DQPSK:
            printf("-I- Modulation Scheme: R_DQPSK \n\r");
		break;

        default:
            printf("-I-  Modulation Scheme: ERROR CFG\n\r");
	}
    
    printf("-I- Forced Tx: %u\n\r", (unsigned int)appPlcTx.pl360Tx.forced);
    
    switch (appPlcTx.pl360Tx.frameType) {
        case FRAME_TYPE_A:
            printf("-I- PRIME mode: MODE_TYPE_A\n\r");
            break;

        case FRAME_TYPE_B:
            printf("-I- PRIME mode: MODE_TYPE_B\n\r");
            break;

        case FRAME_TYPE_BC:
            printf("-I- PRIME mode: MODE_TYPE_BC\n\r");
            break;

        default:
            printf("-I- PRIME mode: ERROR CFG\n\r");
	}
    
    switch (appPlcTx.channel) {
        case 1:
            printf("-I- PRIME channel: 1\n\r");
            break;

        case 3:
            printf("-I- PRIME channel: 3\n\r");
            break;

        case 4:
            printf("-I- PRIME channel: 4\n\r");
            break;

        case 5:
            printf("-I- PRIME channel: 5\n\r");
            break;

        case 6:
            printf("-I- PRIME channel: 6\n\r");
            break;

        case 7:
            printf("-I- PRIME channel: 7\n\r");
            break;

        case 8:
            printf("-I- PRIME channel: 8\n\r");
            break;

        default:
            printf("-I- PRIME channel: ERROR CFG\n\r");
	}
    
    if (appPlcTx.txAuto) 
    {
		printf("-I- Branch Mode : Autodetect - ");
	} 
    else 
    {
		printf("-I- Branch Mode : Fixed - ");
	}

	if (appPlcTx.txImpedance == HI_STATE) 
    {
		printf("High Impedance \r\n");
	} 
    else 
    {
		printf("Very Low Impedance \r\n");
	}
    
    printf("-I- Forced No Output Signal: %u\n\r", (unsigned int)appPlcTx.txForceNoOutput);
	printf("-I- Time Period: %u\n\r", (unsigned int)appPlcTx.pl360Tx.time);
	printf("-I- Data Len: %u\n\r", (unsigned int)appPlcTx.pl360Tx.dataLength);    

	if (appPlcTx.pl360Tx.pTransmitData[0] == 0x30) 
    {
		printf("-I- Fixed Data\r\n");
	} 
    else 
    {
		printf("-I- Random Data\r\n");
	}
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_CONSOLE_Initialize ( void )

  Remarks:
    See prototype in app_console.h.
 */

void APP_CONSOLE_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appConsole.state = APP_CONSOLE_STATE_IDLE;
    
    /* Init Timer handler */
    appConsole.tmr1Handle = SYS_TIME_HANDLE_INVALID;
    
    /* Update state machine */
    appConsole.state = APP_CONSOLE_STATE_INIT; 
    
}

/******************************************************************************
  Function:
    void APP_CONSOLE_Tasks ( void )

  Remarks:
    See prototype in app_console.h.
 */

void APP_CONSOLE_Tasks ( void )
{
    /* Refresh WDG */
    WDT_Clear();

    /* Check the application's current state. */
    switch ( appConsole.state )
    {
        /* Application's initial state. */
        case APP_CONSOLE_STATE_INIT:
        {
            /* Configure USART Callback */
            USART1_ReadCallbackRegister(APP_CONSOLE_ReadCallback, 0);
            
            appConsole.state = APP_CONSOLE_STATE_WAIT_PLC;
            
            /* Show App Header */
            printf(STRING_HEADER);
            
            break;
        }
            
        case APP_CONSOLE_STATE_WAIT_PLC:
        {
            /* Wait for PL360 device initialization */
            if (appPlc.state == APP_PLC_STATE_WAITING)
            {                
                /* Show Console menu */
                appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
            }
            else if (appPlc.state == APP_PLC_STATE_TX)
            {
                /* Set TX state */
                appConsole.state = APP_CONSOLE_STATE_TX;
            }
            break;
        }

        case APP_CONSOLE_STATE_SHOW_MENU:
        {
            /* Show console interface */
            printf(MENU_HEADER);

            /* Waiting Console command */
            appConsole.state = APP_CONSOLE_STATE_CONSOLE;
            
            /* Launch initial Reception job */
            APP_CONSOLE_ReadSerialChar(1, true);
            
            break;
        }
            
        case APP_CONSOLE_STATE_CONSOLE:
        {
            if (appConsole.numCharToReceive == 0)
            {
                switch(*appConsole.pReceivedChar)
                {
                    case '0':
                        appConsole.state = APP_CONSOLE_STATE_SET_BUFFER;
                        printf(MENU_TX_BUFFER);
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;
                        
                    case '1':
                        appConsole.state = APP_CONSOLE_STATE_SET_ATT_LEVEL;
                        printf("\r\nEnter attenuation level using 2 digits [00..31] : ");
                        APP_CONSOLE_ReadSerialChar(2, true);
                        break;

                    case '2':
                        appConsole.state = APP_CONSOLE_STATE_SET_SCHEME;
                        printf(MENU_SCHEME);
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                    case '3':
                        appConsole.state = APP_CONSOLE_STATE_SET_FORCED_MODE;
                        printf(MENU_MODE);
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                    case '4':
                        appConsole.state = APP_CONSOLE_STATE_SET_FRAME_TYPE;
                        printf(FRAME_TYPE);
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                    case '5':
                        printf("\r\nEnter transmission period in us. (max. 10 digits and value min 2100 us): ");
                        APP_CONSOLE_ReadSerialChar(1, true);
                        appConsole.state = APP_CONSOLE_STATE_SET_TIME_PERIOD;
                        break;

                    case '6':
                        printf("\r\nEnter length of data to transmit in bytes (max. 512 bytes): ");
                        appConsole.state = APP_CONSOLE_STATE_SET_DATA_LEN;       
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                    case '7':
                        printf("\r\nEnter PLC channel to transmit (1 - 8): ");                      
                        APP_CONSOLE_ReadSerialChar(1, true);
                        appConsole.state = APP_CONSOLE_STATE_SET_CHANNEL;
                        break;

                    case '8':
                        printf(MENU_BRANCH_MODE);                        
                        APP_CONSOLE_ReadSerialChar(1, true);
                        appConsole.state = APP_CONSOLE_STATE_SET_BRANCH_MODE;
                        break;

                    case '9':
                        printf(MENU_NO_OUTPUT);                        
                        APP_CONSOLE_ReadSerialChar(1, true);
                        appConsole.state = APP_CONSOLE_STATE_SET_OUTPUT_SIGNAL;
                        break;

                    case 'v':
                    case 'V':
                        appConsole.state = APP_CONSOLE_STATE_VIEW_CONFIG;
                        break;

                    case 'e':
                    case 'E':
                        printf("\r\nStart transmission, type 'x' to cancel.\r\n");
                        appPlc.state = APP_PLC_STATE_TX;
                        appConsole.state = APP_CONSOLE_STATE_TX;
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                    default:
                        /* Discard character */  
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                }
            }
               
            break;
        }

        case APP_CONSOLE_STATE_SET_BUFFER:
        {
            if (appConsole.numCharToReceive == 0)
            {
                if (APP_CONSOLE_SetBuffer(appConsole.pReceivedChar))
                {
                    printf("\r\nSet Transmission buffer = %u\r\n", 
                            (unsigned int)appPlcTx.pl360Tx.bufferId);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: Transmission buffer not permitted. Try again : ");
                    APP_CONSOLE_ReadSerialChar(1, true);
                }
            }  
            break;
        }

        case APP_CONSOLE_STATE_SET_ATT_LEVEL:
        {
            if (appConsole.numCharToReceive == 0)
            {
                if (APP_CONSOLE_SetAttenuationLevel(appConsole.pReceivedChar))
                {
                    printf("\r\nSet Attenuation level = %u\r\n", 
                            (unsigned int)appPlcTx.pl360Tx.attenuation);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: Attenuation level not permitted. Try again : ");
                    APP_CONSOLE_ReadSerialChar(2, true);
                }
            }  
            break;
        }

        case APP_CONSOLE_STATE_SET_SCHEME:
        {
            if (appConsole.numCharToReceive == 0)
            {
                if (APP_CONSOLE_SetScheme(appConsole.pReceivedChar))
                {
                    printf("\r\nSet Scheme: %u \r\n", 
                            (unsigned int)appPlcTx.pl360Tx.scheme);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: Scheme not permitted. Try again : ");
                    APP_CONSOLE_ReadSerialChar(1, true);
                }
            }  
            break;
        }

        case APP_CONSOLE_STATE_SET_FORCED_MODE:
        {
            if (appConsole.numCharToReceive == 0)
            {
                if (APP_CONSOLE_SetForcedMode(appConsole.pReceivedChar))
                {
                    printf("\r\nSet Forced Mode: %u\r\n", 
                            (unsigned int)appPlcTx.pl360Tx.forced);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: TX Mode not permitted. Try again : ");
                    APP_CONSOLE_ReadSerialChar(1, true);
                }
            }  
            break;
        }

        case APP_CONSOLE_STATE_SET_FRAME_TYPE:
        {
            if (appConsole.numCharToReceive == 0)
            {
                if (APP_CONSOLE_SetFrameType(appConsole.pReceivedChar))
                {
                    printf("\r\nSet Frame Type: %u\r\n", 
                            (unsigned int)appPlcTx.pl360Tx.frameType);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: Frame Type not permitted. Try again : ");
                    APP_CONSOLE_ReadSerialChar(1, true);
                }
            }  
            break;
        }

        case APP_CONSOLE_STATE_SET_TIME_PERIOD:
        {
            int diff;
            
            diff = appConsole.pNextChar - appConsole.pReceivedChar;
            if ((diff > 0) && ((diff == 10) ||
                (*(appConsole.pNextChar - 1) == '\n') ||
                (*(appConsole.pNextChar - 1) == '\r')))
            {
                if (APP_CONSOLE_SetTransmissionPeriod(appConsole.pReceivedChar, 
                        appConsole.pNextChar - appConsole.pReceivedChar))
                {
                    printf("\r\nSet Time Period = %u us.\r\n", 
                            (unsigned int)appPlcTx.pl360Tx.time);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: Time Period not defined. Try again : ");
                    APP_CONSOLE_ReadSerialChar(1, true);
                }
            }
            else if (appConsole.numCharToReceive == 0)
            {
                APP_CONSOLE_ReadSerialChar(1, false);
            }
            break;
        }

        case APP_CONSOLE_STATE_SET_DATA_LEN:
        {
            if ((*(appConsole.pNextChar - 1) == '\r') ||
                (*(appConsole.pNextChar - 1) == '\n') ||
                ((appConsole.pNextChar - appConsole.pReceivedChar) == 3))
            {
                if (APP_CONSOLE_SetDataLength(appConsole.pReceivedChar, 
                        appConsole.pNextChar - appConsole.pReceivedChar))
                {
                    printf("\r\nSet Data Length = %u bytes\r\n", 
                            (unsigned int)appPlcTx.pl360Tx.dataLength);
                    
                    /* Set Data content */
                    printf(MENU_DATA_MODE);
                    APP_CONSOLE_ReadSerialChar(1, true);
                    appConsole.state = APP_CONSOLE_STATE_SET_DATA;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: Time Period not defined. Try again : ");
                    APP_CONSOLE_ReadSerialChar(1, true);
                }
            }
            else if (appConsole.numCharToReceive == 0)
            {
                APP_CONSOLE_ReadSerialChar(1, false);
            }
            break;
        }

        case APP_CONSOLE_STATE_SET_DATA:
        {
            if (appConsole.numCharToReceive == 0)
            {
                if (APP_CONSOLE_SetDataMode(appConsole.pReceivedChar))
                {
                    printf("\r\nSet Data mode successfully\r\n");
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: Data Mode not permitted. Try again : ");
                    APP_CONSOLE_ReadSerialChar(1, true);
                }
            }  
            break;
        }

        case APP_CONSOLE_STATE_SET_CHANNEL:
        {
            if (appConsole.numCharToReceive == 0)
            {
                if (APP_CONSOLE_SetChannel(appConsole.pReceivedChar))
                {
                    printf("\r\nSet Channel = %u\r\n", 
                            (unsigned int)appPlcTx.channel);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: Channel not permitted. Try again : ");
                    APP_CONSOLE_ReadSerialChar(1, true);  
                }
            }  
            break;
        }
        
        case APP_CONSOLE_STATE_SET_BRANCH_MODE:
        {
            if (appConsole.numCharToReceive == 0)
            {
                if (APP_CONSOLE_SetBranchMode(appConsole.pReceivedChar))
                {
                    printf("\r\nSet Auto: %u, Branch: %u \r\n", 
                            (unsigned int)appPlcTx.txAuto,
                            (unsigned int)appPlcTx.txImpedance);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: Branch Mode not permitted. Try again : ");
                    APP_CONSOLE_ReadSerialChar(1, true);
                }
            }  
            break;
        }

        case APP_CONSOLE_STATE_SET_OUTPUT_SIGNAL:
        {
            if (appConsole.numCharToReceive == 0)
            {
                if (APP_CONSOLE_SetOutputMode(appConsole.pReceivedChar))
                {
                    printf("\r\nSet Force No Output : %u\r\n", 
                            (unsigned int)appPlcTx.txForceNoOutput);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: Value not permitted. Try again : ");
                    APP_CONSOLE_ReadSerialChar(1, true);
                }
            }  
            break;
        }

        case APP_CONSOLE_STATE_VIEW_CONFIG:
        {
            APP_CONSOLE_ShowConfiguration();
            appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
            break;
        }

        case APP_CONSOLE_STATE_TX:
        {
            if (appConsole.numCharToReceive == 0)
            {
                if ((*appConsole.pReceivedChar == 'x') || (*appConsole.pReceivedChar == 'X'))
                {
                    printf("\r\nCancel transmission\r\n");
                    appPlc.state = APP_PLC_STATE_STOP_TX;
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    APP_CONSOLE_ReadSerialChar(1, true);
                }
            }
            break;
        }

        case APP_CONSOLE_STATE_ERROR:
        {
            printf("\r\nERROR: Unknown received character\r\n");
            appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
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
