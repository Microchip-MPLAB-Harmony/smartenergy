######################  PLC LIBRARY  ######################
def loadModule():
	print("Load Module: Harmony Smart Energy PLC")
	
	## External PHY PL360 Driver
	drvExtPhyPl360Component = Module.CreateComponent("drvExtPhyPl360", "PL360", "/Harmony/Drivers/PLC", "driver/pl360/config/drv_pl360.py")
	drvExtPhyPl360Component.addCapability("libdrvExtPhyPL360", "DRV_PLC")	
	drvExtPhyPl360Component.addDependency("drv_pl360_SPI_dependency", "SPI", False, True)
	drvExtPhyPl360Component.addDependency("drv_pl360_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
	drvExtPhyPl360Component.setDisplayType("PLC Driver")

	## USI Service (Universal Synchronous Interface)
	srvUSIComponent = Module.CreateGeneratorComponent("srv_usi", "USI", "/Libraries/PLC/Services/", "service/usi/config/srv_usi_common.py", "service/usi/config/srv_usi.py")
	srvUSIComponent.addCapability("libsrvUSI", "USI_PLC")
	srvUSIComponent.addDependency("srv_usi_USART_dependency", "UART", False, False)
	## srvUSIComponent.addDependency("srv_usi_CDC_dependency", "USB_DEVICE_CDC", False, False)
	## srvUSIComponent.addDependency("srv_usi_TCP_AUX_dependency", "USI_TCP_AUX", False, False)
	srvUSIComponent.addDependency("drv_usi_HarmonyCore_dependency", "Core Service", "Core Service", True, True)
	srvUSIComponent.addDependency("drv_usi_CRC_dependency", "PCRC", "PLC CRC", True, True)
	srvUSIComponent.setDisplayType("PLC Service")

	## srvUSITCPAuxComponent = Module.CreateComponent("usi_tcp_aux", "USI_TCP_AUX", "/Libraries/PLC/Services/", "service/usi_tcp_aux/config/srv_usi_tcp_aux.py")
	## srvUSITCPAuxComponent.addCapability("libsrvUSITCPAux", "USI_TCP_AUX")
	## srvUSITCPAuxComponent.addDependency("srv_usi_TCP_dependency", "TCP", True, True)
	## srvUSITCPAuxComponent.setDisplayType("PLC Service")

	## PCRC Service (PLC CRC service)
	srvPCRCComponent = Module.CreateComponent("srv_pcrc", "PLC CRC", "/Libraries/PLC/Services/", "service/pcrc/config/srv_pcrc.py")
	srvPCRCComponent.addCapability("libsrvPCRC", "PCRC", "PCRC", True)
	srvPCRCComponent.setDisplayType("PLC Service")

	## PSERIAL Service (PHY Layer serialization service)
	srvPSERIALComponent = Module.CreateComponent("srv_pserial", "PLC PHY Serial", "/Libraries/PLC/Services/", "service/pserial/config/srv_pserial.py")
	srvPSERIALComponent.addCapability("libsrvPSerial", "PSER", "PSER", True)
	srvPSERIALComponent.setDisplayType("PLC Service")

	## PSNIFFER Service (PHY Sniffer serialization service)
	srvPSNIFFERComponent = Module.CreateComponent("srv_psniffer", "PLC PHY Sniffer", "/Libraries/PLC/Services/", "service/psniffer/config/srv_psniffer.py")
	srvPSNIFFERComponent.addCapability("libsrvPSniffer", "PSNF", "PSNF", True)
	srvPSNIFFERComponent.setDisplayType("PLC Service")