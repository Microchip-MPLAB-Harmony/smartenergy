
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>static void lDRV_G3_MACRT_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        /* Maintain G3 MAC RT Driver */
    <#lt>        DRV_G3_MACRT_Tasks(sysObj.drvG3MacRt);
    <#lt>    }
    <#lt>}
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>TX_THREAD      lDRV_G3_MACRT_Task_TCB;
    <#lt>uint8_t*       lDRV_G3_MACRT_Task_Stk_Ptr;

    <#lt>static void lDRV_G3_MACRT_Tasks( ULONG thread_input )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        /* Maintain G3 MAC RT Driver */
    <#lt>        DRV_G3_MACRT_Tasks(sysObj.drvG3MacRt);
    <#lt>    }
    <#lt>}
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MicriumOSIII">
    <#lt>OS_TCB  lDRV_G3_MACRT_Tasks_TCB;
    <#lt>CPU_STK lDRV_G3_MACRT_TasksStk[DRV_G3_MACRT_RTOS_STACK_SIZE];

    <#lt>static void lDRV_G3_MACRT_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    OS_ERR os_err;
    <#lt>    while(true)
    <#lt>    {
    <#lt>        /* Maintain G3 MAC RT Driver */
    <#lt>        DRV_G3_MACRT_Tasks(sysObj.drvG3MacRt);
    <#lt>    }
    <#lt>}
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MbedOS">
    <#lt>static void lDRV_G3_MACRT_Tasks( void *pvParameters )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        /* Maintain G3 MAC RT Driver */
    <#lt>        DRV_G3_MACRT_Tasks(sysObj.drvG3MacRt);
    <#lt>    }
    <#lt>}
</#if>
