// <editor-fold defaultstate="collapsed" desc="SRV_USI Instance ${INDEX?string} Initialization Data">

<#if SRV_USI_USART_API_INDEX == true>
uint8_t gSrvUSI${SRV_USI_PLIB?string}ReadBuffer[SRV_USI${INDEX?string}_RD_BUF_SIZE] = {0};
uint8_t gSrvUSI${SRV_USI_PLIB?string}WriteBuffer[SRV_USI${INDEX?string}_WR_BUF_SIZE] = {0};

const SRV_USI_USART_INTERFACE srvUsi${SRV_USI_PLIB?string}PlibAPI = {
    .readCallbackRegister = (USI_USART_PLIB_READ_CALLBACK_REG)${.vars["${SRV_USI_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_ReadCallbackRegister,
    .read = (USI_USART_PLIB_WRRD)${.vars["${SRV_USI_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_Read,
    .writeCallbackRegister = (USI_USART_PLIB_WRITE_CALLBACK_REG)${.vars["${SRV_USI_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_WriteCallbackRegister,
<#if SRV_USI_TX_DMA == true>     
    .dmaChannelTx = SYS_DMA_CHANNEL_${SRV_USI_USART_TX_DMA_CHANNEL?string}, 
    .usartAddressTx = (void *)${.vars["${SRV_USI_PLIB?lower_case}"].TRANSMIT_DATA_REGISTER}
<#else>
    .write = (USI_USART_PLIB_WRRD)${.vars["${SRV_USI_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_Write,
    .writeIsBusy = (USI_USART_PLIB_WRITE_ISBUSY)${.vars["${SRV_USI_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_WriteIsBusy,
    .serialSetup = (USI_USART_PLIB_SERIAL_SETUP)${.vars["${SRV_USI_PLIB?lower_case}"].USART_PLIB_API_PREFIX}_SerialSetup
</#if>
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

    .usiApi = (SRV_USI_USART_INTERFACE *)&srvUsi${SRV_USI_PLIB?string}PlibAPI,

    .readBuffer = (void*)gSrvUSI${SRV_USI_PLIB?string}ReadBuffer,

    .readSizeMax = SRV_USI${INDEX?string}_RD_BUF_SIZE,

    .writeBuffer = (void*)gSrvUSI${SRV_USI_PLIB?string}WriteBuffer,

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