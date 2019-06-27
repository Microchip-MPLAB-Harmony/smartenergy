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
    global g3MacRTSourceBinFileCENA
    global g3MacRTSourceBinFileCENB
    global g3MacRTSourceBinFileFCC
    global g3MacRTSourceBinFileARIB
    global binExternalAddressing
    global binAssemblyFile
    if event["value"] == True:
        print("externalAddressingTrigger disable BIN files")
        g3MacRTSourceBinFileCENA.setEnabled(False)
        g3MacRTSourceBinFileCENB.setEnabled(False)
        g3MacRTSourceBinFileFCC.setEnabled(False)
        g3MacRTSourceBinFileARIB.setEnabled(False)
        binAssemblyFile.setEnabled(False)
    else:
        binAssemblyFile.setEnabled(True)
        prof = symbol.getValue()
        if(prof == 0):
            print("externalAddressingTrigger restore G3 CENA")
            g3MacRTSourceBinFileCENA.setEnabled(True)
        elif(prof == 1):
            print("externalAddressingTrigger restore G3 CENB")
            g3MacRTSourceBinFileCENB.setEnabled(True)
        elif(prof == 2):
            print("externalAddressingTrigger restore G3 FCC")
            g3MacRTSourceBinFileFCC.setEnabled(True)
        elif(prof == 3):
            print("externalAddressingTrigger restore G3 ARIB")
            g3MacRTSourceBinFileARIB.setEnabled(True)
        elif(prof == 4):
            print("externalAddressingTrigger restore G3 CENA (multi)")
            g3MacRTSourceBinFileCENA.setEnabled(True)



def profileConfiguration(symbol, event):
    global g3MacRTSourceBinFileCENA
    global g3MacRTSourceBinFileCENB
    global g3MacRTSourceBinFileFCC
    global g3MacRTSourceBinFileARIB
    global binExternalAddressing
    global binAssemblyFile

    if (binExternalAddressing.getValue() == False):
        binAssemblyFile.setEnabled(True)
        if (event["symbol"].getKeyDescription(event["value"]) == "G3_CEN_A"):            
            g3MacRTSourceBinFileCENA.setEnabled(True)
            g3MacRTSourceBinFileCENB.setEnabled(False)
            g3MacRTSourceBinFileFCC.setEnabled(False)
            g3MacRTSourceBinFileARIB.setEnabled(False)
            print("profileConfiguration update G3 CEN A files")
        elif (event["symbol"].getKeyDescription(event["value"]) == "G3_CEN_B"):           
            g3MacRTSourceBinFileCENA.setEnabled(False)
            g3MacRTSourceBinFileCENB.setEnabled(True)
            g3MacRTSourceBinFileFCC.setEnabled(False)
            g3MacRTSourceBinFileARIB.setEnabled(False)
            print("profileConfiguration update G3 CEN B files")
        elif (event["symbol"].getKeyDescription(event["value"]) == "G3_FCC"):           
            g3MacRTSourceBinFileCENA.setEnabled(False)
            g3MacRTSourceBinFileCENB.setEnabled(False)
            g3MacRTSourceBinFileFCC.setEnabled(True)
            g3MacRTSourceBinFileARIB.setEnabled(False)
            print("profileConfiguration update G3 FCC files")
        elif (event["symbol"].getKeyDescription(event["value"]) == "G3_ARIB"):           
            g3MacRTSourceBinFileCENA.setEnabled(False)
            g3MacRTSourceBinFileCENB.setEnabled(False)
            g3MacRTSourceBinFileFCC.setEnabled(False)
            g3MacRTSourceBinFileARIB.setEnabled(True)
            print("profileConfiguration update G3 FCC files")
        else:           
            g3MacRTSourceBinFileCENA.setEnabled(True)
            g3MacRTSourceBinFileCENB.setEnabled(False)
            g3MacRTSourceBinFileFCC.setEnabled(True)
            g3MacRTSourceBinFileARIB.setEnabled(False)
            print("profileConfiguration update G3 CEN A nd FCC files")
    else:
        print("profileConfiguration update no bin files (G3)")
        g3MacRTSourceBinFileCENA.setEnabled(False)
        g3MacRTSourceBinFileCENB.setEnabled(False)
        g3MacRTSourceBinFileFCC.setEnabled(False)
        g3MacRTSourceBinFileARIB.setEnabled(False)
        binAssemblyFile.setEnabled(False)

def externalInterruptTrigger(symbol, event):
    global g3MacRtExtIntSource

    key = event["symbol"].getKeyDescription(event["value"])
    intSrc = "PIO" + key[1] + "_IRQn"
    g3MacRtExtIntSource.setValue(intSrc, 1)

        
def enableEncriptScript(symbol, event):
    if (event["value"] == True):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False) 

def visibleEncriptComment(symbol, event):
    if (event["value"] == True):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)        
		
def instantiateComponent(g3MacRtComponent):  

    res = Database.activateComponents(["HarmonyCore"])

    # Enable "Generate Harmony Driver Common Files" option in MHC
    # Database.setSymbolValue("HarmonyCore", "ENABLE_DRV_COMMON", True, 1)

    # Enable "Generate Harmony System Service Common Files" option in MHC
    # Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_COMMON", True, 1)
    
    g3MacRtSymNumInst = g3MacRtComponent.createIntegerSymbol("DRV_G3_MACRT_NUM_INSTANCES", None)
    g3MacRtSymNumInst.setLabel("Number of Instances")
    g3MacRtSymNumInst.setVisible(False)
    g3MacRtSymNumInst.setDefaultValue(1)

    g3MacRtPLIB = g3MacRtComponent.createStringSymbol("DRV_G3_MACRT_PLIB", None)
    g3MacRtPLIB.setLabel("PLIB Used")
    g3MacRtPLIB.setReadOnly(True)
    
    g3MacRtSymNumClients = g3MacRtComponent.createIntegerSymbol("DRV_G3_MACRT_NUM_CLIENTS", None)
    g3MacRtSymNumClients.setLabel("Number of Clients")
    g3MacRtSymNumClients.setReadOnly(True)
    g3MacRtSymNumClients.setDefaultValue(1)

    global g3MacRtExtIntPin
    g3MacRtExtIntPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_G3_MACRT_EXT_INT_PIN", None)
    g3MacRtExtIntPin.setLabel("External Interrupt Pin")
    g3MacRtExtIntPin.setDefaultValue(104) #PD28
    g3MacRtExtIntPin.setOutputMode("Key")
    g3MacRtExtIntPin.setDisplayMode("Description")

    global g3MacRtExtIntSource
    g3MacRtExtIntSource = g3MacRtComponent.createStringSymbol("DRV_G3_MACRT_EXT_INT_SRC", None)
    g3MacRtExtIntSource.setLabel("External Interrupt Source")
    g3MacRtExtIntSource.setDefaultValue("PIOD_IRQn") #PIOD_IRQn
    g3MacRtExtIntSource.setVisible(True)
    g3MacRtExtIntSource.setReadOnly(True)
    g3MacRtExtIntSource.setDependencies(externalInterruptTrigger, ["DRV_G3_MACRT_EXT_INT_PIN"])

    g3MacRtResetPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_G3_MACRT_RESET_PIN", None)
    g3MacRtResetPin.setLabel("Reset Pin")
    g3MacRtResetPin.setDefaultValue(34) #PB2
    g3MacRtResetPin.setOutputMode("Key")
    g3MacRtResetPin.setDisplayMode("Description")

    g3MacRtLDOEnPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_G3_MACRT_LDO_EN_PIN", None)
    g3MacRtLDOEnPin.setLabel("LDO Enable Pin")
    g3MacRtLDOEnPin.setDefaultValue(35) #PB3
    g3MacRtLDOEnPin.setOutputMode("Key")
    g3MacRtLDOEnPin.setDisplayMode("Description")    

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
            g3MacRtExtIntPin.addKey(key, value, description)
            g3MacRtResetPin.addKey(key, value, description)
            g3MacRtLDOEnPin.addKey(key, value, description)

    g3MacRtSymPinConfigComment = g3MacRtComponent.createCommentSymbol("DRV_G3_MACRT_PINS_CONFIG_COMMENT", None)
    g3MacRtSymPinConfigComment.setVisible(True)
    g3MacRtSymPinConfigComment.setLabel("***Above selected pins must be configured as GPIO Output in Pin Manager***")
    
    global g3MacRtTXDMAChannel
    g3MacRtTXDMAChannel = g3MacRtComponent.createIntegerSymbol("DRV_G3_MACRT_TX_DMA_CHANNEL", None)
    g3MacRtTXDMAChannel.setLabel("DMA Channel For Transmit")
    g3MacRtTXDMAChannel.setDefaultValue(0)
    g3MacRtTXDMAChannel.setVisible(False)
    g3MacRtTXDMAChannel.setReadOnly(True)

    global g3MacRtRXDMAChannel
    g3MacRtRXDMAChannel = g3MacRtComponent.createIntegerSymbol("DRV_G3_MACRT_RX_DMA_CHANNEL", None)
    g3MacRtRXDMAChannel.setLabel("DMA Channel For Receive")
    g3MacRtRXDMAChannel.setDefaultValue(1)
    g3MacRtRXDMAChannel.setVisible(False)
    g3MacRtRXDMAChannel.setReadOnly(True)

    global g3MacRtDMAChannelComment
    g3MacRtDMAChannelComment = g3MacRtComponent.createCommentSymbol("DRV_G3_MACRT_DMA_CH_COMMENT", None)
    g3MacRtDMAChannelComment.setLabel("***Couldn't Allocate DMA Channel for Transmit/Receive. Check DMA manager.***")
    g3MacRtDMAChannelComment.setVisible(False)

    global binExternalAddressing
    binExternalAddressing = g3MacRtComponent.createBooleanSymbol("DRV_G3_MACRT_EXTERNAL_ADDRESSING", None)
    binExternalAddressing.setLabel("External Binary Image Block Transfer")
    binExternalAddressing.setVisible(True)
    binExternalAddressing.setDefaultValue(False)

    global g3MacRtProfile
    g3MacRtProfile = g3MacRtComponent.createKeyValueSetSymbol("DRV_G3_MACRT_PLC_PROFILE", None)
    g3MacRtProfile.setLabel("Select PLC Profile")
    g3MacRtProfile.addKey("G3_CEN_A", "0", "G3_CEN_A")
    g3MacRtProfile.addKey("G3_CEN_B", "1", "G3_CEN_B")
    g3MacRtProfile.addKey("G3_FCC", "2", "G3_FCC")
    g3MacRtProfile.addKey("G3_ARIB", "3", "G3_ARIB")
    g3MacRtProfile.addKey("G3_MULTIBAND", "4", "G3_MULTIBAND")
    g3MacRtProfile.setDisplayMode("Description")
    g3MacRtProfile.setOutputMode("Value")
    g3MacRtProfile.setDefaultValue(0)
    g3MacRtProfile.setDependencies(externalAddressingTrigger, ["DRV_G3_MACRT_EXTERNAL_ADDRESSING"])

    g3MacRtSecureMode = g3MacRtComponent.createBooleanSymbol("DRV_G3_MACRT_SECURE_MODE", None)
    g3MacRtSecureMode.setLabel("PLC Secure Mode")
    g3MacRtSecureMode.setVisible(True)
    g3MacRtSecureMode.setDefaultValue(False)

    g3MacRtSecureComment = g3MacRtComponent.createCommentSymbol("DRV_G3_MACRT_SECURE_COMMENT", None)
    g3MacRtSecureComment.setLabel("***Run encription script to get a secure binary file and replace the current non-secure file. Check bin folder.***")
    g3MacRtSecureComment.setVisible(False)
    g3MacRtSecureComment.setDependencies(visibleEncriptComment, ["DRV_G3_MACRT_SECURE_MODE"])

    g3MacRtSpecCompliance = g3MacRtComponent.createKeyValueSetSymbol("DRV_G3_MACRT_SPEC_COMPLIANCE", None)
    g3MacRtSpecCompliance.setLabel("G3 Spec. Compliance")
    g3MacRtSpecCompliance.addKey("G3_17", "0", "G3 Specification 17")
    g3MacRtSpecCompliance.addKey("G3_15", "1", "G3 Specification 15")
    g3MacRtSpecCompliance.setDisplayMode("Description")
    g3MacRtSpecCompliance.setOutputMode("Value")
    g3MacRtSpecCompliance.setDefaultValue(0)

    ############################################################################
    #### Code Generation ####
    ############################################################################

    configName = Variables.get("__CONFIGURATION_NAME")

    g3MacRtSecureScript = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SECURE_SCRIPT", None)
    g3MacRtSecureScript.setSourcePath("driver/g3MacRt/src/bin/plc_encfile.py")
    g3MacRtSecureScript.setOutputName("plc_encfile.py")
    g3MacRtSecureScript.setDestPath("driver/g3MacRt/bin/")
    g3MacRtSecureScript.setProjectPath("config/" + configName + "/driver/g3MacRt/bin/")
    g3MacRtSecureScript.setType("SOURCE")
    g3MacRtSecureScript.setEnabled(False)
    g3MacRtSecureScript.setDependencies(enableEncriptScript, ["DRV_G3_MACRT_SECURE_MODE"])

    global binAssemblyFile
    binAssemblyFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_ASSEMBLY_BIN", None)
    binAssemblyFile.setSourcePath("driver/g3MacRt/src/bin/drv_g3_macrt_bin.S.ftl")
    binAssemblyFile.setOutputName("drv_g3_macrt_bin.S")
    binAssemblyFile.setDestPath("driver/g3MacRt/bin/")
    binAssemblyFile.setProjectPath("config/" + configName + "/driver/g3MacRt/bin/")
    binAssemblyFile.setType("SOURCE")
    binAssemblyFile.setMarkup(True)

    g3MacRtAsmPathSetting = g3MacRtComponent.createSettingSymbol("DRV_G3_MACRT_ASM_PATH_SETTING", None)
    g3MacRtAsmPathSetting.setCategory("C32-AS")
    g3MacRtAsmPathSetting.setKey("extra-include-directories-for-assembler")
    g3MacRtAsmPathSetting.setValue("../src/config/" + configName + "/driver/g3MacRt/bin")
    g3MacRtAsmPathSetting.setAppend(True, ";")

    g3MacRtHalHeaderFile = g3MacRtComponent.createFileSymbol("PLC_HAL_HEADER", None)
    g3MacRtHalHeaderFile.setSourcePath("driver/g3MacRt/drv_plc_hal.h")
    g3MacRtHalHeaderFile.setOutputName("drv_plc_hal.h")
    g3MacRtHalHeaderFile.setDestPath("driver/g3MacRt/")
    g3MacRtHalHeaderFile.setProjectPath("config/" + configName + "/driver/g3MacRt/")
    g3MacRtHalHeaderFile.setType("HEADER")

    g3MacRtHeaderFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_HEADER", None)
    g3MacRtHeaderFile.setSourcePath("driver/g3MacRt/drv_g3_macrt.h")
    g3MacRtHeaderFile.setOutputName("drv_g3_macrt.h")
    g3MacRtHeaderFile.setDestPath("driver/g3MacRt/")
    g3MacRtHeaderFile.setProjectPath("config/" + configName + "/driver/g3MacRt/")
    g3MacRtHeaderFile.setType("HEADER")

    g3MacRtSymHeaderDefFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_DEF_HEADER", None)
    g3MacRtSymHeaderDefFile.setSourcePath("driver/g3MacRt/drv_g3_macrt_definitions.h")
    g3MacRtSymHeaderDefFile.setOutputName("drv_g3_macrt_definitions.h")
    g3MacRtSymHeaderDefFile.setDestPath("driver/g3MacRt")
    g3MacRtSymHeaderDefFile.setProjectPath("config/" + configName + "/driver/g3MacRt/")
    g3MacRtSymHeaderDefFile.setType("HEADER")

    g3MacRtProfileDefFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_PROFILE_DEF", None)
    g3MacRtProfileDefFile.setSourcePath("driver/g3MacRt/drv_g3_macrt_comm.h")
    g3MacRtProfileDefFile.setOutputName("drv_g3_macrt_comm.h")
    g3MacRtProfileDefFile.setDestPath("driver/g3MacRt")
    g3MacRtProfileDefFile.setProjectPath("config/" + configName + "/driver/g3MacRt/")
    g3MacRtProfileDefFile.setType("HEADER")

    g3MacRtProfileFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_PROFILE", None)
    g3MacRtProfileFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt_comm.c")
    g3MacRtProfileFile.setOutputName("drv_g3_macrt_comm.c")
    g3MacRtProfileFile.setDestPath("driver/g3MacRt")
    g3MacRtProfileFile.setProjectPath("config/" + configName + "/driver/g3MacRt/")
    g3MacRtProfileFile.setType("SOURCE")
    g3MacRtProfileFile.setDependencies(profileConfiguration, ["DRV_G3_MACRT_PLC_PROFILE"])

    g3MacRtSourceFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SOURCE", None)
    g3MacRtSourceFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt.c")
    g3MacRtSourceFile.setOutputName("drv_g3_macrt.c")
    g3MacRtSourceFile.setDestPath("driver/g3MacRt/")
    g3MacRtSourceFile.setProjectPath("config/" + configName + "/driver/g3MacRt/")
    g3MacRtSourceFile.setType("SOURCE")

    global g3MacRTSourceBinFileCENA
    g3MacRTSourceBinFileCENA = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SOURCE_BIN_G3_CENA", None)
    g3MacRTSourceBinFileCENA.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_CENA.bin")
    g3MacRTSourceBinFileCENA.setOutputName("G3_MAC_RT_CENA.bin")
    g3MacRTSourceBinFileCENA.setDestPath("driver/g3MacRt/bin/")
    g3MacRTSourceBinFileCENA.setProjectPath("config/" + configName + "/driver/g3MacRt/bin/")
    g3MacRTSourceBinFileCENA.setType("SOURCE")
    g3MacRTSourceBinFileCENA.setEnabled(True)
    g3MacRTSourceBinFileCENA.setVisible(False)

    global g3MacRTSourceBinFileCENB
    g3MacRTSourceBinFileCENB = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SOURCE_BIN_G3_CENB", None)
    g3MacRTSourceBinFileCENB.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_CENB.bin")
    g3MacRTSourceBinFileCENB.setOutputName("G3_MAC_RT_CENB.bin")
    g3MacRTSourceBinFileCENB.setDestPath("driver/g3MacRt/bin/")
    g3MacRTSourceBinFileCENB.setProjectPath("config/" + configName + "/driver/g3MacRt/bin/")
    g3MacRTSourceBinFileCENB.setType("SOURCE")
    g3MacRTSourceBinFileCENB.setEnabled(False)
    g3MacRTSourceBinFileCENB.setVisible(False)

    global g3MacRTSourceBinFileFCC
    g3MacRTSourceBinFileFCC = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SOURCE_BIN_G3_FCC", None)
    g3MacRTSourceBinFileFCC.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_FCC.bin")
    g3MacRTSourceBinFileFCC.setOutputName("G3_MAC_RT_FCC.bin")
    g3MacRTSourceBinFileFCC.setDestPath("driver/g3MacRt/bin/")
    g3MacRTSourceBinFileFCC.setProjectPath("config/" + configName + "/driver/g3MacRt/bin/")
    g3MacRTSourceBinFileFCC.setType("SOURCE")
    g3MacRTSourceBinFileFCC.setEnabled(False)
    g3MacRTSourceBinFileFCC.setVisible(False)

    global g3MacRTSourceBinFileARIB
    g3MacRTSourceBinFileARIB = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SOURCE_BIN_G3_ARIB", None)
    g3MacRTSourceBinFileARIB.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_ARIB.bin")
    g3MacRTSourceBinFileARIB.setOutputName("G3_MAC_RT_ARIB.bin")
    g3MacRTSourceBinFileARIB.setDestPath("driver/g3MacRt/bin/")
    g3MacRTSourceBinFileARIB.setProjectPath("config/" + configName + "/driver/g3MacRt/bin/")
    g3MacRTSourceBinFileARIB.setType("SOURCE")
    g3MacRTSourceBinFileARIB.setEnabled(False)
    g3MacRTSourceBinFileARIB.setVisible(False)

    g3MacRtHalFile = g3MacRtComponent.createFileSymbol("PL360_HAL", None)
    g3MacRtHalFile.setSourcePath("driver/g3MacRt/src/drv_plc_hal.c")
    g3MacRtHalFile.setOutputName("drv_plc_hal.c")
    g3MacRtHalFile.setDestPath("driver/g3MacRt/")
    g3MacRtHalFile.setProjectPath("config/" + configName + "/driver/g3MacRt/")
    g3MacRtHalFile.setType("SOURCE")

    g3MacRtBootFile = g3MacRtComponent.createFileSymbol("PLC_BOOT", None)
    g3MacRtBootFile.setSourcePath("driver/g3MacRt/src/drv_plc_boot.c")
    g3MacRtBootFile.setOutputName("drv_plc_boot.c")
    g3MacRtBootFile.setDestPath("driver/g3MacRt/")
    g3MacRtBootFile.setProjectPath("config/" + configName + "/driver/g3MacRt/")
    g3MacRtBootFile.setType("SOURCE")

    g3MacRtBootHeaderFile = g3MacRtComponent.createFileSymbol("PL360_BOOT_HEADER", None)
    g3MacRtBootHeaderFile.setSourcePath("driver/g3MacRt/src/drv_plc_boot.h")
    g3MacRtBootHeaderFile.setOutputName("drv_plc_boot.h")
    g3MacRtBootHeaderFile.setDestPath("driver/g3MacRt")
    g3MacRtBootHeaderFile.setProjectPath("config/" + configName + "/driver/g3MacRt/")
    g3MacRtBootHeaderFile.setType("SOURCE")

    g3MacRtHeaderLocalFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_HEADER_LOCAL", None)
    g3MacRtHeaderLocalFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt_local.h")
    g3MacRtHeaderLocalFile.setOutputName("drv_g3_macrt_local.h")
    g3MacRtHeaderLocalFile.setDestPath("driver/g3MacRt")
    g3MacRtHeaderLocalFile.setProjectPath("config/" + configName + "/driver/g3MacRt/")
    g3MacRtHeaderLocalFile.setType("SOURCE")
    g3MacRtHeaderLocalFile.setEnabled(True)

    g3MacRtProfileHeaderLocalFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_PROFILE_LOCAL", None)
    g3MacRtProfileHeaderLocalFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt_local_comm.h")
    g3MacRtProfileHeaderLocalFile.setOutputName("drv_g3_macrt_local_comm.h")
    g3MacRtProfileHeaderLocalFile.setDestPath("driver/g3MacRt")
    g3MacRtProfileHeaderLocalFile.setProjectPath("config/" + configName + "/driver/g3MacRt/")
    g3MacRtProfileHeaderLocalFile.setType("SOURCE")
    g3MacRtProfileHeaderLocalFile.setEnabled(True)

    g3MacRtSystemDefFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_DEF", None)
    g3MacRtSystemDefFile.setType("STRING")
    g3MacRtSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    g3MacRtSystemDefFile.setSourcePath("driver/g3MacRt/templates/system/definitions.h.ftl")
    g3MacRtSystemDefFile.setMarkup(True)

    g3MacRtSymSystemDefObjFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SYSTEM_DEF_OBJECT", None)
    g3MacRtSymSystemDefObjFile.setType("STRING")
    g3MacRtSymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    g3MacRtSymSystemDefObjFile.setSourcePath("driver/g3MacRt/templates/system/definitions_objects.h.ftl")
    g3MacRtSymSystemDefObjFile.setMarkup(True)

    g3MacRtSymSystemConfigFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_CONFIGURATION", None)
    g3MacRtSymSystemConfigFile.setType("STRING")
    g3MacRtSymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    g3MacRtSymSystemConfigFile.setSourcePath("driver/g3MacRt/templates/system/configuration.h.ftl")
    g3MacRtSymSystemConfigFile.setMarkup(True)

    g3MacRtSymSystemInitDataFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_INIT_DATA", None)
    g3MacRtSymSystemInitDataFile.setType("STRING")
    g3MacRtSymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    g3MacRtSymSystemInitDataFile.setSourcePath("driver/g3MacRt/templates/system/initialize_data.c.ftl")
    g3MacRtSymSystemInitDataFile.setMarkup(True)

    g3MacRtSystemInitFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_INIT", None)
    g3MacRtSystemInitFile.setType("STRING")
    g3MacRtSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    g3MacRtSystemInitFile.setSourcePath("driver/g3MacRt/templates/system/initialize.c.ftl")
    g3MacRtSystemInitFile.setMarkup(True)

    g3MacRtSystemTasksFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SYS_TASK", None)
    g3MacRtSystemTasksFile.setType("STRING")
    g3MacRtSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    g3MacRtSystemTasksFile.setSourcePath("driver/g3MacRt/templates/system/system_tasks.c.ftl")
    g3MacRtSystemTasksFile.setMarkup(True)

################################################################################
#### Business Logic ####
################################################################################  
def onAttachmentConnected(source, target):
    global g3MacRtTXDMAChannel
    global g3MacRtRXDMAChannel
    global g3MacRtDMAChannelComment
    global g3MacRtProfile

    print("onAttachmentConnected event")

    localComponent = source["component"]
    remoteComponent = target["component"]
    g3MacRtPlibId = remoteComponent.getID().upper()
    connectID = source["id"]

    dmaRxRequestID = "DMA_CH_NEEDED_FOR_" + g3MacRtPlibId + "_Receive"
    dmaTxRequestID = "DMA_CH_NEEDED_FOR_" + g3MacRtPlibId + "_Transmit"
    dmaTxChannelID = "DMA_CH_FOR_" + g3MacRtPlibId + "_Transmit"
    dmaRxChannelID = "DMA_CH_FOR_" + g3MacRtPlibId + "_Receive"

    if connectID == "drv_g3_macrt_SPI_dependency" :
        print("drv_g3_macrt_SPI_dependency")
        plibUsed = localComponent.getSymbolByID("DRV_G3_MACRT_PLIB")
        plibUsed.clearValue()
        plibUsed.setValue(g3MacRtPlibId, 1)
        Database.setSymbolValue(g3MacRtPlibId, "SPI_DRIVER_CONTROLLED", True, 1)

        # Set DMA in database
        Database.setSymbolValue("core", dmaRxRequestID, True, 2)
        Database.setSymbolValue("core", dmaTxRequestID, True, 2)

        # Get the allocated channel and assign it
        txChannel = Database.getSymbolValue("core", dmaTxChannelID)
        localComponent.setSymbolValue("DRV_G3_MACRT_TX_DMA_CHANNEL", txChannel, 2)
        rxChannel = Database.getSymbolValue("core", dmaRxChannelID)
        localComponent.setSymbolValue("DRV_G3_MACRT_RX_DMA_CHANNEL", rxChannel, 2)
    
        # Get the allocated channel and assign it
        g3MacRtTXDMAChannel.setValue(txChannel, 2)
        g3MacRtTXDMAChannel.setVisible(True)
        g3MacRtRXDMAChannel.setValue(rxChannel, 2)
        g3MacRtRXDMAChannel.setVisible(True)
        g3MacRtDMAChannelComment.setVisible(True)

        # Disable SPI interrupts
        plibIntUsed = remoteComponent.getSymbolByID("SPI_INTERRUPT_MODE")
        plibIntUsed.clearValue()
        plibIntUsed.setValue(False, 1)

        # Set SPI baudrate
        plibBaudrate = remoteComponent.getSymbolByID("SPI_BAUD_RATE")
        plibBaudrate.clearValue()
        if (g3MacRtProfile.getSelectedValue() == "2"): 
            plibBaudrate.setValue(12000000, 1)
            print("Set SPI baudrate: 12000000 - " + g3MacRtProfile.getSelectedValue())
        else:
            plibBaudrate.setValue(8000000, 1)
            print("Set SPI baudrate: 8000000 - " + g3MacRtProfile.getSelectedValue())

  
def onAttachmentDisconnected(source, target):
    global g3MacRtTXDMAChannel
    global g3MacRtRXDMAChannel
    global g3MacRtDMAChannelComment

    print("onDependencyDisconnected event")

    localComponent = source["component"]
    remoteComponent = target["component"]
    g3MacRtPlibId = remoteComponent.getID().upper()
    connectID = source["id"]

    dmaRxRequestID = "DMA_CH_NEEDED_FOR_" + g3MacRtPlibId + "_Receive"
    dmaTxRequestID = "DMA_CH_NEEDED_FOR_" + g3MacRtPlibId + "_Transmit"
    dmaTxChannelID = "DMA_CH_FOR_" + g3MacRtPlibId + "_Transmit"
    dmaRxChannelID = "DMA_CH_FOR_" + g3MacRtPlibId + "_Receive"

    if connectID == "drv_g3_macrt_SPI_dependency":
        plibUsed = localComponent.getSymbolByID("DRV_G3_MACRT_PLIB")
        plibUsed.clearValue()
        Database.setSymbolValue(g3MacRtPlibId, "SPI_DRIVER_CONTROLLED", False, 1)

        # Set DMA in database
        Database.setSymbolValue("core", dmaRxRequestID, False, 2)
        Database.setSymbolValue("core", dmaTxRequestID, False, 2)
        print("DBG -> dmaTxRequestID: " + dmaTxRequestID)
        print("DBG -> dmaRxRequestID: " + dmaRxRequestID)
   
        # Get the allocated channel and assign it
        g3MacRtTXDMAChannel.setVisible(False)
        g3MacRtRXDMAChannel.setVisible(False)
        g3MacRtDMAChannelComment.setVisible(False)


def destroyComponent(spiComponent):
    spiPeripheral = Database.getSymbolValue("drv_g3_macrt", "DRV_G3_MACRT_PLIB")
    dmaTxID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Transmit"
    dmaRxID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Receive"
    Database.setSymbolValue("core", dmaTxID, False, 2) 
    Database.setSymbolValue("core", dmaRxID, False, 2) 