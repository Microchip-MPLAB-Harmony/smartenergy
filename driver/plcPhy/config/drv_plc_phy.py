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

global plcBandInUse

PLC_PROFILE_G3_CEN_A = 1
PLC_PROFILE_G3_FCC = 2
PLC_PROFILE_G3_ARIB = 3
PLC_PROFILE_G3_CEN_B = 4
PLC_PROFILE_PRIME = 5
PLC_PROFILE_G3_FCC_CEN_A = 6
PLC_PROFILE_G3_FCC_CEN_B = 7

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

def setPlcBandInUse(plcBand):
    if (plcBand == "CEN-A"):
        plcBandInUse.setValue(PLC_PROFILE_G3_CEN_A)
    elif (plcBand == "CEN-B"):
        plcBandInUse.setValue(PLC_PROFILE_G3_CEN_B)
    elif (plcBand == "FCC"):
        plcBandInUse.setValue(PLC_PROFILE_G3_FCC)
    elif (plcBand == "ARIB"):
        plcBandInUse.setValue(PLC_PROFILE_G3_ARIB)
    elif (plcBand == "PRIME"):
        plcBandInUse.setValue(PLC_PROFILE_PRIME)

def setPlcMultiBandInUse(plcBand):
    if (plcBand == "CEN-A"):
        plcBandInUse.setValue(PLC_PROFILE_G3_FCC_CEN_A)
    elif (plcBand == "CEN-B"):
        plcBandInUse.setValue(PLC_PROFILE_G3_FCC_CEN_B)


def plcBinAddressingMode(symbol, event):
    # print("[CHRIS_dbg] : plcBinAddressingMode")
    symbol.setVisible(event["value"])

def removeAllBinFiles():
    # print("[CHRIS_dbg] : removeAllBinFiles")
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
        # print("[CHRIS_dbg] : includeBinFile PRIME")
    else:
        plcSourceBinFilePRIME.setEnabled(False)
        if (plcBand == "CEN-A"):          
            plcSourceBinFileG3CENA.setEnabled(True)
            # print("[CHRIS_dbg] : includeBinFile CEN-A")
        elif (plcBand == "CEN-B"):            
            plcSourceBinFileG3CENB.setEnabled(True)
            # print("[CHRIS_dbg] : includeBinFile CEN-B")
        elif (plcBand == "FCC"):            
            plcSourceBinFileG3FCC.setEnabled(True)
            # print("[CHRIS_dbg] : includeBinFile FCC")
        elif (plcBand == "ARIB"):           
            plcSourceBinFileG3ARIB.setEnabled(True) 
            # print("[CHRIS_dbg] : includeBinFile ARIB")

def updateBinFiles():
    removeAllBinFiles()
    if (Database.getSymbolValue("drvPlc", "DRV_PLC_PROFILE") == "G3") :
        g3_band = Database.getSymbolValue("drvPlc", "DRV_PLC_G3_BAND")
        includeBinFile(g3_band)
        setPlcBandInUse(g3_band)
        if (Database.getSymbolValue("drvPlc", "DRV_PLC_COUP_G3_MULTIBAND") == True) :
            g3_aux_band = Database.getSymbolValue("drvPlc", "DRV_PLC_G3_BAND_AUX")
            includeBinFile(g3_aux_band)
            setPlcMultiBandInUse(g3_aux_band)
    else:
        includeBinFile("PRIME")
        setPlcBandInUse("PRIME")

def setPlcProfile(symbol, event):
    global plcProfileFile
    global plcProfileDefFile
    global plcProfileHeaderLocalFile

    # print("[CHRIS_dbg] : setPlcProfile event value " + event["value"])

    if (event["value"] == "PRIME"):
        # print("[CHRIS_dbg] : setPlcProfile update PRIME files")
        plcProfileFile.setSourcePath("driver/plc/src/drv_plc_prime.c")
        plcProfileDefFile.setSourcePath("driver/plc/drv_plc_prime.h")
        plcProfileHeaderLocalFile.setSourcePath("driver/plc/src/drv_plc_local_prime.h")
    else:
        # print("[CHRIS_dbg] : setPlcProfile update G3 files")
        plcSourceBinFilePRIME.setEnabled(False)
        plcProfileFile.setSourcePath("driver/plc/src/drv_plc_g3.c")
        plcProfileDefFile.setSourcePath("driver/plc/drv_plc_g3.h")
        plcProfileHeaderLocalFile.setSourcePath("driver/plc/src/drv_plc_local_g3.h")

def showPL460Mode(symbol, event):
    # print("[CHRIS_dbg] : showPL460Mode")
    if (event["value"] == "PL460"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)   

def showG3HighAttenuation(symbol, event):
    # print("[CHRIS_dbg] : showG3HighAttenuation")
    if (event["symbol"].getValue() == "FCC") or (event["symbol"].getValue() == "ARIB"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)
        symbol.setReadOnly(True)
        symbol.setValue(False)
        symbol.setReadOnly(False)

def showG3InternalDriver(symbol, event):
    # print("[CHRIS_dbg] : showG3InternalDriver")
    if (event["symbol"].getValue() == "CEN-B"):
        symbol.setVisible(True)
    else:
        symbol.setReadOnly(True)
        symbol.setValue(False)
        symbol.setReadOnly(False)
        symbol.setVisible(False)

def showG3Multiband(symbol, event):
    # print("[CHRIS_dbg] : showG3Multiband")
    if (event["symbol"].getValue() == "FCC") or (event["symbol"].getValue() == "ARIB"):
        symbol.setVisible(True)
        updateBinFiles()
    else:
        symbol.setVisible(False)
        symbol.setReadOnly(True)
        symbol.setValue(False)
        symbol.setReadOnly(False)

def showG3AuxBand(symbol, event):
    # print("[CHRIS_dbg] : showG3AuxBand")
    if (event["value"] == True):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def showPRIMEHighAttenuation(symbol, event):
    # print("[CHRIS_dbg] : showPRIMEHighAttenuation")
    if (event["symbol"].getValue() == "CH1"):
        symbol.setVisible(False)
        symbol.setReadOnly(True)
        symbol.setValue(False)
        symbol.setReadOnly(False)
    else:
        symbol.setVisible(True)

def showPRIMEMultiband(symbol, event):
    # print("[CHRIS_dbg] : showPRIMEMultiband")
    if (event["symbol"].getValue() == "CH1"):
        symbol.setVisible(False)
        symbol.setReadOnly(True)
        symbol.setValue(False)
        symbol.setReadOnly(False)
    else:
        symbol.setVisible(True)

def showPRIMEAuxBand(symbol, event):
    # print("[CHRIS_dbg] : showPRIMEAuxBand")
    if (event["value"] == True):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)    

def updateShowCoupSettings(symbol, event):
    # print("[CHRIS_dbg] : updateShowCoupSettings")
    global plcCoupPRIMESettings
    global plcCoupG3Settings

    if (Database.getSymbolValue("drvPlc", "DRV_PLC_PROFILE") == "G3") :
        plcCoupG3Settings.setVisible(True)
        plcCoupPRIMESettings.setVisible(False)
    else :
        plcCoupG3Settings.setVisible(False)
        plcCoupPRIMESettings.setVisible(True)

    updateBinFiles()

    
def instantiateComponent(plcComponent):  
    global isDMAPresent

    res = Database.activateComponents(["HarmonyCore"])

    # Enable "Generate Harmony Driver Common Files" option in MHC
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":True})

    # Enable "Generate Harmony System Service Common Files" option in MHC
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":True})

    # Enable "Generate Harmony System DMA Files" option in MHC
    if Database.getSymbolValue("core", "DMA_ENABLE") != None:
        Database.sendMessage("HarmonyCore", "ENABLE_SYS_DMA", {"isEnabled":True})

    if Database.getSymbolValue("core", "DMA_ENABLE") == None:
        isDMAPresent = False
        # print("[CHRIS_dbg] : DMA is NOT present")
    else:
        isDMAPresent = True
        # print("[CHRIS_dbg] : DMA is present")

    plcDriverMode = plcComponent.createComboSymbol("DRV_PLC_MODE", None, ["PLC", "PL460"])
    plcDriverMode.setLabel("PLC Driver Mode")
    plcDriverMode.setDefaultValue("PL460")

    global plcBandInUse
    plcBandInUse = plcComponent.createIntegerSymbol("DRV_PLC_PLC_PROFILE", None)
    plcBandInUse.setLabel("PLC Band in use")
    plcBandInUse.setDefaultValue(PLC_PROFILE_G3_CEN_A)
    # plcBandInUse.setVisible(True)
    # plcBandInUse.setReadOnly(True)

    plcPLIB = plcComponent.createStringSymbol("DRV_PLC_PLIB", None)
    plcPLIB.setLabel("PLIB Used")
    plcPLIB.setReadOnly(True)

    plcExtIntPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_EXT_INT_PIN", None)
    plcExtIntPin.setLabel("External Interrupt Pin")
    plcExtIntPin.setDefaultValue(104) #PD28
    plcExtIntPin.setOutputMode("Key")
    plcExtIntPin.setDisplayMode("Description")

    plcExtIntSource = plcComponent.createStringSymbol("DRV_PLC_EXT_INT_SRC", None)
    plcExtIntSource.setLabel("External Interrupt Source")
    plcExtIntSource.setDefaultValue("PIOD_IRQn") #PIOD_IRQn
    plcExtIntSource.setVisible(True)
    plcExtIntSource.setReadOnly(True)
    plcExtIntSource.setDependencies(plcExternalInterruptTrigger, ["DRV_PLC_EXT_INT_PIN"])

    plcResetPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_RESET_PIN", None)
    plcResetPin.setLabel("Reset Pin")
    plcResetPin.setDefaultValue(34) #PB2
    plcResetPin.setOutputMode("Key")
    plcResetPin.setDisplayMode("Description")

    plcLDOEnPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_LDO_EN_PIN", None)
    plcLDOEnPin.setLabel("LDO Enable Pin")
    plcLDOEnPin.setDefaultValue(35) #PB3
    plcLDOEnPin.setOutputMode("Key")
    plcLDOEnPin.setDisplayMode("Description")

    plcStbyPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_STBY_PIN", None)
    plcStbyPin.setLabel("Stand By Pin")
    plcStbyPin.setDefaultValue(4) #PA3
    plcStbyPin.setOutputMode("Key")
    plcStbyPin.setDisplayMode("Description")
    plcStbyPin.setDependencies(showPL460Mode, ["DRV_PLC_MODE"])

    plcThMonPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_THMON_PIN", None)
    plcThMonPin.setLabel("Thermal Monitor Pin")
    plcThMonPin.setDefaultValue(4) #PA3
    plcThMonPin.setOutputMode("Key")
    plcThMonPin.setDisplayMode("Description")
    plcThMonPin.setDependencies(showPL460Mode, ["DRV_PLC_MODE"])

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
        plcStbyPin.addKey(key, value, description)
        plcThMonPin.addKey(key, value, description)

    plcSymPinConfigComment = plcComponent.createCommentSymbol("DRV_PLC_PINS_CONFIG_COMMENT", None)
    plcSymPinConfigComment.setVisible(True)
    plcSymPinConfigComment.setLabel("***Above selected pins must be configured as GPIO Output in Pin Manager***")
    
    ##### Do not modify below symbol names as they are used by Memory Driver #####

    global plcTXRXDMA
    plcTXRXDMA = plcComponent.createBooleanSymbol("DRV_PLC_TX_RX_DMA", None)
    plcTXRXDMA.setLabel("Use DMA for Transmit and Receive?")
    plcTXRXDMA.setDefaultValue(1)
    plcTXRXDMA.setVisible(True) #### Change to hide it
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

    plcProfile = plcComponent.createComboSymbol("DRV_PLC_PROFILE", None, ["G3", "PRIME"])
    plcProfile.setLabel("PLC Profile")
    plcProfile.setDefaultValue("G3")

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
    
    ##### Coupling Settings : G3  ####################################################
    global plcCoupG3Settings
    plcCoupG3Settings = plcComponent.createMenuSymbol("DRV_PLC_COUP_G3_SETTING", None)
    plcCoupG3Settings.setLabel("PLC Coupling Settings")
    plcCoupG3Settings.setDescription("Coupling Settings")
    plcCoupG3Settings.setVisible(True)

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
    plcG3BandAux.setLabel("G3 Phy Second Band")
    plcG3BandAux.setDefaultValue("CEN-A")
    plcG3BandAux.setVisible(False)
    plcG3BandAux.setDependencies(showG3AuxBand, ["DRV_PLC_COUP_G3_MULTIBAND"])

    plcG3Band = plcComponent.createComboSymbol("DRV_PLC_G3_BAND", plcCoupG3Settings, ["CEN-A", "CEN-B", "FCC", "ARIB"])
    plcG3Band.setLabel("G3 Phy Band")
    plcG3Band.setDefaultValue("CEN-A")

    plcCoupG3HighAttenuation = plcComponent.createBooleanSymbol("DRV_PLC_G3_HIGH_ATTENUATION", plcG3Band)
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

    updBinFilesCtrl = plcComponent.createBooleanSymbol("DRV_PLC_UPD_BIN_FILES", None)
    updBinFilesCtrl.setLabel("updBinFilesCtrl")
    updBinFilesCtrl.setDescription("Auxiliary control")
    updBinFilesCtrl.setVisible(False)
    updBinFilesCtrl.setDefaultValue(False)
    updBinFilesCtrl.setDependencies(updateShowCoupSettings, ["DRV_PLC_PROFILE"])

    ############################################################################
    #### Code Generation ####
    ############################################################################

    configName = Variables.get("__CONFIGURATION_NAME")

    plcAsmPathSetting = plcComponent.createSettingSymbol("DRV_PLC_ASM_PATH_SETTING", None)
    plcAsmPathSetting.setCategory("C32-AS")
    plcAsmPathSetting.setKey("extra-include-directories-for-assembler")
    plcAsmPathSetting.setValue("../src/config/" + configName + "/driver/plc/bin")
    plcAsmPathSetting.setAppend(True, ";")

    plcHeaderFile = plcComponent.createFileSymbol("PLC_HEADER", None)
    plcHeaderFile.setSourcePath("driver/plc/drv_plc.h")
    plcHeaderFile.setOutputName("drv_plc.h")
    plcHeaderFile.setDestPath("driver/plc/")
    plcHeaderFile.setProjectPath("config/" + configName + "/driver/plc/")
    plcHeaderFile.setType("HEADER")

    plcSymHeaderDefFile = plcComponent.createFileSymbol("DRV_PLC_DEF", None)
    plcSymHeaderDefFile.setSourcePath("driver/plc/drv_plc_definitions.h")
    plcSymHeaderDefFile.setOutputName("drv_plc_definitions.h")
    plcSymHeaderDefFile.setDestPath("driver/plc")
    plcSymHeaderDefFile.setProjectPath("config/" + configName + "/driver/plc/")
    plcSymHeaderDefFile.setType("HEADER")

    global plcProfileFile
    plcProfileFile = plcComponent.createFileSymbol("DRV_PLC_PROFILE", None)
    plcProfileFile.setSourcePath("driver/plc/src/drv_plc_g3.c")
    plcProfileFile.setOutputName("drv_plc_comm.c")
    plcProfileFile.setDestPath("driver/plc")
    plcProfileFile.setProjectPath("config/" + configName + "/driver/plc/")
    plcProfileFile.setType("SOURCE")
    plcProfileFile.setDependencies(setPlcProfile, ["DRV_PLC_PROFILE"])

    global plcProfileDefFile
    plcProfileDefFile = plcComponent.createFileSymbol("DRV_PLC_PROFILE_DEF", None)
    plcProfileDefFile.setSourcePath("driver/plc/drv_plc_g3.h")
    plcProfileDefFile.setOutputName("drv_plc_comm.h")
    plcProfileDefFile.setDestPath("driver/plc")
    plcProfileDefFile.setProjectPath("config/" + configName + "/driver/plc/")
    plcProfileDefFile.setType("HEADER")

    plcSourceFile = plcComponent.createFileSymbol("PLC_SOURCE", None)
    plcSourceFile.setSourcePath("driver/plc/src/drv_plc.c")
    plcSourceFile.setOutputName("drv_plc.c")
    plcSourceFile.setDestPath("driver/plc/")
    plcSourceFile.setProjectPath("config/" + configName + "/driver/plc/")
    plcSourceFile.setType("SOURCE")

    global plcSourceBinFileG3CENA
    plcSourceBinFileG3CENA = plcComponent.createFileSymbol("PLC_SOURCE_BIN_G3_CENA", None)
    plcSourceBinFileG3CENA.setSourcePath("driver/plc/src/bin/ATPLCB_G3_CENA.bin")
    plcSourceBinFileG3CENA.setOutputName("PLC_G3_CENA.bin")
    plcSourceBinFileG3CENA.setDestPath("driver/plc/bin/")
    plcSourceBinFileG3CENA.setProjectPath("config/" + configName + "/driver/plc/bin/")
    plcSourceBinFileG3CENA.setType("SOURCE")
    plcSourceBinFileG3CENA.setEnabled(True)
    plcSourceBinFileG3CENA.setVisible(False)

    global plcSourceBinFileG3CENB
    plcSourceBinFileG3CENB = plcComponent.createFileSymbol("PLC_SOURCE_BIN_G3_CENB", None)
    plcSourceBinFileG3CENB.setSourcePath("driver/plc/src/bin/ATPLCB_G3_CENB.bin")
    plcSourceBinFileG3CENB.setOutputName("PLC_G3_CENB.bin")
    plcSourceBinFileG3CENB.setDestPath("driver/plc/bin/")
    plcSourceBinFileG3CENB.setProjectPath("config/" + configName + "/driver/plc/bin/")
    plcSourceBinFileG3CENB.setType("SOURCE")
    plcSourceBinFileG3CENB.setEnabled(False)
    plcSourceBinFileG3CENB.setVisible(False)

    global plcSourceBinFileG3FCC
    plcSourceBinFileG3FCC = plcComponent.createFileSymbol("PLC_SOURCE_BIN_G3_FCC", None)
    plcSourceBinFileG3FCC.setSourcePath("driver/plc/src/bin/ATPLCB_G3_FCC.bin")
    plcSourceBinFileG3FCC.setOutputName("PLC_G3_FCC.bin")
    plcSourceBinFileG3FCC.setDestPath("driver/plc/bin/")
    plcSourceBinFileG3FCC.setProjectPath("config/" + configName + "/driver/plc/bin/")
    plcSourceBinFileG3FCC.setType("SOURCE")
    plcSourceBinFileG3FCC.setEnabled(False)
    plcSourceBinFileG3FCC.setVisible(False)

    global plcSourceBinFileG3ARIB
    plcSourceBinFileG3ARIB = plcComponent.createFileSymbol("PLC_SOURCE_BIN_G3_ARIB", None)
    plcSourceBinFileG3ARIB.setSourcePath("driver/plc/src/bin/ATPLCB_G3_ARIB.bin")
    plcSourceBinFileG3ARIB.setOutputName("PLC_G3_ARIB.bin")
    plcSourceBinFileG3ARIB.setDestPath("driver/plc/bin/")
    plcSourceBinFileG3ARIB.setProjectPath("config/" + configName + "/driver/plc/bin/")
    plcSourceBinFileG3ARIB.setType("SOURCE")
    plcSourceBinFileG3ARIB.setEnabled(False)
    plcSourceBinFileG3ARIB.setVisible(False)

    global plcSourceBinFilePRIME
    plcSourceBinFilePRIME = plcComponent.createFileSymbol("PLC_SOURCE_BIN_PRIME", None)
    plcSourceBinFilePRIME.setSourcePath("driver/plc/src/bin/ATPLCB_PRIME_2CHN.bin")
    plcSourceBinFilePRIME.setOutputName("ATPLCB_PRIME_2CHN.bin")
    plcSourceBinFilePRIME.setDestPath("driver/plc/bin/")
    plcSourceBinFilePRIME.setProjectPath("config/" + configName + "/driver/plc/bin/")
    plcSourceBinFilePRIME.setType("SOURCE")
    plcSourceBinFilePRIME.setEnabled(False)
    plcSourceBinFilePRIME.setVisible(False)

    plcAssemblyBinFile = plcComponent.createFileSymbol("PLC_ASSEMBLY_BIN", None)
    plcAssemblyBinFile.setSourcePath("driver/plc/src/bin/plc_bin.S.ftl")
    plcAssemblyBinFile.setOutputName("plc_bin.S")
    plcAssemblyBinFile.setDestPath("driver/plc/bin/")
    plcAssemblyBinFile.setProjectPath("config/" + configName + "/driver/plc/bin/")
    plcAssemblyBinFile.setType("SOURCE")
    plcAssemblyBinFile.setMarkup(True)

    plcBootFile = plcComponent.createFileSymbol("PLC_BOOT", None)
    plcBootFile.setSourcePath("driver/plc/src/drv_plc_boot.c")
    plcBootFile.setOutputName("drv_plc_boot.c")
    plcBootFile.setDestPath("driver/plc/")
    plcBootFile.setProjectPath("config/" + configName + "/driver/plc/")
    plcBootFile.setType("SOURCE")

    plcBootHeaderFile = plcComponent.createFileSymbol("PLC_BOOT_HEADER", None)
    plcBootHeaderFile.setSourcePath("driver/plc/src/drv_plc_boot.h")
    plcBootHeaderFile.setOutputName("drv_plc_boot.h")
    plcBootHeaderFile.setDestPath("driver/plc/src")
    plcBootHeaderFile.setProjectPath("config/" + configName + "/driver/plc/")
    plcBootHeaderFile.setType("SOURCE")

    plcHalFile = plcComponent.createFileSymbol("PLC_HAL", None)
    plcHalFile.setSourcePath("driver/plc/src/drv_plc_hal.c")
    plcHalFile.setOutputName("drv_plc_hal.c")
    plcHalFile.setDestPath("driver/plc/")
    plcHalFile.setProjectPath("config/" + configName + "/driver/plc/")
    plcHalFile.setType("SOURCE")

    plcHalHeaderFile = plcComponent.createFileSymbol("PLC_HAL_HEADER", None)
    plcHalHeaderFile.setSourcePath("driver/plc/drv_plc_hal.h")
    plcHalHeaderFile.setOutputName("drv_plc_hal.h")
    plcHalHeaderFile.setDestPath("driver/plc/")
    plcHalHeaderFile.setProjectPath("config/" + configName + "/driver/plc/")
    plcHalHeaderFile.setType("HEADER")

    plcHeaderLocalFile = plcComponent.createFileSymbol("DRV_PLC_HEADER_LOCAL", None)
    plcHeaderLocalFile.setSourcePath("driver/plc/src/drv_plc_local.h")
    plcHeaderLocalFile.setOutputName("drv_plc_local.h")
    plcHeaderLocalFile.setDestPath("driver/plc/src")
    plcHeaderLocalFile.setProjectPath("config/" + configName + "/driver/plc/")
    plcHeaderLocalFile.setType("SOURCE")
    plcHeaderLocalFile.setEnabled(True)

    global plcProfileHeaderLocalFile
    plcProfileHeaderLocalFile = plcComponent.createFileSymbol("DRV_PLC_PROFILE_LOCAL", None)
    plcProfileHeaderLocalFile.setSourcePath("driver/plc/src/drv_plc_local_g3.h")
    plcProfileHeaderLocalFile.setOutputName("drv_plc_local_comm.h")
    plcProfileHeaderLocalFile.setDestPath("driver/plc/src")
    plcProfileHeaderLocalFile.setProjectPath("config/" + configName + "/driver/plc/")
    plcProfileHeaderLocalFile.setType("SOURCE")
    plcProfileHeaderLocalFile.setEnabled(True)

    plcSystemDefFile = plcComponent.createFileSymbol("PLC_DEF", None)
    plcSystemDefFile.setType("STRING")
    plcSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    plcSystemDefFile.setSourcePath("driver/plc/templates/system/definitions.h.ftl")
    plcSystemDefFile.setMarkup(True)

    plcSymSystemDefObjFile = plcComponent.createFileSymbol("DRV_PLC_SYSTEM_DEF_OBJECT", None)
    plcSymSystemDefObjFile.setType("STRING")
    plcSymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    plcSymSystemDefObjFile.setSourcePath("driver/plc/templates/system/definitions_objects.h.ftl")
    plcSymSystemDefObjFile.setMarkup(True)

    plcSymSystemConfigFile = plcComponent.createFileSymbol("DRV_PLC_CONFIGIRUTION", None)
    plcSymSystemConfigFile.setType("STRING")
    plcSymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    plcSymSystemConfigFile.setSourcePath("driver/plc/templates/system/configuration.h.ftl")
    plcSymSystemConfigFile.setMarkup(True)

    plcSymSystemInitDataFile = plcComponent.createFileSymbol("DRV_PLC_INIT_DATA", None)
    plcSymSystemInitDataFile.setType("STRING")
    plcSymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    plcSymSystemInitDataFile.setSourcePath("driver/plc/templates/system/initialize_data.c.ftl")
    plcSymSystemInitDataFile.setMarkup(True)

    plcSystemInitFile = plcComponent.createFileSymbol("PLC_INIT", None)
    plcSystemInitFile.setType("STRING")
    plcSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    plcSystemInitFile.setSourcePath("driver/plc/templates/system/initialize.c.ftl")
    plcSystemInitFile.setMarkup(True)

    plcSystemTasksFile = plcComponent.createFileSymbol("DRV_PLC_SYS_TASK", None)
    plcSystemTasksFile.setType("STRING")
    plcSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    plcSystemTasksFile.setSourcePath("driver/plc/templates/system/system_tasks.c.ftl")
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

    if connectID == "drv_plc_phy_SPI_dependency" :
        # print("[CHIRS_dbg] : drv_plc_phy_SPI_dependency")
        plibUsed = localComponent.getSymbolByID("DRV_PLC_PLIB")
        plibUsed.clearValue()
        plibUsed.setValue(remoteID.upper())

        # Set SPI baudrate
        plibBaudrate = remoteComponent.getSymbolByID("SPI_BAUD_RATE")
        plibBaudrate.clearValue()
        plibBaudrate.setValue(8000000)
        # print("[CHIRS_dbg] : Set SPI baudrate: 8000000")

        dmaChannelSym = Database.getSymbolValue("core", "DMA_CH_FOR_" + remoteID.upper() + "_Transmit")
        dmaRequestSym = Database.getSymbolValue("core", "DMA_CH_NEEDED_FOR_" + remoteID.upper() + "_Transmit")

        # Do not change the order as DMA Channels needs to be allocated
        # after setting the plibUsed symbol
        # Both device and connected plib should support DMA
        if isDMAPresent == True and dmaChannelSym != None and dmaRequestSym != None:
            localComponent.getSymbolByID("DRV_PLC_DEPENDENCY_DMA_COMMENT").setVisible(False)
            localComponent.getSymbolByID("DRV_PLC_TX_RX_DMA").setValue(1)
        else:
            # print("[CHIRS_dbg] : DMA ERROR")
            localComponent.getSymbolByID("DRV_PLC_TX_DMA_CH_COMMENT").setVisible(True)
            localComponent.getSymbolByID("DRV_PLC_RX_DMA_CH_COMMENT").setVisible(True)

  
def onAttachmentDisconnected(source, target):
    global isDMAPresent

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    # print("[CHIRS_dbg] : onAttachmentDisconnected event - remoteID: " + remoteID.upper())

    if connectID == "drv_plc_phy_SPI_dependency" :
        # print("[CHIRS_dbg] : drv_plc_phy_SPI_dependency")

        dmaChannelSym = Database.getSymbolValue("core", "DMA_CH_FOR_" + remoteID.upper() + "_Transmit")
        dmaRequestSym = Database.getSymbolValue("core", "DMA_CH_NEEDED_FOR_" + remoteID.upper() + "_Transmit")

        dummyDict = {}
        dummyDict = Database.sendMessage(remoteID, "SPI_MASTER_MODE", {"isReadOnly":False})
        dummyDict = Database.sendMessage(remoteID, "SPI_MASTER_INTERRUPT_MODE", {"isReadOnly":False})

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

        localComponent.getSymbolByID("DRV_PLC_PLIB").clearValue()

def requestAndAssignTxDMAChannel(symbol, event):
    global spiTXDMAChannelComment

    spiPeripheral = Database.getSymbolValue("drvPlc", "DRV_PLC_PLIB")

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
    global spiRXDMAChannelComment

    spiPeripheral = Database.getSymbolValue("drvPlc", "DRV_PLC_PLIB")

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

def destroyComponent(plcComponent):
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_PORTS", {"isEnabled":False})

    if Database.getSymbolValue("core", "DMA_ENABLE") != None:
        Database.sendMessage("HarmonyCore", "ENABLE_SYS_DMA", {"isEnabled":False})
