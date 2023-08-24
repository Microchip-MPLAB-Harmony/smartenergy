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
srv_met_helpkeyword = "mcc_h3_drv_metrology_config"

GAIN_P_K_T_Q = 24
GAIN_VI_Q = 10

def _get_KI():
    freq = Database.getSymbolValue("drvMet", "DRV_MET_CONF_F")
    st = Database.getSymbolValue("drvMet", "DRV_MET_CONF_ST")
    tr = Database.getSymbolValue("drvMet", "DRV_MET_CONF_TR")
    gain = 2**Database.getSymbolValue("drvMet", "DRV_MET_CONF_GAIN")
    rl = Database.getSymbolValue("drvMet", "DRV_MET_CONF_RL")
    
    if (st == 0): 
        # Current Transformer
        rl = float(rl) / 1000000
        m = float(tr) / (rl * gain)
    elif (st == 1): 
        rl = float(rl) / 1000000
        # Shunt Resistor
        m = 1 / float(gain * rl)
    elif (st == 2): 
        # Rogowski Coil
        Ksf = float(1000000) / tr
        Kdi = float(60) / freq
        m = Ksf / (gain * Kdi)

    return m

def updateTRLabel(symbol, event):
    if (event["value"]) == 2:
        # Rogowski Coil
        symbol.setLabel("Current Sensitivity (uV/A)")
    else:
        symbol.setLabel("Transformer Ratio")

def updateRLLabel(symbol, event):
    if (event["value"]) == 1:
        symbol.setLabel("Shunt Resistor (uOhms)")
    else:
        symbol.setLabel("Resistor Load (uOhms)")

def updateConfigWaveform(symbol, event):
    global drvMetRegWaveformCaptureSize
    
    enable = Database.getSymbolValue("drvMet", "DRV_MET_WAVEFORM_CAPTURE")
    ia = Database.getSymbolValue("drvMet", "DRV_MET_WAVECAPT_CH_IA")
    va = Database.getSymbolValue("drvMet", "DRV_MET_WAVECAPT_CH_VA")
    ib = Database.getSymbolValue("drvMet", "DRV_MET_WAVECAPT_CH_IB")
    vb = Database.getSymbolValue("drvMet", "DRV_MET_WAVECAPT_CH_VB")
    ic = Database.getSymbolValue("drvMet", "DRV_MET_WAVECAPT_CH_IC")
    vc = Database.getSymbolValue("drvMet", "DRV_MET_WAVECAPT_CH_VC")
    src = Database.getSymbolValue("drvMet", "DRV_MET_WAVECAPT_SRC")
    type = Database.getSymbolValue("drvMet", "DRV_MET_WAVECAPT_TYPE")

    reg = (vc << 13) | (ic << 12) | (vb << 11) | (ib << 10) | (va << 9) | (ia << 8) | (src << 4) | type
    symbol.setValue(reg)        

    if (enable):
        bufferSize = Database.getSymbolValue("drvMet", "DRV_MET_CAPTURE_BUF_SIZE_CHN")
        channels_sel = (reg >> 8) & 0x3F
        num_chn = 0
        for ch_sel in range(0, 6):
            if (channels_sel & 0x01) == 0x01:
                num_chn = num_chn + 1
            
            channels_sel = channels_sel >> 1
    else:
        bufferSize = 0
        num_chn = 0

    drvMetRegWaveformCaptureSize.setValue(bufferSize * num_chn)
        
def updateConfigPKT(symbol, event):
    m = 1000000000.0 / int(event["value"])
    m *= 2**GAIN_P_K_T_Q
    m /= 1000000
    symbol.setValue(int(m))

def updateConfigMT(symbol, event):
    stValue = event["value"] & 0x03
    reg = symbol.getValue()
    reg = reg & 0xFFFFFCCC
    reg = reg | (stValue << 8) | (stValue << 4) | (stValue)
    symbol.setValue(reg)

def updateConfigATS2023(symbol, event):
    gainValue = Database.getSymbolValue("drvMet", "DRV_MET_CONF_GAIN")
    i0 = Database.getSymbolValue("drvMet", "DRV_MET_CONF_I0")
    i1 = Database.getSymbolValue("drvMet", "DRV_MET_CONF_I1")
    v1 = Database.getSymbolValue("drvMet", "DRV_MET_CONF_V1")
    i2 = Database.getSymbolValue("drvMet", "DRV_MET_CONF_I2")

    reg = (gainValue << 28) | (gainValue << 12) | (gainValue << 4) | (i2 << 24) | (v1 << 16) | (i1 << 8) | (1 << 1) | i0
    symbol.setValue(reg)

def updateConfigATS2427(symbol, event):
    gainValue = Database.getSymbolValue("drvMet", "DRV_MET_CONF_GAIN")
    v2 = Database.getSymbolValue("drvMet", "DRV_MET_CONF_V2")
    i3 = Database.getSymbolValue("drvMet", "DRV_MET_CONF_I3")
    v3 = Database.getSymbolValue("drvMet", "DRV_MET_CONF_V3")
    
    reg = symbol.getValue()
    reg = reg & 0xFF000000
    reg = reg | (gainValue << 12) | (v3 << 16) | (i3 << 8) | v2
    symbol.setValue(reg)

def updateConfigKI(symbol, event):
    m = _get_KI()
    m = int(m * 2**GAIN_VI_Q)
    symbol.setValue(m)

def updateConfigKV(symbol, event):
    i = (event["value"] * 2**GAIN_VI_Q)
    symbol.setValue(i)

def updateConfigSwellSag(symbol, event):
    ku = Database.getSymbolValue("drvMet", "DRV_MET_CONF_KU")
    threshold = Database.getSymbolValue("drvMet", "DRV_MET_CONF_SWELL")
    m = (threshold/float(ku))**2
    m = int(m * 2**32)
    symbol.setValue(m)

def updateConfigCreepPQ(symbol, event):
    freq = Database.getSymbolValue("drvMet", "DRV_MET_CONF_F")
    creep = Database.getSymbolValue("drvMet", "DRV_MET_CONF_CREEP_P")
    m = float(creep) / (freq * 3600)
    m = int(m * 2**30)
    symbol.setValue(m)

def updateConfigCreepI(symbol, event):
    K_Ix = _get_KI()
    creep = Database.getSymbolValue("drvMet", "DRV_MET_CONF_CREEP_I")
    m = creep / K_Ix
    m = int(m * 2**20)
    symbol.setValue(m)

def updateConfigFeatCtrl0(symbol, event):
    pA = Database.getSymbolValue("drvMet", "DRV_MET_CONF_I1") or Database.getSymbolValue("drvMet", "DRV_MET_CONF_V1")
    pB = Database.getSymbolValue("drvMet", "DRV_MET_CONF_I2") or Database.getSymbolValue("drvMet", "DRV_MET_CONF_V2")
    pC = Database.getSymbolValue("drvMet", "DRV_MET_CONF_I3") or Database.getSymbolValue("drvMet", "DRV_MET_CONF_V3")
    reg = symbol.getValue()
    reg = reg & 0xFFFFF8FF
    reg = reg | (pC << 10) | (pB << 9) | (pA << 8)
    symbol.setValue(reg)

def updateConfigFeatCtrl1(symbol, event):
    pEn = Database.getSymbolValue("drvMet", "DRV_MET_CONF_CREEP_P_EN")
    qEn = Database.getSymbolValue("drvMet", "DRV_MET_CONF_CREEP_Q_EN")
    iEn = Database.getSymbolValue("drvMet", "DRV_MET_CONF_CREEP_I_EN")
    reg = symbol.getValue()
    reg = reg & 0xFFFFFFF8
    reg = reg | (pEn << 2) | (qEn << 1) | iEn
    symbol.setValue(reg)

def updateConfigPulseXCtrl(symbol, event):
    pulseId = event["id"]
    pulseId = pulseId[-1]
    pEn = Database.getSymbolValue("drvMet", "DRV_MET_CONF_EN_P" + str(pulseId))
    pDet = Database.getSymbolValue("drvMet", "DRV_MET_CONF_DET_P" + str(pulseId))
    pPol = Database.getSymbolValue("drvMet", "DRV_MET_CONF_POL_P" + str(pulseId))
    pTyp = Database.getSymbolValue("drvMet", "DRV_MET_CONF_TYP_P" + str(pulseId))
    pWid = Database.getSymbolValue("drvMet", "DRV_MET_CONF_WID_P" + str(pulseId))
    reg = (pEn << 31) | (pDet << 28) | (pPol << 24) | (pTyp << 16) | pWid
    symbol.setValue(reg)

def showSymbolOnBoolEvent(symbol, event):
    symbol.setVisible(event["value"])

def setWaveformCaptureSize(symbol, event):
    global drvMetCaptureNumChannels
    global drvMetCaptureBufSizeChn

    symbol.setValue(drvMetCaptureNumChannels.getValue() * drvMetCaptureBufSizeChn.getValue())

def updateTaskDelayVisiblity(symbol, event):
    symbol.setVisible(event["value"])

def showRTOSMenu(symbol, event):
    show_rtos_menu = False

    if (Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"):
        show_rtos_menu = True

    symbol.setVisible(show_rtos_menu)

def enableRTOS(symbol, event):
    enable_rtos = False

    if (Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"):
        enable_rtos = True

    symbol.setValue(enable_rtos)

def genRtosTask(symbol, event):
    gen_rtos_task = False
    component = symbol.getComponent()

    if (Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"):
        gen_rtos_task = True

    symbol.setEnabled(gen_rtos_task)

def commandRtosMicriumOSIIIAppTaskVisibility(symbol, event):
    if (event["value"] == "MicriumOSIII"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def commandRtosMicriumOSIIITaskOptVisibility(symbol, event):
    symbol.setVisible(event["value"])

def commandSendMessageHeapSize(symbol, event):
    dummyDict = {}
    dummyDict = Database.sendMessage("core", "HEAP_SIZE", {"heap_size" : 1024})  

def getActiveRtos():
    activeComponents = Database.getActiveComponentIDs()

    for i in range(0, len(activeComponents)):
        if (activeComponents[i] == "FreeRTOS"):
            return "FreeRTOS"
        elif (activeComponents[i] == "ThreadX"):
            return "ThreadX"
        elif (activeComponents[i] == "MicriumOSIII"):
            return "MicriumOSIII"
        elif (activeComponents[i] == "MbedOS"):
            return "MbedOS"      

def instantiateComponent(metComponentCommon):
    
    Log.writeInfoMessage("Loading Metrology Driver")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    if ("MTC" in str(Variables.get("__PROCESSOR"))):
        enablePhase3 = 1
        featCtrl0Value = 0x00000700
        at2427Value = 0x07010101
    else:
        enablePhase3 = 0
        featCtrl0Value = 0x00000300
        at2427Value = 0x07000001
    
    drvMetRegBaseAddress = metComponentCommon.createHexSymbol("DRV_MET_BASE_ADDRESS", None)
    drvMetRegBaseAddress.setLabel("Register Base Address")
    drvMetRegBaseAddress.setVisible(True)
    drvMetRegBaseAddress.setDefaultValue(0x20088000)
    drvMetRegBaseAddress.setReadOnly(True)
    drvMetRegBaseAddress.setHelp(srv_met_helpkeyword)
    
    drvMetWaveformCapture = metComponentCommon.createBooleanSymbol("DRV_MET_WAVEFORM_CAPTURE", None)
    drvMetWaveformCapture.setLabel("Waveform Capture")
    drvMetWaveformCapture.setDefaultValue(False)
    drvMetWaveformCapture.setHelp(srv_met_helpkeyword)
    
    drvMetWaveformCaptureChIA = metComponentCommon.createBooleanSymbol("DRV_MET_WAVECAPT_CH_IA", drvMetWaveformCapture)
    drvMetWaveformCaptureChIA.setLabel("Capture Channel IA")
    drvMetWaveformCaptureChIA.setDefaultValue(False)
    drvMetWaveformCaptureChIA.setVisible(False)
    drvMetWaveformCaptureChIA.setHelp(srv_met_helpkeyword)
    drvMetWaveformCaptureChIA.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_WAVEFORM_CAPTURE"])
    
    drvMetWaveformCaptureChVA = metComponentCommon.createBooleanSymbol("DRV_MET_WAVECAPT_CH_VA", drvMetWaveformCapture)
    drvMetWaveformCaptureChVA.setLabel("Capture Channel VA")
    drvMetWaveformCaptureChVA.setDefaultValue(False)
    drvMetWaveformCaptureChVA.setVisible(False)
    drvMetWaveformCaptureChVA.setHelp(srv_met_helpkeyword)
    drvMetWaveformCaptureChVA.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_WAVEFORM_CAPTURE"])
    
    drvMetWaveformCaptureChIB = metComponentCommon.createBooleanSymbol("DRV_MET_WAVECAPT_CH_IB", drvMetWaveformCapture)
    drvMetWaveformCaptureChIB.setLabel("Capture Channel IB")
    drvMetWaveformCaptureChIB.setDefaultValue(False)
    drvMetWaveformCaptureChIB.setVisible(False)
    drvMetWaveformCaptureChIB.setHelp(srv_met_helpkeyword)
    drvMetWaveformCaptureChIB.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_WAVEFORM_CAPTURE"])
    
    drvMetWaveformCaptureChVB = metComponentCommon.createBooleanSymbol("DRV_MET_WAVECAPT_CH_VB", drvMetWaveformCapture)
    drvMetWaveformCaptureChVB.setLabel("Capture Channel VB")
    drvMetWaveformCaptureChVB.setDefaultValue(False)
    drvMetWaveformCaptureChVB.setVisible(False)
    drvMetWaveformCaptureChVB.setHelp(srv_met_helpkeyword)
    drvMetWaveformCaptureChVB.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_WAVEFORM_CAPTURE"])
    
    drvMetWaveformCaptureChIC = metComponentCommon.createBooleanSymbol("DRV_MET_WAVECAPT_CH_IC", drvMetWaveformCapture)
    drvMetWaveformCaptureChIC.setLabel("Capture Channel IC")
    drvMetWaveformCaptureChIC.setDefaultValue(False)
    drvMetWaveformCaptureChIC.setVisible(False)
    drvMetWaveformCaptureChIC.setHelp(srv_met_helpkeyword)
    drvMetWaveformCaptureChIC.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_WAVEFORM_CAPTURE"])
    
    drvMetWaveformCaptureChVC = metComponentCommon.createBooleanSymbol("DRV_MET_WAVECAPT_CH_VC", drvMetWaveformCapture)
    drvMetWaveformCaptureChVC.setLabel("Capture Channel VC")
    drvMetWaveformCaptureChVC.setDefaultValue(False)
    drvMetWaveformCaptureChVC.setVisible(False)
    drvMetWaveformCaptureChVC.setHelp(srv_met_helpkeyword)
    drvMetWaveformCaptureChVC.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_WAVEFORM_CAPTURE"])

    drvMetWaveformCaptureSrc = metComponentCommon.createKeyValueSetSymbol("DRV_MET_WAVECAPT_SRC", drvMetWaveformCapture)
    drvMetWaveformCaptureSrc.setLabel("Capture Source")
    drvMetWaveformCaptureSrc.setDefaultValue(0)
    drvMetWaveformCaptureSrc.setVisible(False)
    drvMetWaveformCaptureSrc.setOutputMode("Key")
    drvMetWaveformCaptureSrc.setDisplayMode("Description")
    drvMetWaveformCaptureSrc.setHelp(srv_met_helpkeyword)
    drvMetWaveformCaptureSrc.addKey("16KHz", "0", "16KHz data")
    drvMetWaveformCaptureSrc.addKey("4KHz_FBW", "1", "4KHz Full Bandwidth")
    drvMetWaveformCaptureSrc.addKey("4KHz_NBW", "2", "4KHz Narrow Bandwidth")
    drvMetWaveformCaptureSrc.addKey("8KHz_FBW", "3", "8KHz Full Bandwidth")
    drvMetWaveformCaptureSrc.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_WAVEFORM_CAPTURE"])

    drvMetWaveformCaptureType = metComponentCommon.createKeyValueSetSymbol("DRV_MET_WAVECAPT_TYPE", drvMetWaveformCapture)
    drvMetWaveformCaptureType.setLabel("Capture Type")
    drvMetWaveformCaptureType.setDefaultValue(0)
    drvMetWaveformCaptureType.setVisible(False)
    drvMetWaveformCaptureType.setOutputMode("Key")
    drvMetWaveformCaptureType.setDisplayMode("Description")
    drvMetWaveformCaptureType.setHelp(srv_met_helpkeyword)
    drvMetWaveformCaptureType.addKey("One-shoot", "0", "One-shot capture")
    drvMetWaveformCaptureType.addKey("Continuous", "1", "Continuous capture")
    drvMetWaveformCaptureType.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_WAVEFORM_CAPTURE"])

    drvMetCaptureBufSizeChn = metComponentCommon.createIntegerSymbol("DRV_MET_CAPTURE_BUF_SIZE_CHN", drvMetWaveformCapture)
    drvMetCaptureBufSizeChn.setLabel("Samples per Channel")
    drvMetCaptureBufSizeChn.setVisible(False)
    drvMetCaptureBufSizeChn.setDefaultValue(8000)
    drvMetCaptureBufSizeChn.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_WAVEFORM_CAPTURE"])
    drvMetCaptureBufSizeChn.setHelp(srv_met_helpkeyword)

    drvMetNotificationMenu = metComponentCommon.createMenuSymbol("DRV_MET_NOTIFICATION_MENU", None)
    drvMetNotificationMenu.setLabel("Additional Notifications")
    drvMetNotificationMenu.setHelp(srv_met_helpkeyword)
    
    drvMetNotFullCycle = metComponentCommon.createBooleanSymbol("DRV_MET_NOT_FULL_CYCLE", drvMetNotificationMenu)
    drvMetNotFullCycle.setLabel("Line Cycle")
    drvMetNotFullCycle.setDefaultValue(False)
    drvMetNotFullCycle.setHelp(srv_met_helpkeyword)
    
    drvMetNotHalfCycle = metComponentCommon.createBooleanSymbol("DRV_MET_NOT_HALF_CYCLE", drvMetNotificationMenu)
    drvMetNotHalfCycle.setLabel("Half Line Cycle")
    drvMetNotHalfCycle.setDefaultValue(False)
    drvMetNotHalfCycle.setHelp(srv_met_helpkeyword)
    
    drvMetNotRawZeroCrossing = metComponentCommon.createBooleanSymbol("DRV_MET_RAW_ZERO_CROSSING", drvMetNotificationMenu)
    drvMetNotRawZeroCrossing.setLabel("Raw Zero Crossing")
    drvMetNotRawZeroCrossing.setDefaultValue(False)
    drvMetNotRawZeroCrossing.setHelp(srv_met_helpkeyword)
    
    drvMetNotPulse0 = metComponentCommon.createBooleanSymbol("DRV_MET_PULSE_0", drvMetNotificationMenu)
    drvMetNotPulse0.setLabel("Pulse 0")
    drvMetNotPulse0.setDefaultValue(False)
    drvMetNotPulse0.setHelp(srv_met_helpkeyword)
    
    drvMetNotPulse1 = metComponentCommon.createBooleanSymbol("DRV_MET_PULSE_1", drvMetNotificationMenu)
    drvMetNotPulse1.setLabel("Pulse 1")
    drvMetNotPulse1.setDefaultValue(False)
    drvMetNotPulse1.setHelp(srv_met_helpkeyword)
    
    drvMetNotPulse2 = metComponentCommon.createBooleanSymbol("DRV_MET_PULSE_2", drvMetNotificationMenu)
    drvMetNotPulse2.setLabel("Pulse 2")
    drvMetNotPulse2.setDefaultValue(False)
    drvMetNotPulse2.setHelp(srv_met_helpkeyword)

    #####################################################################################################################################
    # CONFIGURATION PARAMS 

    drvMetConfChannels = metComponentCommon.createMenuSymbol("DRV_MET_CONF_CHANNELS", None)
    drvMetConfChannels.setLabel("Enable Channels")
    drvMetConfChannels.setHelp(srv_met_helpkeyword)

    drvMetConfI0 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_I0", drvMetConfChannels)
    drvMetConfI0.setLabel("Enable Channel I0")
    drvMetConfI0.setDefaultValue(1)
    drvMetConfI0.setHelp(srv_met_helpkeyword)

    drvMetConfI1 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_I1", drvMetConfChannels)
    drvMetConfI1.setLabel("Enable Channel I1")
    drvMetConfI1.setDefaultValue(1)
    drvMetConfI1.setHelp(srv_met_helpkeyword)

    drvMetConfV1 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_V1", drvMetConfChannels)
    drvMetConfV1.setLabel("Enable Channel V1")
    drvMetConfV1.setDefaultValue(1)
    drvMetConfV1.setHelp(srv_met_helpkeyword)

    drvMetConfI2 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_I2", drvMetConfChannels)
    drvMetConfI2.setLabel("Enable Channel I2")
    drvMetConfI2.setDefaultValue(1)
    drvMetConfI2.setHelp(srv_met_helpkeyword)

    drvMetConfV2 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_V2", drvMetConfChannels)
    drvMetConfV2.setLabel("Enable Channel V2")
    drvMetConfV2.setDefaultValue(1)
    drvMetConfV2.setHelp(srv_met_helpkeyword)

    drvMetConfI3 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_I3", drvMetConfChannels)
    drvMetConfI3.setLabel("Enable Channel I3")
    drvMetConfI3.setDefaultValue(enablePhase3)
    drvMetConfI3.setHelp(srv_met_helpkeyword)

    drvMetConfV3 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_V3", drvMetConfChannels)
    drvMetConfV3.setLabel("Enable Channel V3")
    drvMetConfV3.setDefaultValue(enablePhase3)
    drvMetConfV3.setHelp(srv_met_helpkeyword)
    
    drvMetConfByfDef = metComponentCommon.createMenuSymbol("DRV_MET_CONF_BY_DEFAULT", None)
    drvMetConfByfDef.setLabel("Configuration")
    drvMetConfByfDef.setHelp(srv_met_helpkeyword)

    drvMetConfST = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_ST", drvMetConfByfDef)
    drvMetConfST.setLabel("Sensor Type")
    drvMetConfST.setDefaultValue(0)
    drvMetConfST.setOutputMode("Key")
    drvMetConfST.setDisplayMode("Description")
    drvMetConfST.setHelp(srv_met_helpkeyword)
    drvMetConfST.addKey("CT", "0", "Current Transformer")
    drvMetConfST.addKey("SHUNT", "1", "Shunt resistor")
    drvMetConfST.addKey("COIL", "2", "Rogowski Coil")

    drvMetConfMC = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_MC", drvMetConfByfDef)
    drvMetConfMC.setLabel("Meter Constant [Pulses/KWh]")
    drvMetConfMC.setDefaultValue(3200)
    drvMetConfMC.setHelp(srv_met_helpkeyword)

    drvMetConfFreq = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_F", drvMetConfByfDef)
    drvMetConfFreq.setLabel("Mains Frequency (Hz)")
    drvMetConfFreq.setDefaultValue(50)
    drvMetConfFreq.setHelp(srv_met_helpkeyword)

    drvMetConfTr = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_TR", drvMetConfByfDef)
    drvMetConfTr.setLabel("Transformer Ratio")
    drvMetConfTr.setDefaultValue(2000)
    drvMetConfTr.setHelp(srv_met_helpkeyword)
    drvMetConfTr.setDependencies(updateTRLabel, ["DRV_MET_CONF_ST"])

    drvMetConfRl = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_RL", drvMetConfByfDef)
    drvMetConfRl.setLabel("Resistor Load (uOhms)")
    drvMetConfRl.setDefaultValue(3240000)
    drvMetConfRl.setHelp(srv_met_helpkeyword)
    drvMetConfRl.setDependencies(updateRLLabel, ["DRV_MET_CONF_ST"])

    drvMetConfKU = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_KU", drvMetConfByfDef)
    drvMetConfKU.setLabel("Voltage divider ratio")
    drvMetConfKU.setDefaultValue(1651)
    drvMetConfKU.setHelp(srv_met_helpkeyword)

    drvMetConfGain = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_GAIN", drvMetConfByfDef)
    drvMetConfGain.setLabel("Programmable Gain Amplifier")
    drvMetConfGain.setDefaultValue(0)
    drvMetConfGain.setOutputMode("Key")
    drvMetConfGain.setDisplayMode("Description")
    drvMetConfGain.setHelp(srv_met_helpkeyword)
    drvMetConfGain.addKey("GAIN_1", "0", "GAIN_1")
    drvMetConfGain.addKey("GAIN_2", "1", "GAIN_2")
    drvMetConfGain.addKey("GAIN_4", "2", "GAIN_4")
    drvMetConfGain.addKey("GAIN_8", "3", "GAIN_8")

    drvMetConfSwell = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_SWELL", drvMetConfByfDef)
    drvMetConfSwell.setLabel("SWELL Threshold RMS (V)")
    drvMetConfSwell.setDefaultValue(251)
    drvMetConfSwell.setHelp(srv_met_helpkeyword)

    drvMetConfSwell = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_SAG", drvMetConfByfDef)
    drvMetConfSwell.setLabel("SAG Threshold RMS (V)")
    drvMetConfSwell.setDefaultValue(132)
    drvMetConfSwell.setHelp(srv_met_helpkeyword)

    drvMetConfCreepPEn = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_CREEP_P_EN", drvMetConfByfDef)
    drvMetConfCreepPEn.setLabel("Active Power Creep Threshold Enable")
    drvMetConfCreepPEn.setDefaultValue(0)
    drvMetConfCreepPEn.setHelp(srv_met_helpkeyword)

    drvMetConfCreepP = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_CREEP_P", drvMetConfCreepPEn)
    drvMetConfCreepP.setLabel("Creep Active Energy (Wh)")
    drvMetConfCreepP.setDefaultValue(2)
    drvMetConfCreepP.setVisible(False)
    drvMetConfCreepP.setHelp(srv_met_helpkeyword)
    drvMetConfCreepP.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_CONF_CREEP_P_EN"])

    drvMetConfCreepQEn = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_CREEP_Q_EN", drvMetConfByfDef)
    drvMetConfCreepQEn.setLabel("Reactive Power Creep Threshold Enable")
    drvMetConfCreepQEn.setDefaultValue(0)
    drvMetConfCreepQEn.setHelp(srv_met_helpkeyword)

    drvMetConfCreepQ = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_CREEP_Q", drvMetConfCreepQEn)
    drvMetConfCreepQ.setLabel("Creep Reactive Energy (VARh)")
    drvMetConfCreepQ.setDefaultValue(2)
    drvMetConfCreepQ.setVisible(False)
    drvMetConfCreepQ.setHelp(srv_met_helpkeyword)
    drvMetConfCreepQ.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_CONF_CREEP_Q_EN"])

    drvMetConfCreepIEn = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_CREEP_I_EN", drvMetConfByfDef)
    drvMetConfCreepIEn.setLabel("Current Creep Threshold Enable")
    drvMetConfCreepIEn.setDefaultValue(0)
    drvMetConfCreepIEn.setHelp(srv_met_helpkeyword)

    drvMetConfCreepI = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_CREEP_I", drvMetConfCreepIEn)
    drvMetConfCreepI.setLabel("Creep Current (mA)")
    drvMetConfCreepI.setDefaultValue(5)
    drvMetConfCreepI.setVisible(False)
    drvMetConfCreepI.setHelp(srv_met_helpkeyword)
    drvMetConfCreepI.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_CONF_CREEP_I_EN"])
    
    drvMetConfPulse0 = metComponentCommon.createMenuSymbol("DRV_MET_CONF_PULSE0", drvMetConfByfDef)
    drvMetConfPulse0.setLabel("Pulse 0 Control")
    drvMetConfPulse0.setHelp(srv_met_helpkeyword)

    drvMetConfP0En = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_EN_P0", drvMetConfPulse0)
    drvMetConfP0En.setLabel("Output Pulse Enable (PD17)")
    drvMetConfP0En.setDefaultValue(1)
    drvMetConfP0En.setHelp(srv_met_helpkeyword)

    drvMetConfP0Detent = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_DET_P0", drvMetConfPulse0)
    drvMetConfP0Detent.setLabel("Total Absolute Values (P/Q)")
    drvMetConfP0Detent.setDefaultValue(0)
    drvMetConfP0Detent.setOutputMode("Key")
    drvMetConfP0Detent.setDisplayMode("Description")
    drvMetConfP0Detent.setHelp(srv_met_helpkeyword)
    drvMetConfP0Detent.addKey("NET", "0", "NET")
    drvMetConfP0Detent.addKey("ABSOLUTE", "1", "ABSOLUTE")
    drvMetConfP0Detent.addKey("DELIVERED", "2", "DELIVERED")
    drvMetConfP0Detent.addKey("GENERATED", "3", "GENERATED")

    drvMetConfP0Polarity = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_POL_P0", drvMetConfPulse0)
    drvMetConfP0Polarity.setLabel("Polarity")
    drvMetConfP0Polarity.setDefaultValue(1)
    drvMetConfP0Polarity.setOutputMode("Key")
    drvMetConfP0Polarity.setDisplayMode("Description")
    drvMetConfP0Polarity.setHelp(srv_met_helpkeyword)
    drvMetConfP0Polarity.addKey("PULSE_LOW", "0", "PULSE_LOW")
    drvMetConfP0Polarity.addKey("PULSE_HIGH", "1", "PULSE_HIGH")

    drvMetConfP0Type = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_TYP_P0", drvMetConfPulse0)
    drvMetConfP0Type.setLabel("Type")
    drvMetConfP0Type.setDefaultValue(0)
    drvMetConfP0Type.setOutputMode("Key")
    drvMetConfP0Type.setDisplayMode("Description")
    drvMetConfP0Type.setHelp(srv_met_helpkeyword)
    drvMetConfP0Type.addKey("P_T", "0", "P_T")
    drvMetConfP0Type.addKey("P_T_F", "1", "P_T_F")
    drvMetConfP0Type.addKey("Q_T", "2", "Q_T")
    drvMetConfP0Type.addKey("Q_T_F", "3", "Q_T_F")
    drvMetConfP0Type.addKey("I_T", "4", "I_T")
    drvMetConfP0Type.addKey("I_T_F", "5", "I_T_F")  

    drvMetConfP0Width = metComponentCommon.createHexSymbol("DRV_MET_CONF_WID_P0", drvMetConfPulse0)
    drvMetConfP0Width.setLabel("Width")
    drvMetConfP0Width.setVisible(True)
    drvMetConfP0Width.setDefaultValue(0x9100)
    drvMetConfP0Width.setHelp(srv_met_helpkeyword)
    
    drvMetConfPulse1 = metComponentCommon.createMenuSymbol("DRV_MET_CONF_PULSE1", drvMetConfByfDef)
    drvMetConfPulse1.setLabel("Pulse 1 Control")
    drvMetConfPulse1.setHelp(srv_met_helpkeyword)

    drvMetConfP1En = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_EN_P1", drvMetConfPulse1)
    drvMetConfP1En.setLabel("Output Pulse Enable (PD18)")
    drvMetConfP1En.setDefaultValue(1)
    drvMetConfP1En.setHelp(srv_met_helpkeyword)

    drvMetConfP1Detent = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_DET_P1", drvMetConfPulse1)
    drvMetConfP1Detent.setLabel("Total Absolute Values (P/Q)")
    drvMetConfP1Detent.setDefaultValue(0)
    drvMetConfP1Detent.setOutputMode("Key")
    drvMetConfP1Detent.setDisplayMode("Description")
    drvMetConfP1Detent.setHelp(srv_met_helpkeyword)
    drvMetConfP1Detent.addKey("NET", "0", "NET")
    drvMetConfP1Detent.addKey("ABSOLUTE", "1", "ABSOLUTE")
    drvMetConfP1Detent.addKey("DELIVERED", "2", "DELIVERED")
    drvMetConfP1Detent.addKey("GENERATED", "3", "GENERATED")

    drvMetConfP1Polarity = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_POL_P1", drvMetConfPulse1)
    drvMetConfP1Polarity.setLabel("Polarity")
    drvMetConfP1Polarity.setDefaultValue(1)
    drvMetConfP1Polarity.setOutputMode("Key")
    drvMetConfP1Polarity.setDisplayMode("Description")
    drvMetConfP1Polarity.setHelp(srv_met_helpkeyword)
    drvMetConfP1Polarity.addKey("PULSE_LOW", "0", "PULSE_LOW")
    drvMetConfP1Polarity.addKey("PULSE_HIGH", "1", "PULSE_HIGH")

    drvMetConfP1Type = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_TYP_P1", drvMetConfPulse1)
    drvMetConfP1Type.setLabel("Type")
    drvMetConfP1Type.setDefaultValue(2)
    drvMetConfP1Type.setOutputMode("Key")
    drvMetConfP1Type.setDisplayMode("Description")
    drvMetConfP1Type.setHelp(srv_met_helpkeyword)
    drvMetConfP1Type.addKey("P_T", "0", "P_T")
    drvMetConfP1Type.addKey("P_T_F", "1", "P_T_F")
    drvMetConfP1Type.addKey("Q_T", "2", "Q_T")
    drvMetConfP1Type.addKey("Q_T_F", "3", "Q_T_F")
    drvMetConfP1Type.addKey("I_T", "4", "I_T")
    drvMetConfP1Type.addKey("I_T_F", "5", "I_T_F")    

    drvMetConfP1Width = metComponentCommon.createHexSymbol("DRV_MET_CONF_WID_P1", drvMetConfPulse1)
    drvMetConfP1Width.setLabel("Width")
    drvMetConfP1Width.setVisible(True)
    drvMetConfP1Width.setDefaultValue(0x9100)
    drvMetConfP1Width.setHelp(srv_met_helpkeyword)
    
    drvMetConfPulse2 = metComponentCommon.createMenuSymbol("DRV_MET_CONF_PULSE2", drvMetConfByfDef)
    drvMetConfPulse2.setLabel("Pulse 2 Control")
    drvMetConfPulse2.setHelp(srv_met_helpkeyword)

    drvMetConfP2En = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_EN_P2", drvMetConfPulse2)
    drvMetConfP2En.setLabel("Output Pulse Enable (PD19)")
    drvMetConfP2En.setDefaultValue(0)
    drvMetConfP2En.setHelp(srv_met_helpkeyword)

    drvMetConfP2Detent = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_DET_P2", drvMetConfPulse2)
    drvMetConfP2Detent.setLabel("Total Absolute Values (P/Q)")
    drvMetConfP2Detent.setDefaultValue(0)
    drvMetConfP2Detent.setOutputMode("Key")
    drvMetConfP2Detent.setDisplayMode("Description")
    drvMetConfP2Detent.setHelp(srv_met_helpkeyword)
    drvMetConfP2Detent.addKey("NET", "0", "NET")
    drvMetConfP2Detent.addKey("ABSOLUTE", "1", "ABSOLUTE")
    drvMetConfP2Detent.addKey("DELIVERED", "2", "DELIVERED")
    drvMetConfP2Detent.addKey("GENERATED", "3", "GENERATED")

    drvMetConfP2Polarity = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_POL_P2", drvMetConfPulse2)
    drvMetConfP2Polarity.setLabel("Polarity")
    drvMetConfP2Polarity.setDefaultValue(0)
    drvMetConfP2Polarity.setOutputMode("Key")
    drvMetConfP2Polarity.setDisplayMode("Description")
    drvMetConfP2Polarity.setHelp(srv_met_helpkeyword)
    drvMetConfP2Polarity.addKey("PULSE_LOW", "0", "PULSE_LOW")
    drvMetConfP2Polarity.addKey("PULSE_HIGH", "1", "PULSE_HIGH")

    drvMetConfP2Type = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_TYP_P2", drvMetConfPulse2)
    drvMetConfP2Type.setLabel("Type")
    drvMetConfP2Type.setDefaultValue(0)
    drvMetConfP2Type.setOutputMode("Key")
    drvMetConfP2Type.setDisplayMode("Description")
    drvMetConfP2Type.setHelp(srv_met_helpkeyword)
    drvMetConfP2Type.addKey("P_T", "0", "P_T")
    drvMetConfP2Type.addKey("P_T_F", "1", "P_T_F")
    drvMetConfP2Type.addKey("Q_T", "2", "Q_T")
    drvMetConfP2Type.addKey("Q_T_F", "3", "Q_T_F")
    drvMetConfP2Type.addKey("I_T", "4", "I_T")
    drvMetConfP2Type.addKey("I_T_F", "5", "I_T_F")    

    drvMetConfP2Width = metComponentCommon.createHexSymbol("DRV_MET_CONF_WID_P2", drvMetConfPulse2)
    drvMetConfP2Width.setLabel("Width")
    drvMetConfP2Width.setVisible(True)
    drvMetConfP2Width.setDefaultValue(0x0000)
    drvMetConfP2Width.setHelp(srv_met_helpkeyword)

    #####################################################################################################################################
    # METROLOGY REGISTERS 
    
    drvMetRegWaveformCapture = metComponentCommon.createHexSymbol("DRV_MET_CONF_WAVEFORM_CAPTURE", None)
    drvMetRegWaveformCapture.setLabel("WAVEFORM_CAPTURE")
    drvMetRegWaveformCapture.setVisible(False)
    drvMetRegWaveformCapture.setDefaultValue(0x00000000)
    drvMetRegWaveformCapture.setReadOnly(True)
    drvMetRegWaveformCapture.setDependencies(updateConfigWaveform, ["DRV_MET_WAVEFORM_CAPTURE", "DRV_MET_WAVECAPT_CH_IA", "DRV_MET_WAVECAPT_CH_VA", "DRV_MET_WAVECAPT_CH_IB", "DRV_MET_WAVECAPT_CH_VB", "DRV_MET_WAVECAPT_CH_IC", "DRV_MET_WAVECAPT_CH_VC", "DRV_MET_WAVECAPT_SRC", "DRV_MET_WAVECAPT_TYPE", "DRV_MET_CAPTURE_BUF_SIZE_CHN"])

    global drvMetRegWaveformCaptureSize
    drvMetRegWaveformCaptureSize = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_CAPTURE_BUFFER_SIZE", None)
    drvMetRegWaveformCaptureSize.setLabel("CAPTURE_BUFFER_SIZE")
    drvMetRegWaveformCaptureSize.setVisible(False)
    drvMetRegWaveformCaptureSize.setDefaultValue(0)
    drvMetRegWaveformCaptureSize.setReadOnly(True)
    
    drvMetRegPKT = metComponentCommon.createHexSymbol("DRV_MET_CTRL_PKT", None)
    drvMetRegPKT.setLabel("PKT")
    drvMetRegPKT.setVisible(False)
    drvMetRegPKT.setDefaultValue(0x00500000)
    drvMetRegPKT.setReadOnly(True)
    drvMetRegPKT.setDependencies(updateConfigPKT, ["DRV_MET_CONF_MC"])

    drvMetRegMT = metComponentCommon.createHexSymbol("DRV_MET_CTRL_MT", None)
    drvMetRegMT.setLabel("MT")
    drvMetRegMT.setVisible(False)
    drvMetRegMT.setDefaultValue(0x00000CCC)
    drvMetRegMT.setReadOnly(True)
    drvMetRegMT.setDependencies(updateConfigMT, ["DRV_MET_CONF_ST"])

    drvMetRegKI = metComponentCommon.createHexSymbol("DRV_MET_CTRL_KI", None)
    drvMetRegKI.setLabel("KI")
    drvMetRegKI.setVisible(False)
    drvMetRegKI.setDefaultValue(0x0009A523)
    drvMetRegKI.setReadOnly(True)
    drvMetRegKI.setDependencies(updateConfigKI, ["DRV_MET_CONF_F", "DRV_MET_CONF_TR", "DRV_MET_CONF_RL", "DRV_MET_CONF_GAIN"])

    drvMetRegKV = metComponentCommon.createHexSymbol("DRV_MET_CTRL_KV", None)
    drvMetRegKV.setLabel("KV")
    drvMetRegKV.setVisible(False)
    drvMetRegKV.setDefaultValue(0x0019CC00)
    drvMetRegKV.setReadOnly(True)
    drvMetRegKV.setDependencies(updateConfigKV, ["DRV_MET_CONF_KU"])

    drvMetRegATS2023 = metComponentCommon.createHexSymbol("DRV_MET_CTRL_ATS2023", None)
    drvMetRegATS2023.setLabel("ATS2023")
    drvMetRegATS2023.setVisible(False)
    drvMetRegATS2023.setDefaultValue(0x01010103)
    drvMetRegATS2023.setReadOnly(True)
    drvMetRegATS2023.setDependencies(updateConfigATS2023, ["DRV_MET_CONF_GAIN", "DRV_MET_CONF_I0", "DRV_MET_CONF_I1", "DRV_MET_CONF_V1", "DRV_MET_CONF_I2"])

    drvMetRegATS2427 = metComponentCommon.createHexSymbol("DRV_MET_CTRL_ATS2427", None)
    drvMetRegATS2427.setLabel("ATS2427")
    drvMetRegATS2427.setVisible(False)
    drvMetRegATS2427.setDefaultValue(at2427Value)
    drvMetRegATS2427.setReadOnly(True)
    drvMetRegATS2427.setDependencies(updateConfigATS2427, ["DRV_MET_CONF_GAIN", "DRV_MET_CONF_V2", "DRV_MET_CONF_I3", "DRV_MET_CONF_V3"])

    drvMetRegSWELL = metComponentCommon.createHexSymbol("DRV_MET_CTRL_SWELL", None)
    drvMetRegSWELL.setLabel("SWELL")
    drvMetRegSWELL.setVisible(False)
    drvMetRegSWELL.setDefaultValue(0x5eab918)
    drvMetRegSWELL.setReadOnly(True)
    drvMetRegSWELL.setDependencies(updateConfigSwellSag, ["DRV_MET_CONF_SWELL", "DRV_MET_CONF_KU"])

    drvMetRegSAG = metComponentCommon.createHexSymbol("DRV_MET_CTRL_SAG", None)
    drvMetRegSAG.setLabel("SAG")
    drvMetRegSAG.setVisible(False)
    drvMetRegSAG.setDefaultValue(0x1a2ec26)
    drvMetRegSAG.setReadOnly(True)
    drvMetRegSAG.setDependencies(updateConfigSwellSag, ["DRV_MET_CONF_SAG"])

    drvMetRegCreepP = metComponentCommon.createHexSymbol("DRV_MET_CTRL_CREEP_P", None)
    drvMetRegCreepP.setLabel("CREEP_P")
    drvMetRegCreepP.setVisible(False)
    drvMetRegCreepP.setDefaultValue(0x00002E9A)
    drvMetRegCreepP.setReadOnly(True)
    drvMetRegCreepP.setDependencies(updateConfigCreepPQ, ["DRV_MET_CONF_CREEP_P", "DRV_MET_CONF_F"])

    drvMetRegCreepQ = metComponentCommon.createHexSymbol("DRV_MET_CTRL_CREEP_Q", None)
    drvMetRegCreepQ.setLabel("CREEP_Q")
    drvMetRegCreepQ.setVisible(False)
    drvMetRegCreepQ.setDefaultValue(0x00002E9A)
    drvMetRegCreepQ.setReadOnly(True)
    drvMetRegCreepQ.setDependencies(updateConfigCreepPQ, ["DRV_MET_CONF_CREEP_Q", "DRV_MET_CONF_F"])

    drvMetRegCreepI = metComponentCommon.createHexSymbol("DRV_MET_CTRL_CREEP_I", None)
    drvMetRegCreepI.setLabel("CREEP_I")
    drvMetRegCreepI.setVisible(False)
    drvMetRegCreepI.setDefaultValue(0x0000212D)
    drvMetRegCreepI.setReadOnly(True)
    drvMetRegCreepI.setDependencies(updateConfigCreepI, ["DRV_MET_CONF_CREEP_I", "DRV_MET_CONF_F", "DRV_MET_CONF_TR", "DRV_MET_CONF_RL", "DRV_MET_CONF_GAIN"])

    drvMetRegFEATCTRL0 = metComponentCommon.createHexSymbol("DRV_MET_CTRL_FEATCTRL0", None)
    drvMetRegFEATCTRL0.setLabel("FEATURE_CTRL0")
    drvMetRegFEATCTRL0.setVisible(False)
    drvMetRegFEATCTRL0.setDefaultValue(featCtrl0Value)
    drvMetRegFEATCTRL0.setReadOnly(True)
    drvMetRegFEATCTRL0.setDependencies(updateConfigFeatCtrl0, ["DRV_MET_CONF_I1", "DRV_MET_CONF_V1", "DRV_MET_CONF_I2", "DRV_MET_CONF_V2", "DRV_MET_CONF_I3", "DRV_MET_CONF_V3"])

    drvMetRegFEATCTRL1 = metComponentCommon.createHexSymbol("DRV_MET_CTRL_FEATCTRL1", None)
    drvMetRegFEATCTRL1.setLabel("FEATURE_CTRL1")
    drvMetRegFEATCTRL1.setVisible(False)
    drvMetRegFEATCTRL1.setDefaultValue(0)
    drvMetRegFEATCTRL1.setReadOnly(True)
    drvMetRegFEATCTRL1.setDependencies(updateConfigFeatCtrl1, ["DRV_MET_CONF_CREEP_P_EN", "DRV_MET_CONF_CREEP_Q_EN", "DRV_MET_CONF_CREEP_I_EN"])

    drvMetRegPulse0Ctrl = metComponentCommon.createHexSymbol("DRV_MET_CTRL_PULSE_CTRL_0", None)
    drvMetRegPulse0Ctrl.setLabel("PULSE0_CTRL")
    drvMetRegPulse0Ctrl.setVisible(False)
    drvMetRegPulse0Ctrl.setDefaultValue(0x81009100)
    drvMetRegPulse0Ctrl.setReadOnly(True)
    drvMetRegPulse0Ctrl.setDependencies(updateConfigPulseXCtrl, ["DRV_MET_CONF_EN_P0", "DRV_MET_CONF_DET_P0", "DRV_MET_CONF_POL_P0", "DRV_MET_CONF_TYP_P0", "DRV_MET_CONF_WID_P0"])

    drvMetRegPulse1Ctrl = metComponentCommon.createHexSymbol("DRV_MET_CTRL_PULSE_CTRL_1", None)
    drvMetRegPulse1Ctrl.setLabel("PULSE1_CTRL")
    drvMetRegPulse1Ctrl.setVisible(False)
    drvMetRegPulse1Ctrl.setDefaultValue(0x81029100)
    drvMetRegPulse1Ctrl.setReadOnly(True)
    drvMetRegPulse1Ctrl.setDependencies(updateConfigPulseXCtrl, ["DRV_MET_CONF_EN_P1", "DRV_MET_CONF_DET_P1", "DRV_MET_CONF_POL_P1", "DRV_MET_CONF_TYP_P1", "DRV_MET_CONF_WID_P1"])

    drvMetRegPulse2Ctrl = metComponentCommon.createHexSymbol("DRV_MET_CTRL_PULSE_CTRL_2", None)
    drvMetRegPulse2Ctrl.setLabel("PULSE2_CTRL")
    drvMetRegPulse2Ctrl.setVisible(False)
    drvMetRegPulse2Ctrl.setDefaultValue(0x0)
    drvMetRegPulse2Ctrl.setReadOnly(True)
    drvMetRegPulse2Ctrl.setDependencies(updateConfigPulseXCtrl, ["DRV_MET_CONF_EN_P2", "DRV_MET_CONF_DET_P2", "DRV_MET_CONF_POL_P2", "DRV_MET_CONF_TYP_P2", "DRV_MET_CONF_WID_P2"])

    #####################################################################################################################################
    # RTOS CONFIG
    metrologyRTOSSupport = metComponentCommon.createBooleanSymbol("DRV_MET_RTOS_ENABLE", None)
    metrologyRTOSSupport.setLabel("RTOS support")
    metrologyRTOSSupport.setDefaultValue(0)
    metrologyRTOSSupport.setVisible(False)
    metrologyRTOSSupport.setDependencies(enableRTOS, ["HarmonyCore.SELECT_RTOS"])

    metrologyRTOSMenu = metComponentCommon.createMenuSymbol("DRV_MET_RTOS_MENU", None)
    metrologyRTOSMenu.setLabel("RTOS settings")
    metrologyRTOSMenu.setHelp(srv_met_helpkeyword)
    metrologyRTOSMenu.setDescription("RTOS settings")
    metrologyRTOSMenu.setVisible(False)
    metrologyRTOSMenu.setDependencies(showRTOSMenu, ["HarmonyCore.SELECT_RTOS"])

    metrologyRTOSStackSize = metComponentCommon.createIntegerSymbol("DRV_MET_RTOS_STACK_SIZE", metrologyRTOSMenu)
    metrologyRTOSStackSize.setLabel("Stack Size (in bytes)")
    metrologyRTOSStackSize.setHelp(srv_met_helpkeyword)
    metrologyRTOSStackSize.setDefaultValue(1024)

    metrologyRTOSMsgQSize = metComponentCommon.createIntegerSymbol("DRV_MET_RTOS_TASK_MSG_QTY", metrologyRTOSMenu)
    metrologyRTOSMsgQSize.setLabel("Maximum Message Queue Size")
    metrologyRTOSMsgQSize.setHelp(srv_met_helpkeyword)
    metrologyRTOSMsgQSize.setDescription("A µC/OS-III task contains an optional internal message queue (if OS_CFG_TASK_Q_EN is set to DEF_ENABLED in os_cfg.h). This argument specifies the maximum number of messages that the task can receive through this message queue. The user may specify that the task is unable to receive messages by setting this argument to 0")
    metrologyRTOSMsgQSize.setDefaultValue(0)
    metrologyRTOSMsgQSize.setVisible(getActiveRtos() == "MicriumOSIII")
    metrologyRTOSMsgQSize.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])

    metrologyRTOSTaskTimeQuanta = metComponentCommon.createIntegerSymbol("DRV_MET_RTOS_TASK_TIME_QUANTA", metrologyRTOSMenu)
    metrologyRTOSTaskTimeQuanta.setLabel("Task Time Quanta")
    metrologyRTOSTaskTimeQuanta.setHelp(srv_met_helpkeyword)
    metrologyRTOSTaskTimeQuanta.setDescription("The amount of time (in clock ticks) for the time quanta when Round Robin is enabled. If you specify 0, then the default time quanta will be used which is the tick rate divided by 10.")
    metrologyRTOSTaskTimeQuanta.setDefaultValue(0)
    metrologyRTOSTaskTimeQuanta.setVisible(getActiveRtos() == "MicriumOSIII")
    metrologyRTOSTaskTimeQuanta.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])

    metrologyRTOSTaskPriority = metComponentCommon.createIntegerSymbol("DRV_MET_RTOS_TASK_PRIORITY", metrologyRTOSMenu)
    metrologyRTOSTaskPriority.setLabel("Task Priority")
    metrologyRTOSTaskPriority.setHelp(srv_met_helpkeyword)
    metrologyRTOSTaskPriority.setDefaultValue(1)

    metrologyRTOSTaskSpecificOpt = metComponentCommon.createBooleanSymbol("DRV_MET_RTOS_TASK_OPT_NONE", metrologyRTOSMenu)
    metrologyRTOSTaskSpecificOpt.setLabel("Task Specific Options")
    metrologyRTOSTaskSpecificOpt.setHelp(srv_met_helpkeyword)
    metrologyRTOSTaskSpecificOpt.setDescription("Contains task-specific options. Each option consists of one bit. The option is selected when the bit is set. The current version of µC/OS-III supports the following options:")
    metrologyRTOSTaskSpecificOpt.setDefaultValue(True)
    metrologyRTOSTaskSpecificOpt.setVisible(getActiveRtos() == "MicriumOSIII")
    metrologyRTOSTaskSpecificOpt.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])

    metrologyRTOSTaskStkChk = metComponentCommon.createBooleanSymbol("DRV_MET_RTOS_TASK_OPT_STK_CHK", metrologyRTOSTaskSpecificOpt)
    metrologyRTOSTaskStkChk.setLabel("Stack checking is allowed for the task")
    metrologyRTOSTaskStkChk.setHelp(srv_met_helpkeyword)
    metrologyRTOSTaskStkChk.setDescription("Specifies whether stack checking is allowed for the task")
    metrologyRTOSTaskStkChk.setDefaultValue(True)
    metrologyRTOSTaskStkChk.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_MET_RTOS_TASK_OPT_NONE"])

    metrologyRTOSTaskStkClr = metComponentCommon.createBooleanSymbol("DRV_MET_RTOS_TASK_OPT_STK_CLR", metrologyRTOSTaskSpecificOpt)
    metrologyRTOSTaskStkClr.setLabel("Stack needs to be cleared")
    metrologyRTOSTaskStkClr.setHelp(srv_met_helpkeyword)
    metrologyRTOSTaskStkClr.setDescription("Specifies whether the stack needs to be cleared")
    metrologyRTOSTaskStkClr.setDefaultValue(True)
    metrologyRTOSTaskStkClr.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_MET_RTOS_TASK_OPT_NONE"])

    metrologyRTOSTaskSaveFp = metComponentCommon.createBooleanSymbol("DRV_MET_RTOS_TASK_OPT_SAVE_FP", metrologyRTOSTaskSpecificOpt)
    metrologyRTOSTaskSaveFp.setLabel("Floating-point registers needs to be saved")
    metrologyRTOSTaskSaveFp.setHelp(srv_met_helpkeyword)
    metrologyRTOSTaskSaveFp.setDescription("Specifies whether floating-point registers are saved. This option is only valid if the processor has floating-point hardware and the processor-specific code saves the floating-point registers")
    metrologyRTOSTaskSaveFp.setDefaultValue(False)
    metrologyRTOSTaskSaveFp.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_MET_RTOS_TASK_OPT_NONE"])

    metrologyRTOSTaskNoTls = metComponentCommon.createBooleanSymbol("DRV_MET_RTOS_TASK_OPT_NO_TLS", metrologyRTOSTaskSpecificOpt)
    metrologyRTOSTaskNoTls.setLabel("TLS (Thread Local Storage) support needed for the task")
    metrologyRTOSTaskNoTls.setHelp(srv_met_helpkeyword)
    metrologyRTOSTaskNoTls.setDescription("If the caller doesn’t want or need TLS (Thread Local Storage) support for the task being created. If you do not include this option, TLS will be supported by default. TLS support was added in V3.03.00")
    metrologyRTOSTaskNoTls.setDefaultValue(False)
    metrologyRTOSTaskNoTls.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_MET_RTOS_TASK_OPT_NONE"])

    
    #####################################################################################################################################
    # METROLOGY FILES 

    drvMetSourceFile = metComponentCommon.createFileSymbol("DRV_MET_SOURCE", None)
    drvMetSourceFile.setSourcePath("driver/metrology/drv_metrology.c.ftl")
    drvMetSourceFile.setOutputName("drv_metrology.c")
    drvMetSourceFile.setDestPath("driver/metrology")
    drvMetSourceFile.setProjectPath("config/" + configName + "/driver/metrology/")
    drvMetSourceFile.setType("SOURCE")
    drvMetSourceFile.setOverwrite(True)
    drvMetSourceFile.setMarkup(True)

    drvMetHeaderFile = metComponentCommon.createFileSymbol("DRV_MET_HEADER", None)
    drvMetHeaderFile.setSourcePath("driver/metrology/drv_metrology.h.ftl")
    drvMetHeaderFile.setOutputName("drv_metrology.h")
    drvMetHeaderFile.setDestPath("driver/metrology")
    drvMetHeaderFile.setProjectPath("config/" + configName + "/driver/metrology/")
    drvMetHeaderFile.setType("HEADER")
    drvMetHeaderFile.setOverwrite(True)
    drvMetHeaderFile.setMarkup(True)

    drvMetHeaderFileDefs = metComponentCommon.createFileSymbol("DRV_MET_HEADER_DEFINITIONS", None)
    drvMetHeaderFileDefs.setSourcePath("driver/metrology/drv_metrology_definitions.h.ftl")
    drvMetHeaderFileDefs.setOutputName("drv_metrology_definitions.h")
    drvMetHeaderFileDefs.setDestPath("driver/metrology")
    drvMetHeaderFileDefs.setProjectPath("config/" + configName + "/driver/metrology/")
    drvMetHeaderFileDefs.setType("HEADER")
    drvMetHeaderFileDefs.setOverwrite(True)
    drvMetHeaderFileDefs.setMarkup(True)

    drvMetHeaderRegsFile = metComponentCommon.createFileSymbol("DRV_MET_HEADER_REGS", None)
    drvMetHeaderRegsFile.setSourcePath("driver/metrology/drv_metrology_regs.h")
    drvMetHeaderRegsFile.setOutputName("drv_metrology_regs.h")
    drvMetHeaderRegsFile.setDestPath("driver/metrology")
    drvMetHeaderRegsFile.setProjectPath("config/" + configName + "/driver/metrology/")
    drvMetHeaderRegsFile.setType("HEADER")
    drvMetHeaderRegsFile.setOverwrite(True)
    
    #### ASM Path Settings #####################################################

    drvMetAsmPathSetting = metComponentCommon.createSettingSymbol("DRV_MET_ASM_PATH_SETTING", None)
    drvMetAsmPathSetting.setCategory("C32-AS")
    drvMetAsmPathSetting.setKey("extra-include-directories-for-assembler")
    drvMetAsmPathSetting.setValue("../src/config/" + configName + "/driver/metrology/bin")
    drvMetAsmPathSetting.setAppend(False, None)

    #### Binary MET Files ######################################################

    drvMetBinFile = metComponentCommon.createLibrarySymbol("DRV_MET_BIN", None)
    drvMetBinFile.setSourcePath("driver/metrology/bin/core1_metlib.bin")
    drvMetBinFile.setOutputName("core1_metlib.bin")
    drvMetBinFile.setDestPath("driver/metrology/bin")

    drvMetAssemblyBinFile = metComponentCommon.createFileSymbol("DRV_MET_ASSEMBLY_BIN", None)
    drvMetAssemblyBinFile.setSourcePath("driver/metrology/bin/met_bin.S")
    drvMetAssemblyBinFile.setOutputName("met_bin.S")
    drvMetAssemblyBinFile.setDestPath("driver/metrology/bin")
    drvMetAssemblyBinFile.setProjectPath("config/" + configName + "/driver/metrology/bin")
    drvMetAssemblyBinFile.setType("SOURCE")

    #### FreeMaker System Files ######################################################

    drvMetSystemDefFile = metComponentCommon.createFileSymbol("DRV_MET_SYSTEM_DEF", None)
    drvMetSystemDefFile.setType("STRING")
    drvMetSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    drvMetSystemDefFile.setSourcePath("driver/metrology/templates/system/definitions.h.ftl")
    drvMetSystemDefFile.setMarkup(True)

    drvMetSystemConfigFile = metComponentCommon.createFileSymbol("DRV_MET_CONFIGURATION", None)
    drvMetSystemConfigFile.setType("STRING")
    drvMetSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    drvMetSystemConfigFile.setSourcePath("driver/metrology/templates/system/configuration.h.ftl")
    drvMetSystemConfigFile.setMarkup(True)

    drvMetSystemInitDataFile = metComponentCommon.createFileSymbol("DRV_MET_INIT_DATA", None)
    drvMetSystemInitDataFile.setType("STRING")
    drvMetSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    drvMetSystemInitDataFile.setSourcePath("driver/metrology/templates/system/initialize_data.c.ftl")
    drvMetSystemInitDataFile.setMarkup(True)

    drvMetSystemInitFile = metComponentCommon.createFileSymbol("DRV_MET_INIT", None)
    drvMetSystemInitFile.setType("STRING")
    drvMetSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    drvMetSystemInitFile.setSourcePath("driver/metrology/templates/system/initialize.c.ftl")
    drvMetSystemInitFile.setMarkup(True)

    drvMetSystemTasksFile = metComponentCommon.createFileSymbol("DRV_MET_SYS_TASK", None)
    drvMetSystemTasksFile.setType("STRING")
    drvMetSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    drvMetSystemTasksFile.setSourcePath("driver/metrology/templates/system/system_tasks.c.ftl")
    drvMetSystemTasksFile.setMarkup(True)

    drvMetSystemDefObjFile = metComponentCommon.createFileSymbol("DRV_MET_SYSTEM_DEF_OBJECT", None)
    drvMetSystemDefObjFile.setType("STRING")
    drvMetSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    drvMetSystemDefObjFile.setSourcePath("driver/metrology/templates/system/definitions_objects.h.ftl")
    drvMetSystemDefObjFile.setMarkup(True)

    drvMetSystemRtosTasksFile = metComponentCommon.createFileSymbol("DRV_MET_SYS_RTOS_TASK", None)
    drvMetSystemRtosTasksFile.setType("STRING")
    drvMetSystemRtosTasksFile.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
    drvMetSystemRtosTasksFile.setSourcePath("driver/metrology/templates/system/system_rtos_tasks.c.ftl")
    drvMetSystemRtosTasksFile.setMarkup(True)
    drvMetSystemRtosTasksFile.setEnabled(False)
    drvMetSystemRtosTasksFile.setDependencies(genRtosTask, ["HarmonyCore.SELECT_RTOS"])

