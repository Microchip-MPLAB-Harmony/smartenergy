"""
Copyright (C) 2022-2024, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
"""

######################  Harmony Smart Energy  ######################
def loadModule():

    print("Load Module: Harmony Smart Energy")

    processor = Variables.get("__PROCESSOR")

    ## PLC PHY Driver (G3, PRIME)
    drvPlcPhyComponent = Module.CreateComponent("drvPlcPhy", "PLC PHY", "/SmartEnergy/Drivers/", "driver/plcPhy/config/drv_plc_phy.py")
    drvPlcPhyComponent.addCapability("libdrvPlcPhy", "DRV_PLC_PHY", True)
    drvPlcPhyComponent.addDependency("drv_plc_phy_SPI_dependency", "SPI", False, True)
    drvPlcPhyComponent.addDependency("drv_plc_phy_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
    drvPlcPhyComponent.addDependency("drv_plc_phy_CouplingDependency", "PCOUP", "PCOUP", True, False)
    drvPlcPhyComponent.setDisplayType("PLC PHY Driver")
    drvPlcPhyComponent.setHelpKeyword("drv_plc_phy")

    ## G3 MAC RT Driver
    drvG3MacRTComponent = Module.CreateComponent("drvG3MacRt", "G3 MAC RT", "/SmartEnergy/Drivers/", "driver/g3MacRt/config/drv_g3_macrt.py")
    drvG3MacRTComponent.addCapability("libdrvG3MacRt", "DRV_G3_MAC_RT", True)
    drvG3MacRTComponent.addDependency("drv_g3_macrt_SPI_dependency", "SPI", False, True)
    drvG3MacRTComponent.addDependency("drv_g3_macrt_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
    drvG3MacRTComponent.addDependency("drv_g3_macrt_CouplingDependency", "PCOUP", "PCOUP", True, False)
    drvG3MacRTComponent.setDisplayType("G3 MAC RT Driver")
    drvG3MacRTComponent.setHelpKeyword("drv_g3_macrt")

    ## Metrology Driver
    if (("MTSH" in processor) or ("MTC" in processor)):
        drvMetrologyComponent = Module.CreateComponent("drvMet", "METROLOGY", "/SmartEnergy/Drivers/", "driver/metrology/config/drv_metrology.py")
        drvMetrologyComponent.addDependency("drvMet_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
        drvMetrologyComponent.addCapability("libdrvMetrology", "DRV_METROLOGY", True)
        drvMetrologyComponent.setDisplayType("Metrology")
        drvMetrologyComponent.setHelpKeyword("drv_metrology")

    ## RF215 Driver
    if ("WBZ45" not in processor) and ("PIC32CX1012BZ" not in processor):
        drvRf215Component = Module.CreateComponent("drvRf215", "RF215", "/SmartEnergy/Drivers/", "driver/rf215/config/drv_rf215.py")
        drvRf215Component.addCapability("libdrvRf215", "DRV_RF_PHY", True)
        drvRf215Component.addDependency("drv_rf215_SPI_dependency", "SPI", False, True)
        drvRf215Component.addDependency("drv_rf215_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
        drvRf215Component.addDependency("drv_rf215_SysTime", "SYS_TIME", "SYS_TIME", True, True)
        drvRf215Component.setDisplayType("RF PHY Driver")
        drvRf215Component.setHelpKeyword("drv_rf215")

    ## PCOUP Service (PHY PLC Coupling Settings)
    srvPhyCouplingComponent = Module.CreateComponent("srv_pcoup", "PLC PHY Coupling", "/SmartEnergy/Services/", "service/pcoup/config/srv_pcoup.py")
    srvPhyCouplingComponent.addCapability("libsrvPCoup", "PCOUP", "PCOUP", True)
    srvPhyCouplingComponent.setDisplayType("PLC Coupling")
    srvPhyCouplingComponent.setHelpKeyword("srv_pcoup")

    ## USI Service (Universal Synchronous Interface)
    srvUSIComponent = Module.CreateGeneratorComponent("srv_usi", "USI", "/SmartEnergy/Services/", "service/usi/config/srv_usi_common.py", "service/usi/config/srv_usi.py")
    srvUSIComponent.addCapability("libsrvUSI", "USI_PLC", True)
    srvUSIComponent.addDependency("srv_usi_USART_dependency", "UART", False, False)
    srvUSIComponent.addDependency("srv_usi_CDC_dependency", "USB_DEVICE_CDC", False, False)
    ## srvUSIComponent.addDependency("srv_usi_TCP_AUX_dependency", "USI_TCP_AUX", False, False)
    srvUSIComponent.addDependency("srv_usi_HarmonyCore_dependency", "Core Service", "Core Service", True, True)
    srvUSIComponent.addDependency("srv_usi_CRC_dependency", "PCRC", "PCRC", True, True)
    srvUSIComponent.addDependency("srv_usi_logreport_dependency", "SE Log Report", True, True)
    srvUSIComponent.setDisplayType("USI Service")
    srvUSIComponent.setHelpKeyword("srv_usi")

    ## srvUSITCPAuxComponent = Module.CreateComponent("usi_tcp_aux", "USI_TCP_AUX", "/SmartEnergy/Services/", "service/usi_tcp_aux/config/srv_usi_tcp_aux.py")
    ## srvUSITCPAuxComponent.addCapability("libsrvUSITCPAux", "USI_TCP_AUX")
    ## srvUSITCPAuxComponent.addDependency("srv_usi_TCP_dependency", "TCP", True, True)
    ## srvUSITCPAuxComponent.setDisplayType("PLC Service")

    ## PCRC Service (SE CRC service)
    srvPCRCComponent = Module.CreateComponent("srv_pcrc", "SE CRC", "/SmartEnergy/Services/", "service/pcrc/config/srv_pcrc.py")
    srvPCRCComponent.addCapability("libsrvPCRC", "PCRC", "PCRC", True)
    srvPCRCComponent.setDisplayType("SE CRC Service")
    srvPCRCComponent.setHelpKeyword("srv_pcrc")

    ## PSERIAL Service (PHY Layer serialization service)
    srvPSERIALComponent = Module.CreateComponent("srv_pserial", "PLC PHY Serial", "/SmartEnergy/Services/", "service/pserial/config/srv_pserial.py")
    srvPSERIALComponent.addCapability("libsrvPSerial", "PSER", "PSER", True)
    srvPSERIALComponent.setDisplayType("Phy Serial Service")
    srvPSERIALComponent.setHelpKeyword("srv_pserial")

    ## PSNIFFER Service (PHY Sniffer serialization service)
    srvPSNIFFERComponent = Module.CreateComponent("srv_psniffer", "PLC PHY Sniffer", "/SmartEnergy/Services/", "service/psniffer/config/srv_psniffer.py")
    srvPSNIFFERComponent.addCapability("libsrvPSniffer", "PSNF", "PSNF", True)
    srvPSNIFFERComponent.setDisplayType("Phy Sniffer Service")
    srvPSNIFFERComponent.setHelpKeyword("srv_psniffer")

    ## PLC PVDD Monitor Service (Voltage Monitor Service)
    srvPVDDMonitorComponent = Module.CreateComponent("srv_pvddmon", "PLC PVDD Monitor", "/SmartEnergy/Services/", "service/pvddmon/config/srv_pvddmon.py")
    srvPVDDMonitorComponent.addDependency("srv_pvddmon_ADC_dependency", "ADC", False, False)
    srvPVDDMonitorComponent.setDisplayType("PLC PVDD Monitor Service")
    srvPVDDMonitorComponent.setHelpKeyword("srv_pvddmon")

    ## RSERIAL Service (RF Layer serialization service)
    srvRSERIALComponent = Module.CreateComponent("srv_rserial", "RF PHY Serial", "/SmartEnergy/Services/", "service/rserial/config/srv_rserial.py")
    srvRSERIALComponent.addCapability("libsrvRSerial", "RSER", "RSER", True)
    srvRSERIALComponent.setDisplayType("RF Serial Service")
    srvRSERIALComponent.setHelpKeyword("srv_rserial")

    ## RSNIFFER Service (RF PHY Sniffer sniffer service)
    srvRSNIFFERComponent = Module.CreateComponent("srv_rsniffer", "RF PHY Sniffer", "/SmartEnergy/Services/", "service/rsniffer/config/srv_rsniffer.py")
    srvRSNIFFERComponent.addCapability("libsrvRSniffer", "RSNF", "RSNF", True)
    srvRSNIFFERComponent.setDisplayType("RF Sniffer Service")
    srvRSNIFFERComponent.setHelpKeyword("srv_rsniffer")

    ## SPI splitter Service
    srvSPISplitterComponent = Module.CreateComponent("srv_spisplit", "SPI Splitter", "/SmartEnergy/Services/", "service/spisplit/config/srv_spisplit.py")
    srvSPISplitterComponent.addDependency("srv_spiplit_SPI_dependency", "SPI", False, True)
    srvSPISplitterComponent.addMultiCapability("srv_spiplit_SPI_capability", "SPI", "SPI")
    srvSPISplitterComponent.setDisplayType("SPI SPlitter")
    srvSPISplitterComponent.setHelpKeyword("srv_spisplitter")

    ## Log Report Service
    srvLogReportComponent = Module.CreateComponent("srvLogReport", "SE Log Report", "/SmartEnergy/Services", "service/log_report/config/srv_log_report.py")
    srvLogReportComponent.addCapability("libsrvLogReport", "SE Log Report", True)
    srvLogReportComponent.addDependency("srv_logreport_sys_debug_dependency", "SYS_DEBUG", True, False)
    srvLogReportComponent.setDisplayType("SE Log Report Service")
    srvLogReportComponent.setHelpKeyword("srv_log_report")

    ## Queue Service
    srvQueueComponent = Module.CreateComponent("srvQueue", "SE Queue", "/SmartEnergy/Services", "service/queue/config/srv_queue.py")
    srvQueueComponent.addCapability("libsrvQueue", "SE Queue", True)
    srvQueueComponent.addDependency("srv_queue_logreport_dependency", "SE Log Report", True, True)
    srvQueueComponent.setDisplayType("SE Queue Service")
    srvQueueComponent.setHelpKeyword("srv_queue")

    ## Random Service
    srvRandomComponent = Module.CreateComponent("srvRandom", "SE Random", "/SmartEnergy/Services", "service/random/config/srv_random.py")
    srvRandomComponent.addCapability("libsrvRandom", "SE Random", True)
    srvRandomComponent.addDependency("srv_random_crypto_dependency", "LIB_CRYPTO", True, False)
    srvRandomComponent.setDisplayType("SE Random Service")
    srvRandomComponent.setHelpKeyword("srv_random")

    ## Security Service
    srvSecurityComponent = Module.CreateComponent("srvSecurity", "SE Security", "/SmartEnergy/Services", "service/security/config/srv_security.py")
    srvSecurityComponent.addCapability("libsrvSecurity", "SE Security", True)
    srvSecurityComponent.addDependency("security_crypto_dependency", "LIB_CRYPTO", True, True)
    srvSecurityComponent.setDisplayType("SE Security Service")
    srvSecurityComponent.setHelpKeyword("srv_security")
