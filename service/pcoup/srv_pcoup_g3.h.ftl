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
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
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
<#elseif (drvG3MacRt)??>
#include "driver/plc/g3MacRt/drv_g3_macrt.h"
<#assign SRV_PCOUP_COMM_H = "drv_g3_macrt_comm.h">
<#assign SRV_PCOUP_PIB_SET = "DRV_G3_MACRT_PIBSet">
<#assign SRV_PCOUP_DRV_OPEN = "DRV_G3_MACRT_Open">
</#if>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

/* Default branch of the PLC transmission coupling */
<#if (drvPlcPhy)??>
  <#if ((drvPlcPhy.DRV_PLC_BAND_IN_USE > 4) && (drvPlcPhy.DRV_PLC_G3_BAND_AUX_ACTIVE == true))>
#define SRV_PCOUP_DEFAULT_BRANCH                 SRV_PLC_PCOUP_AUXILIARY_BRANCH
  <#else>
#define SRV_PCOUP_DEFAULT_BRANCH                 SRV_PLC_PCOUP_MAIN_BRANCH
  </#if>
<#elseif (drvG3MacRt)??>
  <#if ((drvG3MacRt.DRV_PLC_BAND_IN_USE > 4) && (drvG3MacRt.DRV_PLC_G3_BAND_AUX_ACTIVE == true))>
#define SRV_PCOUP_DEFAULT_BRANCH                 SRV_PLC_PCOUP_AUXILIARY_BRANCH
  <#else>
#define SRV_PCOUP_DEFAULT_BRANCH                 SRV_PLC_PCOUP_MAIN_BRANCH
  </#if>
</#if>

/* Equalization number of coefficients (number of carriers) for Main branch */
<#if SRV_PCOUP_G3_MAIN_BAND == "CEN-A">

#define SRV_PCOUP_EQU_NUM_COEF                   36
<#elseif SRV_PCOUP_G3_MAIN_BAND == "CEN-B">
#define SRV_PCOUP_EQU_NUM_COEF                   16
<#elseif SRV_PCOUP_G3_MAIN_BAND == "FCC">
#define SRV_PCOUP_EQU_NUM_COEF                   72
<#elseif SRV_PCOUP_G3_MAIN_BAND == "ARIB">
#define SRV_PCOUP_EQU_NUM_COEF                   54
</#if>

/* PLC PHY Coupling parameters for Main branch */
#define SRV_PCOUP_RMS_HIGH_TBL                   {${SRV_PCOUP_G3_RMS_HIGH_0?string}, ${SRV_PCOUP_G3_RMS_HIGH_1?string}, ${SRV_PCOUP_G3_RMS_HIGH_2?string}, ${SRV_PCOUP_G3_RMS_HIGH_3?string}, ${SRV_PCOUP_G3_RMS_HIGH_4?string}, ${SRV_PCOUP_G3_RMS_HIGH_5?string}, ${SRV_PCOUP_G3_RMS_HIGH_6?string}, ${SRV_PCOUP_G3_RMS_HIGH_7?string}}
#define SRV_PCOUP_RMS_VLOW_TBL                   {${SRV_PCOUP_G3_RMS_VLOW_0?string}, ${SRV_PCOUP_G3_RMS_VLOW_1?string}, ${SRV_PCOUP_G3_RMS_VLOW_2?string}, ${SRV_PCOUP_G3_RMS_VLOW_3?string}, ${SRV_PCOUP_G3_RMS_VLOW_4?string}, ${SRV_PCOUP_G3_RMS_VLOW_5?string}, ${SRV_PCOUP_G3_RMS_VLOW_6?string}, ${SRV_PCOUP_G3_RMS_VLOW_7?string}}
#define SRV_PCOUP_THRS_HIGH_TBL                  {${SRV_PCOUP_G3_THRS_HIGH_0?string}, ${SRV_PCOUP_G3_THRS_HIGH_1?string}, ${SRV_PCOUP_G3_THRS_HIGH_2?string}, ${SRV_PCOUP_G3_THRS_HIGH_3?string}, ${SRV_PCOUP_G3_THRS_HIGH_4?string}, ${SRV_PCOUP_G3_THRS_HIGH_5?string}, ${SRV_PCOUP_G3_THRS_HIGH_6?string}, ${SRV_PCOUP_G3_THRS_HIGH_7?string}, ${SRV_PCOUP_G3_THRS_HIGH_8?string}, ${SRV_PCOUP_G3_THRS_HIGH_9?string}, ${SRV_PCOUP_G3_THRS_HIGH_10?string}, ${SRV_PCOUP_G3_THRS_HIGH_11?string}, ${SRV_PCOUP_G3_THRS_HIGH_12?string}, ${SRV_PCOUP_G3_THRS_HIGH_13?string}, ${SRV_PCOUP_G3_THRS_HIGH_14?string}, ${SRV_PCOUP_G3_THRS_HIGH_15?string}}
#define SRV_PCOUP_THRS_VLOW_TBL                  {${SRV_PCOUP_G3_THRS_VLOW_0?string}, ${SRV_PCOUP_G3_THRS_VLOW_1?string}, ${SRV_PCOUP_G3_THRS_VLOW_2?string}, ${SRV_PCOUP_G3_THRS_VLOW_3?string}, ${SRV_PCOUP_G3_THRS_VLOW_4?string}, ${SRV_PCOUP_G3_THRS_VLOW_5?string}, ${SRV_PCOUP_G3_THRS_VLOW_6?string}, ${SRV_PCOUP_G3_THRS_VLOW_7?string}, ${SRV_PCOUP_G3_THRS_VLOW_8?string}, ${SRV_PCOUP_G3_THRS_VLOW_9?string}, ${SRV_PCOUP_G3_THRS_VLOW_10?string}, ${SRV_PCOUP_G3_THRS_VLOW_11?string}, ${SRV_PCOUP_G3_THRS_VLOW_12?string}, ${SRV_PCOUP_G3_THRS_VLOW_13?string}, ${SRV_PCOUP_G3_THRS_VLOW_14?string}, ${SRV_PCOUP_G3_THRS_VLOW_15?string}}
#define SRV_PCOUP_DACC_TBL                       {0x${SRV_PCOUP_G3_DACC_0?string}, 0x${SRV_PCOUP_G3_DACC_1?string}, 0x${SRV_PCOUP_G3_DACC_2?string}, 0x${SRV_PCOUP_G3_DACC_3?string}, 0x${SRV_PCOUP_G3_DACC_4?string}, 0x${SRV_PCOUP_G3_DACC_5?string}, \
                                                 0x${SRV_PCOUP_G3_DACC_6?string}, 0x${SRV_PCOUP_G3_DACC_7?string}, 0x${SRV_PCOUP_G3_DACC_8?string}, 0x${SRV_PCOUP_G3_DACC_9?string}, 0x${SRV_PCOUP_G3_DACC_10?string}, 0x${SRV_PCOUP_G3_DACC_11?string}, \
                                                 0x${SRV_PCOUP_G3_DACC_12?string}, 0x${SRV_PCOUP_G3_DACC_13?string}, 0x${SRV_PCOUP_G3_DACC_14?string}, 0x${SRV_PCOUP_G3_DACC_15?string}, 0x${SRV_PCOUP_G3_DACC_16?string}}
#define SRV_PCOUP_GAIN_HIGH_TBL                  {${SRV_PCOUP_G3_GAIN_HIGH_0?string}, ${SRV_PCOUP_G3_GAIN_HIGH_1?string}, ${SRV_PCOUP_G3_GAIN_HIGH_2?string}}
#define SRV_PCOUP_GAIN_VLOW_TBL                  {${SRV_PCOUP_G3_GAIN_VLOW_0?string}, ${SRV_PCOUP_G3_GAIN_VLOW_1?string}, ${SRV_PCOUP_G3_GAIN_VLOW_2?string}}
#define SRV_PCOUP_NUM_TX_LEVELS                  ${SRV_PCOUP_G3_NUM_TX_LVL?string}
#define SRV_PCOUP_LINE_DRV_CONF                  ${SRV_PCOUP_G3_LINE_DRIVER?string}

<#if (((drvPlcPhy)?? && (drvPlcPhy.DRV_PLC_MODE == "PL460")) || ((drvG3MacRt)?? && (drvG3MacRt.DRV_PLC_MODE == "PL460")))> 
  <#if SRV_PCOUP_G3_MAIN_BAND == "CEN-A">
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x5620, 0x59C7, 0x5E1E, 0x6333, 0x698B, 0x6F03, 0x72CD, 0x760E, 0x7904, 0x7B57, 0x7D2C, 0x7E72, 0x7F0F, 0x7FC6, \
                                                  0x7FFF, 0x7ED1, 0x7D11, 0x7BCE, 0x7A1A, 0x777C, 0x7496, 0x720F, 0x6F8E, 0x6BE0, 0x6780, 0x6357, 0x5F5E, 0x5C0C, \
                                                  0x597B, 0x5782, 0x572D, 0x57A2, 0x5823, 0x59F2, 0x5D86, 0x6153}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7F81, 0x7E57, 0x7C6F, 0x7A35, 0x771F, 0x730B, 0x6E99, 0x6A40, 0x6654, 0x62C6, 0x5F77, 0x5CE6, 0x5B68, \
                                                  0x5A7B, 0x5A08, 0x5A66, 0x5BAD, 0x5D58, 0x5F29, 0x6109, 0x6338, 0x6539, 0x6686, 0x672E, 0x67D2, 0x686D, 0x68D2, \
                                                  0x68F6, 0x6927, 0x6995, 0x6989, 0x68C3, 0x68D1, 0x69AA, 0x6AC3}
  <#elseif SRV_PCOUP_G3_MAIN_BAND == "CEN-B">
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}  
  <#elseif SRV_PCOUP_G3_MAIN_BAND == "FCC">
    <#if (((drvPlcPhy)?? && (drvPlcPhy.DRV_PLC_COUP_G3_HIGH_ATTENUATION == true)) || ((drvG3MacRt)?? && (drvG3MacRt.DRV_PLC_COUP_G3_HIGH_ATTENUATION == true)))> 
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x6FFD, 0x6AD0, 0x65CF, 0x6073, 0x5AF7, 0x5618, 0x5158, 0x4CA7, 0x4869, 0x44EC, 0x4222, 0x3FD7, 0x3E4E, 0x3DB9, \
                                                  0x3DC3, 0x3E05, 0x3E97, 0x3F8B, 0x407B, 0x4130, 0x41D1, 0x4285, 0x4330, 0x4379, 0x4394, 0x43C5, 0x4407, 0x43FA, \
                                                  0x43C6, 0x43B2, 0x43C5, 0x43B2, 0x435D, 0x4359, 0x43AD, 0x43FB, 0x4437, 0x44CD, 0x45EC, 0x46C7, 0x47D3, 0x48F6, \
                                                  0x4ABD, 0x4C07, 0x4D9C, 0x4F0B, 0x5125, 0x52CE, 0x5479, 0x564A, 0x5844, 0x5A45, 0x5BE9, 0x5DAC, 0x5F88, 0x617E, \
                                                  0x62F0, 0x64A8, 0x66AA, 0x68AB, 0x6A56, 0x6BCB, 0x6DC4, 0x6F84, 0x70F2, 0x7268, 0x747B, 0x7660, 0x77D4, 0x79E1, \
                                                  0x7CD8, 0x7FFF}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}  
    <#else>
#define SRV_PCOUP_PRED_HIGH_TBL                  {0x7399, 0x6D5B, 0x6982, 0x671E, 0x6699, 0x6730, 0x6875, 0x6975, 0x6AE7, 0x6CE3, 0x6EF9, 0x70A7, 0x7276, 0x74B0, \
                                                  0x76BF, 0x77FE, 0x7905, 0x7A70, 0x7BC9, 0x7C88, 0x7D0A, 0x7DF6, 0x7EDF, 0x7F32, 0x7EF1, 0x7F6D, 0x7FFB, 0x7FFF, \
                                                  0x7F96, 0x7F76, 0x7F9D, 0x7EF8, 0x7E1B, 0x7D55, 0x7D2F, 0x7C3C, 0x7B39, 0x7A6C, 0x79CE, 0x790C, 0x779B, 0x76A4, \
                                                  0x7560, 0x7498, 0x72B8, 0x7185, 0x7049, 0x6F5D, 0x6DA6, 0x6C38, 0x6B46, 0x6A5E, 0x6940, 0x6855, 0x6802, 0x678A, \
                                                  0x6676, 0x6567, 0x654C, 0x6546, 0x651F, 0x65CD, 0x673D, 0x6876, 0x69C8, 0x6AD5, 0x6C7A, 0x6E1D, 0x6F4E, 0x70B3, \
                                                  0x72F9, 0x74E0}
#define SRV_PCOUP_PRED_VLOW_TBL                  {0x7FEC, 0x7D9A, 0x7BBA, 0x7987, 0x7752, 0x75E3, 0x7429, 0x71CE, 0x6FA1, 0x6E0A, 0x6C89, 0x6A9E, 0x68D7, 0x67BA, \
                                                  0x66CC, 0x655C, 0x63F4, 0x6318, 0x626F, 0x6186, 0x6093, 0x602C, 0x604E, 0x6022, 0x5F9A, 0x5FB6, 0x602F, 0x6049, \
                                                  0x6024, 0x608F, 0x615F, 0x61D9, 0x61E3, 0x6265, 0x6372, 0x6414, 0x6464, 0x6519, 0x6647, 0x672B, 0x679F, 0x6834, \
                                                  0x6959, 0x6A44, 0x6A93, 0x6B1F, 0x6C52, 0x6D4F, 0x6D98, 0x6E0E, 0x6F43, 0x7047, 0x70A5, 0x7136, 0x7258, 0x732C, \
                                                  0x7348, 0x7371, 0x7453, 0x7566, 0x75C8, 0x764F, 0x77A2, 0x78F2, 0x7929, 0x7990, 0x7AB0, 0x7B90, 0x7B35, 0x7C1E, \
                                                  0x7DE6, 0x7FFF}  
    </#if>
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

<#if (SRV_PCOUP_G3_MAIN_BAND == "FCC" || SRV_PCOUP_G3_MAIN_BAND == "ARIB") && 
(((drvPlcPhy)?? && (drvPlcPhy.DRV_PLC_COUP_G3_MULTIBAND == true)) || ((drvG3MacRt)?? && (drvG3MacRt.DRV_PLC_COUP_G3_MULTIBAND == true)))>
/* Equalization number of coefficients (number of carriers) for Auxiliary branch */
<#if SRV_PCOUP_G3_AUX_BAND == "CEN-A">
#define SRV_PCOUP_AUX_EQU_NUM_COEF               36
<#elseif SRV_PCOUP_G3_AUX_BAND == "CEN-B">
#define SRV_PCOUP_AUX_EQU_NUM_COEF               16
</#if>

/* PLC PHY Coupling parameters for Auxiliary branch */
#define SRV_PCOUP_AUX_RMS_HIGH_TBL               {${SRV_PCOUP_G3_AUX_RMS_HIGH_0?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_1?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_2?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_3?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_4?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_5?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_6?string}, ${SRV_PCOUP_G3_AUX_RMS_HIGH_7?string}}
#define SRV_PCOUP_AUX_RMS_VLOW_TBL               {${SRV_PCOUP_G3_AUX_RMS_VLOW_0?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_1?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_2?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_3?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_4?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_5?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_6?string}, ${SRV_PCOUP_G3_AUX_RMS_VLOW_7?string}}
#define SRV_PCOUP_AUX_THRS_HIGH_TBL              {${SRV_PCOUP_G3_AUX_THRS_HIGH_0?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_1?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_2?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_3?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_4?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_5?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_6?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_7?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_8?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_9?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_10?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_11?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_12?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_13?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_14?string}, ${SRV_PCOUP_G3_AUX_THRS_HIGH_15?string}}
#define SRV_PCOUP_AUX_THRS_VLOW_TBL              {${SRV_PCOUP_G3_AUX_THRS_VLOW_0?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_1?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_2?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_3?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_4?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_5?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_6?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_7?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_8?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_9?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_10?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_11?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_12?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_13?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_14?string}, ${SRV_PCOUP_G3_AUX_THRS_VLOW_15?string}}
#define SRV_PCOUP_AUX_DACC_TBL                   {0x${SRV_PCOUP_G3_AUX_DACC_0?string}, 0x${SRV_PCOUP_G3_AUX_DACC_1?string}, 0x${SRV_PCOUP_G3_AUX_DACC_2?string}, 0x${SRV_PCOUP_G3_AUX_DACC_3?string}, 0x${SRV_PCOUP_G3_AUX_DACC_4?string}, 0x${SRV_PCOUP_G3_AUX_DACC_5?string}, \
                                                 0x${SRV_PCOUP_G3_AUX_DACC_6?string}, 0x${SRV_PCOUP_G3_AUX_DACC_7?string}, 0x${SRV_PCOUP_G3_AUX_DACC_8?string}, 0x${SRV_PCOUP_G3_AUX_DACC_9?string}, 0x${SRV_PCOUP_G3_AUX_DACC_10?string}, 0x${SRV_PCOUP_G3_AUX_DACC_11?string}, \
                                                 0x${SRV_PCOUP_G3_AUX_DACC_12?string}, 0x${SRV_PCOUP_G3_AUX_DACC_13?string}, 0x${SRV_PCOUP_G3_AUX_DACC_14?string}, 0x${SRV_PCOUP_G3_AUX_DACC_15?string}, 0x${SRV_PCOUP_G3_AUX_DACC_16?string}}
#define SRV_PCOUP_AUX_GAIN_HIGH_TBL              {${SRV_PCOUP_G3_AUX_GAIN_HIGH_0?string}, ${SRV_PCOUP_G3_AUX_GAIN_HIGH_1?string}, ${SRV_PCOUP_G3_AUX_GAIN_HIGH_2?string}}
#define SRV_PCOUP_AUX_GAIN_VLOW_TBL              {${SRV_PCOUP_G3_AUX_GAIN_VLOW_0?string}, ${SRV_PCOUP_G3_AUX_GAIN_VLOW_1?string}, ${SRV_PCOUP_G3_AUX_GAIN_VLOW_2?string}}
#define SRV_PCOUP_AUX_NUM_TX_LEVELS              ${SRV_PCOUP_G3_AUX_NUM_TX_LVL?string}
#define SRV_PCOUP_AUX_LINE_DRV_CONF              ${SRV_PCOUP_G3_AUX_LINE_DRIVER?string}

  <#if SRV_PCOUP_G3_AUX_BAND == "CEN-A">  
#define SRV_PCOUP_AUX_PRED_HIGH_TBL              {0x670A, 0x660F, 0x676A, 0x6A6B, 0x6F3F, 0x7440, 0x74ED, 0x7792, 0x762D, 0x7530, 0x7938, 0x7C0A, 0x7C2A, 0x7B0E, \
                                                  0x7AF2, 0x784B, 0x7899, 0x76F9, 0x76D6, 0x769F, 0x775D, 0x70C0, 0x6EB9, 0x6F18, 0x6F1E, 0x6FA2, 0x6862, 0x67C9, \
                                                  0x68F9, 0x68A5, 0x6CA3, 0x7153, 0x7533, 0x750B, 0x7B59, 0x7FFF}
#define SRV_PCOUP_AUX_PRED_VLOW_TBL              {0x7FFF, 0x7DB1, 0x7CE6, 0x7B36, 0x772F, 0x7472, 0x70AA, 0x6BC2, 0x682D, 0x6618, 0x6384, 0x6210, 0x61D7, 0x6244, \
                                                  0x6269, 0x63A8, 0x6528, 0x65CC, 0x67F6, 0x693B, 0x6B13, 0x6C29, 0x6D43, 0x6E26, 0x6D70, 0x6C94, 0x6BB5, 0x6AC9, \
                                                  0x6A5F, 0x6B65, 0x6B8C, 0x6A62, 0x6CEC, 0x6D5A, 0x6F9D, 0x6FD3}  
  <#elseif SRV_PCOUP_G3_AUX_BAND == "CEN-B">
#define SRV_PCOUP_AUX_PRED_HIGH_TBL              {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}
#define SRV_PCOUP_AUX_PRED_VLOW_TBL              {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, \
                                                  0x7FFF, 0x7FFF}  
  </#if>
</#if>

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

    /* Auxiliary Transmission Branch */
    SRV_PLC_PCOUP_AUXILIARY_BRANCH,

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
    SRV_PLC_PCOUP_DATA * SRV_PCOUP_Get_Config(SRV_PLC_PCOUP_BRANCH branch)
    
  Summary:
    Get the PLC PHY Coupling parameters for the specified transmission branch.

  Description:
    This function allows to get the PLC PHY Coupling parameters for the 
    specified transmission branch. These parameters can be sent to the PLC 
    device through PLC Driver PIB interface (${SRV_PCOUP_PIB_SET}).

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
    interface (${SRV_PCOUP_PIB_SET}).

  Precondition:
    ${SRV_PCOUP_DRV_OPEN} must have been called to obtain a valid 
    opened device handle.

  Parameters:
    handle  - A valid instance handle, returned from ${SRV_PCOUP_DRV_OPEN}
    branch  - Transmission branch for which the parameters will be set

  Returns:
    - true
      - Successful configuration
    - false
      - if branch parameter is not valid
      - if there is an error when calling ${SRV_PCOUP_PIB_SET}

  Example:
    <code>
    // 'handle', returned from ${SRV_PCOUP_DRV_OPEN}
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
    (see ${SRV_PCOUP_COMM_H}):
    - 0: G3_CEN_A
    - 1: G3_CEN_B
    - 2: G3_FCC
    - 3: G3_ARIB
    - 0xFF: G3_INVALID (if transmission branch is not valid)

  Example:
    <code>
    phyBand = SRV_PCOUP_Get_Phy_Band(SRV_PLC_PCOUP_MAIN_BRANCH);

    if (phyBand == G3_CEN_A)
    {
        // G3 CEN-A band
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
