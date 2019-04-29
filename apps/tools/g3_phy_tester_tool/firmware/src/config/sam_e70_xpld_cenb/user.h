/*******************************************************************************
  User Configuration Header

  File Name:
    user.h

  Summary:
    Build-time configuration header for the user defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    It only provides macro definitions for build-time configuration options

*******************************************************************************/

#ifndef USER_H
#define USER_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

/* The below tables should be redefined by customer to calibrate their own hardware
 * These values have been got from ATPLCOUP014v1 coupling board */    
    
/* Number of TX levels: Max.8 */
#define NUM_TX_LEVELS           8

/* Coupling Values to define RMS values in High/Very Low impedance */
#define MAX_RMS_HI_TABLE       {1133, 793, 559, 396, 280, 199, 143, 108}
#define MAX_RMS_VLO_TABLE      {2871, 2120, 1498, 1054, 740, 519, 366, 259}

/* Coupling Values to define Threshold values to check in High/Very Low impedance */
#define THRESHOLD_HI_TABLE     {0, 0, 0, 0, 0, 0, 0, 0, \
                                950, 667, 471, 334, 238, 169, 122, 90}
#define THRESHOLD_VLO_TABLE    {0, 0, 0, 0, 0, 0, 0, 0, \
                                3878, 2749, 1935, 1362, 965, 686, 493, 353}

/* Coupling Values to define Predistorsion Coefficents in High/Very Low impedance */
#define PREDIST_HI_TABLE       {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}
    
#define PREDIST_VLO_TABLE      {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}

/* Coupling Values to define Gain in High/Very Low impedance: {INI, MIN, MAX} */
#define GAIN_HI                {248, 119, 496}
#define GAIN_VLO               {701, 350, 883}

#define DACC_CFG_TABLE         {0x00000000, 0x00002120, 0x0000073F, 0x00003F3F, \
                                0x00000333, 0x00000000, 0x58CA00FF, 0x19191919, \
                                0x00002020, 0x00000044, 0x0FD20004, 0x00000355, \
                                0x0F000000, 0x001020F0, 0x00000355, 0x0F000000, \
                                0x001020FF}   
    
#define APP_CONFIG_PLC_COUP     true 


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
