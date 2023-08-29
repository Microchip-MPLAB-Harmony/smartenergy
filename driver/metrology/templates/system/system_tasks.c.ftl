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

<#if HarmonyCore.SELECT_RTOS == "BareMetal">
    <#lt>   DRV_METROLOGY_Tasks(sysObj.drvMet);
<#elseif HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>    xTaskCreate(lDRV_METROLOGY_Tasks,
    <#lt>        "DRV_METROLOGY_TASKS",
    <#lt>        DRV_METROLOGY_RTOS_STACK_SIZE,
    <#lt>        (void*)NULL,
    <#lt>        DRV_METROLOGY_RTOS_TASK_PRIORITY,
    <#lt>        (TaskHandle_t*)NULL
    <#lt>    );
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>    tx_byte_allocate(&byte_pool_0,
    <#lt>        (VOID **) &lDRV_METROLOGY_Task_Stk_Ptr,
    <#lt>        DRV_METROLOGY_RTOS_STACK_SIZE,
    <#lt>        TX_NO_WAIT
    <#lt>    );

    <#lt>    tx_thread_create(&lDRV_METROLOGY_Task_TCB,
    <#lt>        "DRV_METROLOGY_TASKS",
    <#lt>        lDRV_METROLOGY_Tasks,
    <#lt>        0U,
    <#lt>        lDRV_METROLOGY_Task_Stk_Ptr,
    <#lt>        DRV_METROLOGY_RTOS_STACK_SIZE,
    <#lt>        DRV_METROLOGY_RTOS_TASK_PRIORITY,
    <#lt>        DRV_METROLOGY_RTOS_TASK_PRIORITY,
    <#lt>        TX_NO_TIME_SLICE,
    <#lt>        TX_AUTO_START
    <#lt>    );
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MicriumOSIII">
    <#assign DRV_METROLOGY_RTOS_TASK_OPTIONS = "OS_OPT_TASK_NONE" + DRV_MET_RTOS_TASK_OPT_STK_CHK?then(' | OS_OPT_TASK_STK_CHK', '') + DRV_MET_RTOS_TASK_OPT_STK_CLR?then(' | OS_OPT_TASK_STK_CLR', '') + DRV_MET_RTOS_TASK_OPT_SAVE_FP?then(' | OS_OPT_TASK_SAVE_FP', '') + DRV_MET_RTOS_TASK_OPT_NO_TLS?then(' | OS_OPT_TASK_NO_TLS', '')>
    <#lt>    OSTaskCreate((OS_TCB      *)&lDRV_METROLOGY_Tasks_TCB,
    <#lt>                 (CPU_CHAR    *)"DRV_METROLOGY_TASKS",
    <#lt>                 (OS_TASK_PTR  )lDRV_METROLOGY_Tasks,
    <#lt>                 (void        *)0U,
    <#lt>                 (OS_PRIO      )DRV_METROLOGY_RTOS_TASK_PRIORITY,
    <#lt>                 (CPU_STK     *)&lDRV_METROLOGY_TasksStk[0],
    <#lt>                 (CPU_STK_SIZE )0U,
    <#lt>                 (CPU_STK_SIZE )DRV_METROLOGY_RTOS_STACK_SIZE,
    <#if MicriumOSIII.UCOSIII_CFG_TASK_Q_EN == true>
    <#lt>                 (OS_MSG_QTY   )DRV_METROLOGY_RTOS_TASK_MSG_QTY,
    <#else>
    <#lt>                 (OS_MSG_QTY   )0U,
    </#if>
    <#if MicriumOSIII.UCOSIII_CFG_SCHED_ROUND_ROBIN_EN == true>
    <#lt>                 (OS_TICK      )DRV_METROLOGY_RTOS_TASK_TIME_QUANTA,
    <#else>
    <#lt>                 (OS_TICK      )0U,
    </#if>
    <#lt>                 (void        *)0U,
    <#lt>                 (OS_OPT       )(${DRV_MET_RTOS_TASK_OPTIONS}),
    <#lt>                 (OS_ERR      *)&os_err);
<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MbedOS">
    <#lt>    Thread DRV_METROLOGY_thread((osPriority)(osPriorityNormal + (DRV_METROLOGY_RTOS_TASK_PRIORITY - 1)), DRV_METROLOGY_RTOS_STACK_SIZE, NULL, "lDRV_METROLOGY_Tasks");
    <#lt>    DRV_METROLOGY_thread.start(callback(lDRV_METROLOGY_Tasks, (void *)NULL));
</#if>
