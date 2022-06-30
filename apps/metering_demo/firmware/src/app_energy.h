/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_energy.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_ENERGY_Initialize" and "APP_ENERGY_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_ENERGY_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_ENERGY_H
#define _APP_ENERGY_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
    
typedef struct {
	uint32_t energy;
	uint32_t Pt;
} APP_ENERGY_QUEUE_DATA;

typedef enum {
	TARIFF_1 = 1,
	TARIFF_2,
	TARIFF_3,
	TARIFF_4,
    TARIFF_MAX_NUM = TARIFF_4,
	TARIFF_INVALID = 0xFF
} APP_ENERGY_TARIFF_TYPE;

typedef struct {
	APP_ENERGY_TARIFF_TYPE tariff;
	uint8_t hour;
	uint8_t minute;
} APP_ENERGY_TOU_TIME_ZONE;

#define APP_ENERGY_TOU_MAX_ZONES             8

typedef struct {
	APP_ENERGY_TOU_TIME_ZONE timeZone[APP_ENERGY_TOU_MAX_ZONES];
    uint32_t usedZones;
} APP_ENERGY_TOU;

typedef struct {
    uint32_t value;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
	uint8_t minute;
} APP_ENERGY_DEMAND_DATA;

typedef struct {
    APP_ENERGY_DEMAND_DATA maxDemad;
    APP_ENERGY_DEMAND_DATA tariff[TARIFF_MAX_NUM];
} APP_ENERGY_MAX_DEMAND;

typedef struct {
    APP_ENERGY_MAX_DEMAND maxDemand;
    uint32_t window[60];
} APP_ENERGY_DEMAND;

typedef struct {
    uint64_t tariff[TARIFF_MAX_NUM];
} APP_ENERGY_ACCUMULATORS;

/* Energy threshold to update energy register stored in external memory */
#define APP_ENERGY_TOU_THRESHOLD  10000 // 0.01kWh (Units: 10^-4 Wh)

typedef void ( * APP_ENERGY_MAXDEMAND_CALLBACK ) ( uint8_t month );
typedef void ( * APP_ENERGY_MONTH_CALLBACK ) ( uint8_t month );
// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    /* Application's state machine's initial state. */
    APP_ENERGY_STATE_INIT_RTC = 0,
    APP_ENERGY_STATE_INIT_TOU,
    APP_ENERGY_STATE_INIT_DEMAND,
    APP_ENERGY_STATE_INIT_EVENTS,
    APP_ENERGY_STATE_RUNNING,
    APP_ENERGY_STATE_GET_MAX_DEMAND,
    APP_ENERGY_STATE_GET_MONTH_ENERGY,
    APP_ENERGY_STATE_ERROR

} APP_ENERGY_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_ENERGY_STATES state;
    
    APP_ENERGY_QUEUE_DATA newQueuedData;
    
    APP_ENERGY_TARIFF_TYPE currentTariff;

    uint32_t energyThreshold;
    
	APP_ENERGY_TOU tou;
    
    APP_ENERGY_DEMAND demand;
    
    APP_ENERGY_ACCUMULATORS energyAccumulator;
    
    uint32_t demandAccumulator;
    
    bool eventMinute;
    
    bool eventMonth;
    
    APP_ENERGY_MAXDEMAND_CALLBACK maxDemandCallback;
    
    APP_ENERGY_MAX_DEMAND * pMaxDemandResponse;
    
    APP_ENERGY_MONTH_CALLBACK monthEnergyCallback;
    
    APP_ENERGY_ACCUMULATORS * pMonthEnergyResponse;
    
    uint8_t maxDemandMonthResponse;
    
    uint8_t monthEnergyResponse;

} APP_ENERGY_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_ENERGY_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_ENERGY_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_ENERGY_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_ENERGY_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_ENERGY_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_ENERGY_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_ENERGY_Tasks( void );


APP_ENERGY_TOU_TIME_ZONE * APP_ENERGY_GetTOUTimeZone( void );
void APP_ENERGY_SetTOUTimeZone( APP_ENERGY_TOU_TIME_ZONE *timeZone );
void APP_ENERGY_SetMonthEnergyCallback(APP_ENERGY_MONTH_CALLBACK callback, 
        APP_ENERGY_ACCUMULATORS * pMonthEnergyResponse);
bool APP_ENERGY_GetMonthEnergy( uint8_t month );
void APP_ENERGY_ClearEnergy( void );
void APP_ENERGY_SetMaxDemandCallback(APP_ENERGY_MAXDEMAND_CALLBACK callback, 
        APP_ENERGY_MAX_DEMAND * pMaxDemandResponse);
bool APP_ENERGY_GetMonthMaxDemand( uint8_t month );
void APP_ENERGY_ClearMaxDemand( void );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_ENERGY_H */

/*******************************************************************************
 End of File
 */
