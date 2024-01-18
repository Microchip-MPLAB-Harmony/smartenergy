
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>static void lDRV_RF215_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        /* Maintain RF215 Driver */
    <#lt>        DRV_RF215_Tasks(sysObj.drvRf215);
    <#lt>    }
    <#lt>}
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>TX_THREAD      lDRV_RF215_Task_TCB;
    <#lt>uint8_t*       lDRV_RF215_Task_Stk_Ptr;

    <#lt>static void lDRV_RF215_Tasks( ULONG thread_input )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        /* Maintain RF215 Driver */
    <#lt>        DRV_RF215_Tasks(sysObj.drvRf215);
    <#lt>    }
    <#lt>}
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MicriumOSIII">
    <#lt>OS_TCB  lDRV_RF215_Tasks_TCB;
    <#lt>CPU_STK lDRV_RF215_TasksStk[DRV_RF215_RTOS_STACK_SIZE];

    <#lt>static void lDRV_RF215_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    OS_ERR os_err;
    <#lt>    while(true)
    <#lt>    {
    <#lt>        /* Maintain RF215 Driver */
    <#lt>        DRV_RF215_Tasks(sysObj.drvRf215);
    <#lt>    }
    <#lt>}
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MbedOS">
    <#lt>static void lDRV_RF215_Tasks( void *pvParameters )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        /* Maintain RF215 Driver */
    <#lt>        DRV_RF215_Tasks(sysObj.drvRf215);
    <#lt>    }
    <#lt>}
</#if>
