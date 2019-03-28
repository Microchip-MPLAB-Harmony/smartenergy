    /* Initialize PL360 Driver Instance */
    sysObj.drvPL360 = DRV_PL360_Initialize(DRV_PL360_INDEX, (SYS_MODULE_INIT *)&drvPL360InitData);
    /* Register Callback function to handle PL360 interruption */
    PIO_PinInterruptCallbackRegister(DRV_PL360_EXT_INT_PIN, DRV_PL360_ExternalInterruptHandler, sysObj.drvPL360);
    