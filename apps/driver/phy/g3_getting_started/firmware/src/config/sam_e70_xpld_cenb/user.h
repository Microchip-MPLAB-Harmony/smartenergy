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
 * initialization with the value defined below */
#define APP_CONFIG_STATIC_NOTCHING   false

/* Configure Coupling Parameters for PLC device. If APP_CONFIG_PLC_COUP is true,
 * Coupling parameters are set at initialization with the values defined below
 */
#define APP_CONFIG_PLC_COUP          false

/* Enable multiband. G3_MULTIBAND should be selected in the PLC Profile menu of
 * the PLC PHY Driver configuration options in Harmony Configurator. Two
 * binaries (CENELEC-A and FCC) have to be linked. If APP_CONFIG_PLC_MULTIBAND
 * is true, it will be possible to change the G3 band through the console.
 * This example is for CENELEC-B, so multiband is not supported. */
#define APP_CONFIG_PLC_MULTIBAND     false

/* Tone Mask (Static Notching): Each carrier corresponding to the band can be
 * notched (no energy is sent in those carriers). Each carrier is represented by
 * one byte (0: carrier used; 1: carrier notched). By default it is all 0's in
 * the PLC transceiver. The length is the number of carriers corresponding to
 * the band in use (see "drv_plc_phy_comm.h"). In this case 16 (CENELEC-B). The
 * same Tone Mask must be set in both transmitter and receiver, otherwise they
 * don't understand each other */
#define STATIC_NOTCHING_CENB     {0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, \
                                  0}

/**************************** Coupling Parameters ****************************/
/* CENELEC-B: Recommended to use PLCOUP014_v1 (external driver) /
 * PLCOUP012_v1/PLCOUP013_v1 (internal driver) coupling board (Single Branch).
 * The following values are the default values, calibrated for MCHP
 * PLCOUP014_v1 reference designs. For other Hardware designs, it may be needed
 * to calibrate and obtain your own values. MCHP PHY Calibration Tool should be
 * used */

/* PLC_ID_NUM_TX_LEVELS: Number of TX levels. Number of Tx attenuation levels
 * (3 dB steps) for normal behavior. Next levels use always LO mode.
 * Maximum values is 8 */
#define NUM_TX_LEVELS            8

/* PLC_ID_MAX_RMS_TABLE_HI: Target RMS_CALC value in HI mode when dynamic gain
 * is enabled (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2). Each value (up to 8)
 * corresponds to the first attenuation levels (3 dB steps) */
#define MAX_RMS_HI_TABLE_CENB    {1133, 793, 559, 396, 280, 199, 143, 108}

/* PLC_ID_MAX_RMS_TABLE_VLO: Target RMS_CALC value in VLO mode when dynamic gain
 * is enabled (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2). Each value (up to 8)
 * corresponds to the first attenuation levels (3 dB steps) */
#define MAX_RMS_VLO_TABLE_CENB   {2871, 2120, 1498, 1054, 740, 519, 366, 259}

/* PLC_ID_THRESHOLDS_TABLE_HI: Thresholds to change impedance mode
 * (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1) from HI mode.
 * First 8 values (one per Tx level) are thresholds to change from HI to LO
 * (0 to disable).
 * Next 8 values (one per Tx level) are thresholds to change from HI to VLO.
 * When RMS_CALC is below threshold, impedance mode changes to VLO */
#define THRESHOLD_HI_TABLE_CENB  {0, 0, 0, 0, 0, 0, 0, 0, \
                                  950, 667, 471, 334, 238, 169, 122, 90}

/* PLC_ID_THRESHOLDS_TABLE_VLO: Thresholds to change impedance mode
 * (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1) from VLO mode.
 * First 8 values (one per Tx level) are thresholds to change from VLO to LO
 * (0 to disable).
 * Next 8 values (one per Tx level) are thresholds to change from VLO to HI.
 * When RMS_CALC is above threshold, impedance mode changes to HI */
#define THRESHOLD_VLO_TABLE_CENB {0, 0, 0, 0, 0, 0, 0, 0, \
                                  3878, 2749, 1935, 1362, 965, 686, 493, 353}

/* PLC_ID_PREDIST_COEF_TABLE_HI: Equalization values for HI mode. Specific gain
 * for each carrier to equalize transmission and compensate HW filter frequency
 * response */
#define PREDIST_HI_TABLE_CENB    {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF}

/* PLC_ID_PREDIST_COEF_TABLE_VLO: Equalization values for VLO mode. Specific
 * gain for each carrier to equalize transmission and compensate HW filter
 * frequency response */
#define PREDIST_VLO_TABLE_CENB   {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                  0x7FFF}

/* PLC_ID_GAIN_TABLE_HI: Gain values for HI mode. {Initial, minimum, maximum}.
 * Minimum and Maximum values are used when dynamic gain is enabled
 * (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2) */
#define GAIN_HI_CENB              {248, 119, 496}

/* PLC_ID_GAIN_TABLE_VLO: Gain values for VLO mode. {Initial, minimum, maximum}.
 * Minimum and Maximum values are used when dynamic gain is enabled
 * (PLC_ID_CFG_AUTODETECT_IMPEDANCE = 1 or 2) */
#define GAIN_VLO_CENB             {701, 350, 883}

/* PLC_ID_DACC_TABLE_CFG: DAC Table configuration.
 * Configuration for PLCOUP014 (only use Branch 0) */
 #define DACC_CFG_TABLE_CENB      {0x00000000, 0x00002120, 0x0000073F, \
                                   0x00003F3F, 0x00000333, 0x00000000, \
                                   0x58CA00FF, 0x19191919, 0x00002020, \
                                   0x00000044, 0x0FD20004, 0x00000355, \
                                   0x0F000000, 0x001020F0, 0x00000355, \
                                   0x0F000000, 0x001020FF}

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
