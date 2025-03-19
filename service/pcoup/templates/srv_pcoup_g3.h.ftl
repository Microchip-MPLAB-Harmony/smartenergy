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
<#if (drvPlcPhy)??>
#include "driver/plc/phy/drv_plc_phy.h"
<#assign SRV_PCOUP_COMM_H = "drv_plc_phy_comm.h">
<#assign SRV_PCOUP_PIB_SET = "DRV_PLC_PHY_PIBSet">
<#assign SRV_PCOUP_DRV_OPEN = "DRV_PLC_PHY_Open">
<#assign SRV_PCOUP_PLC_MODE = drvPlcPhy.DRV_PLC_MODE>
<#assign SRV_PCOUP_PLC_COUP_SETTINGS_PL460 = drvPlcPhy.DRV_PLC_COUP_G3_SETTING_PL460>
<#assign SRV_PCOUP_PLC_COUP_SETTINGS_PL360 = drvPlcPhy.DRV_PLC_COUP_G3_SETTING_PL360>
<#assign SRV_PCOUP_DEFAULT_G3_BAND_CENA = drvPlcPhy.DRV_PLC_COUP_DEFAULT_G3_BAND_CENA>
<#assign SRV_PCOUP_DEFAULT_G3_BAND_CENB = drvPlcPhy.DRV_PLC_COUP_DEFAULT_G3_BAND_CENB>
<#elseif (drvG3MacRt)??>
#include "driver/plc/g3MacRt/drv_g3_macrt.h"
<#assign SRV_PCOUP_COMM_H = "drv_g3_macrt_comm.h">
<#assign SRV_PCOUP_PIB_SET = "DRV_G3_MACRT_PIBSet">
<#assign SRV_PCOUP_DRV_OPEN = "DRV_G3_MACRT_Open">
<#assign SRV_PCOUP_PLC_MODE = drvG3MacRt.DRV_PLC_MODE>
<#assign SRV_PCOUP_PLC_COUP_SETTINGS_PL460 = drvG3MacRt.DRV_PLC_COUP_G3_SETTING_PL460>
<#assign SRV_PCOUP_PLC_COUP_SETTINGS_PL360 = drvG3MacRt.DRV_PLC_COUP_G3_SETTING_PL360>
<#assign SRV_PCOUP_DEFAULT_G3_BAND_CENA = drvG3MacRt.DRV_PLC_COUP_DEFAULT_G3_BAND_CENA>
<#assign SRV_PCOUP_DEFAULT_G3_BAND_CENB = drvG3MacRt.DRV_PLC_COUP_DEFAULT_G3_BAND_CENB>
</#if>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

/* Default G3-PLC PHY band of the PLC transmission coupling */
<#if SRV_PCOUP_G3_AUX_BAND == "None">
  <#if SRV_PCOUP_G3_MAIN_BAND == "CEN-A">
#define SRV_PCOUP_DEFAULT_BAND                   G3_CEN_A
  <#elseif SRV_PCOUP_G3_MAIN_BAND == "CEN-B">
#define SRV_PCOUP_DEFAULT_BAND                   G3_CEN_B
  <#elseif SRV_PCOUP_G3_MAIN_BAND == "FCC">
#define SRV_PCOUP_DEFAULT_BAND                   G3_FCC
  <#elseif SRV_PCOUP_G3_MAIN_BAND == "ARIB">
#define SRV_PCOUP_DEFAULT_BAND                   G3_ARIB
  </#if>
<#elseif SRV_PCOUP_G3_MAIN_BAND == "None">
  <#if SRV_PCOUP_G3_AUX_BAND == "CEN-A">
#define SRV_PCOUP_DEFAULT_BAND                   G3_CEN_A
  <#elseif SRV_PCOUP_G3_AUX_BAND == "CEN-B">
#define SRV_PCOUP_DEFAULT_BAND                   G3_CEN_B
  <#elseif SRV_PCOUP_G3_AUX_BAND == "FCC">
#define SRV_PCOUP_DEFAULT_BAND                   G3_FCC
  <#elseif SRV_PCOUP_G3_AUX_BAND == "ARIB">
#define SRV_PCOUP_DEFAULT_BAND                   G3_ARIB
  </#if>
<#else>
  <#if (SRV_PCOUP_G3_MAIN_BAND == "CEN-A") || (SRV_PCOUP_G3_AUX_BAND == "CEN-A")>
    <#if (SRV_PCOUP_DEFAULT_G3_BAND_CENA == "CENELEC-A")>
#define SRV_PCOUP_DEFAULT_BAND                   G3_CEN_A
    <#elseif (SRV_PCOUP_G3_MAIN_BAND == "FCC") || (SRV_PCOUP_G3_AUX_BAND == "FCC")>
#define SRV_PCOUP_DEFAULT_BAND                   G3_FCC
    <#else>
#define SRV_PCOUP_DEFAULT_BAND                   G3_ARIB
    </#if>
  <#else>
    <#if (SRV_PCOUP_DEFAULT_G3_BAND_CENB == "CENELEC-B")>
#define SRV_PCOUP_DEFAULT_BAND                   G3_CEN_B
    <#elseif (SRV_PCOUP_G3_MAIN_BAND == "FCC") || (SRV_PCOUP_G3_AUX_BAND == "FCC")>
#define SRV_PCOUP_DEFAULT_BAND                   G3_FCC
    <#else>
#define SRV_PCOUP_DEFAULT_BAND                   G3_ARIB
    </#if>
  </#if>
</#if>

<#if SRV_PCOUP_G3_MAIN_BAND != "None">
/* Equalization number of coefficients (number of carriers) for Main branch */
  <#if SRV_PCOUP_G3_MAIN_BAND == "CEN-A">
#define SRV_PCOUP_EQU_NUM_COEF                   36U
  <#elseif SRV_PCOUP_G3_MAIN_BAND == "CEN-B">
#define SRV_PCOUP_EQU_NUM_COEF                   16U
  <#elseif SRV_PCOUP_G3_MAIN_BAND == "FCC">
#define SRV_PCOUP_EQU_NUM_COEF                   72U
  <#elseif SRV_PCOUP_G3_MAIN_BAND == "ARIB">
#define SRV_PCOUP_EQU_NUM_COEF                   54U
  </#if>

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

  <#if (SRV_PCOUP_PLC_MODE == "PL460")>
    <#if (SRV_PCOUP_PLC_COUP_SETTINGS_PL460 == "CEN-A (CENELEC-A only; main branch)")>
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x5620, 0x59C7, 0x5E1E, 0x6333, 0x698B, 0x6F03, 0x72CD, 0x760E, 0x7904, 0x7B57, 0x7D2C, 0x7E72, 0x7F0F, 0x7FC6, \
                                                  0x7FFF, 0x7ED1, 0x7D11, 0x7BCE, 0x7A1A, 0x777C, 0x7496, 0x720F, 0x6F8E, 0x6BE0, 0x6780, 0x6357, 0x5F5E, 0x5C0C, \
                                                  0x597B, 0x5782, 0x572D, 0x57A2, 0x5823, 0x59F2, 0x5D86, 0x6153}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7934, 0x71F2, 0x6C8A, 0x694E, 0x6481, 0x5F97, 0x5A9E, 0x5487, 0x4F83, 0x4BF5, 0x495C, 0x4777, 0x46B8,  \
                                                  0x4634, 0x45B7, 0x45C1, 0x46FC, 0x4890, 0x4AC5, 0x4CEE, 0x4E87, 0x4F98, 0x50BC, 0x5111, 0x5244, 0x53A1, 0x55A0,  \
                                                  0x5661, 0x56EE, 0x5748, 0x5880, 0x5944, 0x5C03, 0x5F63, 0x630B}
    <#elseif (SRV_PCOUP_PLC_COUP_SETTINGS_PL460 == "CEN-B (CENELEC-B only; main branch)")>
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}
    <#elseif SRV_PCOUP_PLC_COUP_SETTINGS_PL460?contains("high attenuation")>
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x43D3, 0x3E8A, 0x3984, 0x3430, 0x2EB8, 0x2A49, 0x25EB, 0x21D0, 0x1E56, 0x1B9E, 0x1965, 0x179F, 0x165E, 0x15B9,  \
                                                  0x1584, 0x156E, 0x1591, 0x1608, 0x168B, 0x16FB, 0x176C, 0x1809, 0x18A4, 0x1911, 0x1949, 0x19AB, 0x1A04, 0x1A30,  \
                                                  0x1A2B, 0x1A45, 0x1A74, 0x1A83, 0x1A52, 0x1A5E, 0x1AB1, 0x1AF1, 0x1B21, 0x1B93, 0x1C80, 0x1D38, 0x1E22, 0x1F24,  \
                                                  0x20B7, 0x21FE, 0x2389, 0x2513, 0x2748, 0x2941, 0x2B48, 0x2DA0, 0x3034, 0x3306, 0x358A, 0x381E, 0x3B10, 0x3E0D,  \
                                                  0x409A, 0x4374, 0x46F4, 0x4AA9, 0x4DFF, 0x50B7, 0x54B8, 0x5893, 0x5B77, 0x5EF5, 0x63D3, 0x67EC, 0x6BA4, 0x70DC,  \
                                                  0x7808, 0x7FFF}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7648, 0x6EDE, 0x6945, 0x653C, 0x61FE, 0x5FD1, 0x5D7B, 0x5BE4, 0x59D0, 0x57DC, 0x5462, 0x50FF, 0x4E28,  \
                                                  0x4BDC, 0x49BE, 0x47FA, 0x46CC, 0x45A4, 0x443E, 0x41FF, 0x410D, 0x40C1, 0x409A, 0x40C5, 0x4163, 0x4223, 0x42D0,  \
                                                  0x42E5, 0x42F8, 0x4434, 0x4506, 0x4529, 0x46A6, 0x47D6, 0x48F0, 0x4930, 0x4AAB, 0x4B26, 0x4C58, 0x4C39, 0x4D1B,  \
                                                  0x4DD6, 0x4EBD, 0x4F0D, 0x5124, 0x5193, 0x51F0, 0x5204, 0x5258, 0x5299, 0x5451, 0x551A, 0x54ED, 0x55BE, 0x55BB,  \
                                                  0x5608, 0x5650, 0x57A1, 0x5990, 0x5AB7, 0x5B6D, 0x5CFE, 0x5DE5, 0x5D88, 0x5E2D, 0x5ED5, 0x5F73, 0x60F9, 0x625E,  \
                                                  0x6334, 0x6483}
    <#elseif SRV_PCOUP_PLC_COUP_SETTINGS_PL460?contains("FCC default")>
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x7FFF, 0x7519, 0x6C54, 0x6476, 0x5E1A, 0x58E0, 0x5539, 0x51FC, 0x4FFA, 0x4EFA, 0x4F08, 0x4F36, 0x5006, 0x5182,  \
                                                  0x536B, 0x5503, 0x5674, 0x586A, 0x5A42, 0x5BB6, 0x5CC7, 0x5E5C, 0x5FEB, 0x610F, 0x61EF, 0x6356, 0x64E5, 0x65C1,  \
                                                  0x6664, 0x6721, 0x6881, 0x6886, 0x6924, 0x693D, 0x6A8F, 0x6A3C, 0x6AC5, 0x6A68, 0x6B60, 0x6B17, 0x6B3C, 0x6AEF,  \
                                                  0x6B6A, 0x6B46, 0x6AE5, 0x6A38, 0x6A61, 0x69FF, 0x6976, 0x6844, 0x681A, 0x67A6, 0x66B6, 0x6609, 0x65B8, 0x653D,  \
                                                  0x63FE, 0x62A1, 0x61C5, 0x614F, 0x6029, 0x5F81, 0x5FA3, 0x5F3D, 0x5EB8, 0x5E23, 0x5DC9, 0x5D56, 0x5CD2, 0x5CC2,  \
                                                  0x5D78, 0x5E49}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7666, 0x6ED8, 0x6939, 0x650D, 0x6178, 0x5F4B, 0x5D03, 0x5B7B, 0x593E, 0x5784, 0x5465, 0x515B, 0x4E98,  \
                                                  0x4CDB, 0x4B46, 0x49F2, 0x495C, 0x48D0, 0x47B9, 0x459C, 0x44E1, 0x449D, 0x4487, 0x448A, 0x452A, 0x45B4, 0x462B,  \
                                                  0x45DA, 0x45CE, 0x4699, 0x46F5, 0x46B9, 0x47D9, 0x488E, 0x495C, 0x497C, 0x4AAF, 0x4AD6, 0x4BA0, 0x4B14, 0x4B7E,  \
                                                  0x4BA5, 0x4C44, 0x4C2A, 0x4DEF, 0x4DE4, 0x4E0F, 0x4DC3, 0x4DAB, 0x4D8B, 0x4EA7, 0x4F17, 0x4E98, 0x4F3C, 0x4EF0,  \
                                                  0x4EFA, 0x4EEB, 0x4FC6, 0x5109, 0x51A2, 0x51FC, 0x52D0, 0x535B, 0x52E4, 0x530D, 0x534B, 0x539D, 0x5468, 0x5515,  \
                                                  0x5577, 0x5629}
    <#elseif SRV_PCOUP_PLC_COUP_SETTINGS_PL460?contains("Multiband single-branch FCC & CEN-A")>
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x51E6, 0x540D, 0x571D, 0x5AB1, 0x5F10, 0x641F, 0x6869, 0x6B95, 0x6E6D, 0x71A7, 0x73E5, 0x754E, 0x76C6, 0x78DE, \
                                                  0x7AC7, 0x7BAD, 0x7C20, 0x7D77, 0x7EC9, 0x7EB6, 0x7E86, 0x7F33, 0x7FFF, 0x7FBC, 0x7F18, 0x7F11, 0x7FB6, 0x7ED9, \
                                                  0x7DE6, 0x7D4D, 0x7D57, 0x7C18, 0x7B01, 0x79E0, 0x799F, 0x78A5, 0x7718, 0x761C, 0x75B6, 0x74B1, 0x7357, 0x72B6, \
                                                  0x7217, 0x712D, 0x6F72, 0x6DFF, 0x6D17, 0x6C26, 0x6A8C, 0x68F1, 0x6793, 0x6626, 0x6496, 0x63E1, 0x6487, 0x652E, \
                                                  0x64AF, 0x63A8, 0x62FB, 0x624E, 0x619B, 0x620D, 0x63B2, 0x65B2, 0x676C, 0x689D, 0x6A71, 0x6C46, 0x6C87, 0x6D94, \
                                                  0x7056, 0x733C}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7A34, 0x75AF, 0x7282, 0x705A, 0x6EA9, 0x6DCF, 0x6CEA, 0x6C5B, 0x6B5D, 0x6A84, 0x6875, 0x665B, 0x647E, \
                                                  0x6331, 0x61FE, 0x60EE, 0x607A, 0x5FF4, 0x5F11, 0x5D42, 0x5CAD, 0x5C5A, 0x5C33, 0x5C26, 0x5C74, 0x5CCE, 0x5D11, \
                                                  0x5CA2, 0x5C51, 0x5D0A, 0x5D0C, 0x5CB4, 0x5D86, 0x5E05, 0x5E41, 0x5E2C, 0x5EE1, 0x5EDA, 0x5F47, 0x5EF8, 0x5F56, \
                                                  0x5F98, 0x6001, 0x6011, 0x6165, 0x617D, 0x61A4, 0x6194, 0x61B4, 0x61AC, 0x62CA, 0x6337, 0x6303, 0x6382, 0x6354, \
                                                  0x6353, 0x6345, 0x63D2, 0x64C9, 0x6545, 0x6556, 0x6638, 0x66BD, 0x6655, 0x66A5, 0x6728, 0x675D, 0x682F, 0x6934, \
                                                  0x6A26, 0x6B83}
    <#elseif SRV_PCOUP_G3_MAIN_BAND == "ARIB">
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}
    </#if>
  <#else>
    <#if SRV_PCOUP_G3_MAIN_BAND == "CEN-A">
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x670A, 0x660F, 0x676A, 0x6A6B, 0x6F3F, 0x7440, 0x74ED, 0x7792, 0x762D, 0x7530, 0x7938, 0x7C0A, 0x7C2A, 0x7B0E, \
                                                  0x7AF2, 0x784B, 0x7899, 0x76F9, 0x76D6, 0x769F, 0x775D, 0x70C0, 0x6EB9, 0x6F18, 0x6F1E, 0x6FA2, 0x6862, 0x67C9, \
                                                  0x68F9, 0x68A5, 0x6CA3, 0x7153, 0x7533, 0x750B, 0x7B59, 0x7FFF}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7DB1, 0x7CE6, 0x7B36, 0x772F, 0x7472, 0x70AA, 0x6BC2, 0x682D, 0x6618, 0x6384, 0x6210, 0x61D7, 0x6244, \
                                                  0x6269, 0x63A8, 0x6528, 0x65CC, 0x67F6, 0x693B, 0x6B13, 0x6C29, 0x6D43, 0x6E26, 0x6D70, 0x6C94, 0x6BB5, 0x6AC9, \
                                                  0x6A5F, 0x6B65, 0x6B8C, 0x6A62, 0x6CEC, 0x6D5A, 0x6F9D, 0x6FD3}
    <#elseif SRV_PCOUP_G3_MAIN_BAND == "CEN-B">
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}
    <#elseif SRV_PCOUP_G3_MAIN_BAND == "FCC">
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}
    <#elseif SRV_PCOUP_G3_MAIN_BAND == "ARIB">
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}
    </#if>
  </#if>
</#if>

<#if SRV_PCOUP_G3_AUX_BAND != "None">
/* Equalization number of coefficients (number of carriers) for Auxiliary branch */
  <#if SRV_PCOUP_G3_AUX_BAND == "CEN-A">
#define SRV_PCOUP_AUX_EQU_NUM_COEF               36U
  <#elseif SRV_PCOUP_G3_AUX_BAND == "CEN-B">
#define SRV_PCOUP_AUX_EQU_NUM_COEF               16U
  <#elseif SRV_PCOUP_G3_AUX_BAND == "FCC">
#define SRV_PCOUP_AUX_EQU_NUM_COEF               72U
  <#elseif SRV_PCOUP_G3_AUX_BAND == "ARIB">
#define SRV_PCOUP_AUX_EQU_NUM_COEF               54U
  </#if>

/* PLC PHY Coupling parameters for Auxiliary branch */
#define SRV_PCOUP_AUX_RMS_HIGH_TBL               {${SRV_PCOUP_G3_AUX_RMS_HIGH_0?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_1?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_2?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_3?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_4?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_5?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_6?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_7?string}}
#define SRV_PCOUP_AUX_RMS_VLOW_TBL               {${SRV_PCOUP_G3_AUX_RMS_VLOW_0?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_1?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_2?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_3?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_4?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_5?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_6?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_7?string}}
#define SRV_PCOUP_AUX_THRS_HIGH_TBL              {${SRV_PCOUP_G3_AUX_THRS_HIGH_0?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_1?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_2?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_3?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_4?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_5?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_6?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_7?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_8?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_9?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_10?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_11?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_12?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_13?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_14?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_15?string}}
#define SRV_PCOUP_AUX_THRS_VLOW_TBL              {${SRV_PCOUP_G3_AUX_THRS_VLOW_0?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_1?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_2?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_3?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_4?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_5?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_6?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_7?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_8?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_9?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_10?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_11?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_12?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_13?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_14?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_15?string}}
#define SRV_PCOUP_AUX_DACC_TBL                   {0x${SRV_PCOUP_G3_AUX_DACC_0?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_1?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_2?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_3?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_4?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_5?string}UL, \
                                                 0x${SRV_PCOUP_G3_AUX_DACC_6?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_7?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_8?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_9?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_10?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_11?string}UL, \
                                                 0x${SRV_PCOUP_G3_AUX_DACC_12?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_13?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_14?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_15?string}UL, 0x${SRV_PCOUP_G3_AUX_DACC_16?string}UL}
#define SRV_PCOUP_AUX_GAIN_HIGH_TBL              {${SRV_PCOUP_G3_AUX_GAIN_HIGH_0?string}, ${SRV_PCOUP_G3_AUX_GAIN_HIGH_1?string}, ${SRV_PCOUP_G3_AUX_GAIN_HIGH_2?string}}
#define SRV_PCOUP_AUX_GAIN_VLOW_TBL              {${SRV_PCOUP_G3_AUX_GAIN_VLOW_0?string}, ${SRV_PCOUP_G3_AUX_GAIN_VLOW_1?string}, ${SRV_PCOUP_G3_AUX_GAIN_VLOW_2?string}}
#define SRV_PCOUP_AUX_NUM_TX_LEVELS              ${SRV_PCOUP_G3_AUX_NUM_TX_LVL?string}
#define SRV_PCOUP_AUX_LINE_DRV_CONF              ${SRV_PCOUP_G3_AUX_LINE_DRIVER?string}

  <#if SRV_PCOUP_G3_AUX_BAND == "CEN-A">
    <#if ((SRV_PCOUP_PLC_MODE == "PL460") && (SRV_PCOUP_PLC_COUP_SETTINGS_PL460?contains("Multiband single-branch FCC & CEN-A")))>
#define SRV_PCOUP_AUX_PRED_HIGH_TBL              {0x5994, 0x57F2, 0x5681, 0x551E, 0x5453, 0x538E, 0x5266, 0x50DE, 0x4FF4, 0x4FBB, 0x4F88, 0x4F74, 0x504A, 0x5189,  \
                                                  0x527D, 0x535A, 0x5497, 0x5651, 0x585D, 0x5A72, 0x5C91, 0x5F17, 0x61C0, 0x63D4, 0x6557, 0x6732, 0x69BA, 0x6C09,  \
                                                  0x6D8A, 0x6F8F, 0x7254, 0x741F, 0x74E5, 0x77A7, 0x7BA9, 0x7FFF}
#define SRV_PCOUP_AUX_PRED_VLOW_TBL              {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}
    <#else>
#define SRV_PCOUP_AUX_PRED_HIGH_TBL              {0x670A, 0x660F, 0x676A, 0x6A6B, 0x6F3F, 0x7440, 0x74ED, 0x7792, 0x762D, 0x7530, 0x7938, 0x7C0A, 0x7C2A, 0x7B0E, \
                                                  0x7AF2, 0x784B, 0x7899, 0x76F9, 0x76D6, 0x769F, 0x775D, 0x70C0, 0x6EB9, 0x6F18, 0x6F1E, 0x6FA2, 0x6862, 0x67C9, \
                                                  0x68F9, 0x68A5, 0x6CA3, 0x7153, 0x7533, 0x750B, 0x7B59, 0x7FFF}
#define SRV_PCOUP_AUX_PRED_VLOW_TBL              {0x7FFF, 0x7DB1, 0x7CE6, 0x7B36, 0x772F, 0x7472, 0x70AA, 0x6BC2, 0x682D, 0x6618, 0x6384, 0x6210, 0x61D7, 0x6244, \
                                                  0x6269, 0x63A8, 0x6528, 0x65CC, 0x67F6, 0x693B, 0x6B13, 0x6C29, 0x6D43, 0x6E26, 0x6D70, 0x6C94, 0x6BB5, 0x6AC9, \
                                                  0x6A5F, 0x6B65, 0x6B8C, 0x6A62, 0x6CEC, 0x6D5A, 0x6F9D, 0x6FD3}
    </#if>
  <#elseif SRV_PCOUP_G3_AUX_BAND == "CEN-B">
#define SRV_PCOUP_AUX_PRED_HIGH_TBL              {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}
#define SRV_PCOUP_AUX_PRED_VLOW_TBL              {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}
  <#elseif SRV_PCOUP_G3_MAIN_BAND == "FCC">
#define SRV_PCOUP_AUX_PRED_HIGH_TBL              {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}
#define SRV_PCOUP_AUX_PRED_VLOW_TBL              {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}
  <#elseif SRV_PCOUP_G3_MAIN_BAND == "ARIB">
#define SRV_PCOUP_AUX_PRED_HIGH_TBL               {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}
#define SRV_PCOUP_AUX_PRED_VLOW_TBL               {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}
  </#if>
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

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

    /* Pointer to Tx equalization coefficients table in HIGH mode.
       There is one coefficient for each carrier in the used band */
    const uint16_t *                 equHigh;

    /* Pointer to Tx equalization coefficients table in VLOW mode.
       There is one coefficient for each carrier in the used band */
    const uint16_t *                 equVlow;

    /* Tx gain values for HIGH mode [HIGH_INI, HIGH_MIN, HIGH_MAX] */
    uint16_t                         gainHigh[3];

    /* Tx gain values for VLOW mode [VLOW_INI, VLOW_MIN, VLOW_MAX] */
    uint16_t                         gainVLow[3];

    /* Number of Tx attenuation levels (3 dB step) suppoting dynamic Tx mode */
    uint8_t                          numTxLevels;

    /* Size of Tx equalization coefficients table in bytes */
    uint8_t                          equSize;

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
    SRV_PLC_PCOUP_DATA * SRV_PCOUP_Get_Config(uint8_t phyBand)

  Summary:
    Get the PLC PHY Coupling parameters for the specified G3-PLC PHY band.

  Description:
    This function allows to get the PLC PHY Coupling parameters for the
    specified G3-PLC PHY band. These parameters can be sent to the PLC
    device through PLC Driver PIB interface (${SRV_PCOUP_PIB_SET}).

  Precondition:
    None.

  Parameters:
    phyBand - G3-PLC PHY band for which the parameters are requested

  Returns:
    - Pointer PLC PHY Coupling parameters
      - if phyBand parameter is valid
    - NULL
      - if phyBand parameter is not valid

  Example:
    <code>
    SRV_PLC_PCOUP_DATA *pCoupValues;

    pCoupValues = SRV_PCOUP_Get_Config(G3_FCC);
    </code>

  Remarks:
    If SRV_PCOUP_Set_Config is used to set the PLC PHY Coupling parameters,
    this function is not needed.
  ***************************************************************************/

SRV_PLC_PCOUP_DATA * SRV_PCOUP_Get_Config(uint8_t phyBand);

/***************************************************************************
  Function:
    bool SRV_PCOUP_Set_Config(DRV_HANDLE handle, uint8_t phyBand);

  Summary:
    Set the PLC PHY Coupling parameters for the specified G3-PLC PHY band.

  Description:
    This function allows to set the PLC PHY Coupling parameters for the
    specified G3-PLC PHY band, using the PLC Driver PIB interface
    (${SRV_PCOUP_PIB_SET}).

  Precondition:
    ${SRV_PCOUP_DRV_OPEN} must have been called to obtain a valid
    opened device handle.

  Parameters:
    handle  - A valid instance handle, returned from ${SRV_PCOUP_DRV_OPEN}
    phyBand - G3-PLC PHY band for which the parameters are requested

  Returns:
    - true
      - Successful configuration
    - false
      - if phyBand parameter is not valid
      - if there is an error when calling ${SRV_PCOUP_PIB_SET}

  Example:
    <code>
    bool result;

    result = SRV_PCOUP_Set_Config(handle, G3_FCC);
    </code>

  Remarks:
    None.
  ***************************************************************************/

bool SRV_PCOUP_Set_Config(DRV_HANDLE handle, uint8_t phyBand);

/***************************************************************************
  Function:
    uint8_t SRV_PCOUP_Get_Default_Phy_Band( void )

  Summary:
    Get the default G3-PLC PHY band.

  Description:
    This function allows to get the G3-PLC PHY band used by default.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Default G3-PLC PHY band.

  Example:
    <code>
    uint8_t plcPhyBand;

    plcPhyBand = SRV_PCOUP_Get_Default_Phy_Band();
    SRV_PCOUP_Set_Config(plcPhyBand);
    </code>

  Remarks:
    None.
  ***************************************************************************/

uint8_t SRV_PCOUP_Get_Default_Phy_Band( void );

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif //SRV_PCOUP_H
