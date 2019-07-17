/* PLC MAC RT Driver Configuration Options */
#define DRV_PLC_SECURE                       ${DRV_PLC_SECURE_MODE?string}
#define DRV_PLC_EXT_INT_PIO_PORT             ${DRV_PLC_EXT_INT_PIO_PORT?string}
#define DRV_PLC_EXT_INT_SRC                  ${DRV_PLC_EXT_INT_SRC?string}
#define DRV_PLC_EXT_INT_PIN                  ${DRV_PLC_EXT_INT_PIN?string}
#define DRV_PLC_RESET_PIN                    ${DRV_PLC_RESET_PIN?string}
#define DRV_PLC_LDO_EN_PIN                   ${DRV_PLC_LDO_EN_PIN?string}
#define DRV_PLC_CD_PIN                       ${DRV_PLC_CD_PIN?string}
<#if DRV_G3_MACRT_PLC_PROFILE == "0">
#define DRV_PLC_SPI_CLK                      8000000
#define DRV_G3_MACRT_PLC_PROFILE             ${DRV_G3_MACRT_PLC_PROFILE?string}
<#elseif DRV_G3_MACRT_PLC_PROFILE == "1">
#define DRV_PLC_SPI_CLK                      8000000
#define DRV_G3_MACRT_PLC_PROFILE             ${DRV_G3_MACRT_PLC_PROFILE?string}
<#elseif DRV_G3_MACRT_PLC_PROFILE == "2">
#define DRV_PLC_SPI_CLK                      12000000
#define DRV_G3_MACRT_PLC_PROFILE             ${DRV_G3_MACRT_PLC_PROFILE?string}
<#elseif DRV_G3_MACRT_PLC_PROFILE == "3">
#define DRV_PLC_SPI_CLK                      12000000
#define DRV_G3_MACRT_PLC_PROFILE             ${DRV_G3_MACRT_PLC_PROFILE?string}
<#elseif DRV_G3_MACRT_PLC_PROFILE == "4">
#define DRV_PLC_SPI_CLK                      8000000
#define DRV_G3_MACRT_PLC_PROFILE             0
</#if>
#define DRV_G3_MACRT_INSTANCES_NUMBER        ${DRV_G3_MACRT_NUM_INSTANCES}
#define DRV_G3_MACRT_INDEX                   0
#define DRV_G3_MACRT_CLIENTS_NUMBER_IDX      ${DRV_G3_MACRT_NUM_CLIENTS?string}
#define DRV_G3_MACRT_SPEC_COMPLIANCE         ${DRV_G3_MACRT_SPEC_COMPLIANCE?string}