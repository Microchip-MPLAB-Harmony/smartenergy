/* USI Service Common Configuration Options */
#define SRV_USI_INSTANCES_NUMBER              ${__INSTANCE_COUNT}
#define SRV_USI_USART_CONNECTIONS             ${SRV_USI_NUM_USART?string}
#define SRV_USI_CDC_CONNECTIONS               ${SRV_USI_NUM_CDC?string}
#define SRV_USI_MSG_POOL_SIZE                 ${SRV_USI_MSG_POOL_SIZE?string}