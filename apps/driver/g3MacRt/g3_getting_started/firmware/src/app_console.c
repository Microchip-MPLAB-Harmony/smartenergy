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

#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
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

bool APP_CONSOLE_CheckIsPrintable(char data);

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
    /* ErrorGet clears errors */
    if (buffer == (void *)appConsole.pNextChar) 
    {
        if (appConsole.echoOn)
        {
            appConsole.newEcho = true;
        }
            
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

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

static uint32_t APP_CONSOLE_ReadSerialChar(bool restart)
{
    if (restart)
    {
        /* Init data pointer */
        appConsole.pNextChar = appConsole.pReceivedChar;
        appConsole.dataLength = 0;
    }
    
    appConsole.numCharToReceive = 1;
    
    if (SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, 0, (void*)appConsole.pNextChar, 1))
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

static bool APP_CONSOLE_GetShortAddress(char *pData, uint16_t *pAddress)
{
    uint8_t index;
    uint8_t value;
    uint16_t shortAddress;
    
    shortAddress = 0;
    
    for(index = 0; index < 4; index++)
    {
        shortAddress <<= 4;
        value = *(uint8_t *)pData;
        if ((value >= '0') && (value <= '9')) 
        {
			value -= 0x30;
        }
        else if ((value >= 'A') && (value <= 'F')) 
        {
            value -= 0x37;
        } 
        else if ((value >= 'a') && (value <= 'f')) 
        {
            value -= 0x57;
        } 
        else
        {
            return false;
        }
        
        shortAddress += value;
        pData++;
    }
    
    *pAddress = shortAddress;
    
    return true;
}

bool APP_CONSOLE_CheckIsPrintable(char data)
{
    bool isPrintable = 0;
    
    if (((data >= 32) && (data <= 126)) || 
        ((data >= 128) && (data <= 254)) || 
         (data == '\t') || (data == '\n')) 
    {
        isPrintable = 1;
    }
    
    return isPrintable;
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
    
    /* Enable ECHO */
    appConsole.echoOn = true;
    appConsole.newEcho = false;
    
}

/******************************************************************************
  Function:
    void APP_CONSOLE_Tasks ( void )

  Remarks:
    See prototype in app_console.h.
 */

void APP_CONSOLE_Tasks ( void )
{
    char *pData;
    
    /* Refresh WDG */
    //WDT_Clear();
    
    /* ECHO handle */
    if (appConsole.newEcho)
    {
        pData = appConsole.pNextChar - 1;
        
        if (APP_CONSOLE_CheckIsPrintable(*pData))
        {
            APP_CONSOLE_Print("%c", (unsigned int)*pData);
        }
        
        appConsole.newEcho = false;
    }

    /* Check the application's current state. */
    switch ( appConsole.state )
    {
        /* Application's initial state. */
        case APP_CONSOLE_STATE_INIT:
        {
            /* Configure USART Callback */
            SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0, APP_CONSOLE_ReadCallback, 
                    SYS_CONSOLE_EVENT_READ_COMPLETE);
            
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
                /* Waiting Console type */
                APP_CONSOLE_Print("\r\nPress 'CTRL+S' to enter configuration menu. ");
                APP_CONSOLE_Print("\r\nEnter text and press 'ENTER' to trigger transmission");
                APP_CONSOLE_Print("\r\n>>> ");
                appConsole.state = APP_CONSOLE_STATE_CONSOLE;
            
                /* Launch initial Reception job */
                appConsole.dataLength = 0;
                APP_CONSOLE_ReadSerialChar(true);
            }
            break;
        }
            
        case APP_CONSOLE_STATE_PLC_SEND:
        {
            /* Wait for PLC transceiver initialization */
            if (appPlc.state == APP_PLC_STATE_WAITING)
            {     
                /* Waiting Console type */
                APP_CONSOLE_Print("\r\n>>> ");
                appConsole.state = APP_CONSOLE_STATE_CONSOLE;
            
                /* Launch initial Reception job */
                appConsole.dataLength = 0;
                APP_CONSOLE_ReadSerialChar(true);
            }
            break;
        }

        case APP_CONSOLE_STATE_CONSOLE:
        {
            if (appConsole.numCharToReceive == 0)
            {
                pData = appConsole.pNextChar - 1;
                
                switch(*pData)
                {
                    case 0x13: // (CTRL+S) : Configuration Menu
                        appConsole.state = APP_CONSOLE_STATE_CONFIG_MENU;
                        APP_CONSOLE_Print("\r\n--- Configuration Menu ---");
                        APP_CONSOLE_Print("\r\nSelect parameter to configure:");
                        APP_CONSOLE_Print("\r\n\t0: G3 MAC Tx Modulation");
                        APP_CONSOLE_Print("\r\n\t1: G3 MAC Source Address");
                        APP_CONSOLE_Print("\r\n\t2: G3 MAC Destination Address");
                        APP_CONSOLE_Print("\r\n>>> ");
                        APP_CONSOLE_ReadSerialChar(true);
                        break;

                    case '\r': // (\r) : Send G3 MAC RT message
                        /* Send G3 MAC RT message */
                        APP_PLC_SendMessage((uint8_t *)appConsole.pReceivedChar, 
                                appConsole.dataLength);
                        
                        /* Wait to PLC application: TX CFM */
                        appConsole.state = APP_CONSOLE_STATE_PLC_SEND;
                        break;
                        
                    case '\b': // (\b): Backspace
                    case 0x7F: // DEL
                        /* Check BS character (0x08) */
                        if (appConsole.dataLength > 0)
                        {
                            /* Update reception data */
                            appConsole.pNextChar -= 2;
                            appConsole.dataLength -= 1;
                            APP_CONSOLE_Print("\b \b");
                            /* Wait to next character */
                            APP_CONSOLE_ReadSerialChar(false);
                        }
                        break;

                    default:
                        // Check printable character
                        if (APP_CONSOLE_CheckIsPrintable(*pData))
                        {
                            /* Check MAX length */
                            if (appConsole.numCharToReceive >= MAC_RT_MAX_PAYLOAD_SIZE)
                            {
                                /* Send G3 MAC RT message */
                                APP_PLC_SendMessage((uint8_t *)appConsole.pReceivedChar, 
                                        appConsole.dataLength);
                                
                                /* Wait to PLC application: TX CFM */
                                appConsole.state = APP_CONSOLE_STATE_PLC_SEND;
                            }
                            else
                            {
                                /* Wait to next character */
                                APP_CONSOLE_ReadSerialChar(false);
                            }
                        }
                        else
                        {
                            /* Discard character */  
                            APP_CONSOLE_ReadSerialChar(true);
                        }                        
                        break;
                }
            }
            break;
        }

        case APP_CONSOLE_STATE_CONFIG_MENU:
        {
            if (appConsole.numCharToReceive == 0)
            {
                switch(*appConsole.pReceivedChar)
                {
                        
                    case '0': // G3 MAC Tx Modulation
                        appConsole.state = APP_CONSOLE_STATE_SET_MODULATION;
                        APP_CONSOLE_Print("\r\n--- Tx Modulation Configuration Menu ---");
                        APP_CONSOLE_Print("\r\nSelect Modulation:");
                        APP_CONSOLE_Print("\r\n\t0: BPSK Robust Differential");
                        APP_CONSOLE_Print("\r\n\t1: BPSK Differential");
                        APP_CONSOLE_Print("\r\n\t2: QPSK Differential");
                        APP_CONSOLE_Print("\r\n\t3: 8PSK Differential");
                        APP_CONSOLE_Print("\r\n\t4: BPSK Robust Coherent");
                        APP_CONSOLE_Print("\r\n\t5: BPSK Coherent");
                        APP_CONSOLE_Print("\r\n\t6: QPSK Coherent");
                        APP_CONSOLE_Print("\r\n\t7: 8PSK Coherent");
                        APP_CONSOLE_Print("\r\n>>> ");
                        APP_CONSOLE_ReadSerialChar(true);
                        break;
                        
                    case '1': // G3 MAC Source Address
                        appConsole.state = APP_CONSOLE_STATE_SET_SRC_ADDRESS;
                        APP_CONSOLE_Print("\r\nIntroduce new source address [0x0000]: 0x");
                        APP_CONSOLE_ReadSerialChar(true);
                        break;
                        
                    case '2': // G3 MAC Destination Address
                        appConsole.state = APP_CONSOLE_STATE_SET_DST_ADDRESS;
                        APP_CONSOLE_Print("\r\nIntroduce new destination address [0x0000]: 0x");
                        APP_CONSOLE_ReadSerialChar(true);
                        break;
                        
                    default:
                        APP_CONSOLE_Print("\r\nUnknown command. Skipping configuration\r\n");
                        appConsole.state = APP_CONSOLE_STATE_CONSOLE;
                        APP_CONSOLE_ReadSerialChar(true);
                }
            }  
            break;
        }

        case APP_CONSOLE_STATE_SET_SRC_ADDRESS:
        {
            if (appConsole.dataLength == 4)
            {
                uint16_t shortAddress;
                
                if (APP_CONSOLE_GetShortAddress(appConsole.pReceivedChar, &shortAddress))
                {
                    appPlc.sourceAddress = shortAddress;
                    APP_CONSOLE_Print("\r\nSet G3 MAC RT Source Address: 0x%04x", shortAddress);
                }
                else
                {
                    APP_CONSOLE_Print("\r\nShort Address not valid. Use hexadecimal values.");
                }
                
                APP_CONSOLE_Print("\r\n>>> ");
                appConsole.state = APP_CONSOLE_STATE_CONSOLE;
                APP_CONSOLE_ReadSerialChar(true);
            }
            else
            {
                if (appConsole.numCharToReceive == 0)
                {
                    /* Receive next char */
                    APP_CONSOLE_ReadSerialChar(false);
                }
            }
            break;
        }

        case APP_CONSOLE_STATE_SET_DST_ADDRESS:
        {
            if (appConsole.dataLength == 4)
            {
                uint16_t shortAddress;
                
                if (APP_CONSOLE_GetShortAddress(appConsole.pReceivedChar, &shortAddress))
                {
                    appPlc.destinationAddress = shortAddress;
                    APP_CONSOLE_Print("\r\nSet G3 MAC RT Destination Address: 0x%04x", shortAddress);
                }
                else
                {
                    APP_CONSOLE_Print("\r\nShort Address not valid. Use hexadecimal values.");
                }
                
                APP_CONSOLE_Print("\r\n>>> ");
                appConsole.state = APP_CONSOLE_STATE_CONSOLE;
                APP_CONSOLE_ReadSerialChar(true);
            }
            else
            {
                if (appConsole.numCharToReceive == 0)
                {
                    /* Receive next char */
                    APP_CONSOLE_ReadSerialChar(false);
                }
            }
            break;
        }
         
        case APP_CONSOLE_STATE_SET_MODULATION:
        {
            if (appConsole.numCharToReceive == 0)
            {
                switch(*appConsole.pReceivedChar)
                {
                    case '0': // BPSK Robust Differential
                        appPlc.g3MacRtTxParameters.modScheme = MAC_RT_MOD_SCHEME_DIFFERENTIAL;
                        appPlc.g3MacRtTxParameters.modType = MAC_RT_MOD_ROBUST;
                        APP_CONSOLE_Print("\r\nSet modulation: BPSK Robust Differential");
                        break;
                        
                    case '1': // BPSK Differential
                        appPlc.g3MacRtTxParameters.modScheme = MAC_RT_MOD_SCHEME_DIFFERENTIAL;
                        appPlc.g3MacRtTxParameters.modType = MAC_RT_MOD_BPSK;
                        APP_CONSOLE_Print("\r\nSet modulation: BPSK Differential");
                        break;
                        
                    case '2': // QPSK Differential
                        appPlc.g3MacRtTxParameters.modScheme = MAC_RT_MOD_SCHEME_DIFFERENTIAL;
                        appPlc.g3MacRtTxParameters.modType = MAC_RT_MOD_QPSK;
                        APP_CONSOLE_Print("\r\nSet modulation: QPSK Differential");
                        break;
                        
                    case '3': // 8PSK Differential
                        appPlc.g3MacRtTxParameters.modScheme = MAC_RT_MOD_SCHEME_DIFFERENTIAL;
                        appPlc.g3MacRtTxParameters.modType = MAC_RT_MOD_8PSK;
                        APP_CONSOLE_Print("\r\nSet modulation: 8PSK Differential");
                        break;
                        
                    case '4': // BPSK Robust Coherent
                        appPlc.g3MacRtTxParameters.modScheme = MAC_RT_MOD_SCHEME_COHERENT;
                        appPlc.g3MacRtTxParameters.modType = MAC_RT_MOD_ROBUST;
                        APP_CONSOLE_Print("\r\nSet modulation: BPSK Robust Coherent");
                        break;
                        
                    case '5': // BPSK Coherent
                        appPlc.g3MacRtTxParameters.modScheme = MAC_RT_MOD_SCHEME_COHERENT;
                        appPlc.g3MacRtTxParameters.modType = MAC_RT_MOD_BPSK;
                        APP_CONSOLE_Print("\r\nSet modulation: BPSK Coherent");
                        break;
                        
                    case '6': // QPSK Coherent
                        appPlc.g3MacRtTxParameters.modScheme = MAC_RT_MOD_SCHEME_COHERENT;
                        appPlc.g3MacRtTxParameters.modType = MAC_RT_MOD_QPSK;
                        APP_CONSOLE_Print("\r\nSet modulation: QPSK Coherent");
                        break;
                        
                    case '7': // 8PSK Coherent
                        appPlc.g3MacRtTxParameters.modScheme = MAC_RT_MOD_SCHEME_COHERENT;
                        appPlc.g3MacRtTxParameters.modType = MAC_RT_MOD_8PSK;
                        APP_CONSOLE_Print("\r\nSet modulation: 8PSK Coherent");
                        break;
                        
                    default:
                        APP_CONSOLE_Print("\r\nUnknown modulation. Skipping configuration\r\n");
                }
                
                APP_CONSOLE_Print("\r\n>>> ");
                appConsole.state = APP_CONSOLE_STATE_CONSOLE;
                APP_CONSOLE_ReadSerialChar(true);
            }  
            break;
        }

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
    
    while (SYS_CONSOLE_Status(sysObj.sysConsole0) != SYS_STATUS_READY);

    va_start( args, format );
    len = vsnprintf(appConsole.pTrasmitChar, SERIAL_BUFFER_SIZE - 1, format, args);
    va_end( args );

    if (len > 0 && len < SERIAL_BUFFER_SIZE - 1)
    {
        appConsole.pTrasmitChar[len] = '\0';

        SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, 0, appConsole.pTrasmitChar, len);
    }
}

/*******************************************************************************
 End of File
 */
