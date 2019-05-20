/* PL360 Driver Configuration Options */
#define DRV_PL360_INSTANCES_NUMBER              ${DRV_PL360_NUM_INSTANCES}
#define DRV_PL360_INDEX                         0
#define DRV_PL360_CLIENTS_NUMBER_IDX            ${DRV_PL360_NUM_CLIENTS?string}
#define DRV_PL360_EXT_INT_SRC                   ${DRV_PL360_EXT_INT_SRC?string}
#define DRV_PL360_EXT_INT_PIN                   ${DRV_PL360_EXT_INT_PIN?string}
#define DRV_PL360_RESET_PIN                     ${DRV_PL360_RESET_PIN?string}
#define DRV_PL360_LDO_EN_PIN                    ${DRV_PL360_LDO_EN_PIN?string}
<#if DRV_PL360_PLC_PROFILE == "5">
#define DRV_PL360_PLC_PROFILE                   0
<#else>
#define DRV_PL360_PLC_PROFILE                   ${DRV_PL360_PLC_PROFILE?string}
</#if>
<#if DRV_PL360_PLC_PROFILE == "2">
#define DRV_PL360_SPI_CLK                       12000000
<#else>
#define DRV_PL360_SPI_CLK                       8000000
</#if>

<#if DRV_PL360_BIN_STATIC_ADDRESSING == true>
#define DRV_PL360_BIN_ADDRESS                   0x${DRV_PL360_PLC_BIN_ADDRESS?string}
#define DRV_PL360_BIN_SIZE                      0x${DRV_PL360_PLC_BIN_SIZE?string}
</#if>
#define DRV_PL360_SECURE                        ${DRV_PL360_SECURE_MODE?string}
/* PL360 Driver Identification */
<#if DRV_PL360_PLC_PROFILE == "0">
#define DRV_PL360_NUM_CARRIERS                  NUM_CARRIERS_CENELEC_A
#define DRV_PL360_HOST_PRODUCT                  0x3601
#define DRV_PL360_HOST_VERSION                  0x36010300
#define DRV_PL360_HOST_PHY                      0x36010103
<#elseif DRV_PL360_PLC_PROFILE == "1">
#define DRV_PL360_NUM_CARRIERS                  NUM_CARRIERS_CENELEC_B
#define DRV_PL360_HOST_PRODUCT                  0x3601
#define DRV_PL360_HOST_VERSION                  0x36010300
#define DRV_PL360_HOST_PHY                      0x36040103
<#elseif DRV_PL360_PLC_PROFILE == "2">
#define DRV_PL360_NUM_CARRIERS                  NUM_CARRIERS_FCC
#define DRV_PL360_HOST_PRODUCT                  0x3601
#define DRV_PL360_HOST_VERSION                  0x36010300
#define DRV_PL360_HOST_PHY                      0x36020103
<#elseif DRV_PL360_PLC_PROFILE == "4">
#define DRV_PL360_NUM_CARRIERS                  97
#define DRV_PL360_HOST_PRODUCT                  0x3600
#define DRV_PL360_HOST_VERSION                  0x36000300
#define DRV_PL360_HOST_PHY                      0x36000003
<#elseif DRV_PL360_PLC_PROFILE == "5">
#define DRV_PL360_NUM_CARRIERS                  NUM_CARRIERS_CENELEC_A
#define DRV_PL360_HOST_PRODUCT                  0x3601
#define DRV_PL360_HOST_VERSION                  0x36010300
#define DRV_PL360_HOST_PHY                      0x36010103
</#if>
#define DRV_PL360_HOST_DESC                     "${__PROCESSOR?string}"
#define DRV_PL360_HOST_MODEL                    3
#define DRV_PL360_HOST_BAND                     DRV_PL360_PLC_PROFILE