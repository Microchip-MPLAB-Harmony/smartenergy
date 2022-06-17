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


// Temporary definitions, to be replaced by other modules definitions
#define DRV_MET_NUM_CONTROL_REGS       20
#define DRV_MET_NUM_STATUS_REGS        16
#define DRV_MET_NUM_ACCUMULATOR_REGS   12
#define DRV_MET_NUM_HARMONICS_REGS      8

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

APP_CONSOLE_DATA CACHE_ALIGN app_consoleData;

/* Semaphore to stay idle until a new command is received */
OSAL_SEM_HANDLE_TYPE appConsoleSemID;

/* Local storage objects */
//API static DRV_MET_HRR_OBJECT hrrLocalObject;
//API static DRV_MET_MONTHLY_ENERGY_OBJECT energyLocalObject;
//API static DRV_MET_EVENT_OBJECT eventLocalObject;
//API static DRV_MET_MAX_DEMAND_OBJECT maxDemandLocalObject;
//API static DRV_MET_VOLTAGE_OBJECT voltageLocalObject;
//API static DRV_MET_CURRENT_OBJECT currentLocalObject;
//API static DRV_MET_ACTIVE_POWER_OBJECT activePowerLocalObject;
//API static DRV_MET_REACTIVE_POWER_OBJECT reactivePowerLocalObject;

#define APP_CONSOLE_DEFAULT_PWD   "PIC"
static char meterID[6];
static char metPwd[6] = APP_CONSOLE_DEFAULT_PWD;

// Last Times Event requested
static uint8_t lastTimesEvent;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

//API static void _hrrCallback(DRV_MET_HRR_OBJECT *hrrObj)
//API {
//API     // Copy HRR object to local variable and go to corresponding state
//API     memcpy(&hrrLocalObject, hrrObj, sizeof(hrrLocalObject));
//API     app_consoleData.state = APP_CONSOLE_STATE_PRINT_HRR;
//API     // Post semaphore to wakeup task
//API     OSAL_SEM_Post(&appConsoleSemID);
//API }
//API 
//API static void _monthlyEnergyCallback(DRV_MET_MONTHLY_ENERGY_OBJECT *energyObj)
//API {
//API     // Copy energy object to local variable and go to corresponding state
//API     memcpy(&energyLocalObject, energyObj, sizeof(energyLocalObject));
//API     app_consoleData.state = APP_CONSOLE_STATE_PRINT_MONTHLY_ENERGY;
//API     // Post semaphore to wakeup task
//API     OSAL_SEM_Post(&appConsoleSemID);
//API }
//API 
//API static void _eventCallback(DRV_MET_EVENT_OBJECT *eventObj)
//API {
//API     // Copy event object to local variable and go to corresponding state
//API     memcpy(&eventLocalObject, eventObj, sizeof(eventLocalObject));
//API     app_consoleData.state = APP_CONSOLE_STATE_PRINT_EVENT;
//API     // Post semaphore to wakeup task
//API     OSAL_SEM_Post(&appConsoleSemID);
//API }
//API 
//API static void _maxDemandCallback(DRV_MET_MAX_DEMAND_OBJECT *maxDemandObj)
//API {
//API     // Copy event object to local variable and go to corresponding state
//API     memcpy(&maxDemandLocalObject, maxDemandObj, sizeof(maxDemandLocalObject));
//API     app_consoleData.state = APP_CONSOLE_STATE_PRINT_MAX_DEMAND;
//API     // Post semaphore to wakeup task
//API     OSAL_SEM_Post(&appConsoleSemID);
//API }
//API 
//API static void _voltageCallback(DRV_MET_VOLTAGE_OBJECT *voltageObj)
//API {
//API     // Copy voltage object to local variable and go to corresponding state
//API     memcpy(&voltageLocalObject, voltageObj, sizeof(voltageLocalObject));
//API     app_consoleData.state = APP_CONSOLE_STATE_PRINT_VOLTAGE;
//API     // Post semaphore to wakeup task
//API     OSAL_SEM_Post(&appConsoleSemID);
//API }
//API 
//API static void _currentCallback(DRV_MET_CURRENT_OBJECT *currentObj)
//API {
//API     // Copy current object to local variable and go to corresponding state
//API     memcpy(&currentLocalObject, currentObj, sizeof(currentLocalObject));
//API     app_consoleData.state = APP_CONSOLE_STATE_PRINT_CURRENT;
//API     // Post semaphore to wakeup task
//API     OSAL_SEM_Post(&appConsoleSemID);
//API }
//API 
//API static void _activePowerCallback(DRV_MET_ACTIVE_POWER_OBJECT *activePowerObj)
//API {
//API     // Copy active power object to local variable and go to corresponding state
//API     memcpy(&activePowerLocalObject, activePowerObj, sizeof(activePowerLocalObject));
//API     app_consoleData.state = APP_CONSOLE_STATE_PRINT_ACTIVE_POWER;
//API     // Post semaphore to wakeup task
//API     OSAL_SEM_Post(&appConsoleSemID);
//API }
//API 
//API static void _reactivePowerCallback(DRV_MET_REACTIVE_POWER_OBJECT *reactivePowerObj)
//API {
//API     // Copy reactive power object to local variable and go to corresponding state
//API     memcpy(&reactivePowerLocalObject, reactivePowerObj, sizeof(reactivePowerLocalObject));
//API     app_consoleData.state = APP_CONSOLE_STATE_PRINT_REACTIVE_POWER;
//API     // Post semaphore to wakeup task
//API     OSAL_SEM_Post(&appConsoleSemID);
//API }
//API 
//API static void _calibrationCallback(bool calibrationDone)
//API {
//API     if (calibrationDone) {
//API         SYS_CMD_MESSAGE("Calibration Done!\n\r");
//API     }
//API     else {
//API         SYS_CMD_MESSAGE("Calibration Failed!\n\r");
//API     }
//API }
//API 
//API static void _waveformDataCallback(uint32_t *rawData, uint16_t dataSize)
//API {
//API     SYS_CMD_MESSAGE("Waveform Capture Data:\n\r");
//API     // Store parameters in local variables and go to corresponding state
//API     app_consoleData.rawData = rawData;
//API     app_consoleData.rawDataLen = dataSize;
//API     app_consoleData.state = APP_CONSOLE_STATE_PRINT_WAVEFORM_DATA;
//API     // Post semaphore to wakeup task
//API     OSAL_SEM_Post(&appConsoleSemID);
//API }

// *****************************************************************************
// COMMANDS
// *****************************************************************************

static void Command_BUF(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t idx;

    if (argc == 1) {
        // Read all captured data
        idx = 0xFF;
//API         DRV_MET_GetWaveformData(idx);
    }
    else if (argc == 2) {
        // Extract index from parameters
        idx = (uint8_t)strtol(argv[1], NULL, 10);
//API         DRV_MET_GetWaveformData(idx);
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_CAL(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
//API     DRV_MET_CALIBRATION_OBJECT newCalA, newCalB, newCalC;
    uint8_t idx, phase = 0;
    char *p;
    bool parseError = false;

    if (argc == 5) {
        if (strcmp(argv[1], "A") == 0) {
            // Parse arguments
            for (idx = 2; idx < argc; idx++) {
                if (strncmp(argv[idx], "Ua", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalA.voltage = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Ia", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalA.current = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Aa", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalA.angle = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else {
                    parseError = true;
                    break;
                }
            }
            if (!parseError) {
                phase = 1;
            }
        }
        else if (strcmp(argv[1], "B") == 0) {
            // Parse arguments
            for (idx = 2; idx < argc; idx++) {
                if (strncmp(argv[idx], "Ub", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalB.voltage = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Ib", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalB.current = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Ab", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalB.angle = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else {
                    parseError = true;
                    break;
                }
            }
            if (!parseError) {
                phase = 2;
            }
        }
        else if (strcmp(argv[1], "C") == 0) {
            // Parse arguments
            for (idx = 2; idx < argc; idx++) {
                if (strncmp(argv[idx], "Uc", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalC.voltage = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Ic", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalC.current = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Ac", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalC.angle = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else {
                    parseError = true;
                    break;
                }
            }
            if (!parseError) {
                phase = 3;
            }
        }
        else {
            parseError = true;
        }
    }
    else if (argc == 11) {
        if (strcmp(argv[1], "T") == 0) {
            // Parse arguments
            for (idx = 2; idx < argc; idx++) {
                if (strncmp(argv[idx], "Ua", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalA.voltage = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Ia", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalA.current = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Aa", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalA.angle = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Ub", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalB.voltage = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Ib", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalB.current = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Ab", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalB.angle = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Uc", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalC.voltage = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Ic", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalC.current = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else if (strncmp(argv[idx], "Ac", 2) == 0) {
                    // Get substring after '=' char
                    p = strstr(argv[idx], "=");
                    if (p != NULL) {
                        // Advance ptr to ignore '=' and extract value
                        p++;
//API                         newCalC.angle = strtod(p, NULL);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                }
                else {
                    parseError = true;
                    break;
                }
            }
            if (!parseError) {
                phase = 0xFF;
            }
        }
        else {
            parseError = true;
        }
    }
    else {
        // Incorrect parameter number
        parseError = true;
    }

    if (parseError) {
        SYS_CMD_MESSAGE("Unsupported Command !\n\r");
    }
    else {
        SYS_CMD_MESSAGE("Calibrating...\n\r");
        switch (phase) {
        case 1:
//API             DRV_MET_OnePhaseCalibration(phase, &newCalA);
            break;
        case 2:
//API             DRV_MET_OnePhaseCalibration(phase, &newCalB);
            break;
        case 3:
//API             DRV_MET_OnePhaseCalibration(phase, &newCalC);
            break;
        case 0xFF:
//API             DRV_MET_ThreePhaseCalibration(&newCalA, &newCalB, &newCalC);
            break;
        default:
            SYS_CMD_MESSAGE("Could not set Calibration\n\r");
            break;
        }
    }
}

static void Command_CNF(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
//API     DRV_MET_CONF_OBJECT newConf;
    uint8_t idx, value;
    char *p;
    bool parseError = false;

    if (argc == 8) {
        // Parse arguments
        for (idx = 1; idx < argc; idx++) {
            if (strncmp(argv[idx], "MC", 2) == 0) {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL) {
                    // Advance ptr to ignore '=' and extract value
                    p++;
//API                     newConf.meterConstant = strtol(p, NULL, 10);
                }
                else {
                    parseError = true;
                    break;
                }
            }
            else if (strncmp(argv[idx], "ST", 2) == 0) {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL) {
                    // Advance ptr to ignore '=' and extract value
                    p++;
                    value = strtol(p, NULL, 10);
                    switch (value) {
                    case 0:
//API                         newConf.sensorType = DRV_MET_SENSOR_CURRENT_TRANFORMER;
                        break;
                    case 1:
//API                         newConf.sensorType = DRV_MET_SENSOR_SHUNT_RESISTOR;
                        break;
                    case 2:
//API                         newConf.sensorType = DRV_MET_SENSOR_ROGOWSKI_COIL;
                        break;
                    default:
                        parseError = true;
                        break;
                    }
                }
                else {
                    parseError = true;
                    break;
                }
            }
            else if (strncmp(argv[idx], "F", 1) == 0) {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL) {
                    // Advance ptr to ignore '=' and extract value
                    p++;
//API                     newConf.mainsFrequency = strtod(p, NULL);
                }
                else {
                    parseError = true;
                    break;
                }
            }
            else if (strncmp(argv[idx], "G", 1) == 0) {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL) {
                    // Advance ptr to ignore '=' and extract value
                    p++;
                    value = strtol(p, NULL, 10);
                    switch (value) {
                    case 1:
//API                         newConf.pgaValue = DRV_MET_PGA_1;
                        break;
                    case 2:
//API                         newConf.pgaValue = DRV_MET_PGA_2;
                        break;
                    case 4:
//API                         newConf.pgaValue = DRV_MET_PGA_4;
                        break;
                    case 8:
//API                         newConf.pgaValue = DRV_MET_PGA_8;
                        break;
                    default:
                        parseError = true;
                        break;
                    }
                }
                else {
                    parseError = true;
                    break;
                }
            }
            else if (strncmp(argv[idx], "Tr", 2) == 0) {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL) {
                    // Advance ptr to ignore '=' and extract value
                    p++;
//API                     newConf.trafoSpec = strtod(p, NULL);
                }
                else {
                    parseError = true;
                    break;
                }
            }
            else if (strncmp(argv[idx], "Rl", 2) == 0) {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL) {
                    // Advance ptr to ignore '=' and extract value
                    p++;
//API                     newConf.resistorValue = strtod(p, NULL);
                }
                else {
                    parseError = true;
                    break;
                }
            }
            else if (strncmp(argv[idx], "Ku", 2) == 0) {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL) {
                    // Advance ptr to ignore '=' and extract value
                    p++;
//API                     newConf.voltageDividerRatio = strtol(p, NULL, 10);
                }
                else {
                    parseError = true;
                    break;
                }
            }
            else {
                parseError = true;
                break;
            }
        }
    }
    else {
        // Incorrect parameter number
        parseError = true;
    }

    if (parseError) {
        SYS_CMD_MESSAGE("Unsupported Command !\n\r");
    }
    else {
//API         if (DRV_MET_WriteConfiguration(&newConf) == DRV_MET_RESULT_SUCCESS) {
//API             SYS_CMD_MESSAGE("Configure Meter is Ok !\n\r");
//API         }
//API         else {
//API             SYS_CMD_MESSAGE("Could not write Configuration\n\r");
//API         }
    }
}

static void Command_DAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t idx;

    if (argc == 1) {
        // Read all metrology accumulator registers
        app_consoleData.accumRegToRead = 0;
        app_consoleData.state = APP_CONSOLE_STATE_READ_ALL_ACCUM_REGS;
        // Post semaphore to wakeup task
        OSAL_SEM_Post(&appConsoleSemID);
    }
    else if (argc == 2) {
        // Extract register index from parameters
        idx = (uint8_t)strtol(argv[1], NULL, 10);
        if (idx < DRV_MET_NUM_ACCUMULATOR_REGS) {
            // Read register value
            app_consoleData.accumRegToRead = idx;
            app_consoleData.state = APP_CONSOLE_STATE_READ_ACCUM_REG;
            // Post semaphore to wakeup task
            OSAL_SEM_Post(&appConsoleSemID);
        }
        else {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\n\r");
        }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DCB(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1) {
        SYS_CMD_MESSAGE("Entering Low Power\n\r");
        // Go to Low Power mode
//API         DRV_MET_SetLowPowerMode();
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DCD(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1) {
        SYS_CMD_MESSAGE("Load Default Is Ok !\n\r");
        // Set default control register values
//API         DRV_MET_SetControlRegsToDefault();
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DCM(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t i;
    uint8_t numRegsToWrite;
    char *p;
    bool parseError = false;

    if (argc > 1) {
        // Each argument includes reg index and value
        numRegsToWrite = (argc - 1);
        for (i = 0; i < numRegsToWrite; i++) {
            // Extract register index and value from argument
            p = argv[i + 1];
            app_consoleData.regsToModify[i].index = (uint8_t)strtol(p, NULL, 10);
            // Look for ":" char and advance to next char
            p = strstr(p, ":");
            if (p != NULL) {
                p++;
                app_consoleData.regsToModify[i].value = (uint32_t)strtol(p, NULL, 16);
            }
            else {
                SYS_CMD_MESSAGE("Unsupported Command !\n\r");
                parseError = true;
                break;
            }
        }
        if (!parseError) {
            // Write invalid values after last, to later detect it
            app_consoleData.regsToModify[i].index = 0xFF;
            app_consoleData.regsToModify[i].value = 0xFFFFFFFF;
            // Go to corresponding state
            app_consoleData.state = APP_CONSOLE_STATE_WRITE_CONTROL_REG;
            // Post semaphore to wakeup task
            OSAL_SEM_Post(&appConsoleSemID);
        }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DCR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t idx;

    if (argc == 1) {
        // Read all metrology control registers
        app_consoleData.ctrlRegToRead = 0;
        app_consoleData.state = APP_CONSOLE_STATE_READ_ALL_CONTROL_REGS;
        // Post semaphore to wakeup task
        OSAL_SEM_Post(&appConsoleSemID);
    }
    else if (argc == 2) {
        // Extract register index from parameters
        idx = (uint8_t)strtol(argv[1], NULL, 10);
        if (idx < DRV_MET_NUM_CONTROL_REGS) {
            // Read register value
            app_consoleData.ctrlRegToRead = idx;
            app_consoleData.state = APP_CONSOLE_STATE_READ_CONTROL_REG;
            // Post semaphore to wakeup task
            OSAL_SEM_Post(&appConsoleSemID);
        }
        else {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\n\r");
        }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DCS(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1) {
        SYS_CMD_MESSAGE("Save Data Is Ok !\n\r");
        // Save Metrology Constants to NVM
//API         DRV_MET_SaveMetrologyConstants();
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DCW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t idx;
    uint32_t regValue;

    if (argc == 3) {
        // Extract register index from parameters
        idx = (uint8_t)strtol(argv[1], NULL, 10);
        if (idx < DRV_MET_NUM_CONTROL_REGS) {
            // Extract register value
            regValue = (uint32_t)strtol(argv[2], NULL, 16);
            // Write register value
//API             if (DRV_MET_WriteControlRegister(idx, regValue) == DRV_MET_RESULT_SUCCESS) {
//API                 // Show response on console
//API                 SYS_CMD_MESSAGE("Set Is Ok !\n\r");
//API             }
//API             else {
//API                 // Cannot write register
//API                 SYS_CMD_PRINT("Could not write register %02d\n\r", idx);
//API             }
        }
        else {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\n\r");
        }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DSR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t idx;

    if (argc == 1) {
        // Read all metrology status registers
        app_consoleData.statusRegToRead = 0;
        app_consoleData.state = APP_CONSOLE_STATE_READ_ALL_STATUS_REGS;
        // Post semaphore to wakeup task
        OSAL_SEM_Post(&appConsoleSemID);
    }
    else if (argc == 2) {
        // Extract register index from parameters
        idx = (uint8_t)strtol(argv[1], NULL, 10);
        if (idx < DRV_MET_NUM_STATUS_REGS) {
            // Read register value
            app_consoleData.statusRegToRead = idx;
            app_consoleData.state = APP_CONSOLE_STATE_READ_STATUS_REG;
            // Post semaphore to wakeup task
            OSAL_SEM_Post(&appConsoleSemID);
        }
        else {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\n\r");
        }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_ENC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 2) {
        // Check password from parameters
        if (strcmp(argv[1], metPwd) == 0) {
            // Correct password, Clear Energy records
//API             if (DRV_MET_ClearEnergyRecords() == DRV_MET_RESULT_SUCCESS) {
//API                 // Show response on console
//API                 SYS_CMD_MESSAGE("Clear Energy is ok !\n\r");
//API             }
//API             else {
//API                 // Cannot clear Energy records
//API                 SYS_CMD_MESSAGE("Could not clear Energy Records\n\r");
//API             }
        }
        else {
            // Invalid password
            SYS_CMD_MESSAGE("Invalid password\n\r");
        }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_ENR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t monthIndex;

    if (argc == 2) {
        // Extract month index from parameters
        monthIndex = (uint8_t)strtol(argv[1], NULL, 10);
        // Get monthly energy from metrology driver
//API         DRV_MET_GetMonthlyEnergy(monthIndex);
        // Response will be provided on _monthlyEnergyCallback function
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_EVEC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 2) {
        // Check password from parameters
        if (strcmp(argv[1], metPwd) == 0) {
            // Correct password, Clear Event records
//API             if (DRV_MET_ClearEventRecords() == DRV_MET_RESULT_SUCCESS) {
//API                 // Show response on console
//API                 SYS_CMD_MESSAGE("Clear All Event is ok !\n\r");
//API             }
//API             else {
//API                 // Cannot clear Event records
//API                 SYS_CMD_MESSAGE("Could not clear Event Records\n\r");
//API             }
        }
        else {
            // Invalid password
            SYS_CMD_MESSAGE("Invalid password\n\r");
        }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_EVER(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
//API     DRV_MET_EVENT_ID eventId = DRV_MET_INVALID_EVENT;

    if (argc == 3) {
        // Extract last times from parameters
        lastTimesEvent = (uint8_t)strtol(argv[2], NULL, 10);
        // Extract event id from parameters
//API         if (strcmp(argv[1], "Ua") == 0) {
//API             eventId = DRV_MET_SAG_UA_EVENT;
//API         }
//API         else if (strcmp(argv[1], "Ub") == 0) {
//API             eventId = DRV_MET_SAG_UB_EVENT;
//API         }
//API         else if (strcmp(argv[1], "Uc") == 0) {
//API             eventId = DRV_MET_SAG_UC_EVENT;
//API         }
//API         else if (strcmp(argv[1], "Pa") == 0) {
//API             eventId = DRV_MET_POW_UA_EVENT;
//API         }
//API         else if (strcmp(argv[1], "Pb") == 0) {
//API             eventId = DRV_MET_POW_UB_EVENT;
//API         }
//API         else if (strcmp(argv[1], "Pc") == 0) {
//API             eventId = DRV_MET_POW_UC_EVENT;
//API         }
//API 
//API         if (eventId != DRV_MET_INVALID_EVENT) {
//API             // Get monthly energy from metrology driver
//API             DRV_MET_GetEvent(eventId, lastTimesEvent);
//API             // Response will be provided on _eventCallback function
//API         }
//API         else {
//API             // Invalid Command
//API             SYS_CMD_MESSAGE("Unsupported Command !\n\r");
//API         }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_HAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t idx;

    if (argc == 1) {
        // Read all metrology harmonics registers
        app_consoleData.harRegToRead = 0;
        app_consoleData.state = APP_CONSOLE_STATE_READ_ALL_HARMONICS_REGS;
        // Post semaphore to wakeup task
        OSAL_SEM_Post(&appConsoleSemID);
    }
    else if (argc == 2) {
        // Extract register index from parameters
        idx = (uint8_t)strtol(argv[1], NULL, 10);
        if (idx < DRV_MET_NUM_HARMONICS_REGS) {
            // Read register value
            app_consoleData.harRegToRead = idx;
            app_consoleData.state = APP_CONSOLE_STATE_READ_HARMONICS_REG;
            // Post semaphore to wakeup task
            OSAL_SEM_Post(&appConsoleSemID);
        }
        else {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\n\r");
        }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_HRR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t harmonicNum;

    if (argc == 2) {
        // Extract harmonic number from parameters
        harmonicNum = (uint8_t)strtol(argv[1], NULL, 10);
        // Set harmonics calculation mode on metrology driver
//API         DRV_MET_SetHarmonicsCalculation(harmonicNum);
        // Response will be provided on _hrrCallback function
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_IDR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1) {
        // Read Meter ID
        app_consoleData.state = APP_CONSOLE_STATE_READ_METER_ID;
        // Post semaphore to wakeup task
        OSAL_SEM_Post(&appConsoleSemID);
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_IDW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 3) {
        // Check password from parameters
        if (strcmp(argv[1], metPwd) == 0) {
            // Correct password, write Meter ID
//API             if (DRV_MET_WriteMeterID(argv[2]) == DRV_MET_RESULT_SUCCESS) {
//API                 // Show response on console
//API                 SYS_CMD_MESSAGE("Set Meter ID is Ok\n\r");
//API             }
//API             else {
//API                 // Cannot write ID
//API                 SYS_CMD_MESSAGE("Could not set Meter ID\n\r");
//API             }
        }
        else {
            // Invalid password
            SYS_CMD_MESSAGE("Invalid password\n\r");
        }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_MDC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 2) {
        // Check password from parameters
        if (strcmp(argv[1], metPwd) == 0) {
            // Correct password, Clear Max Demand records
//API             if (DRV_MET_ClearMaxDemandRecords() == DRV_MET_RESULT_SUCCESS) {
//API                 // Show response on console
//API                 SYS_CMD_MESSAGE("Clear MaxDemand is ok !\n\r");
//API             }
//API             else {
//API                 // Cannot clear Max Demand records
//API                 SYS_CMD_MESSAGE("Could not clear MaxDemand Records\n\r");
//API             }
        }
        else {
            // Invalid password
            SYS_CMD_MESSAGE("Invalid password\n\r");
        }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_MDR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t monthIndex;

    if (argc == 2) {
        // Extract month index from parameters
        monthIndex = (uint8_t)strtol(argv[1], NULL, 10);
        // Get max demand from metrology driver
//API         DRV_MET_GetMonthlyMaxDemand(monthIndex);
        // Response will be provided on _maxDemandCallback function
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_PAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 2) {
        // Extract data to retrieve from parameters
//API         if (strcmp(argv[1], "U") == 0) {
//API             DRV_MET_GetVoltage();
//API         }
//API         else if (strcmp(argv[1], "I") == 0) {
//API             DRV_MET_GetCurrent();
//API         }
//API         else if (strcmp(argv[1], "P") == 0) {
//API             DRV_MET_GetActivePower();
//API         }
//API         else if (strcmp(argv[1], "Q") == 0) {
//API             DRV_MET_GetReactivePower();
//API         }
//API         else {
//API             // Invalid Command
//API             SYS_CMD_MESSAGE("Unsupported Command !\n\r");
//API         }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_RTCR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1) {
        // Read RTC
        app_consoleData.state = APP_CONSOLE_STATE_READ_RTC;
        // Post semaphore to wakeup task
        OSAL_SEM_Post(&appConsoleSemID);
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_RTCW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 5)
    {
        if (strcmp(argv[1], metPwd) == 0)
        {
            struct tm sysTime;
            char *data;
            
            // Get Date
            data = argv[2];
            // Year
            sysTime.tm_year = ((data[0] - '0') * 10 + (data[1] - '0')) + 2000 - 1900;
            // Month
            sysTime.tm_mon = ((data[3] - '0') * 10 + (data[4] - '0')) - 1;
            // Day
            sysTime.tm_mday = (data[6] - '0') * 10 + (data[7] - '0');
            
            // Get Week Day
            data = argv[3];
            sysTime.tm_wday = (data[0] - '0') - 1;
            
            // Get Time
            data = argv[4];
            // Hour
            sysTime.tm_hour = (data[0] - '0') * 10 + (data[1] - '0');
            // Minute
            sysTime.tm_min = (data[3] - '0') * 10 + (data[4] - '0');
            // Second
            sysTime.tm_sec = (data[6] - '0') * 10 + (data[7] - '0');    
            
            if (RTC_TimeSet(&sysTime))
            {
                SYS_CMD_MESSAGE("Set RTC is ok!\n\r");
            }
            else
            {
                SYS_CMD_MESSAGE("Unsupported Command !\n\r");
            }
        }
        else
        {
            SYS_CMD_MESSAGE("Password Error !\n\r");
        }
    }
    else
    {
        SYS_CMD_MESSAGE("Unsupported Command !\n\r");
    }
}

static void Command_TOUR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1) {
        // Go to state to read TOU
        app_consoleData.state = APP_CONSOLE_STATE_READ_TOU;
        // Post semaphore to wakeup task
        OSAL_SEM_Post(&appConsoleSemID);
    }
    else {
        // Incorrect param number
        SYS_CMD_MESSAGE("Unsupported Command !\n\r");
    }
}

static void Command_TOUW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
//API     DRV_MET_TOU_OBJECT newTou[DRV_MET_MAX_TOU_NUM];
    uint8_t idx, argIdx;
    char *p;
    bool parseError = false;

    if ((argc > 3) && ((argc - 2) % 2 == 0)) {
        // Check password from parameters
        if (strcmp(argv[1], metPwd) == 0) {
//API             // Correct password, write TOW
//API             for (idx = 0; idx < DRV_MET_MAX_TOU_NUM; idx++) {
//API                 // Check whether there are arguments left to write
//API                 argIdx = ((idx << 1) + 2);
//API                 if (argc > argIdx) {
//API                     // Extract hour, minute and tariff
//API                     p = argv[argIdx];
//API                     // Extract hour from argument
//API                     newTou[idx].hourStart = (uint8_t)strtol(p, NULL, 10);
//API                     // Look for ":" char and advance to next char
//API                     p = strstr(p, ":");
//API                     if (p != NULL) {
//API                         p++;
//API                         // Extract minute from argument
//API                         newTou[idx].minuteStart = (uint8_t)strtol(p, NULL, 10);
//API                         // Extract tariff from next argument
//API                         newTou[idx].touTariff = (uint8_t)strtol(argv[argIdx + 1], NULL, 10);
//API                     }
//API                     else {
//API                         parseError = true;
//API                         break;
//API                     }
//API                     if ((newTou[idx].hourStart > 23) ||
//API                         (newTou[idx].minuteStart > 59) ||
//API                         (newTou[idx].touTariff > DRV_MET_TOU_TARIFF_4)) {
//API                             parseError = true;
//API                             break;
//API                     }
//API                 }
//API                 else {
//API                     // No more arguments, fill TOU index with invalid data
//API                     newTou[idx].hourStart = 0;
//API                     newTou[idx].minuteStart = 0;
//API                     newTou[idx].touTariff = DRV_MET_TOU_TARIFF_INVALID;
//API                 }
//API             }
        }
        else {
            // Invalid password
            parseError = true;
        }
    }
    else {
        // Incorrect parameter number
        parseError = true;
    }

    if (parseError) {
        SYS_CMD_MESSAGE("Unsupported Command !\n\r");
    }
    else {
//API         if (DRV_MET_WriteTOU(newTou) == DRV_MET_RESULT_SUCCESS) {
//API             SYS_CMD_MESSAGE("Set TOU is Ok !\n\r");
//API         }
//API         else {
//API             SYS_CMD_MESSAGE("Could not write TOU\n\r");
//API         }
    }
}

static void Command_RST(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 2) {
        // Check password from parameters
        if (strcmp(argv[1], metPwd) == 0) {
            // Correct password, Reset System
            SYS_CMD_MESSAGE("Reset Command is Ok !\n\r");
//API             DRV_MET_ResetSystem();
        }
        else {
            // Invalid password
            SYS_CMD_MESSAGE("Invalid password\n\r");
        }
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_RLD(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1) {
        SYS_CMD_MESSAGE("Reloading Metrology...\n\r");
        // Reload Metrology coprocessor
//API         DRV_MET_ReloadMetrology();
    }
    else {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_RFC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    SYS_CMD_MESSAGE("Read File Content   \n\r");
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
    {"RLD", Command_RLD, ": Reload Metrology Coprocessor"},
    {"RFC", Command_RFC, ": Read File Content"},
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
    char regName[16];
    uint32_t regValue;
    char meterID[6];
    struct tm sysTime;

    /* Check the application's current state. */
    switch ( app_consoleData.state )
    {
        /* Application's initial state. */
        case APP_CONSOLE_STATE_INIT:
        {
            if (SYS_CMD_READY_TO_READ())
            {
                /* Initialize Metrology Driver callbacks */
//API                 DRV_MET_HRRCallbackRegister(_hrrCallback);
//API                 DRV_MET_CalibrationCallbackRegister(_calibrationCallback);
//API                 DRV_MET_MonthlyEnergyCallbackRegister(_monthlyEnergyCallback);
//API                 DRV_MET_EventCallbackRegister(_eventCallback);
//API                 DRV_MET_MaxDemandCallbackRegister(_maxDemandCallback);
//API                 DRV_MET_VoltageCallbackRegister(_voltageCallback);
//API                 DRV_MET_CurrentCallbackRegister(_currentCallback);
//API                 DRV_MET_ActivePowerCallbackRegister(_activePowerCallback);
//API                 DRV_MET_ReactivePowerCallbackRegister(_reactivePowerCallback);
//API                 DRV_MET_WaveformDataCallbackRegister(_waveformDataCallback);
//API                 memset(touObject, 0xFF, sizeof(touObject)); /* 0xFF means invalid tariff */
                if (OSAL_SEM_Create(&appConsoleSemID, OSAL_SEM_TYPE_BINARY, 1, 0) == OSAL_RESULT_TRUE) {
                    app_consoleData.state = APP_CONSOLE_STATE_IDLE;
                }
            }
            break;
        }

        case APP_CONSOLE_STATE_IDLE:
        {
            OSAL_SEM_Pend(&appConsoleSemID, OSAL_WAIT_FOREVER);
            break;
        }

        case APP_CONSOLE_STATE_READ_CONTROL_REG:
        {
            // Read register value
//API             if (DRV_MET_ReadControlRegister(app_consoleData.ctrlRegToRead, regName, &regValue) == DRV_MET_RESULT_SUCCESS) {
//API                 SYS_CMD_PRINT("%02d %s\n\r", app_consoleData.ctrlRegToRead, regName);
//API                 SYS_CMD_PRINT("%08X\n\r", regValue);
//API             }
//API             else {
//API                 // Cannot read register
//API                 SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.ctrlRegToRead);
//API             }
            // Go back to Idle
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_WRITE_CONTROL_REG:
        {
            uint8_t idx;
            for (idx = 0; idx < APP_CONSOLE_MAX_REGS; idx++) {
                if (app_consoleData.regsToModify[idx].index != 0xFF) {
                    if (app_consoleData.regsToModify[idx].index < DRV_MET_NUM_CONTROL_REGS) {
                        // Write register value
//API                         if (DRV_MET_WriteControlRegister(app_consoleData.regsToModify[idx].index, app_consoleData.regsToModify[idx].value) == DRV_MET_RESULT_SUCCESS) {
//API                             // Show response on console
//API                             SYS_CMD_PRINT("Set %02d Is Ok !\n\r", app_consoleData.regsToModify[idx].index);
//API                         }
//API                         else {
//API                             // Cannot write register
//API                             SYS_CMD_PRINT("Could not write register %02d\n\r", app_consoleData.regsToModify[idx].index);
//API                         }
                    }
                    else {
                        // Invalid index
                        SYS_CMD_PRINT("Invalid register index %02d\n\r", app_consoleData.regsToModify[idx].index);
                    }
                }
                else {
                    // All registers have been written
                    app_consoleData.state = APP_CONSOLE_STATE_IDLE;
                    break;
                }
                if ((idx % 10) == 0) {
                    vTaskDelay(10 / portTICK_PERIOD_MS);
                }
            }
            break;
        }

        case APP_CONSOLE_STATE_READ_ALL_CONTROL_REGS:
        {
            if (app_consoleData.ctrlRegToRead < DRV_MET_NUM_CONTROL_REGS) {
                // Read register value
 //API                if (DRV_MET_ReadControlRegister(app_consoleData.ctrlRegToRead, regName, &regValue) == DRV_MET_RESULT_SUCCESS) {
 //API                    SYS_CMD_PRINT("%02d %s\n\r", app_consoleData.ctrlRegToRead, regName);
 //API                    SYS_CMD_PRINT("%08X\n\r", regValue);
 //API                }
 //API                else {
 //API                    // Cannot read register
 //API                    SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.ctrlRegToRead);
 //API                }
                // Advance to next register
                app_consoleData.ctrlRegToRead++;
            }
            else {
                // All registers have been read
                app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_CONSOLE_STATE_READ_ACCUM_REG:
        {
            // Read register value
//API             if (DRV_MET_ReadAccumulatorRegister(app_consoleData.accumRegToRead, regName, &regValue) == DRV_MET_RESULT_SUCCESS) {
//API                 SYS_CMD_PRINT("%02d %s\n\r", app_consoleData.accumRegToRead, regName);
//API                 SYS_CMD_PRINT("%08X\n\r", regValue);
//API             }
//API             else {
//API                 // Cannot read register
//API                 SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.accumRegToRead);
//API             }
            // Go back to Idle
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_READ_ALL_ACCUM_REGS:
        {
            if (app_consoleData.accumRegToRead < DRV_MET_NUM_ACCUMULATOR_REGS) {
                // Read register value
//API                 if (DRV_MET_ReadAccumulatorRegister(app_consoleData.accumRegToRead, regName, &regValue) == DRV_MET_RESULT_SUCCESS) {
//API                     SYS_CMD_PRINT("%02d %s\n\r", app_consoleData.accumRegToRead, regName);
//API                     SYS_CMD_PRINT("%08X\n\r", regValue);
//API                 }
//API                 else {
//API                     // Cannot read register
//API                     SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.accumRegToRead);
//API                 }
                // Advance to next register
                app_consoleData.accumRegToRead++;
            }
            else {
                // All registers have been read
                app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_CONSOLE_STATE_READ_STATUS_REG:
        {
            // Read register value
//API             if (DRV_MET_ReadStatusRegister(app_consoleData.statusRegToRead, regName, &regValue) == DRV_MET_RESULT_SUCCESS) {
//API                 SYS_CMD_PRINT("%02d %s\n\r", app_consoleData.statusRegToRead, regName);
//API                 SYS_CMD_PRINT("%08X\n\r", regValue);
//API             }
//API             else {
//API                 // Cannot read register
//API                 SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.statusRegToRead);
//API             }
            // Go back to Idle
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_READ_ALL_STATUS_REGS:
        {
            if (app_consoleData.statusRegToRead < DRV_MET_NUM_STATUS_REGS) {
                // Read register value
//API                 if (DRV_MET_ReadStatusRegister(app_consoleData.statusRegToRead, regName, &regValue) == DRV_MET_RESULT_SUCCESS) {
//API                     SYS_CMD_PRINT("%02d %s\n\r", app_consoleData.statusRegToRead, regName);
//API                     SYS_CMD_PRINT("%08X\n\r", regValue);
//API                 }
//API                 else {
//API                     // Cannot read register
//API                     SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.statusRegToRead);
//API                 }
                // Advance to next register
                app_consoleData.statusRegToRead++;
            }
            else {
                // All registers have been read
                app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_CONSOLE_STATE_READ_HARMONICS_REG:
        {
            // Read register value
//API             if (DRV_MET_ReadHarmonicsRegister(app_consoleData.harRegToRead, regName, &regValue) == DRV_MET_RESULT_SUCCESS) {
//API                 SYS_CMD_PRINT("%02d %s\n\r", app_consoleData.harRegToRead, regName);
//API                 SYS_CMD_PRINT("%08X\n\r", regValue);
//API             }
//API             else {
//API                 // Cannot read register
//API                 SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.harRegToRead);
//API             }
            // Go back to Idle
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_READ_ALL_HARMONICS_REGS:
        {
            if (app_consoleData.harRegToRead < DRV_MET_NUM_HARMONICS_REGS) {
                // Read register value
//API                 if (DRV_MET_ReadHarmonicsRegister(app_consoleData.harRegToRead, regName, &regValue) == DRV_MET_RESULT_SUCCESS) {
//API                     SYS_CMD_PRINT("%02d %s\n\r", app_consoleData.harRegToRead, regName);
//API                     SYS_CMD_PRINT("%08X\n\r", regValue);
//API                 }
//API                 else {
//API                     // Cannot read register
//API                     SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.harRegToRead);
//API                 }
                // Advance to next register
                app_consoleData.harRegToRead++;
            }
            else {
                // All registers have been read
                app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_CONSOLE_STATE_READ_METER_ID:
        {
//API             if (DRV_MET_ReadMeterID(meterID) == DRV_MET_RESULT_SUCCESS) {
//API                 // Show response on console
//API                 SYS_CMD_MESSAGE("Meter ID is :\n\r");
//API                 SYS_CMD_PRINT("%s\n\r", meterID);
//API             }
//API             else {
//API                 // Cannot read ID
//API                 SYS_CMD_MESSAGE("Could not read Meter ID\n\r");
//API             }
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_READ_RTC:
        {
            // Read and print RTC
            RTC_TimeGet(&sysTime);
            SYS_CMD_MESSAGE("Present RTC is(yy-mm-dd w hh:mm:ss):\n\r");
            SYS_CMD_PRINT("%02u-%02u-%02u %u %02u:%02u:%02u\r\n",
                    sysTime.tm_year + 1900 - 2000, sysTime.tm_mon + 1, sysTime.tm_mday, 
                    sysTime.tm_wday + 1, sysTime.tm_hour, sysTime.tm_min, sysTime.tm_sec);
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_READ_TOU:
        {
//API             DRV_MET_TOU_OBJECT currentTou[DRV_MET_MAX_TOU_NUM];
//API             uint8_t idx;
//API 
//API             if (DRV_MET_ReadTOU(currentTou) == DRV_MET_RESULT_SUCCESS) {
//API                 SYS_CMD_MESSAGE("TOU table is :\n\r");
//API                 for (idx = 0; idx < DRV_MET_MAX_TOU_NUM; idx++) {
//API                     if (currentTou[idx].touTariff != DRV_MET_TOU_TARIFF_INVALID) {
//API                         SYS_CMD_PRINT("T%d=%02d:%02d %d ",
//API                             (idx + 1), currentTou[idx].hourStart, currentTou[idx].minuteStart, currentTou[idx].touTariff);
//API                     }
//API                 }
//API                 SYS_CMD_MESSAGE("\n\r");
//API             }
//API             else {
//API                 SYS_CMD_MESSAGE("Could not read TOU\n\r");
//API             }
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_HRR:
        {
            // Show received data on console
            SYS_CMD_MESSAGE("The calculated harmonic Irms/Vrms:\n\r");
//API 
//API             SYS_CMD_MESSAGE("Irms_Har_A(A)\n\r");
//API             SYS_CMD_PRINT("%f\n\r", hrrLocalObject.Irms_A);
//API             
//API             SYS_CMD_MESSAGE("Irms_Har_B(A)\n\r");
//API             SYS_CMD_PRINT("%f\n\r", hrrLocalObject.Irms_B);
//API             
//API             SYS_CMD_MESSAGE("Irms_Har_C(A)\n\r");
//API             SYS_CMD_PRINT("%f\n\r", hrrLocalObject.Irms_C);
//API             
//API             SYS_CMD_MESSAGE("Vrms_Har_A(V)\n\r");
//API             SYS_CMD_PRINT("%f\n\r", hrrLocalObject.Vrms_A);
//API             
//API             SYS_CMD_MESSAGE("Vrms_Har_B(V)\n\r");
//API             SYS_CMD_PRINT("%f\n\r", hrrLocalObject.Vrms_B);
//API             
//API             SYS_CMD_MESSAGE("Vrms_Har_C(V)\n\r");
//API             SYS_CMD_PRINT("%f\n\r", hrrLocalObject.Vrms_C);
//API             
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_MONTHLY_ENERGY:
        {
            // Show received data on console
//API             SYS_CMD_PRINT("Last %d Month Energy is :\n\r", energyLocalObject.monthIndex);
//API             SYS_CMD_PRINT("TT=%fkWh T1=%fkWh T2=%fkWh T3=%fkWh T4=%fkWh\n\r",
//API                 energyLocalObject.totalEnergy,
//API                 energyLocalObject.energyPerTariff[0],
//API                 energyLocalObject.energyPerTariff[1],
//API                 energyLocalObject.energyPerTariff[2],
//API                 energyLocalObject.energyPerTariff[3]);
            
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_EVENT:
        {
            // Print Event ID and requested Times
//API             if (eventLocalObject.id == DRV_MET_SAG_UA_EVENT) {
//API                 SYS_CMD_PRINT("Last %d Times Ua Sag is :\n\r", lastTimesEvent);
//API             }
//API             else if (eventLocalObject.id == DRV_MET_SAG_UB_EVENT) {
//API                 SYS_CMD_PRINT("Last %d Times Ub Sag is :\n\r", lastTimesEvent);
//API             }
//API             else if (eventLocalObject.id == DRV_MET_SAG_UC_EVENT) {
//API                 SYS_CMD_PRINT("Last %d Times Uc Sag is :\n\r", lastTimesEvent);
//API             }
//API             else if (eventLocalObject.id == DRV_MET_POW_UA_EVENT) {
//API                 SYS_CMD_PRINT("Last %d Times Pa reverse is :\n\r", lastTimesEvent);
//API             }
//API             else if (eventLocalObject.id == DRV_MET_POW_UB_EVENT) {
//API                 SYS_CMD_PRINT("Last %d Times Pb reverse is :\n\r", lastTimesEvent);
//API             }
//API             else if (eventLocalObject.id == DRV_MET_POW_UC_EVENT) {
//API                 SYS_CMD_PRINT("Last %d Times Pc reverse is :\n\r", lastTimesEvent);
//API             }
//API             // Show received data on console
//API             SYS_CMD_PRINT("Total Num=%d start time is %02d-%02d %02d:%02d end time is %02d-%02d %02d:%02d\n\r",
//API                 eventLocalObject.counter,
//API                 eventLocalObject.dateTimeStart.tm_mon,
//API                 eventLocalObject.dateTimeStart.tm_mday,
//API                 eventLocalObject.dateTimeStart.tm_hour,
//API                 eventLocalObject.dateTimeStart.tm_min,
//API                 eventLocalObject.dateTimeEnd.tm_mon,
//API                 eventLocalObject.dateTimeEnd.tm_mday,
//API                 eventLocalObject.dateTimeEnd.tm_hour,
//API                 eventLocalObject.dateTimeEnd.tm_min);
            
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_MAX_DEMAND:
        {
            // Show received data on console
//API             SYS_CMD_PRINT("Last %d Month MaxDemand is :\n\r", maxDemandLocalObject.monthIndex);
//API             SYS_CMD_PRINT("TT=%fkW %d-%d %02d:%02d T1=%fkW %d-%d %02d:%02d T2=%fkW %d-%d %02d:%02d T3=%fkW %d-%d %02d:%02d T4=%fkW %d-%d %02d:%02d\n\r",
//API                 maxDemandLocalObject.maxDemand = 2,
//API                 maxDemandLocalObject.dateTimePeak.tm_mon,
//API                 maxDemandLocalObject.dateTimePeak.tm_mday,
//API                 maxDemandLocalObject.dateTimePeak.tm_hour,
//API                 maxDemandLocalObject.dateTimePeak.tm_min,
//API                 maxDemandLocalObject.maxDemandPerTariff[0],
//API                 maxDemandLocalObject.dateTimePeakPerTariff[0].tm_mon,
//API                 maxDemandLocalObject.dateTimePeakPerTariff[0].tm_mday,
//API                 maxDemandLocalObject.dateTimePeakPerTariff[0].tm_hour,
//API                 maxDemandLocalObject.dateTimePeakPerTariff[0].tm_min,
//API                 maxDemandLocalObject.maxDemandPerTariff[1],
//API                 maxDemandLocalObject.dateTimePeakPerTariff[1].tm_mon,
//API                 maxDemandLocalObject.dateTimePeakPerTariff[1].tm_mday,
//API                 maxDemandLocalObject.dateTimePeakPerTariff[1].tm_hour,
//API                 maxDemandLocalObject.dateTimePeakPerTariff[1].tm_min,
//API                 maxDemandLocalObject.maxDemandPerTariff[2],
//API                 maxDemandLocalObject.dateTimePeakPerTariff[2].tm_mon,
//API                 maxDemandLocalObject.dateTimePeakPerTariff[2].tm_mday,
//API                 maxDemandLocalObject.dateTimePeakPerTariff[2].tm_hour,
//API                 maxDemandLocalObject.dateTimePeakPerTariff[2].tm_min,
//API                 maxDemandLocalObject.maxDemandPerTariff[3],
//API                 maxDemandLocalObject.dateTimePeakPerTariff[3].tm_mon,
//API                 maxDemandLocalObject.dateTimePeakPerTariff[3].tm_mday,
//API                 maxDemandLocalObject.dateTimePeakPerTariff[3].tm_hour,
//API                 maxDemandLocalObject.dateTimePeakPerTariff[3].tm_min);
            
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_VOLTAGE:
        {
            // Show received data on console
            SYS_CMD_MESSAGE("Present voltage is :\n\r");
//API             SYS_CMD_PRINT("Ua=%fV Ub=%fV Uc=%fV\n\r",
//API                 voltageLocalObject.V_a,
//API                 voltageLocalObject.V_b,
//API                 voltageLocalObject.V_c);
            
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_CURRENT:
        {
            // Show received data on console
            SYS_CMD_MESSAGE("Present current is :\n\r");
//API             SYS_CMD_PRINT("Ia=%fA Ib=%fA Ic=%fA Ini=%fA Inm=%fA Inmi=%fA\n\r",
//API                 currentLocalObject.I_a,
//API                 currentLocalObject.I_b,
//API                 currentLocalObject.I_c,
//API                 currentLocalObject.I_ni,
//API                 currentLocalObject.I_nm,
//API                 currentLocalObject.I_nmi);
            
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_ACTIVE_POWER:
        {
            // Show received data on console
            SYS_CMD_MESSAGE("Present active power is :\n\r");
//API             SYS_CMD_PRINT("Pt=%fW Pa=%fW Pb=%fW Pc=%fW\n\r",
//API                 activePowerLocalObject.P_t,
//API                 activePowerLocalObject.P_a,
//API                 activePowerLocalObject.P_b,
//API                 activePowerLocalObject.P_c);
            
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_REACTIVE_POWER:
        {
            // Show received data on console
            SYS_CMD_MESSAGE("Present reactive power is :\n\r");
//API             SYS_CMD_PRINT("Qt=%fVar Qa=%fVar Qb=%fVar Qc=%fVar\n\r",
//API                 reactivePowerLocalObject.Q_t,
//API                 reactivePowerLocalObject.Q_a,
//API                 reactivePowerLocalObject.Q_b,
//API                 reactivePowerLocalObject.Q_c);
            
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_WAVEFORM_DATA:
        {
            uint8_t idx;
            for (idx = 0; idx < 10; idx++) {
                if (app_consoleData.rawDataLen > 0) {
                    // Print value
                    SYS_CMD_PRINT("%08X\n\r", *(app_consoleData.rawData));
                    // Advance to next value
                    app_consoleData.rawData++;
                    app_consoleData.rawDataLen--;
                }
                else {
                    // All registers have been read
                    app_consoleData.state = APP_CONSOLE_STATE_IDLE;
                    break;
                }
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
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
