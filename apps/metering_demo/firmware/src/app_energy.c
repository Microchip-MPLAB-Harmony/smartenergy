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

/* Define a queue to signal the Energy Tasks to process new energy measurement */
QueueHandle_t CACHE_ALIGN app_energyQueue = NULL;

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

const APP_ENERGY_TOU gAppEnergyTOUDefault = 
{
    {{TOU_RATE_2, _UINT32_(8), _UINT32_(30)},
	{TOU_RATE_3, _UINT32_(10), _UINT32_(30)},
	{TOU_RATE_1, _UINT32_(18), _UINT32_(30)},
	{TOU_RATE_4, _UINT32_(22), _UINT32_(00)},
	{INVALID_RATE, _UINT32_(0), _UINT32_(0)},
	{INVALID_RATE, _UINT32_(0), _UINT32_(0)},
	{INVALID_RATE, _UINT32_(0), _UINT32_(0)},
	{INVALID_RATE, _UINT32_(0), _UINT32_(0)}}
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

static bool _APP_ENERGY_InitializeRTC(void)
{
    struct tm sysTime;
    
//    if (APP_DATALOG_GetSystemTime(&sysTime) == false)
//    {
        char dateBuff[12];
        uint16_t y, d;
        uint8_t mon, uc_idx;
        uint8_t a, m;

        /* Set RTC time from COMPILER settings */
        sysTime.tm_hour = BUILD_TIME_HOUR;
        sysTime.tm_min = BUILD_TIME_MIN;
        sysTime.tm_sec = BUILD_TIME_SEC;
        sysTime.tm_year = BUILD_DATE_YEAR - 1900;
        sysTime.tm_mday = BUILD_DATE_DAY;

        sprintf(dateBuff, "%s", __DATE__);
        sysTime.tm_mon = 0;
        for (uc_idx = 0; uc_idx < 12; uc_idx++) {
            if (memcmp(&app_energyMonthTbl[uc_idx], dateBuff, 3) == 0) {
                sysTime.tm_mon = uc_idx;
                break;
            }
        }

        /* Get day of the week */
        mon = sysTime.tm_mon + 1;
        a = (14 - mon) / 12;
        y = BUILD_DATE_YEAR - ((14 - mon) / 12);
        m = mon + (12 * a) - 2;
        d = sysTime.tm_mday + y + y/4 - y/100 + y/400 + ((31*m)/12);
        sysTime.tm_wday = (d % 7) - 1;
//    }
    
    /* Set RTC Time to current system time. */
    if (RTC_TimeSet(&sysTime))
    {
        RTC_CallbackRegister(RTC_callback, 0);
        RTC_InterruptEnable(RTC_INT_TIME | RTC_INT_CALENDAR);
        return true;
    }
    
    return false;
}

static bool _APP_ENERGY_InitializeTOU(void)
{
    struct tm time;
    uint8_t counterZones = 0;
    uint8_t index;
    
//    if (APP_DATALOG_GetEnergyTOU(&app_energyData.tou) == false)
//    {
        /* Set TOU defined by default */
        memcpy(&app_energyData.tou, &gAppEnergyTOUDefault, sizeof(APP_ENERGY_TOU));
//        /* Store TOU configuration in Data Log */
//        APP_DATALOG_SetEnergyTOU(&app_energyData.tou);
//    }
        
    /* Count used zones */
    for (index = 0; index < APP_ENERGY_TOU_MAX_ZONES; index++)
    {
        if (app_energyData.tou.timeZone[index].rate != INVALID_RATE)
        {
            counterZones++;
        }
    }    
    app_energyData.tou.usedZones = counterZones;
        
    /* Update current TOU zone */
    RTC_TimeGet(&time);
    app_energyData.tou.currentZone = _APP_ENERGY_getTOUIndex(&time);
    
    return true;
       
}

static bool _APP_ENERGY_InitializeDemand(void)
{
    struct tm sysTime;
    
    RTC_TimeGet(&sysTime);
    
//    APP_DATALOG_GetEnergyDemandMax(&app_energyData.demand.demandMax, &sysTime);
    
    return true;
}

static bool _APP_ENERGY_UpdateDemand(uint32_t demand, struct tm time)
{
    app_energyData.demand.window[time.tm_min] = demand;

    if ((time.tm_min % 15) == 0)
    {
        uint32_t demandMax = 0;
        uint8_t  index = 0;
        uint8_t winStartOffset = 0;
        
        if (time.tm_min == 0)
        {
            winStartOffset = 45;
        }
        else
        {
            winStartOffset = time.tm_min - 15;
        }

        /* Get Max Demand : averaged over the last 15 minutes */
        for (index = 0; index < 15; index++)
        {
            demandMax += app_energyData.demand.window[winStartOffset + index];
        }
        demandMax /= 9000; /* Units are 0.1W, so divided by 10; additionally, 15 minutes, so divided by 15 */
        
        /* Update Demand Max according TOU Zone */
        index = app_energyData.tou.currentZone;
        if (app_energyData.demand.demandMax[index].value < demandMax)
        {
            app_energyData.demand.demandMax[index].value = demandMax;
            app_energyData.demand.demandMax[index].hour = time.tm_hour;
            app_energyData.demand.demandMax[index].minute = time.tm_min;
        }
        
        return true;
    }
    
    return false;
}

static bool _APP_ENERGY_CheckEnergyThreshold( struct tm * time )
{
    uint64_t totalEnergy = 0;
    uint64_t totalEnergyDataLog = 0;
    uint64_t dataLogEnergy[APP_ENERGY_TOU_MAX_ZONES] = {0};
    uint8_t index;
    
//    if (APP_DATALOG_GetEnergyAccumulators(&dataLogEnergy, time) == false)
//    {
        return false;        
//    }
    
    for (index = 0; index < app_energyData.tou.usedZones; index++)
    {
        totalEnergy += app_energyData.energyAccumulator[index];
        totalEnergyDataLog += dataLogEnergy[index];
    }    
    
    if ((totalEnergy - totalEnergyDataLog) >= app_energyData.energyThreshold)
    {
        return true;
    }
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_ENERGY_Initialize ( void )

  Remarks:
    See prototype in app_energy.h.
 */

void APP_ENERGY_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_energyData.state = APP_ENERGY_STATE_INIT_RTC;
    
    /* Clear RTC TIME & CALENDAR events */
    app_energyData.eventMinute = false;
    app_energyData.eventMonth = false;
    
    /* Clear Energy accumulators */
    memset(&app_energyData.energyAccumulator, 0, sizeof(app_energyData.energyAccumulator));
    
    /* Clear Demand data */
    memset(&app_energyData.demand, 0, sizeof(app_energyData.demand));
    
    /* Set Energy TOU Threshold */
    app_energyData.energyThreshold = APP_ENERGY_TOU_THRESHOLD;

    /* Create a queue capable of containing 5 unsigned long values. */
    app_energyQueue = xQueueCreate( 5, sizeof( APP_ENERGY_QUEUE_DATA ) );

    if( app_energyQueue == NULL )
    {
        // Queue was not created and must not be used.
        app_energyData.state = APP_ENERGY_STATE_ERROR;
        return;
    }
}


/******************************************************************************
  Function:
    void APP_ENERGY_Tasks ( void )

  Remarks:
    See prototype in app_energy.h.
 */

void APP_ENERGY_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( app_energyData.state )
    {
        /* Application's initial state. */
        case APP_ENERGY_STATE_INIT_RTC:
        {
//            if (APP_DATALOG_GetStatus() == ADD_DATALOG_READY)
//            {
                if (_APP_ENERGY_InitializeRTC())
                {
                    app_energyData.state = APP_ENERGY_STATE_INIT_TOU;
                }
//            }
                
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_ENERGY_STATE_INIT_TOU:
        {
//            if (APP_DATALOG_GetStatus() == ADD_DATALOG_READY)
//            {
                if (_APP_ENERGY_InitializeTOU())
                {
                    // TBD: Restore Energy Accumulators ????????????????????????
                    app_energyData.state = APP_ENERGY_STATE_INIT_DEMAND;
                }
                else
                {
                    app_energyData.state = APP_ENERGY_STATE_ERROR;
                }
//            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_ENERGY_STATE_INIT_DEMAND:
        {
//            if (APP_DATALOG_GetStatus() == ADD_DATALOG_READY)
//            {
                if (_APP_ENERGY_InitializeDemand())
                {
                    app_energyData.state = APP_ENERGY_STATE_RUNNING;
                }
                else
                {
                    app_energyData.state = APP_ENERGY_STATE_ERROR;
                }
//            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_ENERGY_STATE_RUNNING:
        {
            if (xQueueReceive(app_energyQueue, &app_energyData.newQueuedData, portMAX_DELAY))
            {
                /* Check TIME Event (minute) */
                if (app_energyData.eventMinute)
                {
                    struct tm sysTime;

                    RTC_TimeGet(&sysTime);
                    
                    /* Clear TIME Event flag */
                    app_energyData.eventMinute = false;
                
                    /* Update current TOU zone */
                    app_energyData.tou.currentZone = _APP_ENERGY_getTOUIndex(&sysTime);
                    
                    /* Update demand values */
                    if (_APP_ENERGY_UpdateDemand(app_energyData.demandAccumulator, sysTime))
                    {
//                        APP_DATALOG_SetDemandMax(&app_energyData.demand.demandMax, &sysTime);
                    }
                    app_energyData.demandAccumulator = 0;
                    
                    /* Update DATALOG if accumulators has increased more than APP_ENERGY_TOU_THRESHOLD */
                    if (_APP_ENERGY_CheckEnergyThreshold(&sysTime))
                    {
//                        APP_DATALOG_SetEnergyAccumulators(&app_energyData.energyAccumulator, &sysTime);
                    }
                }
                
                /* Update Energy Accumulator */
                app_energyData.energyAccumulator[app_energyData.tou.currentZone] += app_energyData.newQueuedData.energy;
                
                /* Update Demand Accumulator */
                app_energyData.demandAccumulator += app_energyData.newQueuedData.Pt;
                
                /* Check CALENDAR Event (month) */
                if (app_energyData.eventMonth)
                {
                    app_energyData.eventMonth = false;
                }
                
            }

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


/*******************************************************************************
 End of File
 */
