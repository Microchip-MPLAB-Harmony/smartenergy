<#--
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
-->
// <editor-fold defaultstate="collapsed" desc="SRV_USI Instance ${INDEX?string} Initialization Data">

static uint8_t CACHE_ALIGN srvUSI${INDEX?string}ReadBuffer[SRV_USI${INDEX?string}_RD_BUF_SIZE] = {0};
static uint8_t CACHE_ALIGN srvUSI${INDEX?string}WriteBuffer[SRV_USI${INDEX?string}_WR_BUF_SIZE] = {0};

<#if SRV_USI_DEVICE_SET == "UART">
/* Declared in USI USART service implementation (srv_usi_usart.c) */
extern const SRV_USI_DEV_DESC srvUSIUSARTDevDesc;

const SRV_USI_USART_INTERFACE srvUsi${INDEX?string}InitData${SRV_USI_DEVICE?string} = {
    .readCallbackRegister = (USI_USART_PLIB_READ_CALLBACK_REG)${.vars["${SRV_USI_DEVICE?lower_case}"].USART_PLIB_API_PREFIX}_ReadCallbackRegister,
    .read = (USI_USART_PLIB_WRRD)${.vars["${SRV_USI_DEVICE?lower_case}"].USART_PLIB_API_PREFIX}_Read,
    .write = (USI_USART_PLIB_WRRD)${.vars["${SRV_USI_DEVICE?lower_case}"].USART_PLIB_API_PREFIX}_Write,
    .writeIsBusy = (USI_USART_PLIB_WRITE_ISBUSY)${.vars["${SRV_USI_DEVICE?lower_case}"].USART_PLIB_API_PREFIX}_WriteIsBusy,
    .intSource = ${.vars["${SRV_USI_DEVICE?lower_case}"].USART_PLIB_API_PREFIX}_IRQn,
};

const USI_USART_INIT_DATA srvUsi${INDEX?string}InitData = {
    .plib = (void*)&srvUsi${INDEX?string}InitData${SRV_USI_DEVICE?string},
    .pRdBuffer = (void*)srvUSI${INDEX?string}ReadBuffer,
    .rdBufferSize = SRV_USI${INDEX?string}_RD_BUF_SIZE,
};

const SRV_USI_INIT srvUSI${INDEX?string}Init =
{
    .deviceInitData = (const void*)&srvUsi${INDEX?string}InitData,
    .consDevDesc = &srvUSIUSARTDevDesc,
    .deviceIndex = ${SRV_USI_USART_API_INDEX?string},
    .pWrBuffer = srvUSI${INDEX?string}WriteBuffer,
    .wrBufferSize = SRV_USI${INDEX?string}_WR_BUF_SIZE
};

</#if>
<#if SRV_USI_DEVICE_SET == "USB_CDC">
static uint8_t CACHE_ALIGN srvUSI${INDEX?string}CDCReadBuffer[128] = {0};

/* Declared in USI CDC service implementation (srv_usi_cdc.c) */
extern const SRV_USI_DEV_DESC srvUSICDCDevDesc;

const USI_CDC_INIT_DATA srvUsi${INDEX?string}InitData = {
    .cdcInstanceIndex = ${SRV_USI_CDC_DEVICE_INDEX?string},
    .usiReadBuffer = srvUSI${INDEX?string}ReadBuffer,
    .usiBufferSize = SRV_USI${INDEX?string}_RD_BUF_SIZE,
    .cdcReadBuffer = srvUSI${INDEX?string}CDCReadBuffer,
    .cdcBufferSize = 128
};

const SRV_USI_INIT srvUSI${INDEX?string}Init =
{
    .deviceInitData = (const void*)&srvUsi${INDEX?string}InitData,
    .consDevDesc = &srvUSICDCDevDesc,
    .deviceIndex = ${SRV_USI_CDC_API_INDEX?string},
    .pWrBuffer = srvUSI${INDEX?string}WriteBuffer,
    .wrBufferSize = SRV_USI${INDEX?string}_WR_BUF_SIZE
};

</#if>

// </editor-fold>