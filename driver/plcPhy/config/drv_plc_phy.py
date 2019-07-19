# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

################################################################################
#### Component ####
################################################################################
global sort_alphanumeric

def sort_alphanumeric(l):
    import re
    convert = lambda text: int(text) if text.isdigit() else text.lower()
    alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ]
    return sorted(l, key = alphanum_key)

def externalAddressingTrigger(symbol, event):
    global plcPhySourceBinFileG3CENA
    global plcPhySourceBinFileG3CENB
    global plcPhySourceBinFileG3FCC
    global plcPhySourceBinFilePRIME
    global plcPhyExternalAddressing
    global plcPhyAssemblyBinFile
    if event["value"] == True:
        print("externalAddressingTrigger disable BIN files")
        plcPhySourceBinFileG3CENA.setEnabled(False)
        plcPhySourceBinFileG3CENB.setEnabled(False)
        plcPhySourceBinFileG3FCC.setEnabled(False)
        plcPhySourceBinFilePRIME.setEnabled(False)
        plcPhyAssemblyBinFile.setEnabled(False)
    else:
        plcPhyAssemblyBinFile.setEnabled(True)
        prof = symbol.getValue()
        if(prof == 0):
            print("externalAddressingTrigger restore G3 CENA")
            plcPhySourceBinFileG3CENA.setEnabled(True)
        elif(prof == 1):
            print("externalAddressingTrigger restore G3 CENB")
            plcPhySourceBinFileG3CENB.setEnabled(True)
        elif(prof == 2):
            print("externalAddressingTrigger restore G3 FCC")
            plcPhySourceBinFileG3FCC.setEnabled(True)
        elif(prof == 3):
            print("externalAddressingTrigger restore PRIME")
            plcPhySourceBinFilePRIME.setEnabled(True)
        elif(prof == 4):
            print("externalAddressingTrigger restore G3 CENA (multi)")
            plcPhySourceBinFileG3CENA.setEnabled(True)



def plcPhyProfileConfiguration(symbol, event):
    global plcPhyProfileFile
    global plcPhyProfileDefFile
    global plcPhyProfileHeaderLocalFile
    global plcPhySourceBinFileG3CENA
    global plcPhySourceBinFileG3CENB
    global plcPhySourceBinFileG3FCC
    global plcPhySourceBinFilePRIME
    global plcPhyExternalAddressing
    global plcPhyAssemblyBinFile
    if (event["symbol"].getKeyDescription(event["value"]) == "PRIME"):
        plcPhyProfileFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_prime.c")
        plcPhyProfileDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_prime.h")
        plcPhyProfileHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local_prime.h")
        plcPhySourceBinFileG3CENA.setEnabled(False)
        plcPhySourceBinFileG3CENB.setEnabled(False)
        plcPhySourceBinFileG3FCC.setEnabled(False)
        if (plcPhyExternalAddressing.getValue() == False):
            print("plcPhyProfileConfiguration update PRIME files")
            plcPhySourceBinFilePRIME.setEnabled(True)
            plcPhyAssemblyBinFile.setEnabled(True)
        else:
            plcPhySourceBinFilePRIME.setEnabled(False)
            plcPhyAssemblyBinFile.setEnabled(False)
            print("plcPhyProfileConfiguration update no bin files (PRIME)")

    else:
        plcPhySourceBinFilePRIME.setEnabled(False)
        plcPhyProfileFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_g3.c")
        plcPhyProfileDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_g3.h")
        plcPhyProfileHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local_g3.h")
        if (plcPhyExternalAddressing.getValue() == False):
            plcPhyAssemblyBinFile.setEnabled(True)
            if (event["symbol"].getKeyDescription(event["value"]) == "G3_CEN_A"):            
                plcPhySourceBinFileG3CENA.setEnabled(True)
                plcPhySourceBinFileG3CENB.setEnabled(False)
                plcPhySourceBinFileG3FCC.setEnabled(False)
                print("plcPhyProfileConfiguration update G3 CEN A files")
            elif (event["symbol"].getKeyDescription(event["value"]) == "G3_CEN_B"):           
                plcPhySourceBinFileG3CENA.setEnabled(False)
                plcPhySourceBinFileG3CENB.setEnabled(True)
                plcPhySourceBinFileG3FCC.setEnabled(False)
                print("plcPhyProfileConfiguration update G3 CEN B files")
            elif (event["symbol"].getKeyDescription(event["value"]) == "G3_FCC"):           
                plcPhySourceBinFileG3CENA.setEnabled(False)
                plcPhySourceBinFileG3CENB.setEnabled(False)
                plcPhySourceBinFileG3FCC.setEnabled(True)
                print("plcPhyProfileConfiguration update G3 FCC files")
            else:           
                plcPhySourceBinFileG3CENA.setEnabled(True)
                plcPhySourceBinFileG3CENB.setEnabled(False)
                plcPhySourceBinFileG3FCC.setEnabled(True)
                print("plcPhyProfileConfiguration update G3 CEN A nd FCC files")
        else:
            print("plcPhyProfileConfiguration update no bin files (G3)")
            plcPhySourceBinFileG3CENA.setEnabled(False)
            plcPhySourceBinFileG3CENB.setEnabled(False)
            plcPhySourceBinFileG3FCC.setEnabled(False)
            plcPhyAssemblyBinFile.setEnabled(False)

def plcPhyExternalInterruptSource(symbol, event):
    key = event["symbol"].getKeyDescription(event["value"])
    intSrc = "PIO" + key[1] + "_IRQn"
    symbol.setValue(intSrc)

def plcPhyExternalInterruptPort(symbol, event):
    key = event["symbol"].getKeyDescription(event["value"])
    intPort = "PIO_PORT_" + key[1]
    symbol.setValue(intPort)

        
def plcPhyEnableEncScript(symbol, event):
    if (event["value"] == True):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False) 

def plcPhyVisibleEncComment(symbol, event):
    if (event["value"] == True):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)         
		
def instantiateComponent(plcPhyComponent):  
    global drvPlcPhyInstanceSpace
    global isDMAPresent

    drvPlcPhyInstanceSpace = "drvPlcPhy"

    # Enable "Generate Harmony Driver Common Files" option in MHC
    if (Database.getSymbolValue("HarmonyCore", "ENABLE_DRV_COMMON") == False):
        Database.setSymbolValue("HarmonyCore", "ENABLE_DRV_COMMON", True)

    # Enable "Generate Harmony System Service Common Files" option in MHC
    if (Database.getSymbolValue("HarmonyCore", "ENABLE_SYS_COMMON") == False):
        Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_COMMON", True)

    # Enable "Enable System Ports" option in MHC
    if (Database.getSymbolValue("HarmonyCore", "ENABLE_SYS_PORTS") == False):
        Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_PORTS", True)

    # Enable "ENABLE_SYS_DMA" option in MHC
    if Database.getSymbolValue("core", "DMA_ENABLE") == None:
        isDMAPresent = False
        print(" PLC PHY isDMAPresent = False")
    else:
        isDMAPresent = True
        print("PLC PHY isDMAPresent = True")

        # Enable "Enable System DMA" option in MHC
        if (Database.getSymbolValue("HarmonyCore", "ENABLE_SYS_DMA") == False):
            Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_DMA", True)
            print("PLC PHY HarmonyCore ENABLE_SYS_DMA")
    
    plcPhySymNumInst = plcPhyComponent.createIntegerSymbol("DRV_PLC_PHY_NUM_INSTANCES", None)
    plcPhySymNumInst.setLabel("Number of Instances")
    plcPhySymNumInst.setVisible(False)
    plcPhySymNumInst.setDefaultValue(1)

    plcPhyPLIB = plcPhyComponent.createStringSymbol("DRV_PLC_PLIB", None)
    plcPhyPLIB.setLabel("PLIB Used")
    plcPhyPLIB.setReadOnly(True)
    
    plcPhySymNumClients = plcPhyComponent.createIntegerSymbol("DRV_PLC_PHY_NUM_CLIENTS", None)
    plcPhySymNumClients.setLabel("Number of Clients")
    plcPhySymNumClients.setReadOnly(True)
    plcPhySymNumClients.setDefaultValue(1)

    plcPhyExtIntPin = plcPhyComponent.createKeyValueSetSymbol("DRV_PLC_EXT_INT_PIN", None)
    plcPhyExtIntPin.setLabel("External Interrupt Pin")
    plcPhyExtIntPin.setOutputMode("Key")
    plcPhyExtIntPin.setDisplayMode("Description")

    plcPhyExtIntSource = plcPhyComponent.createStringSymbol("DRV_PLC_PHY_EXT_INT_SRC", None)
    plcPhyExtIntSource.setLabel("External Interrupt Source")
    plcPhyExtIntSource.setDefaultValue("PIOA_IRQn")
    plcPhyExtIntSource.setVisible(True)
    plcPhyExtIntSource.setReadOnly(True)
    plcPhyExtIntSource.setDependencies(plcPhyExternalInterruptSource, ["DRV_PLC_EXT_INT_PIN"])

    plcPhyExtIntPioPort = plcPhyComponent.createStringSymbol("DRV_PLC_PHY_EXT_INT_PIO_PORT", None)
    plcPhyExtIntPioPort.setLabel("External Interrupt Port")
    plcPhyExtIntPioPort.setDefaultValue("PIO_PORT_A")
    plcPhyExtIntPioPort.setVisible(True)
    plcPhyExtIntPioPort.setReadOnly(True)
    plcPhyExtIntPioPort.setDependencies(plcPhyExternalInterruptPort, ["DRV_PLC_EXT_INT_PIN"])

    plcPhyResetPin = plcPhyComponent.createKeyValueSetSymbol("DRV_PLC_RESET_PIN", None)
    plcPhyResetPin.setLabel("Reset Pin")
    plcPhyResetPin.setOutputMode("Key")
    plcPhyResetPin.setDisplayMode("Description")

    plcPhyLDOEnPin = plcPhyComponent.createKeyValueSetSymbol("DRV_PLC_LDO_EN_PIN", None)
    plcPhyLDOEnPin.setLabel("LDO Enable Pin")
    plcPhyLDOEnPin.setOutputMode("Key")
    plcPhyLDOEnPin.setDisplayMode("Description")  

    plcPhyCDPin = plcPhyComponent.createKeyValueSetSymbol("DRV_PLC_CD_PIN", None)
    plcPhyCDPin.setLabel("Carrier Detect Pin")
    plcPhyCDPin.setOutputMode("Key")
    plcPhyCDPin.setDisplayMode("Description")  

    availablePinDictionary = {}

    # Send message to core to get available pins
    availablePinDictionary = Database.sendMessage("core", "PIN_LIST", availablePinDictionary)

    for pad in sort_alphanumeric(availablePinDictionary.values()):
        key = "SYS_PORT_PIN_" + pad
        value = list(availablePinDictionary.keys())[list(availablePinDictionary.values()).index(pad)]
        description = pad
        plcPhyExtIntPin.addKey(key, value, description)
        plcPhyResetPin.addKey(key, value, description)
        plcPhyLDOEnPin.addKey(key, value, description)
        plcPhyCDPin.addKey(key, value, description)

    plcPhySymPinConfigComment = plcPhyComponent.createCommentSymbol("DRV_PLC_PHY_PINS_CONFIG_COMMENT", None)
    plcPhySymPinConfigComment.setVisible(True)
    plcPhySymPinConfigComment.setLabel("***The pins selected above must be the same as those for the GPIO output in Pin Manager***")

    global plcPhyExternalAddressing
    plcPhyExternalAddressing = plcPhyComponent.createBooleanSymbol("DRV_PLC_PHY_EXTERNAL_ADDRESSING", None)
    plcPhyExternalAddressing.setLabel("External Binary Image Block Transfer")
    plcPhyExternalAddressing.setVisible(True)
    plcPhyExternalAddressing.setDefaultValue(False)

    global plcPhyProfile
    plcPhyProfile = plcPhyComponent.createKeyValueSetSymbol("DRV_PLC_PHY_PROFILE", None)
    plcPhyProfile.setLabel("Select PLC Profile")
    plcPhyProfile.addKey("G3_CEN_A", "0", "G3_CEN_A")
    plcPhyProfile.addKey("G3_CEN_B", "1", "G3_CEN_B")
    plcPhyProfile.addKey("G3_FCC", "2", "G3_FCC")
    plcPhyProfile.addKey("G3_ARIB", "3", "G3_ARIB")
    plcPhyProfile.addKey("PRIME", "4", "PRIME")
    plcPhyProfile.addKey("G3_MULTIBAND", "5", "G3_MULTIBAND")
    plcPhyProfile.setDisplayMode("Description")
    plcPhyProfile.setOutputMode("Value")
    plcPhyProfile.setDefaultValue(0)
    plcPhyProfile.setDependencies(externalAddressingTrigger, ["DRV_PLC_PHY_EXTERNAL_ADDRESSING"])

    plcPhySecureMode = plcPhyComponent.createBooleanSymbol("DRV_PLC_SECURE_MODE", None)
    plcPhySecureMode.setLabel("PLC Secure Mode")
    plcPhySecureMode.setVisible(True)
    plcPhySecureMode.setDefaultValue(False)

    plcPhySecureComment = plcPhyComponent.createCommentSymbol("DRV_PLC_SECURE_COMMENT", None)
    plcPhySecureComment.setLabel("***Run encription script to get a secure binary file and replace the current non-secure file. Check bin folder.***")
    plcPhySecureComment.setVisible(False)
    plcPhySecureComment.setDependencies(plcPhyVisibleEncComment, ["DRV_PLC_SECURE_MODE"])
    
    global plcPhyTXRXDMA
    plcPhyTXRXDMA = plcPhyComponent.createBooleanSymbol("DRV_PLC_TX_RX_DMA", None)
    plcPhyTXRXDMA.setLabel("Use DMA for Transmit and Receive?")
    plcPhyTXRXDMA.setVisible(isDMAPresent)
    plcPhyTXRXDMA.setDefaultValue(isDMAPresent)
    plcPhyTXRXDMA.setReadOnly(True)

    global plcPhyTXDMAChannel
    plcPhyTXDMAChannel = plcPhyComponent.createIntegerSymbol("DRV_PLC_TX_DMA_CHANNEL", None)
    plcPhyTXDMAChannel.setLabel("DMA Channel For Transmit")
    plcPhyTXDMAChannel.setDefaultValue(0)
    plcPhyTXDMAChannel.setVisible(False)
    plcPhyTXDMAChannel.setReadOnly(True)
    plcPhyTXDMAChannel.setDependencies(requestAndAssignTxDMAChannel, ["DRV_PLC_TX_RX_DMA"])

    global plcPhyTXDMAChannelComment
    plcPhyTXDMAChannelComment = plcPhyComponent.createCommentSymbol("DRV_PLC_TX_DMA_CH_COMMENT", None)
    plcPhyTXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Transmit. Check DMA Manager. !!!")
    plcPhyTXDMAChannelComment.setVisible(False)
    plcPhyTXDMAChannelComment.setDependencies(requestDMAComment, ["DRV_PLC_TX_DMA_CHANNEL"])

    global plcPhyRXDMAChannel
    plcPhyRXDMAChannel = plcPhyComponent.createIntegerSymbol("DRV_PLC_RX_DMA_CHANNEL", None)
    plcPhyRXDMAChannel.setLabel("DMA Channel For Receive")
    plcPhyRXDMAChannel.setDefaultValue(1)
    plcPhyRXDMAChannel.setVisible(False)
    plcPhyRXDMAChannel.setReadOnly(True)
    plcPhyRXDMAChannel.setDependencies(requestAndAssignRxDMAChannel, ["DRV_PLC_TX_RX_DMA"])

    global plcPhyRXDMAChannelComment
    plcPhyRXDMAChannelComment = plcPhyComponent.createCommentSymbol("DRV_PLC_RX_DMA_CH_COMMENT", None)
    plcPhyRXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Receive. Check DMA Manager. !!!")
    plcPhyRXDMAChannelComment.setVisible(False)
    plcPhyRXDMAChannelComment.setDependencies(requestDMAComment, ["DRV_PLC_RX_DMA_CHANNEL"])

    plcPhyDependencyDMAComment = plcPhyComponent.createCommentSymbol("DRV_PLC_DEPENDENCY_DMA_COMMENT", None)
    plcPhyDependencyDMAComment.setLabel("!!! Satisfy PLIB Dependency to Allocate DMA Channel !!!")
    plcPhyDependencyDMAComment.setVisible(isDMAPresent)

    plcPhyKeyCortex = plcPhyComponent.createHexSymbol("DRV_PLC_CORE_KEY", None)
    plcPhyKeyCortex.setLabel("PLC Phy Key Cortex")
    plcPhyKeyCortex.setDefaultValue(0x1122)
    plcPhyKeyCortex.setVisible(False)
    plcPhyKeyCortex.setReadOnly(True)

    ############################################################################
    #### Code Generation ####
    ############################################################################

    configName = Variables.get("__CONFIGURATION_NAME")

    plcPhyHalHeaderFile = plcPhyComponent.createFileSymbol("PLC_HAL_HEADER", None)
    plcPhyHalHeaderFile.setSourcePath("driver/common/plcHal/drv_plc_hal.h.ftl")
    plcPhyHalHeaderFile.setOutputName("drv_plc_hal.h")
    plcPhyHalHeaderFile.setDestPath("driver/plc/common/")
    plcPhyHalHeaderFile.setProjectPath("config/" + configName + "/driver/plc/common/")
    plcPhyHalHeaderFile.setType("HEADER")
    plcPhyHalHeaderFile.setMarkup(True)

    plcPhyHalFile = plcPhyComponent.createFileSymbol("PLC_HAL", None)
    plcPhyHalFile.setSourcePath("driver/common/plcHal/drv_plc_hal.c.ftl")
    plcPhyHalFile.setOutputName("drv_plc_hal.c")
    plcPhyHalFile.setDestPath("driver/plc/common/")
    plcPhyHalFile.setProjectPath("config/" + configName + "/driver/plc/common/")
    plcPhyHalFile.setType("SOURCE")
    plcPhyHalFile.setMarkup(True)

    plcPhyHALInitDataFile = plcPhyComponent.createFileSymbol("DRV_HAL_INIT_DATA", None)
    plcPhyHALInitDataFile.setType("STRING")
    plcPhyHALInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    plcPhyHALInitDataFile.setSourcePath("driver/common/plcHal/drv_plc_hal_initialization_data.c.ftl")
    plcPhyHALInitDataFile.setMarkup(True)

    plcPhyBootFile = plcPhyComponent.createFileSymbol("PLC_BOOT", None)
    plcPhyBootFile.setSourcePath("driver/common/plcBoot/drv_plc_boot.c")
    plcPhyBootFile.setOutputName("drv_plc_boot.c")
    plcPhyBootFile.setDestPath("driver/plc/common/")
    plcPhyBootFile.setProjectPath("config/" + configName + "/driver/plc/common/")
    plcPhyBootFile.setType("SOURCE")

    plcPhyBootHeaderFile = plcPhyComponent.createFileSymbol("PLC_BOOT_HEADER", None)
    plcPhyBootHeaderFile.setSourcePath("driver/common/plcBoot/drv_plc_boot.h")
    plcPhyBootHeaderFile.setOutputName("drv_plc_boot.h")
    plcPhyBootHeaderFile.setDestPath("driver/plc/common/")
    plcPhyBootHeaderFile.setProjectPath("config/" + configName + "/driver/plc/common/")
    plcPhyBootHeaderFile.setType("HEADER")

    plcPhySecureScript = plcPhyComponent.createFileSymbol("DRV_PLC_PHY_SECURE_SCRIPT", None)
    plcPhySecureScript.setSourcePath("driver/plcPhy/src/bin/plcPhy_encfile.py")
    plcPhySecureScript.setOutputName("plcPhy_encfile.py")
    plcPhySecureScript.setDestPath("driver/plc/phy/bin/")
    plcPhySecureScript.setProjectPath("config/" + configName + "/driver/plc/phy/bin/")
    plcPhySecureScript.setType("SOURCE")
    plcPhySecureScript.setEnabled(False)
    plcPhySecureScript.setDependencies(plcPhyEnableEncScript, ["DRV_PLC_PHY_SECURE_MODE"])

    plcPhyAsmPathSetting = plcPhyComponent.createSettingSymbol("DRV_PLC_PHY_ASM_PATH_SETTING", None)
    plcPhyAsmPathSetting.setCategory("C32-AS")
    plcPhyAsmPathSetting.setKey("extra-include-directories-for-assembler")
    plcPhyAsmPathSetting.setValue("../src/config/" + configName + "/driver/plc/phy/bin")
    plcPhyAsmPathSetting.setAppend(True, ";")

    plcPhyHeaderFile = plcPhyComponent.createFileSymbol("DRV_PLC_PHY_HEADER", None)
    plcPhyHeaderFile.setSourcePath("driver/plcPhy/drv_plc_phy.h")
    plcPhyHeaderFile.setOutputName("drv_plc_phy.h")
    plcPhyHeaderFile.setDestPath("driver/plc/phy")
    plcPhyHeaderFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcPhyHeaderFile.setType("HEADER")

    plcPhySymHeaderDefFile = plcPhyComponent.createFileSymbol("DRV_PLC_PHY_DEF", None)
    plcPhySymHeaderDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_definitions.h")
    plcPhySymHeaderDefFile.setOutputName("drv_plc_phy_definitions.h")
    plcPhySymHeaderDefFile.setDestPath("driver/plc/phy")
    plcPhySymHeaderDefFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcPhySymHeaderDefFile.setType("HEADER")

    global plcPhyProfileDefFile
    plcPhyProfileDefFile = plcPhyComponent.createFileSymbol("DRV_PLC_PHY_COMM_DEF", None)
    plcPhyProfileDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_g3.h")
    plcPhyProfileDefFile.setOutputName("drv_plc_phy_comm.h")
    plcPhyProfileDefFile.setDestPath("driver/plc/phy")
    plcPhyProfileDefFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcPhyProfileDefFile.setType("HEADER")

    global plcPhyProfileFile
    plcPhyProfileFile = plcPhyComponent.createFileSymbol("DRV_PLC_PHY_COMM", None)
    plcPhyProfileFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_g3.c")
    plcPhyProfileFile.setOutputName("drv_plc_phy_comm.c")
    plcPhyProfileFile.setDestPath("driver/plc/phy/src")
    plcPhyProfileFile.setProjectPath("config/" + configName + "/driver/plc/phy/src/")
    plcPhyProfileFile.setType("SOURCE")
    plcPhyProfileFile.setDependencies(plcPhyProfileConfiguration, ["DRV_PLC_PHY_PROFILE"])

    plcPhySourceFile = plcPhyComponent.createFileSymbol("DRV_PLC_PHY_SOURCE", None)
    plcPhySourceFile.setSourcePath("driver/plcPhy/src/drv_plc_phy.c")
    plcPhySourceFile.setOutputName("drv_plc_phy.c")
    plcPhySourceFile.setDestPath("driver/plc/phy/src/")
    plcPhySourceFile.setProjectPath("config/" + configName + "/driver/plc/phy/src/")
    plcPhySourceFile.setType("SOURCE")

    global plcPhySourceBinFileG3CENA
    plcPhySourceBinFileG3CENA = plcPhyComponent.createFileSymbol("PLC_PHY_BIN_G3_CENA", None)
    plcPhySourceBinFileG3CENA.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_G3_CENA.bin")
    plcPhySourceBinFileG3CENA.setOutputName("PLC_PHY_G3_CENA.bin")
    plcPhySourceBinFileG3CENA.setDestPath("driver/plc/phy/bin/")
    plcPhySourceBinFileG3CENA.setProjectPath("config/" + configName + "/driver/plc/phy/bin/")
    plcPhySourceBinFileG3CENA.setType("SOURCE")
    plcPhySourceBinFileG3CENA.setEnabled(True)
    plcPhySourceBinFileG3CENA.setVisible(False)

    global plcPhySourceBinFileG3CENB
    plcPhySourceBinFileG3CENB = plcPhyComponent.createFileSymbol("PLC_PHY_BIN_G3_CENB", None)
    plcPhySourceBinFileG3CENB.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_G3_CENB.bin")
    plcPhySourceBinFileG3CENB.setOutputName("PLC_PHY_G3_CENB.bin")
    plcPhySourceBinFileG3CENB.setDestPath("driver/plc/phy/bin/")
    plcPhySourceBinFileG3CENB.setProjectPath("config/" + configName + "/driver/plc/phy/bin/")
    plcPhySourceBinFileG3CENB.setType("SOURCE")
    plcPhySourceBinFileG3CENB.setEnabled(False)
    plcPhySourceBinFileG3CENB.setVisible(False)

    global plcPhySourceBinFileG3FCC
    plcPhySourceBinFileG3FCC = plcPhyComponent.createFileSymbol("PLC_PHY_BIN_G3_FCC", None)
    plcPhySourceBinFileG3FCC.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_G3_FCC.bin")
    plcPhySourceBinFileG3FCC.setOutputName("PLC_PHY_G3_FCC.bin")
    plcPhySourceBinFileG3FCC.setDestPath("driver/plc/phy/bin/")
    plcPhySourceBinFileG3FCC.setProjectPath("config/" + configName + "/driver/plc/phy/bin/")
    plcPhySourceBinFileG3FCC.setType("SOURCE")
    plcPhySourceBinFileG3FCC.setEnabled(False)
    plcPhySourceBinFileG3FCC.setVisible(False)

    global plcPhySourceBinFileG3ARIB
    plcPhySourceBinFileG3ARIB = plcPhyComponent.createFileSymbol("PLC_PHY_BIN_G3_ARIB", None)
    plcPhySourceBinFileG3ARIB.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_G3_ARIB.bin")
    plcPhySourceBinFileG3ARIB.setOutputName("PLC_PHY_G3_ARIB.bin")
    plcPhySourceBinFileG3ARIB.setDestPath("driver/plc/phy/bin/")
    plcPhySourceBinFileG3ARIB.setProjectPath("config/" + configName + "/driver/plc/phy/bin/")
    plcPhySourceBinFileG3ARIB.setType("SOURCE")
    plcPhySourceBinFileG3ARIB.setEnabled(False)
    plcPhySourceBinFileG3ARIB.setVisible(False)

    global plcPhySourceBinFilePRIME
    plcPhySourceBinFilePRIME = plcPhyComponent.createFileSymbol("PLC_PHY_BIN_PRIME", None)
    plcPhySourceBinFilePRIME.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_PRIME.bin")
    plcPhySourceBinFilePRIME.setOutputName("PLC_PHY_PRIME.bin")
    plcPhySourceBinFilePRIME.setDestPath("driver/plc/phy/bin/")
    plcPhySourceBinFilePRIME.setProjectPath("config/" + configName + "/driver/plc/phy/bin/")
    plcPhySourceBinFilePRIME.setType("SOURCE")
    plcPhySourceBinFilePRIME.setEnabled(False)
    plcPhySourceBinFilePRIME.setVisible(False)

    global plcPhyAssemblyBinFile
    plcPhyAssemblyBinFile = plcPhyComponent.createFileSymbol("PLC_ASSEMBLY_BIN", None)
    plcPhyAssemblyBinFile.setSourcePath("driver/plcPhy/src/bin/plc_phy_bin.S.ftl")
    plcPhyAssemblyBinFile.setOutputName("plc_phy_bin.S")
    plcPhyAssemblyBinFile.setDestPath("driver/plc/phy/bin/")
    plcPhyAssemblyBinFile.setProjectPath("config/" + configName + "/driver/plc/phy/bin/")
    plcPhyAssemblyBinFile.setType("SOURCE")
    plcPhyAssemblyBinFile.setMarkup(True)

    plcPhyHeaderLocalFile = plcPhyComponent.createFileSymbol("DRV_PLC_PHY_HEADER_LOCAL", None)
    plcPhyHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local.h")
    plcPhyHeaderLocalFile.setOutputName("drv_plc_phy_local.h")
    plcPhyHeaderLocalFile.setDestPath("driver/plc/phy/src")
    plcPhyHeaderLocalFile.setProjectPath("config/" + configName + "/driver/plc/phy/src/")
    plcPhyHeaderLocalFile.setType("SOURCE")
    plcPhyHeaderLocalFile.setEnabled(True)

    global plcPhyProfileHeaderLocalFile
    plcPhyProfileHeaderLocalFile = plcPhyComponent.createFileSymbol("DRV_PLC_PHY_PROFILE_LOCAL", None)
    plcPhyProfileHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local_g3.h")
    plcPhyProfileHeaderLocalFile.setOutputName("drv_plc_phy_local_comm.h")
    plcPhyProfileHeaderLocalFile.setDestPath("driver/plc/phy/src")
    plcPhyProfileHeaderLocalFile.setProjectPath("config/" + configName + "/driver/plc/phy/src/")
    plcPhyProfileHeaderLocalFile.setType("SOURCE")
    plcPhyProfileHeaderLocalFile.setEnabled(True)

    plcPhySystemDefFile = plcPhyComponent.createFileSymbol("PLC_DEF", None)
    plcPhySystemDefFile.setType("STRING")
    plcPhySystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    plcPhySystemDefFile.setSourcePath("driver/plcPhy/templates/system/definitions.h.ftl")
    plcPhySystemDefFile.setMarkup(True)

    plcPhySymSystemDefObjFile = plcPhyComponent.createFileSymbol("DRV_PLC_PHY_SYSTEM_DEF_OBJECT", None)
    plcPhySymSystemDefObjFile.setType("STRING")
    plcPhySymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    plcPhySymSystemDefObjFile.setSourcePath("driver/plcPhy/templates/system/definitions_objects.h.ftl")
    plcPhySymSystemDefObjFile.setMarkup(True)

    plcPhySymSystemConfigFile = plcPhyComponent.createFileSymbol("DRV_PLC_PHY_CONFIGIRUTION", None)
    plcPhySymSystemConfigFile.setType("STRING")
    plcPhySymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    plcPhySymSystemConfigFile.setSourcePath("driver/plcPhy/templates/system/configuration.h.ftl")
    plcPhySymSystemConfigFile.setMarkup(True)

    plcPhySymSystemInitDataFile = plcPhyComponent.createFileSymbol("DRV_PLC_PHY_INIT_DATA", None)
    plcPhySymSystemInitDataFile.setType("STRING")
    plcPhySymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    plcPhySymSystemInitDataFile.setSourcePath("driver/plcPhy/templates/system/initialize_data.c.ftl")
    plcPhySymSystemInitDataFile.setMarkup(True)

    plcPhySystemInitFile = plcPhyComponent.createFileSymbol("PLC_INIT", None)
    plcPhySystemInitFile.setType("STRING")
    plcPhySystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    plcPhySystemInitFile.setSourcePath("driver/plcPhy/templates/system/initialize.c.ftl")
    plcPhySystemInitFile.setMarkup(True)

    plcPhySystemTasksFile = plcPhyComponent.createFileSymbol("DRV_PLC_PHY_SYS_TASK", None)
    plcPhySystemTasksFile.setType("STRING")
    plcPhySystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    plcPhySystemTasksFile.setSourcePath("driver/plcPhy/templates/system/system_tasks.c.ftl")
    plcPhySystemTasksFile.setMarkup(True)

################################################################################
#### Business Logic ####
################################################################################  
def onAttachmentConnected(source, target):
    global isDMAPresent

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    targetID = target["id"]

    if connectID == "drv_plc_phy_SPI_dependency":
        plibUsed = localComponent.getSymbolByID("DRV_PLC_PLIB")
        plibUsed.clearValue()
        plibUsed.setValue(remoteID.upper())

        Database.setSymbolValue(remoteID, "SPI_DRIVER_CONTROLLED", True)
        dmaChannelSym = Database.getSymbolValue("core", "DMA_CH_FOR_" + remoteID.upper() + "_Transmit")
        dmaRequestSym = Database.getSymbolValue("core", "DMA_CH_NEEDED_FOR_" + remoteID.upper() + "_Transmit")

        # Do not change the order as DMA Channels needs to be allocated
        # after setting the plibUsed symbol
        # Both device and connected plib should support DMA
        if isDMAPresent == True and dmaChannelSym != None and dmaRequestSym != None:
            localComponent.getSymbolByID("DRV_PLC_DEPENDENCY_DMA_COMMENT").setVisible(False)
            localComponent.getSymbolByID("DRV_PLC_TX_RX_DMA").setReadOnly(False)

def onAttachmentDisconnected(source, target):
    global isDMAPresent

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    if connectID == "drv_plc_phy_SPI_dependency":

        dmaChannelSym = Database.getSymbolValue("core", "DMA_CH_FOR_" + remoteID.upper() + "_Transmit")
        dmaRequestSym = Database.getSymbolValue("core", "DMA_CH_NEEDED_FOR_" + remoteID.upper() + "_Transmit")

        # Do not change the order as DMA Channels needs to be cleared
        # before clearing the plibUsed symbol
        # Both device and connected plib should support DMA
        if isDMAPresent == True and dmaChannelSym != None and dmaRequestSym != None:
            localComponent.getSymbolByID("DRV_PLC_TX_RX_DMA").clearValue()
            localComponent.getSymbolByID("DRV_PLC_TX_RX_DMA").setReadOnly(True)
            localComponent.getSymbolByID("DRV_PLC_DEPENDENCY_DMA_COMMENT").setVisible(True)

        plibUsed = localComponent.getSymbolByID("DRV_PLC_PLIB")
        plibUsed.clearValue()
        Database.setSymbolValue(remoteID, "SPI_DRIVER_CONTROLLED", False)

def requestAndAssignTxDMAChannel(symbol, event):
    global drvPlcPhyInstanceSpace
    global plcPhyTXDMAChannelComment

    print("requestAndAssignTxDMAChannel")

    spiPeripheral = Database.getSymbolValue(drvPlcPhyInstanceSpace, "DRV_PLC_PLIB")

    dmaChannelID = "DMA_CH_FOR_" + str(spiPeripheral) + "_Transmit"
    dmaRequestID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Transmit"

    if event["value"] == False:
        Database.setSymbolValue("core", dmaRequestID, False)
        plcPhyTXDMAChannelComment.setVisible(False)
        symbol.setVisible(False)
    else:
        symbol.setVisible(True)
        Database.setSymbolValue("core", dmaRequestID, True)

    # Get the allocated channel and assign it
    channel = Database.getSymbolValue("core", dmaChannelID)
    print("channel:" + str(channel))
    symbol.setValue(channel)

def requestAndAssignRxDMAChannel(symbol, event):
    global drvPlcPhyInstanceSpace
    global plcPhyRXDMAChannelComment

    print("requestAndAssignRxDMAChannel")

    spiPeripheral = Database.getSymbolValue(drvPlcPhyInstanceSpace, "DRV_PLC_PLIB")

    dmaChannelID = "DMA_CH_FOR_" + str(spiPeripheral) + "_Receive"
    dmaRequestID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Receive"

    if event["value"] == False:
        Database.setSymbolValue("core", dmaRequestID, False)
        plcPhyRXDMAChannelComment.setVisible(False)
        symbol.setVisible(False)
    else:
        symbol.setVisible(True)
        Database.setSymbolValue("core", dmaRequestID, True)

    # Get the allocated channel and assign it
    channel = Database.getSymbolValue("core", dmaChannelID)
    print("channel:" + str(channel))
    symbol.setValue(channel)

def requestDMAComment(symbol, event):
    global plcPhyTXRXDMA

    if(event["value"] == -2) and (plcPhyTXRXDMA.getValue() == True):
        symbol.setVisible(True)
        event["symbol"].setVisible(False)
    else:
        symbol.setVisible(False)

def destroyComponent(spiComponent):
    global drvPlcPhyInstanceSpace

    if isDMAPresent:
        spiPeripheral = Database.getSymbolValue(drvPlcPhyInstanceSpace, "DRV_PLC_PLIB")

        dmaTxID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Transmit"
        dmaRxID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Receive"

        Database.setSymbolValue("core", dmaTxID, False)
        Database.setSymbolValue("core", dmaRxID, False)