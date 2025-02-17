
/* PLC PHY Driver Configuration Options */
#define DRV_PLC_SECURE                        ${DRV_PLC_SECURE_MODE?string}
<#if DRV_PLC_PLIB == "SRV_SPISPLIT">
<#--  Connected to SPI PLIB through SPI Splitter  -->
    <#assign SPI_PLIB = DRV_PLC_PLIB_SPISPLIT>
<#else>
<#--  Connected directly to SPI PLIB  -->
    <#assign SPI_PLIB = DRV_PLC_PLIB>
</#if>
<#if SPI_PLIB?lower_case[0..*6] == "sercom">
#define DRV_PLC_SPI_CS_PIN                    ${DRV_PLC_SPI_CS_PIN?string}
#define DRV_PLC_EXT_INT_SRC                   EIC_IRQn
#define DRV_PLC_EXT_INT_PIO                   ${DRV_PLC_EXT_INT_PIN?string}
#define DRV_PLC_EXT_INT_PIN                   ${DRV_PLC_EIC_SIGNAL?string}
<#else>
#define DRV_PLC_EXT_INT_PIO_PORT              ${DRV_PLC_EXT_INT_PIO_PORT?string}
#define DRV_PLC_EXT_INT_SRC                   ${DRV_PLC_EXT_INT_SRC?string}
#define DRV_PLC_EXT_INT_PIO                   ${DRV_PLC_EXT_INT_PIN?string}
#define DRV_PLC_EXT_INT_PIN                   ${DRV_PLC_EXT_INT_PIN?string}
</#if>
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
<#if DRV_PLC_SPI_NUM_CSR != 0>
#define DRV_PLC_CSR_INDEX                     ${DRV_PLC_SPI_NPCS}
<#if DRV_PLC_PLIB == "SRV_SPISPLIT">
#define DRV_PLC_SPI_CHIP_SELECT               SPI_CHIP_SELECT_NPCS${DRV_PLC_SPI_NPCS}
</#if>
</#if>
<#if DRV_PLC_SPI_NUM_CSR != 0>
  <#--  PLIB with multiple CSR/NPCS  -->
  <#if SPI_PLIB?contains("FLEXCOM")>
    <#assign PLIB_SPI_CSR = "${SPI_PLIB?lower_case}.FLEXCOM_SPI_CSR${DRV_PLC_SPI_NPCS}">
  <#else>
    <#assign PLIB_SPI_CSR = "${SPI_PLIB?lower_case}.SPI_CSR${DRV_PLC_SPI_NPCS}">
  </#if>
  <#assign PLIB_SPI_BAUD_RATE = ("${PLIB_SPI_CSR}_BAUD_RATE")?eval>
<#else>
  <#if SPI_PLIB?contains("USART")>
    <#assign PLIB_SPI_BAUD_RATE = .vars["${SPI_PLIB?lower_case}"].USART_SPI_BAUD_RATE>
  <#elseif SPI_PLIB?contains("SERCOM") || SPI_PLIB?contains("SPI")>
    <#assign PLIB_SPI_BAUD_RATE = .vars["${SPI_PLIB?lower_case}"].SPI_BAUD_RATE>
  <#else>
    <#-- PLIB not supported, set baudrate to 8MHz  -->
    <#assign PLIB_SPI_BAUD_RATE = 8000000>
  </#if>
</#if>
#define DRV_PLC_SPI_CLK                       ${PLIB_SPI_BAUD_RATE}

<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    <#lt>/* PLC PHY Driver RTOS Configuration Options */
    <#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
        <#lt>#define DRV_PLC_RTOS_STACK_SIZE               ${DRV_PLC_RTOS_STACK_SIZE / 4}
    <#else>
        <#lt>#define DRV_PLC_RTOS_STACK_SIZE               ${DRV_PLC_RTOS_STACK_SIZE}
    </#if>
    <#lt>#define DRV_PLC_RTOS_TASK_PRIORITY            ${DRV_PLC_RTOS_TASK_PRIORITY}
    <#if HarmonyCore.SELECT_RTOS == "MicriumOSIII">
        <#lt>#define DRV_PLC_RTOS_TASK_MSG_QTY             ${DRV_PLC_RTOS_TASK_MSG_QTY}u
        <#lt>#define DRV_PLC_RTOS_TASK_TIME_QUANTA         ${DRV_PLC_RTOS_TASK_TIME_QUANTA}u
    </#if>

</#if>
/* PLC Driver Identification */
#define DRV_PLC_PHY_INSTANCES_NUMBER          1U
#define DRV_PLC_PHY_INDEX                     0U
#define DRV_PLC_PHY_CLIENTS_NUMBER_IDX        1U
<#if DRV_PLC_BAND_IN_USE == 1>
#define DRV_PLC_PHY_PROFILE                   0U
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_A
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601U
#define DRV_PLC_PHY_HOST_VERSION              0x36010300UL
#define DRV_PLC_PHY_HOST_PHY                  0x36010103UL
<#elseif DRV_PLC_BAND_IN_USE == 2>
#define DRV_PLC_PHY_PROFILE                   2U
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_FCC
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601U
#define DRV_PLC_PHY_HOST_VERSION              0x36010300UL
#define DRV_PLC_PHY_HOST_PHY                  0x36020103UL
<#elseif DRV_PLC_BAND_IN_USE == 3>
#define DRV_PLC_PHY_PROFILE                   3U
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_ARIB
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601U
#define DRV_PLC_PHY_HOST_VERSION              0x36010300UL
#define DRV_PLC_PHY_HOST_PHY                  0x36030103UL
<#elseif DRV_PLC_BAND_IN_USE == 4>
#define DRV_PLC_PHY_PROFILE                   1U
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_B
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601U
#define DRV_PLC_PHY_HOST_VERSION              0x36010300UL
#define DRV_PLC_PHY_HOST_PHY                  0x36040103UL
<#elseif DRV_PLC_BAND_IN_USE == 5>
#define DRV_PLC_PHY_PROFILE                   4U
#define DRV_PLC_PHY_NUM_CARRIERS              97U
#define DRV_PLC_PHY_HOST_PRODUCT              0x3600U
#define DRV_PLC_PHY_HOST_VERSION              0x36000300UL
#define DRV_PLC_PHY_HOST_PHY                  0x36000003UL
<#elseif DRV_PLC_BAND_IN_USE == 10>
#define DRV_PLC_PHY_PROFILE                   8U
#define DRV_PLC_PHY_HOST_PRODUCT              0x3608U
#define DRV_PLC_PHY_HOST_VERSION              0x36080000UL
#define DRV_PLC_PHY_HOST_PHY                  0x36080000UL
<#elseif (DRV_PLC_BAND_IN_USE == 6 || DRV_PLC_BAND_IN_USE == 7)>
    <#if DRV_PLC_G3_BAND_AUX_ACTIVE == true>
        <#if (DRV_PLC_BAND_IN_USE == 6)>
#define DRV_PLC_PHY_PROFILE                   0U
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_A
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601U
#define DRV_PLC_PHY_HOST_VERSION              0x36010300UL
#define DRV_PLC_PHY_HOST_PHY                  0x36010103UL
        <#else>
#define DRV_PLC_PHY_PROFILE                   1U
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_B
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601U
#define DRV_PLC_PHY_HOST_VERSION              0x36010300UL
#define DRV_PLC_PHY_HOST_PHY                  0x36040103UL
        </#if>
    <#else>
#define DRV_PLC_PHY_PROFILE                   2U
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_FCC
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601U
#define DRV_PLC_PHY_HOST_VERSION              0x36010300UL
#define DRV_PLC_PHY_HOST_PHY                  0x36020103UL
    </#if>
<#elseif (DRV_PLC_BAND_IN_USE == 8 || DRV_PLC_BAND_IN_USE == 9)>
    <#if DRV_PLC_G3_BAND_AUX_ACTIVE == true>
        <#if (DRV_PLC_BAND_IN_USE == 8)>
#define DRV_PLC_PHY_PROFILE                   0U
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_A
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601U
#define DRV_PLC_PHY_HOST_VERSION              0x36010300UL
#define DRV_PLC_PHY_HOST_PHY                  0x36010103UL
        <#else>
#define DRV_PLC_PHY_PROFILE                   1U
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_CENELEC_B
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601U
#define DRV_PLC_PHY_HOST_VERSION              0x36010300UL
#define DRV_PLC_PHY_HOST_PHY                  0x36040103UL
        </#if>
    <#else>
#define DRV_PLC_PHY_PROFILE                   3U
#define DRV_PLC_PHY_NUM_CARRIERS              NUM_CARRIERS_ARIB
#define DRV_PLC_PHY_HOST_PRODUCT              0x3601U
#define DRV_PLC_PHY_HOST_VERSION              0x36010300UL
#define DRV_PLC_PHY_HOST_PHY                  0x36030103UL
    </#if>
</#if>
#define DRV_PLC_PHY_HOST_DESC                 "${__PROCESSOR?string}"
#define DRV_PLC_PHY_HOST_MODEL                3U
#define DRV_PLC_PHY_HOST_BAND                 DRV_PLC_PHY_PROFILE

 <#if DRV_PLC_BIN_STATIC_ADDRESSING == true>
#define DRV_PLC_BIN_START_ADDRESS             0x${DRV_PLC_BIN_ADDRESS?string}
#define DRV_PLC_BIN_SIZE                      ${DRV_PLC_BIN_SIZE?string}
 </#if>