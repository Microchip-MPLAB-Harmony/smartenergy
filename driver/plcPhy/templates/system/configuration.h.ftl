/* PLC PHY Driver Configuration Options */
#define DRV_PLC_SECURE                        ${DRV_PLC_SECURE_MODE?string}
#define DRV_PLC_EXT_INT_PIO_PORT              ${DRV_PLC_EXT_INT_PIO_PORT?string}
#define DRV_PLC_EXT_INT_SRC                   ${DRV_PLC_EXT_INT_SRC?string}
#define DRV_PLC_EXT_INT_PIN                   ${DRV_PLC_EXT_INT_PIN?string}
#define DRV_PLC_RESET_PIN                     ${DRV_PLC_RESET_PIN?string}
#define DRV_PLC_LDO_EN_PIN                    ${DRV_PLC_LDO_EN_PIN?string}
#define DRV_PLC_CD_PIN                        ${DRV_PLC_CD_PIN?string}
<#if DRV_PLC_SLEEP_MODE == true>
#define DRV_PLC_STBY_PIN                      ${DRV_PLC_STBY_PIN?string}
</#if>
<#if DRV_PLC_MODE == "PL460">
#define DRV_PLC_THMON_PIN                     ${DRV_PLC_THMON_PIN?string}
</#if>

#define DRV_PLC_SPI_CLK                       8000000
#define DRV_PLC_PHY_INSTANCES_NUMBER          1
#define DRV_PLC_PHY_INDEX                     0
#define DRV_PLC_PHY_CLIENTS_NUMBER_IDX        1

/* PLC Driver Identification */
<#if DRV_PLC_BAND_IN_USE == 1>
#define DRV_PLC_PHY_PROFILE                   0
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_A
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36010103
<#elseif DRV_PLC_BAND_IN_USE == 2>
#define DRV_PLC_PHY_PROFILE                   2
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_FCC
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36020103
<#elseif DRV_PLC_BAND_IN_USE == 3>
#define DRV_PLC_PHY_PROFILE                   3
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_ARIB
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36030103
<#elseif DRV_PLC_BAND_IN_USE == 4>
#define DRV_PLC_PHY_PROFILE                   1
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_B
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36040103
<#elseif DRV_PLC_BAND_IN_USE == 5>
#define DRV_PLC_PHY_PROFILE                   4
#define DRV_PLC_PHY_NUM_CARRIERS              97
#define DRV_PLC_PHY_HOST_PRODUCT              0x3600
#define DRV_PLC_PHY_HOST_VERSION              0x36000300
#define DRV_PLC_PHY_HOST_PHY                  0x36000003
<#elseif (DRV_PLC_BAND_IN_USE == 6 || DRV_PLC_BAND_IN_USE == 7)>
    <#if DRV_PLC_G3_BAND_AUX_ACTIVE == true>
        <#if (DRV_PLC_BAND_IN_USE == 6)>
#define DRV_PLC_PHY_PROFILE                   0
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_A
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36010103
        <#else>
#define DRV_PLC_PHY_PROFILE                   1
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_B
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36040103
        </#if>
    <#else>
#define DRV_PLC_PHY_PROFILE                   2
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_FCC
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36020103
    </#if>
<#elseif (DRV_PLC_BAND_IN_USE == 8 || DRV_PLC_BAND_IN_USE == 9)>
    <#if DRV_PLC_G3_BAND_AUX_ACTIVE == true>
        <#if (DRV_PLC_BAND_IN_USE == 8)>
#define DRV_PLC_PHY_PROFILE                   0
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_A
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36010103
        <#else>
#define DRV_PLC_PHY_PROFILE                   1
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_B
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36040103
        </#if>
    <#else>
#define DRV_PLC_PHY_PROFILE                   3
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_ARIB
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601
#define DRV_PLC_PHY_HOST_VERSION              0x36010300
#define DRV_PLC_PHY_HOST_PHY                  0x36030103
    </#if>
</#if>
#define DRV_PLC_PHY_HOST_DESC                 "${__PROCESSOR?string}"
#define DRV_PLC_PHY_HOST_MODEL                3
#define DRV_PLC_PHY_HOST_BAND                 DRV_PLC_PHY_PROFILE