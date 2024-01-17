"""*****************************************************************************
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
*****************************************************************************"""

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

    ## G3 MAC RT Driver
    drvG3MacRTComponent = Module.CreateComponent("drvG3MacRt", "G3 MAC RT", "/SmartEnergy/Drivers/", "driver/g3MacRt/config/drv_g3_macrt.py")
    drvG3MacRTComponent.addCapability("libdrvG3MacRt", "DRV_G3_MAC_RT", True)
    drvG3MacRTComponent.addDependency("drv_g3_macrt_SPI_dependency", "SPI", False, True)
    drvG3MacRTComponent.addDependency("drv_g3_macrt_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
    drvG3MacRTComponent.addDependency("drv_g3_macrt_CouplingDependency", "PCOUP", "PCOUP", True, False)
    drvG3MacRTComponent.setDisplayType("G3 MAC RT Driver")

    ## Metrology Driver
    if (("MTSH" in processor) or ("MTC" in processor)):
        drvMetrologyComponent = Module.CreateComponent("drvMet", "METROLOGY", "/SmartEnergy/Drivers/", "driver/metrology/config/drv_metrology.py")
        drvMetrologyComponent.addDependency("drvMet_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
        drvMetrologyComponent.addCapability("libdrvMetrology", "DRV_METROLOGY", True)
        drvMetrologyComponent.setDisplayType("Metrology")

    ## RF215 Driver
    if ("WBZ45" not in processor) and ("PIC32CX1012BZ" not in processor):
        drvRf215Component = Module.CreateComponent("drvRf215", "RF215", "/SmartEnergy/Drivers/", "driver/rf215/config/drv_rf215.py")
        drvRf215Component.addCapability("libdrvRf215", "DRV_RF_PHY", True)
        drvRf215Component.addDependency("drv_rf215_SPI_dependency", "SPI", False, True)
        drvRf215Component.addDependency("drv_rf215_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
        drvRf215Component.addDependency("drv_rf215_SysTime", "SYS_TIME", "SYS_TIME", True, True)
        drvRf215Component.setDisplayType("RF PHY Driver")

    ## PCOUP Service (PHY PLC Coupling Settings)
    srvPhyCouplingComponent = Module.CreateComponent("srv_pcoup", "PLC PHY Coupling", "/SmartEnergy/Services/", "service/pcoup/config/srv_pcoup.py")
    srvPhyCouplingComponent.addCapability("libsrvPCoup", "PCOUP", "PCOUP", True)
    srvPhyCouplingComponent.setDisplayType("PLC Coupling")

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

    ## srvUSITCPAuxComponent = Module.CreateComponent("usi_tcp_aux", "USI_TCP_AUX", "/SmartEnergy/Services/", "service/usi_tcp_aux/config/srv_usi_tcp_aux.py")
    ## srvUSITCPAuxComponent.addCapability("libsrvUSITCPAux", "USI_TCP_AUX")
    ## srvUSITCPAuxComponent.addDependency("srv_usi_TCP_dependency", "TCP", True, True)
    ## srvUSITCPAuxComponent.setDisplayType("PLC Service")

    ## PCRC Service (SE CRC service)
    srvPCRCComponent = Module.CreateComponent("srv_pcrc", "SE CRC", "/SmartEnergy/Services/", "service/pcrc/config/srv_pcrc.py")
    srvPCRCComponent.addCapability("libsrvPCRC", "PCRC", "PCRC", True)
    srvPCRCComponent.setDisplayType("SE CRC Service")

    ## PSERIAL Service (PHY Layer serialization service)
    srvPSERIALComponent = Module.CreateComponent("srv_pserial", "PLC PHY Serial", "/SmartEnergy/Services/", "service/pserial/config/srv_pserial.py")
    srvPSERIALComponent.addCapability("libsrvPSerial", "PSER", "PSER", True)
    srvPSERIALComponent.setDisplayType("Phy Serial Service")

    ## PSNIFFER Service (PHY Sniffer serialization service)
    srvPSNIFFERComponent = Module.CreateComponent("srv_psniffer", "PLC PHY Sniffer", "/SmartEnergy/Services/", "service/psniffer/config/srv_psniffer.py")
    srvPSNIFFERComponent.addCapability("libsrvPSniffer", "PSNF", "PSNF", True)
    srvPSNIFFERComponent.setDisplayType("Phy Sniffer Service")

    ## PLC PVDD Monitor Service (Voltage Monitor Service)
    srvPVDDMonitorComponent = Module.CreateComponent("srv_pvddmon", "PLC PVDD Monitor", "/SmartEnergy/Services/", "service/pvddmon/config/srv_pvddmon.py")
    srvPVDDMonitorComponent.addDependency("srv_pvddmon_ADC_dependency", "ADC", False, False)
    srvPVDDMonitorComponent.setDisplayType("PLC PVDD Monitor Service")

    ## RSERIAL Service (RF Layer serialization service)
    srvRSERIALComponent = Module.CreateComponent("srv_rserial", "RF PHY Serial", "/SmartEnergy/Services/", "service/rserial/config/srv_rserial.py")
    srvRSERIALComponent.addCapability("libsrvRSerial", "RSER", "RSER", True)
    srvRSERIALComponent.setDisplayType("RF Serial Service")

    ## RSNIFFER Service (RF PHY Sniffer sniffer service)
    srvRSNIFFERComponent = Module.CreateComponent("srv_rsniffer", "RF PHY Sniffer", "/SmartEnergy/Services/", "service/rsniffer/config/srv_rsniffer.py")
    srvRSNIFFERComponent.addCapability("libsrvRSniffer", "RSNF", "RSNF", True)
    srvRSNIFFERComponent.setDisplayType("RF Sniffer Service")

    ## SPI splitter Service
    srvSPISplitterComponent = Module.CreateComponent("srv_spisplit", "SPI Splitter", "/SmartEnergy/Services/", "service/spisplit/config/srv_spisplit.py")
    srvSPISplitterComponent.addDependency("srv_spiplit_SPI_dependency", "SPI", False, True)
    srvSPISplitterComponent.addMultiCapability("srv_spiplit_SPI_capability", "SPI", "SPI")
    srvSPISplitterComponent.setDisplayType("SPI SPlitter")

    ## Log Report Service
    srvLogReportComponent = Module.CreateComponent("srvLogReport", "SE Log Report", "/SmartEnergy/Services", "service/log_report/config/srv_log_report.py")
    srvLogReportComponent.addCapability("libsrvLogReport", "SE Log Report", True)
    srvLogReportComponent.addDependency("srv_logreport_sys_debug_dependency", "SYS_DEBUG", True, False)
    srvLogReportComponent.setDisplayType("SE Log Report Service")

    ## Queue Service
    srvQueueComponent = Module.CreateComponent("srvQueue", "SE Queue", "/SmartEnergy/Services", "service/queue/config/srv_queue.py")
    srvQueueComponent.addCapability("libsrvQueue", "SE Queue", True)
    srvQueueComponent.addDependency("srv_queue_logreport_dependency", "SE Log Report", True, True)
    srvQueueComponent.setDisplayType("SE Queue Service")

    ## Random Service
    srvRandomComponent = Module.CreateComponent("srvRandom", "SE Random", "/SmartEnergy/Services", "service/random/config/srv_random.py")
    srvRandomComponent.addCapability("libsrvRandom", "SE Random", True)
    srvRandomComponent.addDependency("srv_random_crypto_dependency", "LIB_CRYPTO", True, False)
    srvRandomComponent.setDisplayType("SE Random Service")

    ## Security Service
    srvSecurityComponent = Module.CreateComponent("srvSecurity", "SE Security", "/SmartEnergy/Services", "service/security/config/srv_security.py")
    srvSecurityComponent.addCapability("libsrvSecurity", "SE Security", True)
    srvSecurityComponent.addDependency("security_crypto_dependency", "LIB_CRYPTO", True, True)
    srvSecurityComponent.setDisplayType("SE Security Service")
