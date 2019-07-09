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
    
/* Number of TX levels: Max.8 */
#define NUM_TX_LEVELS            8    

/* Coupling Values to define RMS values in High/Very Low impedance */
#define MAX_RMS_HI_TABLE_FCC     {1355, 960, 681, 485, 345, 246, 177, 129}
#define MAX_RMS_VLO_TABLE_FCC    {5656, 4174, 2877, 1987, 1413, 1020, 736, 532}

/* Coupling Values to define Threshold values to check in High/Very Low impedance */
#define THRESHOLD_HI_TABLE_FCC   {0, 0, 0, 0, 0, 0, 0, 0, \
                                  1147, 811, 576, 409, 291, 208, 150, 109}
#define THRESHOLD_VLO_TABLE_FCC  {0, 0, 0, 0, 0, 0, 0, 0, \
                                  8153, 5718, 4007, 2871, 2080, 1506, 1083, 778}

/* Coupling Values to define Predistorsion Coefficents in High/Very Low impedance */
#define PREDIST_HI_TABLE_FCC   {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}
    
#define PREDIST_VLO_TABLE_FCC  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}

/* Coupling Values to define Gain in High/Very Low impedance: {INI, MIN, MAX} */
#define GAIN_HI_FCC              {109, 50, 256}
#define GAIN_VLO_FCC             {364, 180, 408}

#define DACC_CFG_TABLE_COUP06  {0x00000000, 0x10102120, 0x033F073F, 0x3F3F3F3F, \
                                0x00000FFF, 0x00000000, 0x2A3000FF, 0x1B1B1B1B, \
                                0x10101010, 0x00001111, 0x04380006, 0x000003AA, \
                                0xF0000000, 0x001020F0, 0x00000355, 0x0F000000, \
                                0x001020FF}

#define DACC_CFG_TABLE_COUP11  {0x00000000, 0x21202120, 0x073F073F, 0x3F3F3F3F, \
                                0x00000FFF, 0x00000000, 0x2A3000FF, 0x1B1B1B1B, \
                                0x10101010, 0x00001111, 0x04380006, 0x000003AA, \
                                0xF0000000, 0x001020FF, 0x000003AA, 0xF0000000, \
                                0x001020FF}    

#define DACC_CFG_TABLE_FCC       DACC_CFG_TABLE_COUP11    
    
#define APP_CONFIG_PLC_COUP      true
    
#define APP_G3_PAN_ID            0x781D   


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
