/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATE" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_H
#define _APP_H

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
#include "definitions.h"

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
#define APP_SERIAL_DATA_BUFFER_SIZE   512
#define APP_PLC_DATA_BUFFER_SIZE      512
#define APP_PLC_PIB_BUFFER_SIZE       256
    
#define LED_BLINK_RATE_MS             500
#define LED_BLINK_PLC_MSG_MS          100
    
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
    APP_STATE_IDLE=0,
    APP_STATE_INIT,
    APP_STATE_REGISTER,
    APP_STATE_SEND_PLC_MSG,
    APP_STATE_SEND_USI_MSG,
    APP_STATE_READY,
    APP_STATE_ERROR

} APP_STATE;

/* PLC Phy Tester Tool command

  Summary:
    PLC Commands enumeration

  Description:
    This enumeration defines the PLC commands used by PLC Phy Tester Tool
    provided by Microchip.
*/
typedef enum
{
    APP_CMD_PHY_GET_CFG = 0,        /* Get data configuration request */
    APP_CMD_PHY_GET_CFG_RSP,        /* Get data configuration response */
    APP_CMD_PHY_SET_CFG,            /* Set data configuration request */
    APP_CMD_PHY_SET_CFG_RSP,        /* Set data configuration response */
    APP_CMD_PHY_CMD_CFG,            /* Get command request */
    APP_CMD_PHY_CMD_CFG_RSP,        /* Get command response */
    APP_CMD_PHY_SEND_MSG,           /* Send message data */
    APP_CMD_PHY_SEND_MSG_RSP,       /* Send message data response */
    APP_CMD_PHY_RECEIVE_MSG,        /* Receive message data */
    APP_CMD_PHY_NOISE_REQ,          /* Noise capture request */
    APP_CMD_PHY_NOISE_RSP,          /* Noise capture response */
    APP_CMD_PHY_GET_CFG_LIST,       /* Get parameter list */
    APP_CMD_PHY_GET_CFG_LIST_RSP,   /* Parameter list response */
	APP_CMD_PHY_RESET_PHY_LAYER     /* Reset PHY layer */
} APP_COMMAND;

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
    SYS_TIME_HANDLE tmr1Handle; 
    
    SYS_TIME_HANDLE tmr2Handle; 
    
    APP_STATE state;
    
    DRV_HANDLE drvPl360Handle;
    
    SRV_USI_HANDLE srvUSIHandle;
    
    bool pl360_exception;
    
    uint32_t pl360_err_unexpected;
    
    uint32_t pl360_err_critical;
    
    uint32_t pl360_err_reset;
    
    uint32_t pl360_err_unknow;
    
    uint8_t pPLCDataTx[APP_PLC_DATA_BUFFER_SIZE];
    
    uint8_t pPLCDataRx[APP_PLC_DATA_BUFFER_SIZE];
    
    uint8_t pPLCDataPIB[APP_PLC_PIB_BUFFER_SIZE];
    
    uint8_t pSerialData[APP_SERIAL_DATA_BUFFER_SIZE];
    
    DRV_PL360_TRANSMISSION_OBJ plcTxObj;
    
    DRV_PL360_TRANSMISSION_CFM_OBJ plcTxCfmObj;
    
    DRV_PL360_RECEPTION_OBJ plcRxObj;
    
    DRV_PL360_PIB_OBJ plcPIB;

} APP_DATA;

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
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

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
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks( void );



#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

