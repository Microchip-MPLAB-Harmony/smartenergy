/*******************************************************************************
  PLC PHY Coupling Service Library Interface Header File

  Company
    Microchip Technology Inc.

  File Name
    srv_pcoup.h

  Summary
    PLC PHY Coupling service library interface.

  Description
    The Microchip G3-PLC and PRIME implementations include default PHY layer
    configuration values optimized for the Evaluation Kits. With the help of
    the PHY Calibration Tool it is possible to obtain the optimal configuration
    values for the customer's hardware implementation. Refer to the online
    documentation for more details about the available configuration values and
    their purpose.

  Remarks:
    This service provides the required information to be included on PLC
    projects for PL360/PL460 in order to apply the custom calibration.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*
Copyright (C) 2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
// DOM-IGNORE-END

#ifndef SRV_PCOUP_H    // Guards against multiple inclusion
#define SRV_PCOUP_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "driver/plc/phy/drv_plc_phy.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

/* Default branch of the PLC transmission coupling */
#define SRV_PCOUP_DEFAULT_BRANCH                 SRV_PLC_PCOUP_MAIN_BRANCH

/* PLC PHY Coupling parameters for Main branch */
#define SRV_PCOUP_RMS_HIGH_TBL                   {${SRV_PCOUP_G3_RMS_HIGH_0?string}, ${SRV_PCOUP_G3_RMS_HIGH_1?string}, ${SRV_PCOUP_G3_RMS_HIGH_2?string}, ${SRV_PCOUP_G3_RMS_HIGH_3?string}, ${SRV_PCOUP_G3_RMS_HIGH_4?string}, ${SRV_PCOUP_G3_RMS_HIGH_5?string}, ${SRV_PCOUP_G3_RMS_HIGH_6?string}, ${SRV_PCOUP_G3_RMS_HIGH_7?string}}
#define SRV_PCOUP_RMS_VLOW_TBL                   {${SRV_PCOUP_G3_RMS_VLOW_0?string}, ${SRV_PCOUP_G3_RMS_VLOW_1?string}, ${SRV_PCOUP_G3_RMS_VLOW_2?string}, ${SRV_PCOUP_G3_RMS_VLOW_3?string}, ${SRV_PCOUP_G3_RMS_VLOW_4?string}, ${SRV_PCOUP_G3_RMS_VLOW_5?string}, ${SRV_PCOUP_G3_RMS_VLOW_6?string}, ${SRV_PCOUP_G3_RMS_VLOW_7?string}}
#define SRV_PCOUP_THRS_HIGH_TBL                  {${SRV_PCOUP_G3_THRS_HIGH_0?string}, ${SRV_PCOUP_G3_THRS_HIGH_1?string}, ${SRV_PCOUP_G3_THRS_HIGH_2?string}, ${SRV_PCOUP_G3_THRS_HIGH_3?string}, ${SRV_PCOUP_G3_THRS_HIGH_4?string}, ${SRV_PCOUP_G3_THRS_HIGH_5?string}, ${SRV_PCOUP_G3_THRS_HIGH_6?string}, ${SRV_PCOUP_G3_THRS_HIGH_7?string}, ${SRV_PCOUP_G3_THRS_HIGH_8?string}, ${SRV_PCOUP_G3_THRS_HIGH_9?string}, ${SRV_PCOUP_G3_THRS_HIGH_10?string}, ${SRV_PCOUP_G3_THRS_HIGH_11?string}, ${SRV_PCOUP_G3_THRS_HIGH_12?string}, ${SRV_PCOUP_G3_THRS_HIGH_13?string}, ${SRV_PCOUP_G3_THRS_HIGH_14?string}, ${SRV_PCOUP_G3_THRS_HIGH_15?string}}
#define SRV_PCOUP_THRS_VLOW_TBL                  {${SRV_PCOUP_G3_THRS_VLOW_0?string}, ${SRV_PCOUP_G3_THRS_VLOW_1?string}, ${SRV_PCOUP_G3_THRS_VLOW_2?string}, ${SRV_PCOUP_G3_THRS_VLOW_3?string}, ${SRV_PCOUP_G3_THRS_VLOW_4?string}, ${SRV_PCOUP_G3_THRS_VLOW_5?string}, ${SRV_PCOUP_G3_THRS_VLOW_6?string}, ${SRV_PCOUP_G3_THRS_VLOW_7?string}, ${SRV_PCOUP_G3_THRS_VLOW_8?string}, ${SRV_PCOUP_G3_THRS_VLOW_9?string}, ${SRV_PCOUP_G3_THRS_VLOW_10?string}, ${SRV_PCOUP_G3_THRS_VLOW_11?string}, ${SRV_PCOUP_G3_THRS_VLOW_12?string}, ${SRV_PCOUP_G3_THRS_VLOW_13?string}, ${SRV_PCOUP_G3_THRS_VLOW_14?string}, ${SRV_PCOUP_G3_THRS_VLOW_15?string}}
#define SRV_PCOUP_DACC_TBL                       {0x${SRV_PCOUP_G3_DACC_0?string}UL, 0x${SRV_PCOUP_G3_DACC_1?string}UL, 0x${SRV_PCOUP_G3_DACC_2?string}UL, 0x${SRV_PCOUP_G3_DACC_3?string}UL, 0x${SRV_PCOUP_G3_DACC_4?string}UL, 0x${SRV_PCOUP_G3_DACC_5?string}UL, \
                                                 0x${SRV_PCOUP_G3_DACC_6?string}UL, 0x${SRV_PCOUP_G3_DACC_7?string}UL, 0x${SRV_PCOUP_G3_DACC_8?string}UL, 0x${SRV_PCOUP_G3_DACC_9?string}UL, 0x${SRV_PCOUP_G3_DACC_10?string}UL, 0x${SRV_PCOUP_G3_DACC_11?string}UL, \
                                                 0x${SRV_PCOUP_G3_DACC_12?string}UL, 0x${SRV_PCOUP_G3_DACC_13?string}UL, 0x${SRV_PCOUP_G3_DACC_14?string}UL, 0x${SRV_PCOUP_G3_DACC_15?string}UL, 0x${SRV_PCOUP_G3_DACC_16?string}UL}
#define SRV_PCOUP_GAIN_HIGH_TBL                  {${SRV_PCOUP_G3_GAIN_HIGH_0?string}, ${SRV_PCOUP_G3_GAIN_HIGH_1?string}, ${SRV_PCOUP_G3_GAIN_HIGH_2?string}}
#define SRV_PCOUP_GAIN_VLOW_TBL                  {${SRV_PCOUP_G3_GAIN_VLOW_0?string}, ${SRV_PCOUP_G3_GAIN_VLOW_1?string}, ${SRV_PCOUP_G3_GAIN_VLOW_2?string}}
#define SRV_PCOUP_NUM_TX_LEVELS                  ${SRV_PCOUP_G3_NUM_TX_LVL?string}
#define SRV_PCOUP_LINE_DRV_CONF                  ${SRV_PCOUP_G3_LINE_DRIVER?string}

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* PLC PHY Coupling Branch definitions

 Summary:
    List of possible transmission branches.

 Description:
    This type defines the possible values of PLC transmission coupling branches.

 Remarks:
    None.
*/

typedef enum
{
    /* Main Transmission Branch */
    SRV_PLC_PCOUP_MAIN_BRANCH,

} SRV_PLC_PCOUP_BRANCH;

// *****************************************************************************
/* PLC PHY Coupling data

  Summary:
    PLC PHY Coupling data.

  Description:
    This structure contains all the data required to set the PLC PHY Coupling
    parameters, for a specific transmission branch (associated to a G3-PLC
    PHY band).

  Remarks:
    Equalization coefficients are not stored in the structure, just pointers to
    arrays were they are actually stored. This allows to use the same type for
    different G3-PLC PHY bands.
*/

typedef struct
{
    /* Target RMS values in HIGH mode for dynamic Tx gain */
    uint32_t                         rmsHigh[8];

    /* Target RMS values in VLOW mode for dynamic Tx gain */
    uint32_t                         rmsVLow[8];

    /* Threshold RMS values in HIGH mode for dynamic Tx mode */
    uint32_t                         thrsHigh[16];

    /* Threshold RMS values in VLOW mode for dynamic Tx mode */
    uint32_t                         thrsVLow[16];

    /* Values for configuration of PLC DACC peripheral, according to hardware
       coupling design and PLC device (PL360/PL460) */
    uint32_t                         daccTable[17];

    /* Tx gain values for HIGH mode [HIGH_INI, HIGH_MIN, HIGH_MAX] */
    uint16_t                         gainHigh[3];

    /* Tx gain values for VLOW mode [VLOW_INI, VLOW_MIN, VLOW_MAX] */
    uint16_t                         gainVLow[3];

    /* Number of Tx attenuation levels (3 dB step) supporting dynamic Tx mode */
    uint8_t                          numTxLevels;

    /* Configuration of the PLC Tx Line Driver, according to hardware coupling
       design and PLC device (PL360/PL460) */
    uint8_t                          lineDrvConf;

} SRV_PLC_PCOUP_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Service Interface Functions
// *****************************************************************************
// *****************************************************************************

/***************************************************************************
  Function:
    SRV_PLC_PCOUP_DATA * SRV_PCOUP_Get_Config(SRV_PLC_PCOUP_BRANCH branch)

  Summary:
    Get the PLC PHY Coupling parameters for the specified transmission branch.

  Description:
    This function allows to get the PLC PHY Coupling parameters for the
    specified transmission branch. These parameters can be sent to the PLC
    device through PLC Driver PIB interface (DRV_PLC_PHY_PIBSet).

  Precondition:
    None.

  Parameters:
    branch          - Transmission branch for which the parameters are requested

  Returns:
    - Pointer PLC PHY Coupling parameters
      - if branch parameter is valid
    - NULL
      - if branch parameter is not valid

  Example:
    <code>
    SRV_PLC_PCOUP_DATA *pCoupValues;

    pCoupValues = SRV_PCOUP_Get_Config(SRV_PLC_PCOUP_MAIN_BRANCH);
    </code>

  Remarks:
    If SRV_PCOUP_Set_Config is used to set the PLC PHY Coupling parameters,
    this function is not needed.
  ***************************************************************************/

SRV_PLC_PCOUP_DATA * SRV_PCOUP_Get_Config(SRV_PLC_PCOUP_BRANCH branch);

/***************************************************************************
  Function:
    bool SRV_PCOUP_Set_Config(DRV_HANDLE handle, SRV_PLC_PCOUP_BRANCH branch);

  Summary:
    Set the PLC PHY Coupling parameters for the specified transmission branch.

  Description:
    This function allows to set the PLC PHY Coupling parameters for the
    specified transmission branch, using the PLC Driver PIB
    interface (DRV_PLC_PHY_PIBSet).

  Precondition:
    DRV_PLC_PHY_Open must have been called to obtain a valid
    opened device handle.

  Parameters:
    handle  - A valid instance handle, returned from DRV_PLC_PHY_Open
    branch  - Transmission branch for which the parameters will be set

  Returns:
    - true
      - Successful configuration
    - false
      - if branch parameter is not valid
      - if there is an error when calling DRV_PLC_PHY_PIBSet

  Example:
    <code>
    bool result;

    result = SRV_PCOUP_Set_Config(handle, SRV_PLC_PCOUP_MAIN_BRANCH);
    </code>

  Remarks:
    None.
  ***************************************************************************/

bool SRV_PCOUP_Set_Config(DRV_HANDLE handle, SRV_PLC_PCOUP_BRANCH branch);

/***************************************************************************
  Function:
    SRV_PLC_PCOUP_BRANCH SRV_PCOUP_Get_Default_Branch( void )

  Summary:
    Get the default branch of the PLC transmission coupling.

  Description:
    This function allows to get the tranmission branch used by default.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Default transmission branch.

  Example:
    <code>
    SRV_PLC_PCOUP_BRANCH plcDefaultBranch;

    plcDefaultBranch = SRV_PCOUP_Get_Default_Branch();
    SRV_PCOUP_Set_Config(plcDefaultBranch);
    </code>

  Remarks:
    None.
  ***************************************************************************/

SRV_PLC_PCOUP_BRANCH SRV_PCOUP_Get_Default_Branch( void );

/***************************************************************************
  Function:
    uint8_t SRV_PCOUP_Get_Phy_Band(SRV_PLC_PCOUP_BRANCH branch)

  Summary:
    Get the G3-PLC PHY band associated to the specified transmission branch.

  Description:
    This function allows to get the G3-PLC PHY band associated to the
    specified transmission branch.

  Precondition:
    None.

  Parameters:
    branch         - Transmission branch from which the PHY band is requested

  Returns:
    G3-PLC PHY band associated to the specified transmission branch
    (see drv_plc_phy_comm):
    - 0: MM_CEN_A
    - 0xFF: MM_INVALID (if transmission branch is not valid)

  Example:
    <code>
    phyBand = SRV_PCOUP_Get_Phy_Band(SRV_PLC_PCOUP_MAIN_BRANCH);

    if (phyBand == MM_CEN_A)
    {

    }
    </code>

  Remarks:
    None.
  ***************************************************************************/

uint8_t SRV_PCOUP_Get_Phy_Band(SRV_PLC_PCOUP_BRANCH branch);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif //SRV_PCOUP_H
