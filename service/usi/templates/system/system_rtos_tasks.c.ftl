
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>static void lSRV_USI${INDEX}_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        SRV_USI_Tasks(sysObj.srvUSI${INDEX});
    <#lt>    }
    <#lt>}
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>TX_THREAD      lSRV_USI${INDEX}_Task_TCB;
    <#lt>uint8_t*       lSRV_USI${INDEX}_Task_Stk_Ptr;

    <#lt>static void lSRV_USI${INDEX}_Tasks( ULONG thread_input )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        SRV_USI_Tasks(sysObj.srvUSI${INDEX});
    <#lt>    }
    <#lt>}
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MicriumOSIII">
    <#lt>OS_TCB  lSRV_USI${INDEX}_Tasks_TCB;
    <#lt>CPU_STK lSRV_USI${INDEX}_TasksStk[SRV_USI${INDEX}_RTOS_STACK_SIZE];

    <#lt>static void lSRV_USI${INDEX}_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    OS_ERR os_err;
    <#lt>    while(true)
    <#lt>    {
    <#lt>        SRV_USI_Tasks(sysObj.srvUSI${INDEX});
    <#lt>    }
    <#lt>}
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MbedOS">
    <#lt>static void lSRV_USI${INDEX}_Tasks( void *pvParameters )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        SRV_USI_Tasks(sysObj.srvUSI${INDEX});
    <#lt>    }
    <#lt>}
</#if>
