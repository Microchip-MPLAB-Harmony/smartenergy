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
#define MAX_RMS_HI_TABLE_CENA    {1991, 1381, 976, 695, 495, 351, 250, 179}
#define MAX_RMS_VLO_TABLE_CENA   {6356, 4706, 3317, 2308, 1602, 1112, 778, 546}

/* Coupling Values to define Threshold values to check in High/Very Low impedance */
#define THRESHOLD_HI_TABLE_CENA  {0, 0, 0, 0, 0, 0, 0, 0, \
                                  1685, 1173, 828, 589, 419, 298, 212, 151}
#define THRESHOLD_VLO_TABLE_CENA {0, 0, 0, 0, 0, 0, 0, 0, \
                                  8988, 6370, 4466, 3119, 2171, 1512, 1061, 752}

/* Coupling Values to define Predistorsion Coefficents in High/Very Low impedance */
#define PREDIST_HI_TABLE_CENA  {0x670A, 0x660F, 0x676A, 0x6A6B, 0x6F3F, 0x7440, \
                                0x74ED, 0x7792, 0x762D, 0x7530, 0x7938, 0x7C0A, \
                                0x7C2A, 0x7B0E, 0x7AF2, 0x784B, 0x7899, 0x76F9, \
                                0x76D6, 0x769F, 0x775D, 0x70C0, 0x6EB9, 0x6F18, \
                                0x6F1E, 0x6FA2, 0x6862, 0x67C9, 0x68F9, 0x68A5, \
                                0x6CA3, 0x7153, 0x7533, 0x750B, 0x7B59, 0x7FFF}
    
#define PREDIST_VLO_TABLE_CENA {0x7FFF, 0x7DB1, 0x7CE6, 0x7B36, 0x772F, 0x7472, \
                                0x70AA, 0x6BC2, 0x682D, 0x6618, 0x6384, 0x6210, \
                                0x61D7, 0x6244, 0x6269, 0x63A8, 0x6528, 0x65CC, \
                                0x67F6, 0x693B, 0x6B13, 0x6C29, 0x6D43, 0x6E26, \
                                0x6D70, 0x6C94, 0x6BB5, 0x6AC9, 0x6A5F, 0x6B65, \
                                0x6B8C, 0x6A62, 0x6CEC, 0x6D5A, 0x6F9D, 0x6FD3}

/* Coupling Values to define Gain in High/Very Low impedance: {INI, MIN, MAX} */
#define GAIN_HI_CENA             {142, 70, 336}
#define GAIN_VLO_CENA            {474, 230, 597}

#define DACC_CFG_TABLE_CENA    {0x00000000, 0x00002120, 0x0000073F, 0x00003F3F, \
                                0x00000333, 0x00000000, 0x610800FF, 0x14141414, \
                                0x00002020, 0x00000044, 0x0FD20004, 0x00000355, \
                                0x0F000000, 0x001020F0, 0x00000355, 0x0F000000, \
                                0x001020FF}  
    
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
