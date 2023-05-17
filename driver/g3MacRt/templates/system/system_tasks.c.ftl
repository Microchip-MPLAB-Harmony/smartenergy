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

<#if !(HarmonyCore.SELECT_RTOS)?? || (HarmonyCore.SELECT_RTOS == "BareMetal")>
    /* Maintain G3 MAC RT Driver */
    DRV_G3_MACRT_Tasks(sysObj.drvG3MacRt);
<#elseif HarmonyCore.SELECT_RTOS == "FreeRTOS">
    xTaskCreate( _DRV_G3_MACRT_Tasks,
        "DRV_G3_MACRT_TASKS",
        DRV_PLC_RTOS_STACK_SIZE,
        (void*)NULL,
        DRV_PLC_RTOS_TASK_PRIORITY,
        (TaskHandle_t*)NULL
    );
<#elseif HarmonyCore.SELECT_RTOS == "ThreadX">
    tx_byte_allocate(&byte_pool_0,
        (VOID **) &_DRV_G3_MACRT_Task_Stk_Ptr,
        DRV_PLC_RTOS_STACK_SIZE,
        TX_NO_WAIT
    );

    tx_thread_create(&_DRV_G3_MACRT_Task_TCB,
        "DRV_G3_MACRT_TASKS",
        _DRV_G3_MACRT_Tasks,
        0,
        _DRV_G3_MACRT_Task_Stk_Ptr,
        DRV_PLC_RTOS_STACK_SIZE,
        DRV_PLC_RTOS_TASK_PRIORITY,
        DRV_PLC_RTOS_TASK_PRIORITY,
        TX_NO_TIME_SLICE,
        TX_AUTO_START
    );
<#elseif HarmonyCore.SELECT_RTOS == "MicriumOSIII">
    <#assign DRV_PLC_RTOS_TASK_OPTIONS = "OS_OPT_TASK_NONE" + DRV_PLC_RTOS_TASK_OPT_STK_CHK?then(' | OS_OPT_TASK_STK_CHK', '') + DRV_PLC_RTOS_TASK_OPT_STK_CLR?then(' | OS_OPT_TASK_STK_CLR', '') + DRV_PLC_RTOS_TASK_OPT_SAVE_FP?then(' | OS_OPT_TASK_SAVE_FP', '') + DRV_PLC_RTOS_TASK_OPT_NO_TLS?then(' | OS_OPT_TASK_NO_TLS', '')>
    OSTaskCreate((OS_TCB      *)&_DRV_G3_MACRT_Tasks_TCB,
                 (CPU_CHAR    *)"DRV_G3_MACRT_TASKS",
                 (OS_TASK_PTR  )_DRV_G3_MACRT_Tasks,
                 (void        *)0,
                 (OS_PRIO      )DRV_PLC_RTOS_TASK_PRIORITY,
                 (CPU_STK     *)&_DRV_G3_MACRT_TasksStk[0],
                 (CPU_STK_SIZE )0u,
                 (CPU_STK_SIZE )DRV_PLC_RTOS_STACK_SIZE,
    <#if MicriumOSIII.UCOSIII_CFG_TASK_Q_EN == true>
                 (OS_MSG_QTY   )DRV_PLC_RTOS_TASK_MSG_QTY,
    <#else>
                 (OS_MSG_QTY   )0u,
    </#if>
    <#if MicriumOSIII.UCOSIII_CFG_SCHED_ROUND_ROBIN_EN == true>
                 (OS_TICK      )DRV_PLC_RTOS_TASK_TIME_QUANTA,
    <#else>
                 (OS_TICK      )0u,
    </#if>
                 (void        *)0,
                 (OS_OPT       )(${DRV_PLC_RTOS_TASK_OPTIONS}),
                 (OS_ERR      *)&os_err);
<#elseif HarmonyCore.SELECT_RTOS == "MbedOS">
    Thread DRV_G3_MACRT_thread((osPriority)(osPriorityNormal + (DRV_PLC_RTOS_TASK_PRIORITY - 1)), DRV_PLC_RTOS_STACK_SIZE, NULL, "_DRV_G3_MACRT_Tasks");
    DRV_G3_MACRT_thread.start(callback(_DRV_G3_MACRT_Tasks, (void *)NULL));
</#if>