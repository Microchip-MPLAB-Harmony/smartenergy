// <editor-fold defaultstate="collapsed" desc="SRV_USI Instance ${INDEX?string} Initialization Data">

<#if SRV_USI_USART_API_INDEX == true>
uint8_t gSrvUSI${SRV_USI_COMM_API?string}ReadBuffer[SRV_USI${INDEX?string}_RD_BUF_SIZE] = {0};
uint8_t gSrvUSI${SRV_USI_COMM_API?string}WriteBuffer[SRV_USI${INDEX?string}_WR_BUF_SIZE] = {0};

const SRV_USI_USART_INTERFACE srvUsi${SRV_USI_COMM_API?string}PlibAPI = {
    .readCallbackRegister = (USI_USART_PLIB_READ_CALLBACK_REG)${SRV_USI_COMM_API?string}_ReadCallbackRegister,
    .read = (USI_USART_PLIB_READ)${SRV_USI_COMM_API?string}_Read,
    .writeCallbackRegister = (USI_USART_PLIB_WRITE_CALLBACK_REG)${SRV_USI_COMM_API?string}_WriteCallbackRegister,
    .dmaChannelTx = SYS_DMA_CHANNEL_${SRV_USI_USART_TX_DMA_CHANNEL?string},
    .usartAddressTx = (void *)&(${SRV_USI_COMM_API?string}_REGS->US_THR)
};

</#if>
<#if SRV_USI_CDC_API_INDEX == true>
const SRV_USI_CDC_INTERFACE srvUSICdcApi = {
    .open = (SRV_USI_CDC_OPEN)USI_USART_Open
};

</#if>
<#if SRV_USI_TCP_API_INDEX == true>
const SRV_USI_TCP_INTERFACE srvUSITcpApi = {
    .open = (SRV_USI_TCP_OPEN)USI_USART_Open
};

</#if>
const SRV_USI_INIT srvUSI${INDEX?string}InitData =
{
<#if SRV_USI_USART_API_INDEX == true>
    .usiInterfaceApi = SRV_USI_USART_API,

    .usiApi = (SRV_USI_USART_INTERFACE *)&srvUsi${SRV_USI_COMM_API?string}PlibAPI,

    .readBuffer = (void*)gSrvUSI${SRV_USI_COMM_API?string}ReadBuffer,

    .readSizeMax = SRV_USI${INDEX?string}_RD_BUF_SIZE,

    .writeBuffer = (void*)gSrvUSI${SRV_USI_COMM_API?string}WriteBuffer,

    .writeSizeMax = SRV_USI${INDEX?string}_WR_BUF_SIZE,

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