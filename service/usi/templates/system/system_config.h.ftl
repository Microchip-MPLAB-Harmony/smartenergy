
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
