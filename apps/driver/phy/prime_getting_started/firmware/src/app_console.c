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

#include "app_console.h"
#include <stdarg.h>

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* Buffer used by APP_CONSOLE_Print */
static char printBuffer[APP_CONSOLE_PRINT_BUFFER_SIZE];

/* Buffer used to receive characters from console */
static char charRcvBuffer[APP_CONSOLE_READ_BUFFER_SIZE];

/* PLC Transmission data buffer */
static uint8_t plcTxBuffer[MAX_DATA_LEN];

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_CONSOLE_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_CONSOLE_DATA appConsoleData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void _APP_CONSOLE_TimerLedToggleCb(uintptr_t context)
{
    /* Avoid warning */
    (void)context;

    /* Status Led Toggle */
    LED_Toggle();
}

static void _APP_CONSOLE_PrintCb(void *pBuffer)
{
    /* Avoid warning */
    (void)pBuffer;

    /* Print operation completed: SYS_CONSOLE is ready for write */
    appConsoleData.consolePrintBusy = false;
}

static void _APP_CONSOLE_RcvCharCb(void *pBuffer)
{
    /* Avoid warning */
    (void)pBuffer;

    /* New character received from console. Start new read operations on
     * SYS_CONSOLE */
    SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, 0,
                        &charRcvBuffer[appConsoleData.charRcvIndex], 1);

    /* Update index of received characters */
    if (appConsoleData.charRcvIndex == (APP_CONSOLE_READ_BUFFER_SIZE - 1))
    {
        appConsoleData.charRcvIndex = 0;
    }
    else
    {
        appConsoleData.charRcvIndex++;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

static void _APP_CONSOLE_ShowDataRate(DRV_PLC_PHY_SCH scheme)
{
    switch (scheme)
    {
        case SCHEME_D8PSK:
            APP_CONSOLE_Print(" ........................ 107.9 kbit/s\r\n");
            break;

        case SCHEME_DQPSK:
            APP_CONSOLE_Print(" ........................ 76.3 kbit/s\r\n");
            break;

        case SCHEME_DBPSK:
            APP_CONSOLE_Print(" ........................ 40.6 kbit/s\r\n");
            break;

        case SCHEME_D8PSK_C:
            APP_CONSOLE_Print(" ... 58.9 kbit/s\r\n");
            break;

        case SCHEME_DQPSK_C:
            APP_CONSOLE_Print(" ... 40.6 kbit/s\r\n");
            break;

        case SCHEME_DBPSK_C:
            APP_CONSOLE_Print(" ... 20.8 kbit/s\r\n");
            break;

        case SCHEME_R_DQPSK:
            APP_CONSOLE_Print(" ................. 10.2 kbit/s\r\n");
            break;

        case SCHEME_R_DBPSK:
            APP_CONSOLE_Print(" ................. 5.2 kbit/s\r\n");
            break;
	}
}

static void _APP_CONSOLE_ShowModulation(DRV_PLC_PHY_SCH scheme, uint8_t format)
{
	if (format == 0)
    {
		switch (scheme)
        {
            case SCHEME_DBPSK:
                APP_CONSOLE_Print("DBPSK");
                break;

            case SCHEME_DQPSK:
                APP_CONSOLE_Print("DQPSK");
                break;

            case SCHEME_D8PSK:
                APP_CONSOLE_Print("D8PSK");
                break;

            case SCHEME_DBPSK_C:
                APP_CONSOLE_Print("DBPSK + Convolutional Code");
                break;

            case SCHEME_DQPSK_C:
                APP_CONSOLE_Print("DQPSK + Convolutional Code");
                break;

            case SCHEME_D8PSK_C:
                APP_CONSOLE_Print("D8PSK + Convolutional Code");
                break;

            case SCHEME_R_DBPSK:
                APP_CONSOLE_Print("Robust DBPSK");
                break;

            case SCHEME_R_DQPSK:
                APP_CONSOLE_Print("Robust DQPSK");
                break;
		}
	}
    else
    {
		switch (scheme)
        {
            case SCHEME_DBPSK:
                APP_CONSOLE_Print("DBPSK");
                break;

            case SCHEME_DQPSK:
                APP_CONSOLE_Print("DQPSK");
                break;

            case SCHEME_D8PSK:
                APP_CONSOLE_Print("D8PSK");
                break;

            case SCHEME_DBPSK_C:
                /* _CC suffix means Convolutional Code */
                APP_CONSOLE_Print("DBPSK_CC");
                break;

            case SCHEME_DQPSK_C:
                /* _CC suffix means Convolutional Code */
                APP_CONSOLE_Print("DQPSK_CC");
                break;

            case SCHEME_D8PSK_C:
                /* _CC suffix means Convolutional Code */
                APP_CONSOLE_Print("D8PSK_CC");
                break;

            case SCHEME_R_DBPSK:
                /* R_ prefix means Robust (it also has Convolutional Code) */
                APP_CONSOLE_Print("R_DBPSK");
                break;

            case SCHEME_R_DQPSK:
                /* R_ prefix means Robust (it also has Convolutional Code) */
                APP_CONSOLE_Print("R_DQPSK");
                break;
		}
	}
}

static void _APP_CONSOLE_ShowTxParams(DRV_PLC_PHY_SCH scheme)
{
	APP_CONSOLE_Print("\r\nTx Modulation: ");
	_APP_CONSOLE_ShowModulation(scheme, 0);

	/* Maximum data length with configured TX Parameters */
	APP_CONSOLE_Print(" (Max data length = %u bytes)\r\n",
            (uint32_t)appPlcData.maxDataLen);
}

static void _APP_CONSOLE_ShowBand(uint8_t channel)
{
	switch (channel)
    {
        case 1:
            APP_CONSOLE_Print("Channel 1 (42 - 89 kHz, CENELEC-A)\r\n");
            break;

        case 2:
            APP_CONSOLE_Print("Channel 2 (97 - 144 kHz, CENELEC-BCD)\r\n");
            break;

        case 3:
            APP_CONSOLE_Print("Channel 3 (151 - 198 kHz, FCC)\r\n");
            break;

        case 4:
            APP_CONSOLE_Print("Channel 4 (206 - 253 kHz, FCC)\r\n");
            break;

        case 5:
            APP_CONSOLE_Print("Channel 5 (261 - 308 kHz, FCC)\r\n");
            break;

        case 6:
            APP_CONSOLE_Print("Channel 6 (315 - 362 kHz, FCC)\r\n");
            break;

        case 7:
            APP_CONSOLE_Print("Channel 7 (370 - 417 kHz, FCC)\r\n");
            break;

        case 8:
            APP_CONSOLE_Print("Channel 8 (425 - 472 kHz, FCC)\r\n");
            break;

        default:
            APP_CONSOLE_Print("Unknown channel)\r\n");
            break;
	}
}

static void _APP_CONSOLE_ShowMenu(bool fullMenu)
{
    switch (appConsoleData.state)
    {
        case APP_CONSOLE_STATE_WAIT_COMMAND:
        {
            /* Idle state: Show available options */
            if (fullMenu)
            {
                APP_CONSOLE_Print("\r\nPress 'CTRL+S' to enter configuration "
                        "menu. Enter text and press 'ENTER' to trigger "
                        "transmission");
            }

            APP_CONSOLE_Print("\r\n>>> ");
            break;
        }

        case APP_CONSOLE_STATE_TYPING:
        {
            /* Typing message to transmit */
            APP_CONSOLE_Print("\r\n>>> %.*s", appConsoleData.txDataIndex - 1,
                    plcTxBuffer + 1);
            break;
        }

        case APP_CONSOLE_STATE_CONFIG_MENU:
        {
            /* Configuration Menu */
            APP_CONSOLE_Print("\r\n--- Configuration Menu ---\r\nSelect "
                    "parameter to configure:\r\n\t0: Tx Modulation\r\n\t1: "
                    "Tx/Rx Channel\r\n>>> ");
            break;
        }

        case APP_CONSOLE_STATE_CONFIG_MOD:
        {
            /* Modulation Configuration */
            APP_CONSOLE_Print("\r\n--- Tx Modulation Configuration Menu ---\r\n"
                    "Select Modulation:\r\n");

            if (appPlcData.plcTxObj.scheme == SCHEME_R_DBPSK)
            {
                APP_CONSOLE_Print("->");
            }
            APP_CONSOLE_Print("\t0: ");
            _APP_CONSOLE_ShowModulation(SCHEME_R_DBPSK, 0);
            _APP_CONSOLE_ShowDataRate(SCHEME_R_DBPSK);

            if (appPlcData.plcTxObj.scheme == SCHEME_R_DQPSK)
            {
                APP_CONSOLE_Print("->");
            }
            APP_CONSOLE_Print("\t1: ");
            _APP_CONSOLE_ShowModulation(SCHEME_R_DQPSK, 0);
            _APP_CONSOLE_ShowDataRate(SCHEME_R_DQPSK);

            if (appPlcData.plcTxObj.scheme == SCHEME_DBPSK_C)
            {
                APP_CONSOLE_Print("->");
            }
            APP_CONSOLE_Print("\t2: ");
            _APP_CONSOLE_ShowModulation(SCHEME_DBPSK_C, 0);
            _APP_CONSOLE_ShowDataRate(SCHEME_DBPSK_C);

            if (appPlcData.plcTxObj.scheme == SCHEME_DQPSK_C)
            {
                APP_CONSOLE_Print("->");
            }
            APP_CONSOLE_Print("\t3: ");
            _APP_CONSOLE_ShowModulation(SCHEME_DQPSK_C, 0);
            _APP_CONSOLE_ShowDataRate(SCHEME_DQPSK_C);

            if (appPlcData.plcTxObj.scheme == SCHEME_D8PSK_C)
            {
                APP_CONSOLE_Print("->");
            }
            APP_CONSOLE_Print("\t4: ");
            _APP_CONSOLE_ShowModulation(SCHEME_D8PSK_C, 0);
            _APP_CONSOLE_ShowDataRate(SCHEME_D8PSK_C);

            if (appPlcData.plcTxObj.scheme == SCHEME_DBPSK)
            {
                APP_CONSOLE_Print("->");
            }
            APP_CONSOLE_Print("\t5: ");
            _APP_CONSOLE_ShowModulation(SCHEME_DBPSK, 0);
            _APP_CONSOLE_ShowDataRate(SCHEME_DBPSK);

            if (appPlcData.plcTxObj.scheme == SCHEME_DQPSK)
            {
                APP_CONSOLE_Print("->");
            }
            APP_CONSOLE_Print("\t6: ");
            _APP_CONSOLE_ShowModulation(SCHEME_DQPSK, 0);
            _APP_CONSOLE_ShowDataRate(SCHEME_DQPSK);

            if (appPlcData.plcTxObj.scheme == SCHEME_D8PSK)
            {
                APP_CONSOLE_Print("->");
            }
            APP_CONSOLE_Print("\t7: ");
            _APP_CONSOLE_ShowModulation(SCHEME_D8PSK, 0);
            _APP_CONSOLE_ShowDataRate(SCHEME_D8PSK);

            APP_CONSOLE_Print(">>> ");
            break;
        }

        case APP_CONSOLE_STATE_CONFIG_CHANNEL:
        {
            /* Channel configuration */
            APP_CONSOLE_Print("\r\n--- Tx/Rx Channel Configuration Menu ---\r\n"
                    "Select Channel:\r\n");

            for (uint8_t channel = 1; channel <= 8; channel++)
            {
                if (appPlcData.plcAllowedChannels & (1 << (channel - 1)))
                {
                    /* Channel allowed */
                    if (channel == appPlcData.plcChannel)
                    {
                        APP_CONSOLE_Print("->");
                    }

                    APP_CONSOLE_Print("\t%u: ", (uint32_t)channel);
                    _APP_CONSOLE_ShowBand(channel);
                }
            }

            APP_CONSOLE_Print(">>> ");
            break;
        }

        default:
            break;
	}
}

static bool _APP_CONSOLE_CheckIsPrintable(char c)
{
    bool isPrintable = false;

    if (((c >= 32) && (c <= 126)) ||
        ((c >= 128) && (c <= 254)) ||
         (c == '\t') || (c == '\n') || (c == '\r'))
    {
        isPrintable = true;
    }

    return isPrintable;
}

static bool _APP_CONSOLE_CheckRcvChar(char *rcvChar)
{
    if (appConsoleData.charRcvIndex == appConsoleData.charProcessedIndex)
    {
        /* No char received */
        return false;
    }
    else
    {
        /* There is at least one char pending to be processed */
        *rcvChar = charRcvBuffer[appConsoleData.charProcessedIndex];

        /* Echo */
        if (appConsoleData.echoOn)
        {
            /* Show received character if it is printable (ASCII) */
            if (_APP_CONSOLE_CheckIsPrintable(*rcvChar))
            {
                APP_CONSOLE_Print("%c", *rcvChar);
            }
        }

        /* Update index of processed characters */
        if (appConsoleData.charProcessedIndex ==
                (APP_CONSOLE_READ_BUFFER_SIZE - 1))
        {
            appConsoleData.charProcessedIndex = 0;
        }
        else
        {
            appConsoleData.charProcessedIndex++;
        }

        return true;
    }
}

void APP_CONSOLE_HandleRxMsgCrcOk(uint8_t *pData, uint16_t dataLen,
        DRV_PLC_PHY_SCH scheme, uint8_t rssi, uint8_t cinrAvg)
{
    APP_CONSOLE_Print("\rRx (");
    /* Show Modulation of received frame */
    _APP_CONSOLE_ShowModulation(scheme, 1);
    /* Show RSSI (Received Signal Strength Indicator) in dBuV */
    APP_CONSOLE_Print(", RSSI %udBuV", (uint32_t)rssi);
    /* Show CINR (Carrier to Interference-plus-Noise Ratio). It is in quarters
     * of dB and 10-dB offset: CINR(dB) = (cinrAvg - 40) / 4 */
	APP_CONSOLE_Print(", CINR %ddB): ", DIV_ROUND((int16_t)cinrAvg - 40, 4));
    /* Show received message */
    APP_CONSOLE_Print("%.*s", dataLen - 1, pData + 1);

	_APP_CONSOLE_ShowMenu(false);
}

void APP_CONSOLE_HandleRxMsgCrcBad(void)
{
	APP_CONSOLE_Print("\rRx ERROR: CRC error");
	_APP_CONSOLE_ShowMenu(false);
}

void APP_CONSOLE_Print(const char *format, ...)
{
    size_t len = 0;
    va_list args = {0};

    /* Wait for previous print to finish */
    while (appConsoleData.consolePrintBusy);

    /* Set SYS_CONSOLE busy for write */
    appConsoleData.consolePrintBusy = true;

    va_start( args, format );

    len = vsnprintf(printBuffer, APP_CONSOLE_PRINT_BUFFER_SIZE - 1, format,
            args);

    va_end( args );

    if (len > 0 && len < APP_CONSOLE_PRINT_BUFFER_SIZE - 1)
    {
        printBuffer[len] = '\0';

        SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, 0, printBuffer, len);
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
    appConsoleData.state = APP_CONSOLE_STATE_INIT;

    /* Initialize flag to indicate if SYS_CONSOLE is busy for write */
    appConsoleData.consolePrintBusy = false;

    /* Initialize indexes to buffer of received chars from console */
    appConsoleData.charRcvIndex = 0;
    appConsoleData.charProcessedIndex = 0;

    /* Enable console echo */
    appConsoleData.echoOn = true;
}


/******************************************************************************
  Function:
    void APP_CONSOLE_Tasks ( void )

  Remarks:
    See prototype in app_console.h.
 */

void APP_CONSOLE_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appConsoleData.state )
    {
        /* Application's initial state. */
        case APP_CONSOLE_STATE_INIT:
        {
            /* Register Timer Callback */
            SYS_TIME_CallbackRegisterMS(_APP_CONSOLE_TimerLedToggleCb, 0,
                    LED_BLINK_RATE_MS, SYS_TIME_PERIODIC);

            /* Configure callback on SYS_CONSOLE to know when print operation
             * finishes */
            SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0,
                    _APP_CONSOLE_PrintCb, SYS_CONSOLE_EVENT_WRITE_COMPLETE);

            /* Configure callback on SYS_CONSOLE to know when new characters are
             * received */
            SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0,
                    _APP_CONSOLE_RcvCharCb, SYS_CONSOLE_EVENT_READ_COMPLETE);

            /* Start read operations on SYS_CONSOLE */
            for (uint8_t index = 0; index < APP_CONSOLE_READ_BUFFER_SIZE;
                    index++)
            {
                SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, 0,
                        &charRcvBuffer[index], 1);
            }

            /* Show Application Header */
            APP_CONSOLE_Print(APP_CONSOLE_STRING_HEADER);

            /* Next Application state: Wait for PLC application initialization
             */
            appConsoleData.state = APP_CONSOLE_STATE_WAIT_PLC;

            break;
        }

        /* Application waiting for initialization of PLC application */
        case APP_CONSOLE_STATE_WAIT_PLC:
        {
            /* Check the state of PLC application */
            if (appPlcData.state == APP_PLC_STATE_WAITING)
            {
                _APP_CONSOLE_ShowTxParams(appPlcData.plcTxObj.scheme);

                /* Initialize data buffer index to point to second byte. In
                 * PRIME Type A frames, the first byte is not fully available
                 * (only 6 LSB bits are available and 2 MSB bits must be 0).
                 * PRIME stack uses the first byte to encode the header type and
                 *  6 bits is enough. But in this application, it is not
                 * acceptable because the first character would change */
                appConsoleData.txDataIndex = 1;
                plcTxBuffer[0] = 0;

                /* Next Application state: Wait for commands from console */
                appConsoleData.state = APP_CONSOLE_STATE_WAIT_COMMAND;

                _APP_CONSOLE_ShowMenu(true);
            }

            break;
        }

        /* Application waiting for commands from console. */
        case APP_CONSOLE_STATE_WAIT_COMMAND:
        {
            char rcvChar;

            /* Check if there is a new character received */
            if (_APP_CONSOLE_CheckRcvChar(&rcvChar))
            {
                /* Process the received char */
                switch (rcvChar)
                {
                    /* Special character: 0x13 (CTRL+S). Enter configuration
                     * menu */
                    case 0x13:
                    {
                        appConsoleData.state = APP_CONSOLE_STATE_CONFIG_MENU;
                        _APP_CONSOLE_ShowMenu(true);
                        break;
                    }

                    /* Special character: '\r' (Carriage Return) or
                     * '\n' (Line Feed). Show menu */
                    case '\r':
                    case '\n':
                    {
                        _APP_CONSOLE_ShowMenu(true);
                        break;
                    }

                    default:
                    {
                        /* Normal character: Start to type message if it is
                         * printable (ASCII) */
                        if (_APP_CONSOLE_CheckIsPrintable(rcvChar)) {
                            appConsoleData.state = APP_CONSOLE_STATE_TYPING;
                            plcTxBuffer[appConsoleData.txDataIndex++] = rcvChar;
                        }
                        break;
                    }
                }
            }
            break;
        }

        /* The user is typing a message to transmit. */
        case APP_CONSOLE_STATE_TYPING:
        {
            char rcvChar;

            /* Check if there is a new character received */
            if (_APP_CONSOLE_CheckRcvChar(&rcvChar))
            {
                /* New character received from console */
                bool sendPlcMsg = false;

                /* Process the received char */
                switch (rcvChar)
                {
                    case '\r':
                    {
                        /* Special character: '\r' (carriage return).
                         * Send message through PLC */
                        sendPlcMsg = true;
                        break;
                    }

                    case '\b':
                    case 0x7F:
                    {
                        /* Special character: '\b' (backspace) or 0x7F (DEL).
                         * Remove character from TX data buffer */
                        if (appConsoleData.txDataIndex > 1)
                        {
                            appConsoleData.txDataIndex--;
                            APP_CONSOLE_Print("\b \b");
                        }

                        if (appConsoleData.txDataIndex == 1)
                        {
                            /* All text has been removed: Go back to idle state
                             */
                            appConsoleData.state =
                                    APP_CONSOLE_STATE_WAIT_COMMAND;
                        }

                        break;
                    }

                    default:
                    {
                        /* Normal character: Add to TX data buffer if it is
                         * printable (ASCII) */
                        if (_APP_CONSOLE_CheckIsPrintable(rcvChar))
                        {
                            plcTxBuffer[appConsoleData.txDataIndex++] = rcvChar;
                            if (appConsoleData.txDataIndex ==
                                    (appPlcData.maxDataLen - 4))
                            {
                                /* Maximum data length reached: Send message
                                 * through PLC */
                                APP_CONSOLE_Print("\r\nMax data length reached"
                                        "... Message will be sent\r\n");
                                sendPlcMsg = true;
                            }
                        }
                        break;
                    }
                }

                if (sendPlcMsg)
                {
                    /* Send PLC message */
                    APP_PLC_SendMsg(plcTxBuffer, appConsoleData.txDataIndex);

                    /* Update Application state to wait for message to be
                     * transmitted. */
                    appConsoleData.state = APP_CONSOLE_STATE_TRANSMITTING;
                }
            }
			break;
        }

        /* Application is waiting for transmission of PLC message */
        case APP_CONSOLE_STATE_TRANSMITTING:
        {
            /* Check if transmission finished */
            if (!appPlcData.waitingTxCfm)
            {
                /* Initialize data buffer index to point to second byte. In
                 * PRIME Type A frames, the first byte is not fully available
                 * (only 6 LSB bits are available and 2 MSB bits must be 0).
                 * PRIME stack uses the first byte to encode the header type and
                 *  6 bits is enough. But in this application, it is not
                 * acceptable because the first character would change */
                appConsoleData.txDataIndex = 1;

                /* Wait for new commands from console */
                appConsoleData.state = APP_CONSOLE_STATE_WAIT_COMMAND;

                _APP_CONSOLE_ShowMenu(false);
            }
            break;
        }

        /* Application waiting for input (Configuration Menu). */
        case APP_CONSOLE_STATE_CONFIG_MENU:
        {
            char rcvChar;

            /* Check if there is a new character received */
            if (_APP_CONSOLE_CheckRcvChar(&rcvChar))
            {
                /* New character received from console */
                switch (rcvChar)
                {
                    case '0':
                        /* Modulation configuration */
                        appConsoleData.state = APP_CONSOLE_STATE_CONFIG_MOD;
                        _APP_CONSOLE_ShowMenu(true);
                        break;

                    case '1':
                        /* Channel configuration */
                        appConsoleData.state = APP_CONSOLE_STATE_CONFIG_CHANNEL;
                        _APP_CONSOLE_ShowMenu(true);
                        break;

                    default:
                        APP_CONSOLE_Print("\r\nUnknown command. Skipping "
                                "configuration\r\n");
                        appConsoleData.state = APP_CONSOLE_STATE_WAIT_COMMAND;
                        break;
                }
            }
			break;
        }

        /* Application waiting for input (Modulation Configuration Menu). */
        case APP_CONSOLE_STATE_CONFIG_MOD:
        {
            char rcvChar;

            /* Check if there is a new character received */
            if (_APP_CONSOLE_CheckRcvChar(&rcvChar))
            {
                /* New character received from console */
                bool validModulation = true;
                DRV_PLC_PHY_SCH scheme;

                switch (rcvChar)
                {
                    case '0':
                        /* R_DBPSK */
                        scheme = SCHEME_R_DBPSK;
                        break;

                    case '1':
                        /* R_DQPSK */
                        scheme = SCHEME_R_DQPSK;
                        break;

                    case '2':
                        /* DBPSK_C */
                        scheme = SCHEME_DBPSK_C;
                        break;

                    case '3':
                        /* DQPSK_C */
                        scheme = SCHEME_DQPSK_C;
                        break;

                    case '4':
                        /* D8PSK_C */
                        scheme = SCHEME_D8PSK_C;
                        break;

                    case '5':
                        /* DBPSK */
                        scheme = SCHEME_DBPSK;
                        break;

                    case '6':
                        /* DQPSK */
                        scheme = SCHEME_DQPSK;
                        break;

                    case '7':
                        /* D8PSK */
                        scheme = SCHEME_D8PSK;
                        break;

                    default:
                        APP_CONSOLE_Print("\r\nUnknown command. Skipping "
                                "configuration\r\n");
                        validModulation = false;
                        break;
                }

                if (validModulation)
                {
                    /* Set new Modulation. Get maximum data length allowed with
                     * configured Tx Parameters */
                    APP_PLC_SetModulation(scheme);
                    _APP_CONSOLE_ShowTxParams(scheme);
                }

                appConsoleData.state = APP_CONSOLE_STATE_WAIT_COMMAND;
                _APP_CONSOLE_ShowMenu(false);
            }
			break;
        }

        /* Application waiting for input (Channel Configuration Menu). */
        case APP_CONSOLE_STATE_CONFIG_CHANNEL:
        {
            char rcvChar;

            /* Check if there is a new character received */
            if (_APP_CONSOLE_CheckRcvChar(&rcvChar))
            {
                /* New character received from console */
                uint8_t newChannel = 0;

                for (char charChannel = '1'; charChannel <= '8'; charChannel++)
                {
                    if (rcvChar == charChannel)
                    {
                        newChannel = (uint8_t)(charChannel - '1' + 1);
                        break;
                    }
                }

                if (newChannel == 0)
                {
                    APP_CONSOLE_Print("\r\nUnknown command. Skipping "
                            "configuration\r\n");
                }
                else
                {
                    if (appPlcData.plcAllowedChannels & (1 << (newChannel - 1)))
                    {
                        /* Channel allowed */
                        if (newChannel == appPlcData.plcChannel)
                        {
                            APP_CONSOLE_Print("\r\nChannel has not changed: "
                                    "Skipping Channel Reconfiguration\r\n");
                        }
                        else
                        {
                            /* Set new channel on the PLC transceiver */
                            APP_CONSOLE_Print("\r\n");
                            APP_PLC_SetChannel(newChannel);
                        }
                    }
                    else
                    {
                        APP_CONSOLE_Print("\r\nChannel not allowed (see "
                                "APP_CONFIG_PRIME_ALLOWED_CHANNELS in 'user.h')"
                                ". Skipping configuration\r\n");
                    }
                }

                appConsoleData.state = APP_CONSOLE_STATE_WAIT_COMMAND;
                _APP_CONSOLE_ShowMenu(false);
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


/*******************************************************************************
 End of File
 */
