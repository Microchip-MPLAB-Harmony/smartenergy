
<#if !(HarmonyCore.SELECT_RTOS)?? || (HarmonyCore.SELECT_RTOS == "BareMetal")>
    SRV_USI_Tasks(sysObj.srvUSI${INDEX});
<#elseif HarmonyCore.SELECT_RTOS == "FreeRTOS">
    (void) xTaskCreate( lSRV_USI${INDEX}_Tasks,
        "SRV_USI${INDEX}_TASKS",
        SRV_USI${INDEX}_RTOS_STACK_SIZE,
        (void*)NULL,
        SRV_USI${INDEX}_RTOS_TASK_PRIORITY,
        (TaskHandle_t*)NULL
    );
<#elseif HarmonyCore.SELECT_RTOS == "ThreadX">
    (void) tx_byte_allocate(&byte_pool_0,
        (VOID **) &lSRV_USI${INDEX}_Task_Stk_Ptr,
        SRV_USI${INDEX}_RTOS_STACK_SIZE,
        TX_NO_WAIT
    );

    (void) tx_thread_create(&lSRV_USI${INDEX}_Task_TCB,
        "SRV_USI${INDEX}_TASKS",
        lSRV_USI${INDEX}_Tasks,
        0,
        lSRV_USI${INDEX}_Task_Stk_Ptr,
        SRV_USI${INDEX}_RTOS_STACK_SIZE,
        SRV_USI${INDEX}_RTOS_TASK_PRIORITY,
        SRV_USI${INDEX}_RTOS_TASK_PRIORITY,
        TX_NO_TIME_SLICE,
        TX_AUTO_START
    );
<#elseif HarmonyCore.SELECT_RTOS == "MicriumOSIII">
    <#assign SRV_USI_RTOS_TASK_OPTIONS = "OS_OPT_TASK_NONE" + SRV_USI_RTOS_TASK_OPT_STK_CHK?then(' | OS_OPT_TASK_STK_CHK', '') + SRV_USI_RTOS_TASK_OPT_STK_CLR?then(' | OS_OPT_TASK_STK_CLR', '') + SRV_USI_RTOS_TASK_OPT_SAVE_FP?then(' | OS_OPT_TASK_SAVE_FP', '') + SRV_USI_RTOS_TASK_OPT_NO_TLS?then(' | OS_OPT_TASK_NO_TLS', '')>
    (void) OSTaskCreate((OS_TCB      *)&lSRV_USI${INDEX}_Tasks_TCB,
                 (CPU_CHAR    *)"SRV_USI${INDEX}_TASKS",
                 (OS_TASK_PTR  )lSRV_USI${INDEX}_Tasks,
                 (void        *)0,
                 (OS_PRIO      )SRV_USI${INDEX}_RTOS_TASK_PRIORITY,
                 (CPU_STK     *)&lSRV_USI${INDEX}_TasksStk[0],
                 (CPU_STK_SIZE )0u,
                 (CPU_STK_SIZE )SRV_USI${INDEX}_RTOS_STACK_SIZE,
    <#if MicriumOSIII.UCOSIII_CFG_TASK_Q_EN == true>
                 (OS_MSG_QTY   )SRV_USI${INDEX}_RTOS_TASK_MSG_QTY,
    <#else>
                 (OS_MSG_QTY   )0u,
    </#if>
    <#if MicriumOSIII.UCOSIII_CFG_SCHED_ROUND_ROBIN_EN == true>
                 (OS_TICK      )SRV_USI${INDEX}_RTOS_TASK_TIME_QUANTA,
    <#else>
                 (OS_TICK      )0u,
    </#if>
                 (void        *)0,
                 (OS_OPT       )(${SRV_USI_RTOS_TASK_OPTIONS}),
                 (OS_ERR      *)&os_err);
<#elseif HarmonyCore.SELECT_RTOS == "MbedOS">
    Thread SRV_USI${INDEX}_thread((osPriority)(osPriorityNormal + (SRV_USI${INDEX}_RTOS_TASK_PRIORITY - 1)), SRV_USI${INDEX}_RTOS_STACK_SIZE, NULL, "lSRV_USI${INDEX}_Tasks");
    (void) SRV_USI${INDEX}_thread.start(callback(lSRV_USI${INDEX}_Tasks, (void *)NULL));
</#if>