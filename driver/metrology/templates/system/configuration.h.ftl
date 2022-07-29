/* Metrology Configuration Options */
#define DRV_METROLOGY_REG_BASE_ADDRESS        0x${DRV_MET_BASE_ADDRESS?string}
<#if DRV_MET_WAVEFORM_CAPTURE == true>  
/* Metrology Capture Buffer Size */
#define DRV_METROLOGY_CAPTURE_BUF_SIZE        ${DRV_MET_CAPTURE_BUF_SIZE?string}
</#if>
/* Metrology Default Config: Meter Constant */
#define DRV_METROLOGY_CONF_PKT                0x${DRV_MET_CTRL_PKT?string}
/* Metrology Default Config: Meter Type */
#define DRV_METROLOGY_CONF_MT                 0x${DRV_MET_CTRL_MT?string}
/* Metrology Default Config: Current conversion factor */
#define DRV_METROLOGY_CONF_KI                 0x${DRV_MET_CTRL_KI?string}
/* Metrology Default Config: Voltage conversion factor */
#define DRV_METROLOGY_CONF_KV                 0x${DRV_MET_CTRL_KV?string}
/* Metrology Default Config: ATSENSE CTRL 20 23 */
#define DRV_METROLOGY_CONF_ATS2023            0x${DRV_MET_CTRL_ATS2023?string}
/* Metrology Default Config: ATSENSE CTRL 24 27 */
#define DRV_METROLOGY_CONF_ATS2427            0x${DRV_MET_CTRL_ATS2427?string}
/* Metrology Default Config: SWELL */
#define DRV_METROLOGY_CONF_SWELL              0x${DRV_MET_CTRL_SWELL?string}
/* Metrology Default Config: SAG */
#define DRV_METROLOGY_CONF_SAG                0x${DRV_MET_CTRL_SAG?string}
/* Metrology Default Config: CREEP P */
#define DRV_METROLOGY_CONF_CREEP_P            0x${DRV_MET_CTRL_CREEP_P?string}
/* Metrology Default Config: CREEP Q */
#define DRV_METROLOGY_CONF_CREEP_Q            0x${DRV_MET_CTRL_CREEP_Q?string}
/* Metrology Default Config: CREEP I */
#define DRV_METROLOGY_CONF_CREEP_I            0x${DRV_MET_CTRL_CREEP_I?string}
/* Metrology Default Config: FEATURE_CTRL0 */
#define DRV_METROLOGY_CONF_FCTRL0             0x${DRV_MET_CTRL_FEATCTRL1?string}