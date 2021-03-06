/* PLC MAC RT Configuration Options */
#define DRV_PLC_SECURE                        ${DRV_PLC_SECURE_MODE?string}
#define DRV_PLC_EXT_INT_PIO_PORT              ${DRV_PLC_EXT_INT_PIO_PORT?string}
#define DRV_PLC_EXT_INT_SRC                   ${DRV_PLC_EXT_INT_SRC?string}
#define DRV_PLC_EXT_INT_PIN                   ${DRV_PLC_EXT_INT_PIN?string}
#define DRV_PLC_RESET_PIN                     ${DRV_PLC_RESET_PIN?string}
#define DRV_PLC_LDO_EN_PIN                    ${DRV_PLC_LDO_EN_PIN?string}
<#if DRV_PLC_MODE == "PL460">
#define DRV_PLC_TX_ENABLE_PIN                 ${DRV_PLC_TX_ENABLE_PIN?string}
</#if>
<#if DRV_PLC_SLEEP_MODE == true>
#define DRV_PLC_STBY_PIN                      ${DRV_PLC_STBY_PIN?string}
</#if>
<#if DRV_PLC_MODE == "PL460" && DRV_PLC_THERMAL_MONITOR == true>
#define DRV_PLC_THMON_PIN                     ${DRV_PLC_THMON_PIN?string}
</#if>

#define DRV_PLC_SPI_CLK                      8000000

/* PLC MAC RT Driver Identification */
#define DRV_G3_MACRT_INDEX                   0
#define DRV_G3_MACRT_INSTANCES_NUMBER        1
#define DRV_G3_MACRT_HOST_DESC               "${__PROCESSOR?string}"