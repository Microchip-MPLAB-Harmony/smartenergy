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
def externalAddressingTrigger(symbol, event):
    global pl360SourceBinFileG3CENA
    global pl360SourceBinFileG3CENB
    global pl360SourceBinFileG3FCC
    global pl360SourceBinFilePRIME
    global pl360ExternalAddressing
    global pl360AssemblyBinFile
    if event["value"] == True:
        print("externalAddressingTrigger disable BIN files")
        pl360SourceBinFileG3CENA.setEnabled(False)
        pl360SourceBinFileG3CENB.setEnabled(False)
        pl360SourceBinFileG3FCC.setEnabled(False)
        pl360SourceBinFilePRIME.setEnabled(False)
        pl360AssemblyBinFile.setEnabled(False)
    else:
        pl360AssemblyBinFile.setEnabled(True)
        prof = symbol.getValue()
        if(prof == 0):
            print("externalAddressingTrigger restore G3 CENA")
            pl360SourceBinFileG3CENA.setEnabled(True)
        elif(prof == 1):
            print("externalAddressingTrigger restore G3 CENB")
            pl360SourceBinFileG3CENB.setEnabled(True)
        elif(prof == 2):
            print("externalAddressingTrigger restore G3 FCC")
            pl360SourceBinFileG3FCC.setEnabled(True)
        elif(prof == 3):
            print("externalAddressingTrigger restore PRIME")
            pl360SourceBinFilePRIME.setEnabled(True)
        elif(prof == 4):
            print("externalAddressingTrigger restore G3 CENA (multi)")
            pl360SourceBinFileG3CENA.setEnabled(True)



def pl360ProfileConfiguration(symbol, event):
    global pl360ProfileFile
    global pl360ProfileDefFile
    global pl360ProfileHeaderLocalFile
    global pl360SourceBinFileG3CENA
    global pl360SourceBinFileG3CENB
    global pl360SourceBinFileG3FCC
    global pl360SourceBinFilePRIME
    global pl360ExternalAddressing
    global pl360AssemblyBinFile
    if (event["symbol"].getKeyDescription(event["value"]) == "PRIME"):
        pl360ProfileFile.setSourcePath("driver/pl360/src/drv_pl360_prime.c")
        pl360ProfileDefFile.setSourcePath("driver/pl360/drv_pl360_prime.h")
        pl360ProfileHeaderLocalFile.setSourcePath("driver/pl360/src/drv_pl360_local_prime.h")
        pl360SourceBinFileG3CENA.setEnabled(False)
        pl360SourceBinFileG3CENB.setEnabled(False)
        pl360SourceBinFileG3FCC.setEnabled(False)
        if (pl360ExternalAddressing.getValue() == False):
            print("pl360ProfileConfiguration update PRIME files")
            pl360SourceBinFilePRIME.setEnabled(True)
            pl360AssemblyBinFile.setEnabled(True)
        else:
            pl360SourceBinFilePRIME.setEnabled(False)
            pl360AssemblyBinFile.setEnabled(False)
            print("pl360ProfileConfiguration update no bin files (PRIME)")

    else:
        pl360SourceBinFilePRIME.setEnabled(False)
        pl360ProfileFile.setSourcePath("driver/pl360/src/drv_pl360_g3.c")
        pl360ProfileDefFile.setSourcePath("driver/pl360/drv_pl360_g3.h")
        pl360ProfileHeaderLocalFile.setSourcePath("driver/pl360/src/drv_pl360_local_g3.h")
        if (pl360ExternalAddressing.getValue() == False):
            pl360AssemblyBinFile.setEnabled(True)
            if (event["symbol"].getKeyDescription(event["value"]) == "G3_CEN_A"):            
                pl360SourceBinFileG3CENA.setEnabled(True)
                pl360SourceBinFileG3CENB.setEnabled(False)
                pl360SourceBinFileG3FCC.setEnabled(False)
                print("pl360ProfileConfiguration update G3 CEN A files")
            elif (event["symbol"].getKeyDescription(event["value"]) == "G3_CEN_B"):           
                pl360SourceBinFileG3CENA.setEnabled(False)
                pl360SourceBinFileG3CENB.setEnabled(True)
                pl360SourceBinFileG3FCC.setEnabled(False)
                print("pl360ProfileConfiguration update G3 CEN B files")
            elif (event["symbol"].getKeyDescription(event["value"]) == "G3_FCC"):           
                pl360SourceBinFileG3CENA.setEnabled(False)
                pl360SourceBinFileG3CENB.setEnabled(False)
                pl360SourceBinFileG3FCC.setEnabled(True)
                print("pl360ProfileConfiguration update G3 FCC files")
            else:           
                pl360SourceBinFileG3CENA.setEnabled(True)
                pl360SourceBinFileG3CENB.setEnabled(False)
                pl360SourceBinFileG3FCC.setEnabled(True)
                print("pl360ProfileConfiguration update G3 CEN A nd FCC files")
        else:
            print("pl360ProfileConfiguration update no bin files (G3)")
            pl360SourceBinFileG3CENA.setEnabled(False)
            pl360SourceBinFileG3CENB.setEnabled(False)
            pl360SourceBinFileG3FCC.setEnabled(False)
            pl360AssemblyBinFile.setEnabled(False)

def pl360ExternalInterruptTrigger(symbol, event):
    global pl360ExtIntSource

    key = event["symbol"].getKeyDescription(event["value"])
    intSrc = "PIO" + key[1] + "_IRQn"
    pl360ExtIntSource.setValue(intSrc, 1)

        
def pl360EnableEncScript(symbol, event):
    if (event["value"] == True):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False) 

def pl360VisibleEncComment(symbol, event):
    if (event["value"] == True):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)         
		
def instantiateComponent(pl360Component):  

    res = Database.activateComponents(["HarmonyCore"])

    # Enable "Generate Harmony Driver Common Files" option in MHC
    Database.setSymbolValue("HarmonyCore", "ENABLE_DRV_COMMON", True, 1)

    # Enable "Generate Harmony System Service Common Files" option in MHC
    Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_COMMON", True, 1)
    
    pl360SymNumInst = pl360Component.createIntegerSymbol("DRV_PL360_NUM_INSTANCES", None)
    pl360SymNumInst.setLabel("Number of Instances")
    pl360SymNumInst.setVisible(False)
    pl360SymNumInst.setDefaultValue(1)

    pl360PLIB = pl360Component.createStringSymbol("DRV_PL360_PLIB", None)
    pl360PLIB.setLabel("PLIB Used")
    pl360PLIB.setReadOnly(True)
    
    pl360SymNumClients = pl360Component.createIntegerSymbol("DRV_PL360_NUM_CLIENTS", None)
    pl360SymNumClients.setLabel("Number of Clients")
    pl360SymNumClients.setReadOnly(True)
    pl360SymNumClients.setDefaultValue(1)

    global pl360ExtIntPin
    pl360ExtIntPin = pl360Component.createKeyValueSetSymbol("DRV_PL360_EXT_INT_PIN", None)
    pl360ExtIntPin.setLabel("External Interrupt Pin")
    pl360ExtIntPin.setDefaultValue(104) #PD28
    pl360ExtIntPin.setOutputMode("Key")
    pl360ExtIntPin.setDisplayMode("Description")

    global pl360ExtIntSource
    pl360ExtIntSource = pl360Component.createStringSymbol("DRV_PL360_EXT_INT_SRC", None)
    pl360ExtIntSource.setLabel("External Interrupt Source")
    pl360ExtIntSource.setDefaultValue("PIOD_IRQn") #PIOD_IRQn
    pl360ExtIntSource.setVisible(True)
    pl360ExtIntSource.setReadOnly(True)
    pl360ExtIntSource.setDependencies(pl360ExternalInterruptTrigger, ["DRV_PL360_EXT_INT_PIN"])

    pl360ResetPin = pl360Component.createKeyValueSetSymbol("DRV_PL360_RESET_PIN", None)
    pl360ResetPin.setLabel("Reset Pin")
    pl360ResetPin.setDefaultValue(34) #PB2
    pl360ResetPin.setOutputMode("Key")
    pl360ResetPin.setDisplayMode("Description")

    pl360LDOEnPin = pl360Component.createKeyValueSetSymbol("DRV_PL360_LDO_EN_PIN", None)
    pl360LDOEnPin.setLabel("LDO Enable Pin")
    pl360LDOEnPin.setDefaultValue(35) #PB3
    pl360LDOEnPin.setOutputMode("Key")
    pl360LDOEnPin.setDisplayMode("Description")    

    myVariableValue = Database.getSymbolValue("core", "COMPONENT_PACKAGE")
    pinOutNode = ATDF.getNode('/avr-tools-device-file/pinouts/pinout@[name= "' + str(myVariableValue) + '"]')
    # pinOutNode = ATDF.getNode("/avr-tools-device-file/pinouts/pinout")
    pinOut = pinOutNode.getChildren()
    
    for pad in range(0, len(pinOut)):
        pin = pinOut[pad].getAttribute("pad")
        if (pin[0] == "P") and (pin[-1].isdigit()):
            key = "SYS_PORT_PIN_" + pin
            value = pinOut[pad].getAttribute("position")
            description = pinOut[pad].getAttribute("pad")
            pl360ExtIntPin.addKey(key, value, description)
            pl360ResetPin.addKey(key, value, description)
            pl360LDOEnPin.addKey(key, value, description)

    pl360SymPinConfigComment = pl360Component.createCommentSymbol("DRV_PL360_PINS_CONFIG_COMMENT", None)
    pl360SymPinConfigComment.setVisible(True)
    pl360SymPinConfigComment.setLabel("***Above selected pins must be configured as GPIO Output in Pin Manager***")
    
    global pl360TXDMAChannel
    pl360TXDMAChannel = pl360Component.createIntegerSymbol("DRV_PL360_TX_DMA_CHANNEL", None)
    pl360TXDMAChannel.setLabel("DMA Channel For Transmit")
    pl360TXDMAChannel.setDefaultValue(0)
    pl360TXDMAChannel.setVisible(False)
    pl360TXDMAChannel.setReadOnly(True)

    global pl360RXDMAChannel
    pl360RXDMAChannel = pl360Component.createIntegerSymbol("DRV_PL360_RX_DMA_CHANNEL", None)
    pl360RXDMAChannel.setLabel("DMA Channel For Receive")
    pl360RXDMAChannel.setDefaultValue(1)
    pl360RXDMAChannel.setVisible(False)
    pl360RXDMAChannel.setReadOnly(True)

    global pl360DMAChannelComment
    pl360DMAChannelComment = pl360Component.createCommentSymbol("DRV_PL360_DMA_CH_COMMENT", None)
    pl360DMAChannelComment.setLabel("***Couldn't Allocate DMA Channel for Transmit/Receive. Check DMA manager.***")
    pl360DMAChannelComment.setVisible(False)

    global pl360ExternalAddressing
    pl360ExternalAddressing = pl360Component.createBooleanSymbol("DRV_PL360_EXTERNAL_ADDRESSING", None)
    pl360ExternalAddressing.setLabel("External Binary Image Block Transfer")
    pl360ExternalAddressing.setVisible(True)
    pl360ExternalAddressing.setDefaultValue(False)

    global pl360Profile
    pl360Profile = pl360Component.createKeyValueSetSymbol("DRV_PL360_PLC_PROFILE", None)
    pl360Profile.setLabel("Select PLC Profile")
    pl360Profile.addKey("G3_CEN_A", "0", "G3_CEN_A")
    pl360Profile.addKey("G3_CEN_B", "1", "G3_CEN_B")
    pl360Profile.addKey("G3_FCC", "2", "G3_FCC")
    pl360Profile.addKey("PRIME", "4", "PRIME")
    pl360Profile.addKey("G3_MULTIBAND", "5", "G3_MULTIBAND")
    pl360Profile.setDisplayMode("Description")
    pl360Profile.setOutputMode("Value")
    pl360Profile.setDefaultValue(0)
    pl360Profile.setDependencies(externalAddressingTrigger, ["DRV_PL360_EXTERNAL_ADDRESSING"])

    pl360SecureMode = pl360Component.createBooleanSymbol("DRV_PL360_SECURE_MODE", None)
    pl360SecureMode.setLabel("PL360 Secure Mode")
    pl360SecureMode.setVisible(True)
    pl360SecureMode.setDefaultValue(False)

    pl360SecureComment = pl360Component.createCommentSymbol("DRV_PL360_SECURE_COMMENT", None)
    pl360SecureComment.setLabel("***Run encription script to get a secure binary file and replace the current non-secure file. Check bin folder.***")
    pl360SecureComment.setVisible(False)
    pl360SecureComment.setDependencies(pl360VisibleEncComment, ["DRV_PL360_SECURE_MODE"])

    ############################################################################
    #### Code Generation ####
    ############################################################################

    configName = Variables.get("__CONFIGURATION_NAME")

    pl360SecureScript = pl360Component.createFileSymbol("DRV_PL360_SECURE_SCRIPT", None)
    pl360SecureScript.setSourcePath("driver/pl360/src/bin/pl360_encfile.py")
    pl360SecureScript.setOutputName("pl360_encfile.py")
    pl360SecureScript.setDestPath("driver/pl360/bin/")
    pl360SecureScript.setProjectPath("config/" + configName + "/driver/pl360/bin/")
    pl360SecureScript.setType("SOURCE")
    pl360SecureScript.setEnabled(False)
    pl360SecureScript.setDependencies(pl360EnableEncScript, ["DRV_PL360_SECURE_MODE"])


    pl360AsmPathSetting = pl360Component.createSettingSymbol("DRV_PL360_ASM_PATH_SETTING", None)
    pl360AsmPathSetting.setCategory("C32-AS")
    pl360AsmPathSetting.setKey("extra-include-directories-for-assembler")
    pl360AsmPathSetting.setValue("../src/config/" + configName + "/driver/pl360/bin")
    pl360AsmPathSetting.setAppend(True, ";")

    pl360HeaderFile = pl360Component.createFileSymbol("PL360_HEADER", None)
    pl360HeaderFile.setSourcePath("driver/pl360/drv_pl360.h")
    pl360HeaderFile.setOutputName("drv_pl360.h")
    pl360HeaderFile.setDestPath("driver/pl360/")
    pl360HeaderFile.setProjectPath("config/" + configName + "/driver/pl360/")
    pl360HeaderFile.setType("HEADER")

    pl360SymHeaderDefFile = pl360Component.createFileSymbol("DRV_PL360_DEF", None)
    pl360SymHeaderDefFile.setSourcePath("driver/pl360/drv_pl360_definitions.h")
    pl360SymHeaderDefFile.setOutputName("drv_pl360_definitions.h")
    pl360SymHeaderDefFile.setDestPath("driver/pl360")
    pl360SymHeaderDefFile.setProjectPath("config/" + configName + "/driver/pl360/")
    pl360SymHeaderDefFile.setType("HEADER")

    global pl360ProfileFile
    pl360ProfileFile = pl360Component.createFileSymbol("DRV_PL360_PROFILE", None)
    pl360ProfileFile.setSourcePath("driver/pl360/src/drv_pl360_g3.c")
    pl360ProfileFile.setOutputName("drv_pl360_comm.c")
    pl360ProfileFile.setDestPath("driver/pl360")
    pl360ProfileFile.setProjectPath("config/" + configName + "/driver/pl360/")
    pl360ProfileFile.setType("SOURCE")
    pl360ProfileFile.setDependencies(pl360ProfileConfiguration, ["DRV_PL360_PLC_PROFILE"])

    global pl360ProfileDefFile
    pl360ProfileDefFile = pl360Component.createFileSymbol("DRV_PL360_PROFILE_DEF", None)
    pl360ProfileDefFile.setSourcePath("driver/pl360/drv_pl360_g3.h")
    pl360ProfileDefFile.setOutputName("drv_pl360_comm.h")
    pl360ProfileDefFile.setDestPath("driver/pl360")
    pl360ProfileDefFile.setProjectPath("config/" + configName + "/driver/pl360/")
    pl360ProfileDefFile.setType("HEADER")

    pl360SourceFile = pl360Component.createFileSymbol("PL360_SOURCE", None)
    pl360SourceFile.setSourcePath("driver/pl360/src/drv_pl360.c")
    pl360SourceFile.setOutputName("drv_pl360.c")
    pl360SourceFile.setDestPath("driver/pl360/")
    pl360SourceFile.setProjectPath("config/" + configName + "/driver/pl360/")
    pl360SourceFile.setType("SOURCE")

    global pl360SourceBinFileG3CENA
    pl360SourceBinFileG3CENA = pl360Component.createFileSymbol("PL360_SOURCE_BIN_G3_CENA", None)
    pl360SourceBinFileG3CENA.setSourcePath("driver/pl360/src/bin/ATPL360B_G3_CENA.bin")
    pl360SourceBinFileG3CENA.setOutputName("PL360_G3_CENA.bin")
    pl360SourceBinFileG3CENA.setDestPath("driver/pl360/bin/")
    pl360SourceBinFileG3CENA.setProjectPath("config/" + configName + "/driver/pl360/bin/")
    pl360SourceBinFileG3CENA.setType("SOURCE")
    pl360SourceBinFileG3CENA.setEnabled(True)
    pl360SourceBinFileG3CENA.setVisible(False)

    global pl360SourceBinFileG3CENB
    pl360SourceBinFileG3CENB = pl360Component.createFileSymbol("PL360_SOURCE_BIN_G3_CENB", None)
    pl360SourceBinFileG3CENB.setSourcePath("driver/pl360/src/bin/ATPL360B_G3_CENB.bin")
    pl360SourceBinFileG3CENB.setOutputName("PL360_G3_CENB.bin")
    pl360SourceBinFileG3CENB.setDestPath("driver/pl360/bin/")
    pl360SourceBinFileG3CENB.setProjectPath("config/" + configName + "/driver/pl360/bin/")
    pl360SourceBinFileG3CENB.setType("SOURCE")
    pl360SourceBinFileG3CENB.setEnabled(False)
    pl360SourceBinFileG3CENB.setVisible(False)

    global pl360SourceBinFileG3FCC
    pl360SourceBinFileG3FCC = pl360Component.createFileSymbol("PL360_SOURCE_BIN_G3_FCC", None)
    pl360SourceBinFileG3FCC.setSourcePath("driver/pl360/src/bin/ATPL360B_G3_FCC.bin")
    pl360SourceBinFileG3FCC.setOutputName("PL360_G3_FCC.bin")
    pl360SourceBinFileG3FCC.setDestPath("driver/pl360/bin/")
    pl360SourceBinFileG3FCC.setProjectPath("config/" + configName + "/driver/pl360/bin/")
    pl360SourceBinFileG3FCC.setType("SOURCE")
    pl360SourceBinFileG3FCC.setEnabled(False)
    pl360SourceBinFileG3FCC.setVisible(False)

    global pl360SourceBinFilePRIME
    pl360SourceBinFilePRIME = pl360Component.createFileSymbol("PL360_SOURCE_BIN_PRIME", None)
    pl360SourceBinFilePRIME.setSourcePath("driver/pl360/src/bin/ATPL360B_PRIME.bin")
    pl360SourceBinFilePRIME.setOutputName("PL360_PRIME.bin")
    pl360SourceBinFilePRIME.setDestPath("driver/pl360/bin/")
    pl360SourceBinFilePRIME.setProjectPath("config/" + configName + "/driver/pl360/bin/")
    pl360SourceBinFilePRIME.setType("SOURCE")
    pl360SourceBinFilePRIME.setEnabled(False)
    pl360SourceBinFilePRIME.setVisible(False)

    global pl360AssemblyBinFile
    pl360AssemblyBinFile = pl360Component.createFileSymbol("PL360_ASSEMBLY_BIN", None)
    pl360AssemblyBinFile.setSourcePath("driver/pl360/src/bin/pl360_bin.S.ftl")
    pl360AssemblyBinFile.setOutputName("pl360_bin.S")
    pl360AssemblyBinFile.setDestPath("driver/pl360/bin/")
    pl360AssemblyBinFile.setProjectPath("config/" + configName + "/driver/pl360/bin/")
    pl360AssemblyBinFile.setType("SOURCE")
    pl360AssemblyBinFile.setMarkup(True)

    pl360BootFile = pl360Component.createFileSymbol("PL360_BOOT", None)
    pl360BootFile.setSourcePath("driver/pl360/src/drv_pl360_boot.c")
    pl360BootFile.setOutputName("drv_pl360_boot.c")
    pl360BootFile.setDestPath("driver/pl360/")
    pl360BootFile.setProjectPath("config/" + configName + "/driver/pl360/")
    pl360BootFile.setType("SOURCE")

    pl360BootHeaderFile = pl360Component.createFileSymbol("PL360_BOOT_HEADER", None)
    pl360BootHeaderFile.setSourcePath("driver/pl360/src/drv_pl360_boot.h")
    pl360BootHeaderFile.setOutputName("drv_pl360_boot.h")
    pl360BootHeaderFile.setDestPath("driver/pl360/src")
    pl360BootHeaderFile.setProjectPath("config/" + configName + "/driver/pl360/")
    pl360BootHeaderFile.setType("SOURCE")

    pl360HalFile = pl360Component.createFileSymbol("PL360_HAL", None)
    pl360HalFile.setSourcePath("driver/pl360/src/drv_pl360_hal.c")
    pl360HalFile.setOutputName("drv_pl360_hal.c")
    pl360HalFile.setDestPath("driver/pl360/")
    pl360HalFile.setProjectPath("config/" + configName + "/driver/pl360/")
    pl360HalFile.setType("SOURCE")

    pl360HalHeaderFile = pl360Component.createFileSymbol("PL360_HAL_HEADER", None)
    pl360HalHeaderFile.setSourcePath("driver/pl360/drv_pl360_hal.h")
    pl360HalHeaderFile.setOutputName("drv_pl360_hal.h")
    pl360HalHeaderFile.setDestPath("driver/pl360/")
    pl360HalHeaderFile.setProjectPath("config/" + configName + "/driver/pl360/")
    pl360HalHeaderFile.setType("HEADER")

    pl360HeaderLocalFile = pl360Component.createFileSymbol("DRV_PL360_HEADER_LOCAL", None)
    pl360HeaderLocalFile.setSourcePath("driver/pl360/src/drv_pl360_local.h")
    pl360HeaderLocalFile.setOutputName("drv_pl360_local.h")
    pl360HeaderLocalFile.setDestPath("driver/pl360/src")
    pl360HeaderLocalFile.setProjectPath("config/" + configName + "/driver/pl360/")
    pl360HeaderLocalFile.setType("SOURCE")
    pl360HeaderLocalFile.setEnabled(True)

    global pl360ProfileHeaderLocalFile
    pl360ProfileHeaderLocalFile = pl360Component.createFileSymbol("DRV_PL360_PROFILE_LOCAL", None)
    pl360ProfileHeaderLocalFile.setSourcePath("driver/pl360/src/drv_pl360_local_g3.h")
    pl360ProfileHeaderLocalFile.setOutputName("drv_pl360_local_comm.h")
    pl360ProfileHeaderLocalFile.setDestPath("driver/pl360/src")
    pl360ProfileHeaderLocalFile.setProjectPath("config/" + configName + "/driver/pl360/")
    pl360ProfileHeaderLocalFile.setType("SOURCE")
    pl360ProfileHeaderLocalFile.setEnabled(True)

    pl360SystemDefFile = pl360Component.createFileSymbol("PL360_DEF", None)
    pl360SystemDefFile.setType("STRING")
    pl360SystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    pl360SystemDefFile.setSourcePath("driver/pl360/templates/system/definitions.h.ftl")
    pl360SystemDefFile.setMarkup(True)

    pl360SymSystemDefObjFile = pl360Component.createFileSymbol("DRV_PL360_SYSTEM_DEF_OBJECT", None)
    pl360SymSystemDefObjFile.setType("STRING")
    pl360SymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    pl360SymSystemDefObjFile.setSourcePath("driver/pl360/templates/system/definitions_objects.h.ftl")
    pl360SymSystemDefObjFile.setMarkup(True)

    pl360SymSystemConfigFile = pl360Component.createFileSymbol("DRV_PL360_CONFIGIRUTION", None)
    pl360SymSystemConfigFile.setType("STRING")
    pl360SymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    pl360SymSystemConfigFile.setSourcePath("driver/pl360/templates/system/configuration.h.ftl")
    pl360SymSystemConfigFile.setMarkup(True)

    pl360SymSystemInitDataFile = pl360Component.createFileSymbol("DRV_PL360_INIT_DATA", None)
    pl360SymSystemInitDataFile.setType("STRING")
    pl360SymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    pl360SymSystemInitDataFile.setSourcePath("driver/pl360/templates/system/initialize_data.c.ftl")
    pl360SymSystemInitDataFile.setMarkup(True)

    pl360SystemInitFile = pl360Component.createFileSymbol("PL360_INIT", None)
    pl360SystemInitFile.setType("STRING")
    pl360SystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    pl360SystemInitFile.setSourcePath("driver/pl360/templates/system/initialize.c.ftl")
    pl360SystemInitFile.setMarkup(True)

    pl360SystemTasksFile = pl360Component.createFileSymbol("DRV_PL360_SYS_TASK", None)
    pl360SystemTasksFile.setType("STRING")
    pl360SystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    pl360SystemTasksFile.setSourcePath("driver/pl360/templates/system/system_tasks.c.ftl")
    pl360SystemTasksFile.setMarkup(True)

################################################################################
#### Business Logic ####
################################################################################  
def onAttachmentConnected(source, target):
    global pl360TXDMAChannel
    global pl360RXDMAChannel
    global pl360DMAChannelComment
    global pl360Profile

    print("onAttachmentConnected event")

    localComponent = source["component"]
    remoteComponent = target["component"]
    pl360PlibId = remoteComponent.getID().upper()
    connectID = source["id"]

    dmaRxRequestID = "DMA_CH_NEEDED_FOR_" + pl360PlibId + "_Receive"
    dmaTxRequestID = "DMA_CH_NEEDED_FOR_" + pl360PlibId + "_Transmit"
    dmaTxChannelID = "DMA_CH_FOR_" + pl360PlibId + "_Transmit"
    dmaRxChannelID = "DMA_CH_FOR_" + pl360PlibId + "_Receive"

    if connectID == "drv_pl360_SPI_dependency" :
        print("drv_pl360_SPI_dependency")
        plibUsed = localComponent.getSymbolByID("DRV_PL360_PLIB")
        plibUsed.clearValue()
        plibUsed.setValue(pl360PlibId, 1)
        Database.setSymbolValue(pl360PlibId, "SPI_DRIVER_CONTROLLED", True, 1)

        # Set DMA in database
        Database.setSymbolValue("core", dmaRxRequestID, True, 2)
        Database.setSymbolValue("core", dmaTxRequestID, True, 2)

        # Get the allocated channel and assign it
        txChannel = Database.getSymbolValue("core", dmaTxChannelID)
        localComponent.setSymbolValue("DRV_PL360_TX_DMA_CHANNEL", txChannel, 2)
        rxChannel = Database.getSymbolValue("core", dmaRxChannelID)
        localComponent.setSymbolValue("DRV_PL360_RX_DMA_CHANNEL", rxChannel, 2)
    
        # Get the allocated channel and assign it
        pl360TXDMAChannel.setValue(txChannel, 2)
        pl360TXDMAChannel.setVisible(True)
        pl360RXDMAChannel.setValue(rxChannel, 2)
        pl360RXDMAChannel.setVisible(True)
        pl360DMAChannelComment.setVisible(True)

        # Disable SPI interrupts
        plibIntUsed = remoteComponent.getSymbolByID("SPI_INTERRUPT_MODE")
        plibIntUsed.clearValue()
        plibIntUsed.setValue(False, 1)

        # Set SPI baudrate
        plibBaudrate = remoteComponent.getSymbolByID("SPI_BAUD_RATE")
        plibBaudrate.clearValue()
        if (pl360Profile.getSelectedValue() == "2"): 
            plibBaudrate.setValue(12000000, 1)
            print("Set SPI baudrate: 12000000 - " + pl360Profile.getSelectedValue())
        else:
            plibBaudrate.setValue(8000000, 1)
            print("Set SPI baudrate: 8000000 - " + pl360Profile.getSelectedValue())

  
def onAttachmentDisconnected(source, target):
    global pl360TXDMAChannel
    global pl360RXDMAChannel
    global pl360DMAChannelComment

    print("onDependencyDisconnected event")

    localComponent = source["component"]
    remoteComponent = target["component"]
    pl360PlibId = remoteComponent.getID().upper()
    connectID = source["id"]

    dmaRxRequestID = "DMA_CH_NEEDED_FOR_" + pl360PlibId + "_Receive"
    dmaTxRequestID = "DMA_CH_NEEDED_FOR_" + pl360PlibId + "_Transmit"
    dmaTxChannelID = "DMA_CH_FOR_" + pl360PlibId + "_Transmit"
    dmaRxChannelID = "DMA_CH_FOR_" + pl360PlibId + "_Receive"

    if connectID == "drv_pl360_SPI_dependency":
        plibUsed = localComponent.getSymbolByID("DRV_PL360_PLIB")
        plibUsed.clearValue()
        Database.setSymbolValue(pl360PlibId, "SPI_DRIVER_CONTROLLED", False, 1)

        # Set DMA in database
        Database.setSymbolValue("core", dmaRxRequestID, False, 2)
        Database.setSymbolValue("core", dmaTxRequestID, False, 2)
        print("DBG -> dmaTxRequestID: " + dmaTxRequestID)
        print("DBG -> dmaRxRequestID: " + dmaRxRequestID)
   
        # Get the allocated channel and assign it
        pl360TXDMAChannel.setVisible(False)
        pl360RXDMAChannel.setVisible(False)
        pl360DMAChannelComment.setVisible(False)


def destroyComponent(spiComponent):
    spiPeripheral = Database.getSymbolValue("drv_pl360", "DRV_PL360_PLIB")
    dmaTxID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Transmit"
    dmaRxID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Receive"
    Database.setSymbolValue("core", dmaTxID, False, 2) 
    Database.setSymbolValue("core", dmaRxID, False, 2) 