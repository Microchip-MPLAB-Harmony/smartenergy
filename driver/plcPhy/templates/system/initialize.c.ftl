<#if (((DRV_PLC_PROFILE == "G3") && (DRV_PLC_COUP_G3_MULTIBAND == false)) || (DRV_PLC_PROFILE == "PRIME"))>
    /* Initialize PLC Phy Driver Instance */
    sysObj.drvPlcPhy = DRV_PLC_PHY_Initialize(DRV_PLC_PHY_INDEX, (SYS_MODULE_INIT *)&drvPlcPhyInitData);
    /* Register Callback function to handle PLC interruption */
    PIO_PinInterruptCallbackRegister((PIO_PIN)DRV_PLC_EXT_INT_PIN, DRV_PLC_PHY_ExternalInterruptHandler, sysObj.drvPlcPhy);
</#if>