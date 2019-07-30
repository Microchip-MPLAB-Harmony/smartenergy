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

/* Enable setting Tone Mask (Static Notching) in order to not use all band
 * carriers. If APP_CONFIG_STATIC_NOTCHING is true, PLC_ID_TONE_MASK is set at
 * initialization with the value defined below. */
#define APP_CONFIG_STATIC_NOTCHING   false

/* Configure Coupling Parameters for PLC device. If APP_CONFIG_PLC_COUP is true,
 * Coupling parameters are set at initialization with the values defined below.
 * In this example multiband is supported, so PLCOUP011_v1 should be used. For
 * PLCOUP011_v1, DAC Table configuration must be configured when using FCC band
 * (the default configuration in the PLC transceiver assumes PLCOUP006_v2).
 * Thus, Coupling Parameters configuration must be enabled in this project (DAC
 * Table configuration would be enough, but for code simplicity all parameters
 * will be configured). */
#define APP_CONFIG_PLC_COUP          true

/* Enable multiband. G3_MULTIBAND should be selected in the PLC Profile menu of
 * the PLC PHY Driver configuration options in Harmony Configurator. Two
 * binaries (CENELEC-A and FCC) have to be linked. If APP_CONFIG_PLC_MULTIBAND
 * is true, it will be possible to change the G3 band through the console.
 * This example supports multiband (CENELEC-A / FCC). */
#define APP_CONFIG_PLC_MULTIBAND     true

/* Tone Mask (Static Notching): Each carrier corresponding to the band can be
 * notched (no energy is sent in those carriers). Each carrier is represented by
 * one byte (0: carrier used; 1: carrier notched). By default it is all 0's in
 * the PLC transceiver. The length is the number of carriers corresponding to
 * the band in use (see "drv_plc_phy_comm.h"). In this case 36 (CENELEC-A) or
 * 72 (FCC). The same Tone Mask must be set in both transmitter and receiver,
 * otherwise they don't understand each other */
#define STATIC_NOTCHING_CENA     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, \
                                  0, 0, 0, 0, 0, 0}

#define STATIC_NOTCHING_FCC      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                  0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, \
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

/**************************** Coupling Parameters ****************************/
/* CENELEC-A: Recommended to use PLCOUP007_v2/PLCOUP008_v2/PLCOUP011_v1 coupling
 * board (Single Branch). The following values are the default values,
 * calibrated for MCHP PLCOUP007_v2/PLCOUP011_v1 reference designs. For other
 * Hardware designs, it may be needed to calibrate and obtain your own values.
 * MCHP PHY Calibration Tool should be used. */

/* FCC: Recommended to use PLCOUP006_v2 (Double Branch) / PLCOUP011_v1 (Single
 * Branch) coupling board. The following values are the default values,
 * calibrated for MCHP PLCOUP006_v2 reference design. For other Hardware
 * designs, it may be needed to calibrate and obtain your own values. MCHP PHY
 * Calibration Tool should be used. */

/* PLC_ID_NUM_TX_LEVELS: Number of TX levels. Number of Tx attenuation levels
 * (3 dB steps) for normal behavior. Next levels use always LOW mode.
 * Maximum values is 8 */
#define NUM_TX_LEVELS_CENA       8
#define NUM_TX_LEVELS_FCC        8

/* PLC_ID_MAX_RMS_TABLE_HI: Target RMS_CALC value in HIGH mode when dynamic gain
 * is enabled (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2). Each value (up to 8)
 * corresponds to the first attenuation levels (3 dB steps) */
#define MAX_RMS_HI_TABLE_CENA    {1991, 1381, 976, 695, 495, 351, 250, 179}
#define MAX_RMS_HI_TABLE_FCC     {1355, 960, 681, 485, 345, 246, 177, 129}

/* PLC_ID_MAX_RMS_TABLE_VLO: Target RMS_CALC value in VERY_LOW mode when dynamic
 * gain is enabled (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2). Each value
 * (up to 8) corresponds to the first attenuation levels (3 dB steps) */
#define MAX_RMS_VLO_TABLE_CENA   {6356, 4706, 3317, 2308, 1602, 1112, 778, 546}
#define MAX_RMS_VLO_TABLE_FCC    {5656, 4174, 2877, 1987, 1413, 1020, 736, 532}

/* PLC_ID_THRESHOLDS_TABLE_HI: Thresholds to change impedance mode
 * (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1) from HIGH mode.
 * First 8 values (one per Tx level) are thresholds to change from HIGH to LOW
 * (0 to disable).
 * Next 8 values (one per Tx level) are thresholds to change from HIGH to
 * VERY_LOW. When RMS_CALC is below threshold, impedance mode changes to
 * VERY_LOW */
#define THRESHOLD_HI_TABLE_CENA  {0, 0, 0, 0, 0, 0, 0, 0, \
                                  1685, 1173, 828, 589, 419, 298, 212, 151}
#define THRESHOLD_HI_TABLE_FCC   {0, 0, 0, 0, 0, 0, 0, 0, \
                                  1147, 811, 576, 409, 291, 208, 150, 109}

/* PLC_ID_THRESHOLDS_TABLE_VLO: Thresholds to change impedance mode
 * (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1) from VERY_LOW mode.
 * First 8 values (one per Tx level) are thresholds to change from VERY_LOW to
 * LOW (0 to disable).
 * Next 8 values (one per Tx level) are thresholds to change from VERY_LOW to
 * HIGH. When RMS_CALC is above threshold, impedance mode changes to HIGH */
#define THRESHOLD_VLO_TABLE_CENA {0, 0, 0, 0, 0, 0, 0, 0, \
                                  8988, 6370, 4466, 3119, 2171, 1512, 1061, 752}
#define THRESHOLD_VLO_TABLE_FCC  {0, 0, 0, 0, 0, 0, 0, 0, \
                                  8153, 5718, 4007, 2871, 2080, 1506, 1083, 778}

/* PLC_ID_PREDIST_COEF_TABLE_HI: Equalization values for HIGH mode. Specific
 * gain for each carrier to equalize transmission and compensate HW filter
 * frequency response */
#define PREDIST_HI_TABLE_CENA    {0x670A, 0x660F, 0x676A, 0x6A6B, 0x6F3F, \
                                  0x7440, 0x74ED, 0x7792, 0x762D, 0x7530, \
                                  0x7938, 0x7C0A, 0x7C2A, 0x7B0E, 0x7AF2, \
                                  0x784B, 0x7899, 0x76F9, 0x76D6, 0x769F, \
                                  0x775D, 0x70C0, 0x6EB9, 0x6F18, 0x6F1E, \
					              0x6FA2, 0x6862, 0x67C9, 0x68F9, 0x68A5, \
                                  0x6CA3, 0x7153, 0x7533, 0x750B, 0x7B59, \
                                  0x7FFF}
#define PREDIST_HI_TABLE_FCC     {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
						          0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
						          0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF}

/* PLC_ID_PREDIST_COEF_TABLE_VLO: Equalization values for VERY_LOW mode.
 * Specific gain for each carrier to equalize transmission and compensate HW
 * filter frequency response */
#define PREDIST_VLO_TABLE_CENA   {0x7FFF, 0x7DB1, 0x7CE6, 0x7B36, 0x772F, \
                                  0x7472, 0x70AA, 0x6BC2, 0x682D, 0x6618, \
                                  0x6384, 0x6210, 0x61D7, 0x6244, 0x6269, \
                                  0x63A8, 0x6528, 0x65CC, 0x67F6, 0x693B, \
                                  0x6B13, 0x6C29, 0x6D43, 0x6E26, 0x6D70, \
					              0x6C94, 0x6BB5, 0x6AC9, 0x6A5F, 0x6B65, \
                                  0x6B8C, 0x6A62, 0x6CEC, 0x6D5A, 0x6F9D, \
                                  0x6FD3}
#define PREDIST_VLO_TABLE_FCC    {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
						          0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
						          0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF}

/* PLC_ID_GAIN_TABLE_HI: Gain values for HIGH mode. {Initial, minimum, maximum}.
 * Minimum and Maximum values are used when dynamic gain is enabled
 * (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2) */
#define GAIN_HI_CENA              {142, 70, 336}
#define GAIN_HI_FCC               {109, 50, 256}

/* PLC_ID_GAIN_TABLE_VLO: Gain values for VERY_LOW mode.
 * {Initial, minimum, maximum}. Minimum and Maximum values are used when dynamic
 * gain is enabled (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2) */
#define GAIN_VLO_CENA             {474, 230, 597}
#define GAIN_VLO_FCC              {364, 180, 408}

/* PLC_ID_DACC_TABLE_CFG: DAC Table configuration (CENELEC-A).
 * Configuration for PLCOUP007/PLCOUP008/PLCOUP011 (only use Branch 0) */
 #define DACC_CFG_TABLE_CENA      {0x00000000, 0x00002120, 0x0000073F, \
                                   0x00003F3F, 0x00000333, 0x00000000, \
                                   0x610800FF, 0x14141414, 0x00002020, \
                                   0x00000044, 0x0FD20004, 0x00000355, \
                                   0x0F000000, 0x001020F0, 0x00000355, \
                                   0x0F000000, 0x001020FF}

/* PLC_ID_DACC_TABLE_CFG: DAC Table configuration (FCC).
 * Configuration for PLCOUP006 (use Branch 0 for VERY_LOW and Branch 1 for HIGH)
 */
 #define DACC_CFG_TABLE_FCC_COUP6 {0x00000000, 0x10102120, 0x033F073F, \
                                   0x3F3F3F3F, 0x00000FFF, 0x00000000, \
                                   0x2A3000FF, 0x1B1B1B1B, 0x10101010, \
                                   0x00001111, 0x04380006, 0x000003AA, \
                                   0xF0000000, 0x001020F0, 0x00000355, \
                                   0x0F000000, 0x001020FF}

/* PLC_ID_DACC_TABLE_CFG: DAC Table configuration (FCC).
 * Configuration for PLCOUP011 (only use Branch 1) */
 #define DACC_CFG_TABLE_FCC_COUP11 {0x00000000, 0x21202120, 0x073F073F, \
                                    0x3F3F3F3F, 0x00000FFF, 0x00000000, \
                                    0x2A3000FF, 0x1B1B1B1B, 0x10101010, \
                                    0x00001111, 0x04380006, 0x000003AA, \
                                    0xF0000000, 0x001020FF, 0x000003AA, \
                                    0xF0000000, 0x001020FF}

/* PLC_ID_DACC_TABLE_CFG: DAC Table configuration (FCC).
 * In this case choose PLCOUP011 configuration in order to be able to use the
 * same hardware for both CENELEC-A and FCC bands (this project supports
 * multiband). With PLCOUP011 in FCC it does not make sense to use HIGH Tx mode
 * (PLC_ID_CFG_AUTODETECT_IMPEDANCE should be 0 or 2 and PLC_ID_CFG_IMPEDANCE
 * should be 2 (VERY_LOW)). */
#define DACC_CFG_TABLE_FCC        DACC_CFG_TABLE_FCC_COUP11

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
