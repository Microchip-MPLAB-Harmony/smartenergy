/* PL360 Driver Configuration Options */
#define DRV_PL360_INSTANCES_NUMBER              ${DRV_PL360_NUM_INSTANCES}
#define DRV_PL360_INDEX                         0
#define DRV_PL360_CLIENTS_NUMBER_IDX            ${DRV_PL360_NUM_CLIENTS?string}
#define DRV_PL360_EXT_INT_SRC                   ${DRV_PL360_EXT_INT_SRC?string}
#define DRV_PL360_EXT_INT_PIN                   ${DRV_PL360_EXT_INT_PIN?string}
#define DRV_PL360_RESET_PIN                     ${DRV_PL360_RESET_PIN?string}
#define DRV_PL360_LDO_EN_PIN                    ${DRV_PL360_LDO_EN_PIN?string}
#define DRV_PL360_PLC_PROFILE                   ${DRV_PL360_PLC_PROFILE?string}
<#if DRV_PL360_BIN_STATIC_ADDRESSING == true>
#define DRV_PL360_BIN_ADDRESS                   0x${DRV_PL360_PLC_BIN_ADDRESS?string}
#define DRV_PL360_BIN_SIZE                      0x${DRV_PL360_PLC_BIN_SIZE?string}
</#if>
#define DRV_PL360_SECURE                        ${DRV_PL360_SECURE_MODE?string}
