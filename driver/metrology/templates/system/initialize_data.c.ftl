// <editor-fold defaultstate="collapsed" desc="DRV_METROLOGY Initialization Data">

extern uint8_t met_bin_start;
extern uint8_t met_bin_end;

/* Metrology Driver Initialization Data */
DRV_METROLOGY_INIT drvMetrologyInitData = {

    /* MET bin destination address */
    .regBaseAddress = DRV_METROLOGY_REG_BASE_ADDRESS,

    /* MET Binary start address */
    .binStartAddress = (uint32_t)&met_bin_start,
    
    /* MET Binary end address */
    .binEndAddress = (uint32_t)&met_bin_end,
    
};

// </editor-fold>