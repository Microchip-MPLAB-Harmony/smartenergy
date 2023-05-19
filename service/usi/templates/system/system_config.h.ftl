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
/* USI Service Instance ${INDEX?string} Configuration Options */
#define SRV_USI_INDEX_${INDEX?string}                       ${INDEX?string}
#define SRV_USI${INDEX?string}_RD_BUF_SIZE                  ${SRV_USI_RD_BUF_SIZE?string}
#define SRV_USI${INDEX?string}_WR_BUF_SIZE                  ${SRV_USI_WR_BUF_SIZE?string}
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    <#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
        <#lt>#define SRV_USI${INDEX?string}_RTOS_STACK_SIZE               ${SRV_USI_RTOS_STACK_SIZE / 4}
    <#else>
        <#lt>#define SRV_USI${INDEX?string}_RTOS_STACK_SIZE               ${SRV_USI_RTOS_STACK_SIZE}
    </#if>
    <#lt>#define SRV_USI${INDEX?string}_RTOS_TASK_PRIORITY            ${SRV_USI_RTOS_TASK_PRIORITY}
    <#if HarmonyCore.SELECT_RTOS == "MicriumOSIII">
        <#lt>#define SRV_USI${INDEX?string}_RTOS_TASK_MSG_QTY             ${SRV_USI_RTOS_TASK_MSG_QTY}u
        <#lt>#define SRV_USI${INDEX?string}_RTOS_TASK_TIME_QUANTA         ${SRV_USI_RTOS_TASK_TIME_QUANTA}u
    </#if>
</#if>
