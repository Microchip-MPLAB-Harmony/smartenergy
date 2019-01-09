######################  PLC LIBRARY  ######################
def loadModule():
	print("Load Module: Harmony PLC Stack")
	
	## External PHY PL360 Driver
	drvExtPhyPl360Component = Module.CreateComponent("drvExtPhyPl360", "PL360", "/Harmony/Drivers/PLC Driver", "driver/phy/pl360/config/drv_pl360.py")
	drvExtPhyPl360Component.addCapability("libdrvExtPhyPL360","PLC",True)	
	drvExtPhyPl360Component.addDependency("PL360_SPI_Dependency", "SPI", None, True, True)
	