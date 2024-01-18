
    /* Initialize USI Service Instance ${INDEX} */
    sysObj.srvUSI${INDEX} = SRV_USI_Initialize(SRV_USI_INDEX_${INDEX}, (SYS_MODULE_INIT *)&srvUSI${INDEX?string}Init);