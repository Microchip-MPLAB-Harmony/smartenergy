# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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
srv_met_helpkeyword = "mcc_h3_metrology_driver_configuration"

def showWaveformCapture(symbol, event):
    symbol.setVisible(event["value"])

def setWaveformCaptureSize(symbol, event):
    global srvMetCaptureNumChannels
    global srvMetCaptureBufSizeChn

    symbol.setValue(srvMetCaptureNumChannels.getValue() * srvMetCaptureBufSizeChn.getValue())


def instantiateComponent(metComponentCommon):
    
    Log.writeInfoMessage("Loading Metrology Driver")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")
    
    srvMetRegBaseAddress = metComponentCommon.createHexSymbol("DRV_MET_BASE_ADDRESS", None)
    srvMetRegBaseAddress.setLabel("Register Base Address")
    srvMetRegBaseAddress.setVisible(True)
    srvMetRegBaseAddress.setDefaultValue(0x20088000)
    srvMetRegBaseAddress.setReadOnly(True)
    srvMetRegBaseAddress.setHelp(srv_met_helpkeyword)
    
    srvMetWaveformCapture = metComponentCommon.createBooleanSymbol("DRV_MET_WAVEFORM_CAPTURE", None)
    srvMetWaveformCapture.setLabel("Waveform Capture")
    srvMetWaveformCapture.setDefaultValue(False)
    srvMetWaveformCapture.setHelp(srv_met_helpkeyword)

    global srvMetCaptureNumChannels
    srvMetCaptureNumChannels = metComponentCommon.createIntegerSymbol("DRV_MET_CAPTURE_NUM_CHANNELS", srvMetWaveformCapture)
    srvMetCaptureNumChannels.setLabel("Capture Num Channels")
    srvMetCaptureNumChannels.setVisible(False)
    srvMetCaptureNumChannels.setDefaultValue(1)
    srvMetCaptureNumChannels.setMin(1)
    srvMetCaptureNumChannels.setMax(6)
    srvMetCaptureNumChannels.setDependencies(showWaveformCapture, ["DRV_MET_WAVEFORM_CAPTURE"])
    srvMetCaptureNumChannels.setHelp(srv_met_helpkeyword)

    global srvMetCaptureBufSizeChn
    srvMetCaptureBufSizeChn = metComponentCommon.createIntegerSymbol("DRV_MET_CAPTURE_BUF_SIZE_CHN", srvMetWaveformCapture)
    srvMetCaptureBufSizeChn.setLabel("Capture Size per Channel")
    srvMetCaptureBufSizeChn.setVisible(False)
    srvMetCaptureBufSizeChn.setDefaultValue(8000)
    srvMetCaptureBufSizeChn.setDependencies(showWaveformCapture, ["DRV_MET_WAVEFORM_CAPTURE"])
    srvMetCaptureBufSizeChn.setHelp(srv_met_helpkeyword)

    srvMetCaptureBufSize = metComponentCommon.createIntegerSymbol("DRV_MET_CAPTURE_BUF_SIZE", None)
    srvMetCaptureBufSize.setLabel("Capture Size per Channel")
    srvMetCaptureBufSize.setVisible(False)
    srvMetCaptureBufSize.setDefaultValue(8000)
    srvMetCaptureBufSize.setDependencies(setWaveformCaptureSize, ["DRV_MET_CAPTURE_NUM_CHANNELS", "DRV_MET_CAPTURE_BUF_SIZE_CHN"])

    srvMetSourceFile = metComponentCommon.createFileSymbol("DRV_MET_SOURCE", None)
    srvMetSourceFile.setSourcePath("driver/metrology/drv_metrology.c.ftl")
    srvMetSourceFile.setOutputName("drv_metrology.c")
    srvMetSourceFile.setDestPath("driver/metrology")
    srvMetSourceFile.setProjectPath("config/" + configName + "/driver/metrology/")
    srvMetSourceFile.setType("SOURCE")
    srvMetSourceFile.setOverwrite(True)
    srvMetSourceFile.setMarkup(True)

    srvMetHeaderFile = metComponentCommon.createFileSymbol("DRV_MET_HEADER", None)
    srvMetHeaderFile.setSourcePath("driver/metrology/drv_metrology.h.ftl")
    srvMetHeaderFile.setOutputName("drv_metrology.h")
    srvMetHeaderFile.setDestPath("driver/metrology")
    srvMetHeaderFile.setProjectPath("config/" + configName + "/driver/metrology/")
    srvMetHeaderFile.setType("HEADER")
    srvMetHeaderFile.setOverwrite(True)
    srvMetHeaderFile.setMarkup(True)

    srvMetHeaderFileDefs = metComponentCommon.createFileSymbol("DRV_MET_HEADER_DEFINITIONS", None)
    srvMetHeaderFileDefs.setSourcePath("driver/metrology/drv_metrology_definitions.h")
    srvMetHeaderFileDefs.setOutputName("drv_metrology_definitions.h")
    srvMetHeaderFileDefs.setDestPath("driver/metrology")
    srvMetHeaderFileDefs.setProjectPath("config/" + configName + "/driver/metrology/")
    srvMetHeaderFileDefs.setType("HEADER")
    srvMetHeaderFileDefs.setOverwrite(True)

    srvMetHeaderRegsFile = metComponentCommon.createFileSymbol("DRV_MET_HEADER_REGS", None)
    srvMetHeaderRegsFile.setSourcePath("driver/metrology/drv_metrology_regs.h")
    srvMetHeaderRegsFile.setOutputName("drv_metrology_regs.h")
    srvMetHeaderRegsFile.setDestPath("driver/metrology")
    srvMetHeaderRegsFile.setProjectPath("config/" + configName + "/driver/metrology/")
    srvMetHeaderRegsFile.setType("HEADER")
    srvMetHeaderRegsFile.setOverwrite(True)
    
    #### ASM Path Settings #####################################################

    srvMetAsmPathSetting = metComponentCommon.createSettingSymbol("DRV_MET_ASM_PATH_SETTING", None)
    srvMetAsmPathSetting.setCategory("C32-AS")
    srvMetAsmPathSetting.setKey("extra-include-directories-for-assembler")
    srvMetAsmPathSetting.setValue("../src/config/" + configName + "/driver/metrology/bin")
    srvMetAsmPathSetting.setAppend(False, None)

    #### Binary MET Files ######################################################

    srvMetBinFile = metComponentCommon.createLibrarySymbol("DRV_MET_BIN", None)
    srvMetBinFile.setSourcePath("driver/metrology/bin/core1_metlib.bin")
    srvMetBinFile.setOutputName("core1_metlib.bin")
    srvMetBinFile.setDestPath("driver/metrology/bin")

    srvMetAssemblyBinFile = metComponentCommon.createFileSymbol("DRV_MET_ASSEMBLY_BIN", None)
    srvMetAssemblyBinFile.setSourcePath("driver/metrology/bin/met_bin.S")
    srvMetAssemblyBinFile.setOutputName("met_bin.S")
    srvMetAssemblyBinFile.setDestPath("driver/metrology/bin")
    srvMetAssemblyBinFile.setProjectPath("config/" + configName + "/driver/metrology/bin")
    srvMetAssemblyBinFile.setType("SOURCE")

    #### FreeMaker System Files ######################################################

    srvMetSystemDefFile = metComponentCommon.createFileSymbol("DRV_MET_SYSTEM_DEF", None)
    srvMetSystemDefFile.setType("STRING")
    srvMetSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    srvMetSystemDefFile.setSourcePath("driver/metrology/templates/system/definitions.h.ftl")
    srvMetSystemDefFile.setMarkup(True)

    srvMetSystemConfigFile = metComponentCommon.createFileSymbol("DRV_MET_CONFIGURATION", None)
    srvMetSystemConfigFile.setType("STRING")
    srvMetSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    srvMetSystemConfigFile.setSourcePath("driver/metrology/templates/system/configuration.h.ftl")
    srvMetSystemConfigFile.setMarkup(True)

    srvMetSystemInitDataFile = metComponentCommon.createFileSymbol("DRV_MET_INIT_DATA", None)
    srvMetSystemInitDataFile.setType("STRING")
    srvMetSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    srvMetSystemInitDataFile.setSourcePath("driver/metrology/templates/system/initialize_data.c.ftl")
    srvMetSystemInitDataFile.setMarkup(True)

    srvMetSystemInitFile = metComponentCommon.createFileSymbol("DRV_MET_INIT", None)
    srvMetSystemInitFile.setType("STRING")
    srvMetSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    srvMetSystemInitFile.setSourcePath("driver/metrology/templates/system/initialize.c.ftl")
    srvMetSystemInitFile.setMarkup(True)
