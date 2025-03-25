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
#include "system/system.h"
#include "driver/plc/phy/drv_plc_phy.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

/* PLC PRIME PHY default channel */
#define SRV_PCOUP_DEFAULT_CHANNEL                ${drvPlcPhy.DRV_PLC_PRIME_DEF_CHN?string}

/* PLC PRIME PHY Channel for impedance detection */
#define SRV_PCOUP_CHANNEL_IMP_DET                ${SRV_PCOUP_PRIME_CHANNEL_IMP_DET?string}

/* PLC PRIME PHY Channel List */
#define SRV_PCOUP_CHANNEL_LIST                   ${drvPlcPhy.DRV_PLC_PRIME_CHANNELS_SELECTED?string}

/* Equalization number of coefficients (number of carriers) */
#define SRV_PCOUP_EQU_NUM_COEF_CHN               97U
<#if (SRV_PCOUP_PRIME_CHANNELS_SELECTED >= 256)>
#define SRV_PCOUP_EQU_NUM_COEF_2_CHN             (SRV_PCOUP_EQU_NUM_COEF_CHN << 1)
</#if>

/* Equalization coefficients tables */
<#if (SRV_PCOUP_PRIME_CHN1 == true)>
  <#if (drvPlcPhy.DRV_PLC_MODE == "PL360") || (drvPlcPhy.DRV_PLC_COUP_PRIME_SETTING_PL460?contains("auxiliary"))>
#define SRV_PCOUP_PRED_CHN1_HIGH_TBL             {0x756E, 0x7396, 0x730A, 0x72EB, 0x72B2, 0x7433, 0x755E, 0x75D7, 0x769E, 0x76A4, 0x77C3, 0x7851, 0x7864, 0x78A0, \
                                                 0x78BA, 0x7918, 0x79B6, 0x79E9, 0x7ACC, 0x7B06, 0x7B30, 0x7B27, 0x7C1E, 0x7B96, 0x7A76, 0x7B12, 0x7AFD, 0x7C40, \
                                                 0x7C5E, 0x7B48, 0x7B8A, 0x7C64, 0x7C42, 0x7BCD, 0x7AFD, 0x7A5F, 0x7A03, 0x7A9D, 0x7A1A, 0x7A4A, 0x79FC, 0x7984, \
                                                 0x7A0D, 0x79CC, 0x792E, 0x780D, 0x7676, 0x75E4, 0x747A, 0x7251, 0x707E, 0x6E96, 0x6E30, 0x6D44, 0x6DBD, 0x6C9A, \
                                                 0x6C3C, 0x6CF8, 0x6CA4, 0x6CDF, 0x6C59, 0x6B2C, 0x6CB9, 0x6C1F, 0x6B6D, 0x6BF5, 0x6AF0, 0x6A55, 0x6955, 0x674F, \
                                                 0x6841, 0x685D, 0x670F, 0x6904, 0x6967, 0x6B01, 0x6C31, 0x6C2A, 0x6D82, 0x6F58, 0x6E62, 0x6F18, 0x6EE7, 0x7069, \
                                                 0x717B, 0x7120, 0x7170, 0x72FB, 0x7491, 0x75B3, 0x75A2, 0x7664, 0x784A, 0x7A52, 0x7B51, 0x7D5A, 0x7FFF}

#define SRV_PCOUP_PRED_CHN1_VLOW_TBL             {0x7FFF, 0x7F2B, 0x7E38, 0x7CD3, 0x7B38, 0x7972, 0x77D6, 0x7654, 0x74AE, 0x7288, 0x70C0, 0x6E9A, 0x6D24, 0x6B80, \
                                                 0x6A2F, 0x6852, 0x674E, 0x65DA, 0x652E, 0x637E, 0x6292, 0x6142, 0x60CC, 0x5FF8, 0x5F6D, 0x5EC2, 0x5E6F, 0x5E55, \
                                                 0x5E43, 0x5E02, 0x5E5B, 0x5EB3, 0x5F4A, 0x5FD7, 0x604C, 0x60FC, 0x61F3, 0x6297, 0x63A9, 0x643D, 0x654A, 0x6634, \
                                                 0x675C, 0x6824, 0x6910, 0x69A4, 0x6A73, 0x6B6F, 0x6C15, 0x6CCD, 0x6D64, 0x6E4B, 0x6ED3, 0x6F44, 0x6F85, 0x70A1, \
                                                 0x70AF, 0x71B2, 0x7149, 0x71F3, 0x7203, 0x7279, 0x71FB, 0x72B4, 0x7281, 0x72A4, 0x7262, 0x72BD, 0x7295, 0x72CC, \
                                                 0x729E, 0x7288, 0x7244, 0x7279, 0x726C, 0x7230, 0x71B9, 0x70D8, 0x7045, 0x7052, 0x6F8D, 0x6F3D, 0x6EB0, 0x6E6A, \
                                                 0x6E76, 0x6E1C, 0x6D7A, 0x6D84, 0x6D50, 0x6D45, 0x6CF2, 0x6CA9, 0x6C92, 0x6CBA, 0x6C69, 0x6C27, 0x6C02}
  <#elseif (drvPlcPhy.DRV_PLC_COUP_PRIME_SETTING_PL460?contains("Multiband single-branch"))>
#define SRV_PCOUP_PRED_CHN1_HIGH_TBL             {0x60EF, 0x5DB2, 0x5C48, 0x5B4B, 0x5BBF, 0x5B26, 0x5B7F, 0x5B27, 0x5A36, 0x599E, 0x5848, 0x5814, 0x5857, 0x585F, \
                                                 0x58D1, 0x5A03, 0x5A79, 0x5906, 0x5A2A, 0x5AB5, 0x5A7F, 0x596E, 0x5916, 0x58BB, 0x5728, 0x57E7, 0x5875, 0x59BD, \
                                                 0x5A7B, 0x5C9D, 0x5C94, 0x5C32, 0x5C21, 0x5B51, 0x5B03, 0x5B16, 0x5C0A, 0x5E2F, 0x5F02, 0x5FD1, 0x6213, 0x639F, \
                                                 0x6396, 0x6527, 0x656E, 0x63CA, 0x6293, 0x61B9, 0x61C1, 0x62BF, 0x645A, 0x661D, 0x680F, 0x6AE3, 0x6B7D, 0x6B14, \
                                                 0x6C1B, 0x6D5E, 0x6C55, 0x6C2E, 0x6BFA, 0x6B98, 0x6A1B, 0x6A4A, 0x6CBF, 0x6EF6, 0x70AE, 0x723D, 0x735D, 0x70C2, \
                                                 0x70FB, 0x6FFC, 0x70A4, 0x7021, 0x7112, 0x71BA, 0x72E9, 0x741F, 0x76E3, 0x7916, 0x78CF, 0x79B5, 0x79A0, 0x770A, \
                                                 0x7782, 0x77AA, 0x792C, 0x79EA, 0x7C4C, 0x7CD2, 0x7D99, 0x7E2F, 0x7F81, 0x7FFF, 0x7F07, 0x7F2B, 0x7C99}

#define SRV_PCOUP_PRED_CHN1_VLOW_TBL             {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}
  <#else>
#define SRV_PCOUP_PRED_CHN1_HIGH_TBL             {0x6E32, 0x6D93, 0x6DCC, 0x6EFA, 0x70DC, 0x72E7, 0x7494, 0x75BF, 0x7635, 0x7661, 0x761B, 0x7671, 0x7733, 0x781F, \
                                                 0x793F, 0x7B02, 0x7C59, 0x7CDA, 0x7D3B, 0x7D90, 0x7D1C, 0x7C7E, 0x7C40, 0x7C85, 0x7CA2, 0x7D47, 0x7E4A, 0x7F4B, \
                                                 0x7FEB, 0x7FFF, 0x7F9E, 0x7E77, 0x7D6E, 0x7C19, 0x7B96, 0x7B16, 0x7B1E, 0x7BAB, 0x7C40, 0x7CED, 0x7D72, 0x7D73, \
                                                 0x7CBA, 0x7C11, 0x7A5C, 0x793A, 0x778E, 0x764A, 0x7537, 0x7490, 0x7434, 0x7459, 0x74AF, 0x7497, 0x749E, 0x7386, \
                                                 0x7274, 0x70C1, 0x6F66, 0x6D68, 0x6C10, 0x6B43, 0x6A74, 0x697D, 0x6A03, 0x69DD, 0x690E, 0x6850, 0x6773, 0x65A8, \
                                                 0x64DD, 0x63BB, 0x6298, 0x61D6, 0x617E, 0x6140, 0x6191, 0x6200, 0x624C, 0x622A, 0x61A0, 0x60EF, 0x6036, 0x5FE3, \
                                                 0x5FDC, 0x606F, 0x6185, 0x62CF, 0x6404, 0x64E0, 0x656A, 0x6517, 0x65BC, 0x661C, 0x6811, 0x6AE5, 0x6FFB}

#define SRV_PCOUP_PRED_CHN1_VLOW_TBL             {0x7FFF, 0x7F2B, 0x7E38, 0x7CD3, 0x7B38, 0x7972, 0x77D6, 0x7654, 0x74AE, 0x7288, 0x70C0, 0x6E9A, 0x6D24, 0x6B80, \
                                                 0x6A2F, 0x6852, 0x674E, 0x65DA, 0x652E, 0x637E, 0x6292, 0x6142, 0x60CC, 0x5FF8, 0x5F6D, 0x5EC2, 0x5E6F, 0x5E55, \
                                                 0x5E43, 0x5E02, 0x5E5B, 0x5EB3, 0x5F4A, 0x5FD7, 0x604C, 0x60FC, 0x61F3, 0x6297, 0x63A9, 0x643D, 0x654A, 0x6634, \
                                                 0x675C, 0x6824, 0x6910, 0x69A4, 0x6A73, 0x6B6F, 0x6C15, 0x6CCD, 0x6D64, 0x6E4B, 0x6ED3, 0x6F44, 0x6F85, 0x70A1, \
                                                 0x70AF, 0x71B2, 0x7149, 0x71F3, 0x7203, 0x7279, 0x71FB, 0x72B4, 0x7281, 0x72A4, 0x7262, 0x72BD, 0x7295, 0x72CC, \
                                                 0x729E, 0x7288, 0x7244, 0x7279, 0x726C, 0x7230, 0x71B9, 0x70D8, 0x7045, 0x7052, 0x6F8D, 0x6F3D, 0x6EB0, 0x6E6A, \
                                                 0x6E76, 0x6E1C, 0x6D7A, 0x6D84, 0x6D50, 0x6D45, 0x6CF2, 0x6CA9, 0x6C92, 0x6CBA, 0x6C69, 0x6C27, 0x6C02}
  </#if>
</#if>
<#if ((SRV_PCOUP_PRIME_CHN2 == true) || (SRV_PCOUP_PRIME_CHN3 == true) || (SRV_PCOUP_PRIME_CHN4 == true) || (SRV_PCOUP_PRIME_CHN5 == true) || (SRV_PCOUP_PRIME_CHN6 == true) || (SRV_PCOUP_PRIME_CHN7 == true) || (SRV_PCOUP_PRIME_CHN8 == true))>
#define SRV_PCOUP_PRED_NOT_USED                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}

</#if>
<#if (SRV_PCOUP_PRIME_CHANNELS_SELECTED >= 256)>
#define SRV_PCOUP_PRED_2CHN_NOT_USED             {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}

</#if>
/* DACC configuration tables */
<#if (SRV_PCOUP_PRIME_CHN1 == true)>
#define SRV_PCOUP_DACC_CENA_TBL                  {0x${SRV_PCOUP_DACC_CENA_0?string}UL, 0x${SRV_PCOUP_DACC_CENA_1?string}UL, 0x${SRV_PCOUP_DACC_CENA_2?string}UL, 0x${SRV_PCOUP_DACC_CENA_3?string}UL, 0x${SRV_PCOUP_DACC_CENA_4?string}UL, 0x${SRV_PCOUP_DACC_CENA_5?string}UL, \
                                                 0x${SRV_PCOUP_DACC_CENA_6?string}UL, 0x${SRV_PCOUP_DACC_CENA_7?string}UL, 0x${SRV_PCOUP_DACC_CENA_8?string}UL, 0x${SRV_PCOUP_DACC_CENA_9?string}UL, 0x${SRV_PCOUP_DACC_CENA_10?string}UL, 0x${SRV_PCOUP_DACC_CENA_11?string}UL, \
                                                 0x${SRV_PCOUP_DACC_CENA_12?string}UL, 0x${SRV_PCOUP_DACC_CENA_13?string}UL, 0x${SRV_PCOUP_DACC_CENA_14?string}UL, 0x${SRV_PCOUP_DACC_CENA_15?string}UL, 0x${SRV_PCOUP_DACC_CENA_16?string}UL}

</#if>
<#if ((SRV_PCOUP_PRIME_CHN2 == true) || (SRV_PCOUP_PRIME_CHN3 == true) || (SRV_PCOUP_PRIME_CHN4 == true) || (SRV_PCOUP_PRIME_CHN5 == true) || (SRV_PCOUP_PRIME_CHN6 == true) || (SRV_PCOUP_PRIME_CHN7 == true) || (SRV_PCOUP_PRIME_CHN8 == true))>
#define SRV_PCOUP_DACC_FCC_TBL                   {0x${SRV_PCOUP_DACC_FCC_0?string}UL, 0x${SRV_PCOUP_DACC_FCC_1?string}UL, 0x${SRV_PCOUP_DACC_FCC_2?string}UL, 0x${SRV_PCOUP_DACC_FCC_3?string}UL, 0x${SRV_PCOUP_DACC_FCC_4?string}UL, 0x${SRV_PCOUP_DACC_FCC_5?string}UL, \
                                                 0x${SRV_PCOUP_DACC_FCC_6?string}UL, 0x${SRV_PCOUP_DACC_FCC_7?string}UL, 0x${SRV_PCOUP_DACC_FCC_8?string}UL, 0x${SRV_PCOUP_DACC_FCC_9?string}UL, 0x${SRV_PCOUP_DACC_FCC_10?string}UL, 0x${SRV_PCOUP_DACC_FCC_11?string}UL, \
                                                 0x${SRV_PCOUP_DACC_FCC_12?string}UL, 0x${SRV_PCOUP_DACC_FCC_13?string}UL, 0x${SRV_PCOUP_DACC_FCC_14?string}UL, 0x${SRV_PCOUP_DACC_FCC_15?string}UL, 0x${SRV_PCOUP_DACC_FCC_16?string}UL}

</#if>
<#if (SRV_PCOUP_PRIME_CHANNELS_SELECTED >= 256)>
#define SRV_PCOUP_DACC_2CHN_TBL                  {0x${SRV_PCOUP_DACC_2CHN_0?string}UL, 0x${SRV_PCOUP_DACC_2CHN_1?string}UL, 0x${SRV_PCOUP_DACC_2CHN_2?string}UL, 0x${SRV_PCOUP_DACC_2CHN_3?string}UL, 0x${SRV_PCOUP_DACC_2CHN_4?string}UL, 0x${SRV_PCOUP_DACC_2CHN_5?string}UL, \
                                                 0x${SRV_PCOUP_DACC_2CHN_6?string}UL, 0x${SRV_PCOUP_DACC_2CHN_7?string}UL, 0x${SRV_PCOUP_DACC_2CHN_8?string}UL, 0x${SRV_PCOUP_DACC_2CHN_9?string}UL, 0x${SRV_PCOUP_DACC_2CHN_10?string}UL, 0x${SRV_PCOUP_DACC_2CHN_11?string}UL, \
                                                 0x${SRV_PCOUP_DACC_2CHN_12?string}UL, 0x${SRV_PCOUP_DACC_2CHN_13?string}UL, 0x${SRV_PCOUP_DACC_2CHN_14?string}UL, 0x${SRV_PCOUP_DACC_2CHN_15?string}UL, 0x${SRV_PCOUP_DACC_2CHN_16?string}UL}

</#if>

/* PLC PHY Coupling parameters for each PRIME channel */
<#if (SRV_PCOUP_PRIME_CHN1 == true)>
#define SRV_PCOUP_CHN1_RMS_HIGH_TBL              {${SRV_PCOUP_PRIME_CHN1_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN1_RMS_VLOW_TBL              {${SRV_PCOUP_PRIME_CHN1_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN1_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN1_THRS_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN1_THRS_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_CHN1_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN1_GAIN_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN1_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN1_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN1_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN1_GAIN_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN1_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN1_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN1_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN1_LINE_DRV_CONF             ${SRV_PCOUP_PRIME_CHN1_LINE_DRIVER?string}
#define SRV_PCOUP_CHN1_MAX_NUM_TX_LEVELS         ${SRV_PCOUP_PRIME_CHN1_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_CHN2 == true)>
#define SRV_PCOUP_CHN2_RMS_HIGH_TBL              {${SRV_PCOUP_PRIME_CHN2_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN2_RMS_VLOW_TBL              {${SRV_PCOUP_PRIME_CHN2_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN2_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN2_THRS_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN2_THRS_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_CHN2_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN2_GAIN_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN2_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN2_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN2_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN2_GAIN_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN2_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN2_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN2_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN2_LINE_DRV_CONF             ${SRV_PCOUP_PRIME_CHN2_LINE_DRIVER?string}
#define SRV_PCOUP_CHN2_MAX_NUM_TX_LEVELS         ${SRV_PCOUP_PRIME_CHN2_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_CHN3 == true)>
#define SRV_PCOUP_CHN3_RMS_HIGH_TBL              {${SRV_PCOUP_PRIME_CHN3_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN3_RMS_VLOW_TBL              {${SRV_PCOUP_PRIME_CHN3_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN3_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN3_THRS_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN3_THRS_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_CHN3_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN3_GAIN_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN3_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN3_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN3_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN3_GAIN_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN3_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN3_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN3_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN3_LINE_DRV_CONF             ${SRV_PCOUP_PRIME_CHN3_LINE_DRIVER?string}
#define SRV_PCOUP_CHN3_MAX_NUM_TX_LEVELS         ${SRV_PCOUP_PRIME_CHN3_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_CHN4 == true)>
#define SRV_PCOUP_CHN4_RMS_HIGH_TBL              {${SRV_PCOUP_PRIME_CHN4_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN4_RMS_VLOW_TBL              {${SRV_PCOUP_PRIME_CHN4_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN4_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN4_THRS_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN4_THRS_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_CHN4_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN4_GAIN_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN4_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN4_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN4_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN4_GAIN_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN4_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN4_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN4_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN4_LINE_DRV_CONF             ${SRV_PCOUP_PRIME_CHN4_LINE_DRIVER?string}
#define SRV_PCOUP_CHN4_MAX_NUM_TX_LEVELS         ${SRV_PCOUP_PRIME_CHN4_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_CHN5 == true)>
#define SRV_PCOUP_CHN5_RMS_HIGH_TBL              {${SRV_PCOUP_PRIME_CHN5_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN5_RMS_VLOW_TBL              {${SRV_PCOUP_PRIME_CHN5_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN5_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN5_THRS_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN5_THRS_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_CHN5_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN5_GAIN_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN5_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN5_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN5_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN5_GAIN_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN5_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN5_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN5_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN5_LINE_DRV_CONF             ${SRV_PCOUP_PRIME_CHN5_LINE_DRIVER?string}
#define SRV_PCOUP_CHN5_MAX_NUM_TX_LEVELS         ${SRV_PCOUP_PRIME_CHN5_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_CHN6 == true)>
#define SRV_PCOUP_CHN6_RMS_HIGH_TBL              {${SRV_PCOUP_PRIME_CHN6_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN6_RMS_VLOW_TBL              {${SRV_PCOUP_PRIME_CHN6_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN6_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN6_THRS_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN6_THRS_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_CHN6_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN6_GAIN_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN6_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN6_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN6_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN6_GAIN_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN6_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN6_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN6_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN6_LINE_DRV_CONF             ${SRV_PCOUP_PRIME_CHN6_LINE_DRIVER?string}
#define SRV_PCOUP_CHN6_MAX_NUM_TX_LEVELS         ${SRV_PCOUP_PRIME_CHN6_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_CHN7 == true)>
#define SRV_PCOUP_CHN7_RMS_HIGH_TBL              {${SRV_PCOUP_PRIME_CHN7_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN7_RMS_VLOW_TBL              {${SRV_PCOUP_PRIME_CHN7_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN7_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN7_THRS_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN7_THRS_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_CHN7_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN7_GAIN_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN7_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN7_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN7_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN7_GAIN_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN7_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN7_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN7_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN7_LINE_DRV_CONF             ${SRV_PCOUP_PRIME_CHN7_LINE_DRIVER?string}
#define SRV_PCOUP_CHN7_MAX_NUM_TX_LEVELS         ${SRV_PCOUP_PRIME_CHN7_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_CHN8 == true)>
#define SRV_PCOUP_CHN8_RMS_HIGH_TBL              {${SRV_PCOUP_PRIME_CHN8_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN8_RMS_VLOW_TBL              {${SRV_PCOUP_PRIME_CHN8_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN8_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN8_THRS_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN8_THRS_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_CHN8_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN8_GAIN_HIGH_TBL             {${SRV_PCOUP_PRIME_CHN8_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_CHN8_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_CHN8_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN8_GAIN_VLOW_TBL             {${SRV_PCOUP_PRIME_CHN8_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_CHN8_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_CHN8_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN8_LINE_DRV_CONF             ${SRV_PCOUP_PRIME_CHN8_LINE_DRIVER?string}
#define SRV_PCOUP_CHN8_MAX_NUM_TX_LEVELS         ${SRV_PCOUP_PRIME_CHN8_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_2CHN12 == true)>
#define SRV_PCOUP_CHN12_RMS_HIGH_TBL             {${SRV_PCOUP_PRIME_2CHN12_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN12_RMS_VLOW_TBL             {${SRV_PCOUP_PRIME_2CHN12_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN12_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN12_THRS_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN12_THRS_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_2CHN12_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN12_GAIN_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN12_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN12_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN12_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN12_GAIN_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN12_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN12_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN12_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN12_LINE_DRV_CONF            ${SRV_PCOUP_PRIME_2CHN12_LINE_DRIVER?string}
#define SRV_PCOUP_CHN12_MAX_NUM_TX_LEVELS        ${SRV_PCOUP_PRIME_2CHN12_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_2CHN23 == true)>
#define SRV_PCOUP_CHN23_RMS_HIGH_TBL             {${SRV_PCOUP_PRIME_2CHN23_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN23_RMS_VLOW_TBL             {${SRV_PCOUP_PRIME_2CHN23_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN23_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN23_THRS_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN23_THRS_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_2CHN23_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN23_GAIN_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN23_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN23_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN23_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN23_GAIN_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN23_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN23_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN23_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN23_LINE_DRV_CONF            ${SRV_PCOUP_PRIME_2CHN23_LINE_DRIVER?string}
#define SRV_PCOUP_CHN23_MAX_NUM_TX_LEVELS        ${SRV_PCOUP_PRIME_2CHN23_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_2CHN34 == true)>
#define SRV_PCOUP_CHN34_RMS_HIGH_TBL             {${SRV_PCOUP_PRIME_2CHN34_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN34_RMS_VLOW_TBL             {${SRV_PCOUP_PRIME_2CHN34_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN34_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN34_THRS_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN34_THRS_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_2CHN34_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN34_GAIN_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN34_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN34_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN34_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN34_GAIN_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN34_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN34_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN34_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN34_LINE_DRV_CONF            ${SRV_PCOUP_PRIME_2CHN34_LINE_DRIVER?string}
#define SRV_PCOUP_CHN34_MAX_NUM_TX_LEVELS        ${SRV_PCOUP_PRIME_2CHN34_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_2CHN45 == true)>
#define SRV_PCOUP_CHN45_RMS_HIGH_TBL             {${SRV_PCOUP_PRIME_2CHN45_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN45_RMS_VLOW_TBL             {${SRV_PCOUP_PRIME_2CHN45_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN45_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN45_THRS_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN45_THRS_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_2CHN45_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN45_GAIN_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN45_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN45_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN45_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN45_GAIN_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN45_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN45_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN45_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN45_LINE_DRV_CONF            ${SRV_PCOUP_PRIME_2CHN45_LINE_DRIVER?string}
#define SRV_PCOUP_CHN45_MAX_NUM_TX_LEVELS        ${SRV_PCOUP_PRIME_2CHN45_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_2CHN56 == true)>
#define SRV_PCOUP_CHN56_RMS_HIGH_TBL             {${SRV_PCOUP_PRIME_2CHN56_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN56_RMS_VLOW_TBL             {${SRV_PCOUP_PRIME_2CHN56_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN56_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN56_THRS_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN56_THRS_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_2CHN56_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN56_GAIN_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN56_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN56_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN56_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN56_GAIN_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN56_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN56_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN56_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN56_LINE_DRV_CONF            ${SRV_PCOUP_PRIME_2CHN56_LINE_DRIVER?string}
#define SRV_PCOUP_CHN56_MAX_NUM_TX_LEVELS        ${SRV_PCOUP_PRIME_2CHN56_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_2CHN67 == true)>
#define SRV_PCOUP_CHN67_RMS_HIGH_TBL             {${SRV_PCOUP_PRIME_2CHN67_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN67_RMS_VLOW_TBL             {${SRV_PCOUP_PRIME_2CHN67_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN67_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN67_THRS_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN67_THRS_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_2CHN67_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN67_GAIN_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN67_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN67_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN67_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN67_GAIN_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN67_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN67_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN67_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN67_LINE_DRV_CONF            ${SRV_PCOUP_PRIME_2CHN67_LINE_DRIVER?string}
#define SRV_PCOUP_CHN67_MAX_NUM_TX_LEVELS        ${SRV_PCOUP_PRIME_2CHN67_NUM_TX_LVL?string}

</#if>
<#if (SRV_PCOUP_PRIME_2CHN78 == true)>
#define SRV_PCOUP_CHN78_RMS_HIGH_TBL             {${SRV_PCOUP_PRIME_2CHN78_RMS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_HIGH_7?string}}
#define SRV_PCOUP_CHN78_RMS_VLOW_TBL             {${SRV_PCOUP_PRIME_2CHN78_RMS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN78_RMS_VLOW_7?string}}
#define SRV_PCOUP_CHN78_THRS_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_2?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_3?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_4?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_5?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_6?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_7?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_8?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_9?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_10?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_11?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_12?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_13?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_14?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_HIGH_15?string}}
#define SRV_PCOUP_CHN78_THRS_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_2?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_3?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_4?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_5?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_6?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_7?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_8?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_9?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_10?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_11?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_12?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_13?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_14?string}, ${SRV_PCOUP_PRIME_2CHN78_THRS_VLOW_15?string}}
#define SRV_PCOUP_CHN78_GAIN_HIGH_TBL            {${SRV_PCOUP_PRIME_2CHN78_GAIN_HIGH_0?string}, ${SRV_PCOUP_PRIME_2CHN78_GAIN_HIGH_1?string}, ${SRV_PCOUP_PRIME_2CHN78_GAIN_HIGH_2?string}}
#define SRV_PCOUP_CHN78_GAIN_VLOW_TBL            {${SRV_PCOUP_PRIME_2CHN78_GAIN_VLOW_0?string}, ${SRV_PCOUP_PRIME_2CHN78_GAIN_VLOW_1?string}, ${SRV_PCOUP_PRIME_2CHN78_GAIN_VLOW_2?string}}
#define SRV_PCOUP_CHN78_LINE_DRV_CONF            ${SRV_PCOUP_PRIME_2CHN78_LINE_DRIVER?string}
#define SRV_PCOUP_CHN78_MAX_NUM_TX_LEVELS        ${SRV_PCOUP_PRIME_2CHN78_NUM_TX_LVL?string}

</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

/* PLC PHY Coupling data

  Summary:
    PLC PHY Coupling data.

  Description:
    This structure contains all the data required to set the PLC PHY Coupling
    parameters, for a specific PRIME channel.

  Remarks:
    Equalization coefficients and DACC table are not stored in the structure,
    just pointers to arrays were they are actually stored. This allows to use
    the same type for different PRIME channels.
*/

typedef struct
{
    /* Target RMS values in HIGH mode for dynamic Tx gain */
    uint32_t rmsHigh[8];

    /* Target RMS values in VLOW mode for dynamic Tx gain */
    uint32_t rmsVLow[8];

    /* Threshold RMS values in HIGH mode for dynamic Tx mode */
    uint32_t thrsHigh[16];

    /* Threshold RMS values in VLOW mode for dynamic Tx mode */
    uint32_t thrsVLow[16];

    /* Pointer to values for configuration of PLC DACC peripheral, according to
       hardware coupling design and PLC device (PL360/PL460) */
    const uint32_t * daccTable;

    /* Pointer to Tx equalization coefficients table in HIGH mode.
       There is one coefficient for each carrier in the used band */
    const uint16_t * equHigh;

    /* Pointer to Tx equalization coefficients table in VLOW mode.
       There is one coefficient for each carrier in the used band */
    const uint16_t * equVlow;

    /* Tx gain values for HIGH mode [HIGH_INI, HIGH_MIN, HIGH_MAX] */
    uint16_t gainHigh[3];

    /* Tx gain values for VLOW mode [VLOW_INI, VLOW_MIN, VLOW_MAX] */
    uint16_t gainVLow[3];

    /* Number of Tx attenuation levels (1 dB step) supporting dynamic Tx mode */
    uint8_t numTxLevels;

    /* Configuration of the PLC Tx Line Driver, according to hardware coupling
       design and PLC device (PL360/PL460) */
    uint8_t lineDrvConf;

} SRV_PLC_PCOUP_CHANNEL_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Service Interface Functions
// *****************************************************************************
// *****************************************************************************

/***************************************************************************
  Function:
    DRV_PLC_PHY_CHANNEL SRV_PCOUP_GetDefaultChannel(void)

  Summary:
    Get the default PRIME channel.

  Description:
    This function allows to get the PRIME channel used by default.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Default PRIME channel.

  Example:
    <code>
    DRV_PLC_PHY_CHANNEL plcDefaultChannel;
    DRV_PLC_PHY_PIB_OBJ pibObj;

    plcDefaultChannel = SRV_PCOUP_GetDefaultChannel();

    pibObj.id = PLC_ID_CHANNEL_CFG;
    pibObj.length = 1;
    pibObj.pData = &plcDefaultChannel;
    DRV_PLC_PHY_PIBSet(handle, &pibObj);

    SRV_PCOUP_SetChannelConfig(handle, plcDefaultChannel);
    </code>

  Remarks:
    None.
  ***************************************************************************/

DRV_PLC_PHY_CHANNEL SRV_PCOUP_GetDefaultChannel( void );

/***************************************************************************
  Function:
    SRV_PLC_PCOUP_CHANNEL_DATA * SRV_PCOUP_GetChannelConfig (DRV_PLC_PHY_CHANNEL channel)

  Summary:
    Get the PLC PHY Coupling parameters for the specified PRIME channel.

  Description:
    This function allows to get the PLC PHY Coupling parameters for the
    specified PRIME channel. These parameters can be sent to the PLC device
    through PLC Driver PIB interface (DRV_PLC_PHY_PIBSet).

  Precondition:
    None.

  Parameters:
    channel   - PRIME channel for which the parameters are requested

  Returns:
    - Pointer to PLC PHY Coupling parameters
      - if channel parameter is valid
    - *NULL*
      - if channel parameter is not valid

  Example:
    <code>
    SRV_PLC_PCOUP_CHANNEL_DATA *pCoupChannelData;

    pCoupChannelData = SRV_PCOUP_GetChannelConfig
(SRV_PCOUP_DEFAULT_CHANNEL);
    </code>

  Remarks:
    If SRV_PCOUP_SetChannelConfig is used to set the PLC PHY Coupling
    parameters, this function is not needed.
  ***************************************************************************/

SRV_PLC_PCOUP_CHANNEL_DATA * SRV_PCOUP_GetChannelConfig (DRV_PLC_PHY_CHANNEL channel);

/***************************************************************************
  Function:
    bool SRV_PCOUP_SetChannelConfig(DRV_HANDLE handle, DRV_PLC_PHY_CHANNEL channel);

  Summary:
    Set the PLC PHY Coupling parameters for the specified PRIME channel.

  Description:
    This function allows to set the PLC PHY Coupling parameters for the
    specified PRIME channel, using the PLC Driver PIB interface
    (DRV_PLC_PHY_PIBSet).

  Precondition:
    DRV_PLC_PHY_Open must have been called to obtain a valid opened device
    handle.

  Parameters:
    handle   - A valid instance handle, returned from DRV_PLC_PHY_Open
    channel  - PRIME channel for which the parameters will be set

  Returns:
    - true
      - Successful configuration
    - false
      - if channel parameter is not valid
      - if there is an error when using the PLC Driver PIB interface

  Example:
    <code>
    bool result;

    result = SRV_PCOUP_SetChannelConfig(handle, CHN5);
    </code>

  Remarks:
    None.
  ***************************************************************************/

bool SRV_PCOUP_SetChannelConfig(DRV_HANDLE handle, DRV_PLC_PHY_CHANNEL channel);

/***************************************************************************
  Function:
    uint16_t SRV_PCOUP_GetChannelList(void)

  Summary:
    Get the PRIME channel list.

  Description:
    This function allows to get the PRIME channel list.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    PRIME channel list. The channel list is a bitmask corresponding to the 
    following values.
    Single channel:
      Channel 1 : bit 0
      Channel 2 : bit 1
      Channel 3 : bit 2
      Channel 4 : bit 3
      Channel 5 : bit 4
      Channel 6 : bit 5
      Channel 7 : bit 6
      Channel 8 : bit 7
    In double channel:
      Channel 1-2 : bit 8
      Channel 2-3 : bit 9
      Channel 3-4 : bit 10
      Channel 4-5 : bit 11
      Channel 5-6 : bit 12
      Channel 6-7 : bit 13
      Channel 7-8 : bit 14

  Example:
    <code>
    uint16_t plcChannelList;

    plcChannelList = SRV_PCOUP_GetChannelList();

    </code>

  Remarks:
    None.
  ***************************************************************************/

uint16_t SRV_PCOUP_GetChannelList(void);

DRV_PLC_PHY_CHANNEL SRV_PCOUP_GetChannelImpedanceDetection(void);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif //SRV_PCOUP_H
