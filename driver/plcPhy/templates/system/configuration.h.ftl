<#--
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
-->
/* PLC PHY Driver Configuration Options */
#define DRV_PLC_SECURE                        ${DRV_PLC_SECURE_MODE?string}
<#if DRV_PLC_PLIB == "SRV_SPISPLIT">
<#--  Connected to SPI PLIB through SPI Splitter  -->
    <#assign SPI_PLIB = DRV_PLC_PLIB_SPISPLIT>
<#else>
<#--  Connected directly to SPI PLIB  -->
    <#assign SPI_PLIB = DRV_PLC_PLIB>
</#if>
<#if SPI_PLIB?lower_case[0..*6] == "sercom">
#define DRV_PLC_SPI_CS_PIN                    ${DRV_PLC_SPI_CS_PIN?string}
#define DRV_PLC_EXT_INT_SRC                   EIC_IRQn
<#else>
#define DRV_PLC_EXT_INT_PIO_PORT              ${DRV_PLC_EXT_INT_PIO_PORT?string}
#define DRV_PLC_EXT_INT_SRC                   ${DRV_PLC_EXT_INT_SRC?string}
</#if>
#define DRV_PLC_EXT_INT_PIN                   ${DRV_PLC_EXT_INT_PIN?string}
#define DRV_PLC_RESET_PIN                     ${DRV_PLC_RESET_PIN?string}
#define DRV_PLC_LDO_EN_PIN                    ${DRV_PLC_LDO_EN_PIN?string}
<#if DRV_PLC_MODE == "PL460">
#define DRV_PLC_TX_ENABLE_PIN                 ${DRV_PLC_TX_ENABLE_PIN?string}
</#if>
<#if DRV_PLC_SLEEP_MODE == true>
#define DRV_PLC_STBY_PIN                      ${DRV_PLC_STBY_PIN?string}
</#if>
<#if DRV_PLC_MODE == "PL460" && DRV_PLC_THERMAL_MONITOR == true>
#define DRV_PLC_THMON_PIN                     ${DRV_PLC_THMON_PIN?string}
</#if>
<#if DRV_PLC_SPI_NUM_CSR != 0>
#define DRV_PLC_CSR_INDEX                     ${DRV_PLC_SPI_NPCS}
<#if DRV_PLC_PLIB == "SRV_SPISPLIT">
#define DRV_PLC_SPI_CHIP_SELECT               SPI_CHIP_SELECT_NPCS${DRV_PLC_SPI_NPCS}
</#if>
</#if>
#define DRV_PLC_SPI_CLK                       8000000

<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    <#lt>/* PLC PHY Driver RTOS Configuration Options */
    <#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
        <#lt>#define DRV_PLC_RTOS_STACK_SIZE               ${DRV_PLC_RTOS_STACK_SIZE / 4}
    <#else>
        <#lt>#define DRV_PLC_RTOS_STACK_SIZE               ${DRV_PLC_RTOS_STACK_SIZE}
    </#if>
    <#lt>#define DRV_PLC_RTOS_TASK_PRIORITY            ${DRV_PLC_RTOS_TASK_PRIORITY}
    <#if HarmonyCore.SELECT_RTOS == "MicriumOSIII">
        <#lt>#define DRV_PLC_RTOS_TASK_MSG_QTY             ${DRV_PLC_RTOS_TASK_MSG_QTY}u
        <#lt>#define DRV_PLC_RTOS_TASK_TIME_QUANTA         ${DRV_PLC_RTOS_TASK_TIME_QUANTA}u
    </#if>

</#if>
/* PLC Driver Identification */
#define DRV_PLC_PHY_INSTANCES_NUMBER          1
#define DRV_PLC_PHY_INDEX                     0
#define DRV_PLC_PHY_CLIENTS_NUMBER_IDX        1
<#if DRV_PLC_BAND_IN_USE == 1>
#define DRV_PLC_PHY_PROFILE                   0
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_A
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36010103
<#elseif DRV_PLC_BAND_IN_USE == 2>
#define DRV_PLC_PHY_PROFILE                   2
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_FCC
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36020103
<#elseif DRV_PLC_BAND_IN_USE == 3>
#define DRV_PLC_PHY_PROFILE                   3
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_ARIB
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36030103
<#elseif DRV_PLC_BAND_IN_USE == 4>
#define DRV_PLC_PHY_PROFILE                   1
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_B
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36040103
<#elseif DRV_PLC_BAND_IN_USE == 5>
#define DRV_PLC_PHY_PROFILE                   4
#define DRV_PLC_PHY_NUM_CARRIERS              97
#define DRV_PLC_PHY_HOST_PRODUCT              0x3600
#define DRV_PLC_PHY_HOST_VERSION              0x36000300
#define DRV_PLC_PHY_HOST_PHY                  0x36000003
<#elseif (DRV_PLC_BAND_IN_USE == 6 || DRV_PLC_BAND_IN_USE == 7)>
    <#if DRV_PLC_G3_BAND_AUX_ACTIVE == true>
        <#if (DRV_PLC_BAND_IN_USE == 6)>
#define DRV_PLC_PHY_PROFILE                   0
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_A
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36010103
        <#else>
#define DRV_PLC_PHY_PROFILE                   1
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_B
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36040103
        </#if>
    <#else>
#define DRV_PLC_PHY_PROFILE                   2
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_FCC
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36020103
    </#if>
<#elseif (DRV_PLC_BAND_IN_USE == 8 || DRV_PLC_BAND_IN_USE == 9)>
    <#if DRV_PLC_G3_BAND_AUX_ACTIVE == true>
        <#if (DRV_PLC_BAND_IN_USE == 8)>
#define DRV_PLC_PHY_PROFILE                   0
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_A
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36010103
        <#else>
#define DRV_PLC_PHY_PROFILE                   1
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_B
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36040103
        </#if>
    <#else>
#define DRV_PLC_PHY_PROFILE                   3
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_ARIB
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36030103
    </#if>
</#if>
#define DRV_PLC_PHY_HOST_DESC                 "${__PROCESSOR?string}"
#define DRV_PLC_PHY_HOST_MODEL                3
#define DRV_PLC_PHY_HOST_BAND                 DRV_PLC_PHY_PROFILE