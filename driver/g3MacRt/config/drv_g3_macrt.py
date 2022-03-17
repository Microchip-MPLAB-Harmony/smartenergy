# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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

global g3MacRtBinFileCENA
global g3MacRtBinFileCENB
global g3MacRtBinFileFCC
global g3MacRtBinFileARIB
global g3MacRtAsmBinFile

global plcBandInUse

gPlcBand = ""

PLC_PROFILE_G3_CEN_A = 1
PLC_PROFILE_G3_FCC = 2
PLC_PROFILE_G3_ARIB = 3
PLC_PROFILE_G3_CEN_B = 4
PLC_PROFILE_G3_FCC_CEN_A = 5
PLC_PROFILE_G3_FCC_CEN_B = 6
PLC_PROFILE_G3_ARIB_CEN_A = 7
PLC_PROFILE_G3_ARIB_CEN_B = 8

def handleMessage(messageID, args):

    result_dict = {}

    if (messageID == "REQUEST_CONFIG_PARAMS"):
        if args.get("localComponentID") != None:
            result_dict = Database.sendMessage(args["localComponentID"], "SPI_MASTER_MODE", {"isReadOnly":True, "isEnabled":True})
            result_dict = Database.sendMessage(args["localComponentID"], "SPI_MASTER_INTERRUPT_MODE", {"isReadOnly":True, "isEnabled":False})

    return result_dict

def sort_alphanumeric(l):
    import re
    convert = lambda text: int(text) if text.isdigit() else text.lower()
    alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ]
    return sorted(l, key = alphanum_key)

def g3MacRtExternalInterruptTrigger(symbol, event):
    key = event["symbol"].getKeyDescription(event["value"])
    intSrc = "PIO" + key[1] + "_IRQn"
    symbol.setValue(intSrc, 1)

def g3MacRtExternalInterruptPort(symbol, event):
    key = event["symbol"].getKeyDescription(event["value"])
    intPort = "PIO_PORT_" + key[1]
    symbol.setValue(intPort)

def g3MacRtEnableEncScript(symbol, event):
    if (event["value"] == True):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False) 

def g3MacRtVisibleEncComment(symbol, event):
    if (event["value"] == True):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)         

def setPlcBandInUse(plcBand):
    dict = {}

    if (plcBand == "CEN-A"):
        plcBandInUse.setValue(PLC_PROFILE_G3_CEN_A)
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENA", {})
    elif (plcBand == "CEN-B"):
        plcBandInUse.setValue(PLC_PROFILE_G3_CEN_B)
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENB", {})
    elif (plcBand == "FCC"):
        plcBandInUse.setValue(PLC_PROFILE_G3_FCC)
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_FCC", {})
    elif (plcBand == "ARIB"):
        plcBandInUse.setValue(PLC_PROFILE_G3_ARIB)
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_ARIB", {})

def setPlcMultiBandInUse(g3_band, g3_aux_band):
    dict = {}

    if (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_G3_BAND_AUX_ACTIVE") == True) :
        g3_aux_band_default = True
    else:
        g3_aux_band_default = False

    if (g3_band == "FCC"):
        if (g3_aux_band == "CEN-A"):
            plcBandInUse.setValue(PLC_PROFILE_G3_FCC_CEN_A)
            if (g3_aux_band_default == True):
                dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENA", {})
        elif (g3_aux_band == "CEN-B"):
            plcBandInUse.setValue(PLC_PROFILE_G3_FCC_CEN_B)
            if (g3_aux_band_default == True):
                dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENB", {})
    else:
        if (g3_aux_band == "CEN-A"):
            plcBandInUse.setValue(PLC_PROFILE_G3_ARIB_CEN_A)
            if (g3_aux_band_default == True):
                dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENA", {})
        elif (g3_aux_band == "CEN-B"):
            plcBandInUse.setValue(PLC_PROFILE_G3_ARIB_CEN_B)
            if (g3_aux_band_default == True):
                dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENB", {})

def removeAllBinFiles():
    print("[CHRIS_dbg] : removeAllBinFiles")
    g3MacRtBinFileCENA.setEnabled(False)
    g3MacRtBinFileCENB.setEnabled(False)
    g3MacRtBinFileFCC.setEnabled(False)
    g3MacRtBinFileARIB.setEnabled(False)

def includeBinFile(plcBand):
    if (plcBand == "CEN-A"):          
        g3MacRtBinFileCENA.setEnabled(True)
        print("[CHRIS_dbg] : includeBinFile CEN-A")
    elif (plcBand == "CEN-B"):            
        g3MacRtBinFileCENB.setEnabled(True)
        print("[CHRIS_dbg] : includeBinFile CEN-B")
    elif (plcBand == "FCC"):            
        g3MacRtBinFileFCC.setEnabled(True)
        print("[CHRIS_dbg] : includeBinFile FCC")
    elif (plcBand == "ARIB"):           
        g3MacRtBinFileARIB.setEnabled(True) 
        print("[CHRIS_dbg] : includeBinFile ARIB")

def updateBinFiles():
    dict = {}
    removeAllBinFiles()
    g3_band = Database.getSymbolValue("drvG3MacRt", "DRV_PLC_G3_BAND")
    includeBinFile(g3_band)
    setPlcBandInUse(g3_band)
    if (g3_band == "FCC" or g3_band =="ARIB") and (Database.getSymbolValue("drvG3MacRt", "DRV_PLC_COUP_G3_MULTIBAND") == True) :
        g3_aux_band = Database.getSymbolValue("drvG3MacRt", "DRV_PLC_G3_BAND_AUX")
        includeBinFile(g3_aux_band)
        setPlcMultiBandInUse(g3_band, g3_aux_band)

    # Update Coupling Parameters    
    dict = Database.sendMessage("srv_pcoup", "SRV_PCOUP_UPDATE_G3_PARAMETERS", {})
    
    # Check Internal/External Addressing    
    if (Database.getSymbolValue("drvG3MacRt", "DRV_PLC_BIN_STATIC_ADDRESSING") == False) :
        # Internal Addressing
        g3MacRtAsmBinFile.setEnabled(True)
    else:
        # External Addressing
        removeAllBinFiles()
        g3MacRtAsmBinFile.setEnabled(False)

def updateG3PLCBandInUse(symbol, event):
    # print("[CHRIS_dbg] : updateG3PLCBandInUse <- " + event["id"])
    updateBinFiles()

def plcBinAddressingMode(symbol, event):
    # print("[CHRIS_dbg] : plcBinAddressingMode <- " + event["id"])
    symbol.setVisible(event["value"])
    updateBinFiles()

def showPL460Pins(symbol, event):
    # print("[CHRIS_dbg] : showPL460Pins <- " + event["id"])
    if (event["value"] == "PL460"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)  

def showSleepPin(symbol, event):
    # print("[CHRIS_dbg] : showSleepPin <- " + event["id"])
    symbol.setVisible(event["value"])

def showThermalMonitorPin(symbol, event):
    # print("[CHRIS_dbg] : showThermalMonitorPin <- " + event["id"])
    symbol.setVisible(event["value"])

def showG3HighAttenuation(symbol, event):
    global plcDriverMode
    global plcG3Band

    if (plcDriverMode.getValue() == "PL360"):
        symbol.setVisible(False)
    else:
        if (plcG3Band.getValue() == "FCC") or (plcG3Band.getValue() == "ARIB"):
            symbol.setVisible(True)
        else:
            symbol.setVisible(False)

def showG3InternalDriver(symbol, event):
    global plcDriverMode

    # print("[CHRIS_dbg] : showG3InternalDriver <- " + event["id"])
    if (event["symbol"].getValue() == "CEN-B") and plcDriverMode.getValue() == "PL360":
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)
        # symbol.setReadOnly(True)
        # symbol.setValue(False)
        # symbol.setReadOnly(False)

def showG3Multiband(symbol, event):
    # print("[CHRIS_dbg] : showG3Multiband <- " + event["id"])
    if (event["symbol"].getValue() == "FCC") or (event["symbol"].getValue() == "ARIB"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)
        # symbol.setReadOnly(True)
        # symbol.setValue(False)
        # symbol.setReadOnly(False)

def showG3AuxBand(symbol, event):
    # print("[CHRIS_dbg] : showG3AuxBand <- " + event["id"])
    if (event["value"] == True):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)
    
def resetPlcBand(symbol, event):  
    symbol.setReadOnly(True)
    symbol.setValue("CEN-A")
    symbol.setReadOnly(False)

def enablePL460Capabilities(symbol, event):
    if (event["value"] == "PL460"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def instantiateComponent(g3MacRtComponent):  
    global isDMAPresent

    res = Database.activateComponents(["HarmonyCore"])

    # Enable "Generate Harmony Driver Common Files" option in MHC
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":True})

    # Enable "Generate Harmony System Service Common Files" option in MHC
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":True})

    # Enable "Generate Harmony System Port Files" option in MHC
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_PORTS", {"isEnabled":True})

    # Enable "Generate Harmony System DMA Files" option in MHC
    if Database.getSymbolValue("core", "DMA_ENABLE") != None:
        Database.sendMessage("HarmonyCore", "ENABLE_SYS_DMA", {"isEnabled":True})

    if Database.getSymbolValue("core", "DMA_ENABLE") == None:
        isDMAPresent = False
        # print("[CHRIS_dbg] : DMA is NOT present")
    else:
        isDMAPresent = True
        # print("[CHRIS_dbg] : DMA is present")

    global plcDriverMode
    plcDriverMode = g3MacRtComponent.createComboSymbol("DRV_PLC_MODE", None, ["PL360", "PL460"])
    plcDriverMode.setLabel("PLC Driver Mode")
    plcDriverMode.setDefaultValue("PL460")

    plcPLIB = g3MacRtComponent.createStringSymbol("DRV_PLC_PLIB", None)
    plcPLIB.setLabel("PLIB Used")
    plcPLIB.setReadOnly(True)

    plcPLIBCSRIndex = g3MacRtComponent.createIntegerSymbol("DRV_PLC_PLIB_CSR_INDEX", None)
    plcPLIBCSRIndex.setLabel("PLIB CSR Index")
    plcPLIBCSRIndex.setReadOnly(True)

    plcExtIntPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_EXT_INT_PIN", None)
    plcExtIntPin.setLabel("External Interrupt Pin")
    plcExtIntPin.setDefaultValue(0)
    plcExtIntPin.setOutputMode("Key")
    plcExtIntPin.setDisplayMode("Description")

    plcExtIntSource = g3MacRtComponent.createStringSymbol("DRV_PLC_EXT_INT_SRC", None)
    plcExtIntSource.setLabel("External Interrupt Source")
    plcExtIntSource.setDefaultValue("PIOA_IRQn")
    plcExtIntSource.setVisible(True)
    plcExtIntSource.setReadOnly(True)
    plcExtIntSource.setDependencies(g3MacRtExternalInterruptTrigger, ["DRV_PLC_EXT_INT_PIN"])

    plcExtIntPioPort = g3MacRtComponent.createStringSymbol("DRV_PLC_EXT_INT_PIO_PORT", None)
    plcExtIntPioPort.setLabel("External Interrupt Port")
    plcExtIntPioPort.setDefaultValue("PIO_PORT_A")
    plcExtIntPioPort.setVisible(True)
    plcExtIntPioPort.setReadOnly(True)
    plcExtIntPioPort.setDependencies(g3MacRtExternalInterruptPort, ["DRV_PLC_EXT_INT_PIN"])

    plcResetPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_RESET_PIN", None)
    plcResetPin.setLabel("Reset Pin")
    plcResetPin.setDefaultValue(0)
    plcResetPin.setOutputMode("Key")
    plcResetPin.setDisplayMode("Description")

    plcLDOEnPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_LDO_EN_PIN", None)
    plcLDOEnPin.setLabel("LDO Enable Pin")
    plcLDOEnPin.setDefaultValue(0)
    plcLDOEnPin.setOutputMode("Key")
    plcLDOEnPin.setDisplayMode("Description")

    plcTxEnablePin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_TX_ENABLE_PIN", None)
    plcTxEnablePin.setLabel("TX Enable Pin")
    plcTxEnablePin.setDefaultValue(0)
    plcTxEnablePin.setOutputMode("Key")
    plcTxEnablePin.setDisplayMode("Description")
    plcTxEnablePin.setDependencies(enablePL460Capabilities, ["DRV_PLC_MODE"]);

    plcSleepMode = g3MacRtComponent.createBooleanSymbol("DRV_PLC_SLEEP_MODE", None)
    plcSleepMode.setLabel("Sleep Mode")
    plcSleepMode.setDefaultValue(False)
    plcSleepMode.setVisible(True)

    plcStbyPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_STBY_PIN", plcSleepMode)
    plcStbyPin.setLabel("Stand By Pin")
    plcStbyPin.setDefaultValue(0)
    plcStbyPin.setOutputMode("Key")
    plcStbyPin.setDisplayMode("Description")
    plcStbyPin.setVisible(False)
    plcStbyPin.setDependencies(showSleepPin, ["DRV_PLC_SLEEP_MODE"])
    
    plcThermalMonitor = g3MacRtComponent.createBooleanSymbol("DRV_PLC_THERMAL_MONITOR", None)
    plcThermalMonitor.setLabel("Thermal Monitor")
    plcThermalMonitor.setDefaultValue(False)
    plcThermalMonitor.setDependencies(enablePL460Capabilities, ["DRV_PLC_MODE"]);

    plcThMonPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_THMON_PIN", plcThermalMonitor)
    plcThMonPin.setLabel("Thermal Monitor Pin")
    plcThMonPin.setDefaultValue(0)
    plcThMonPin.setOutputMode("Key")
    plcThMonPin.setDisplayMode("Description")
    plcThMonPin.setVisible(False)
    plcThMonPin.setDependencies(showThermalMonitorPin, ["DRV_PLC_THERMAL_MONITOR"])

    availablePinDictionary = {}

    # Send message to core to get available pins
    availablePinDictionary = Database.sendMessage("core", "PIN_LIST", availablePinDictionary)

    for pad in sort_alphanumeric(availablePinDictionary.values()):
        key = "SYS_PORT_PIN_" + pad
        value = list(availablePinDictionary.keys())[list(availablePinDictionary.values()).index(pad)]
        description = pad
        plcExtIntPin.addKey(key, value, description)
        plcResetPin.addKey(key, value, description)
        plcLDOEnPin.addKey(key, value, description)
        plcTxEnablePin.addKey(key, value, description)
        plcStbyPin.addKey(key, value, description)
        plcThMonPin.addKey(key, value, description)

    plcSymPinConfigComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_PINS_CONFIG_COMMENT", None)
    plcSymPinConfigComment.setVisible(True)
    plcSymPinConfigComment.setLabel("***Above selected pins must be properly configured by Pin Manager***")
    
    ##### Do not modify below symbol names as they are used by Memory Driver #####

    global plcTXRXDMA
    plcTXRXDMA = g3MacRtComponent.createBooleanSymbol("DRV_PLC_TX_RX_DMA", None)
    plcTXRXDMA.setLabel("Use DMA for Transmit and Receive?")
    plcTXRXDMA.setDefaultValue(0)
    plcTXRXDMA.setVisible(False)
    plcTXRXDMA.setReadOnly(True)

    global plcTXDMAChannel
    plcTXDMAChannel = g3MacRtComponent.createIntegerSymbol("DRV_PLC_TX_DMA_CHANNEL", None)
    plcTXDMAChannel.setLabel("DMA Channel For Transmit")
    plcTXDMAChannel.setDefaultValue(0)
    plcTXDMAChannel.setVisible(False)
    plcTXDMAChannel.setReadOnly(True)
    plcTXDMAChannel.setDependencies(requestAndAssignTxDMAChannel, ["DRV_PLC_TX_RX_DMA"])

    global plcTXDMAChannelComment
    plcTXDMAChannelComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_TX_DMA_CH_COMMENT", None)
    plcTXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Transmit. Check DMA manager.")
    plcTXDMAChannelComment.setVisible(False)
    plcTXDMAChannelComment.setDependencies(requestDMAComment, ["DRV_PLC_TX_DMA_CHANNEL"])

    global plcRXDMAChannel
    plcRXDMAChannel = g3MacRtComponent.createIntegerSymbol("DRV_PLC_RX_DMA_CHANNEL", None)
    plcRXDMAChannel.setLabel("DMA Channel For Receive")
    plcRXDMAChannel.setDefaultValue(1)
    plcRXDMAChannel.setVisible(False)
    plcRXDMAChannel.setReadOnly(True)
    plcRXDMAChannel.setDependencies(requestAndAssignRxDMAChannel, ["DRV_PLC_TX_RX_DMA"])

    global plcRXDMAChannelComment
    plcRXDMAChannelComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_RX_DMA_CH_COMMENT", None)
    plcRXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Receive. Check DMA manager.")
    plcRXDMAChannelComment.setVisible(False)
    plcRXDMAChannelComment.setDependencies(requestDMAComment, ["DRV_PLC_RX_DMA_CHANNEL"])

    plcDependencyDMAComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_DEPENDENCY_DMA_COMMENT", None)
    plcDependencyDMAComment.setLabel("!!! Satisfy PLIB Dependency to Allocate DMA Channel !!!")
    plcDependencyDMAComment.setVisible(isDMAPresent)

    plcStaticAddressing = g3MacRtComponent.createBooleanSymbol("DRV_PLC_BIN_STATIC_ADDRESSING", None)
    plcStaticAddressing.setLabel("Static Bin file Addressing")
    plcStaticAddressing.setVisible(False)
    plcStaticAddressing.setDefaultValue(False)

    plcBinaryAddress = g3MacRtComponent.createHexSymbol("DRV_PLC_PLC_BIN_ADDRESS", plcStaticAddressing)
    plcBinaryAddress.setLabel("PLC Bin Address")
    plcBinaryAddress.setVisible(False)
    plcBinaryAddress.setDefaultValue(0x004A0000)
    plcBinaryAddress.setDependencies(plcBinAddressingMode, ["DRV_PLC_BIN_STATIC_ADDRESSING"])

    plcBinarySize = g3MacRtComponent.createHexSymbol("DRV_PLC_PLC_BIN_SIZE", plcStaticAddressing)
    plcBinarySize.setLabel("PLC Bin Size (bytes)")
    plcBinarySize.setVisible(False)
    plcBinarySize.setDefaultValue(0x10000)
    plcBinarySize.setDependencies(plcBinAddressingMode, ["DRV_PLC_BIN_STATIC_ADDRESSING"])

    plcSecureMode = g3MacRtComponent.createBooleanSymbol("DRV_PLC_SECURE_MODE", None)
    plcSecureMode.setLabel("PLC Secure Mode")
    plcSecureMode.setVisible(False)
    plcSecureMode.setDefaultValue(False)

    ############################################################################
    #### Code Generation ####
    ############################################################################

    configName = Variables.get("__CONFIGURATION_NAME")

    #### Common Files ##########################################################

    g3MacRtHalHeaderFile = g3MacRtComponent.createFileSymbol("PLC_HAL_HEADER", None)
    g3MacRtHalHeaderFile.setSourcePath("driver/common/plcHal/drv_plc_hal.h.ftl")
    g3MacRtHalHeaderFile.setOutputName("drv_plc_hal.h")
    g3MacRtHalHeaderFile.setDestPath("driver/plc/common")
    g3MacRtHalHeaderFile.setProjectPath("config/" + configName + "/driver/plc/common")
    g3MacRtHalHeaderFile.setType("HEADER")
    g3MacRtHalHeaderFile.setMarkup(True)

    g3MacRtHalFile = g3MacRtComponent.createFileSymbol("PLC_HAL", None)
    g3MacRtHalFile.setSourcePath("driver/common/plcHal/drv_plc_hal.c.ftl")
    g3MacRtHalFile.setOutputName("drv_plc_hal.c")
    g3MacRtHalFile.setDestPath("driver/plc/common")
    g3MacRtHalFile.setProjectPath("config/" + configName + "/driver/plc/common")
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
    g3MacRtBootFile.setDestPath("driver/plc/common")
    g3MacRtBootFile.setProjectPath("config/" + configName + "/driver/plc/common")
    g3MacRtBootFile.setType("SOURCE")

    g3MacRtBootHeaderFile = g3MacRtComponent.createFileSymbol("PLC_BOOT_HEADER", None)
    g3MacRtBootHeaderFile.setSourcePath("driver/common/plcBoot/drv_plc_boot.h")
    g3MacRtBootHeaderFile.setOutputName("drv_plc_boot.h")
    g3MacRtBootHeaderFile.setDestPath("driver/plc/common")
    g3MacRtBootHeaderFile.setProjectPath("config/" + configName + "/driver/plc/common")
    g3MacRtBootHeaderFile.setType("HEADER")

    #### ASM Path Settings #####################################################

    plcAsmPathSetting = g3MacRtComponent.createSettingSymbol("DRV_PLC_ASM_PATH_SETTING", None)
    plcAsmPathSetting.setCategory("C32-AS")
    plcAsmPathSetting.setKey("extra-include-directories-for-assembler")
    plcAsmPathSetting.setValue("../src/config/" + configName + "/driver/plc/g3MacRt/bin")
    plcAsmPathSetting.setAppend(False, None)

    #### Miscellanea ###########################################################

    g3MacRtSecureScript = g3MacRtComponent.createFileSymbol("DRV_PLC_PHY_SECURE_SCRIPT", None)
    g3MacRtSecureScript.setSourcePath("driver/g3MacRt/src/bin/plc_encfile.py")
    g3MacRtSecureScript.setOutputName("plc_encfile.py")
    g3MacRtSecureScript.setDestPath("driver/plc/g3MacRt/bin/")
    g3MacRtSecureScript.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin/")
    g3MacRtSecureScript.setType("SOURCE")
    g3MacRtSecureScript.setEnabled(False)
    g3MacRtSecureScript.setDependencies(g3MacRtEnableEncScript, ["DRV_PLC_SECURE_MODE"])

    g3MacRtKeyCortex = g3MacRtComponent.createHexSymbol("DRV_PLC_CORE_KEY", None)
    g3MacRtKeyCortex.setLabel("PLC Mac Rt Key Cortex")
    g3MacRtKeyCortex.setDefaultValue(0x5A5A)
    g3MacRtKeyCortex.setVisible(False)
    g3MacRtKeyCortex.setReadOnly(True)

    #### Source and header PLC PHY Files #######################################

    g3MacRtHdrFile = g3MacRtComponent.createFileSymbol("MACRT_HEADER", None)
    g3MacRtHdrFile.setSourcePath("driver/g3MacRt/drv_g3_macrt.h.ftl")
    g3MacRtHdrFile.setOutputName("drv_g3_macrt.h")
    g3MacRtHdrFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtHdrFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtHdrFile.setType("HEADER")
    g3MacRtHdrFile.setMarkup(True)

    g3MacRtHdrCommFile = g3MacRtComponent.createFileSymbol("MACRT_HEADER_COMM", None)
    g3MacRtHdrCommFile.setSourcePath("driver/g3MacRt/drv_g3_macrt_comm.h")
    g3MacRtHdrCommFile.setOutputName("drv_g3_macrt_comm.h")
    g3MacRtHdrCommFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtHdrCommFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtHdrCommFile.setType("HEADER")

    g3MacRtHdrDefFile = g3MacRtComponent.createFileSymbol("MACRT_HEADER_DEF", None)
    g3MacRtHdrDefFile.setSourcePath("driver/g3MacRt/drv_g3_macrt_definitions.h")
    g3MacRtHdrDefFile.setOutputName("drv_g3_macrt_definitions.h")
    g3MacRtHdrDefFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtHdrDefFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtHdrDefFile.setType("HEADER")

    g3MacRtSrcFile = g3MacRtComponent.createFileSymbol("MACRT_SOURCE", None)
    g3MacRtSrcFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt.c.ftl")
    g3MacRtSrcFile.setOutputName("drv_g3_macrt.c")
    g3MacRtSrcFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtSrcFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtSrcFile.setType("SOURCE")
    g3MacRtSrcFile.setMarkup(True)

    g3MacRtSrcCommFile = g3MacRtComponent.createFileSymbol("MACRT_SOURCE_COMM", None)
    g3MacRtSrcCommFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt_local_comm.c")
    g3MacRtSrcCommFile.setOutputName("drv_g3_macrt_local_comm.c")
    g3MacRtSrcCommFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtSrcCommFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtSrcCommFile.setType("SOURCE")

    g3MacRtSrcLocalFile = g3MacRtComponent.createFileSymbol("MACRT_SOURCE_LOCAL", None)
    g3MacRtSrcLocalFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt_local.h.ftl")
    g3MacRtSrcLocalFile.setOutputName("drv_g3_macrt_local.h")
    g3MacRtSrcLocalFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtSrcLocalFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtSrcLocalFile.setType("SOURCE")
    g3MacRtSrcLocalFile.setMarkup(True)

    g3MacRtSrcLocalCommFile = g3MacRtComponent.createFileSymbol("MACRT_SOURCE_LOCAL_COMM", None)
    g3MacRtSrcLocalCommFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt_local_comm.h")
    g3MacRtSrcLocalCommFile.setOutputName("drv_g3_macrt_local_comm.h")
    g3MacRtSrcLocalCommFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtSrcLocalCommFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtSrcLocalCommFile.setType("SOURCE")

    #### Binary PHY Files ######################################################

    global g3MacRtBinFileCENA
    g3MacRtBinFileCENA = g3MacRtComponent.createFileSymbol("MACRT_SOURCE_BIN_G3_CENA", None)
    g3MacRtBinFileCENA.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_CENA.bin")
    g3MacRtBinFileCENA.setOutputName("G3_MAC_RT_CENA.bin")
    g3MacRtBinFileCENA.setDestPath("driver/plc/g3MacRt/bin")
    g3MacRtBinFileCENA.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin")
    g3MacRtBinFileCENA.setType("SOURCE")
    g3MacRtBinFileCENA.setEnabled(True)
    g3MacRtBinFileCENA.setVisible(False)
    g3MacRtBinFileCENA.setMarkup(False)

    global g3MacRtBinFileCENB
    g3MacRtBinFileCENB = g3MacRtComponent.createFileSymbol("MACRT_SOURCE_BIN_G3_CENB", None)
    g3MacRtBinFileCENB.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_CENB.bin")
    g3MacRtBinFileCENB.setOutputName("G3_MAC_RT_CENB.bin")
    g3MacRtBinFileCENB.setDestPath("driver/plc/g3MacRt/bin")
    g3MacRtBinFileCENB.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin")
    g3MacRtBinFileCENB.setType("SOURCE")
    g3MacRtBinFileCENB.setEnabled(False)
    g3MacRtBinFileCENB.setVisible(False)
    g3MacRtBinFileCENB.setMarkup(False)

    global g3MacRtBinFileFCC
    g3MacRtBinFileFCC = g3MacRtComponent.createFileSymbol("MACRT_SOURCE_BIN_G3_FCC", None)
    g3MacRtBinFileFCC.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_FCC.bin")
    g3MacRtBinFileFCC.setOutputName("G3_MAC_RT_FCC.bin")
    g3MacRtBinFileFCC.setDestPath("driver/plc/g3MacRt/bin")
    g3MacRtBinFileFCC.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin")
    g3MacRtBinFileFCC.setType("SOURCE")
    g3MacRtBinFileFCC.setEnabled(False)
    g3MacRtBinFileFCC.setVisible(False)
    g3MacRtBinFileFCC.setMarkup(False)

    global g3MacRtBinFileARIB
    g3MacRtBinFileARIB = g3MacRtComponent.createFileSymbol("MACRT_SOURCE_BIN_G3_ARIB", None)
    g3MacRtBinFileARIB.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_ARIB.bin")
    g3MacRtBinFileARIB.setOutputName("G3_MAC_RT_ARIB.bin")
    g3MacRtBinFileARIB.setDestPath("driver/plc/g3MacRt/bin")
    g3MacRtBinFileARIB.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin")
    g3MacRtBinFileARIB.setType("SOURCE")
    g3MacRtBinFileARIB.setEnabled(False)
    g3MacRtBinFileARIB.setVisible(False)
    g3MacRtBinFileARIB.setMarkup(False)

    global g3MacRtAsmBinFile
    g3MacRtAsmBinFile = g3MacRtComponent.createFileSymbol("MACRT_ASM_BIN", None)
    g3MacRtAsmBinFile.setSourcePath("driver/g3MacRt/src/bin/drv_g3_macrt_bin.S.ftl")
    g3MacRtAsmBinFile.setOutputName("drv_g3_macrt_bin.S")
    g3MacRtAsmBinFile.setDestPath("driver/plc/g3MacRt/bin")
    g3MacRtAsmBinFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin")
    g3MacRtAsmBinFile.setType("SOURCE")
    g3MacRtAsmBinFile.setMarkup(True)
    
    ##### Coupling Settings : G3  ####################################################

    updBinFilesCtrl = g3MacRtComponent.createBooleanSymbol("DRV_PLC_UPD_BIN_FILES", None)
    updBinFilesCtrl.setLabel("updBinFilesCtrl")
    updBinFilesCtrl.setDescription("Auxiliary control")
    updBinFilesCtrl.setVisible(False)
    updBinFilesCtrl.setDefaultValue(False)

    global plcCoupG3Settings
    plcCoupG3Settings = g3MacRtComponent.createMenuSymbol("DRV_PLC_COUP_G3_SETTING", None)
    plcCoupG3Settings.setLabel("PLC Coupling Settings")
    plcCoupG3Settings.setDescription("Coupling Settings")
    plcCoupG3Settings.setVisible(True)

    global plcG3Band
    # plcG3Band = g3MacRtComponent.createComboSymbol("DRV_PLC_G3_BAND", plcCoupG3Settings, ["CEN-A", "CEN-B", "FCC", "ARIB"])
    plcG3Band = g3MacRtComponent.createComboSymbol("DRV_PLC_G3_BAND", plcCoupG3Settings, ["CEN-A", "CEN-B", "FCC"])
    plcG3Band.setLabel("Main Branch")
    plcG3Band.setDefaultValue("CEN-A")
    # plcG3Band.setDependencies(resetPlcBand, ["DRV_PLC_MODE"])

    plcCoupG3Internal = g3MacRtComponent.createBooleanSymbol("DRV_PLC_COUP_G3_INTERNAL", plcCoupG3Settings)
    plcCoupG3Internal.setLabel("Internal Driver")
    plcCoupG3Internal.setDescription("Internal Driver")
    plcCoupG3Internal.setVisible(False)
    plcCoupG3Internal.setDefaultValue(False)
    plcCoupG3Internal.setDependencies(showG3InternalDriver, ["DRV_PLC_G3_BAND"])

    plcCoupGMultiBand = g3MacRtComponent.createBooleanSymbol("DRV_PLC_COUP_G3_MULTIBAND", plcCoupG3Settings)
    plcCoupGMultiBand.setLabel("Multiband")
    plcCoupGMultiBand.setDescription("Multiband")
    plcCoupGMultiBand.setVisible(False)
    plcCoupGMultiBand.setDefaultValue(False)
    plcCoupGMultiBand.setDependencies(showG3Multiband, ["DRV_PLC_G3_BAND"])

    plcG3BandAux = g3MacRtComponent.createComboSymbol("DRV_PLC_G3_BAND_AUX", plcCoupGMultiBand, ["CEN-A", "CEN-B"])
    plcG3BandAux.setLabel("Auxiliary Branch")
    plcG3BandAux.setDefaultValue("CEN-A")
    plcG3BandAux.setVisible(False)
    plcG3BandAux.setDependencies(showG3AuxBand, ["DRV_PLC_COUP_G3_MULTIBAND"])

    plcG3BandAuxActive = g3MacRtComponent.createBooleanSymbol("DRV_PLC_G3_BAND_AUX_ACTIVE", plcCoupGMultiBand)
    plcG3BandAuxActive.setLabel("Set as default branch")
    plcG3BandAuxActive.setDefaultValue(False)
    plcG3BandAuxActive.setVisible(False)
    plcG3BandAuxActive.setDependencies(showG3AuxBand, ["DRV_PLC_COUP_G3_MULTIBAND"])

    plcCoupG3HighAttenuation = g3MacRtComponent.createBooleanSymbol("DRV_PLC_COUP_G3_HIGH_ATTENUATION", plcG3Band)
    plcCoupG3HighAttenuation.setLabel("FCC high attenuation branch")
    plcCoupG3HighAttenuation.setDescription("FCC high attenuation")
    plcCoupG3HighAttenuation.setVisible(False)
    plcCoupG3HighAttenuation.setDefaultValue(False)
    plcCoupG3HighAttenuation.setDependencies(showG3HighAttenuation, ["DRV_PLC_G3_BAND", "DRV_PLC_MODE"])
    
    ##### Coupling Settings : Generic  ####################################################

    global plcBandInUse
    plcBandInUse = g3MacRtComponent.createIntegerSymbol("DRV_PLC_BAND_IN_USE", None)
    plcBandInUse.setLabel("PLC Band in use")
    plcBandInUse.setDefaultValue(PLC_PROFILE_G3_CEN_A)
    #plcBandInUse.setVisible(True)
    plcBandInUse.setReadOnly(True)
    plcBandInUse.setDependencies(updateG3PLCBandInUse, ["DRV_PLC_G3_BAND", "DRV_PLC_G3_BAND_AUX", "DRV_PLC_COUP_G3_MULTIBAND"])

    #### FreeMaker Files ######################################################

    plcSystemDefFile = g3MacRtComponent.createFileSymbol("PLC_DEF", None)
    plcSystemDefFile.setType("STRING")
    plcSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    plcSystemDefFile.setSourcePath("driver/g3MacRt/templates/system/definitions.h.ftl")
    plcSystemDefFile.setMarkup(True)

    plcSymSystemDefObjFile = g3MacRtComponent.createFileSymbol("DRV_PLC_SYSTEM_DEF_OBJECT", None)
    plcSymSystemDefObjFile.setType("STRING")
    plcSymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    plcSymSystemDefObjFile.setSourcePath("driver/g3MacRt/templates/system/definitions_objects.h.ftl")
    plcSymSystemDefObjFile.setMarkup(True)

    plcSymSystemConfigFile = g3MacRtComponent.createFileSymbol("DRV_PLC_CONFIGURATION", None)
    plcSymSystemConfigFile.setType("STRING")
    plcSymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    plcSymSystemConfigFile.setSourcePath("driver/g3MacRt/templates/system/configuration.h.ftl")
    plcSymSystemConfigFile.setMarkup(True)

    plcSymSystemInitDataFile = g3MacRtComponent.createFileSymbol("DRV_PLC_INIT_DATA", None)
    plcSymSystemInitDataFile.setType("STRING")
    plcSymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    plcSymSystemInitDataFile.setSourcePath("driver/g3MacRt/templates/system/initialize_data.c.ftl")
    plcSymSystemInitDataFile.setMarkup(True)

    plcSystemInitFile = g3MacRtComponent.createFileSymbol("PLC_INIT", None)
    plcSystemInitFile.setType("STRING")
    plcSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    plcSystemInitFile.setSourcePath("driver/g3MacRt/templates/system/initialize.c.ftl")
    plcSystemInitFile.setMarkup(True)

    plcSystemTasksFile = g3MacRtComponent.createFileSymbol("DRV_PLC_SYS_TASK", None)
    plcSystemTasksFile.setType("STRING")
    plcSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    plcSystemTasksFile.setSourcePath("driver/g3MacRt/templates/system/system_tasks.c.ftl")
    plcSystemTasksFile.setMarkup(True)

    updateBinFiles()

################################################################################
#### Business Logic ####
################################################################################  
def onAttachmentConnected(source, target):
    global isDMAPresent

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    # print("[CHIRS_dbg] : onAttachmentConnected event - remoteID: " + remoteID.upper())

    if connectID == "drv_g3_macrt_SPI_dependency" :
        # print("[CHIRS_dbg] : drv_g3_macrt_SPI_dependency")
        plibUsed = localComponent.getSymbolByID("DRV_PLC_PLIB")
        plibUsed.clearValue()
        plibUsed.setValue(remoteID.upper())

        # Set SPI baudrate
        if "FLEXCOM" in remoteID.upper():
            plibBaudrate = remoteComponent.getSymbolByID("FLEXCOM_SPI_BAUD_RATE")
        else:
            plibBaudrate = remoteComponent.getSymbolByID("SPI_BAUD_RATE")
        plibBaudrate.clearValue()
        plibBaudrate.setValue(8000000)
        # print("[CHIRS_dbg] : Set SPI baudrate: 8000000")

        if (isDMAPresent == True):

            dmaChannelSym = Database.getSymbolValue("core", "DMA_CH_FOR_" + remoteID.upper() + "_Transmit")
            dmaRequestSym = Database.getSymbolValue("core", "DMA_CH_NEEDED_FOR_" + remoteID.upper() + "_Transmit")

            # Do not change the order as DMA Channels needs to be allocated
            # after setting the plibUsed symbol
            # Both device and connected plib should support DMA
            if dmaChannelSym != None and dmaRequestSym != None:
                localComponent.getSymbolByID("DRV_PLC_DEPENDENCY_DMA_COMMENT").setVisible(False)
                localComponent.getSymbolByID("DRV_PLC_TX_RX_DMA").setValue(1)
            else:
                # print("[CHIRS_dbg] : DMA ERROR")
                localComponent.getSymbolByID("DRV_PLC_TX_DMA_CH_COMMENT").setVisible(True)
                localComponent.getSymbolByID("DRV_PLC_RX_DMA_CH_COMMENT").setVisible(True)
        else:
            if "FLEXCOM" in remoteID.upper():
                remoteSym = remoteComponent.getSymbolByID("SPI_INTERRUPT_MODE")
                remoteSym.clearValue()
                remoteSym.setValue(True)
                remoteSym.setReadOnly(True)
                remoteSym = remoteComponent.getSymbolByID("USE_SPI_DMA")
                remoteSym.clearValue()
                remoteSym.setValue(True)
                remoteSym.setReadOnly(True)

  
def onAttachmentDisconnected(source, target):
    global isDMAPresent

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    # print("[CHIRS_dbg] : onAttachmentDisconnected event - remoteID: " + remoteID.upper())

    if connectID == "drv_g3_macrt_SPI_dependency" :
        print("[CHIRS_dbg] : drv_g3_macrt_SPI_dependency")

        localComponent.getSymbolByID("DRV_PLC_PLIB").clearValue()

        dummyDict = {}
        dummyDict = Database.sendMessage(remoteID, "SPI_MASTER_MODE", {"isReadOnly":False})
        dummyDict = Database.sendMessage(remoteID, "SPI_MASTER_INTERRUPT_MODE", {"isReadOnly":False})

        if (isDMAPresent == True):

            dmaChannelSym = Database.getSymbolValue("core", "DMA_CH_FOR_" + remoteID.upper() + "_Transmit")
            dmaRequestSym = Database.getSymbolValue("core", "DMA_CH_NEEDED_FOR_" + remoteID.upper() + "_Transmit")

            # Do not change the order as DMA Channels needs to be allocated
            # after setting the plibUsed symbol
            # Both device and connected plib should support DMA
            if isDMAPresent == True and dmaChannelSym != None and dmaRequestSym != None:
                localComponent.getSymbolByID("DRV_PLC_TX_RX_DMA").setValue(0)
                localComponent.getSymbolByID("DRV_PLC_DEPENDENCY_DMA_COMMENT").setVisible(True)
            else:
                # print("[CHIRS_dbg] : DMA ERROR")
                localComponent.getSymbolByID("DRV_PLC_TX_DMA_CH_COMMENT").setVisible(True)
                localComponent.getSymbolByID("DRV_PLC_RX_DMA_CH_COMMENT").setVisible(True)
        else:
            if "FLEXCOM" in remoteID.upper():
                remoteSym = remoteComponent.getSymbolByID("SPI_INTERRUPT_MODE")
                remoteSym.clearValue()
                remoteSym.setReadOnly(False)
                remoteSym = remoteComponent.getSymbolByID("USE_SPI_DMA")
                remoteSym.clearValue()
                remoteSym.setReadOnly(False)

        

def requestAndAssignTxDMAChannel(symbol, event):
    global plcTXDMAChannelComment

    spiPeripheral = Database.getSymbolValue("drvG3MacRt", "DRV_PLC_PLIB")

    dmaChannelID = "DMA_CH_FOR_" + str(spiPeripheral) + "_Transmit"
    dmaRequestID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Transmit"

    # print("[CHRIS_dbg] : requestAndAssignTxDMAChannel - " + dmaChannelID)

    # Clear the DMA symbol. Done for backward compatibility.
    Database.clearSymbolValue("core", dmaRequestID)

    dummyDict = {}

    if event["value"] == False:
        dummyDict = Database.sendMessage("core", "DMA_CHANNEL_DISABLE", {"dma_channel":dmaRequestID})
        plcTXDMAChannelComment.setVisible(False)
        symbol.setVisible(False)
    else:
        symbol.setVisible(True)
        dummyDict = Database.sendMessage("core", "DMA_CHANNEL_ENABLE", {"dma_channel":dmaRequestID})

    # Get the allocated channel and assign it
    channel = Database.getSymbolValue("core", dmaChannelID)
    # print("[CHRIS_dbg] : requestAndAssignTxDMAChannel - channel " + str(channel))
    symbol.setValue(channel)

def requestAndAssignRxDMAChannel(symbol, event):
    global plcRXDMAChannelComment

    spiPeripheral = Database.getSymbolValue("drvG3MacRt", "DRV_PLC_PLIB")

    dmaChannelID = "DMA_CH_FOR_" + str(spiPeripheral) + "_Receive"
    dmaRequestID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Receive"

    # print("[CHRIS_dbg] : requestAndAssignRxDMAChannel - " + dmaChannelID)

    # Clear the DMA symbol. Done for backward compatibility.
    Database.clearSymbolValue("core", dmaRequestID)

    dummyDict = {}

    if event["value"] == False:
        dummyDict = Database.sendMessage("core", "DMA_CHANNEL_DISABLE", {"dma_channel":dmaRequestID})
        plcRXDMAChannelComment.setVisible(False)
        symbol.setVisible(False)
    else:
        symbol.setVisible(True)
        dummyDict = Database.sendMessage("core", "DMA_CHANNEL_ENABLE", {"dma_channel":dmaRequestID})

    # Get the allocated channel and assign it
    channel = Database.getSymbolValue("core", dmaChannelID)
    symbol.setValue(channel)

def requestDMAComment(symbol, event):
    global plcTXRXDMA

    if(event["value"] == -2) and (plcTXRXDMA.getValue() == True):
        symbol.setVisible(True)
        event["symbol"].setVisible(False)
    else:
        symbol.setVisible(False)        

def destroyComponent(g3MacRtComponent):
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_PORTS", {"isEnabled":False})

    if Database.getSymbolValue("core", "DMA_ENABLE") != None:
        Database.sendMessage("HarmonyCore", "ENABLE_SYS_DMA", {"isEnabled":False})
