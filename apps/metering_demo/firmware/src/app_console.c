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

#include "definitions.h"
#include "app_console.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define CONSOLE_HEADER   "\r\n-- Microchip Demo Meter --\r\n" \
  "-- Compiled: "__DATE__ " "__TIME__ " -- \r\n"

/* Structure containing data to be stored in non volatile memory */
typedef struct
{
    /* Meter ID */
    char meterID[7];

} APP_CONSOLE_STORAGE_DATA;

#define CONSOLE_TASK_DELAY_MS_UNTIL_DATALOG_READY      100
#define CONSOLE_TASK_DEFAULT_DELAY_MS_BETWEEN_STATES   10
#define CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT       30

#define CONSOLE_MAX_WAIT_MS_UNTIL_DATALOG_READY        2000


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

/* Local variable to hold a duplicate of storage data */
APP_CONSOLE_STORAGE_DATA CACHE_ALIGN app_consoleStorageData;
/* Constant conaining default value for storage data */
const APP_CONSOLE_STORAGE_DATA CACHE_ALIGN app_defaultConsoleStorageData = {{'1', '2', '3', '4', '\0', '\0', '\0'}};

/* Semaphore to stay idle until a new command is received */
OSAL_SEM_HANDLE_TYPE appConsoleSemID;

/* Semaphore to wait until data is read from storage */
OSAL_SEM_HANDLE_TYPE appConsoleStorageSemID;

/* Local storage objects */
static APP_ENERGY_ACCUMULATORS energyLocalObject;
static APP_ENERGY_MAX_DEMAND maxDemandLocalObject;

/* Local Queue element to request Datalog operations */
APP_DATALOG_QUEUE_DATA datalogQueueElement;

/* Reference to datalog queue */
extern QueueHandle_t CACHE_ALIGN appDatalogQueueID;

/* Local DateTime struct */
static struct tm sysTime;

#define APP_CONSOLE_DEFAULT_PWD   "PIC"
static char metPwd[6] = APP_CONSOLE_DEFAULT_PWD;

/* Last Times Event requested */
static uint8_t lastTimesEvent;

/* Control max wait for Datalog ready */
static uint32_t currentWaitForDatalogReady = 0;

static char sign[2] = {' ', '-'};

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void _consoleReadStorage(APP_DATALOG_RESULT result)
{
    // Check result and go to corresponding state
    if (result == APP_DATALOG_RESULT_SUCCESS)
    {
        app_consoleData.state = APP_CONSOLE_STATE_READ_STORAGE_OK;
    }
    else
    {
        app_consoleData.state = APP_CONSOLE_STATE_READ_STORAGE_ERROR;
    }
    // Post semaphore to wakeup task
    OSAL_SEM_Post(&appConsoleStorageSemID);
}

static void _monthlyEnergyCallback(struct tm * time, bool dataValid)
{
    app_consoleData.timeRequest = *time;
    app_consoleData.dataValid = dataValid;
    app_consoleData.state = APP_CONSOLE_STATE_PRINT_MONTHLY_ENERGY;
    // Post semaphore to wakeup task
    OSAL_SEM_Post(&appConsoleSemID);
}

static void _maxDemandCallback(struct tm * time, bool dataValid)
{
    app_consoleData.timeRequest = *time;
    app_consoleData.dataValid = dataValid;
    app_consoleData.state = APP_CONSOLE_STATE_PRINT_MAX_DEMAND;

    // Post semaphore to wakeup task
    OSAL_SEM_Post(&appConsoleSemID);
}

// *****************************************************************************
// COMMANDS
// *****************************************************************************

static void Command_BUF(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint32_t captureAddress;
    size_t captureSize;
    uint8_t idx = 0xFF;
    
    
    if (argc > 2)
    {
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
        return;
    }

    if (argc == 2) 
    {
        // Extract index from parameters
        idx = (uint8_t)strtol(argv[1], NULL, 10);
    }
    
    // Store parameters in local variables and go to corresponding state
    captureSize = APP_METROLOGY_GetWaveformCaptureData(&captureAddress);
    
    if (idx > (captureSize >> 9))
    {
        SYS_CMD_MESSAGE("Parameter is out of range.\n\r");
        return;
    }
    
    if (idx == 0xFF)
    {
        app_consoleData.rawDataLen = captureSize;
        app_consoleData.rawData = (uint32_t *)captureAddress;
    }
    else
    {
        app_consoleData.rawDataLen = 512;
        app_consoleData.rawData = (uint32_t *)captureAddress;
        app_consoleData.rawData += (512 * idx);
    }
    
    app_consoleData.state = APP_CONSOLE_STATE_PRINT_WAVEFORM_DATA;
    // Post semaphore to wakeup task
    OSAL_SEM_Post(&appConsoleSemID);
    
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
    DRV_METROLOGY_CONFIGURATION newConf;
    uint8_t idx, value;
    char *p;
    bool parseError = false;

    if (argc == 8)
    {
        // Parse arguments
        for (idx = 1; idx < argc; idx++)
        {
            if (strncmp(argv[idx], "MC", 2) == 0)
            {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL)
                {
                    // Advance ptr to ignore '=' and extract value
                    p++;
                    newConf.mc = strtol(p, NULL, 10);
                }
                else
                {
                    parseError = true;
                    break;
                }
            }
            else if (strncmp(argv[idx], "ST", 2) == 0)
            {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL)
                {
                    // Advance ptr to ignore '=' and extract value
                    p++;
                    value = strtol(p, NULL, 10);
                    if (value < SENSOR_NUM_TYPE)
                    {
                        newConf.st = (DRV_METROLOGY_SENSOR_TYPE)value;
                    }
                    else
                    {
                        parseError = true;
                    }
                }
                else
                {
                    parseError = true;
                    break;
                }
            }
            else if (strncmp(argv[idx], "F", 1) == 0)
            {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL)
                {
                    // Advance ptr to ignore '=' and extract value
                    p++;
                    newConf.freq = strtod(p, NULL);
                }
                else
                {
                    parseError = true;
                    break;
                }
            }
            else if (strncmp(argv[idx], "G", 1) == 0)
            {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL)
                {
                    // Advance ptr to ignore '=' and extract value
                    p++;
                    value = strtol(p, NULL, 10);
                    if (value < GAIN_NUM_TYPE)
                    {
                        newConf.gain = (DRV_METROLOGY_GAIN_TYPE)value;
                    }
                    else
                    {
                        parseError = true;
                    }
                }
                else
                {
                    parseError = true;
                    break;
                }
            }
            else if (strncmp(argv[idx], "Tr", 2) == 0)
            {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL)
                {
                    // Advance ptr to ignore '=' and extract value
                    p++;
                    newConf.tr = strtod(p, NULL);
                }
                else
                {
                    parseError = true;
                    break;
                }
            }
            else if (strncmp(argv[idx], "Rl", 2) == 0)
            {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL)
                {
                    // Advance ptr to ignore '=' and extract value
                    p++;
                    newConf.rl = strtod(p, NULL);
                }
                else
                {
                    parseError = true;
                    break;
                }
            }
            else if (strncmp(argv[idx], "Ku", 2) == 0)
            {
                // Get substring after '=' char
                p = strstr(argv[idx], "=");
                if (p != NULL)
                {
                    // Advance ptr to ignore '=' and extract value
                    p++;
                    newConf.ku = strtol(p, NULL, 10);
                }
                else
                {
                    parseError = true;
                    break;
                }
            }
            else
            {
                parseError = true;
                break;
            }
        }
    }
    else
    {
        // Incorrect parameter number
        parseError = true;
    }

    if (parseError)
    {
        SYS_CMD_MESSAGE("Unsupported Command !\n\r");
    }
    else
    {
        APP_METROLOGY_SetConfiguration(&newConf);
        SYS_CMD_MESSAGE("Configure Meter is Ok !\n\r");
    }
}

static void Command_DAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t idx;

    if (argc == 1)
    {
        // Read all metrology accumulator registers
        app_consoleData.accumRegToRead = 0;
        app_consoleData.state = APP_CONSOLE_STATE_READ_ALL_ACCUM_REGS;
        // Post semaphore to wakeup task
        OSAL_SEM_Post(&appConsoleSemID);
    }
    else if (argc == 2)
    {
        // Extract register index from parameters
        idx = (uint8_t)strtol(argv[1], NULL, 10);
        if (idx < ACCUMULATOR_REG_NUM)
        {
            // Read register value
            app_consoleData.accumRegToRead = idx;
            app_consoleData.state = APP_CONSOLE_STATE_READ_ACCUM_REG;
            // Post semaphore to wakeup task
            OSAL_SEM_Post(&appConsoleSemID);
        }
        else
        {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\n\r");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DCB(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1)
    {
        SYS_CMD_MESSAGE("Entering Low Power\n\r");
        // Go to Low Power mode
//API         DRV_MET_SetLowPowerMode();
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DCD(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1)
    {
        SYS_CMD_MESSAGE("Load Default Is Ok !\n\r");
        // Set default control register values
        APP_METROLOGY_SetControlByDefault();
    }
    else
    {
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

    if (argc > 1)
    {
        // Each argument includes reg index and value
        numRegsToWrite = (argc - 1);
        for (i = 0; i < numRegsToWrite; i++)
        {
            // Extract register index and value from argument
            p = argv[i + 1];
            app_consoleData.regsToModify[i].index = (uint8_t)strtol(p, NULL, 10);
            // Look for ":" char and advance to next char
            p = strstr(p, ":");
            if (p != NULL)
            {
                p++;
                app_consoleData.regsToModify[i].value = (uint32_t)strtol(p, NULL, 16);
            }
            else
            {
                SYS_CMD_MESSAGE("Unsupported Command !\n\r");
                parseError = true;
                break;
            }
        }

        if (!parseError)
        {
            // Write invalid values after last, to later detect it
            app_consoleData.regsToModify[i].index = 0xFF;
            app_consoleData.regsToModify[i].value = 0xFFFFFFFF;
            // Go to corresponding state
            app_consoleData.state = APP_CONSOLE_STATE_WRITE_CONTROL_REG;
            // Post semaphore to wakeup task
            OSAL_SEM_Post(&appConsoleSemID);
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DCR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t idx;

    if (argc == 1)
    {
        // Read all metrology control registers
        app_consoleData.ctrlRegToRead = 0;
        app_consoleData.state = APP_CONSOLE_STATE_READ_ALL_CONTROL_REGS;
        // Post semaphore to wakeup task
        OSAL_SEM_Post(&appConsoleSemID);
    }
    else if (argc == 2)
    {
        // Extract register index from parameters
        idx = (uint8_t)strtol(argv[1], NULL, 10);
        if (idx < CONTROL_REG_NUM)
        {
            // Read register value
            app_consoleData.ctrlRegToRead = idx;
            app_consoleData.state = APP_CONSOLE_STATE_READ_CONTROL_REG;
            // Post semaphore to wakeup task
            OSAL_SEM_Post(&appConsoleSemID);
        }
        else
        {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\n\r");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DCS(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1)
    {
        SYS_CMD_MESSAGE("Save Data Is Ok !\n\r");
        // Save Metrology Constants and configuration settings to NVM
        APP_METROLOGY_StoreMetrologyData();
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DCW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t idx;
    uint32_t regValue;

    if (argc == 3)
    {
        // Extract register index from parameters
        idx = (uint8_t)strtol(argv[1], NULL, 10);
        if (idx < CONTROL_REG_NUM)
        {
            // Extract register value
            regValue = (uint32_t)strtol(argv[2], NULL, 16);
            // Write register value
            if (APP_METROLOGY_SetControlRegister((CONTROL_REG_ID)idx, regValue))
            {
                // Show response on console
                SYS_CMD_MESSAGE("Set Is Ok !\n\r");
            }
            else
            {
                // Cannot write register
                SYS_CMD_PRINT("Could not write register %02d\n\r", idx);
            }
        }
        else
        {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\n\r");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_DSR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t idx;

    if (argc == 1)
    {
        // Read all metrology status registers
        app_consoleData.statusRegToRead = 0;
        app_consoleData.state = APP_CONSOLE_STATE_READ_ALL_STATUS_REGS;
        // Post semaphore to wakeup task
        OSAL_SEM_Post(&appConsoleSemID);
    }
    else if (argc == 2)
    {
        // Extract register index from parameters
        idx = (uint8_t)strtol(argv[1], NULL, 10);
        if (idx < STATUS_REG_NUM)
        {
            // Read register value
            app_consoleData.statusRegToRead = idx;
            app_consoleData.state = APP_CONSOLE_STATE_READ_STATUS_REG;
            // Post semaphore to wakeup task
            OSAL_SEM_Post(&appConsoleSemID);
        }
        else
        {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\n\r");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_ENC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 2)
    {
        // Check password from parameters
        if (strcmp(argv[1], metPwd) == 0)
        {
            // Correct password, Clear Energy records
            APP_ENERGY_ClearEnergy();
            // Show response on console
            SYS_CMD_MESSAGE("Clear Energy is ok !\n\r");
        }
        else
        {
            // Invalid password
            SYS_CMD_MESSAGE("Invalid password\n\r");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_ENR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t monthIndex;

    if (argc == 2) {
        RTC_TimeGet(&sysTime);
        // Extract month index from parameters
        monthIndex = (uint8_t)strtol(argv[1], NULL, 10);
        monthIndex %= 12;
        if (sysTime.tm_mon < monthIndex)
        {
            sysTime.tm_mon += 12;
            sysTime.tm_year--;
        }
        sysTime.tm_mon -= monthIndex;

        // Get monthly energy from energy app
        if (APP_ENERGY_GetMonthEnergy(&sysTime) == false)
        {
            // Incorrect parameter number
            SYS_CMD_MESSAGE("Incorrect param\n\r");
        }
        // Response will be provided on _monthlyEnergyCallback function
    }
    else
    {
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
        if (idx < HARMONICS_REG_NUM) {
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
            memcpy(&app_consoleStorageData.meterID, argv[2], sizeof(app_consoleStorageData.meterID));
            // Build queue element to write it to storage
            datalogQueueElement.userId = APP_DATALOG_USER_CONSOLE;
            datalogQueueElement.operation = APP_DATALOG_WRITE;
            datalogQueueElement.endCallback = NULL;
            datalogQueueElement.sysTime = sysTime; /* Don't need to be updated, not used */
            datalogQueueElement.dataLen = sizeof(app_consoleStorageData);
            datalogQueueElement.pData = (uint8_t*)&app_consoleStorageData;
            // Put it in queue
            xQueueSend(appDatalogQueueID, &datalogQueueElement, (TickType_t)0);
            SYS_CMD_MESSAGE("Set Meter ID is Ok\n\r");
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
            APP_ENERGY_ClearMaxDemand();
            // Show response on console
            SYS_CMD_MESSAGE("Clear MaxDemand is ok !\n\r");
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
        RTC_TimeGet(&app_consoleData.timeRequest);
        // Extract month index from parameters
        monthIndex = (uint8_t)strtol(argv[1], NULL, 10);
        monthIndex %= 12;
        if (app_consoleData.timeRequest.tm_mon < monthIndex)
        {
            app_consoleData.timeRequest.tm_mon += 12;
            app_consoleData.timeRequest.tm_year--;
        }
        app_consoleData.timeRequest.tm_mon -= monthIndex;

        // Get max demand from energy app
        if (APP_ENERGY_GetMonthMaxDemand(&app_consoleData.timeRequest) == false)
        {
            // Incorrect parameter number
            SYS_CMD_MESSAGE("Incorrect param\n\r");
        }
        // Response will be provided on _maxDemandCallback function
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\n\r");
    }
}

static void Command_PAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    bool wakeup = false;

    if (argc == 2)
    {
        // Extract data to retrieve from parameters
        if (strcmp(argv[1], "U") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_VOLTAGE;
            wakeup = true;
        }
        else if (strcmp(argv[1], "I") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_CURRENT;
            wakeup = true;
        }
        else if (strcmp(argv[1], "P") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_ACTIVE_POWER;
            wakeup = true;
        }
        else if (strcmp(argv[1], "Q") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_REACTIVE_POWER;
            wakeup = true;
        }
        else if (strcmp(argv[1], "S") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_APARENT_POWER;
            wakeup = true;
        }
        else if (strcmp(argv[1], "F") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_FREQUENCY;
            wakeup = true;
        }
        else if (strcmp(argv[1], "A") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_ANGLE;
            wakeup = true;
        }
        else
        {
            // Invalid Command
            SYS_CMD_MESSAGE("Unsupported Command !\n\r");
        }

        if (wakeup)
        {
            // Post semaphore to wakeup task
            OSAL_SEM_Post(&appConsoleSemID);
        }
    }
    else
    {
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
            char *p;

            // Get Date
            p = argv[2];
            // Year
            sysTime.tm_year = (uint8_t)strtol(p, NULL, 10) + 2000 - 1900;
            // Look for "-" char and advance to next char
            p = strstr(p, "-");
            if (p != NULL) {
                p++;
                // Month
                sysTime.tm_mon = (uint8_t)strtol(p, NULL, 10) - 1;
                // Look for "-" char and advance to next char
                p = strstr(p, "-");
                if (p != NULL) {
                    p++;
                    // Day
                    sysTime.tm_mday = (uint8_t)strtol(p, NULL, 10);
                }
            }

            // Get Week Day
            p = argv[3];
            sysTime.tm_wday = (uint8_t)strtol(p, NULL, 10) - 1;

            // Get Time
            p = argv[4];
            // Hour
            sysTime.tm_hour = (uint8_t)strtol(p, NULL, 10);
            // Look for ":" char and advance to next char
            p = strstr(p, ":");
            if (p != NULL) {
                p++;
                // Minute
                sysTime.tm_min = (uint8_t)strtol(p, NULL, 10);
                // Look for ":" char and advance to next char
                p = strstr(p, ":");
                if (p != NULL) {
                    p++;
                    // Second
                    sysTime.tm_sec = (uint8_t)strtol(p, NULL, 10);
                }
            }

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
    char *p;
    APP_ENERGY_TOU_TIME_ZONE timeZone[APP_ENERGY_TOU_MAX_ZONES];
    uint8_t idx, argIdx;
    bool parseError = false;

    if ((argc > 3) && ((argc - 2) % 2 == 0))
    {
        // Check password from parameters
        if (strcmp(argv[1], metPwd) == 0)
        {
             // Correct password, write TOW
             for (idx = 0; idx < APP_ENERGY_TOU_MAX_ZONES; idx++)
             {
                // Check whether there are arguments left to write
                argIdx = ((idx << 1) + 2);
                if (argc > argIdx) {
                    // Extract hour, minute and rate
                    p = argv[argIdx];
                    // Extract hour from argument
                    timeZone[idx].hour = (uint8_t)strtol(p, NULL, 10);
                    // Look for ":" char and advance to next char
                    p = strstr(p, ":");
                    if (p != NULL) {
                        p++;
                        // Extract minute from argument
                        timeZone[idx].minute = (uint8_t)strtol(p, NULL, 10);
                        // Extract rate from next argument
                        timeZone[idx].tariff = (uint8_t)strtol(argv[argIdx + 1], NULL, 10);
                    }
                    else {
                        parseError = true;
                        break;
                    }
                    if ((timeZone[idx].hour > 23) ||
                        (timeZone[idx].minute > 59) ||
                        (timeZone[idx].tariff > TARIFF_4)) {
                            parseError = true;
                            break;
                    }
                }
                else {
                    // No more arguments, fill TOU index with invalid data
                    timeZone[idx].hour = 0;
                    timeZone[idx].minute = 0;
                    timeZone[idx].tariff = TARIFF_INVALID;
                }
             }
        }
        else
        {
            // Invalid password
            parseError = true;
        }
    }
    else
    {
        // Incorrect parameter number
        parseError = true;
    }

    if (parseError)
    {
        SYS_CMD_MESSAGE("Unsupported Command !\n\r");
    }
    else
    {
        APP_ENERGY_SetTOUTimeZone(timeZone);
        SYS_CMD_MESSAGE("Set TOU is Ok !\n\r");
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
    char regName[4][18];
    uint32_t regValue32[4];
    uint64_t regValue64[4];
    uint8_t numRegsPending;

    /* Check the application's current state. */
    switch ( app_consoleData.state )
    {
        /* Application's initial state. */
        case APP_CONSOLE_STATE_INIT:
        {
            if (SYS_CMD_READY_TO_READ())
            {
                /* Initialize Metrology Driver callbacks */
                APP_ENERGY_SetMonthEnergyCallback(_monthlyEnergyCallback, &energyLocalObject);
                APP_ENERGY_SetMaxDemandCallback(_maxDemandCallback, &maxDemandLocalObject);
//API                 DRV_MET_HRRCallbackRegister(_hrrCallback);
//API                 DRV_MET_CalibrationCallbackRegister(_calibrationCallback);
//API                 DRV_MET_EventCallbackRegister(_eventCallback);
//API                 DRV_MET_WaveformDataCallbackRegister(_waveformDataCallback);
//API                 memset(touObject, 0xFF, sizeof(touObject)); /* 0xFF means invalid tariff */
                if ((OSAL_SEM_Create(&appConsoleSemID, OSAL_SEM_TYPE_BINARY, 1, 0) == OSAL_RESULT_TRUE) &&
                    (OSAL_SEM_Create(&appConsoleStorageSemID, OSAL_SEM_TYPE_BINARY, 1, 0) == OSAL_RESULT_TRUE)) {
                    app_consoleData.state = APP_CONSOLE_STATE_WAIT_STORAGE_READY;
                }
                // Set default console storage data just in case it cannot be read later
                app_consoleStorageData = app_defaultConsoleStorageData;
            }
            break;
        }

        case APP_CONSOLE_STATE_WAIT_STORAGE_READY:
        {
            if (APP_DATALOG_GetStatus() == APP_DATALOG_STATE_READY) {
               app_consoleData.state = APP_CONSOLE_STATE_READ_STORAGE;
            }
            else
            {
                vTaskDelay(CONSOLE_TASK_DELAY_MS_UNTIL_DATALOG_READY / portTICK_PERIOD_MS);
                currentWaitForDatalogReady += CONSOLE_TASK_DELAY_MS_UNTIL_DATALOG_READY;
                if (currentWaitForDatalogReady > CONSOLE_MAX_WAIT_MS_UNTIL_DATALOG_READY) {
                    // Go to Datalog not ready state
                    app_consoleData.state = APP_CONSOLE_STATE_DATALOG_NOT_READY;
                }
            }
            break;
        }

        case APP_CONSOLE_STATE_READ_STORAGE:
        {
            // Build queue element
            datalogQueueElement.userId = APP_DATALOG_USER_CONSOLE;
            datalogQueueElement.operation = APP_DATALOG_READ;
            datalogQueueElement.endCallback = _consoleReadStorage;
            datalogQueueElement.sysTime = sysTime; /* Don't need to be updated, not used */
            datalogQueueElement.dataLen = sizeof(app_consoleStorageData);
            datalogQueueElement.pData = (uint8_t*)&app_consoleStorageData;
            // Put it in queue
            xQueueSend(appDatalogQueueID, &datalogQueueElement, (TickType_t)0);
            // Wait for data to be read (semaphore is released in callback)
            OSAL_SEM_Pend(&appConsoleStorageSemID, OSAL_WAIT_FOREVER);
            // Data read, depending on read result, state has changed to READ_OK or READ_ERROR
            break;
        }

        case APP_CONSOLE_STATE_READ_STORAGE_OK:
        {
            SYS_CMD_MESSAGE("Storage Successfully Read\n\r");
            // Go to Idle state
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_READ_STORAGE_ERROR:
        {
            SYS_CMD_MESSAGE("No Console Data found in Storage\n\r");
            // Go to Idle state
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_DATALOG_NOT_READY:
        {
            SYS_CMD_MESSAGE("Datalog Service not ready!\n\rApplication will run without storage capabilities\n\r");
            // Go to Idle state
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
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
            if (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID)app_consoleData.ctrlRegToRead, &regValue32[0], regName[0])) {
                SYS_CMD_PRINT("%s\n\r%X\n\r", regName[0], regValue32[0]);
            }
            else {
                // Cannot read register
                SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.ctrlRegToRead);
            }
            // Go back to Idle
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_WRITE_CONTROL_REG:
        {
            uint8_t idx;
            for (idx = 0; idx < APP_CONSOLE_MAX_REGS; idx++) {
                if (app_consoleData.regsToModify[idx].index != 0xFF) {
                    if (app_consoleData.regsToModify[idx].index < CONTROL_REG_NUM) {
                        // Write register value
                        if (APP_METROLOGY_SetControlRegister((CONTROL_REG_ID)app_consoleData.regsToModify[idx].index, app_consoleData.regsToModify[idx].value)) {
                            // Show response on console
                            SYS_CMD_PRINT("Set %02d Is Ok !\n\r", app_consoleData.regsToModify[idx].index);
                        }
                        else {
                            // Cannot write register
                            SYS_CMD_PRINT("Could not write register %02d\n\r", app_consoleData.regsToModify[idx].index);
                        }
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
                    vTaskDelay(CONSOLE_TASK_DEFAULT_DELAY_MS_BETWEEN_STATES / portTICK_PERIOD_MS);
                }
            }
            break;
        }

        case APP_CONSOLE_STATE_READ_ALL_CONTROL_REGS:
        {
            if (app_consoleData.ctrlRegToRead < CONTROL_REG_NUM)
            {
                if (app_consoleData.ctrlRegToRead == 0)
                {
                    SYS_CMD_MESSAGE("\n\r");
                }

                // Check how many registers are pending to print, to format line
                numRegsPending = CONTROL_REG_NUM - app_consoleData.ctrlRegToRead;
                // Read and print register values
                if (numRegsPending >= 4)
                {
                    if ((APP_METROLOGY_GetControlRegister((CONTROL_REG_ID)app_consoleData.ctrlRegToRead, &regValue32[0], regName[0])) &&
                        (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID)app_consoleData.ctrlRegToRead + 1, &regValue32[1], regName[1])) &&
                        (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID)app_consoleData.ctrlRegToRead + 2, &regValue32[2], regName[2])) &&
                        (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID)app_consoleData.ctrlRegToRead + 3, &regValue32[3], regName[3]))) {
                        SYS_CMD_PRINT("%-19s%-19s%-19s%-19s\n\r", regName[0], regName[1], regName[2], regName[3]);
                        SYS_CMD_PRINT("%-19X%-19X%-19X%-19X\n\r", regValue32[0], regValue32[1], regValue32[2], regValue32[3]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.ctrlRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.ctrlRegToRead += 4;
                }
                else if (numRegsPending == 3)
                {
                    if ((APP_METROLOGY_GetControlRegister((CONTROL_REG_ID)app_consoleData.ctrlRegToRead, &regValue32[0], regName[0])) &&
                        (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID)app_consoleData.ctrlRegToRead + 1, &regValue32[1], regName[1])) &&
                        (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID)app_consoleData.ctrlRegToRead + 2, &regValue32[2], regName[2]))) {
                        SYS_CMD_PRINT("%-19s%-19s%-19s\n\r", regName[0], regName[1], regName[2]);
                        SYS_CMD_PRINT("%-19X%-19X%-19X\n\r", regValue32[0], regValue32[1], regValue32[2]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.ctrlRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.ctrlRegToRead += 3;
                }
                else if (numRegsPending == 2)
                {
                    if ((APP_METROLOGY_GetControlRegister((CONTROL_REG_ID)app_consoleData.ctrlRegToRead, &regValue32[0], regName[0])) &&
                        (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID)app_consoleData.ctrlRegToRead + 1, &regValue32[1], regName[1]))) {
                        SYS_CMD_PRINT("%-19s%-19s\n\r", regName[0], regName[1]);
                        SYS_CMD_PRINT("%-19X%-19X\n\r", regValue32[0], regValue32[1]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.ctrlRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.ctrlRegToRead += 2;
                }
                else if (numRegsPending == 1)
                {
                    if ((APP_METROLOGY_GetControlRegister((CONTROL_REG_ID)app_consoleData.ctrlRegToRead, &regValue32[0], regName[0]))) {
                        SYS_CMD_PRINT("%-19s\n\r", regName[0]);
                        SYS_CMD_PRINT("%-19X\n\r", regValue32[0]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.ctrlRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.ctrlRegToRead += 1;
                }
            }
            else {
                // All registers have been read
                app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            }
            vTaskDelay(CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT / portTICK_PERIOD_MS);
            break;
        }

        case APP_CONSOLE_STATE_READ_ACCUM_REG:
        {
            // Read register value
            if (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID)app_consoleData.accumRegToRead, &regValue64[0], regName[0])) {
                SYS_CMD_PRINT("%s\n\r%X\n\r", regName[0], regValue64[0]);
            }
            else {
                // Cannot read register
                SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.accumRegToRead);
            }
            // Go back to Idle
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_READ_ALL_ACCUM_REGS:
        {
            if (app_consoleData.accumRegToRead < ACCUMULATOR_REG_NUM) {
                if (app_consoleData.accumRegToRead == 0)
                {
                    SYS_CMD_MESSAGE("\n\r");
                }

                // Check how many registers are pending to print, to format line
                numRegsPending = ACCUMULATOR_REG_NUM - app_consoleData.accumRegToRead;
                // Read and print register values
                if (numRegsPending >= 4)
                {
                    if ((APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID)app_consoleData.accumRegToRead, &regValue64[0], regName[0])) &&
                        (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID)app_consoleData.accumRegToRead + 1, &regValue64[1], regName[1])) &&
                        (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID)app_consoleData.accumRegToRead + 2, &regValue64[2], regName[2])) &&
                        (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID)app_consoleData.accumRegToRead + 3, &regValue64[3], regName[3]))) {
                        SYS_CMD_PRINT("%-19s%-19s%-19s%-19s\n\r", regName[0], regName[1], regName[2], regName[3]);
                        SYS_CMD_PRINT("%-19X%-19X%-19X%-19X\n\r", regValue64[0], regValue64[1], regValue64[2], regValue64[3]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.accumRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.accumRegToRead += 4;
                }
                else if (numRegsPending == 3)
                {
                    if ((APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID)app_consoleData.accumRegToRead, &regValue64[0], regName[0])) &&
                        (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID)app_consoleData.accumRegToRead + 1, &regValue64[1], regName[1])) &&
                        (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID)app_consoleData.accumRegToRead + 2, &regValue64[2], regName[2]))) {
                        SYS_CMD_PRINT("%-19s%-19s%-19s\n\r", regName[0], regName[1], regName[2]);
                        SYS_CMD_PRINT("%-19X%-19X%-19X\n\r", regValue64[0], regValue64[1], regValue64[2]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.accumRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.accumRegToRead += 3;
                }
                else if (numRegsPending == 2)
                {
                    if ((APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID)app_consoleData.accumRegToRead, &regValue64[0], regName[0])) &&
                        (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID)app_consoleData.accumRegToRead + 1, &regValue64[1], regName[1]))) {
                        SYS_CMD_PRINT("%-19s%-19s\n\r", regName[0], regName[1]);
                        SYS_CMD_PRINT("%-19X%-19X\n\r", regValue64[0], regValue64[1]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.accumRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.accumRegToRead += 2;
                }
                else if (numRegsPending == 1)
                {
                    if ((APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID)app_consoleData.accumRegToRead, &regValue64[0], regName[0]))) {
                        SYS_CMD_PRINT("%-19s\n\r", regName[0]);
                        SYS_CMD_PRINT("%-19X\n\r", regValue64[0]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.accumRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.accumRegToRead += 1;
                }
            }
            else {
                // All registers have been read
                app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            }
            vTaskDelay(CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT / portTICK_PERIOD_MS);
            break;
        }

        case APP_CONSOLE_STATE_READ_STATUS_REG:
        {
            // Read register value
            if (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID)app_consoleData.statusRegToRead, &regValue32[0], regName[0])) {
                SYS_CMD_PRINT("%s\n\r%X\n\r", regName[0], regValue32[0]);
            }
            else {
                // Cannot read register
                SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.statusRegToRead);
            }
            // Go back to Idle
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_READ_ALL_STATUS_REGS:
        {
            if (app_consoleData.statusRegToRead < STATUS_REG_NUM) {
                if (app_consoleData.statusRegToRead == 0)
                {
                    SYS_CMD_MESSAGE("\n\r");
                }

                // Check how many registers are pending to print, to format line
                numRegsPending = STATUS_REG_NUM - app_consoleData.statusRegToRead;
                // Read and print register values
                if (numRegsPending >= 4)
                {
                    if ((APP_METROLOGY_GetStatusRegister((STATUS_REG_ID)app_consoleData.statusRegToRead, &regValue32[0], regName[0])) &&
                        (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID)app_consoleData.statusRegToRead + 1, &regValue32[1], regName[1])) &&
                        (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID)app_consoleData.statusRegToRead + 2, &regValue32[2], regName[2])) &&
                        (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID)app_consoleData.statusRegToRead + 3, &regValue32[3], regName[3]))) {
                        SYS_CMD_PRINT("%-19s%-19s%-19s%-19s\n\r", regName[0], regName[1], regName[2], regName[3]);
                        SYS_CMD_PRINT("%-19X%-19X%-19X%-19X\n\r", regValue32[0], regValue32[1], regValue32[2], regValue32[3]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.statusRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.statusRegToRead += 4;
                }
                else if (numRegsPending == 3)
                {
                    if ((APP_METROLOGY_GetStatusRegister((STATUS_REG_ID)app_consoleData.statusRegToRead, &regValue32[0], regName[0])) &&
                        (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID)app_consoleData.statusRegToRead + 1, &regValue32[1], regName[1])) &&
                        (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID)app_consoleData.statusRegToRead + 2, &regValue32[2], regName[2]))) {
                        SYS_CMD_PRINT("%-19s%-19s%-19s\n\r", regName[0], regName[1], regName[2]);
                        SYS_CMD_PRINT("%-19X%-19X%-19X\n\r", regValue32[0], regValue32[1], regValue32[2]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.statusRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.statusRegToRead += 3;
                }
                else if (numRegsPending == 2)
                {
                    if ((APP_METROLOGY_GetStatusRegister((STATUS_REG_ID)app_consoleData.statusRegToRead, &regValue32[0], regName[0])) &&
                        (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID)app_consoleData.statusRegToRead + 1, &regValue32[1], regName[1]))) {
                        SYS_CMD_PRINT("%-19s%-19s\n\r", regName[0], regName[1]);
                        SYS_CMD_PRINT("%-19X%-19X\n\r", regValue32[0], regValue32[1]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.statusRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.statusRegToRead += 2;
                }
                else if (numRegsPending == 1)
                {
                    if ((APP_METROLOGY_GetStatusRegister((STATUS_REG_ID)app_consoleData.statusRegToRead, &regValue32[0], regName[0]))) {
                        SYS_CMD_PRINT("%-19s\n\r", regName[0]);
                        SYS_CMD_PRINT("%-19X\n\r", regValue32[0]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.statusRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.statusRegToRead += 1;
                }
            }
            else {
                // All registers have been read
                app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            }
            vTaskDelay(CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT / portTICK_PERIOD_MS);
            break;
        }

        case APP_CONSOLE_STATE_READ_HARMONICS_REG:
        {
            // Read register value
            if (APP_METROLOGY_GetHarmonicsRegister((HARMONICS_REG_ID)app_consoleData.harRegToRead, &regValue32[0], regName[0])) {
                SYS_CMD_PRINT("%s\n\r%X\n\r", regName[0], regValue32[0]);
            }
            else {
                // Cannot read register
                SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.harRegToRead);
            }
            // Go back to Idle
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_READ_ALL_HARMONICS_REGS:
        {
            if (app_consoleData.harRegToRead < HARMONICS_REG_NUM) {
                if (app_consoleData.harRegToRead == 0)
                {
                    SYS_CMD_MESSAGE("\n\r");
                }

                // Check how many registers are pending to print, to format line
                numRegsPending = HARMONICS_REG_NUM - app_consoleData.harRegToRead;
                // Read and print register values
                if (numRegsPending >= 4)
                {
                    if ((APP_METROLOGY_GetHarmonicsRegister((HARMONICS_REG_ID)app_consoleData.harRegToRead, &regValue32[0], regName[0])) &&
                        (APP_METROLOGY_GetHarmonicsRegister((HARMONICS_REG_ID)app_consoleData.harRegToRead + 1, &regValue32[1], regName[1])) &&
                        (APP_METROLOGY_GetHarmonicsRegister((HARMONICS_REG_ID)app_consoleData.harRegToRead + 2, &regValue32[2], regName[2])) &&
                        (APP_METROLOGY_GetHarmonicsRegister((HARMONICS_REG_ID)app_consoleData.harRegToRead + 3, &regValue32[3], regName[3]))) {
                        SYS_CMD_PRINT("%-19s%-19s%-19s%-19s\n\r", regName[0], regName[1], regName[2], regName[3]);
                        SYS_CMD_PRINT("%-19X%-19X%-19X%-19X\n\r", regValue32[0], regValue32[1], regValue32[2], regValue32[3]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.harRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.harRegToRead += 4;
                }
                else if (numRegsPending == 3)
                {
                    if ((APP_METROLOGY_GetHarmonicsRegister((HARMONICS_REG_ID)app_consoleData.harRegToRead, &regValue32[0], regName[0])) &&
                        (APP_METROLOGY_GetHarmonicsRegister((HARMONICS_REG_ID)app_consoleData.harRegToRead + 1, &regValue32[1], regName[1])) &&
                        (APP_METROLOGY_GetHarmonicsRegister((HARMONICS_REG_ID)app_consoleData.harRegToRead + 2, &regValue32[2], regName[2]))) {
                        SYS_CMD_PRINT("%-19s%-19s%-19s\n\r", regName[0], regName[1], regName[2]);
                        SYS_CMD_PRINT("%-19X%-19X%-19X\n\r", regValue32[0], regValue32[1], regValue32[2]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.harRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.harRegToRead += 3;
                }
                else if (numRegsPending == 2)
                {
                    if ((APP_METROLOGY_GetHarmonicsRegister((HARMONICS_REG_ID)app_consoleData.harRegToRead, &regValue32[0], regName[0])) &&
                        (APP_METROLOGY_GetHarmonicsRegister((HARMONICS_REG_ID)app_consoleData.harRegToRead + 1, &regValue32[1], regName[1]))) {
                        SYS_CMD_PRINT("%-19s%-19s\n\r", regName[0], regName[1]);
                        SYS_CMD_PRINT("%-19X%-19X\n\r", regValue32[0], regValue32[1]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.harRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.harRegToRead += 2;
                }
                else if (numRegsPending == 1)
                {
                    if ((APP_METROLOGY_GetHarmonicsRegister((HARMONICS_REG_ID)app_consoleData.harRegToRead, &regValue32[0], regName[0]))) {
                        SYS_CMD_PRINT("%-19s\n\r", regName[0]);
                        SYS_CMD_PRINT("%-19X\n\r", regValue32[0]);
                    }
                    else {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\n\r", app_consoleData.harRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.harRegToRead += 1;
                }
            }
            else {
                // All registers have been read
                app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            }
            vTaskDelay(CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT / portTICK_PERIOD_MS);
            break;
        }

        case APP_CONSOLE_STATE_READ_METER_ID:
        {
            // Show response on console
            SYS_CMD_MESSAGE("Meter ID is :\n\r");
            SYS_CMD_PRINT("%s\n\r", app_consoleStorageData.meterID);
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
            APP_ENERGY_TOU_TIME_ZONE * timeZone;
            uint8_t idx;

            timeZone = APP_ENERGY_GetTOUTimeZone();

            SYS_CMD_MESSAGE("TOU table is :\n\r");
            for (idx = 0; idx < APP_ENERGY_TOU_MAX_ZONES; idx++, timeZone++)
            {
                if (timeZone->tariff != TARIFF_INVALID)
                {
                    SYS_CMD_PRINT("T%d=%02d:%02d %d ",
                        (idx + 1), timeZone->hour, timeZone->minute, timeZone->tariff);
                }
            }
            SYS_CMD_MESSAGE("\n\r");

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
            RTC_TimeGet(&sysTime);
            
            /* Check if data received through callback is valid */
            if (app_consoleData.dataValid)
            {
                uint64_t total = 0;
                uint8_t idx;
                
                for (idx = 0; idx < TARIFF_NUM_TYPE; idx ++)
                {
                    total += energyLocalObject.tariff[idx];
                }
                
                // Show received data on console
                SYS_CMD_PRINT("Last %d Month Energy is :\n\r", sysTime.tm_mon - app_consoleData.timeRequest.tm_mon);

                SYS_CMD_PRINT("TT=%.2fkWh T1=%.2fkWh T2=%.2fkWh T3=%.2fkWh T4=%.2fkWh\r\n",
                    (float)total/10000000, (float)energyLocalObject.tariff[0]/10000000, (float)energyLocalObject.tariff[1]/10000000, 
                        (float)energyLocalObject.tariff[2]/10000000, (float)energyLocalObject.tariff[3]/10000000);
            }
            else
            {
                // Data is not found
                SYS_CMD_PRINT("Last %d Month Energy is not found\n\r", sysTime.tm_mon - app_consoleData.timeRequest.tm_mon);
            }

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            


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
            RTC_TimeGet(&sysTime);
            
            /* Check if data received through callback is valid */
            if (app_consoleData.dataValid)
            {
                APP_ENERGY_DEMAND_DATA *pDataMax;
                APP_ENERGY_DEMAND_DATA *pDataTariff;
                
                // Show received data on console
                SYS_CMD_PRINT("Last %d Month MaxDemand is :\n\r", sysTime.tm_mon - app_consoleData.timeRequest.tm_mon);

                pDataMax = &maxDemandLocalObject.maxDemad;
                pDataTariff = &maxDemandLocalObject.tariff[0];
                SYS_CMD_PRINT("TT=%.3fkW %d-%d %02d:%02d T1=%.3fkW %d-%d %02d:%02d T2=%.3fkW %d-%d %02d:%02d T3=%.3fkW %d-%d %02d:%02d T4=%.3fkW %d-%d %02d:%02d\n\r",
                        (float)pDataMax->value/1000, pDataMax->month, pDataMax->day, pDataMax->hour, pDataMax->minute,
                        (float)pDataTariff->value/1000, pDataTariff->month, pDataTariff->day, pDataTariff->hour, pDataTariff->minute,
                        (float)(pDataTariff + 1)->value/1000, (pDataTariff + 1)->month, (pDataTariff + 1)->day, (pDataTariff + 1)->hour, (pDataTariff + 1)->minute,
                        (float)(pDataTariff + 2)->value/1000, (pDataTariff + 2)->month, (pDataTariff + 2)->day, (pDataTariff + 2)->hour, (pDataTariff + 2)->minute,
                        (float)(pDataTariff + 3)->value/1000, (pDataTariff + 3)->month, (pDataTariff + 3)->day, (pDataTariff + 3)->hour, (pDataTariff + 3)->minute);
            }
            else
            {
                // Data is not found
                SYS_CMD_PRINT("Last %d Month MaxDemand is not found\n\r", sysTime.tm_mon - app_consoleData.timeRequest.tm_mon);
            }

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_VOLTAGE:
        {
            uint32_t va, vb, vc;

            APP_METROLOGY_GetRMS(RMS_UA, &va, 0);
            APP_METROLOGY_GetRMS(RMS_UB, &vb, 0);
            APP_METROLOGY_GetRMS(RMS_UC, &vc, 0);
            // Show received data on console
            SYS_CMD_PRINT("Present voltage is :\n\rUa=%.3fV Ub=%.3fV Uc=%.3fV\r\n",(float)va/10000, (float)va/10000, (float)vc/10000);

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_CURRENT:
        {
            uint32_t ia, ib, ic;
            uint32_t ini, inm, inmi;

            APP_METROLOGY_GetRMS(RMS_IA, &ia, 0);
            APP_METROLOGY_GetRMS(RMS_IB, &ib, 0);
            APP_METROLOGY_GetRMS(RMS_IC, &ic, 0);
            APP_METROLOGY_GetRMS(RMS_INI, &ini, 0);
            APP_METROLOGY_GetRMS(RMS_INM, &inm, 0);
            APP_METROLOGY_GetRMS(RMS_INMI, &inmi, 0);
            // Show received data on console
            SYS_CMD_PRINT("Present current is :\n\rIa=%fA Ib=%fA Ic=%fA Ini=%fA Inm=%fA Inmi=%fA\n\r",
                    (float)ia/10000, (float)ib/10000, (float)ic/10000, (float)ini/10000,
                    (float)inm/10000, (float)inmi/10000);

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_ACTIVE_POWER:
        {
            uint32_t pt, pa, pb, pc;
            DRV_METROLOGY_RMS_SIGN signt, signa, signb, signc;

            APP_METROLOGY_GetRMS(RMS_PT, &pt, &signt);
            APP_METROLOGY_GetRMS(RMS_PA, &pa, &signa);
            APP_METROLOGY_GetRMS(RMS_PB, &pb, &signb);
            APP_METROLOGY_GetRMS(RMS_PC, &pc, &signc);
            // Show received data on console
            SYS_CMD_PRINT("Present active power is :\n\rPt=%c%.1fW Pa=%c%.1fW Pb=%c%.1fW Pc=%c%.1fW\r\n",
                   sign[signt], (float)pt/10, sign[signa], (float)pa/10, sign[signb],
                    (float)pb/10, sign[signc], (float)pc/10);

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_REACTIVE_POWER:
        {
            uint32_t qt, qa, qb, qc;
            DRV_METROLOGY_RMS_SIGN signt, signa, signb, signc;

            APP_METROLOGY_GetRMS(RMS_QT, &qt, &signt);
            APP_METROLOGY_GetRMS(RMS_QA, &qa, &signa);
            APP_METROLOGY_GetRMS(RMS_QB, &qb, &signb);
            APP_METROLOGY_GetRMS(RMS_QC, &qc, &signc);
            // Show received data on console
            SYS_CMD_PRINT("Present reactive power is :\n\rQt=%c%.1fW Qa=%c%.1fW Qb=%c%.1fW qc=%c%.1fW\r\n",
                   sign[signt], (float)qt/10, sign[signa], (float)qa/10, sign[signb],
                    (float)qb/10, sign[signc], (float)qc/10);

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_APARENT_POWER:
        {
            uint32_t st, sa, sb, sc;

            APP_METROLOGY_GetRMS(RMS_ST, &st, 0);
            APP_METROLOGY_GetRMS(RMS_SA, &sa, 0);
            APP_METROLOGY_GetRMS(RMS_SB, &sb, 0);
            APP_METROLOGY_GetRMS(RMS_SC, &sc, 0);
            // Show received data on console
            SYS_CMD_PRINT("Present apparent power is :\n\rSt=%.1fVA Sa=%.1fVA Sb=%.1fVA Sc=%.1fVA\r\n",
                   (float)st/10, (float)sa/10, (float)sb/10, (float)sc/10);

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_FREQUENCY:
        {
            uint32_t freq;

            APP_METROLOGY_GetRMS(RMS_FREQ, &freq, 0);
            // Show received data on console
            SYS_CMD_PRINT("Present frequency is : \r\nFreq=%.2fHz\r\n", (float)freq/10);

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_ANGLE:
        {
            uint32_t aa, ab, ac;
            DRV_METROLOGY_RMS_SIGN signa, signb, signc;

            APP_METROLOGY_GetRMS(RMS_ANGLEA, &aa, &signa);
            APP_METROLOGY_GetRMS(RMS_ANGLEB, &ab, &signb);
            APP_METROLOGY_GetRMS(RMS_ANGLEC, &ac, &signc);
            // Show received data on console
            SYS_CMD_PRINT("Voltage and current angle is : \r\nAngle_A=%c%.3f Angle_B=%c%.3f Angle_C=%c%.3f\r\n",
                    sign[signa], (float)(aa & 0xFFFFF)/1000, sign[signb], (float)(ab & 0xFFFFF)/1000,
                    sign[signc], (float)(ac & 0xFFFFF)/1000);

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
            vTaskDelay(CONSOLE_TASK_DEFAULT_DELAY_MS_BETWEEN_STATES / portTICK_PERIOD_MS);
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
