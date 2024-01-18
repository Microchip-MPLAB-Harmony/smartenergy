
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>static void lDRV_PLC_PHY_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        /* Maintain PLC PHY Driver */
    <#lt>        DRV_PLC_PHY_Tasks(sysObj.drvPlcPhy);
    <#lt>    }
    <#lt>}
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>TX_THREAD      lDRV_PLC_PHY_Task_TCB;
    <#lt>uint8_t*       lDRV_PLC_PHY_Task_Stk_Ptr;

    <#lt>static void lDRV_PLC_PHY_Tasks( ULONG thread_input )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        /* Maintain PLC PHY Driver */
    <#lt>        DRV_PLC_PHY_Tasks(sysObj.drvPlcPhy);
    <#lt>    }
    <#lt>}
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MicriumOSIII">
    <#lt>OS_TCB  lDRV_PLC_PHY_Tasks_TCB;
    <#lt>CPU_STK lDRV_PLC_PHY_TasksStk[DRV_PLC_PHY_RTOS_STACK_SIZE];

    <#lt>static void lDRV_PLC_PHY_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    OS_ERR os_err;
    <#lt>    while(true)
    <#lt>    {
    <#lt>        /* Maintain PLC PHY Driver */
    <#lt>        DRV_PLC_PHY_Tasks(sysObj.drvPlcPhy);
    <#lt>    }
    <#lt>}
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MbedOS">
    <#lt>static void lDRV_PLC_PHY_Tasks( void *pvParameters )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        /* Maintain PLC PHY Driver */
    <#lt>        DRV_PLC_PHY_Tasks(sysObj.drvPlcPhy);
    <#lt>    }
    <#lt>}
</#if>
