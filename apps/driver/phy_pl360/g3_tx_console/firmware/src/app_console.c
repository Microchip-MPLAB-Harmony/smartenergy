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
#include <math.h>
#include "definitions.h"

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
			appPlcTx.pl360Tx.modType = MOD_TYPE_BPSK;
			appPlcTx.pl360Tx.modScheme = MOD_SCHEME_DIFFERENTIAL;
			break;

		case '1':
			appPlcTx.pl360Tx.modType = MOD_TYPE_QPSK;
			appPlcTx.pl360Tx.modScheme = MOD_SCHEME_DIFFERENTIAL;
			break;

		case '2':
			appPlcTx.pl360Tx.modType = MOD_TYPE_8PSK;
			appPlcTx.pl360Tx.modScheme = MOD_SCHEME_DIFFERENTIAL;
			break;

		case '3':
			appPlcTx.pl360Tx.modType = MOD_TYPE_BPSK_ROBO;
			appPlcTx.pl360Tx.modScheme = MOD_SCHEME_DIFFERENTIAL;
			break;

		case '4':
			appPlcTx.pl360Tx.modType = MOD_TYPE_BPSK;
			appPlcTx.pl360Tx.modScheme = MOD_SCHEME_COHERENT;
			break;

		case '5':
			appPlcTx.pl360Tx.modType = MOD_TYPE_QPSK;
			appPlcTx.pl360Tx.modScheme = MOD_SCHEME_COHERENT;
			break;

		case '6':
			appPlcTx.pl360Tx.modType = MOD_TYPE_8PSK;
			appPlcTx.pl360Tx.modScheme = MOD_SCHEME_COHERENT;
			break;

		case '7':
			appPlcTx.pl360Tx.modType = MOD_TYPE_BPSK_ROBO;
			appPlcTx.pl360Tx.modScheme = MOD_SCHEME_COHERENT;
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

static bool APP_CONSOLE_SetToneMap(char *toneMap, size_t length)
{
    uint8_t index;
    uint8_t* pToneMapTx;
    bool res = true;
    
    pToneMapTx = appPlcTx.pl360Tx.toneMap;
    memset(pToneMapTx, 0, sizeof(appPlcTx.pl360Tx.toneMap));

    for(index = 0; index < length * 2; index++, toneMap++)
    {
        if (*toneMap >= 0x30 && *toneMap < 0x40) { 
            /* 0 to 9 */
            if (index % 2) {
                *pToneMapTx++ += *toneMap - 0x30;
            } else {
                *pToneMapTx = (*toneMap - 0x30) << 4;
            }
        } else if (*toneMap >= 0x41 && *toneMap < 0x47) {
            /* A to F */
            if (index % 2) {
                *pToneMapTx++ += *toneMap - 0x37;
            } else {
                *pToneMapTx = (*toneMap - 0x37) << 4;
            }
        } else if (*toneMap >= 0x61 && *toneMap < 0x67) {
            /* a to f */
            if (index % 2) {
                *pToneMapTx++ += *toneMap - 0x57;
            } else {
                *pToneMapTx = (*toneMap - 0x57) << 4;
            }
        } else {
            res = false;
            break;
        }
    }
    
    return res;
}

static bool APP_CONSOLE_SetPreemphasis(char *preemphasis, size_t length)
{
    uint8_t index;
    uint8_t* pPreemphasisTx;
    bool res = true;
    
    pPreemphasisTx = appPlcTx.pl360Tx.preemphasis;
    memset(pPreemphasisTx, 0, sizeof(appPlcTx.pl360Tx.preemphasis));

    for(index = 0; index < length * 2; index++, preemphasis++)
    {
        if (*preemphasis >= 0x30 && *preemphasis < 0x40) { 
            /* 0 to 9 */
            if (index % 2) {
                *pPreemphasisTx++ += *preemphasis - 0x30;
            } else {
                *pPreemphasisTx = (*preemphasis - 0x30) << 4;
            }
        } else if (*preemphasis >= 0x41 && *preemphasis < 0x47) {
            /* A to F */
            if (index % 2) {
                *pPreemphasisTx++ += *preemphasis - 0x37;
            } else {
                *pPreemphasisTx = (*preemphasis - 0x37) << 4;
            }
        } else if (*preemphasis >= 0x61 && *preemphasis < 0x67) {
            /* a to f */
            if (index % 2) {
                *pPreemphasisTx++ += *preemphasis - 0x57;
            } else {
                *pPreemphasisTx = (*preemphasis - 0x57) << 4;
            }
        } else {
            res = false;
            break;
        }
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

static void APP_CONSOLE_ShowConfiguration(void)
{
    uint8_t index;
    
    printf("\n\r-- Configuration Info --------------\r\n");
    printf("-I- PHY Version: 0x%08X\n\r", (unsigned int)appPlcTx.pl360PhyVersion);
    printf("-I- TX Attenuation: %u\n\r", (unsigned int)appPlcTx.pl360Tx.attenuation);
    switch (appPlcTx.pl360Tx.modType) 
    {
        case MOD_TYPE_BPSK:
            if (appPlcTx.pl360Tx.modScheme) 
            {
                printf("-I- Modulation Scheme: Coherent BPSK \n\r");
            }
            else
            {
                printf("-I- Modulation Scheme: Differential BPSK \n\r");
            }
		break;

        case MOD_TYPE_QPSK:
            if (appPlcTx.pl360Tx.modScheme) 
            {
                printf("-I- Modulation Scheme: Coherent QPSK \n\r");
            }
            else
            {
                printf("-I- Modulation Scheme: Differential QPSK \n\r");
            }
            break;

        case MOD_TYPE_8PSK:
            if (appPlcTx.pl360Tx.modScheme) 
            {
                printf("-I- Modulation Scheme: Coherent 8PSK \n\r");
            } 
            else 
            {
                printf("-I- Modulation Scheme: Differential 8PSK \n\r");
            }
            break;

        case MOD_TYPE_BPSK_ROBO:
            if (appPlcTx.pl360Tx.modScheme) 
            {
                printf("-I- Modulation Scheme: Coherent Robust\n\r");
            } 
            else 
            {
                printf("-I- Modulation Scheme: Differential Robust\n\r");
            }
            break;

        default:
            printf("-I-  Modulation Scheme: ERROR CFG\n\r");
	}
    
    printf("-I- Tone Map: ");
	for (index = 0; index < appPlcTx.toneMapSize; index++) {
		printf("%02X", appPlcTx.pl360Tx.toneMap[index]);
	}
	printf("\r\n");
    
    printf("-I- Preemphasis: ");
	for (index = 0; index < appPlcTx.preemphasisSize; index++) {
		printf("%02X", appPlcTx.pl360Tx.preemphasis[index]);
	}
	printf("\r\n");
    
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
    See prototype in app.h.
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
    See prototype in app.h.
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
                        appConsole.state = APP_CONSOLE_STATE_SET_ATT_LEVEL;
                        printf("\r\nEnter attenuation level using 2 digits [00..31] : ");
                        APP_CONSOLE_ReadSerialChar(2, true);
                        break;

                    case '1':
                        appConsole.state = APP_CONSOLE_STATE_SET_SCHEME;
                        printf(MENU_SCHEME);
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                    case '2':
                        printf("\r\nEnter transmission period in us. (max. 10 digits and value min 2100 us): ");
                        APP_CONSOLE_ReadSerialChar(1, true);
                        appConsole.state = APP_CONSOLE_STATE_SET_TIME_PERIOD;
                        break;

                    case '3':
                        printf("\r\nEnter length of data to transmit in bytes (max. 512 bytes): ");
                        appConsole.state = APP_CONSOLE_STATE_SET_DATA_LEN;       
                        APP_CONSOLE_ReadSerialChar(1, true);
                        break;

                    case '4':
                        if (((appPlcTx.pl360PhyVersion >> 16) & 0xFF) == 0x01)
                        {
                            printf("\r\nEnter enter value for tone map. CENA(01 - 3F) : ");
                            appPlcTx.toneMapSize = 1;
                        }
                        else if (((appPlcTx.pl360PhyVersion >> 16) & 0xFF) == 0x02)
                        {
                            printf("\r\nEnter enter value for tone map. FCC(000001 - FFFFFF) : ");
                            appPlcTx.toneMapSize = 3;
                        }
                        else if (((appPlcTx.pl360PhyVersion >> 16) & 0xFF) == 0x04)
                        {
                            printf("\r\nEnter enter value for tone map. CENB(01 - 0F) : ");
                            appPlcTx.toneMapSize = 1;
                        }
                        
                        APP_CONSOLE_ReadSerialChar(appPlcTx.toneMapSize << 1, true);                        
                        appConsole.state = APP_CONSOLE_STATE_SET_TONE_MAP;
                        break;

                    case '5':
                        if (((appPlcTx.pl360PhyVersion >> 16) & 0xFF) == 0x01)
                        {
                            printf("\r\nEnter enter value for preemphasis. CENA(6 bytes) : ");
                            appPlcTx.preemphasisSize = 6;
                        }
                        else if (((appPlcTx.pl360PhyVersion >> 16) & 0xFF) == 0x02)
                        {
                            printf("\r\nEnter enter value for preemphasis. FCC(24 bytes) : ");
                            appPlcTx.preemphasisSize = 24;
                        }
                        else if (((appPlcTx.pl360PhyVersion >> 16) & 0xFF) == 0x04)
                        {
                            printf("\r\nEnter enter value for preemphasis. CENB(3 bytes) : ");
                            appPlcTx.preemphasisSize = 3;
                        }
                        
                        APP_CONSOLE_ReadSerialChar(appPlcTx.preemphasisSize << 1, true);     
                        appConsole.state = APP_CONSOLE_STATE_SET_PREEMPHASIS;
                        break;

                    case '6':
                        printf(MENU_BRANCH_MODE);                        
                        APP_CONSOLE_ReadSerialChar(1, true);
                        appConsole.state = APP_CONSOLE_STATE_SET_BRANCH_MODE;
                        break;

                    case '7':
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
                    printf("\r\nSet Type: %u, Scheme: %u \r\n", 
                            (unsigned int)appPlcTx.pl360Tx.modType,
                            (unsigned int)appPlcTx.pl360Tx.modScheme);
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

        case APP_CONSOLE_STATE_SET_TIME_PERIOD:
        {
            if ((*(appConsole.pNextChar - 1) == '\r') ||
                (*(appConsole.pNextChar - 1) == '\n') ||
                ((appConsole.pNextChar - appConsole.pReceivedChar) == 10))
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

        case APP_CONSOLE_STATE_SET_TONE_MAP:
        {
            if (appConsole.numCharToReceive == 0)
            {
                if (APP_CONSOLE_SetToneMap(appConsole.pReceivedChar, appPlcTx.toneMapSize))
                {
                    if (appPlcTx.toneMapSize == 3)
                    {
                        printf("\r\nSet ToneMap: %02x%02x%02x \r\n", 
                            (unsigned int)appPlcTx.pl360Tx.toneMap[0],
                            (unsigned int)appPlcTx.pl360Tx.toneMap[1],
                            (unsigned int)appPlcTx.pl360Tx.toneMap[2]);
                    }
                    else
                    {
                        printf("\r\nSet ToneMap: %02X \r\n", 
                            (unsigned int)appPlcTx.pl360Tx.toneMap[0]);
                    }
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: ToneMap not permitted. Try again : ");
                    APP_CONSOLE_ReadSerialChar(appPlcTx.toneMapSize << 1, true);  
                }
            }  
            break;
        }

        case APP_CONSOLE_STATE_SET_PREEMPHASIS:
        {
            if (appConsole.numCharToReceive == 0)
            {
                if (APP_CONSOLE_SetPreemphasis(appConsole.pReceivedChar, appPlcTx.preemphasisSize))
                {
                    printf("\r\nSet Preemphasis values successfully\r\n");
                    appConsole.state = APP_CONSOLE_STATE_SHOW_MENU;
                }
                else
                {
                    /* Try it again */
                    printf("\r\nERROR: Preemphasis values not permitted. Try again : ");
                    APP_CONSOLE_ReadSerialChar(appPlcTx.preemphasisSize << 1, true);  
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
