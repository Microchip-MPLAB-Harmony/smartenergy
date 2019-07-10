/* PLC PHY Driver Configuration Options */
#define DRV_PLC_SECURE                        ${DRV_PLC_SECURE_MODE?string}
#define DRV_PLC_EXT_INT_SRC                   ${DRV_PLC_PHY_EXT_INT_SRC?string}
#define DRV_PLC_EXT_INT_PIN                   ${DRV_PLC_EXT_INT_PIN?string}
#define DRV_PLC_RESET_PIN                     ${DRV_PLC_RESET_PIN?string}
#define DRV_PLC_LDO_EN_PIN                    ${DRV_PLC_LDO_EN_PIN?string}
<#if DRV_PLC_PHY_PROFILE == "0">
#define DRV_PLC_SPI_CLK                       8000000
#define DRV_PLC_PHY_PROFILE                   ${DRV_PLC_PHY_PROFILE?string}
<#elseif DRV_PLC_PHY_PROFILE == "1">
#define DRV_PLC_SPI_CLK                       8000000
#define DRV_PLC_PHY_PROFILE                   ${DRV_PLC_PHY_PROFILE?string}
<#elseif DRV_PLC_PHY_PROFILE == "2">
#define DRV_PLC_SPI_CLK                       12000000
#define DRV_PLC_PHY_PROFILE                   ${DRV_PLC_PHY_PROFILE?string}
<#elseif DRV_PLC_PHY_PROFILE == "3">
#define DRV_PLC_SPI_CLK                       12000000
#define DRV_PLC_PHY_PROFILE                   ${DRV_PLC_PHY_PROFILE?string}
<#elseif DRV_PLC_PHY_PROFILE == "4">
#define DRV_PLC_SPI_CLK                       8000000
#define DRV_PLC_PHY_PROFILE                   ${DRV_PLC_PHY_PROFILE?string}
<#elseif DRV_PLC_PHY_PROFILE == "5">
#define DRV_PLC_SPI_CLK                       8000000
#define DRV_PLC_PHY_PROFILE                   0
</#if>
#define DRV_PLC_PHY_INSTANCES_NUMBER          ${DRV_PLC_PHY_NUM_INSTANCES}
#define DRV_PLC_PHY_INDEX                     0
#define DRV_PLC_PHY_CLIENTS_NUMBER_IDX        ${DRV_PLC_PHY_NUM_CLIENTS?string}

/* PLC Driver Identification */
<#if DRV_PLC_PHY_PROFILE == "0">
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_A
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36010103
<#elseif DRV_PLC_PHY_PROFILE == "1">
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_B
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36040103
<#elseif DRV_PLC_PHY_PROFILE == "2">
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_FCC
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36020103
<#elseif DRV_PLC_PHY_PROFILE == "3">
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_ARIB
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36030103
<#elseif DRV_PLC_PHY_PROFILE == "4">
#define DRV_PLC_PHY_NUM_CARRIERS              97
#define DRV_PLC_PHY_HOST_PRODUCT              0x3600
#define DRV_PLC_PHY_HOST_VERSION              0x36000300
#define DRV_PLC_PHY_HOST_PHY                  0x36000003
<#elseif DRV_PLC_PHY_PROFILE == "5">
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_A
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36010103
</#if>
#define DRV_PLC_PHY_HOST_DESC                 "${__PROCESSOR?string}"
#define DRV_PLC_PHY_HOST_MODEL                3
#define DRV_PLC_PHY_HOST_BAND                 DRV_PLC_PHY_PROFILE