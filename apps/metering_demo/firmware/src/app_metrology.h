/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_metrology.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_METROLOGY_Initialize" and "APP_METROLOGY_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_METROLOGY_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_METROLOGY_H
#define _APP_METROLOGY_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

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
// Waveform capture buffer length register offset
#define MET_CAPTURE_BUFF_LENGTH_OFFSET      46
// Waveform capture buffer address register offset
#define MET_CAPTURE_ADDR_OFFSET             47
// Waveform capture buffer length
#define MET_CAPTURE_BUFF_LEN                8000
    
typedef enum
{
    CONTROL_STATE_CTRL = 0,         
    CONTROL_FEATURE_CTRL0_ID,      
    CONTROL_FEATURE_CTRL1_ID,      
    CONTROL_METER_TYPE_ID,         
    CONTROL_M_ID,                  
    CONTROL_N_MAX_ID,              
    CONTROL_PULSE0_CTRL_ID,        
    CONTROL_PULSE1_CTRL_ID,        
    CONTROL_PULSE2_CTRL_ID,        
    CONTROL_P_K_t_ID,              
    CONTROL_Q_K_t_ID,              
    CONTROL_I_K_t_ID,              
    CONTROL_CREEP_THRESHOLD_P_ID,  
    CONTROL_CREEP_THRESHOLD_Q_ID,  
    CONTROL_CREEP_THRESHOLD_I_ID,  
    CONTROL_POWER_OFFSET_CTRL_ID,  
    CONTROL_POWER_OFFSET_P_ID,     
    CONTROL_POWER_OFFSET_Q_ID,     
    CONTROL_SWELL_THRESHOLD_VA_ID, 
    CONTROL_SWELL_THRESHOLD_VB_ID, 
    CONTROL_SWELL_THRESHOLD_VC_ID, 
    CONTROL_SAG_THRESHOLD_VA_ID,   
    CONTROL_SAG_THRESHOLD_VB_ID,   
    CONTROL_SAG_THRESHOLD_VC_ID,   
    CONTROL_K_IA_ID,               
    CONTROL_K_VA_ID,               
    CONTROL_K_IB_ID,               
    CONTROL_K_VB_ID,               
    CONTROL_K_IC_ID,               
    CONTROL_K_VC_ID,               
    CONTROL_K_IN_ID,               
    CONTROL_CAL_M_IA_ID,           
    CONTROL_CAL_M_VA_ID,           
    CONTROL_CAL_M_IB_ID,           
    CONTROL_CAL_M_VB_ID,           
    CONTROL_CAL_M_IC_ID,           
    CONTROL_CAL_M_VC_ID,           
    CONTROL_CAL_M_IN_ID,           
    CONTROL_CAL_PH_IA_ID,          
    CONTROL_CAL_PH_VA_ID,          
    CONTROL_CAL_PH_IB_ID,          
    CONTROL_CAL_PH_VB_ID,          
    CONTROL_CAL_PH_IC_ID,          
    CONTROL_CAL_PH_VC_ID,          
    CONTROL_CAL_PH_IN_ID,          
    CONTROL_CAPTURE_CTRL_ID,       
    CONTROL_CAPTURE_BUFF_SIZE_ID,  
    CONTROL_CAPTURE_ADDR_ID,       
    CONTROL_RES1_ID,
    CONTROL_RES2_ID,
    CONTROL_RES3_ID,
    CONTROL_ATSENSE_CTRL_20_23_ID, 
    CONTROL_ATSENSE_CTRL_24_27_ID, 
    CONTROL_ATSENSE_CTRL_28_2B_ID, 
    CONTROL_RES4_ID,
    CONTROL_POWER_OFFSET_P_A_ID,   
    CONTROL_POWER_OFFSET_P_B_ID,   
    CONTROL_POWER_OFFSET_P_C_ID,   
    CONTROL_POWER_OFFSET_Q_A_ID,   
    CONTROL_POWER_OFFSET_Q_B_ID,   
    CONTROL_POWER_OFFSET_Q_C_ID,
    CONTROL_REG_NUM,
} CONTROL_REG_ID;

typedef enum
{
    STATUS_VERSION_ID = 0,          
    STATUS_STATUS_ID,           
    STATUS_STATE_FLAG_ID,       
    STATUS_CAPTURE_STATUS_ID,   
    STATUS_INTERVAL_NUM_ID,     
    STATUS_N_ID,                
    STATUS_PH_OFFSET_ID,        
    STATUS_FREQ_ID,             
    STATUS_FREQ_VA_ID,          
    STATUS_FREQ_VB_ID,          
    STATUS_FREQ_VC_ID,          
    STATUS_RES1_ID,  
    STATUS_TEMPERATURE_ID,      
    STATUS_I_A_MAX_ID,          
    STATUS_I_B_MAX_ID,          
    STATUS_I_C_MAX_ID,          
    STATUS_I_Ni_MAX_ID,         
    STATUS_I_Nm_MAX_ID,         
    STATUS_V_A_MAX_ID,          
    STATUS_V_B_MAX_ID,          
    STATUS_V_C_MAX_ID,          
    STATUS_FEATURES_ID,         
    STATUS_RES2_ID,     
    STATUS_RES3_ID,     
    STATUS_RES4_ID,     
    STATUS_RES5_ID,     
    STATUS_RES6_ID,     
    STATUS_RES7_ID,     
    STATUS_RES8_ID,     
    STATUS_RES9_ID,  
    STATUS_ZC_N_VA_ID,          
    STATUS_ZC_N_VB_ID,          
    STATUS_ZC_N_VC_ID,          
    STATUS_ATSENSE_CAL_41_44_ID,
    STATUS_ATSENSE_CAL_45_48_ID,
    STATUS_REG_NUM,
} STATUS_REG_ID;

typedef enum
{
    ACCUMULATOR_I_A_ID = 0,           
    ACCUMULATOR_I_B_ID,           
    ACCUMULATOR_I_C_ID,           
    ACCUMULATOR_I_Ni_ID,          
    ACCUMULATOR_I_Nm_ID,          
    ACCUMULATOR_I_A_F_ID,         
    ACCUMULATOR_I_B_F_ID,         
    ACCUMULATOR_I_C_F_ID,         
    ACCUMULATOR_I_Nmi_ID,
    ACCUMULATOR_RES1_ID,
    ACCUMULATOR_RES2_ID,
    ACCUMULATOR_RES3_ID,
    ACCUMULATOR_RES4_ID,
    ACCUMULATOR_RES5_ID,
    ACCUMULATOR_RES6_ID,
    ACCUMULATOR_P_A_ID,           
    ACCUMULATOR_P_B_ID,           
    ACCUMULATOR_P_C_ID,           
    ACCUMULATOR_P_A_F_ID,         
    ACCUMULATOR_P_B_F_ID,         
    ACCUMULATOR_P_C_F_ID,         
    ACCUMULATOR_P_N_ID,           
    ACCUMULATOR_P_N_F_ID,   
    ACCUMULATOR_RES7_ID,    
    ACCUMULATOR_Q_A_ID,           
    ACCUMULATOR_Q_B_ID,           
    ACCUMULATOR_Q_C_ID,           
    ACCUMULATOR_Q_A_F_ID,         
    ACCUMULATOR_Q_B_F_ID,         
    ACCUMULATOR_Q_C_F_ID,         
    ACCUMULATOR_Q_N_ID,           
    ACCUMULATOR_Q_N_F_ID,    
    ACCUMULATOR_RES8_ID,
    ACCUMULATOR_V_A_ID,           
    ACCUMULATOR_V_B_ID,           
    ACCUMULATOR_V_C_ID,      
    ACCUMULATOR_RES9_ID,
    ACCUMULATOR_V_A_F_ID,         
    ACCUMULATOR_V_B_F_ID,         
    ACCUMULATOR_V_C_F_ID,
    ACCUMULATOR_RES10_ID,
    ACCUMULATOR_V_AB_ID,          
    ACCUMULATOR_V_BC_ID,          
    ACCUMULATOR_V_CA_ID,          
    ACCUMULATOR_V_AB_F_ID,        
    ACCUMULATOR_V_BC_F_ID,        
    ACCUMULATOR_V_CA_F_ID, 
    ACCUMULATOR_RES11_ID,
    ACCUMULATOR_RES12_ID,
    ACCUMULATOR_RES13_ID,
    ACCUMULATOR_ACC_T0_ID,        
    ACCUMULATOR_ACC_T1_ID,        
    ACCUMULATOR_ACC_T2_ID,
    ACCUMULATOR_REG_NUM,
} ACCUMULATOR_REG_ID;

typedef enum
{
    HARMONICS_I_A_m_R_ID = 0,
    HARMONICS_V_A_m_R_ID,
    HARMONICS_I_B_m_R_ID,
    HARMONICS_V_B_m_R_ID,
    HARMONICS_I_C_m_R_ID,
    HARMONICS_V_C_m_R_ID,
    HARMONICS_I_A_m_I_ID,
    HARMONICS_V_A_m_I_ID,
    HARMONICS_I_B_m_I_ID,
    HARMONICS_V_B_m_I_ID,
    HARMONICS_I_C_m_I_ID,
    HARMONICS_V_C_m_I_ID,
    HARMONICS_REG_NUM,
} HARMONICS_REG_ID;
    
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
    APP_METROLOGY_STATE_INIT=0,
    APP_METROLOGY_STATE_START,
    APP_METROLOGY_STATE_RUNNING,
    APP_METROLOGY_STATE_ERROR

} APP_METROLOGY_STATES;


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
    APP_METROLOGY_STATES state;
    
    MET_CONTROL *        pMetControl;
    MET_STATUS *         pMetStatus;
    MET_ACCUMULATORS *   pMetAccData;
    MET_HARMONICS *      pMetHarData;

} APP_METROLOGY_DATA;

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
    void APP_METROLOGY_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_METROLOGY_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_METROLOGY_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_METROLOGY_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_METROLOGY_Tasks ( void )

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
    APP_METROLOGY_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_METROLOGY_Tasks( void );




uint32_t APP_METROLOGY_GetControlRegister( CONTROL_REG_ID regId );
bool APP_METROLOGY_SetControlRegister( CONTROL_REG_ID regId, uint32_t value );
uint32_t APP_METROLOGY_GetStatusRegister( STATUS_REG_ID regId );
uint64_t APP_METROLOGY_GetAccumulatorRegister( ACCUMULATOR_REG_ID regId );
uint32_t APP_METROLOGY_GetHarmonicsRegister( HARMONICS_REG_ID regId );

uint32_t APP_METROLOGY_GetRMS( MET_RMS_TYPE rmsId );


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_METROLOGY_H */

/*******************************************************************************
 End of File
 */

