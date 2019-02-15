######################  PLC LIBRARY  ######################
def loadModule():
	print("Load Module: Harmony PLC Stack")
	
	## External PHY PL360 Driver
	drvExtPhyPl360Component = Module.CreateComponent("drvExtPhyPl360", "PL360", "/Harmony/Drivers/PLC Driver", "driver/phy/pl360/config/drv_pl360.py")
	drvExtPhyPl360Component.addCapability("libdrvExtPhyPL360", "DRV_PLC")	
	drvExtPhyPl360Component.addDependency("drv_pl360_SPI_dependency", "SPI", False, True)
	drvExtPhyPl360Component.setDisplayType("Driver")

	## USI Service (Universal Synchronous Interface)
	srvUSIComponent = Module.CreateGeneratorComponent("srv_USI", "USI", "/Libraries/PLC/Service/USI", "service/usi/config/srv_usi_common.py", "service/usi/config/srv_usi.py")
	
	srvUSIComponent.addCapability("libsrvUSI", "USI_PLC")

	srvUSIComponent.addDependency("srv_usi_UART_dependency", "UART", False, False)
	srvUSIComponent.addDependency("srv_usi_USB_dependency", "USB", False, False)
	srvUSIComponent.addDependency("srv_usi_ETH_dependency", "ETH", False, False)

	srvUSIComponent.setDisplayType("Service")