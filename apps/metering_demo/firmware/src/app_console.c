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
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define CONSOLE_HEADER   "\r\n-- Microchip Demo Meter --\r\n" \
	"-- Compiled: "__DATE__ " "__TIME__ " -- \r\n"

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

APP_CONSOLE_DATA app_consoleData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
static void Command_BUF(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_BUF\n\r");
}

static void Command_CAL(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_CAL\n\r");
}

static void Command_CNF(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_CNF\n\r");
}

static void Command_DAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_DAR\n\r");
}

static void Command_DCB(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_DCB\n\r");
}

static void Command_DCD(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_DCD\n\r");
}

static void Command_DCM(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_DCM\n\r");
}

static void Command_DCR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_DCR\n\r");
}

static void Command_DCS(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_DCS\n\r");
}

static void Command_DCW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_DCW\n\r");
}

static void Command_DSR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_DSR\n\r");
}

static void Command_ENC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_ENC\n\r");
}

static void Command_ENR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_ENR\n\r");
}

static void Command_EVEC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_EVEC\n\r");
}

static void Command_EVER(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_EVER\n\r");
}

static void Command_HAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_HAR\n\r");
}

static void Command_HRR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_HRR\n\r");
}

static void Command_IDR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_IDR\n\r");
}

static void Command_IDW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_IDW\n\r");
}

static void Command_MDC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_MDC\n\r");
}

static void Command_MDR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_MDR\n\r");
}

static void Command_PAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_PAR\n\r");
}

static void Command_RTCR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_RTCR\n\r");
}

static void Command_RTCW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_RTCW\n\r");
}

static void Command_TOUR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_TOUR\n\r");
}

static void Command_TOUW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_TOUW\n\r");
}

static void Command_RST(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_RST\n\r");
}

static void Command_RLD(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Command_RLD\n\r");
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

static const SYS_CMD_DESCRIPTOR appCmdTbl[]=
{
    {"BUF", Command_BUF, ": Read waveform capture data (if a parameter is used, only a 512 samples sector is returned)"},
    {"CAL", Command_CAL, ": Automatic calibration"},
    {"CNF", Command_CNF, ": Automatic configuration"},
    {"DAR", Command_DAR, ": Read DSP_ACC register"},
    {"DCB", Command_DCB, ": Go to low power mode"},
    {"DCD", Command_DCD, ": Load default metrology control register values"},
    {"DCM", Command_DCM, ": Write DSP_CONTROL several registers"},
    {"DCR", Command_DCR, ": Read DSP_CONTROL registers"},
    {"DCS", Command_DCS, ": Save metrology constants to non volatile memory"},
    {"DCW", Command_DCW, ": Write DSP_CONTROL register"},
    {"DSR", Command_DSR, ": Read DSP_ST register"},
    {"ENC", Command_ENC, ": Clear all energy"},
    {"ENR", Command_ENR, ": Read energy"},
    {"EVEC",Command_EVEC, ": Clear all event record"},
    {"EVER",Command_EVER, ": Read single event record"},
    {"HAR", Command_HAR, ": Read harmonic register"},
    {"HRR", Command_HRR, ": Read harmonic Irms/Vrms"},
    {"IDR", Command_IDR, ": Read meter id"},
    {"IDW", Command_IDW, ": Write meter id (id length limited to 6 characters)"},
    {"MDC", Command_MDC, ": Clear all maxim demand and happen time"},
    {"MDR", Command_MDR, ": Read maxim demand"},
    {"PAR", Command_PAR, ": Read measure parameter"},
    {"RTCR",Command_RTCR, ": Read meter RTC"},
    {"RTCW",Command_RTCW, ": Write meter RTC"},
    {"TOUR",Command_TOUR, ": Read meter TOU"},
    {"TOUW",Command_TOUW, ": Write meter TOU"},
    {"RST", Command_RST, ": System reset"},
    {"RLD", Command_RLD, ": Reload Metrology Coprocessor"}
};


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
    app_consoleData.state = APP_CONSOLE_STATE_INIT;

    if (!SYS_CMD_ADDGRP(appCmdTbl, sizeof(appCmdTbl)/sizeof(*appCmdTbl), "App Console", ": Metering console commands"))
    {
        SYS_CONSOLE_Print(SYS_CONSOLE_INDEX_0, "Failed to create APP Console Commands\r\n");
    }
    else
    {
        SYS_CONSOLE_Print(SYS_CONSOLE_INDEX_0, CONSOLE_HEADER);
    }
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
    switch ( app_consoleData.state )
    {
        /* Application's initial state. */
        case APP_CONSOLE_STATE_INIT:
        {
            if (SYS_CMD_READY_TO_READ())
            {
                app_consoleData.state = APP_CONSOLE_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_CONSOLE_STATE_SERVICE_TASKS:
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
