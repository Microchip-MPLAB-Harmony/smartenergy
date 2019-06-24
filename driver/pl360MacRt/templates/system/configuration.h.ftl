/* PL360 MAC RT Driver Configuration Options */
#define DRV_PL360_MACRT_INSTANCES_NUMBER        ${DRV_PL360_MACRT_NUM_INSTANCES}
#define DRV_PL360_MACRT_INDEX                   0
#define DRV_PL360_MACRT_CLIENTS_NUMBER_IDX      ${DRV_PL360_MACRT_NUM_CLIENTS?string}
#define DRV_PL360_EXT_INT_SRC                   ${DRV_PL360_EXT_INT_SRC?string}
#define DRV_PL360_EXT_INT_PIN                   ${DRV_PL360_EXT_INT_PIN?string}
#define DRV_PL360_RESET_PIN                     ${DRV_PL360_RESET_PIN?string}
#define DRV_PL360_LDO_EN_PIN                    ${DRV_PL360_LDO_EN_PIN?string}
<#if DRV_PL360_MACRT_PLC_PROFILE == "5">
#define DRV_PL360_MACRT_PLC_PROFILE             0
<#else>
#define DRV_PL360_MACRT_PLC_PROFILE             ${DRV_PL360_MACRT_PLC_PROFILE?string}
</#if>
<#if DRV_PL360_MACRT_PLC_PROFILE == "2">
#define DRV_PL360_SPI_CLK                       12000000
<#else>
#define DRV_PL360_SPI_CLK                       8000000
</#if>

#define DRV_PL360_SECURE                        ${DRV_PL360_SECURE_MODE?string}
