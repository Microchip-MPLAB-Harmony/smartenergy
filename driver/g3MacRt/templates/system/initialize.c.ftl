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
    /* Initialize G3 MAC RT Driver Instance */
    sysObj.drvG3MacRt = DRV_G3_MACRT_Initialize(DRV_G3_MACRT_INDEX, (SYS_MODULE_INIT *)&drvG3MacRtInitData);
<#if DRV_PLC_PLIB == "SRV_SPISPLIT">
<#--  Connected to SPI PLIB through SPI Splitter  -->
    <#assign SPI_PLIB = DRV_PLC_PLIB_SPISPLIT>
<#else>
<#--  Connected directly to SPI PLIB  -->
    <#assign SPI_PLIB = DRV_PLC_PLIB>
</#if>
<#if SPI_PLIB?lower_case[0..*6] == "sercom">
    EIC_CallbackRegister(DRV_PLC_EXT_INT_PIN, DRV_G3_MACRT_ExternalInterruptHandler, sysObj.drvG3MacRt);
<#else>
    PIO_PinInterruptCallbackRegister((PIO_PIN)DRV_PLC_EXT_INT_PIN, DRV_G3_MACRT_ExternalInterruptHandler, sysObj.drvG3MacRt);
</#if>