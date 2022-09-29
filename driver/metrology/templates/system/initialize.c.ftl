    /* Initialize Metrology Driver Instance */
    sysObj.drvMet = DRV_METROLOGY_Initialize((SYS_MODULE_INIT *)&drvMetrologyInitData, RSTC_ResetCauseGet());
    