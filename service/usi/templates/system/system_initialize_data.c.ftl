// <editor-fold defaultstate="collapsed" desc="SRV_USI Instance ${INDEX?string} Initialization Data">

<#if SRV_USI_USART_API_INDEX == true>
const SRV_USI_USART_INTERFACE srvUSIUsartApi = {
    .open = (SRV_USI_USART_OPEN)DRV_USART_Open,
    .read = (SRV_USI_USART_READ)DRV_USART_ReadBufferAdd,
    .write = (SRV_USI_USART_WRITE)DRV_USART_WriteBufferAdd,
    .eventHandlerSet = (SRV_USI_USART_EVENT_HANDLER_SET)DRV_USART_BufferEventHandlerSet,
    .close = (SRV_USI_USART_CLOSE)DRV_USART_Close
};

</#if>
<#if SRV_USI_CDC_API_INDEX == true>
const SRV_USI_CDC_INTERFACE srvUSICdcApi = {
    .open = (SRV_USI_CDC_OPEN)DRV_USART_Open
};

</#if>
<#if SRV_USI_TCP_API_INDEX == true>
const SRV_USI_TCP_INTERFACE srvUSITcpApi = {
    .open = (SRV_USI_TCP_OPEN)DRV_USART_Open
};

</#if>
const SRV_USI_INIT srvUSI${INDEX?string}InitData =
{
<#if SRV_USI_USART_API_INDEX == true>
    .usiInterfaceApi = SRV_USI_USART_API,

    .usiApi = (void*)&srvUSIUsartApi,

<#elseif SRV_USI_CDC_API_INDEX == true>
    .usiInterfaceApi = SRV_USI_CDC_API,
    
    .usiApi = (void*)&srvUSICdcApi,
<#elseif SRV_USI_TCP_API_INDEX == true>
    .usiInterfaceApi = SRV_USI_TCP_API,
    
    .usiApi = (void*)&srvUSITcpApi,
<#else>
    .usiInterfaceApi = SRV_USI_INVALID_API,
    
    .usiApi = (void*)SRV_USI_INVALID_API,
</#if>
};

// </editor-fold>