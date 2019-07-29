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
static char printBuffer[PRINT_BUFFER_SIZE];

/* Buffer used to receive characters from console */
static char charRcvBuffer[CONSOLE_READ_BUFFER_SIZE];

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
    
    /* Status Led Toogle */
    LED_Toggle();
}

static void _APP_CONSOLE_PrintCb(void *pBuffer)
{
    /* Print operation completed: SYS_CONSOLE is ready for write */
    appConsoleData.consolePrintBusy = false;
}

static void _APP_CONSOLE_RcvCharCb(void *pBuffer)
{
    /* New character received from console. Start new read operations on
     * SYS_CONSOLE */
    SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, 0,
                        &charRcvBuffer[appConsoleData.charRcvIndex], 1);
    
    /* Update index of received characters */
    if (appConsoleData.charRcvIndex == (CONSOLE_READ_BUFFER_SIZE - 1))
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

static void _APP_CONSOLE_ShowDataRate(DRV_PLC_PHY_MOD_SCHEME modScheme,
        DRV_PLC_PHY_MOD_TYPE modType)
{
    if (modScheme == MOD_SCHEME_DIFFERENTIAL)
    {
		switch (modType)
        {
		case MOD_TYPE_BPSK:
			switch (appPlcData.plcBand) {
			case G3_CEN_A:
				APP_CONSOLE_Print(" .......... 20.1 kbit/s\r\n");
				break;

			case G3_FCC:
				if (appPlcData.plcTxObj.rs2Blocks == 0) {
					/* 1 Reed-Solomon block */
					APP_CONSOLE_Print(" .......... 105.3 kbit/s\r\n");
				} else {
					/* 2 Reed-Solomon blocks */
					APP_CONSOLE_Print(" .......... 122 kbit/s\r\n");
				}

				break;

			case G3_ARIB:
				APP_CONSOLE_Print(" .......... 81.2 kbit/s\r\n");
				break;

			case G3_CEN_B:
				APP_CONSOLE_Print(" .......... 9.2 kbit/s\r\n");
				break;
			}
			break;

		case MOD_TYPE_QPSK:
			switch (appPlcData.plcBand) {
			case G3_CEN_A:
				APP_CONSOLE_Print(" .......... 34.5 kbit/s\r\n");
				break;

			case G3_FCC:
				if (appPlcData.plcTxObj.rs2Blocks == 0) {
					/* 1 Reed-Solomon block */
					APP_CONSOLE_Print(" .......... 165.1 kbit/s\r\n");
				} else {
					/* 2 Reed-Solomon blocks */
					APP_CONSOLE_Print(" .......... 210.6 kbit/s\r\n");
				}

				break;

			case G3_ARIB:
				APP_CONSOLE_Print(" .......... 130.4 kbit/s\r\n");
				break;

			case G3_CEN_B:
				APP_CONSOLE_Print(" .......... 16.4 kbit/s\r\n");
				break;
			}
			break;

		case MOD_TYPE_8PSK:
			switch (appPlcData.plcBand) {
			case G3_CEN_A:
				APP_CONSOLE_Print(" .......... 44.6 kbit/s\r\n");
				break;

			case G3_FCC:
				if (appPlcData.plcTxObj.rs2Blocks == 0) {
					/* 1 Reed-Solomon block */
					APP_CONSOLE_Print(" .......... 205.9 kbit/s\r\n");
				} else {
					/* 2 Reed-Solomon blocks */
					APP_CONSOLE_Print(" .......... 279.8 kbit/s\r\n");
				}

				break;

			case G3_CEN_B:
				APP_CONSOLE_Print(" .......... 21.8 kbit/s\r\n");
				break;
			}
			break;

		case MOD_TYPE_BPSK_ROBO:
			switch (appPlcData.plcBand) {
			case G3_CEN_A:
				APP_CONSOLE_Print(" ... 5.5 kbit/s\r\n");
				break;

			case G3_FCC:
				if (appPlcData.plcTxObj.rs2Blocks == 0) {
					/* 1 Reed-Solomon block */
					APP_CONSOLE_Print(" ... 34 kbit/s\r\n");
				} else {
					/* 2 Reed-Solomon blocks */
					APP_CONSOLE_Print(" ... 35.6 kbit/s\r\n");
				}

				break;

			case G3_ARIB:
				APP_CONSOLE_Print(" ... 25.7 kbit/s\r\n");
				break;

			case G3_CEN_B:
				APP_CONSOLE_Print(" ... 2.1 kbit/s\r\n");
				break;
			}
			break;
		}
	} else if (modScheme == MOD_SCHEME_COHERENT) {
		switch (modType) {
		case MOD_TYPE_BPSK:
			switch (appPlcData.plcBand) {
			case G3_CEN_A:
				APP_CONSOLE_Print(" .............. 18.5 kbit/s\r\n");
				break;

			case G3_FCC:
				if (appPlcData.plcTxObj.rs2Blocks == 0) {
					/* 1 Reed-Solomon block */
					APP_CONSOLE_Print(" .............. 96.6 kbit/s\r\n");
				} else {
					/* 2 Reed-Solomon blocks */
					APP_CONSOLE_Print(" .............. 112 kbit/s\r\n");
				}

				break;

			case G3_CEN_B:
				APP_CONSOLE_Print(" .............. 7.9 kbit/s\r\n");
				break;
			}
			break;

		case MOD_TYPE_QPSK:
			switch (appPlcData.plcBand) {
			case G3_CEN_A:
				APP_CONSOLE_Print(" .............. 31.3 kbit/s\r\n");
				break;

			case G3_FCC:
				if (appPlcData.plcTxObj.rs2Blocks == 0) {
					/* 1 Reed-Solomon block */
					APP_CONSOLE_Print(" .............. 152.3 kbit/s\r\n");
				} else {
					/* 2 Reed-Solomon blocks */
					APP_CONSOLE_Print(" .............. 194 kbit/s\r\n");
				}

				break;

			case G3_CEN_B:
				APP_CONSOLE_Print(" .............. 14.5 kbit/s\r\n");
				break;
			}
			break;

		case MOD_TYPE_8PSK:
			switch (appPlcData.plcBand) {
			case G3_CEN_A:
				APP_CONSOLE_Print(" .............. 41.2 kbit/s\r\n");
				break;

			case G3_FCC:
				if (appPlcData.plcTxObj.rs2Blocks == 0) {
					/* 1 Reed-Solomon block */
					APP_CONSOLE_Print(" .............. 184.2 kbit/s\r\n");
				} else {
					/* 2 Reed-Solomon blocks */
					APP_CONSOLE_Print(" .............. 252.1 kbit/s\r\n");
				}

				break;

			case G3_CEN_B:
				APP_CONSOLE_Print(" .............. 19.6 kbit/s\r\n");
				break;
			}
			break;

		case MOD_TYPE_BPSK_ROBO:
			switch (appPlcData.plcBand) {
			case G3_CEN_A:
				APP_CONSOLE_Print(" ....... 5 kbit/s\r\n");
				break;

			case G3_FCC:
				if (appPlcData.plcTxObj.rs2Blocks == 0) {
					/* 1 Reed-Solomon block */
					APP_CONSOLE_Print(" ....... 31.2 kbit/s\r\n");
				} else {
					/* 2 Reed-Solomon blocks */
					APP_CONSOLE_Print(" ....... 32.8 kbit/s\r\n");
				}

				break;

			case G3_CEN_B:
				APP_CONSOLE_Print(" ....... 1.7 kbit/s\r\n");
				break;
			}
			break;
		}
	}
    
}

static void _APP_CONSOLE_ShowModulation(DRV_PLC_PHY_MOD_SCHEME modScheme,
        DRV_PLC_PHY_MOD_TYPE modType)
{
	if (modScheme == MOD_SCHEME_DIFFERENTIAL) {
		switch (modType) {
		case MOD_TYPE_BPSK:
			APP_CONSOLE_Print("BPSK Differential");
			break;

		case MOD_TYPE_QPSK:
			APP_CONSOLE_Print("QPSK Differential");
			break;

		case MOD_TYPE_8PSK:
			APP_CONSOLE_Print("8PSK Differential");
			break;

		case MOD_TYPE_BPSK_ROBO:
			APP_CONSOLE_Print("BPSK Robust Differential");
			break;

		default:
			APP_CONSOLE_Print("Unknown");
			break;
		}
	} else if (modScheme == MOD_SCHEME_COHERENT) {
		switch (modType) {
		case MOD_TYPE_BPSK:
			APP_CONSOLE_Print("BPSK Coherent");
			break;

		case MOD_TYPE_QPSK:
			APP_CONSOLE_Print("QPSK Coherent");
			break;

		case MOD_TYPE_8PSK:
			APP_CONSOLE_Print("8PSK Coherent");
			break;

		case MOD_TYPE_BPSK_ROBO:
			APP_CONSOLE_Print("BPSK Robust Coherent");
			break;

		default:
			APP_CONSOLE_Print("Unknown");
			break;
		}
	} else {
		APP_CONSOLE_Print("Unknown");
	}
}

static void _APP_CONSOLE_ShowTxParams(DRV_PLC_PHY_MOD_SCHEME modScheme,
        DRV_PLC_PHY_MOD_TYPE modType)
{
	/* Modulation Type & Modulation Scheme */
	APP_CONSOLE_Print("\r\nTx Modulation: ");
	_APP_CONSOLE_ShowModulation(modScheme, modType);

	/* Maximum data length with configured Tx Parameters */
	APP_CONSOLE_Print(" (Max data length = %u bytes)\r\n",
            (uint32_t)appPlcData.maxDataLen);
}

static void _APP_CONSOLE_ShowMenu(bool fullMenu)
{
    switch (appConsoleData.state) {
	case APP_CONSOLE_STATE_WAIT_COMMAND:
		/* Idle state: Show available options */
		if (fullMenu) {
			APP_CONSOLE_Print("\r\nPress 'CTRL+S' to enter configuration menu. ");
			APP_CONSOLE_Print("Enter text and press 'ENTER' to trigger transmission");
		}

		APP_CONSOLE_Print("\r\n>>> ");
		break;

	case APP_CONSOLE_STATE_TYPING:
		/* Typing message to transmit */
		APP_CONSOLE_Print("\r\n>>> %.*s", appConsoleData.txDataIndex - 2, plcTxBuffer + 2);
		break;

	case APP_CONSOLE_STATE_CONFIG_MENU:
		/* Configuration Menu */
		APP_CONSOLE_Print("\r\n--- Configuration Menu ---\r\n");
		APP_CONSOLE_Print("Select parameter to configure: \r\n");
		APP_CONSOLE_Print("\t0: Tx Modulation\r\n");
		APP_CONSOLE_Print("\t1: Tx/Rx Band\r\n");
		APP_CONSOLE_Print(">>> ");
		break;

	case APP_CONSOLE_STATE_CONFIG_MOD:
		/* Modulation Configuration */
		APP_CONSOLE_Print("\r\n--- Tx Modulation Configuration Menu ---\r\n");
		APP_CONSOLE_Print("Select Modulation:\r\n");

		if ((appPlcData.plcTxObj.modScheme == MOD_SCHEME_DIFFERENTIAL) &&
                (appPlcData.plcTxObj.modType == MOD_TYPE_BPSK_ROBO)) {
			APP_CONSOLE_Print("->");
		}

		APP_CONSOLE_Print("\t0: ");
		_APP_CONSOLE_ShowModulation(MOD_SCHEME_DIFFERENTIAL, MOD_TYPE_BPSK_ROBO);
		_APP_CONSOLE_ShowDataRate(MOD_SCHEME_DIFFERENTIAL, MOD_TYPE_BPSK_ROBO);

		if ((appPlcData.plcTxObj.modScheme == MOD_SCHEME_DIFFERENTIAL) &&
                (appPlcData.plcTxObj.modType == MOD_TYPE_BPSK)) {
			APP_CONSOLE_Print("->");
		}

		APP_CONSOLE_Print("\t1: ");
		_APP_CONSOLE_ShowModulation(MOD_SCHEME_DIFFERENTIAL, MOD_TYPE_BPSK);
		_APP_CONSOLE_ShowDataRate(MOD_SCHEME_DIFFERENTIAL, MOD_TYPE_BPSK);

		if ((appPlcData.plcTxObj.modScheme == MOD_SCHEME_DIFFERENTIAL) &&
                (appPlcData.plcTxObj.modType == MOD_TYPE_QPSK)) {
			APP_CONSOLE_Print("->");
		}

		APP_CONSOLE_Print("\t2: ");
		_APP_CONSOLE_ShowModulation(MOD_SCHEME_DIFFERENTIAL, MOD_TYPE_QPSK);
		_APP_CONSOLE_ShowDataRate(MOD_SCHEME_DIFFERENTIAL, MOD_TYPE_QPSK);

		if (appPlcData.plcBand != G3_ARIB) {
			/* 8PSK modulation type and coherent modulation scheme not allowed in ARIB band */
			if ((appPlcData.plcTxObj.modScheme == MOD_SCHEME_DIFFERENTIAL) &&
                    (appPlcData.plcTxObj.modType == MOD_TYPE_8PSK)) {
				APP_CONSOLE_Print("->");
			}

			APP_CONSOLE_Print("\t3: ");
			_APP_CONSOLE_ShowModulation(MOD_SCHEME_DIFFERENTIAL, MOD_TYPE_8PSK);
			_APP_CONSOLE_ShowDataRate(MOD_SCHEME_DIFFERENTIAL, MOD_TYPE_8PSK);

			if ((appPlcData.plcTxObj.modScheme == MOD_SCHEME_COHERENT) &&
                    (appPlcData.plcTxObj.modType == MOD_TYPE_BPSK_ROBO)) {
				APP_CONSOLE_Print("->");
			}

			APP_CONSOLE_Print("\t4: ");
			_APP_CONSOLE_ShowModulation(MOD_SCHEME_COHERENT, MOD_TYPE_BPSK_ROBO);
			_APP_CONSOLE_ShowDataRate(MOD_SCHEME_COHERENT, MOD_TYPE_BPSK_ROBO);

			if ((appPlcData.plcTxObj.modScheme == MOD_SCHEME_COHERENT) &&
                    (appPlcData.plcTxObj.modType == MOD_TYPE_BPSK)) {
				APP_CONSOLE_Print("->");
			}

			APP_CONSOLE_Print("\t5: ");
			_APP_CONSOLE_ShowModulation(MOD_SCHEME_COHERENT, MOD_TYPE_BPSK);
			_APP_CONSOLE_ShowDataRate(MOD_SCHEME_COHERENT, MOD_TYPE_BPSK);

			if ((appPlcData.plcTxObj.modScheme == MOD_SCHEME_COHERENT) &&
                    (appPlcData.plcTxObj.modType == MOD_TYPE_QPSK)) {
				APP_CONSOLE_Print("->");
			}

			APP_CONSOLE_Print("\t6: ");
			_APP_CONSOLE_ShowModulation(MOD_SCHEME_COHERENT, MOD_TYPE_QPSK);
			_APP_CONSOLE_ShowDataRate(MOD_SCHEME_COHERENT, MOD_TYPE_QPSK);

			if ((appPlcData.plcTxObj.modScheme == MOD_SCHEME_COHERENT) &&
                    (appPlcData.plcTxObj.modType == MOD_TYPE_8PSK)) {
				APP_CONSOLE_Print("->");
			}

			APP_CONSOLE_Print("\t7: ");
			_APP_CONSOLE_ShowModulation(MOD_SCHEME_COHERENT, MOD_TYPE_8PSK);
			_APP_CONSOLE_ShowDataRate(MOD_SCHEME_COHERENT, MOD_TYPE_8PSK);
		}

		APP_CONSOLE_Print(">>> ");
		break;

	case APP_CONSOLE_STATE_CONFIG_BAND:
		/* Band configuration */
		APP_CONSOLE_Print("\r\n--- Band Configuration Menu ---\r\n");
		APP_CONSOLE_Print("Select Band:\r\n");

		if (appPlcData.plcBand == G3_CEN_A) {
			APP_CONSOLE_Print("->");
		}

		APP_CONSOLE_Print("\t1: CENELEC-A band (35 - 91 kHz)\r\n");

		if (appPlcData.plcBand == G3_FCC) {
			APP_CONSOLE_Print("->");
		}

		APP_CONSOLE_Print("\t2: FCC band (154 - 488 kHz)\r\n");

		APP_CONSOLE_Print(">>> ");
		break;
        
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
        if (appConsoleData.charProcessedIndex == (CONSOLE_READ_BUFFER_SIZE - 1))
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

void APP_CONSOLE_HandleRxMsgCrcOk(uint8_t *pData, uint16_t dataLen, DRV_PLC_PHY_MOD_SCHEME modScheme, DRV_PLC_PHY_MOD_TYPE modType, uint16_t rssi, uint8_t lqi)
{
	uint16_t msgLen;

	/* Extract real data length from two first bytes */
	/* It does not have to be the same as dataLen because dataLen may contain padding length */
	msgLen = pData[0] << 8;
	msgLen += pData[1];

	if (msgLen > dataLen) {
		/* Length error: length in message content should never be more than total data length from PHY */
		APP_CONSOLE_Print("\rRx ERROR: length error");
	} else {
		APP_CONSOLE_Print("\rRx (");
		/* Show Modulation of received frame */
		_APP_CONSOLE_ShowModulation(modScheme, modType);
		/* Show RSSI (Received Signal Strength Indicator) in dBuV */
		APP_CONSOLE_Print(", RSSI %udBuV", (uint32_t)rssi);
		/* Show LQI (Link Quality Indicator). It is in quarters of dB and 10-dB offset: SNR(dB) = (LQI - 40) / 4 */
		APP_CONSOLE_Print(", LQI %ddB): ", DIV_ROUND((int16_t)lqi - 40, 4));
		/* Show received message */
		APP_CONSOLE_Print("%.*s", msgLen - 2, pData + 2);
	}

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

    len = vsnprintf(printBuffer, PRINT_BUFFER_SIZE - 1, format, args); 

    va_end( args ); 

    if (len > 0 && len < PRINT_BUFFER_SIZE - 1)
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
             * received finishes */
            SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0,
                    _APP_CONSOLE_RcvCharCb, SYS_CONSOLE_EVENT_READ_COMPLETE);
            
            /* Start read operations on SYS_CONSOLE */
            for (uint8_t index = 0; index < CONSOLE_READ_BUFFER_SIZE; index++)
            {
                SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, 0,
                        &charRcvBuffer[index], 1);
            }
            
            /* Show Application Header */
            APP_CONSOLE_Print(STRING_HEADER);

            /* Next Application state: Wait for PLC application initialization */
            appConsoleData.state = APP_CONSOLE_STATE_WAIT_PLC;
                
            break;
        }

        /* Application waiting for initialization of PLC application */
        case APP_CONSOLE_STATE_WAIT_PLC:
        {
            /* Check the state of PLC application */
            if (appPlcData.state == APP_PLC_STATE_WAITING)
            {
                _APP_CONSOLE_ShowTxParams(appPlcData.plcTxObj.modScheme,
                        appPlcData.plcTxObj.modType);
                
                /* Initialize data buffer index to point to third byte. First 2
                 * bytes are used to store message length because Physical Layer
                 * may add padding in order to complete symbols with data. Data
                 * length reported in received frames includes such padding
                 * length. Therefore, it is needed to include data length in
                 * message content in order to know the real data length in
                 * reception */
                appConsoleData.txDataIndex = 2;                
                
                /* Next Application state: Wait for commands from console */
                appConsoleData.state = APP_CONSOLE_STATE_WAIT_COMMAND;
                
                _APP_CONSOLE_ShowMenu(true);
            }

            break;
        }
        
        case APP_CONSOLE_STATE_WAIT_COMMAND:
        {
            char rcvChar;
            
            /* Check if there is a new character received */
            if (_APP_CONSOLE_CheckRcvChar(&rcvChar))
            {
                /* Process the received char */
                switch (rcvChar) {
                    /* Special character: 0x13 (CTRL+S). Enter configuration menu */
                    case 0x13:      
                        if (appPlcData.plcMultiband)
                        {
                            /* Multiband supported: Go to configuration menu to
                             * select Band or Modulation configuration */
                           appConsoleData.state = APP_CONSOLE_STATE_CONFIG_MENU;
                        }
                        else
                        {
                            /* Multiband not supported: Go directly to Modulation configuration */
                            appConsoleData.state = APP_CONSOLE_STATE_CONFIG_MOD;
                        }
                        _APP_CONSOLE_ShowMenu(true);
                        break;

                    /* Special character: '\r' (Carriage Return) or
                     * '\n' (Line Feed). Show menu */
                    case '\r':
                    case '\n':                        
                        _APP_CONSOLE_ShowMenu(true);
                        break;

                    default:
                        /* Normal character: Start to type message if it is
                         * printable (ASCII) */
                        if (_APP_CONSOLE_CheckIsPrintable(rcvChar)) {
                            appConsoleData.state = APP_CONSOLE_STATE_TYPING;
                            plcTxBuffer[appConsoleData.txDataIndex++] = rcvChar;
                        }
                }
            }
            
            break;
        }
        
        case APP_CONSOLE_STATE_TYPING:
        {
            char rcvChar;
            
            /* Check if there is a new character received */
            if (_APP_CONSOLE_CheckRcvChar(&rcvChar))
            {
                /* New character received from console */
                bool sendPlcMsg = false;
                
                /* Process the received char */
                switch (rcvChar) {
                    case '\r':
                        /* Special character: '\r' (carriage return).
                         * Send message through PLC */
                        sendPlcMsg = true;
                        break;

                    case '\b':
                    case 0x7F:
                        /* Special character: '\b' (backspace) or 0x7F (DEL).
                         * Remove character from Tx data buffer */
                        if (appConsoleData.txDataIndex > 2) {
                            appConsoleData.txDataIndex--;
                            APP_CONSOLE_Print("\b \b");
                        }

                        if (appConsoleData.txDataIndex == 2) {
                            /* All text has been removed: Go back to idle state */
                            appConsoleData.state = APP_CONSOLE_STATE_WAIT_COMMAND;
                        }

                        break;

                    default:
                        /* Normal character: Add to Tx data buffer if it is
                         * printable (ASCII) */
                        if (_APP_CONSOLE_CheckIsPrintable(rcvChar))
                        {
                            plcTxBuffer[appConsoleData.txDataIndex++] = rcvChar;
                            if (appConsoleData.txDataIndex == appPlcData.maxDataLen)
                            {
                                /* Maximum data length reached: Send message through PLC */
                                APP_CONSOLE_Print("\r\nMax data length reached"
                                        "... Message will be sent\r\n");
                                sendPlcMsg = true;
                            }
                        }
                    }
                
                if (sendPlcMsg) 
                {
                    /* Fill 2 first bytes with data length. Physical Layer may
                     * add padding bytes in order to complete symbols with data.
                     * It is needed to include real data length in the message
                     * because otherwise at reception is not possible to know if
                     * there is padding or not */
                    plcTxBuffer[0] = appConsoleData.txDataIndex >> 8;
                    plcTxBuffer[1] = appConsoleData.txDataIndex & 0xFF;

                    /* Send PLC message */
                    APP_PLC_SendMsg(plcTxBuffer, appConsoleData.txDataIndex);

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
                /* Initialize data buffer index to point to third byte. First
                 * 2 bytes are used to store message length because Physical
                 * Layer may add padding in order to complete symbols with data.
                 * Data length reported in received frames includes such padding
                 * length. Therefore, it is needed to include data length in
                 * message content in order to know the real data length in
                 * reception */
                appConsoleData.txDataIndex = 2;

                /* Reset Chat App state */
                appConsoleData.state = APP_CONSOLE_STATE_WAIT_COMMAND;

                _APP_CONSOLE_ShowMenu(false);
            }
            break;
        }
        
        case APP_CONSOLE_STATE_CONFIG_MENU:
        {
            char rcvChar;
            
            /* Check if there is a new character received */
            if (_APP_CONSOLE_CheckRcvChar(&rcvChar))
            {
                /* New character received from console */
                switch (rcvChar) {
                    case '0':
                        /* Modulation configuration */
                        appConsoleData.state = APP_CONSOLE_STATE_CONFIG_MOD;
                        _APP_CONSOLE_ShowMenu(true);
                        break;

                    case '1':
                        /* Band configuration */
                        appConsoleData.state = APP_CONSOLE_STATE_CONFIG_BAND;
                        _APP_CONSOLE_ShowMenu(true);
                        break;

                    default:
                        APP_CONSOLE_Print("\r\nUnknown command. Skipping configuration\r\n");
                        appConsoleData.state = APP_CONSOLE_STATE_WAIT_COMMAND;
                        break;
                }
                
            }
			break;
        }
        
         case APP_CONSOLE_STATE_CONFIG_MOD:
        {
            char rcvChar;
            
            /* Check if there is a new character received */
            if (_APP_CONSOLE_CheckRcvChar(&rcvChar))
            {
                /* New character received from console */
                bool validModulation = true;
                DRV_PLC_PHY_MOD_TYPE modType;
                DRV_PLC_PHY_MOD_SCHEME modScheme;
                
                switch (rcvChar) {
                    case '0':
                        /* BPSK_ROBO_D */
                        modType = MOD_TYPE_BPSK_ROBO;
                        modScheme = MOD_SCHEME_DIFFERENTIAL;
                        break;

                    case '1':
                        /* BPSK_D */
                        modType = MOD_TYPE_BPSK;
                        modScheme = MOD_SCHEME_DIFFERENTIAL;
                        break;

                    case '2':
                        /* QPSK_D */
                        modType = MOD_TYPE_QPSK;
                        modScheme = MOD_SCHEME_DIFFERENTIAL;
                        break;

                    case '3':
                        /* 8PSK_D */
                        modType = MOD_TYPE_8PSK;
                        modScheme = MOD_SCHEME_DIFFERENTIAL;
                        if (appPlcData.plcBand == G3_ARIB) {
                            /* 8PSK Modulation Type not supported in ARIB band */
                            APP_CONSOLE_Print("\r\n8PSK modulation not supported in ARIB band. Skipping configuration\r\n");
                            validModulation = false;
                        }

                        break;

                    case '4':
                        /* BPSK_ROBO_C */
                        modType = MOD_TYPE_BPSK_ROBO;
                        modScheme = MOD_SCHEME_COHERENT;
                        if (appPlcData.plcBand == G3_ARIB) {
                            /* Coherent Modulation Scheme not supported in ARIB band */
                            APP_CONSOLE_Print("\r\nCoherent modulation not supported in ARIB band. Skipping configuration\r\n");
                            validModulation = false;
                        }

                        break;

                    case '5':
                        /* BPSK_C */
                        modType = MOD_TYPE_BPSK;
                        modScheme = MOD_SCHEME_COHERENT;
                        if (appPlcData.plcBand == G3_ARIB) {
                            /* Coherent Modulation Scheme not supported in ARIB band */
                            APP_CONSOLE_Print("\r\nCoherent modulation not supported in ARIB band. Skipping configuration\r\n");
                            validModulation = false;
                        }

                        break;

                    case '6':
                        /* QPSK_C */
                        modType = MOD_TYPE_QPSK;
                        modScheme = MOD_SCHEME_COHERENT;
                        if (appPlcData.plcBand == G3_ARIB) {
                            /* Coherent Modulation Scheme not supported in ARIB band */
                            APP_CONSOLE_Print("\r\nCoherent modulation not supported in ARIB band. Skipping configuration\r\n");
                            validModulation = false;
                        }

                        break;

                    case '7':
                        /* 8PSK_C */
                        modType = MOD_TYPE_8PSK;
                        modScheme = MOD_SCHEME_COHERENT;
                        if (appPlcData.plcBand == G3_ARIB) {
                            /* Coherent Modulation Scheme not supported in ARIB band */
                            APP_CONSOLE_Print("\r\nCoherent modulation not supported in ARIB band. Skipping configuration\r\n");
                            validModulation = false;
                        }

                        break;

                    default:
                        APP_CONSOLE_Print("\r\nUnknown command. Skipping configuration\r\n");
                        validModulation = false;
                        break;
                    }

                    if (validModulation) {
                        /* Set new Modulation Type & Modulation Scheme. Get maximum data length allowed with configured Tx Parameters */
                        APP_PLC_SetModulation(modScheme, modType);
                        _APP_CONSOLE_ShowTxParams(modScheme, modType);
                    }

                    appConsoleData.state = APP_CONSOLE_STATE_WAIT_COMMAND;
                    _APP_CONSOLE_ShowMenu(false);                    
            }
			break;
        }
         
        case APP_CONSOLE_STATE_CONFIG_BAND:
        {
            char rcvChar;
            
            /* Check if there is a new character received */
            if (_APP_CONSOLE_CheckRcvChar(&rcvChar))
            {
                /* New character received from console */
                bool validBand = true;
                uint8_t newBand;
                
                switch (rcvChar) {
                    case '1':
                        /* CENELEC-A */
                        newBand = G3_CEN_A;
                        break;

                    case '2':
                        /* CENELEC-A */
                        newBand = G3_FCC;
                        break;

                    default:
                        APP_CONSOLE_Print("\r\nUnknown command. Skipping configuration\r\n");
                        validBand = false;
                        break;
                }

                if (validBand) {
                    /* Set new Band on PL360 */
                    validBand = APP_PLC_SetBand(newBand);
                }
                
                if (validBand)
                {
                    appConsoleData.state = APP_CONSOLE_STATE_WAIT_PLC;
                }
                else
                {
                    appConsoleData.state = APP_CONSOLE_STATE_WAIT_COMMAND;
                    _APP_CONSOLE_ShowMenu(false);
                }
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
