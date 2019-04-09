######################  PLC LIBRARY  ######################
def loadModule():
	print("Load Module: Harmony PLC Stack")
	
	## External PHY PL360 Driver
	drvExtPhyPl360Component = Module.CreateComponent("drvExtPhyPl360", "PL360", "/Harmony/Drivers/PLC", "driver/pl360/config/drv_pl360.py")
	drvExtPhyPl360Component.addCapability("libdrvExtPhyPL360", "DRV_PLC")	
	drvExtPhyPl360Component.addDependency("drv_pl360_SPI_dependency", "SPI", False, True)
	drvExtPhyPl360Component.addDependency("drv_pl360_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
	drvExtPhyPl360Component.setDisplayType("Driver")

	## USI Service (Universal Synchronous Interface)
	srvUSIComponent = Module.CreateGeneratorComponent("srv_usi", "USI", "/Libraries/PLC/Services/", "service/usi/config/srv_usi_common.py", "service/usi/config/srv_usi.py")
	srvUSIComponent.addCapability("libsrvUSI", "USI_PLC")
	srvUSIComponent.addDependency("srv_usi_USART_dependency", "UART", False, False)
	srvUSIComponent.addDependency("srv_usi_CDC_dependency", "USB_DEVICE_CDC", False, False)
	srvUSIComponent.addDependency("srv_usi_TCP_AUX_dependency", "USI_TCP_AUX", False, False)
	srvUSIComponent.addDependency("drv_usi_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
	srvUSIComponent.setDisplayType("Service")

	srvUSITCPAuxComponent = Module.CreateComponent("usi_tcp_aux", "USI_TCP_AUX", "/Libraries/PLC/Services/", "service/usi_tcp_aux/config/srv_usi_tcp_aux.py")
	srvUSITCPAuxComponent.addCapability("libsrvUSITCPAux", "USI_TCP_AUX")
	srvUSITCPAuxComponent.addDependency("srv_usi_TCP_dependency", "TCP", True, True)
	srvUSITCPAuxComponent.setDisplayType("Service")