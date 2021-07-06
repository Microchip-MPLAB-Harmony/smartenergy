#include "service/usi/srv_usi.h"
<#if SRV_USI_USART_API == true>
#include "service/usi/srv_usi_usart.h"
</#if>
<#if SRV_USI_CDC_API == true>
#include "service/usi/srv_usi_cdc.h"
</#if>