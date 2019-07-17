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

def externalInterruptSource(symbol, event):
    key = event["symbol"].getKeyDescription(event["value"])
    intSrc = "PIO" + key[1] + "_IRQn"
    symbol.setValue(intSrc)

def externalInterruptPort(symbol, event):
    key = event["symbol"].getKeyDescription(event["value"])
    intPort = "PIO_PORT_" + key[1]
    symbol.setValue(intPort)

        
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
    global drvG3MacRtInstanceSpace
    global isDMAPresent

    drvG3MacRtInstanceSpace = "drvG3MacRt"

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
        print("isDMAPresent = False")
    else:
        isDMAPresent = True
        print("isDMAPresent = False")

        # Enable "Enable System DMA" option in MHC
        if (Database.getSymbolValue("HarmonyCore", "ENABLE_SYS_DMA") == False):
            Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_DMA", True)
    
    g3MacRtSymNumInst = g3MacRtComponent.createIntegerSymbol("DRV_G3_MACRT_NUM_INSTANCES", None)
    g3MacRtSymNumInst.setLabel("Number of Instances")
    g3MacRtSymNumInst.setVisible(False)
    g3MacRtSymNumInst.setDefaultValue(1)

    g3MacRtPLIB = g3MacRtComponent.createStringSymbol("DRV_PLC_PLIB", None)
    g3MacRtPLIB.setLabel("PLIB Used")
    g3MacRtPLIB.setReadOnly(True)
    
    g3MacRtSymNumClients = g3MacRtComponent.createIntegerSymbol("DRV_G3_MACRT_NUM_CLIENTS", None)
    g3MacRtSymNumClients.setLabel("Number of Clients")
    g3MacRtSymNumClients.setReadOnly(True)
    g3MacRtSymNumClients.setDefaultValue(1)

    g3MacRtExtIntPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_EXT_INT_PIN", None)
    g3MacRtExtIntPin.setLabel("External Interrupt Pin")
    g3MacRtExtIntPin.setOutputMode("Key")
    g3MacRtExtIntPin.setDisplayMode("Description")

    g3MacRtExtIntSource = g3MacRtComponent.createStringSymbol("DRV_PLC_EXT_INT_SRC", None)
    g3MacRtExtIntSource.setLabel("External Interrupt Source")
    g3MacRtExtIntSource.setDefaultValue("PIOA_IRQn")
    g3MacRtExtIntSource.setVisible(True)
    g3MacRtExtIntSource.setReadOnly(True)
    g3MacRtExtIntSource.setDependencies(externalInterruptSource, ["DRV_PLC_EXT_INT_PIN"])

    g3MacRtExtIntPioPort = g3MacRtComponent.createStringSymbol("DRV_PLC_EXT_INT_PIO_PORT", None)
    g3MacRtExtIntPioPort.setLabel("External Interrupt Port")
    g3MacRtExtIntPioPort.setDefaultValue("PIO_PORT_A")
    g3MacRtExtIntPioPort.setVisible(True)
    g3MacRtExtIntPioPort.setReadOnly(True)
    g3MacRtExtIntPioPort.setDependencies(externalInterruptPort, ["DRV_PLC_EXT_INT_PIN"])

    g3MacRtResetPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_RESET_PIN", None)
    g3MacRtResetPin.setLabel("Reset Pin")
    g3MacRtResetPin.setOutputMode("Key")
    g3MacRtResetPin.setDisplayMode("Description")

    g3MacRtLDOEnPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_LDO_EN_PIN", None)
    g3MacRtLDOEnPin.setLabel("LDO Enable Pin")
    g3MacRtLDOEnPin.setOutputMode("Key")
    g3MacRtLDOEnPin.setDisplayMode("Description")   

    g3MacRtCDPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_CD_PIN", None)
    g3MacRtCDPin.setLabel("Carrier Detect Pin")
    g3MacRtCDPin.setOutputMode("Key")
    g3MacRtCDPin.setDisplayMode("Description")   

    availablePinDictionary = {}

    # Send message to core to get available pins
    availablePinDictionary = Database.sendMessage("core", "PIN_LIST", availablePinDictionary)

    for pad in sort_alphanumeric(availablePinDictionary.values()):
        key = "SYS_PORT_PIN_" + pad
        value = list(availablePinDictionary.keys())[list(availablePinDictionary.values()).index(pad)]
        description = pad
        g3MacRtExtIntPin.addKey(key, value, description)
        g3MacRtResetPin.addKey(key, value, description)
        g3MacRtLDOEnPin.addKey(key, value, description)
        g3MacRtCDPin.addKey(key, value, description)

    g3MacRtSymPinConfigComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_PINS_CONFIG_COMMENT", None)
    g3MacRtSymPinConfigComment.setVisible(True)
    g3MacRtSymPinConfigComment.setLabel("***Above selected pins must be configured as GPIO Output in Pin Manager***")

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
    #g3MacRtProfile.addKey("G3_ARIB", "3", "G3_ARIB")
    g3MacRtProfile.addKey("G3_MULTIBAND", "4", "G3_MULTIBAND")
    g3MacRtProfile.setDisplayMode("Description")
    g3MacRtProfile.setOutputMode("Value")
    g3MacRtProfile.setDefaultValue(0)
    g3MacRtProfile.setDependencies(externalAddressingTrigger, ["DRV_G3_MACRT_EXTERNAL_ADDRESSING"])

    g3MacRtSecureMode = g3MacRtComponent.createBooleanSymbol("DRV_PLC_SECURE_MODE", None)
    g3MacRtSecureMode.setLabel("PLC Secure Mode")
    g3MacRtSecureMode.setVisible(True)
    g3MacRtSecureMode.setDefaultValue(False)

    g3MacRtSecureComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_SECURE_COMMENT", None)
    g3MacRtSecureComment.setLabel("***Run encription script to get a secure binary file and replace the current non-secure file. Check bin folder.***")
    g3MacRtSecureComment.setVisible(False)
    g3MacRtSecureComment.setDependencies(visibleEncriptComment, ["DRV_PLC_SECURE_MODE"])

    g3MacRtSpecCompliance = g3MacRtComponent.createKeyValueSetSymbol("DRV_G3_MACRT_SPEC_COMPLIANCE", None)
    g3MacRtSpecCompliance.setLabel("G3 Spec. Compliance")
    g3MacRtSpecCompliance.addKey("G3_17", "0", "G3 Specification 17")
    g3MacRtSpecCompliance.addKey("G3_15", "1", "G3 Specification 15")
    g3MacRtSpecCompliance.setDisplayMode("Description")
    g3MacRtSpecCompliance.setOutputMode("Value")
    g3MacRtSpecCompliance.setDefaultValue(0)
    
    global g3MacRtTXRXDMA
    g3MacRtTXRXDMA = g3MacRtComponent.createBooleanSymbol("DRV_PLC_TX_RX_DMA", None)
    g3MacRtTXRXDMA.setLabel("Use DMA for Transmit and Receive?")
    g3MacRtTXRXDMA.setVisible(isDMAPresent)
    g3MacRtTXRXDMA.setReadOnly(True)

    global g3MacRtTXDMAChannel
    g3MacRtTXDMAChannel = g3MacRtComponent.createIntegerSymbol("DRV_PLC_TX_DMA_CHANNEL", None)
    g3MacRtTXDMAChannel.setLabel("DMA Channel For Transmit")
    g3MacRtTXDMAChannel.setDefaultValue(0)
    g3MacRtTXDMAChannel.setVisible(False)
    g3MacRtTXDMAChannel.setReadOnly(True)
    g3MacRtTXDMAChannel.setDependencies(requestAndAssignTxDMAChannel, ["DRV_PLC_TX_RX_DMA"])

    global g3MacRtTXDMAChannelComment
    g3MacRtTXDMAChannelComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_TX_DMA_CH_COMMENT", None)
    g3MacRtTXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Transmit. Check DMA Manager. !!!")
    g3MacRtTXDMAChannelComment.setVisible(False)
    g3MacRtTXDMAChannelComment.setDependencies(requestDMAComment, ["DRV_PLC_TX_DMA_CHANNEL"])

    global g3MacRtRXDMAChannel
    g3MacRtRXDMAChannel = g3MacRtComponent.createIntegerSymbol("DRV_PLC_RX_DMA_CHANNEL", None)
    g3MacRtRXDMAChannel.setLabel("DMA Channel For Receive")
    g3MacRtRXDMAChannel.setDefaultValue(1)
    g3MacRtRXDMAChannel.setVisible(False)
    g3MacRtRXDMAChannel.setReadOnly(True)
    g3MacRtRXDMAChannel.setDependencies(requestAndAssignRxDMAChannel, ["DRV_PLC_TX_RX_DMA"])

    global g3MacRtRXDMAChannelComment
    g3MacRtRXDMAChannelComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_RX_DMA_CH_COMMENT", None)
    g3MacRtRXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Receive. Check DMA Manager. !!!")
    g3MacRtRXDMAChannelComment.setVisible(False)
    g3MacRtRXDMAChannelComment.setDependencies(requestDMAComment, ["DRV_PLC_RX_DMA_CHANNEL"])

    g3MacRtDependencyDMAComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_DEPENDENCY_DMA_COMMENT", None)
    g3MacRtDependencyDMAComment.setLabel("!!! Satisfy PLIB Dependency to Allocate DMA Channel !!!")
    g3MacRtDependencyDMAComment.setVisible(isDMAPresent)

    g3MacRtKeyCortex = g3MacRtComponent.createHexSymbol("DRV_PLC_CORE_KEY", None)
    g3MacRtKeyCortex.setLabel("G3 MAC RT Key Cortex")
    g3MacRtKeyCortex.setDefaultValue(0x5A5A)
    g3MacRtKeyCortex.setVisible(False)
    g3MacRtKeyCortex.setReadOnly(True)

    ############################################################################
    #### Code Generation ####
    ############################################################################

    configName = Variables.get("__CONFIGURATION_NAME")

    g3MacRtHalHeaderFile = g3MacRtComponent.createFileSymbol("PLC_HAL_HEADER", None)
    g3MacRtHalHeaderFile.setSourcePath("driver/common/plcHal/drv_plc_hal.h.ftl")
    g3MacRtHalHeaderFile.setOutputName("drv_plc_hal.h")
    g3MacRtHalHeaderFile.setDestPath("driver/plc/common/")
    g3MacRtHalHeaderFile.setProjectPath("config/" + configName + "/driver/plc/common/")
    g3MacRtHalHeaderFile.setType("HEADER")
    g3MacRtHalHeaderFile.setMarkup(True)

    g3MacRtHalFile = g3MacRtComponent.createFileSymbol("PLC_HAL", None)
    g3MacRtHalFile.setSourcePath("driver/common/plcHal/drv_plc_hal.c.ftl")
    g3MacRtHalFile.setOutputName("drv_plc_hal.c")
    g3MacRtHalFile.setDestPath("driver/plc/common/")
    g3MacRtHalFile.setProjectPath("config/" + configName + "/driver/plc/common/")
    g3MacRtHalFile.setType("SOURCE")
    g3MacRtHalFile.setMarkup(True)

    g3MacRtHALInitDataFile = g3MacRtComponent.createFileSymbol("DRV_HAL_INIT_DATA", None)
    g3MacRtHALInitDataFile.setType("STRING")
    g3MacRtHALInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    g3MacRtHALInitDataFile.setSourcePath("driver/common/plcHal/drv_plc_hal_initialization_data.c.ftl")
    g3MacRtHALInitDataFile.setMarkup(True)

    g3MacRtBootFile = g3MacRtComponent.createFileSymbol("PLC_BOOT", None)
    g3MacRtBootFile.setSourcePath("driver/common/plcBoot/drv_plc_boot.c")
    g3MacRtBootFile.setOutputName("drv_plc_boot.c")
    g3MacRtBootFile.setDestPath("driver/plc/common/")
    g3MacRtBootFile.setProjectPath("config/" + configName + "/driver/plc/common/")
    g3MacRtBootFile.setType("SOURCE")

    g3MacRtBootHeaderFile = g3MacRtComponent.createFileSymbol("PLC_BOOT_HEADER", None)
    g3MacRtBootHeaderFile.setSourcePath("driver/common/plcBoot/drv_plc_boot.h")
    g3MacRtBootHeaderFile.setOutputName("drv_plc_boot.h")
    g3MacRtBootHeaderFile.setDestPath("driver/plc/common")
    g3MacRtBootHeaderFile.setProjectPath("config/" + configName + "/driver/plc/common/")
    g3MacRtBootHeaderFile.setType("HEADER")

    g3MacRtSecureScript = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SECURE_SCRIPT", None)
    g3MacRtSecureScript.setSourcePath("driver/g3MacRt/src/bin/plc_encfile.py")
    g3MacRtSecureScript.setOutputName("plc_encfile.py")
    g3MacRtSecureScript.setDestPath("driver/plc/g3MacRt/bin/")
    g3MacRtSecureScript.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin/")
    g3MacRtSecureScript.setType("SOURCE")
    g3MacRtSecureScript.setEnabled(False)
    g3MacRtSecureScript.setDependencies(enableEncriptScript, ["DRV_G3_MACRT_SECURE_MODE"])

    global binAssemblyFile
    binAssemblyFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_ASSEMBLY_BIN", None)
    binAssemblyFile.setSourcePath("driver/g3MacRt/src/bin/drv_g3_macrt_bin.S.ftl")
    binAssemblyFile.setOutputName("drv_g3_macrt_bin.S")
    binAssemblyFile.setDestPath("driver/plc/g3MacRt/bin/")
    binAssemblyFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin/")
    binAssemblyFile.setType("SOURCE")
    binAssemblyFile.setMarkup(True)

    g3MacRtAsmPathSetting = g3MacRtComponent.createSettingSymbol("DRV_G3_MACRT_ASM_PATH_SETTING", None)
    g3MacRtAsmPathSetting.setCategory("C32-AS")
    g3MacRtAsmPathSetting.setKey("extra-include-directories-for-assembler")
    g3MacRtAsmPathSetting.setValue("../src/config/" + configName + "/driver/plc/g3MacRt/bin")
    g3MacRtAsmPathSetting.setAppend(True, ";")

    g3MacRtHeaderFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_HEADER", None)
    g3MacRtHeaderFile.setSourcePath("driver/g3MacRt/drv_g3_macrt.h")
    g3MacRtHeaderFile.setOutputName("drv_g3_macrt.h")
    g3MacRtHeaderFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtHeaderFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtHeaderFile.setType("HEADER")

    g3MacRtSymHeaderDefFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_DEF_HEADER", None)
    g3MacRtSymHeaderDefFile.setSourcePath("driver/g3MacRt/drv_g3_macrt_definitions.h")
    g3MacRtSymHeaderDefFile.setOutputName("drv_g3_macrt_definitions.h")
    g3MacRtSymHeaderDefFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtSymHeaderDefFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtSymHeaderDefFile.setType("HEADER")

    g3MacRtProfileDefFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_PROFILE_DEF", None)
    g3MacRtProfileDefFile.setSourcePath("driver/g3MacRt/drv_g3_macrt_comm.h")
    g3MacRtProfileDefFile.setOutputName("drv_g3_macrt_comm.h")
    g3MacRtProfileDefFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtProfileDefFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtProfileDefFile.setType("HEADER")

    g3MacRtProfileFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_PROFILE", None)
    g3MacRtProfileFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt_comm.c")
    g3MacRtProfileFile.setOutputName("drv_g3_macrt_comm.c")
    g3MacRtProfileFile.setDestPath("driver/plc/g3MacRt/src")
    g3MacRtProfileFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/src")
    g3MacRtProfileFile.setType("SOURCE")
    g3MacRtProfileFile.setDependencies(profileConfiguration, ["DRV_G3_MACRT_PLC_PROFILE"])

    g3MacRtSourceFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SOURCE", None)
    g3MacRtSourceFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt.c")
    g3MacRtSourceFile.setOutputName("drv_g3_macrt.c")
    g3MacRtSourceFile.setDestPath("driver/plc/g3MacRt/src")
    g3MacRtSourceFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/src")
    g3MacRtSourceFile.setType("SOURCE")

    global g3MacRTSourceBinFileCENA
    g3MacRTSourceBinFileCENA = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SOURCE_BIN_G3_CENA", None)
    g3MacRTSourceBinFileCENA.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_CENA.bin")
    g3MacRTSourceBinFileCENA.setOutputName("G3_MAC_RT_CENA.bin")
    g3MacRTSourceBinFileCENA.setDestPath("driver/plc/g3MacRt/bin/")
    g3MacRTSourceBinFileCENA.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin/")
    g3MacRTSourceBinFileCENA.setType("SOURCE")
    g3MacRTSourceBinFileCENA.setEnabled(True)
    g3MacRTSourceBinFileCENA.setVisible(False)

    global g3MacRTSourceBinFileCENB
    g3MacRTSourceBinFileCENB = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SOURCE_BIN_G3_CENB", None)
    g3MacRTSourceBinFileCENB.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_CENB.bin")
    g3MacRTSourceBinFileCENB.setOutputName("G3_MAC_RT_CENB.bin")
    g3MacRTSourceBinFileCENB.setDestPath("driver/plc/g3MacRt/bin/")
    g3MacRTSourceBinFileCENB.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin/")
    g3MacRTSourceBinFileCENB.setType("SOURCE")
    g3MacRTSourceBinFileCENB.setEnabled(False)
    g3MacRTSourceBinFileCENB.setVisible(False)

    global g3MacRTSourceBinFileFCC
    g3MacRTSourceBinFileFCC = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SOURCE_BIN_G3_FCC", None)
    g3MacRTSourceBinFileFCC.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_FCC.bin")
    g3MacRTSourceBinFileFCC.setOutputName("G3_MAC_RT_FCC.bin")
    g3MacRTSourceBinFileFCC.setDestPath("driver/plc/g3MacRt/bin/")
    g3MacRTSourceBinFileFCC.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin/")
    g3MacRTSourceBinFileFCC.setType("SOURCE")
    g3MacRTSourceBinFileFCC.setEnabled(False)
    g3MacRTSourceBinFileFCC.setVisible(False)

    global g3MacRTSourceBinFileARIB
    g3MacRTSourceBinFileARIB = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_SOURCE_BIN_G3_ARIB", None)
    g3MacRTSourceBinFileARIB.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_ARIB.bin")
    g3MacRTSourceBinFileARIB.setOutputName("G3_MAC_RT_ARIB.bin")
    g3MacRTSourceBinFileARIB.setDestPath("driver/plc/g3MacRt/bin/")
    g3MacRTSourceBinFileARIB.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin/")
    g3MacRTSourceBinFileARIB.setType("SOURCE")
    g3MacRTSourceBinFileARIB.setEnabled(False)
    g3MacRTSourceBinFileARIB.setVisible(False)

    g3MacRtHeaderLocalFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_HEADER_LOCAL", None)
    g3MacRtHeaderLocalFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt_local.h")
    g3MacRtHeaderLocalFile.setOutputName("drv_g3_macrt_local.h")
    g3MacRtHeaderLocalFile.setDestPath("driver/plc/g3MacRt/src")
    g3MacRtHeaderLocalFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/src")
    g3MacRtHeaderLocalFile.setType("SOURCE")
    g3MacRtHeaderLocalFile.setEnabled(True)

    g3MacRtProfileHeaderLocalFile = g3MacRtComponent.createFileSymbol("DRV_G3_MACRT_PROFILE_LOCAL", None)
    g3MacRtProfileHeaderLocalFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt_local_comm.h")
    g3MacRtProfileHeaderLocalFile.setOutputName("drv_g3_macrt_local_comm.h")
    g3MacRtProfileHeaderLocalFile.setDestPath("driver/plc/g3MacRt/src")
    g3MacRtProfileHeaderLocalFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/src")
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
    global isDMAPresent

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    targetID = target["id"]

    if connectID == "drv_g3_macrt_SPI_dependency":
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

    if connectID == "drv_g3_macrt_SPI_dependency":

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
    global drvG3MacRtInstanceSpace
    global g3MacRtTXDMAChannelComment

    spiPeripheral = Database.getSymbolValue(drvG3MacRtInstanceSpace, "DRV_PLC_PLIB")

    dmaChannelID = "DMA_CH_FOR_" + str(spiPeripheral) + "_Transmit"
    dmaRequestID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Transmit"

    if event["value"] == False:
        Database.setSymbolValue("core", dmaRequestID, False)
        g3MacRtXDMAChannelComment.setVisible(False)
        symbol.setVisible(False)
    else:
        symbol.setVisible(True)
        Database.setSymbolValue("core", dmaRequestID, True)

    # Get the allocated channel and assign it
    channel = Database.getSymbolValue("core", dmaChannelID)
    symbol.setValue(channel)

def requestAndAssignRxDMAChannel(symbol, event):
    global drvG3MacRtInstanceSpace
    global g3MacRtRXDMAChannelComment

    spiPeripheral = Database.getSymbolValue(drvG3MacRtInstanceSpace, "DRV_PLC_PLIB")

    dmaChannelID = "DMA_CH_FOR_" + str(spiPeripheral) + "_Receive"
    dmaRequestID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Receive"

    if event["value"] == False:
        Database.setSymbolValue("core", dmaRequestID, False)
        g3MacRtRXDMAChannelComment.setVisible(False)
        symbol.setVisible(False)
    else:
        symbol.setVisible(True)
        Database.setSymbolValue("core", dmaRequestID, True)

    # Get the allocated channel and assign it
    channel = Database.getSymbolValue("core", dmaChannelID)
    symbol.setValue(channel)

def requestDMAComment(symbol, event):
    global g3MacRtTXRXDMA

    if(event["value"] == -2) and (g3MacRtTXRXDMA.getValue() == True):
        symbol.setVisible(True)
        event["symbol"].setVisible(False)
    else:
        symbol.setVisible(False)

def destroyComponent(spiComponent):
    global drvG3MacRtInstanceSpace

    if isDMAPresent:
        spiPeripheral = Database.getSymbolValue(drvG3MacRtInstanceSpace, "DRV_PLC_PLIB")

        dmaTxID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Transmit"
        dmaRxID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Receive"

        Database.setSymbolValue("core", dmaTxID, False)
        Database.setSymbolValue("core", dmaRxID, False)