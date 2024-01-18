
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>TX_THREAD      lDRV_METROLOGY_Task_TCB;
    <#lt>uint8_t*       lDRV_METROLOGY_Task_Stk_Ptr;

<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MicriumOSIII">
    <#lt>OS_TCB  lDRV_METROLOGY_Tasks_TCB;
    <#lt>CPU_STK lDRV_METROLOGY_TasksStk[DRV_METROLOGY_RTOS_STACK_SIZE];

</#if>
    <#lt>static void lDRV_METROLOGY_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        DRV_METROLOGY_Tasks(sysObj.drvMet);
    <#lt>    }
    <#lt>}
