/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_energy.c

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
#include "app_energy.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
/* Define a queue to signal the Datalog Tasks to store data */
extern QueueHandle_t appDatalogQueueID;
APP_DATALOG_QUEUE_DATA appEnergyDatalogQueueData;

/* Define a queue to signal the Energy Tasks to process new energy measurement */
QueueHandle_t appEnergyQueueID = NULL;

/* Define a semaphore to wait callbacks from DataLog application */
OSAL_SEM_DECLARE(appEnergySemID);

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_ENERGY_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_ENERGY_DATA app_energyData;

#define APP_ENERGY_MIN_RTC_BACKUP     15

#define BUILD_TIME_HOUR     ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0')
#define BUILD_TIME_MIN      ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0')
#define BUILD_TIME_SEC      ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0')


#define BUILD_DATE_YEAR     ((__DATE__[7] - '0') * 1000 + \
                             (__DATE__[8] - '0') * 100 + \
                             (__DATE__[9] - '0') * 10  + \
                             (__DATE__[10] - '0'))

#define BUILD_DATE_DAY      (((__DATE__[4] == 0x20)? 0:(__DATE__[4] - '0') * 10) + \
                             (__DATE__[5] - '0'))

const uint8_t app_energyDayTbl[12] = {0x31, 0x28, 0x31, 0x30, 0x31, 0x30, 0x31, 0x31, 0x30, 0x31, 0x30, 0x31};
const char app_energyMonthTbl[12][3] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const uint8_t app_energyDayWeekTbl[12] = {6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

const APP_ENERGY_TOU_TIME_ZONE gAppEnergyTOUDefault[APP_ENERGY_TOU_MAX_ZONES] =
{
    {TARIFF_2, _UINT32_(8), _UINT32_(30)},
    {TARIFF_3, _UINT32_(10), _UINT32_(30)},
    {TARIFF_1, _UINT32_(18), _UINT32_(30)},
    {TARIFF_4, _UINT32_(22), _UINT32_(00)},
    {TARIFF_INVALID, _UINT32_(0), _UINT32_(0)},
    {TARIFF_INVALID, _UINT32_(0), _UINT32_(0)},
    {TARIFF_INVALID, _UINT32_(0), _UINT32_(0)},
    {TARIFF_INVALID, _UINT32_(0), _UINT32_(0)}
};

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

uint32_t int_cnt = 0;
uint32_t int_cnt_timev = 0;
uint32_t int_cnt_calev = 0;

void RTC_callback(uint32_t int_cause, uintptr_t context)
{
    int_cnt++;
    if (int_cause & RTC_SR_TIMEV_Msk)
    {
        app_energyData.eventMinute = true;
        int_cnt_timev++;
    }
    else if (int_cause & RTC_SR_CALEV_Msk)
    {
        app_energyData.eventMonth = true;
        int_cnt_calev++;
    }
}

void _APP_ENERGY_GetDataLogCallback(APP_DATALOG_RESULT result)
{
    if (result == APP_DATALOG_RESULT_SUCCESS)
    {
        app_energyData.dataIsRdy = true;
    }

    // Post semaphore to wakeup task
    OSAL_SEM_Post(&appEnergySemID);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

static uint8_t _APP_ENERGY_getTOUIndex(struct tm * time)
{
    uint32_t value, slotValue;
    uint8_t index;
    uint8_t counterZones;

    counterZones = app_energyData.tou.usedZones;
    value = (time->tm_hour * 100) + time->tm_min;
    for (index = 0; index < counterZones; index++)
    {
        slotValue = (app_energyData.tou.timeZone[index].hour * 100) + app_energyData.tou.timeZone[index].minute;
        if (value < slotValue)
        {
            if (index == 0)
            {
                return counterZones - 1;
            }
            else
            {
                return index - 1;
            }
        }
    }

    return counterZones - 1;
}

static APP_ENERGY_TARIFF_TYPE _APP_ENERGY_getTariff(struct tm * time)
{
    uint8_t touIndex;

    touIndex = _APP_ENERGY_getTOUIndex(time);
    return app_energyData.tou.timeZone[touIndex].tariff;
}

static bool _APP_ENERGY_CheckRTCFromReset(void)
{
    uint32_t enabledInterrupts = RTC_REGS->RTC_IMR;
    
    if (enabledInterrupts & (RTC_INT_TIME | RTC_INT_CALENDAR))
    {
        return false;
    }
    else
    {
        return true;
    }
}

static bool _APP_ENERGY_InitializeRTC(bool dataValid)
{
    if (!dataValid)
    {
        char dateBuff[12];
        uint16_t y, d;
        uint8_t mon, uc_idx;
        uint8_t a, m;

        /* Set RTC time from COMPILER settings */
        app_energyData.time.tm_hour = BUILD_TIME_HOUR;
        app_energyData.time.tm_min = BUILD_TIME_MIN;
        app_energyData.time.tm_sec = BUILD_TIME_SEC;
        app_energyData.time.tm_year = BUILD_DATE_YEAR - 1900;
        app_energyData.time.tm_mday = BUILD_DATE_DAY;

        sprintf(dateBuff, "%s", __DATE__);
        app_energyData.time.tm_mon = 0;
        for (uc_idx = 0; uc_idx < 12; uc_idx++) {
            if (memcmp(&app_energyMonthTbl[uc_idx], dateBuff, 3) == 0) {
                app_energyData.time.tm_mon = uc_idx;
                break;
            }
        }

        /* Get day of the week */
        mon = app_energyData.time.tm_mon + 1;
        a = (14 - mon) / 12;
        y = BUILD_DATE_YEAR - ((14 - mon) / 12);
        m = mon + (12 * a) - 2;
        d = app_energyData.time.tm_mday + y + y/4 - y/100 + y/400 + ((31*m)/12);
        app_energyData.time.tm_wday = (d % 7) - 1;
    }

    /* Set RTC Time to current system time. */
    if (RTC_TimeSet(&app_energyData.time))
    {
        RTC_CallbackRegister(RTC_callback, 0);
        RTC_InterruptEnable(RTC_INT_TIME | RTC_INT_CALENDAR);
        return true;
    }

    return false;
}

static void _APP_ENERGY_InitializeTOU(bool dataValid)
{
    struct tm time;
    uint8_t counterZones = 0;
    uint8_t index;

    if (!dataValid)
    {
        /* Set TOU by default */
        memcpy(&app_energyData.tou.timeZone, &gAppEnergyTOUDefault, sizeof(app_energyData.tou.timeZone));
    }

    /* Count used zones */
    for (index = 0; index < APP_ENERGY_TOU_MAX_ZONES; index++)
    {
        if (app_energyData.tou.timeZone[index].tariff != TARIFF_INVALID)
        {
            counterZones++;
        }
    }
    app_energyData.tou.usedZones = counterZones;

    /* Update current Tariff Type */
    RTC_TimeGet(&time);
    app_energyData.currentTariff = _APP_ENERGY_getTariff(&time);
}

static bool _APP_ENERGY_UpdateDemand(uint32_t demand, struct tm * time)
{
    app_energyData.demand.window[time->tm_min] = demand;

    if ((time->tm_min % 15) == 0)
    {
        uint32_t demandMax = 0;
        uint8_t  index = 0;
        uint8_t winStartOffset = 0;
        bool update = false;

        if (time->tm_min == 0)
        {
            winStartOffset = 45;
        }
        else
        {
            winStartOffset = time->tm_min - 15;
        }

        /* Get Max Demand : averaged over the last 15 minutes */
        for (index = 0; index < 15; index++)
        {
            demandMax += app_energyData.demand.window[winStartOffset + index];
        }
        demandMax /= 9000; /* Units are 0.1W, so divided by 10; additionally, 15 minutes, so divided by 15 */

        /* Update Demand Max according TOU Zone */
        if (app_energyData.demand.maxDemand.tariff[app_energyData.currentTariff - 1].value < demandMax)
        {
            app_energyData.demand.maxDemand.tariff[app_energyData.currentTariff - 1].value = demandMax;
            app_energyData.demand.maxDemand.tariff[app_energyData.currentTariff - 1].month = time->tm_mon;
            app_energyData.demand.maxDemand.tariff[app_energyData.currentTariff - 1].day = time->tm_mday;
            app_energyData.demand.maxDemand.tariff[app_energyData.currentTariff - 1].hour = time->tm_hour;
            app_energyData.demand.maxDemand.tariff[app_energyData.currentTariff - 1].minute = time->tm_min;

            update = true;
        }

        /* Update Demand Max in total */
        if (app_energyData.demand.maxDemand.maxDemad.value < demandMax)
        {
            app_energyData.demand.maxDemand.maxDemad.value = demandMax;
            app_energyData.demand.maxDemand.maxDemad.month = time->tm_mon;
            app_energyData.demand.maxDemand.maxDemad.day = time->tm_mday;
            app_energyData.demand.maxDemand.maxDemad.hour = time->tm_hour;
            app_energyData.demand.maxDemand.maxDemad.minute = time->tm_min;

            update = true;
        }

        return update;
    }

    return false;
}

static bool _APP_ENERGY_CheckEnergyThreshold(APP_ENERGY_ACCUMULATORS * pAccumulatorsDiff)
{
    uint64_t totalEnergy = 0;
    uint64_t totalEnergyDiff = 0;
    uint8_t index;

    for (index = 0; index < TARIFF_NUM_TYPE; index++)
    {
        totalEnergy += app_energyData.energyAccumulator.tariff[index];
        totalEnergyDiff += pAccumulatorsDiff->tariff[index];
    }

    if ((totalEnergy - totalEnergyDiff) >= app_energyData.energyThreshold)
    {
        return true;
    }

    return false;
}

static void _APP_ENERGY_LoadRTCDataFromMemory(void)
{
    appEnergyDatalogQueueData.userId = APP_DATALOG_USER_RTC;
    appEnergyDatalogQueueData.operation = APP_DATALOG_READ;
    appEnergyDatalogQueueData.pData = (uint8_t *)&app_energyData.time;
    appEnergyDatalogQueueData.dataLen = sizeof(struct tm);
    appEnergyDatalogQueueData.endCallback = _APP_ENERGY_GetDataLogCallback;
    appEnergyDatalogQueueData.date.month = APP_DATALOG_INVALID_MONTH;
    appEnergyDatalogQueueData.date.year = APP_DATALOG_INVALID_YEAR;

    xQueueSend(appDatalogQueueID, &appEnergyDatalogQueueData, (TickType_t) 0);
}

static void _APP_ENERGY_StoreRTCDataInMemory(void)
{
    appEnergyDatalogQueueData.userId = APP_DATALOG_USER_RTC;
    appEnergyDatalogQueueData.operation = APP_DATALOG_WRITE;
    appEnergyDatalogQueueData.pData = (uint8_t *)&app_energyData.time;
    appEnergyDatalogQueueData.dataLen = sizeof(struct tm);
    appEnergyDatalogQueueData.endCallback = NULL;
    appEnergyDatalogQueueData.date.month = APP_DATALOG_INVALID_MONTH;
    appEnergyDatalogQueueData.date.year = APP_DATALOG_INVALID_YEAR;

    xQueueSend(appDatalogQueueID, &appEnergyDatalogQueueData, (TickType_t) 0);
}

static void _APP_ENERGY_LoadTOUDataFromMemory(void)
{
    appEnergyDatalogQueueData.userId = APP_DATALOG_USER_TOU;
    appEnergyDatalogQueueData.operation = APP_DATALOG_READ;
    appEnergyDatalogQueueData.pData = (uint8_t *)&app_energyData.tou;
    appEnergyDatalogQueueData.dataLen = sizeof(APP_ENERGY_TOU);
    appEnergyDatalogQueueData.endCallback = _APP_ENERGY_GetDataLogCallback;
    appEnergyDatalogQueueData.date.month = APP_DATALOG_INVALID_MONTH;
    appEnergyDatalogQueueData.date.year = APP_DATALOG_INVALID_YEAR;

    xQueueSend(appDatalogQueueID, &appEnergyDatalogQueueData, (TickType_t) 0);
}

static void _APP_ENERGY_StoreTOUDataInMemory(void)
{
    appEnergyDatalogQueueData.userId = APP_DATALOG_USER_TOU;
    appEnergyDatalogQueueData.operation = APP_DATALOG_WRITE;
    appEnergyDatalogQueueData.pData = (uint8_t *)&app_energyData.tou;
    appEnergyDatalogQueueData.dataLen = sizeof(APP_ENERGY_TOU);
    appEnergyDatalogQueueData.endCallback = NULL;
    appEnergyDatalogQueueData.date.month = APP_DATALOG_INVALID_MONTH;
    appEnergyDatalogQueueData.date.year = APP_DATALOG_INVALID_YEAR;

    xQueueSend(appDatalogQueueID, &appEnergyDatalogQueueData, (TickType_t) 0);
}

static void _APP_ENERGY_LoadEnergyDataFromMemory(struct tm * time, void *pData)
{
    appEnergyDatalogQueueData.userId = APP_DATALOG_USER_ENERGY;
    appEnergyDatalogQueueData.operation = APP_DATALOG_READ;
    appEnergyDatalogQueueData.pData = (uint8_t *)pData;
    appEnergyDatalogQueueData.dataLen = sizeof(APP_ENERGY_ACCUMULATORS);
    appEnergyDatalogQueueData.date.month = time->tm_mon + 1;
    appEnergyDatalogQueueData.date.year = time->tm_year - 100;
    appEnergyDatalogQueueData.endCallback = _APP_ENERGY_GetDataLogCallback;

    xQueueSend(appDatalogQueueID, &appEnergyDatalogQueueData, (TickType_t) 0);
}

static void _APP_ENERGY_StoreEnergyDataInMemory(struct tm * time, void *pData)
{
    appEnergyDatalogQueueData.userId = APP_DATALOG_USER_ENERGY;
    appEnergyDatalogQueueData.operation = APP_DATALOG_WRITE;
    appEnergyDatalogQueueData.pData = (uint8_t *)pData;
    appEnergyDatalogQueueData.dataLen = sizeof(APP_ENERGY_ACCUMULATORS);
    appEnergyDatalogQueueData.date.month = time->tm_mon + 1;
    appEnergyDatalogQueueData.date.year = time->tm_year - 100;
    appEnergyDatalogQueueData.endCallback = NULL;

    xQueueSend(appDatalogQueueID, &appEnergyDatalogQueueData, (TickType_t) 0);
}

static void _APP_ENERGY_LoadDemandDataFromMemory(struct tm * time, void *pData)
{
    appEnergyDatalogQueueData.userId = APP_DATALOG_USER_DEMAND;
    appEnergyDatalogQueueData.operation = APP_DATALOG_READ;
    appEnergyDatalogQueueData.pData = (uint8_t *)pData;
    appEnergyDatalogQueueData.dataLen = sizeof(APP_ENERGY_MAX_DEMAND);
    appEnergyDatalogQueueData.date.month = time->tm_mon + 1;
    appEnergyDatalogQueueData.date.year = time->tm_year - 100;
    appEnergyDatalogQueueData.endCallback = _APP_ENERGY_GetDataLogCallback;

    xQueueSend(appDatalogQueueID, &appEnergyDatalogQueueData, (TickType_t) 0);
}

static void _APP_ENERGY_StoreDemandDataInMemory(struct tm * time, void *pData)
{
    appEnergyDatalogQueueData.userId = APP_DATALOG_USER_DEMAND;
    appEnergyDatalogQueueData.operation = APP_DATALOG_WRITE;
    appEnergyDatalogQueueData.pData = (uint8_t *)pData;
    appEnergyDatalogQueueData.dataLen = sizeof(APP_ENERGY_MAX_DEMAND);
    appEnergyDatalogQueueData.date.month = time->tm_mon + 1;
    appEnergyDatalogQueueData.date.year = time->tm_year - 100;
    appEnergyDatalogQueueData.endCallback = NULL;

    xQueueSend(appDatalogQueueID, &appEnergyDatalogQueueData, (TickType_t) 0);
}

static void _APP_ENERGY_SupplyMonitorCallback(uint32_t supc_status, uintptr_t context)
{
    if (supc_status & SUPC_IMR_VDD3V3SMEV_Msk)
    {
        // Go to Low Power mode
        APP_METROLOGY_SetLowPowerMode();
    }
}

static void _APP_ENERGY_CheckTamperDetection(void)
{
    /* Check the Tamper Event Counter value */
    if (RTC_REGS->RTC_SUB0[0].RTC_FSTR & RTC_FSTR_TEVCNT_Msk)
    {
        uint32_t reg;
        struct tm tamperTime;
        
        RTC_LastTimeStampGet(&tamperTime, 0);
        SYS_CMD_PRINT("Detected TAMPER event: [%02u/%02u/%04u %02u:%02u:%02u]\r\n",
				  tamperTime.tm_mon + 1, tamperTime.tm_mday, tamperTime.tm_year + 1900,
				  tamperTime.tm_hour, tamperTime.tm_min, tamperTime.tm_sec);
    
        SYS_CMD_MESSAGE("\r\nReading GPBR registers:\r\n");
        reg = SUPC_GPBRRead(GPBR_REGS_0);
        SYS_CMD_PRINT("\tGPBR_REGS_0  = 0x%08x\r\n", reg);
        reg = SUPC_GPBRRead(GPBR_REGS_23);
        SYS_CMD_PRINT("\tGPBR_REGS_23 = 0x%08x\r\n\r\n", reg );
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_ENERGY_Initialize (void)

  Remarks:
    See prototype in app_energy.h.
 */

void APP_ENERGY_Initialize (void)
{
    /* Place the App state machine in its initial state. */
    app_energyData.state = APP_ENERGY_STATE_WAITING_DATALOG;

     /* Initialize Energy callbacks */
    app_energyData.maxDemandCallback = NULL;
    app_energyData.monthEnergyCallback = NULL;
    
    /* Set callback for the supply monitor */
    SUPC_CallbackRegister(_APP_ENERGY_SupplyMonitorCallback, 0);

    /* Clear RTC TIME & CALENDAR events */
    app_energyData.eventMinute = false;
    app_energyData.eventMonth = false;
    
    /* Set time to store RTC time in external memory as backup */
    app_energyData.minRtcBackup = APP_ENERGY_MIN_RTC_BACKUP;

    /* Clear Energy accumulators */
    memset(&app_energyData.energyAccumulator.tariff, 0, sizeof(APP_ENERGY_ACCUMULATORS));

    /* Clear Demand data */
    memset(&app_energyData.demand, 0, sizeof(APP_ENERGY_DEMAND));

    /* Set Energy TOU Threshold */
    app_energyData.energyThreshold = APP_ENERGY_TOU_THRESHOLD;

    /* Create a queue capable of containing 5 queue data elements. */
    appEnergyQueueID = xQueueCreate(5, sizeof(APP_ENERGY_QUEUE_DATA));

    if (appEnergyQueueID == NULL)
    {
        // Queue was not created and must not be used.
        app_energyData.state = APP_ENERGY_STATE_ERROR;
        return;
    }

    /* Create the Switches Semaphore. */
    if (OSAL_SEM_Create(&appEnergySemID, OSAL_SEM_TYPE_BINARY, 0, 0) == OSAL_RESULT_FALSE)
    {
        /* Handle error condition. Not sufficient memory to create semaphore */
    }
}


/******************************************************************************
  Function:
    void APP_ENERGY_Tasks (void)

  Remarks:
    See prototype in app_energy.h.
 */

void APP_ENERGY_Tasks (void)
{
    /* Check the application's current state. */
    switch (app_energyData.state)
    {
        case APP_ENERGY_STATE_WAITING_DATALOG:
        {
            if (APP_DATALOG_GetStatus() == APP_DATALOG_STATE_READY)
            {
                /* Write value in GPBR as initialization flag example */
                /* In case of TAMPER detection, this value will be erased */
                SUPC_GPBRWrite(GPBR_REGS_0, 0xA55A);
                SUPC_GPBRWrite(GPBR_REGS_23, 0xB66B);
                SYS_CMD_MESSAGE("\r\nSetting GPBR registers to be checked in tamper detection:\r\n");
                SYS_CMD_PRINT("\tGPBR_REGS_0  = 0x%08x\r\n", SUPC_GPBRRead(GPBR_REGS_0));
                SYS_CMD_PRINT("\tGPBR_REGS_23 = 0x%08x\r\n", SUPC_GPBRRead(GPBR_REGS_23));
                
                app_energyData.state = APP_ENERGY_STATE_INIT_RTC;
            }

            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_ENERGY_STATE_INIT_RTC:
        {
            /* Reset flag to request data to datalog app */
            app_energyData.dataIsRdy = false;
            
            if (_APP_ENERGY_CheckRTCFromReset())
            {
                /* Check if there are RTC data in memory */
                if (APP_DATALOG_FileExists(APP_DATALOG_USER_RTC, NULL))
                {
                    /* RTC data exists */
                    _APP_ENERGY_LoadRTCDataFromMemory();
                    /* Wait for the semaphore to load data from memory */
                    OSAL_SEM_Pend(&appEnergySemID, OSAL_WAIT_FOREVER);
                }

                if (_APP_ENERGY_InitializeRTC(app_energyData.dataIsRdy))
                {
                    if (app_energyData.dataIsRdy == false)
                    {
                        /* There is no valid data in memory. Create RTC Data in memory. */
                        _APP_ENERGY_StoreRTCDataInMemory();
                    }
                    app_energyData.state = APP_ENERGY_STATE_INIT_TOU;
                }
                else
                {
                    app_energyData.state = APP_ENERGY_STATE_ERROR;
                }
            }
            else
            {
                RTC_TimeGet(&app_energyData.time);
                if (_APP_ENERGY_InitializeRTC(true))
                {
                    app_energyData.state = APP_ENERGY_STATE_INIT_TOU;
                }
                else
                {
                    app_energyData.state = APP_ENERGY_STATE_ERROR;
                }
            }

            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_ENERGY_STATE_INIT_TOU:
        {
            /* Reset flag to request data to datalog app */
            app_energyData.dataIsRdy = false;
            
            /* Check if there are TOU data in memory */
            if (APP_DATALOG_FileExists(APP_DATALOG_USER_TOU, NULL))
            {
                /* TOU data exists */
                _APP_ENERGY_LoadTOUDataFromMemory();
                /* Wait for the semaphore to load data from memory */
                OSAL_SEM_Pend(&appEnergySemID, OSAL_WAIT_FOREVER);
            }

            _APP_ENERGY_InitializeTOU(app_energyData.dataIsRdy);

            if (app_energyData.dataIsRdy == false)
            {
                /* There is no valid data in memory. Create TOU Data in memory. */
                _APP_ENERGY_StoreTOUDataInMemory();
            }

            app_energyData.state = APP_ENERGY_STATE_INIT_ENERGY;

            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_ENERGY_STATE_INIT_ENERGY:
        {
            APP_DATALOG_DATE date;
            
            /* Reset flag to request data to datalog app */
            app_energyData.dataIsRdy = false;
            
            date.month = app_energyData.time.tm_mon + 1;
            date.year = app_energyData.time.tm_year - 100;
            /* Check if there are ENERGY data in memory */
            if (APP_DATALOG_FileExists(APP_DATALOG_USER_ENERGY, &date))
            {
                /* ENERGY data exists */
                _APP_ENERGY_LoadEnergyDataFromMemory(&app_energyData.time, &app_energyData.energyAccumulator);
                /* Wait for the semaphore to load data from memory */
                OSAL_SEM_Pend(&appEnergySemID, OSAL_WAIT_FOREVER);
            }
            else
            {
                /* There is no valid data in memory. Create Energy Data in memory. */
                _APP_ENERGY_StoreEnergyDataInMemory(&app_energyData.time, &app_energyData.energyAccumulator);
            }

            app_energyData.state = APP_ENERGY_STATE_INIT_DEMAND;

            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_ENERGY_STATE_INIT_DEMAND:
        {
            APP_DATALOG_DATE date;
            
            /* Reset flag to request data to datalog app */
            app_energyData.dataIsRdy = false;
            
            date.month = app_energyData.time.tm_mon + 1;
            date.year = app_energyData.time.tm_year - 100;
            /* Check if there are DEMAND data in memory */
            if (APP_DATALOG_FileExists(APP_DATALOG_USER_DEMAND, &date))
            {
                /* DEMAND data exists */
                _APP_ENERGY_LoadDemandDataFromMemory(&app_energyData.time, &app_energyData.demand.maxDemand);
                /* Wait for the semaphore to load data from memory */
                OSAL_SEM_Pend(&appEnergySemID, OSAL_WAIT_FOREVER);
            }
            else
            {
                /* There is no valid data in memory. Create Demand Data in memory. */
                _APP_ENERGY_StoreDemandDataInMemory(&app_energyData.time, &app_energyData.demand.maxDemand);
            }

            /* Update RTC */
            RTC_TimeGet(&app_energyData.time);

            app_energyData.state = APP_ENERGY_STATE_RUNNING;

            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_ENERGY_STATE_RUNNING:
        {
            if (xQueueReceive(appEnergyQueueID, &app_energyData.newQueuedData, 250 / portTICK_PERIOD_MS) == pdPASS)
            {
                /* Update Energy Accumulator */
                app_energyData.energyAccumulator.tariff[app_energyData.currentTariff - 1] += app_energyData.newQueuedData.energy;

                /* Update Demand Accumulator */
                app_energyData.demandAccumulator += app_energyData.newQueuedData.Pt;

                /* Check TIME Event (minute) */
                /* Don't update the RTC because we need to handle the energy accumulated in the previous minute */
                if (app_energyData.eventMinute)
                {
                    APP_ENERGY_ACCUMULATORS dataLogEnergy = {0};

                    /* Clear TIME Event flag */
                    app_energyData.eventMinute = false;

                    /* Update current Tariff type */
                    app_energyData.currentTariff = _APP_ENERGY_getTariff(&app_energyData.time);

                    /* Update demand values */
                    if (_APP_ENERGY_UpdateDemand(app_energyData.demandAccumulator, &app_energyData.time))
                    {
                        /* Update maximum demand each 15 minutes */
                        _APP_ENERGY_StoreDemandDataInMemory(&app_energyData.time, &app_energyData.demand.maxDemand);
                    }
                    app_energyData.demandAccumulator = 0;

                    /* Update DATALOG if accumulators has increased more than APP_ENERGY_TOU_THRESHOLD */
                    if (_APP_ENERGY_CheckEnergyThreshold(&dataLogEnergy))
                    {
                        _APP_ENERGY_StoreEnergyDataInMemory(&app_energyData.time, &app_energyData.energyAccumulator);
                    }
                    
                    /* Check Counter to store a RTC backup */
                    if (!app_energyData.minRtcBackup--)
                    {
                        app_energyData.minRtcBackup = APP_ENERGY_MIN_RTC_BACKUP;

                        /* Read RTC */
                        RTC_TimeGet(&app_energyData.time);
                        /* Update RTC data in memory */
                        _APP_ENERGY_StoreRTCDataInMemory();
                    }
                }

                /* Check CALENDAR Event (month) */
                if (app_energyData.eventMonth)
                {
                    app_energyData.eventMonth = false;

                    /* Clear Energy Accumulators */
                    memset(app_energyData.energyAccumulator.tariff, 0, sizeof(APP_ENERGY_ACCUMULATORS));

                    /* Clear MaxDemand data */
                    memset(&app_energyData.demand.maxDemand, 0, sizeof(APP_ENERGY_MAX_DEMAND));
                }

                /* Check TAMPER detection */
                _APP_ENERGY_CheckTamperDetection();
            }

            break;
        }

        case APP_ENERGY_STATE_GET_MAX_DEMAND:
        {
            APP_DATALOG_DATE date;
            
            /* Reset flag to request data to datalog app */
            app_energyData.dataIsRdy = false;
            
            date.month = app_energyData.timeResponse.tm_mon + 1;
            date.year = app_energyData.timeResponse.tm_year - 100;
            /* Check if there are ENERGY data in memory */
            if (APP_DATALOG_FileExists(APP_DATALOG_USER_DEMAND, &date))
            {
                /* ENERGY data exists */
                _APP_ENERGY_LoadDemandDataFromMemory(&app_energyData.timeResponse, app_energyData.pMaxDemandResponse);
                /* Wait for the semaphore to load data from memory */
                OSAL_SEM_Pend(&appEnergySemID, OSAL_WAIT_FOREVER);
            }
            
            app_energyData.maxDemandCallback(&app_energyData.timeResponse, app_energyData.dataIsRdy);
            
            app_energyData.state = APP_ENERGY_STATE_RUNNING;

            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_ENERGY_STATE_GET_MONTH_ENERGY:
        {
            APP_DATALOG_DATE date;
            
            /* Reset flag to request data to datalog app */
            app_energyData.dataIsRdy = false;
            
            date.month = app_energyData.timeResponse.tm_mon + 1;
            date.year = app_energyData.timeResponse.tm_year - 100;
            /* Check if there are ENERGY data in memory */
            if (APP_DATALOG_FileExists(APP_DATALOG_USER_ENERGY, &date))
            {
                /* ENERGY data exists */
                _APP_ENERGY_LoadEnergyDataFromMemory(&app_energyData.timeResponse, app_energyData.pMonthEnergyResponse);
                /* Wait for the semaphore to load data from memory */
                OSAL_SEM_Pend(&appEnergySemID, OSAL_WAIT_FOREVER);
            }
            
            app_energyData.monthEnergyCallback(&app_energyData.timeResponse, app_energyData.dataIsRdy);

            app_energyData.state = APP_ENERGY_STATE_RUNNING;

            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        /* The default state should never be executed. */
        case APP_ENERGY_STATE_ERROR:
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

APP_ENERGY_TOU_TIME_ZONE * APP_ENERGY_GetTOUTimeZone(void)
{
    return app_energyData.tou.timeZone;
}

void APP_ENERGY_SetTOUTimeZone(APP_ENERGY_TOU_TIME_ZONE *timeZone)
{
    _APP_ENERGY_InitializeTOU(timeZone);
}

void APP_ENERGY_SetMonthEnergyCallback(APP_ENERGY_MONTH_CALLBACK callback,
        APP_ENERGY_ACCUMULATORS * pMonthEnergyResponse)
{
    app_energyData.monthEnergyCallback = callback;
    app_energyData.pMonthEnergyResponse = pMonthEnergyResponse;
}

bool APP_ENERGY_GetMonthEnergy(struct tm * time)
{
    if ((app_energyData.monthEnergyCallback) && (app_energyData.state == APP_ENERGY_STATE_RUNNING))
    {
        app_energyData.timeResponse = *time;
        app_energyData.state = APP_ENERGY_STATE_GET_MONTH_ENERGY;
        return true;
    }

    return false;
}

void APP_ENERGY_GetCurrentEnergy(APP_ENERGY_ACCUMULATORS * pEnergy)
{
    if (pEnergy)
    {
        *pEnergy = app_energyData.energyAccumulator;
    }
}

void APP_ENERGY_ClearEnergy(void)
{
    /* Erase all the energy records stored in non volatile memory */
    APP_DATALOG_ClearData(APP_DATALOG_USER_ENERGY);
}

void APP_ENERGY_SetMaxDemandCallback(APP_ENERGY_MAXDEMAND_CALLBACK callback,
        APP_ENERGY_MAX_DEMAND * pMaxDemandResponse)
{
    app_energyData.maxDemandCallback = callback;
    app_energyData.pMaxDemandResponse = pMaxDemandResponse;
}

bool APP_ENERGY_GetMonthMaxDemand(struct tm * time)
{
    if ((app_energyData.maxDemandCallback) && (app_energyData.state == APP_ENERGY_STATE_RUNNING))
    {
        app_energyData.timeResponse = *time;
        app_energyData.state = APP_ENERGY_STATE_GET_MAX_DEMAND;
        return true;
    }

    return false;
}

void APP_ENERGY_GetCurrentMaxDemand(APP_ENERGY_MAX_DEMAND * pMaxDemand)
{
    if (pMaxDemand)
    {
        *pMaxDemand = app_energyData.demand.maxDemand;
    }
}

void APP_ENERGY_ClearMaxDemand(void)
{
    /* Erase all the demand records stored in non volatile memory */
    APP_DATALOG_ClearData(APP_DATALOG_USER_DEMAND);
}


/*******************************************************************************
 End of File
 */
