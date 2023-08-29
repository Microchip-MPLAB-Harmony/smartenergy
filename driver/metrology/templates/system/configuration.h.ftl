<#--
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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
-->
/* Metrology Configuration Options */
#define DRV_METROLOGY_REG_BASE_ADDRESS        0x${DRV_MET_BASE_ADDRESS?string}UL
/* Metrology Default Config: Meter Constant */
#define DRV_METROLOGY_CONF_PKT                0x${DRV_MET_CTRL_PKT?string}UL
/* Metrology Default Config: Meter Type */
#define DRV_METROLOGY_CONF_MT                 0x${DRV_MET_CTRL_MT?string}UL
/* Metrology Default Config: Current conversion factor */
#define DRV_METROLOGY_CONF_KI                 0x${DRV_MET_CTRL_KI?string}UL
/* Metrology Default Config: Voltage conversion factor */
#define DRV_METROLOGY_CONF_KV                 0x${DRV_MET_CTRL_KV?string}UL
/* Metrology Default Config: ATSENSE CTRL 20 23 */
#define DRV_METROLOGY_CONF_ATS2023            0x${DRV_MET_CTRL_ATS2023?string}UL
/* Metrology Default Config: ATSENSE CTRL 24 27 */
#define DRV_METROLOGY_CONF_ATS2427            0x${DRV_MET_CTRL_ATS2427?string}UL
/* Metrology Default Config: SWELL */
#define DRV_METROLOGY_CONF_SWELL              0x${DRV_MET_CTRL_SWELL?string}UL
/* Metrology Default Config: SAG */
#define DRV_METROLOGY_CONF_SAG                0x${DRV_MET_CTRL_SAG?string}UL
/* Metrology Default Config: CREEP P */
#define DRV_METROLOGY_CONF_CREEP_P            0x${DRV_MET_CTRL_CREEP_P?string}UL
/* Metrology Default Config: CREEP Q */
#define DRV_METROLOGY_CONF_CREEP_Q            0x${DRV_MET_CTRL_CREEP_Q?string}UL
/* Metrology Default Config: CREEP I */
#define DRV_METROLOGY_CONF_CREEP_I            0x${DRV_MET_CTRL_CREEP_I?string}UL
/* Metrology Default Config: FEATURE_CTRL0 */
#define DRV_METROLOGY_CONF_FCTRL0             0x${DRV_MET_CTRL_FEATCTRL0?string}UL
/* Metrology Default Config: FEATURE_CTRL1 */
#define DRV_METROLOGY_CONF_FCTRL1             0x${DRV_MET_CTRL_FEATCTRL1?string}UL
/* Metrology Default Config: PULSE0_CTRL */
#define DRV_METROLOGY_CONF_PULSE0_CTRL        0x${DRV_MET_CTRL_PULSE_CTRL_0?string}UL
/* Metrology Default Config: PULSE1_CTRL */
#define DRV_METROLOGY_CONF_PULSE1_CTRL        0x${DRV_MET_CTRL_PULSE_CTRL_1?string}UL
/* Metrology Default Config: PULSE2_CTRL */
#define DRV_METROLOGY_CONF_PULSE2_CTRL        0x${DRV_MET_CTRL_PULSE_CTRL_2?string}UL
/* Metrology Default Config: Waveform Capture */
#define DRV_METROLOGY_CONF_WAVEFORM           0x${DRV_MET_CONF_WAVEFORM_CAPTURE?string}UL
/* Metrology Default Config: Capture Buffer Size */
#define DRV_METROLOGY_CAPTURE_BUF_SIZE        ${DRV_MET_CONF_CAPTURE_BUFFER_SIZE?string}UL

<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    <#lt>/* Metrology Driver RTOS Configurations */
    <#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "FreeRTOS">
        <#lt>#define DRV_METROLOGY_RTOS_STACK_SIZE         ${DRV_MET_RTOS_STACK_SIZE / 4}U
    <#else>
        <#lt>#define DRV_METROLOGY_RTOS_STACK_SIZE         ${DRV_MET_RTOS_STACK_SIZE}U
    </#if>
        <#lt>#define DRV_METROLOGY_RTOS_TASK_PRIORITY      ${DRV_MET_RTOS_TASK_PRIORITY}U
        <#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MicriumOSIII">
            <#lt>#define DRV_METROLOGY_RTOS_TASK_MSG_QTY       ${DRV_MET_RTOS_TASK_MSG_QTY}U
            <#lt>#define DRV_METROLOGY_RTOS_TASK_TIME_QUANTA   ${DRV_MET_RTOS_TASK_TIME_QUANTA}U
    </#if>
</#if>