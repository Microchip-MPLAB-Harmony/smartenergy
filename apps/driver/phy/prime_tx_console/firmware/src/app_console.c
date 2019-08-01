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
#include <stdarg.h>
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

static bool APP_CONSOLE_CheckIsPrintable(char data);

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

void APP_CONSOLE_ReadCallback(void *buffer)
{
    if (buffer == (void *)appConsole.pNextChar)
    {
        if (APP_CONSOLE_CheckIsPrintable(*appConsole.pNextChar))
        {
            /* Update Message length */
            appConsole.dataLength++;
        }

        /* Update reception data */
        appConsole.pNextChar++;
        appConsole.numCharToReceive = 0;
    }
}

void APP_CONSOLE_WriteCallback(void *buffer)
{
    appConsole.numCharToTransmit = 0;
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
        appConsole.dataLength = 0;
    }

    appConsole.numCharToReceive = size;

    if (SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, 0, (void*)appConsole.pNextChar, size))
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

static bool APP_CONSOLE_CharToHex(char value, uint8_t *hex)
{
    if ((value >= '0') && (value <= '9'))
    {
        *hex = value - 0x30;
    }
    else if ((value >= 'A') && (value <= 'F'))
    {
        *hex = value - 0x37;
    }
    else if ((value >= 'a') && (value <= 'f'))
    {
        *hex = value - 0x57;
    }
    else
    {
        return 0;
    }

    return 1;
}

static bool APP_CONSOLE_CheckIsPrintable(char data)
{
    if (((data >= 32) && (data <= 126)) ||
        ((data >= 128) && (data <= 254)) ||
         (data == '\t') || (data == '\n'))
    {
        return 1;
    }

    return 0;
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
	uint8_t attLevel;
    uint8_t attLevelHex;

    if (APP_CONSOLE_CharToHex(*level++, &attLevelHex))
    {
        attLevel = attLevelHex << 4;

        attLevelHex = *level;
        if (APP_CONSOLE_CharToHex(*level, &attLevelHex))
        {
            attLevel += attLevelHex;

            if ((attLevel <= 0x1F) || (attLevel == 0xFF)) {
                appPlcTx.pl360Tx.attenuation = attLevel;
                return true;
            }
        }
    }

    return false;
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
    APP_CONSOLE_Print("\n\r-- Configuration Info --------------\r\n");
    APP_CONSOLE_Print("-I- PHY Version: 0x%08X\n\r", (unsigned int)appPlcTx.pl360PhyVersion);
    APP_CONSOLE_Print("-I- Buffer: %u\n\r", (unsigned int)appPlcTx.pl360Tx.bufferId);

    if (appPlcTx.pl360Tx.attenuation == 0xFF)
    {
        APP_CONSOLE_Print("-I- TX Attenuation: 0xFF (no signal)\n\r");
    }
    else
    {
        APP_CONSOLE_Print("-I- TX Attenuation: 0x%02X\n\r", (unsigned int)appPlcTx.pl360Tx.attenuation);
    }

    switch (appPlcTx.pl360Tx.scheme)
    {
        case SCHEME_DBPSK:
            APP_CONSOLE_Print("-I- Modulation Scheme: DBPSK \n\r");
		break;

        case SCHEME_DQPSK:
            APP_CONSOLE_Print("-I- Modulation Scheme: DQPSK \n\r");
		break;

        case SCHEME_D8PSK:
            APP_CONSOLE_Print("-I- Modulation Scheme: D8PSK \n\r");
		break;

        case SCHEME_DBPSK_C:
            APP_CONSOLE_Print("-I- Modulation Scheme: DBPSK_C \n\r");
		break;

        case SCHEME_DQPSK_C:
            APP_CONSOLE_Print("-I- Modulation Scheme: DQPSK_C \n\r");
		break;

        case SCHEME_D8PSK_C:
            APP_CONSOLE_Print("-I- Modulation Scheme: D8PSK_C \n\r");
		break;

        case SCHEME_R_DBPSK:
            APP_CONSOLE_Print("-I- Modulation Scheme: R_DBPSK \n\r");
		break;

        case SCHEME_R_DQPSK:
            APP_CONSOLE_Print("-I- Modulation Scheme: R_DQPSK \n\r");
		break;

        default:
            APP_CONSOLE_Print("-I-  Modulation Scheme: ERROR CFG\n\r");
	}

    APP_CONSOLE_Print("-I- Forced Tx: %u\n\r", (unsigned int)appPlcTx.pl360Tx.forced);

    switch (appPlcTx.pl360Tx.frameType) {
        case FRAME_TYPE_A:
            APP_CONSOLE_Print("-I- PRIME mode: MODE_TYPE_A\n\r");
            break;

        case FRAME_TYPE_B:
            APP_CONSOLE_Print("-I- PRIME mode: MODE_TYPE_B\n\r");
            break;

        case FRAME_TYPE_BC:
            APP_CONSOLE_Print("-I- PRIME mode: MODE_TYPE_BC\n\r");
            break;

        default:
            APP_CONSOLE_Print("-I- PRIME mode: ERROR CFG\n\r");
	}

    switch (appPlcTx.channel) {
        case 1:
            APP_CONSOLE_Print("-I- PRIME channel: 1\n\r");
            break;

        case 3:
            APP_CONSOLE_Print("-I- PRIME channel: 3\n\r");
            break;

        case 4:
            APP_CONSOLE_Print("-I- PRIME channel: 4\n\r");
            break;

        case 5:
            APP_CONSOLE_Print("-I- PRIME channel: 5\n\r");
            break;

        case 6:
            APP_CONSOLE_Print("-I- PRIME channel: 6\n\r");
            break;

        case 7:
            APP_CONSOLE_Print("-I- PRIME channel: 7\n\r");
            break;

        case 8:
            APP_CONSOLE_Print("-I- PRIME channel: 8\n\r");
            break;

        default:
            APP_CONSOLE_Print("-I- PRIME channel: ERROR CFG\n\r");
	}

    if (appPlcTx.txAuto)
    {
		APP_CONSOLE_Print("-I- Branch Mode : Autodetect - ");
	}
    else
    {
		APP_CONSOLE_Print("-I- Branch Mode : Fixed - ");
	}

	if (appPlcTx.txImpedance == HI_STATE)
    {
		APP_CONSOLE_Print("High Impedance \r\n");
	}
    else
    {
		APP_CONSOLE_Print("Very Low Impedance \r\n");
	}

	APP_CONSOLE_Print("-I- Time Period: %u\n\r", (unsigned int)appPlcTx.pl360Tx.time);
	APP_CONSOLE_Print("-I- Data Len: %u\n\r", (unsigned int)appPlcTx.pl360Tx.dataLength);

	if (appPlcTx.pl360Tx.pTransmitData[0] == 0x30)
    {
		APP_CONSOLE_Print("-I- Fixed Data\r\n");
	}
    else
    {
		APP_CONSOLE_Print("-I- Random Data\r\n");
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

    /* Init App vars */
    appConsole.numCharToTransmit = 0;
    appConsole.numCharToReceive = 0;

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
            /* Configure CONSOLE Callbacks */
            SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0, APP_CONSOLE_ReadCallback,
                    SYS_CONSOLE_EVENT_READ_COMPLETE);
            SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0, APP_CONSOLE_WriteCallback,
                    SYS_CONSOLE_EVENT_WRITE_COMPLETE);

            appConsole.state = APP_CONSOLE_STATE_WAIT_PLC;

            /* Show App Header */
            APP_CONSOLE_Print(STRING_HEADER);

            break;
        }

        case APP_CONSOLE_STATE_WAIT_PLC:
        {
            /* Wait for PLC transceiver initialization */
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
            APP_CONSOLE_Print(MENU_HEADER);

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
                        APP_CONSOLE_Print(MENU_TX_BUFFER);
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                    case '1':
                        appConsole.state = APP_CONSOLE_STATE_SET_ATT_LEVEL;
                        APP_CONSOLE_Print("\r\nEnter attenuation level using 2 digits [00..1F][use FF for signal 0] : ");
                        APP_CONSOLE_ReadSerialChar(2, true);
                        break;

                    case '2':
                        appConsole.state = APP_CONSOLE_STATE_SET_SCHEME;
                        APP_CONSOLE_Print(MENU_SCHEME);
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                    case '3':
                        appConsole.state = APP_CONSOLE_STATE_SET_FORCED_MODE;
                        APP_CONSOLE_Print(MENU_MODE);
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                    case '4':
                        appConsole.state = APP_CONSOLE_STATE_SET_FRAME_TYPE;
                        APP_CONSOLE_Print(FRAME_TYPE);
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                    case '5':
                        APP_CONSOLE_Print("\r\nEnter transmission period in us. (max. 10 digits and value min 2100 us): ");
                        APP_CONSOLE_ReadSerialChar(1, true);
                        appConsole.state = APP_CONSOLE_STATE_SET_TIME_PERIOD;
                        break;

                    case '6':
                        APP_CONSOLE_Print("\r\nEnter length of data to transmit in bytes (max. 512 bytes): ");
                        appConsole.state = APP_CONSOLE_STATE_SET_DATA_LEN;
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                    case '7':
                        APP_CONSOLE_Print("\r\nEnter PLC channel to transmit (1 - 8): ");
                        APP_CONSOLE_ReadSerialChar(1, true);
                        appConsole.state = APP_CONSOLE_STATE_SET_CHANNEL;
                        break;

                    case '8':
                        APP_CONSOLE_Print(MENU_BRANCH_MODE);
                        APP_CONSOLE_ReadSerialChar(1, true);
                        appConsole.state = APP_CONSOLE_STATE_SET_BRANCH_MODE;
                        break;

                    case 'v':
                    case 'V':
                        appConsole.state = APP_CONSOLE_STATE_VIEW_CONFIG;
                        break;

                    case 'e':
                    case 'E':
                        APP_CONSOLE_Print("\r\nStart transmission, type 'x' to cancel.\r\n");
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
                    APP_CONSOLE_Print("\r\nSet Transmission buffer = %u\r\n",
                            (unsigned int)appPlcTx.pl360Tx.bufferId);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    APP_CONSOLE_Print("\r\nERROR: Transmission buffer not permitted. Try again : ");
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
                    APP_CONSOLE_Print("\r\nSet Attenuation level = 0x%02x\r\n",
                            (unsigned int)appPlcTx.pl360Tx.attenuation);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    APP_CONSOLE_Print("\r\nERROR: Attenuation level not permitted. Try again : ");
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
                    APP_CONSOLE_Print("\r\nSet Scheme: %u \r\n",
                            (unsigned int)appPlcTx.pl360Tx.scheme);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    APP_CONSOLE_Print("\r\nERROR: Scheme not permitted. Try again : ");
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
                    APP_CONSOLE_Print("\r\nSet Forced Mode: %u\r\n",
                            (unsigned int)appPlcTx.pl360Tx.forced);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    APP_CONSOLE_Print("\r\nERROR: TX Mode not permitted. Try again : ");
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
                    APP_CONSOLE_Print("\r\nSet Frame Type: %u\r\n",
                            (unsigned int)appPlcTx.pl360Tx.frameType);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    APP_CONSOLE_Print("\r\nERROR: Frame Type not permitted. Try again : ");
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
                    APP_CONSOLE_Print("\r\nSet Time Period = %u us.\r\n",
                            (unsigned int)appPlcTx.pl360Tx.time);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    APP_CONSOLE_Print("\r\nERROR: Time Period not defined. Try again : ");
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
                    APP_CONSOLE_Print("\r\nSet Data Length = %u bytes\r\n",
                            (unsigned int)appPlcTx.pl360Tx.dataLength);

                    /* Set Data content */
                    APP_CONSOLE_Print(MENU_DATA_MODE);
                    APP_CONSOLE_ReadSerialChar(1, true);
                    appConsole.state = APP_CONSOLE_STATE_SET_DATA;
                }
                else
                {
                    /* Try it again */
                    APP_CONSOLE_Print("\r\nERROR: Time Period not defined. Try again : ");
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
                    APP_CONSOLE_Print("\r\nSet Data mode successfully\r\n");
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    APP_CONSOLE_Print("\r\nERROR: Data Mode not permitted. Try again : ");
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
                    APP_CONSOLE_Print("\r\nSet Channel = %u\r\n",
                            (unsigned int)appPlcTx.channel);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    APP_CONSOLE_Print("\r\nERROR: Channel not permitted. Try again : ");
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
                    APP_CONSOLE_Print("\r\nSet Auto: %u, Branch: %u \r\n",
                            (unsigned int)appPlcTx.txAuto,
                            (unsigned int)appPlcTx.txImpedance);
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    APP_CONSOLE_Print("\r\nERROR: Branch Mode not permitted. Try again : ");
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
                    APP_CONSOLE_Print("\r\nCancel transmission\r\n");
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
            APP_CONSOLE_Print("\r\nERROR: Unknown received character\r\n");
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

void APP_CONSOLE_Print(const char *format, ...)
{
    size_t len = 0;
    va_list args = {0};

    while (appConsole.numCharToTransmit);

    va_start( args, format );
    len = vsnprintf(appConsole.pTrasmitChar, SERIAL_BUFFER_SIZE - 1, format, args);
    va_end( args );

    if (len > 0 && len < SERIAL_BUFFER_SIZE - 1)
    {
        appConsole.pTrasmitChar[len] = '\0';

        appConsole.numCharToTransmit = len;

        SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, 0, appConsole.pTrasmitChar, len);
    }
}


/*******************************************************************************
 End of File
 */
