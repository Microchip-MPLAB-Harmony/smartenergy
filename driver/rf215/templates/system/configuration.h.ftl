/* RF215 Driver Configuration Options */
#define DRV_RF215_INDEX_0                     0
#define DRV_RF215_CLIENTS_NUMBER              ${DRV_RF215_NUM_CLIENTS}
#define DRV_RF215_TX_BUFFERS_NUMBER           ${DRV_RF215_NUM_TX_BUFFERS}
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    <#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
        <#lt>#define DRV_RF215_RTOS_STACK_SIZE             ${DRV_RF215_RTOS_STACK_SIZE / 4}
    <#else>
        <#lt>#define DRV_RF215_RTOS_STACK_SIZE             ${DRV_RF215_RTOS_STACK_SIZE}
    </#if>
    <#lt>#define DRV_RF215_RTOS_TASK_PRIORITY          ${DRV_RF215_RTOS_TASK_PRIORITY}
    <#if HarmonyCore.SELECT_RTOS == "MicriumOSIII">
        <#lt>#define DRV_RF215_RTOS_TASK_MSG_QTY           ${DRV_RF215_RTOS_TASK_MSG_QTY}u
        <#lt>#define DRV_RF215_RTOS_TASK_TIME_QUANTA       ${DRV_RF215_RTOS_TASK_TIME_QUANTA}u
    </#if>
</#if>
<#if DRV_RF215_SPI_NUM_CSR != 0>
    <#lt>#define DRV_RF215_CSR_INDEX                   ${DRV_RF215_SPI_NPCS}
    <#if DRV_RF215_PLIB == "SRV_SPISPLIT">
        <#lt>#define DRV_RF215_SPI_CHIP_SELECT             SPI_CHIP_SELECT_NPCS${DRV_RF215_SPI_NPCS}
    </#if>
</#if>
<#if core.DMA_ENABLE?has_content>
    <#if DRV_RF215_TX_DMA_CHANNEL gte 0>
        <#lt>#define DRV_RF215_SPI_TX_DMA_CH               SYS_DMA_CHANNEL_${DRV_RF215_TX_DMA_CHANNEL}
    <#else>
        <#lt>#define DRV_RF215_SPI_TX_DMA_CH               SYS_DMA_CHANNEL_NONE
    </#if>
    <#if DRV_RF215_RX_DMA_CHANNEL gte 0>
        <#lt>#define DRV_RF215_SPI_RX_DMA_CH               SYS_DMA_CHANNEL_${DRV_RF215_RX_DMA_CHANNEL}
    <#else>
        <#lt>#define DRV_RF215_SPI_RX_DMA_CH               SYS_DMA_CHANNEL_NONE
    </#if>
</#if>
#define DRV_RF215_EXT_INT_PIN                 ${DRV_RF215_EXT_INT_PIN}
#define DRV_RF215_RESET_PIN                   ${DRV_RF215_RESET_PIN}
<#if DRV_RF215_USE_LED_TX == true>
    <#lt>#define DRV_RF215_LED_TX_PIN                  ${DRV_RF215_LED_TX_PIN}
</#if>
<#if DRV_RF215_USE_LED_RX == true>
    <#lt>#define DRV_RF215_LED_RX_PIN                  ${DRV_RF215_LED_RX_PIN}
</#if>
#define DRV_RF215_NUM_TRX                     <#if DRV_RF215_TRX09_EN == true && DRV_RF215_TRX24_EN == true>2<#else>1</#if>
#define DRV_RF215_FCS_LEN                     ${DRV_RF215_FCS_MODE}
#define DRV_RF215_MAX_PSDU_LEN                ${DRV_RF215_MAX_PSDU_LEN}
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
    <#lt>#define DRV_RF215_MAX_TX_TIME_DELAY_ERROR_US  ${DRV_RF215_TX_TIME_ERROR}
    <#if core.CoreArchitecture == "CORTEX-M7">
        <#if core.DATA_CACHE_ENABLE?? && core.DATA_CACHE_ENABLE == true && core.INSTRUCTION_CACHE_ENABLE?? && core.INSTRUCTION_CACHE_ENABLE == true >
            <#lt>#define DRV_RF215_TIME_SYNC_EXECUTION_CYCLES  70
            <#lt>#define DRV_RF215_TX_COMMAND_EXECUTION_CYCLES 660
        <#elseif core.DATA_CACHE_ENABLE?? && core.DATA_CACHE_ENABLE == true >
            <#lt>#define DRV_RF215_TIME_SYNC_EXECUTION_CYCLES  300
            <#lt>#define DRV_RF215_TX_COMMAND_EXECUTION_CYCLES 3000
        <#elseif core.INSTRUCTION_CACHE_ENABLE?? && core.INSTRUCTION_CACHE_ENABLE == true >
            <#lt>#define DRV_RF215_TIME_SYNC_EXECUTION_CYCLES  230
            <#lt>#define DRV_RF215_TX_COMMAND_EXECUTION_CYCLES 1650
        <#else>
            <#lt>#define DRV_RF215_TIME_SYNC_EXECUTION_CYCLES  410
            <#lt>#define DRV_RF215_TX_COMMAND_EXECUTION_CYCLES 3600
        </#if>
    <#elseif core.CoreArchitecture == "CORTEX-M4">
        <#lt>#define DRV_RF215_TIME_SYNC_EXECUTION_CYCLES  180
        <#lt>#define DRV_RF215_TX_COMMAND_EXECUTION_CYCLES 1400
    <#else>
        <#lt>#define DRV_RF215_TIME_SYNC_EXECUTION_CYCLES  100 /* TODO: Measure execution cycles for this core architecture */
        <#lt>#define DRV_RF215_TX_COMMAND_EXECUTION_CYCLES 500 /* TODO: Measure execution cycles for this core architecture */
    </#if>
</#if>
