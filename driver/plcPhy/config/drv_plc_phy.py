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

global plcSourceBinFileG3CENA
global plcSourceBinFileG3CENB
global plcSourceBinFileG3FCC
global plcSourceBinFilePRIME
global plcSourceBinFileARIB
global plcAssemblyBinFile

global plcBandInUse

gPlcBand = ""

PLC_PROFILE_G3_CEN_A = 1
PLC_PROFILE_G3_FCC = 2
PLC_PROFILE_G3_ARIB = 3
PLC_PROFILE_G3_CEN_B = 4
PLC_PROFILE_PRIME = 5
PLC_PROFILE_G3_FCC_CEN_A = 6
PLC_PROFILE_G3_FCC_CEN_B = 7
PLC_PROFILE_G3_ARIB_CEN_A = 8
PLC_PROFILE_G3_ARIB_CEN_B = 9

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

def plcExternalInterruptTrigger(symbol, event):
    key = event["symbol"].getKeyDescription(event["value"])
    intSrc = "PIO" + key[1] + "_IRQn"
    symbol.setValue(intSrc, 1)

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

def setPlcBandInUse(plcBand):
    dict = {}

    if (plcBand == "CEN-A"):
        plcBandInUse.setValue(PLC_PROFILE_G3_CEN_A)
        dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_G3_CENA", {})
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENA", {})
    elif (plcBand == "CEN-B"):
        plcBandInUse.setValue(PLC_PROFILE_G3_CEN_B)
        dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_G3_CENB", {})
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENB", {})
    elif (plcBand == "FCC"):
        plcBandInUse.setValue(PLC_PROFILE_G3_FCC)
        dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_G3_FCC", {})
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_FCC", {})
    elif (plcBand == "ARIB"):
        plcBandInUse.setValue(PLC_PROFILE_G3_ARIB)
        dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_G3_ARIB", {})
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_ARIB", {})
    elif (plcBand == "PRIME"):
        plcBandInUse.setValue(PLC_PROFILE_PRIME)
        dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_PRIME", {})
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_PRIME", {})

def setPlcMultiBandInUse(g3_band, g3_aux_band):
    if (g3_band == "FCC"):
        if (g3_aux_band == "CEN-A"):
            plcBandInUse.setValue(PLC_PROFILE_G3_FCC_CEN_A)
        elif (g3_aux_band == "CEN-B"):
            plcBandInUse.setValue(PLC_PROFILE_G3_FCC_CEN_B)
    else:
        if (g3_aux_band == "CEN-A"):
            plcBandInUse.setValue(PLC_PROFILE_G3_ARIB_CEN_A)
        elif (g3_aux_band == "CEN-B"):
            plcBandInUse.setValue(PLC_PROFILE_G3_ARIB_CEN_B)

def removeAllBinFiles():
    print("[CHRIS_dbg] : removeAllBinFiles")
    plcSourceBinFilePRIME.setEnabled(False)
    plcSourceBinFileG3CENA.setEnabled(False)
    plcSourceBinFileG3CENB.setEnabled(False)
    plcSourceBinFileG3FCC.setEnabled(False)
    plcSourceBinFileG3ARIB.setEnabled(False)

def includeBinFile(plcBand):
    if (plcBand == "PRIME"):
        plcSourceBinFilePRIME.setEnabled(True)
        plcSourceBinFileG3CENA.setEnabled(False)
        plcSourceBinFileG3CENB.setEnabled(False)
        plcSourceBinFileG3FCC.setEnabled(False)
        plcSourceBinFileG3ARIB.setEnabled(False)
        print("[CHRIS_dbg] : includeBinFile PRIME")
    else:
        plcSourceBinFilePRIME.setEnabled(False)
        if (plcBand == "CEN-A"):          
            plcSourceBinFileG3CENA.setEnabled(True)
            print("[CHRIS_dbg] : includeBinFile CEN-A")
        elif (plcBand == "CEN-B"):            
            plcSourceBinFileG3CENB.setEnabled(True)
            print("[CHRIS_dbg] : includeBinFile CEN-B")
        elif (plcBand == "FCC"):            
            plcSourceBinFileG3FCC.setEnabled(True)
            print("[CHRIS_dbg] : includeBinFile FCC")
        elif (plcBand == "ARIB"):           
            plcSourceBinFileG3ARIB.setEnabled(True) 
            print("[CHRIS_dbg] : includeBinFile ARIB")

def updateBinFiles():
    removeAllBinFiles()
    if (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PROFILE") == "G3") :
        g3_band = Database.getSymbolValue("drvPlcPhy", "DRV_PLC_G3_BAND")
        includeBinFile(g3_band)
        setPlcBandInUse(g3_band)
        if (g3_band == "FCC" or g3_band =="ARIB") and (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_COUP_G3_MULTIBAND") == True) :
            g3_aux_band = Database.getSymbolValue("drvPlcPhy", "DRV_PLC_G3_BAND_AUX")
            includeBinFile(g3_aux_band)
            setPlcMultiBandInUse(g3_band, g3_aux_band)
    else:
        includeBinFile("PRIME")
        setPlcBandInUse("PRIME")
    
    # Check Internal/External Addressing    
    if (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_BIN_STATIC_ADDRESSING") == False) :
        # Internal Addressing
        plcAssemblyBinFile.setEnabled(True)
    else:
        # External Addressing
        removeAllBinFiles()
        plcAssemblyBinFile.setEnabled(False)

    dict = {}
    dict = Database.sendMessage("srv_pcoup", "SRV_PCOUP_UPDATE_PARAMETERS", {})
    

# def updatePLCBinFiles(symbol, event):
#     updateBinFiles()

def updatePLCBandInUse(symbol, event):
    print("[CHRIS_dbg] : updatePLCBandInUse <- " + event["id"])
    updateBinFiles()

def plcBinAddressingMode(symbol, event):
    print("[CHRIS_dbg] : plcBinAddressingMode <- " + event["id"])
    symbol.setVisible(event["value"])
    updateBinFiles()

def setPlcProfile(symbol, event):
    global plcProfileFile
    global plcProfileDefFile
    global plcProfileHeaderLocalFile

    print("[CHRIS_dbg] : setPlcProfile event value " + event["value"])

    if (event["value"] == "PRIME"):
        print("[CHRIS_dbg] : setPlcProfile update PRIME files")
        plcProfileFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_prime.c")
        plcProfileDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_prime.h")
        plcProfileHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local_prime.h")
    else:
        print("[CHRIS_dbg] : setPlcProfile update G3 files")
        plcSourceBinFilePRIME.setEnabled(False)
        plcProfileFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_g3.c.ftl")
        plcProfileDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_g3.h.ftl")
        plcProfileHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local_g3.h")

def showPL460Pins(symbol, event):
    print("[CHRIS_dbg] : showPL460Pins <- " + event["id"])
    if (event["value"] == "PL460"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)  

def showSleepPin(symbol, event):
    print("[CHRIS_dbg] : showSleepPin <- " + event["id"])
    symbol.setVisible(event["value"])

def showG3HighAttenuation(symbol, event):
    global plcDriverMode

    print("[CHRIS_dbg] : showG3HighAttenuation <- " + event["id"])
    if (event["symbol"].getValue() == "FCC") or (event["symbol"].getValue() == "ARIB"):
        if (plcDriverMode.getValue() == "PL460"):
            symbol.setVisible(True)
        else:
            symbol.setVisible(False)
    else:
        symbol.setVisible(False)
        # symbol.setReadOnly(True)
        # symbol.setValue(False)
        # symbol.setReadOnly(False)

def showG3InternalDriver(symbol, event):
    global plcDriverMode

    print("[CHRIS_dbg] : showG3InternalDriver <- " + event["id"])
    if (event["symbol"].getValue() == "CEN-B") and plcDriverMode.getValue() == "PL360":
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)
        # symbol.setReadOnly(True)
        # symbol.setValue(False)
        # symbol.setReadOnly(False)

def showG3Multiband(symbol, event):
    print("[CHRIS_dbg] : showG3Multiband <- " + event["id"])
    if (event["symbol"].getValue() == "FCC") or (event["symbol"].getValue() == "ARIB"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)
        # symbol.setReadOnly(True)
        # symbol.setValue(False)
        # symbol.setReadOnly(False)

def showG3AuxBand(symbol, event):
    print("[CHRIS_dbg] : showG3AuxBand <- " + event["id"])
    if (event["value"] == True):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)
    
def showPRIMEHighAttenuation(symbol, event):
    print("[CHRIS_dbg] : showPRIMEHighAttenuation <- " + event["id"])
    if (event["symbol"].getValue() == "CH1"):
        symbol.setVisible(False)
        # symbol.setReadOnly(True)
        # symbol.setValue(False)
        # symbol.setReadOnly(False)
    else:
        symbol.setVisible(True)

def showPRIMEMultiband(symbol, event):
    print("[CHRIS_dbg] : showPRIMEMultiband <- " + event["id"])
    if (event["symbol"].getValue() == "CH1"):
        symbol.setVisible(False)
        # symbol.setReadOnly(True)
        # symbol.setValue(False)
        # symbol.setReadOnly(False)
    else:
        symbol.setVisible(True)

def showPRIMEAuxBand(symbol, event):
    print("[CHRIS_dbg] : showPRIMEAuxBand <- " + event["id"])
    if (event["value"] == True):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)    

def updateShowCoupSettings(symbol, event):
    print("[CHRIS_dbg] : updateShowCoupSettings <- " + event["id"])
    global plcCoupPRIMESettings
    global plcCoupG3Settings

    if (event["value"] == "G3"):
        plcCoupG3Settings.setVisible(True)
        plcCoupPRIMESettings.setVisible(False)
    else :
        plcCoupG3Settings.setVisible(False)
        plcCoupPRIMESettings.setVisible(True)

def resetPlcBand(symbol, event):  
    print("[CHRIS_dbg] : resetPlcBand <- " + event["id"])
    symbol.setReadOnly(True)
    symbol.setValue("CEN-A")
    symbol.setReadOnly(False)

def enablePL460Capabilities(symbol, event):
    if (event["value"] == "PL460"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def instantiateComponent(plcComponent):  
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
        print("[CHRIS_dbg] : DMA is NOT present")
    else:
        isDMAPresent = True
        print("[CHRIS_dbg] : DMA is present")

    global plcDriverMode
    plcDriverMode = plcComponent.createComboSymbol("DRV_PLC_MODE", None, ["PL360", "PL460"])
    plcDriverMode.setLabel("PLC Driver Mode")
    plcDriverMode.setDefaultValue("PL460")

    plcPLIB = plcComponent.createStringSymbol("DRV_PLC_PLIB", None)
    plcPLIB.setLabel("PLIB Used")
    plcPLIB.setReadOnly(True)

    plcPLIBCSRIndex = plcComponent.createIntegerSymbol("DRV_PLC_PLIB_CSR_INDEX", None)
    plcPLIBCSRIndex.setLabel("PLIB CSR Index")
    plcPLIBCSRIndex.setReadOnly(True)

    plcExtIntPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_EXT_INT_PIN", None)
    plcExtIntPin.setLabel("External Interrupt Pin")
    plcExtIntPin.setDefaultValue(0)
    plcExtIntPin.setOutputMode("Key")
    plcExtIntPin.setDisplayMode("Description")

    plcExtIntSource = plcComponent.createStringSymbol("DRV_PLC_EXT_INT_SRC", None)
    plcExtIntSource.setLabel("External Interrupt Source")
    plcExtIntSource.setDefaultValue("PIOA_IRQn")
    plcExtIntSource.setVisible(True)
    plcExtIntSource.setReadOnly(True)
    plcExtIntSource.setDependencies(plcExternalInterruptTrigger, ["DRV_PLC_EXT_INT_PIN"])

    plcPhyExtIntPioPort = plcComponent.createStringSymbol("DRV_PLC_EXT_INT_PIO_PORT", None)
    plcPhyExtIntPioPort.setLabel("External Interrupt Port")
    plcPhyExtIntPioPort.setDefaultValue("PIO_PORT_A")
    plcPhyExtIntPioPort.setVisible(True)
    plcPhyExtIntPioPort.setReadOnly(True)
    plcPhyExtIntPioPort.setDependencies(plcPhyExternalInterruptPort, ["DRV_PLC_EXT_INT_PIN"])

    plcResetPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_RESET_PIN", None)
    plcResetPin.setLabel("Reset Pin")
    plcResetPin.setDefaultValue(0)
    plcResetPin.setOutputMode("Key")
    plcResetPin.setDisplayMode("Description")

    plcLDOEnPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_LDO_EN_PIN", None)
    plcLDOEnPin.setLabel("LDO Enable Pin")
    plcLDOEnPin.setDefaultValue(0)
    plcLDOEnPin.setOutputMode("Key")
    plcLDOEnPin.setDisplayMode("Description")

    plcCDPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_CD_PIN", None)
    plcCDPin.setLabel("Carrier Detect Pin")
    plcCDPin.setDefaultValue(0)
    plcCDPin.setOutputMode("Key")
    plcCDPin.setDisplayMode("Description")

    plcSleepMode = plcComponent.createBooleanSymbol("DRV_PLC_SLEEP_MODE", None)
    plcSleepMode.setLabel("Sleep Mode")
    plcSleepMode.setDefaultValue(False)
    plcSleepMode.setVisible(True)

    plcStbyPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_STBY_PIN", plcSleepMode)
    plcStbyPin.setLabel("Stand By Pin")
    plcStbyPin.setDefaultValue(0)
    plcStbyPin.setOutputMode("Key")
    plcStbyPin.setDisplayMode("Description")
    plcStbyPin.setVisible(False)
    plcStbyPin.setDependencies(showSleepPin, ["DRV_PLC_SLEEP_MODE"])

    plcThMonPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_THMON_PIN", None)
    plcThMonPin.setLabel("Thermal Monitor Pin")
    plcThMonPin.setDefaultValue(0)
    plcThMonPin.setOutputMode("Key")
    plcThMonPin.setDisplayMode("Description")
    plcThMonPin.setDependencies(showPL460Pins, ["DRV_PLC_MODE"])

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
        plcCDPin.addKey(key, value, description)
        plcStbyPin.addKey(key, value, description)
        plcThMonPin.addKey(key, value, description)

    plcSymPinConfigComment = plcComponent.createCommentSymbol("DRV_PLC_PINS_CONFIG_COMMENT", None)
    plcSymPinConfigComment.setVisible(True)
    plcSymPinConfigComment.setLabel("***Above selected pins must be configured as GPIO Output in Pin Manager***")
    
    ##### Do not modify below symbol names as they are used by Memory Driver #####

    global plcTXRXDMA
    plcTXRXDMA = plcComponent.createBooleanSymbol("DRV_PLC_TX_RX_DMA", None)
    plcTXRXDMA.setLabel("Use DMA for Transmit and Receive?")
    plcTXRXDMA.setDefaultValue(0)
    plcTXRXDMA.setVisible(False) #### Change to hide it
    plcTXRXDMA.setReadOnly(True)

    global plcTXDMAChannel
    plcTXDMAChannel = plcComponent.createIntegerSymbol("DRV_PLC_TX_DMA_CHANNEL", None)
    plcTXDMAChannel.setLabel("DMA Channel For Transmit")
    plcTXDMAChannel.setDefaultValue(0)
    plcTXDMAChannel.setVisible(False)
    plcTXDMAChannel.setReadOnly(True)
    plcTXDMAChannel.setDependencies(requestAndAssignTxDMAChannel, ["DRV_PLC_TX_RX_DMA"])

    global plcTXDMAChannelComment
    plcTXDMAChannelComment = plcComponent.createCommentSymbol("DRV_PLC_TX_DMA_CH_COMMENT", None)
    plcTXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Transmit. Check DMA manager.")
    plcTXDMAChannelComment.setVisible(False)
    plcTXDMAChannelComment.setDependencies(requestDMAComment, ["DRV_PLC_TX_DMA_CHANNEL"])

    global plcRXDMAChannel
    plcRXDMAChannel = plcComponent.createIntegerSymbol("DRV_PLC_RX_DMA_CHANNEL", None)
    plcRXDMAChannel.setLabel("DMA Channel For Receive")
    plcRXDMAChannel.setDefaultValue(1)
    plcRXDMAChannel.setVisible(False)
    plcRXDMAChannel.setReadOnly(True)
    plcRXDMAChannel.setDependencies(requestAndAssignRxDMAChannel, ["DRV_PLC_TX_RX_DMA"])

    global plcRXDMAChannelComment
    plcRXDMAChannelComment = plcComponent.createCommentSymbol("DRV_PLC_RX_DMA_CH_COMMENT", None)
    plcRXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Receive. Check DMA manager.")
    plcRXDMAChannelComment.setVisible(False)
    plcRXDMAChannelComment.setDependencies(requestDMAComment, ["DRV_PLC_RX_DMA_CHANNEL"])

    plcDependencyDMAComment = plcComponent.createCommentSymbol("DRV_PLC_DEPENDENCY_DMA_COMMENT", None)
    plcDependencyDMAComment.setLabel("!!! Satisfy PLIB Dependency to Allocate DMA Channel !!!")
    plcDependencyDMAComment.setVisible(isDMAPresent)

    plcStaticAddressing = plcComponent.createBooleanSymbol("DRV_PLC_BIN_STATIC_ADDRESSING", None)
    plcStaticAddressing.setLabel("Static Bin file Addressing")
    plcStaticAddressing.setVisible(True)
    plcStaticAddressing.setDefaultValue(False)

    plcBinaryAddress = plcComponent.createHexSymbol("DRV_PLC_PLC_BIN_ADDRESS", plcStaticAddressing)
    plcBinaryAddress.setLabel("PLC Bin Address")
    plcBinaryAddress.setVisible(False)
    plcBinaryAddress.setDefaultValue(0x004A0000)
    plcBinaryAddress.setDependencies(plcBinAddressingMode, ["DRV_PLC_BIN_STATIC_ADDRESSING"])

    plcBinarySize = plcComponent.createHexSymbol("DRV_PLC_PLC_BIN_SIZE", plcStaticAddressing)
    plcBinarySize.setLabel("PLC Bin Size (bytes)")
    plcBinarySize.setVisible(False)
    plcBinarySize.setDefaultValue(0x10000)
    plcBinarySize.setDependencies(plcBinAddressingMode, ["DRV_PLC_BIN_STATIC_ADDRESSING"])

    plcSecureMode = plcComponent.createBooleanSymbol("DRV_PLC_SECURE_MODE", None)
    plcSecureMode.setLabel("PLC Secure Mode")
    plcSecureMode.setVisible(True)
    plcSecureMode.setDefaultValue(False)

    ############################################################################
    #### Code Generation ####
    ############################################################################

    configName = Variables.get("__CONFIGURATION_NAME")

    #### Common Files ##########################################################

    plcPhyHalHeaderFile = plcComponent.createFileSymbol("PLC_HAL_HEADER", None)
    plcPhyHalHeaderFile.setSourcePath("driver/common/plcHal/drv_plc_hal.h.ftl")
    plcPhyHalHeaderFile.setOutputName("drv_plc_hal.h")
    plcPhyHalHeaderFile.setDestPath("driver/plc/common")
    plcPhyHalHeaderFile.setProjectPath("config/" + configName + "/driver/plc/common")
    plcPhyHalHeaderFile.setType("HEADER")
    plcPhyHalHeaderFile.setMarkup(True)

    plcPhyHalFile = plcComponent.createFileSymbol("PLC_HAL", None)
    plcPhyHalFile.setSourcePath("driver/common/plcHal/drv_plc_hal.c.ftl")
    plcPhyHalFile.setOutputName("drv_plc_hal.c")
    plcPhyHalFile.setDestPath("driver/plc/common")
    plcPhyHalFile.setProjectPath("config/" + configName + "/driver/plc/common")
    plcPhyHalFile.setType("SOURCE")
    plcPhyHalFile.setMarkup(True)

    plcPhyHALInitDataFile = plcComponent.createFileSymbol("DRV_HAL_INIT_DATA", None)
    plcPhyHALInitDataFile.setType("STRING")
    plcPhyHALInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    plcPhyHALInitDataFile.setSourcePath("driver/common/plcHal/drv_plc_hal_initialization_data.c.ftl")
    plcPhyHALInitDataFile.setMarkup(True)

    plcPhyBootFile = plcComponent.createFileSymbol("PLC_BOOT", None)
    plcPhyBootFile.setSourcePath("driver/common/plcBoot/drv_plc_boot.c")
    plcPhyBootFile.setOutputName("drv_plc_boot.c")
    plcPhyBootFile.setDestPath("driver/plc/common")
    plcPhyBootFile.setProjectPath("config/" + configName + "/driver/plc/common")
    plcPhyBootFile.setType("SOURCE")

    plcPhyBootHeaderFile = plcComponent.createFileSymbol("PLC_BOOT_HEADER", None)
    plcPhyBootHeaderFile.setSourcePath("driver/common/plcBoot/drv_plc_boot.h")
    plcPhyBootHeaderFile.setOutputName("drv_plc_boot.h")
    plcPhyBootHeaderFile.setDestPath("driver/plc/common")
    plcPhyBootHeaderFile.setProjectPath("config/" + configName + "/driver/plc/common")
    plcPhyBootHeaderFile.setType("HEADER")

    #### ASM Path Settings #####################################################

    plcAsmPathSetting = plcComponent.createSettingSymbol("DRV_PLC_ASM_PATH_SETTING", None)
    plcAsmPathSetting.setCategory("C32-AS")
    plcAsmPathSetting.setKey("extra-include-directories-for-assembler")
    plcAsmPathSetting.setValue("../src/config/" + configName + "/driver/plc/phy/bin")
    plcAsmPathSetting.setAppend(True, ";")

    #### Miscellanea ###########################################################

    plcPhySecureScript = plcComponent.createFileSymbol("DRV_PLC_PHY_SECURE_SCRIPT", None)
    plcPhySecureScript.setSourcePath("driver/plcPhy/src/bin/plcPhy_encfile.py")
    plcPhySecureScript.setOutputName("plcPhy_encfile.py")
    plcPhySecureScript.setDestPath("driver/plc/phy/bin/")
    plcPhySecureScript.setProjectPath("config/" + configName + "/driver/plc/phy/bin/")
    plcPhySecureScript.setType("SOURCE")
    plcPhySecureScript.setEnabled(False)
    plcPhySecureScript.setDependencies(plcPhyEnableEncScript, ["DRV_PLC_SECURE_MODE"])

    #### Source and header PLC PHY Files #######################################

    plcHeaderFile = plcComponent.createFileSymbol("PLC_HEADER", None)
    plcHeaderFile.setSourcePath("driver/plcPhy/drv_plc_phy.h.ftl")
    plcHeaderFile.setOutputName("drv_plc_phy.h")
    plcHeaderFile.setDestPath("driver/plc/phy")
    plcHeaderFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcHeaderFile.setType("HEADER")
    plcHeaderFile.setMarkup(True)

    plcSymHeaderDefFile = plcComponent.createFileSymbol("DRV_PLC_DEF", None)
    plcSymHeaderDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_definitions.h")
    plcSymHeaderDefFile.setOutputName("drv_plc_phy_definitions.h")
    plcSymHeaderDefFile.setDestPath("driver/plc/phy")
    plcSymHeaderDefFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcSymHeaderDefFile.setType("HEADER")

    plcHeaderLocalFile = plcComponent.createFileSymbol("PLC_HEADER_LOCAL", None)
    plcHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local.h.ftl")
    plcHeaderLocalFile.setOutputName("drv_plc_phy_local.h")
    plcHeaderLocalFile.setDestPath("driver/plc/phy")
    plcHeaderLocalFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcHeaderLocalFile.setType("HEADER")
    plcHeaderLocalFile.setMarkup(True)

    global plcProfileFile
    plcProfileFile = plcComponent.createFileSymbol("DRV_PLC_PROF_FILE", None)
    plcProfileFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_g3.c.ftl")
    plcProfileFile.setOutputName("drv_plc_phy_comm.c")
    plcProfileFile.setDestPath("driver/plc/phy")
    plcProfileFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcProfileFile.setType("SOURCE")
    plcProfileFile.setDependencies(setPlcProfile, ["DRV_PLC_PROFILE"])
    plcProfileFile.setMarkup(True)

    global plcProfileDefFile
    plcProfileDefFile = plcComponent.createFileSymbol("DRV_PLC_PROFILE_DEF", None)
    plcProfileDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_g3.h.ftl")
    plcProfileDefFile.setOutputName("drv_plc_phy_comm.h")
    plcProfileDefFile.setDestPath("driver/plc/phy")
    plcProfileDefFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcProfileDefFile.setType("HEADER")
    plcProfileDefFile.setMarkup(True)

    plcSourceFile = plcComponent.createFileSymbol("PLC_SOURCE", None)
    plcSourceFile.setSourcePath("driver/plcPhy/src/drv_plc_phy.c.ftl")
    plcSourceFile.setOutputName("drv_plc_phy.c")
    plcSourceFile.setDestPath("driver/plc/phy")
    plcSourceFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcSourceFile.setType("SOURCE")
    plcSourceFile.setMarkup(True)

    global plcProfileHeaderLocalFile
    plcProfileHeaderLocalFile = plcComponent.createFileSymbol("DRV_PLC_PROFILE_LOCAL", None)
    plcProfileHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local_g3.h")
    plcProfileHeaderLocalFile.setOutputName("drv_plc_phy_local_comm.h")
    plcProfileHeaderLocalFile.setDestPath("driver/plc/phy")
    plcProfileHeaderLocalFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcProfileHeaderLocalFile.setType("SOURCE")
    plcProfileHeaderLocalFile.setEnabled(True)

    #### Binary PHY Files ######################################################

    global plcSourceBinFileG3CENA
    plcSourceBinFileG3CENA = plcComponent.createFileSymbol("PLC_SOURCE_BIN_G3_CENA", None)
    plcSourceBinFileG3CENA.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_G3_CENA.bin")
    plcSourceBinFileG3CENA.setOutputName("PLC_PHY_G3_CENA.bin")
    plcSourceBinFileG3CENA.setDestPath("driver/plc/phy/bin")
    plcSourceBinFileG3CENA.setProjectPath("config/" + configName + "/driver/plc/phy/bin")
    plcSourceBinFileG3CENA.setType("SOURCE")
    plcSourceBinFileG3CENA.setEnabled(True)
    plcSourceBinFileG3CENA.setVisible(False)

    global plcSourceBinFileG3CENB
    plcSourceBinFileG3CENB = plcComponent.createFileSymbol("PLC_SOURCE_BIN_G3_CENB", None)
    plcSourceBinFileG3CENB.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_G3_CENB.bin")
    plcSourceBinFileG3CENB.setOutputName("PLC_PHY_G3_CENB.bin")
    plcSourceBinFileG3CENB.setDestPath("driver/plc/phy/bin")
    plcSourceBinFileG3CENB.setProjectPath("config/" + configName + "/driver/plc/phy/bin")
    plcSourceBinFileG3CENB.setType("SOURCE")
    plcSourceBinFileG3CENB.setEnabled(False)
    plcSourceBinFileG3CENB.setVisible(False)

    global plcSourceBinFileG3FCC
    plcSourceBinFileG3FCC = plcComponent.createFileSymbol("PLC_SOURCE_BIN_G3_FCC", None)
    plcSourceBinFileG3FCC.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_G3_FCC.bin")
    plcSourceBinFileG3FCC.setOutputName("PLC_PHY_G3_FCC.bin")
    plcSourceBinFileG3FCC.setDestPath("driver/plc/phy/bin")
    plcSourceBinFileG3FCC.setProjectPath("config/" + configName + "/driver/plc/phy/bin")
    plcSourceBinFileG3FCC.setType("SOURCE")
    plcSourceBinFileG3FCC.setEnabled(False)
    plcSourceBinFileG3FCC.setVisible(False)

    global plcSourceBinFileG3ARIB
    plcSourceBinFileG3ARIB = plcComponent.createFileSymbol("PLC_SOURCE_BIN_G3_ARIB", None)
    plcSourceBinFileG3ARIB.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_G3_ARIB.bin")
    plcSourceBinFileG3ARIB.setOutputName("PLC_PHY_G3_ARIB.bin")
    plcSourceBinFileG3ARIB.setDestPath("driver/plc/phy/bin")
    plcSourceBinFileG3ARIB.setProjectPath("config/" + configName + "/driver/plc/phy/bin")
    plcSourceBinFileG3ARIB.setType("SOURCE")
    plcSourceBinFileG3ARIB.setEnabled(False)
    plcSourceBinFileG3ARIB.setVisible(False)

    global plcSourceBinFilePRIME
    plcSourceBinFilePRIME = plcComponent.createFileSymbol("PLC_SOURCE_BIN_PRIME", None)
    plcSourceBinFilePRIME.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_PRIME_2CHN.bin")
    plcSourceBinFilePRIME.setOutputName("PLC_PHY_PRIME_2CHN.bin")
    plcSourceBinFilePRIME.setDestPath("driver/plc/phy/bin")
    plcSourceBinFilePRIME.setProjectPath("config/" + configName + "/driver/plc/phy/bin")
    plcSourceBinFilePRIME.setType("SOURCE")
    plcSourceBinFilePRIME.setEnabled(False)
    plcSourceBinFilePRIME.setVisible(False)

    global plcAssemblyBinFile
    plcAssemblyBinFile = plcComponent.createFileSymbol("PLC_ASSEMBLY_BIN", None)
    plcAssemblyBinFile.setSourcePath("driver/plcPhy/src/bin/plc_phy_bin.S.ftl")
    plcAssemblyBinFile.setOutputName("plc_phy_bin.S")
    plcAssemblyBinFile.setDestPath("driver/plc/phy/bin")
    plcAssemblyBinFile.setProjectPath("config/" + configName + "/driver/plc/phy/bin")
    plcAssemblyBinFile.setType("SOURCE")
    plcAssemblyBinFile.setMarkup(True)
    
    ##### PLC Profile Selector  ####################################################

    plcProfile = plcComponent.createComboSymbol("DRV_PLC_PROFILE", None, ["G3", "PRIME"])
    plcProfile.setLabel("PLC Profile")
    plcProfile.setDefaultValue("G3")

    ##### Coupling Settings : G3  ####################################################

    updBinFilesCtrl = plcComponent.createBooleanSymbol("DRV_PLC_UPD_BIN_FILES", None)
    updBinFilesCtrl.setLabel("updBinFilesCtrl")
    updBinFilesCtrl.setDescription("Auxiliary control")
    updBinFilesCtrl.setVisible(False)
    updBinFilesCtrl.setDefaultValue(False)
    updBinFilesCtrl.setDependencies(updateShowCoupSettings, ["DRV_PLC_PROFILE"])

    global plcCoupG3Settings
    plcCoupG3Settings = plcComponent.createMenuSymbol("DRV_PLC_COUP_G3_SETTING", None)
    plcCoupG3Settings.setLabel("PLC Coupling Settings")
    plcCoupG3Settings.setDescription("Coupling Settings")
    plcCoupG3Settings.setVisible(True)

    plcG3Band = plcComponent.createComboSymbol("DRV_PLC_G3_BAND", plcCoupG3Settings, ["CEN-A", "CEN-B", "FCC", "ARIB"])
    plcG3Band.setLabel("G3 Phy Band")
    plcG3Band.setDefaultValue("CEN-A")
    # plcG3Band.setDependencies(resetPlcBand, ["DRV_PLC_MODE"])

    plcCoupG3Internal = plcComponent.createBooleanSymbol("DRV_PLC_COUP_G3_INTERNAL", plcCoupG3Settings)
    plcCoupG3Internal.setLabel("Internal Driver")
    plcCoupG3Internal.setDescription("Internal Driver")
    plcCoupG3Internal.setVisible(False)
    plcCoupG3Internal.setDefaultValue(False)
    plcCoupG3Internal.setDependencies(showG3InternalDriver, ["DRV_PLC_G3_BAND"])

    plcCoupGMultiBand = plcComponent.createBooleanSymbol("DRV_PLC_COUP_G3_MULTIBAND", plcCoupG3Settings)
    plcCoupGMultiBand.setLabel("Multiband")
    plcCoupGMultiBand.setDescription("Multiband")
    plcCoupGMultiBand.setVisible(False)
    plcCoupGMultiBand.setDefaultValue(False)
    plcCoupGMultiBand.setDependencies(showG3Multiband, ["DRV_PLC_G3_BAND"])

    plcG3BandAux = plcComponent.createComboSymbol("DRV_PLC_G3_BAND_AUX", plcCoupGMultiBand, ["CEN-A", "CEN-B"])
    plcG3BandAux.setLabel("G3 Phy Auxiliary Band")
    plcG3BandAux.setDefaultValue("CEN-A")
    plcG3BandAux.setVisible(False)
    plcG3BandAux.setDependencies(showG3AuxBand, ["DRV_PLC_COUP_G3_MULTIBAND"])

    plcG3BandAuxActive = plcComponent.createBooleanSymbol("DRV_PLC_G3_BAND_AUX_ACTIVE", plcCoupGMultiBand)
    plcG3BandAuxActive.setLabel("Set as default branch")
    plcG3BandAuxActive.setDefaultValue(False)
    plcG3BandAuxActive.setVisible(False)
    plcG3BandAuxActive.setDependencies(showG3AuxBand, ["DRV_PLC_COUP_G3_MULTIBAND"])

    plcCoupG3HighAttenuation = plcComponent.createBooleanSymbol("DRV_PLC_COUP_G3_HIGH_ATTENUATION", plcG3Band)
    plcCoupG3HighAttenuation.setLabel("FCC high attenuation coupling")
    plcCoupG3HighAttenuation.setDescription("FCC high attenuation")
    plcCoupG3HighAttenuation.setVisible(False)
    plcCoupG3HighAttenuation.setDefaultValue(False)
    plcCoupG3HighAttenuation.setDependencies(showG3HighAttenuation, ["DRV_PLC_G3_BAND"])
    
    ##### Coupling Settings : PRIME  ####################################################
    global plcCoupPRIMESettings
    plcCoupPRIMESettings = plcComponent.createMenuSymbol("DRV_PLC_COUP_PRIME_SETTING", None)
    plcCoupPRIMESettings.setLabel("PLC Coupling Settings")
    plcCoupPRIMESettings.setDescription("Coupling Settings")
    plcCoupPRIMESettings.setVisible(False)

    plcCoupPRIMEMultiBand = plcComponent.createBooleanSymbol("DRV_PLC_COUP_PRIME_MULTIBAND", plcCoupPRIMESettings)
    plcCoupPRIMEMultiBand.setLabel("Multiband")
    plcCoupPRIMEMultiBand.setDescription("Multiband")
    plcCoupPRIMEMultiBand.setVisible(True)
    plcCoupPRIMEMultiBand.setDefaultValue(False)
    plcCoupPRIMEMultiBand.setDependencies(showPRIMEMultiband, ["DRV_PLC_PRIME_CHN"])

    plcPRIME3BandAux = plcComponent.createComboSymbol("DRV_PLC_PRIME_BAND_AUX", plcCoupPRIMEMultiBand, ["CH1"])
    plcPRIME3BandAux.setLabel("PRIME Phy Second Channel")
    plcPRIME3BandAux.setDefaultValue("CEN-A")
    plcPRIME3BandAux.setVisible(False)
    plcPRIME3BandAux.setDependencies(showPRIMEAuxBand, ["DRV_PLC_COUP_PRIME_MULTIBAND"])

    plcPRIMEChannel = plcComponent.createComboSymbol("DRV_PLC_PRIME_CHN", plcCoupPRIMESettings, ["CH1", "CH3", "CH4", "CH5", "CH6", "CH7", "CH8", "CH3-4", "CH4-5", "CH5-6", "CH6-7", "CH7-8"])
    plcPRIMEChannel.setLabel("PRIME Phy Channel")
    plcPRIMEChannel.setDefaultValue("CH1")
    plcPRIMEChannel.setVisible(True)

    plcCoupPRIMEHighAttenuation = plcComponent.createBooleanSymbol("DRV_PLC_PRIME_HIGH_ATTENUATION", plcPRIMEChannel)
    plcCoupPRIMEHighAttenuation.setLabel("FCC high attenuation coupling")
    plcCoupPRIMEHighAttenuation.setDescription("FCC high attenuation")
    plcCoupPRIMEHighAttenuation.setVisible(True)
    plcCoupPRIMEHighAttenuation.setDefaultValue(False)
    plcCoupPRIMEHighAttenuation.setDependencies(showPRIMEHighAttenuation, ["DRV_PLC_PRIME_CHN"])

    plcPhyKeyCortex = plcComponent.createHexSymbol("DRV_PLC_CORE_KEY", None)
    plcPhyKeyCortex.setLabel("PLC Phy Key Cortex")
    plcPhyKeyCortex.setDefaultValue(0x1122)
    plcPhyKeyCortex.setVisible(False)
    plcPhyKeyCortex.setReadOnly(True)

    ##### Coupling Settings : Generic  ####################################################

    global plcBandInUse
    plcBandInUse = plcComponent.createIntegerSymbol("DRV_PLC_BAND_IN_USE", None)
    plcBandInUse.setLabel("PLC Band in use")
    plcBandInUse.setDefaultValue(PLC_PROFILE_G3_CEN_A)
    #plcBandInUse.setVisible(True)
    plcBandInUse.setReadOnly(True)
    plcBandInUse.setDependencies(updatePLCBandInUse, ["DRV_PLC_PROFILE", "DRV_PLC_G3_BAND", "DRV_PLC_G3_BAND_AUX", "DRV_PLC_COUP_G3_MULTIBAND"])

    plcThermalMonitor = plcComponent.createBooleanSymbol("DRV_PLC_THERMAL_MONITOR", None)
    plcThermalMonitor.setLabel("Thermal Monitor")
    plcThermalMonitor.setDefaultValue(False)
    plcThermalMonitor.setDependencies(enablePL460Capabilities, ["DRV_PLC_MODE"]);
    

    #### FreeMaker Files ######################################################

    plcSystemDefFile = plcComponent.createFileSymbol("PLC_DEF", None)
    plcSystemDefFile.setType("STRING")
    plcSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    plcSystemDefFile.setSourcePath("driver/plcPhy/templates/system/definitions.h.ftl")
    plcSystemDefFile.setMarkup(True)

    plcSymSystemDefObjFile = plcComponent.createFileSymbol("DRV_PLC_SYSTEM_DEF_OBJECT", None)
    plcSymSystemDefObjFile.setType("STRING")
    plcSymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    plcSymSystemDefObjFile.setSourcePath("driver/plcPhy/templates/system/definitions_objects.h.ftl")
    plcSymSystemDefObjFile.setMarkup(True)

    plcSymSystemConfigFile = plcComponent.createFileSymbol("DRV_PLC_CONFIGIRUTION", None)
    plcSymSystemConfigFile.setType("STRING")
    plcSymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    plcSymSystemConfigFile.setSourcePath("driver/plcPhy/templates/system/configuration.h.ftl")
    plcSymSystemConfigFile.setMarkup(True)

    plcSymSystemInitDataFile = plcComponent.createFileSymbol("DRV_PLC_INIT_DATA", None)
    plcSymSystemInitDataFile.setType("STRING")
    plcSymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    plcSymSystemInitDataFile.setSourcePath("driver/plcPhy/templates/system/initialize_data.c.ftl")
    plcSymSystemInitDataFile.setMarkup(True)

    plcSystemInitFile = plcComponent.createFileSymbol("PLC_INIT", None)
    plcSystemInitFile.setType("STRING")
    plcSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    plcSystemInitFile.setSourcePath("driver/plcPhy/templates/system/initialize.c.ftl")
    plcSystemInitFile.setMarkup(True)

    plcSystemTasksFile = plcComponent.createFileSymbol("DRV_PLC_SYS_TASK", None)
    plcSystemTasksFile.setType("STRING")
    plcSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    plcSystemTasksFile.setSourcePath("driver/plcPhy/templates/system/system_tasks.c.ftl")
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

    print("[CHIRS_dbg] : onAttachmentConnected event - remoteID: " + remoteID.upper())

    if connectID == "drv_plc_phy_SPI_dependency" :
        print("[CHIRS_dbg] : drv_plc_phy_SPI_dependency")
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
        print("[CHIRS_dbg] : Set SPI baudrate: 8000000")

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
                print("[CHIRS_dbg] : DMA ERROR")
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

    print("[CHIRS_dbg] : onAttachmentDisconnected event - remoteID: " + remoteID.upper())

    if connectID == "drv_plc_phy_SPI_dependency" :
        print("[CHIRS_dbg] : drv_plc_phy_SPI_dependency")

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
                print("[CHIRS_dbg] : DMA ERROR")
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

    spiPeripheral = Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PLIB")

    dmaChannelID = "DMA_CH_FOR_" + str(spiPeripheral) + "_Transmit"
    dmaRequestID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Transmit"

    print("[CHRIS_dbg] : requestAndAssignTxDMAChannel - " + dmaChannelID)

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
    print("[CHRIS_dbg] : requestAndAssignTxDMAChannel - channel " + str(channel))
    symbol.setValue(channel)

def requestAndAssignRxDMAChannel(symbol, event):
    global plcRXDMAChannelComment

    spiPeripheral = Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PLIB")

    dmaChannelID = "DMA_CH_FOR_" + str(spiPeripheral) + "_Receive"
    dmaRequestID = "DMA_CH_NEEDED_FOR_" + str(spiPeripheral) + "_Receive"

    print("[CHRIS_dbg] : requestAndAssignRxDMAChannel - " + dmaChannelID)

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

def destroyComponent(plcComponent):
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_PORTS", {"isEnabled":False})

    if Database.getSymbolValue("core", "DMA_ENABLE") != None:
        Database.sendMessage("HarmonyCore", "ENABLE_SYS_DMA", {"isEnabled":False})
