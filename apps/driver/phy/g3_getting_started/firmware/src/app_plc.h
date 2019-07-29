/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_plc.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_PLC_Initialize" and "APP_PLC_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_PLC_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_PLC_H
#define _APP_PLC_H

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
    
// *****************************************************************************
/* Maximum data length

  Summary:
    Maximum message data length allowed by G3-PLC Physical Layer

  Description:
    The maximum data length depends on:
      - G3 band (CENELEC-A, CENELEC-B, FCC or ARIB).
      - Modulation Type (BPSK Robust, BPSK, QPSK or 8PSK).
        See "DRV_PLC_PHY_MOD_TYPE" in "drv_plc_phy_comm.h".
      - Modulation Scheme (Differential of coherent).
        See "DRV_PLC_PHY_MOD_SCHEME" in "drv_plc_phy_comm.h".
      - Number of Reed-Solomon blocks ("rs2Blocks"). Only applies to FCC band.
        1 or 2 blocks. With 2 blocks the maximum length is the double.
      - Tone Map ("toneMap"). Dynamic notching: The subbands used to send the
        data can be chosen with Tone Map.
      - Tone Mask (PLC_ID_TONE_MASK). Static notching: Carriers can be notched
        and no energy is sent in those carriers.
 
  Remarks:
    The absolute maximum corresponds to FCC band with 2 Reed-Solomon blocks and
    BPSK Robust modulation. 2 * (255 - 8) = 494.
*/
#define MAX_DATA_LEN        494
    
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
    /* Application's initial state. */
    APP_PLC_STATE_INIT=0,
            
    /* Application waiting for completion of PLC driver open process */
    APP_PLC_STATE_OPEN,
            
    /* Application configures PIB parameters through PLC driver */
    APP_PLC_STATE_PARAM_CONFIG,
            
    /* Application initializes transmission parameters */
    APP_PLC_STATE_INIT_TX_PARAMS,
            
    /* Waiting for commands from console */
    APP_PLC_STATE_WAITING,
            
    /*  */
    APP_PLC_STATE_ERROR,

} APP_PLC_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_PLC_STATES state;

    /* Handle to the PLC driver */
    DRV_HANDLE drvPlcHandle;
    
    /* Parameters for PLC transmitted message */
    DRV_PLC_PHY_TRANSMISSION_OBJ plcTxObj;
    
    /* Maximum data length depending on configuration */
    uint16_t maxDataLen;
    
    /* G3-PLC band */
    uint8_t plcBand;
    
    /* Number of carriers (depends on G3-PLC band) */
    uint8_t numCarriers;
    
    /* Number of subbands (depends on G3-PLC band) */
    uint8_t numSubbands;
    
    /* Size of Tone Map in bytes (depends on G3-PLC band) */
    uint8_t toneMapSize;
    
    bool bin2InUse;
    
    bool plcMultiband;
    
    /* Enable/Disable Tone Mask (Static Notching) configuration */
    bool toneMaskConfig;
    
    /* Enable/Disable Coupling parameters configuration */
    bool couplingConfig;
    
    /* Flag to indicate that we are waiting for a Transmission Confirm */
    bool waitingTxCfm;

} APP_PLC_DATA;

extern APP_PLC_DATA appPlcData;

// *****************************************************************************
/* PLC Coupling configuration data

  Summary:
    Holds PLC configuration data

  Description:
    This structure holds the PLC coupling configuration data.

  Remarks: 
    The maximum number of levels is 8.
 */    
typedef struct {
	uint32_t maxRMSHigh[8];
	uint32_t maxRMSVeryLow[8];
	uint32_t thresholdHigh[16];
	uint32_t thresholdVeryLow[16];
	uint32_t daccConfig[17];
	uint16_t predistorsionHigh[PROTOCOL_CARRIERS_MAX];
	uint16_t predistorsionVeryLow[PROTOCOL_CARRIERS_MAX];
	uint16_t gainHigh[3];
	uint16_t gainVeryLow[3];
	uint8_t numTxLevels;
} APP_PLC_COUPLING_DATA;

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
    void APP_PLC_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_PLC_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_PLC_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_PLC_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_PLC_Tasks ( void )

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
    APP_PLC_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_PLC_Tasks( void );

void APP_PLC_SendMsg(uint8_t *pData, uint16_t dataLen);

void APP_PLC_SetModulation(DRV_PLC_PHY_MOD_SCHEME modScheme, DRV_PLC_PHY_MOD_TYPE modType);

bool APP_PLC_SetBand(uint8_t newBand);

#endif /* _APP_PLC_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

