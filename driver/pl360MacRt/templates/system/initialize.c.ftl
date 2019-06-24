 <#if DRV_PL360_MACRT_PLC_PROFILE != "5">
    /* Initialize PL360 MAC RT Driver Instance */
    sysObj.drvPL360MacRt = DRV_PL360_Initialize(DRV_PL360_MACRT_INDEX, (SYS_MODULE_INIT *)&drvPL360MacRtInitData);
    /* Register Callback function to handle PL360 interruption */
    PIO_PinInterruptCallbackRegister((PIO_PIN)DRV_PL360_EXT_INT_PIN, DRV_PL360_ExternalInterruptHandler, sysObj.drvPL360MacRt);
</#if>   