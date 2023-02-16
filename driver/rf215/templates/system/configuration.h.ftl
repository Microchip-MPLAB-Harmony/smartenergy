/* RF215 Driver Configuration Options */
#define DRV_RF215_INDEX_0                     0
#define DRV_RF215_CLIENTS_NUMBER              ${DRV_RF215_NUM_CLIENTS}
#define DRV_RF215_TX_BUFFERS_NUMBER           ${DRV_RF215_NUM_TX_BUFFERS}
<#if DRV_RF215_SPI_NUM_CSR != 0>
#define DRV_RF215_CSR_INDEX                   ${DRV_RF215_SPI_NPCS}
<#if DRV_RF215_PLIB == "SRV_SPISPLIT">
#define DRV_RF215_SPI_CHIP_SELECT             SPI_CHIP_SELECT_NPCS${DRV_RF215_SPI_NPCS}
</#if>
</#if>
<#if core.DMA_ENABLE?has_content>
<#if DRV_RF215_TX_DMA_CHANNEL gte 0>
#define DRV_RF215_SPI_TX_DMA_CH               SYS_DMA_CHANNEL_${DRV_RF215_TX_DMA_CHANNEL}
<#else>
#define DRV_RF215_SPI_TX_DMA_CH               SYS_DMA_CHANNEL_NONE
</#if>
<#if DRV_RF215_RX_DMA_CHANNEL gte 0>
#define DRV_RF215_SPI_RX_DMA_CH               SYS_DMA_CHANNEL_${DRV_RF215_RX_DMA_CHANNEL}
<#else>
#define DRV_RF215_SPI_RX_DMA_CH               SYS_DMA_CHANNEL_NONE
</#if>
</#if>
#define DRV_RF215_EXT_INT_PIN                 ${DRV_RF215_EXT_INT_PIN}
#define DRV_RF215_RESET_PIN                   ${DRV_RF215_RESET_PIN}
<#if DRV_RF215_USE_LED_TX == true>
#define DRV_RF215_LED_TX_PIN                  ${DRV_RF215_LED_TX_PIN}
</#if>
<#if DRV_RF215_USE_LED_RX == true>
#define DRV_RF215_LED_RX_PIN                  ${DRV_RF215_LED_RX_PIN}
</#if>
#define DRV_RF215_NUM_TRX                     <#if DRV_RF215_TRX09_EN == true && DRV_RF215_TRX24_EN == true>2<#else>1</#if>
#define DRV_RF215_FCS_LEN                     ${DRV_RF215_FCS_MODE}
#define DRV_RF215_MAX_PSDU_LEN                ${DRV_RF215_MAX_PSDU_LEN}
<#if DRV_RF215_TXRX_TIME_SUPPORT == true>
#define DRV_RF215_MAX_TX_TIME_DELAY_ERROR_US  ${DRV_RF215_TX_TIME_ERROR}
<#if core.CoreArchitecture == "CORTEX-M7">
<#if core.DATA_CACHE_ENABLE?? && core.DATA_CACHE_ENABLE == true && core.INSTRUCTION_CACHE_ENABLE?? && core.INSTRUCTION_CACHE_ENABLE == true >
#define DRV_RF215_TIME_SYNC_EXECUTION_CYCLES  70
#define DRV_RF215_TX_COMMAND_EXECUTION_CYCLES 660
<#elseif core.DATA_CACHE_ENABLE?? && core.DATA_CACHE_ENABLE == true >
#define DRV_RF215_TIME_SYNC_EXECUTION_CYCLES  300
#define DRV_RF215_TX_COMMAND_EXECUTION_CYCLES 3000
<#elseif core.INSTRUCTION_CACHE_ENABLE?? && core.INSTRUCTION_CACHE_ENABLE == true >
#define DRV_RF215_TIME_SYNC_EXECUTION_CYCLES  230
#define DRV_RF215_TX_COMMAND_EXECUTION_CYCLES 1650
<#else>
#define DRV_RF215_TIME_SYNC_EXECUTION_CYCLES  410
#define DRV_RF215_TX_COMMAND_EXECUTION_CYCLES 3600
</#if>
<#elseif core.CoreArchitecture == "CORTEX-M4">
#define DRV_RF215_TIME_SYNC_EXECUTION_CYCLES  180
#define DRV_RF215_TX_COMMAND_EXECUTION_CYCLES 1400
<#else>
#define DRV_RF215_TIME_SYNC_EXECUTION_CYCLES  100 /* TODO: Measure execution cycles for this core architecture */
#define DRV_RF215_TX_COMMAND_EXECUTION_CYCLES 500 /* TODO: Measure execution cycles for this core architecture */
</#if>
</#if>
