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

######################  PLC LIBRARY  ######################
def loadModule():
    
    print("Load Module: Harmony Smart Energy PLC")

    processor = Variables.get("__PROCESSOR")
    
    ## PLC PHY Driver (G3, PRIME)
    drvPlcPhyComponent = Module.CreateComponent("drvPlcPhy", "PLC_PHY", "/SmartEnergy/PLC/Drivers/", "driver/plcPhy/config/drv_plc_phy.py")
    drvPlcPhyComponent.addCapability("libdrvPlcPhy", "DRV_PLC_PHY") 
    drvPlcPhyComponent.addDependency("drv_plc_phy_SPI_dependency", "SPI", False, True)
    drvPlcPhyComponent.addDependency("drv_plc_phy_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
    drvPlcPhyComponent.addDependency("drv_plc_phy_CouplingDependency", "PCOUP", "PCOUP", True, False)
    drvPlcPhyComponent.setDisplayType("PLC PHY Driver")
    
    ## G3 MAC RT Driver
    drvG3MacRTComponent = Module.CreateComponent("drvG3MacRt", "G3_MAC_RT", "/SmartEnergy/PLC/Drivers/", "driver/g3MacRt/config/drv_g3_macrt.py")
    drvG3MacRTComponent.addCapability("libdrvG3MacRt", "DRV_G3_MAC_RT") 
    drvG3MacRTComponent.addDependency("drv_g3_macrt_SPI_dependency", "SPI", False, True)
    drvG3MacRTComponent.addDependency("drv_g3_macrt_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
    drvG3MacRTComponent.addDependency("drv_g3_macrt_CouplingDependency", "PCOUP", "PCOUP", True, False)
    drvG3MacRTComponent.setDisplayType("G3 MAC RT Driver")
    
    ## Metrology Driver
    if (("MTSH" in processor) or ("MTC" in processor)):
        drvMetrologyComponent = Module.CreateComponent("drvMet", "METROLOGY", "/SmartEnergy/Metrology/", "driver/metrology/config/drv_metrology.py")
        drvMetrologyComponent.addCapability("libdrvMetrology", "DRV_METROLOGY", True)   
        drvMetrologyComponent.setDisplayType("Metrology")

    ## PCOUP Service (PHY PLC Coupling Settings)
    srvPhyCouplingComponent = Module.CreateComponent("srv_pcoup", "PLC PHY Coupling", "/SmartEnergy/PLC/Services/", "service/pcoup/config/srv_pcoup.py")
    srvPhyCouplingComponent.addCapability("libsrvPCoup", "PCOUP", "PCOUP", True)
    srvPhyCouplingComponent.setDisplayType("PLC Service")

    ## USI Service (Universal Synchronous Interface)
    srvUSIComponent = Module.CreateGeneratorComponent("srv_usi", "USI", "/SmartEnergy/PLC/Services/", "service/usi/config/srv_usi_common.py", "service/usi/config/srv_usi.py")
    srvUSIComponent.addCapability("libsrvUSI", "USI_PLC")
    srvUSIComponent.addDependency("srv_usi_USART_dependency", "UART", False, False)
    srvUSIComponent.addDependency("srv_usi_CDC_dependency", "USB_DEVICE_CDC", False, False)
    ## srvUSIComponent.addDependency("srv_usi_TCP_AUX_dependency", "USI_TCP_AUX", False, False)
    srvUSIComponent.addDependency("drv_usi_HarmonyCore_dependency", "Core Service", "Core Service", True, True)
    srvUSIComponent.addDependency("drv_usi_CRC_dependency", "PCRC", "PCRC", True, True)
    srvUSIComponent.setDisplayType("PLC Service")

    ## srvUSITCPAuxComponent = Module.CreateComponent("usi_tcp_aux", "USI_TCP_AUX", "/SmartEnergy/PLC/Services/", "service/usi_tcp_aux/config/srv_usi_tcp_aux.py")
    ## srvUSITCPAuxComponent.addCapability("libsrvUSITCPAux", "USI_TCP_AUX")
    ## srvUSITCPAuxComponent.addDependency("srv_usi_TCP_dependency", "TCP", True, True)
    ## srvUSITCPAuxComponent.setDisplayType("PLC Service")

    ## PCRC Service (PLC CRC service)
    srvPCRCComponent = Module.CreateComponent("srv_pcrc", "PLC CRC", "/SmartEnergy/PLC/Services/", "service/pcrc/config/srv_pcrc.py")
    srvPCRCComponent.addCapability("libsrvPCRC", "PCRC", "PCRC", True)
    srvPCRCComponent.setDisplayType("PLC Service")

    ## PSERIAL Service (PHY Layer serialization service)
    srvPSERIALComponent = Module.CreateComponent("srv_pserial", "PLC PHY Serial", "/SmartEnergy/PLC/Services/", "service/pserial/config/srv_pserial.py")
    srvPSERIALComponent.addCapability("libsrvPSerial", "PSER", "PSER", True)
    srvPSERIALComponent.setDisplayType("PLC Service")

    ## PSNIFFER Service (PHY Sniffer serialization service)
    srvPSNIFFERComponent = Module.CreateComponent("srv_psniffer", "PLC PHY Sniffer", "/SmartEnergy/PLC/Services/", "service/psniffer/config/srv_psniffer.py")
    srvPSNIFFERComponent.addCapability("libsrvPSniffer", "PSNF", "PSNF", True)
    srvPSNIFFERComponent.setDisplayType("PLC Service")

    ## PLC PVDD Monitor Service (True Random Generator Service)
    srvPVDDMonitorComponent = Module.CreateComponent("srv_pvddmon", "PLC PVDD MONITOR", "/SmartEnergy/PLC/Services/", "service/pvddmon/config/srv_pvddmon.py")
    srvPVDDMonitorComponent.addDependency("srv_pvddmon_ADC_dependency", "ADC", False, False)
    srvPVDDMonitorComponent.setDisplayType("PLC PVDD Monitor Service")