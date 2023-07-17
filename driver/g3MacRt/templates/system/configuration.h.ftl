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
/* PLC MAC RT Configuration Options */
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
#define DRV_PLC_EXT_INT_PIO                   ${DRV_PLC_EXT_INT_PIN?string}
#define DRV_PLC_EXT_INT_PIN                   ${DRV_PLC_EIC_SIGNAL?string}
<#else>
#define DRV_PLC_EXT_INT_PIO_PORT              ${DRV_PLC_EXT_INT_PIO_PORT?string}
#define DRV_PLC_EXT_INT_SRC                   ${DRV_PLC_EXT_INT_SRC?string}
#define DRV_PLC_EXT_INT_PIO                   ${DRV_PLC_EXT_INT_PIN?string}
#define DRV_PLC_EXT_INT_PIN                   ${DRV_PLC_EXT_INT_PIN?string}
</#if>
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
<#if DRV_PLC_SPI_NUM_CSR != 0>
  <#--  PLIB with multiple CSR/NPCS  -->
  <#if SPI_PLIB?contains("FLEXCOM")>
    <#assign PLIB_SPI_CSR = "${SPI_PLIB?lower_case}.FLEXCOM_SPI_CSR${DRV_PLC_SPI_NPCS}">
  <#else>
    <#assign PLIB_SPI_CSR = "${SPI_PLIB?lower_case}.SPI_CSR${DRV_PLC_SPI_NPCS}">
  </#if>
  <#assign PLIB_SPI_BAUD_RATE = ("${PLIB_SPI_CSR}_BAUD_RATE")?eval>
<#else>
  <#if SPI_PLIB?contains("USART")>
    <#assign PLIB_SPI_BAUD_RATE = .vars["${SPI_PLIB?lower_case}"].USART_SPI_BAUD_RATE>
  <#elseif SPI_PLIB?contains("SERCOM") || SPI_PLIB?contains("SPI")>
    <#assign PLIB_SPI_BAUD_RATE = .vars["${SPI_PLIB?lower_case}"].SPI_BAUD_RATE>
  <#else>
    <#-- PLIB not supported, set baudrate to 8MHz  -->
    <#assign PLIB_SPI_BAUD_RATE = 8000000>
  </#if>
</#if>
#define DRV_PLC_SPI_CLK                       ${PLIB_SPI_BAUD_RATE}

<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    <#lt>/* PLC MAC RT Driver RTOS Configuration Options */
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
/* PLC MAC RT Driver Identification */
#define DRV_G3_MACRT_INDEX                   0U
#define DRV_G3_MACRT_INSTANCES_NUMBER        1U
#define DRV_G3_MACRT_HOST_DESC               "${__PROCESSOR?string}"