######################  PLC LIBRARY  ######################
def loadModule():
	print("Load Module: Harmony PLC Stack")
	
	## External PHY PL360 Driver
	drvExtPhyPl360Component = Module.CreateComponent("drvExtPhyPl360", "PL360", "/Harmony/Drivers/PLC Driver", "driver/phy/pl360/config/drv_pl360.py")
	drvExtPhyPl360Component.addCapability("libdrvExtPhyPL360", "DRV_PLC")	
	drvExtPhyPl360Component.addDependency("drv_pl360_SPI_dependency", "SPI", False, True)
	drvExtPhyPl360Component.setDisplayType("Driver")