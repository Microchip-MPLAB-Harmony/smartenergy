
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
    (void) PIO_PinInterruptCallbackRegister((PIO_PIN)DRV_PLC_EXT_INT_PIN, DRV_G3_MACRT_ExternalInterruptHandler, sysObj.drvG3MacRt);
</#if>