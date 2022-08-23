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
        
def updateConfigPKT(symbol, event):
    m = 1000000000 / event["value"]
    m = (m << GAIN_P_K_T_Q)
    m = m / 1000000
    symbol.setValue(m)

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
    temp = Database.getSymbolValue("drvMet", "DRV_MET_CONF_TEMP")

    reg = (gainValue << 28) | (gainValue << 12) | (gainValue << 4) | (i2 << 24) | (v1 << 16) | (i1 << 8) | (temp << 1) | i0
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
    i = (event["value"] << GAIN_VI_Q)
    symbol.setValue(i)

def updateConfigSwellSag(symbol, event):
    ku = Database.getSymbolValue("drvMet", "DRV_MET_CONF_KU")
    threshold = event["value"]
    m = (threshold/float(ku))**2
    m = int(m * 2**32)
    symbol.setValue(m)

def updateConfigCreepPQ(symbol, event):
    freq = Database.getSymbolValue("drvMet", "DRV_MET_CONF_F")
    creep = event["value"]
    m = float(creep) / (freq * 3600)
    m = int(m * 2**30)
    symbol.setValue(m)

def updateConfigCreepI(symbol, event):
    K_Ix = _get_KI()
    creep = event["value"]
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
    srvMetCaptureNumChannels.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_WAVEFORM_CAPTURE"])
    srvMetCaptureNumChannels.setHelp(srv_met_helpkeyword)

    global srvMetCaptureBufSizeChn
    srvMetCaptureBufSizeChn = metComponentCommon.createIntegerSymbol("DRV_MET_CAPTURE_BUF_SIZE_CHN", srvMetWaveformCapture)
    srvMetCaptureBufSizeChn.setLabel("Capture Size per Channel")
    srvMetCaptureBufSizeChn.setVisible(False)
    srvMetCaptureBufSizeChn.setDefaultValue(8000)
    srvMetCaptureBufSizeChn.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_WAVEFORM_CAPTURE"])
    srvMetCaptureBufSizeChn.setHelp(srv_met_helpkeyword)

    srvMetCaptureBufSize = metComponentCommon.createIntegerSymbol("DRV_MET_CAPTURE_BUF_SIZE", None)
    srvMetCaptureBufSize.setLabel("Capture Size per Channel")
    srvMetCaptureBufSize.setVisible(False)
    srvMetCaptureBufSize.setDefaultValue(8000)
    srvMetCaptureBufSize.setDependencies(setWaveformCaptureSize, ["DRV_MET_CAPTURE_NUM_CHANNELS", "DRV_MET_CAPTURE_BUF_SIZE_CHN"])
    srvMetCaptureBufSize.setHelp(srv_met_helpkeyword)

    srvMetNotificationMenu = metComponentCommon.createMenuSymbol("DRV_MET_NOTIFICATION_MENU", None)
    srvMetNotificationMenu.setLabel("Additional Notifications")
    srvMetNotificationMenu.setHelp(srv_met_helpkeyword)
    
    srvMetNotFullCycle = metComponentCommon.createBooleanSymbol("DRV_MET_NOT_FULL_CYCLE", srvMetNotificationMenu)
    srvMetNotFullCycle.setLabel("Line Cycle")
    srvMetNotFullCycle.setDefaultValue(False)
    srvMetNotFullCycle.setHelp(srv_met_helpkeyword)
    
    srvMetNotHalfCycle = metComponentCommon.createBooleanSymbol("DRV_MET_NOT_HALF_CYCLE", srvMetNotificationMenu)
    srvMetNotHalfCycle.setLabel("Half Line Cycle")
    srvMetNotHalfCycle.setDefaultValue(False)
    srvMetNotHalfCycle.setHelp(srv_met_helpkeyword)
    
    # srvMetNotCreepDetection = metComponentCommon.createBooleanSymbol("DRV_MET_NOT_CREEP_DETECTION", srvMetNotificationMenu)
    # srvMetNotCreepDetection.setLabel("CREEP Detection")
    # srvMetNotCreepDetection.setDefaultValue(False)
    # srvMetNotCreepDetection.setHelp(srv_met_helpkeyword)
    
    srvMetNotRawZeroCrossing = metComponentCommon.createBooleanSymbol("DRV_MET_RAW_ZERO_CROSSING", srvMetNotificationMenu)
    srvMetNotRawZeroCrossing.setLabel("Raw Zero Crossing")
    srvMetNotRawZeroCrossing.setDefaultValue(False)
    srvMetNotRawZeroCrossing.setHelp(srv_met_helpkeyword)
    
    # srvMetNotStatusUpdate = metComponentCommon.createBooleanSymbol("DRV_MET_STATUS_UPDATE", srvMetNotificationMenu)
    # srvMetNotStatusUpdate.setLabel("Status Update")
    # srvMetNotStatusUpdate.setDefaultValue(False)
    # srvMetNotStatusUpdate.setHelp(srv_met_helpkeyword)
    
    srvMetNotPulse0 = metComponentCommon.createBooleanSymbol("DRV_MET_PULSE_0", srvMetNotificationMenu)
    srvMetNotPulse0.setLabel("Pulse 0")
    srvMetNotPulse0.setDefaultValue(False)
    srvMetNotPulse0.setHelp(srv_met_helpkeyword)
    
    srvMetNotPulse1 = metComponentCommon.createBooleanSymbol("DRV_MET_PULSE_1", srvMetNotificationMenu)
    srvMetNotPulse1.setLabel("Pulse 1")
    srvMetNotPulse1.setDefaultValue(False)
    srvMetNotPulse1.setHelp(srv_met_helpkeyword)
    
    srvMetNotPulse2 = metComponentCommon.createBooleanSymbol("DRV_MET_PULSE_2", srvMetNotificationMenu)
    srvMetNotPulse2.setLabel("Pulse 2")
    srvMetNotPulse2.setDefaultValue(False)
    srvMetNotPulse2.setHelp(srv_met_helpkeyword)

    #####################################################################################################################################
    # CONFIGURATION PARAMS 

    srvMetConfChannels = metComponentCommon.createMenuSymbol("DRV_MET_CONF_CHANNELS", None)
    srvMetConfChannels.setLabel("Enable Channels")
    srvMetConfChannels.setHelp(srv_met_helpkeyword)

    srvMetConfTemp = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_TEMP", srvMetConfChannels)
    srvMetConfTemp.setLabel("Enable Temperature Sensor")
    srvMetConfTemp.setDefaultValue(1)
    srvMetConfTemp.setHelp(srv_met_helpkeyword)

    srvMetConfI0 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_I0", srvMetConfChannels)
    srvMetConfI0.setLabel("Enable Channel I0")
    srvMetConfI0.setDefaultValue(1)
    srvMetConfI0.setHelp(srv_met_helpkeyword)

    srvMetConfI1 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_I1", srvMetConfChannels)
    srvMetConfI1.setLabel("Enable Channel I1")
    srvMetConfI1.setDefaultValue(1)
    srvMetConfI1.setHelp(srv_met_helpkeyword)

    srvMetConfV1 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_V1", srvMetConfChannels)
    srvMetConfV1.setLabel("Enable Channel V1")
    srvMetConfV1.setDefaultValue(1)
    srvMetConfV1.setHelp(srv_met_helpkeyword)

    srvMetConfI2 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_I2", srvMetConfChannels)
    srvMetConfI2.setLabel("Enable Channel I2")
    srvMetConfI2.setDefaultValue(1)
    srvMetConfI2.setHelp(srv_met_helpkeyword)

    srvMetConfV2 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_V2", srvMetConfChannels)
    srvMetConfV2.setLabel("Enable Channel V2")
    srvMetConfV2.setDefaultValue(1)
    srvMetConfV2.setHelp(srv_met_helpkeyword)

    srvMetConfI3 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_I3", srvMetConfChannels)
    srvMetConfI3.setLabel("Enable Channel I3")
    srvMetConfI3.setDefaultValue(0)
    srvMetConfI3.setHelp(srv_met_helpkeyword)

    srvMetConfV3 = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_V3", srvMetConfChannels)
    srvMetConfV3.setLabel("Enable Channel V3")
    srvMetConfV3.setDefaultValue(0)
    srvMetConfV3.setHelp(srv_met_helpkeyword)
    
    srvMetConfByfDef = metComponentCommon.createMenuSymbol("DRV_MET_CONF_BY_DEFAULT", None)
    srvMetConfByfDef.setLabel("Configuration")
    srvMetConfByfDef.setHelp(srv_met_helpkeyword)

    srvMetConfST = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_ST", srvMetConfByfDef)
    srvMetConfST.setLabel("Sensor Type")
    srvMetConfST.setDefaultValue(0)
    srvMetConfST.setOutputMode("Key")
    srvMetConfST.setDisplayMode("Description")
    srvMetConfST.setHelp(srv_met_helpkeyword)
    srvMetConfST.addKey("CT", "0", "Current Transformer")
    srvMetConfST.addKey("SHUNT", "1", "Shunt resistor")
    srvMetConfST.addKey("COIL", "2", "Rogowski Coil")

    srvMetConfMC = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_MC", srvMetConfByfDef)
    srvMetConfMC.setLabel("Meter Constant [Pulses/KWh]")
    srvMetConfMC.setDefaultValue(3200)
    srvMetConfMC.setHelp(srv_met_helpkeyword)

    srvMetConfFreq = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_F", srvMetConfByfDef)
    srvMetConfFreq.setLabel("Mains Frequency (Hz)")
    srvMetConfFreq.setDefaultValue(50)
    srvMetConfFreq.setHelp(srv_met_helpkeyword)

    srvMetConfTr = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_TR", srvMetConfByfDef)
    srvMetConfTr.setLabel("Transformer Ratio")
    srvMetConfTr.setDefaultValue(2000)
    srvMetConfTr.setHelp(srv_met_helpkeyword)
    srvMetConfTr.setDependencies(updateTRLabel, ["DRV_MET_CONF_ST"])

    srvMetConfRl = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_RL", srvMetConfByfDef)
    srvMetConfRl.setLabel("Resistor Load (uOhms)")
    srvMetConfRl.setDefaultValue(3240000)
    srvMetConfRl.setHelp(srv_met_helpkeyword)
    srvMetConfRl.setDependencies(updateRLLabel, ["DRV_MET_CONF_ST"])

    srvMetConfKU = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_KU", srvMetConfByfDef)
    srvMetConfKU.setLabel("Voltage divider ratio")
    srvMetConfKU.setDefaultValue(1651)
    srvMetConfKU.setHelp(srv_met_helpkeyword)

    srvMetConfGain = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_GAIN", srvMetConfByfDef)
    srvMetConfGain.setLabel("Programmable Gain Amplifier")
    srvMetConfGain.setDefaultValue(0)
    srvMetConfGain.setOutputMode("Key")
    srvMetConfGain.setDisplayMode("Description")
    srvMetConfGain.setHelp(srv_met_helpkeyword)
    srvMetConfGain.addKey("GAIN_1", "0", "GAIN_1")
    srvMetConfGain.addKey("GAIN_2", "1", "GAIN_2")
    srvMetConfGain.addKey("GAIN_4", "2", "GAIN_4")
    srvMetConfGain.addKey("GAIN_8", "3", "GAIN_8")

    srvMetConfSwell = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_SWELL", srvMetConfByfDef)
    srvMetConfSwell.setLabel("SWELL Threshold RMS (V)")
    srvMetConfSwell.setDefaultValue(251)
    srvMetConfSwell.setHelp(srv_met_helpkeyword)

    srvMetConfSwell = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_SAG", srvMetConfByfDef)
    srvMetConfSwell.setLabel("SAG Threshold RMS (V)")
    srvMetConfSwell.setDefaultValue(132)
    srvMetConfSwell.setHelp(srv_met_helpkeyword)

    srvMetConfCreepPEn = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_CREEP_P_EN", srvMetConfByfDef)
    srvMetConfCreepPEn.setLabel("Active Power Creep Threshold Enable")
    srvMetConfCreepPEn.setDefaultValue(0)
    srvMetConfCreepPEn.setHelp(srv_met_helpkeyword)

    srvMetConfCreepP = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_CREEP_P", srvMetConfCreepPEn)
    srvMetConfCreepP.setLabel("Creep Active Energy (Wh)")
    srvMetConfCreepP.setDefaultValue(2)
    srvMetConfCreepP.setVisible(False)
    srvMetConfCreepP.setHelp(srv_met_helpkeyword)
    srvMetConfCreepP.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_CONF_CREEP_P_EN"])

    srvMetConfCreepQEn = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_CREEP_Q_EN", srvMetConfByfDef)
    srvMetConfCreepQEn.setLabel("Reactive Power Creep Threshold Enable")
    srvMetConfCreepQEn.setDefaultValue(0)
    srvMetConfCreepQEn.setHelp(srv_met_helpkeyword)

    srvMetConfCreepQ = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_CREEP_Q", srvMetConfCreepQEn)
    srvMetConfCreepQ.setLabel("Creep Reactive Energy (VARh)")
    srvMetConfCreepQ.setDefaultValue(2)
    srvMetConfCreepQ.setVisible(False)
    srvMetConfCreepQ.setHelp(srv_met_helpkeyword)
    srvMetConfCreepQ.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_CONF_CREEP_Q_EN"])

    srvMetConfCreepIEn = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_CREEP_I_EN", srvMetConfByfDef)
    srvMetConfCreepIEn.setLabel("Current Creep Threshold Enable")
    srvMetConfCreepIEn.setDefaultValue(0)
    srvMetConfCreepIEn.setHelp(srv_met_helpkeyword)

    srvMetConfCreepI = metComponentCommon.createIntegerSymbol("DRV_MET_CONF_CREEP_I", srvMetConfCreepIEn)
    srvMetConfCreepI.setLabel("Creep Current (mA)")
    srvMetConfCreepI.setDefaultValue(5)
    srvMetConfCreepI.setVisible(False)
    srvMetConfCreepI.setHelp(srv_met_helpkeyword)
    srvMetConfCreepI.setDependencies(showSymbolOnBoolEvent, ["DRV_MET_CONF_CREEP_I_EN"])
    
    srvMetConfPulse0 = metComponentCommon.createMenuSymbol("DRV_MET_CONF_PULSE0", srvMetConfByfDef)
    srvMetConfPulse0.setLabel("Pulse 0 Control")
    srvMetConfPulse0.setHelp(srv_met_helpkeyword)

    srvMetConfP0En = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_EN_P0", srvMetConfPulse0)
    srvMetConfP0En.setLabel("Output Pulse Enable (PD17)")
    srvMetConfP0En.setDefaultValue(1)
    srvMetConfP0En.setHelp(srv_met_helpkeyword)

    srvMetConfP0Detent = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_DET_P0", srvMetConfPulse0)
    srvMetConfP0Detent.setLabel("Total Absolute Values (P/Q)")
    srvMetConfP0Detent.setDefaultValue(0)
    srvMetConfP0Detent.setOutputMode("Key")
    srvMetConfP0Detent.setDisplayMode("Description")
    srvMetConfP0Detent.setHelp(srv_met_helpkeyword)
    srvMetConfP0Detent.addKey("NET", "0", "NET")
    srvMetConfP0Detent.addKey("ABSOLUTE", "1", "ABSOLUTE")
    srvMetConfP0Detent.addKey("DELIVERED", "2", "DELIVERED")

    srvMetConfP0Polarity = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_POL_P0", srvMetConfPulse0)
    srvMetConfP0Polarity.setLabel("Polarity")
    srvMetConfP0Polarity.setDefaultValue(1)
    srvMetConfP0Polarity.setOutputMode("Key")
    srvMetConfP0Polarity.setDisplayMode("Description")
    srvMetConfP0Polarity.setHelp(srv_met_helpkeyword)
    srvMetConfP0Polarity.addKey("PULSE_LOW", "0", "PULSE_LOW")
    srvMetConfP0Polarity.addKey("PULSE_HIGH", "1", "PULSE_HIGH")

    srvMetConfP0Type = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_TYP_P0", srvMetConfPulse0)
    srvMetConfP0Type.setLabel("Type")
    srvMetConfP0Type.setDefaultValue(0)
    srvMetConfP0Type.setOutputMode("Key")
    srvMetConfP0Type.setDisplayMode("Description")
    srvMetConfP0Type.setHelp(srv_met_helpkeyword)
    srvMetConfP0Type.addKey("P_T", "0", "P_T")
    srvMetConfP0Type.addKey("P_T_F", "1", "P_T_F")
    srvMetConfP0Type.addKey("Q_T", "2", "Q_T")
    srvMetConfP0Type.addKey("Q_T_F", "3", "Q_T_F")
    srvMetConfP0Type.addKey("I_T", "4", "I_T")
    srvMetConfP0Type.addKey("I_T_F", "5", "I_T_F")  

    srvMetConfP0Width = metComponentCommon.createHexSymbol("DRV_MET_CONF_WID_P0", srvMetConfPulse0)
    srvMetConfP0Width.setLabel("Width")
    srvMetConfP0Width.setVisible(True)
    srvMetConfP0Width.setDefaultValue(0x9100)
    srvMetConfP0Width.setHelp(srv_met_helpkeyword)
    
    srvMetConfPulse1 = metComponentCommon.createMenuSymbol("DRV_MET_CONF_PULSE1", srvMetConfByfDef)
    srvMetConfPulse1.setLabel("Pulse 1 Control")
    srvMetConfPulse1.setHelp(srv_met_helpkeyword)

    srvMetConfP1En = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_EN_P1", srvMetConfPulse1)
    srvMetConfP1En.setLabel("Output Pulse Enable (PD18)")
    srvMetConfP1En.setDefaultValue(1)
    srvMetConfP1En.setHelp(srv_met_helpkeyword)

    srvMetConfP1Detent = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_DET_P1", srvMetConfPulse1)
    srvMetConfP1Detent.setLabel("Total Absolute Values (P/Q)")
    srvMetConfP1Detent.setDefaultValue(0)
    srvMetConfP1Detent.setOutputMode("Key")
    srvMetConfP1Detent.setDisplayMode("Description")
    srvMetConfP1Detent.setHelp(srv_met_helpkeyword)
    srvMetConfP1Detent.addKey("NET", "0", "NET")
    srvMetConfP1Detent.addKey("ABSOLUTE", "1", "ABSOLUTE")
    srvMetConfP1Detent.addKey("DELIVERED", "2", "DELIVERED")

    srvMetConfP1Polarity = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_POL_P1", srvMetConfPulse1)
    srvMetConfP1Polarity.setLabel("Polarity")
    srvMetConfP1Polarity.setDefaultValue(1)
    srvMetConfP1Polarity.setOutputMode("Key")
    srvMetConfP1Polarity.setDisplayMode("Description")
    srvMetConfP1Polarity.setHelp(srv_met_helpkeyword)
    srvMetConfP1Polarity.addKey("PULSE_LOW", "0", "PULSE_LOW")
    srvMetConfP1Polarity.addKey("PULSE_HIGH", "1", "PULSE_HIGH")

    srvMetConfP1Type = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_TYP_P1", srvMetConfPulse1)
    srvMetConfP1Type.setLabel("Type")
    srvMetConfP1Type.setDefaultValue(2)
    srvMetConfP1Type.setOutputMode("Key")
    srvMetConfP1Type.setDisplayMode("Description")
    srvMetConfP1Type.setHelp(srv_met_helpkeyword)
    srvMetConfP1Type.addKey("P_T", "0", "P_T")
    srvMetConfP1Type.addKey("P_T_F", "1", "P_T_F")
    srvMetConfP1Type.addKey("Q_T", "2", "Q_T")
    srvMetConfP1Type.addKey("Q_T_F", "3", "Q_T_F")
    srvMetConfP1Type.addKey("I_T", "4", "I_T")
    srvMetConfP1Type.addKey("I_T_F", "5", "I_T_F")    

    srvMetConfP1Width = metComponentCommon.createHexSymbol("DRV_MET_CONF_WID_P1", srvMetConfPulse1)
    srvMetConfP1Width.setLabel("Width")
    srvMetConfP1Width.setVisible(True)
    srvMetConfP1Width.setDefaultValue(0x9100)
    srvMetConfP1Width.setHelp(srv_met_helpkeyword)
    
    srvMetConfPulse2 = metComponentCommon.createMenuSymbol("DRV_MET_CONF_PULSE2", srvMetConfByfDef)
    srvMetConfPulse2.setLabel("Pulse 2 Control")
    srvMetConfPulse2.setHelp(srv_met_helpkeyword)

    srvMetConfP2En = metComponentCommon.createBooleanSymbol("DRV_MET_CONF_EN_P2", srvMetConfPulse2)
    srvMetConfP2En.setLabel("Output Pulse Enable (PD19)")
    srvMetConfP2En.setDefaultValue(0)
    srvMetConfP2En.setHelp(srv_met_helpkeyword)

    srvMetConfP2Detent = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_DET_P2", srvMetConfPulse2)
    srvMetConfP2Detent.setLabel("Total Absolute Values (P/Q)")
    srvMetConfP2Detent.setDefaultValue(0)
    srvMetConfP2Detent.setOutputMode("Key")
    srvMetConfP2Detent.setDisplayMode("Description")
    srvMetConfP2Detent.setHelp(srv_met_helpkeyword)
    srvMetConfP2Detent.addKey("NET", "0", "NET")
    srvMetConfP2Detent.addKey("ABSOLUTE", "1", "ABSOLUTE")
    srvMetConfP2Detent.addKey("DELIVERED", "2", "DELIVERED")

    srvMetConfP2Polarity = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_POL_P2", srvMetConfPulse2)
    srvMetConfP2Polarity.setLabel("Polarity")
    srvMetConfP2Polarity.setDefaultValue(0)
    srvMetConfP2Polarity.setOutputMode("Key")
    srvMetConfP2Polarity.setDisplayMode("Description")
    srvMetConfP2Polarity.setHelp(srv_met_helpkeyword)
    srvMetConfP2Polarity.addKey("PULSE_LOW", "0", "PULSE_LOW")
    srvMetConfP2Polarity.addKey("PULSE_HIGH", "1", "PULSE_HIGH")

    srvMetConfP2Type = metComponentCommon.createKeyValueSetSymbol("DRV_MET_CONF_TYP_P2", srvMetConfPulse2)
    srvMetConfP2Type.setLabel("Type")
    srvMetConfP2Type.setDefaultValue(0)
    srvMetConfP2Type.setOutputMode("Key")
    srvMetConfP2Type.setDisplayMode("Description")
    srvMetConfP2Type.setHelp(srv_met_helpkeyword)
    srvMetConfP2Type.addKey("P_T", "0", "P_T")
    srvMetConfP2Type.addKey("P_T_F", "1", "P_T_F")
    srvMetConfP2Type.addKey("Q_T", "2", "Q_T")
    srvMetConfP2Type.addKey("Q_T_F", "3", "Q_T_F")
    srvMetConfP2Type.addKey("I_T", "4", "I_T")
    srvMetConfP2Type.addKey("I_T_F", "5", "I_T_F")    

    srvMetConfP2Width = metComponentCommon.createHexSymbol("DRV_MET_CONF_WID_P2", srvMetConfPulse2)
    srvMetConfP2Width.setLabel("Width")
    srvMetConfP2Width.setVisible(True)
    srvMetConfP2Width.setDefaultValue(0x0000)
    srvMetConfP2Width.setHelp(srv_met_helpkeyword)

    #####################################################################################################################################
    # METROLOGY REGISTERS 
    
    srvMetRegPKT = metComponentCommon.createHexSymbol("DRV_MET_CTRL_PKT", None)
    srvMetRegPKT.setLabel("PKT")
    srvMetRegPKT.setVisible(True)
    srvMetRegPKT.setDefaultValue(0x00500000)
    srvMetRegPKT.setReadOnly(True)
    srvMetRegPKT.setDependencies(updateConfigPKT, ["DRV_MET_CONF_MC"])

    srvMetRegMT = metComponentCommon.createHexSymbol("DRV_MET_CTRL_MT", None)
    srvMetRegMT.setLabel("MT")
    srvMetRegMT.setVisible(True)
    srvMetRegMT.setDefaultValue(0x00000CCC)
    srvMetRegMT.setReadOnly(True)
    srvMetRegMT.setDependencies(updateConfigMT, ["DRV_MET_CONF_ST"])

    srvMetRegKI = metComponentCommon.createHexSymbol("DRV_MET_CTRL_KI", None)
    srvMetRegKI.setLabel("KI")
    srvMetRegKI.setVisible(True)
    srvMetRegKI.setDefaultValue(0x0009A523)
    srvMetRegKI.setReadOnly(True)
    srvMetRegKI.setDependencies(updateConfigKI, ["DRV_MET_CONF_F", "DRV_MET_CONF_TR", "DRV_MET_CONF_RL", "DRV_MET_CONF_GAIN"])

    srvMetRegKV = metComponentCommon.createHexSymbol("DRV_MET_CTRL_KV", None)
    srvMetRegKV.setLabel("KV")
    srvMetRegKV.setVisible(True)
    srvMetRegKV.setDefaultValue(0x0019CC00)
    srvMetRegKV.setReadOnly(True)
    srvMetRegKV.setDependencies(updateConfigKV, ["DRV_MET_CONF_KU"])

    srvMetRegATS2023 = metComponentCommon.createHexSymbol("DRV_MET_CTRL_ATS2023", None)
    srvMetRegATS2023.setLabel("ATS2023")
    srvMetRegATS2023.setVisible(True)
    srvMetRegATS2023.setDefaultValue(0x01010103)
    srvMetRegATS2023.setReadOnly(True)
    srvMetRegATS2023.setDependencies(updateConfigATS2023, ["DRV_MET_CONF_GAIN", "DRV_MET_CONF_I0", "DRV_MET_CONF_I1", "DRV_MET_CONF_V1", "DRV_MET_CONF_I2", "DRV_MET_CONF_TEMP"])

    srvMetRegATS2427 = metComponentCommon.createHexSymbol("DRV_MET_CTRL_ATS2427", None)
    srvMetRegATS2427.setLabel("ATS2427")
    srvMetRegATS2427.setVisible(True)
    srvMetRegATS2427.setDefaultValue(0x07000001)
    srvMetRegATS2427.setReadOnly(True)
    srvMetRegATS2427.setDependencies(updateConfigATS2427, ["DRV_MET_CONF_GAIN", "DRV_MET_CONF_V2", "DRV_MET_CONF_I3", "DRV_MET_CONF_V3"])

    srvMetRegSWELL = metComponentCommon.createHexSymbol("DRV_MET_CTRL_SWELL", None)
    srvMetRegSWELL.setLabel("SWELL")
    srvMetRegSWELL.setVisible(True)
    srvMetRegSWELL.setDefaultValue(0x5eab918)
    srvMetRegSWELL.setReadOnly(True)
    srvMetRegSWELL.setDependencies(updateConfigSwellSag, ["DRV_MET_CONF_SWELL", "DRV_MET_CONF_KU"])

    srvMetRegSAG = metComponentCommon.createHexSymbol("DRV_MET_CTRL_SAG", None)
    srvMetRegSAG.setLabel("SAG")
    srvMetRegSAG.setVisible(True)
    srvMetRegSAG.setDefaultValue(0x1a2ec26)
    srvMetRegSAG.setReadOnly(True)
    srvMetRegSAG.setDependencies(updateConfigSwellSag, ["DRV_MET_CONF_SAG"])

    srvMetRegCreepP = metComponentCommon.createHexSymbol("DRV_MET_CTRL_CREEP_P", None)
    srvMetRegCreepP.setLabel("CREEP_P")
    srvMetRegCreepP.setVisible(True)
    srvMetRegCreepP.setDefaultValue(0x00002E9A)
    srvMetRegCreepP.setReadOnly(True)
    srvMetRegCreepP.setDependencies(updateConfigCreepPQ, ["DRV_MET_CONF_CREEP_P", "DRV_MET_CONF_F"])

    srvMetRegCreepQ = metComponentCommon.createHexSymbol("DRV_MET_CTRL_CREEP_Q", None)
    srvMetRegCreepQ.setLabel("CREEP_Q")
    srvMetRegCreepQ.setVisible(True)
    srvMetRegCreepQ.setDefaultValue(0x00002E9A)
    srvMetRegCreepQ.setReadOnly(True)
    srvMetRegCreepQ.setDependencies(updateConfigCreepPQ, ["DRV_MET_CONF_CREEP_Q", "DRV_MET_CONF_F"])

    srvMetRegCreepI = metComponentCommon.createHexSymbol("DRV_MET_CTRL_CREEP_I", None)
    srvMetRegCreepI.setLabel("CREEP_I")
    srvMetRegCreepI.setVisible(True)
    srvMetRegCreepI.setDefaultValue(0x0000212D)
    srvMetRegCreepI.setReadOnly(True)
    srvMetRegCreepI.setDependencies(updateConfigCreepI, ["DRV_MET_CONF_CREEP_I", "DRV_MET_CONF_F", "DRV_MET_CONF_TR", "DRV_MET_CONF_RL", "DRV_MET_CONF_GAIN"])

    srvMetRegFEATCTRL0 = metComponentCommon.createHexSymbol("DRV_MET_CTRL_FEATCTRL0", None)
    srvMetRegFEATCTRL0.setLabel("FEATURE_CTRL0")
    srvMetRegFEATCTRL0.setVisible(True)
    srvMetRegFEATCTRL0.setDefaultValue(0x00000300)
    srvMetRegFEATCTRL0.setReadOnly(True)
    srvMetRegFEATCTRL0.setDependencies(updateConfigFeatCtrl0, ["DRV_MET_CONF_I1", "DRV_MET_CONF_V1", "DRV_MET_CONF_I2", "DRV_MET_CONF_V2", "DRV_MET_CONF_I3", "DRV_MET_CONF_V3"])

    srvMetRegFEATCTRL1 = metComponentCommon.createHexSymbol("DRV_MET_CTRL_FEATCTRL1", None)
    srvMetRegFEATCTRL1.setLabel("FEATURE_CTRL1")
    srvMetRegFEATCTRL1.setVisible(True)
    srvMetRegFEATCTRL1.setDefaultValue(0)
    srvMetRegFEATCTRL1.setReadOnly(True)
    srvMetRegFEATCTRL1.setDependencies(updateConfigFeatCtrl1, ["DRV_MET_CONF_CREEP_P_EN", "DRV_MET_CONF_CREEP_Q_EN", "DRV_MET_CONF_CREEP_I_EN"])

    srvMetRegPulse0Ctrl = metComponentCommon.createHexSymbol("DRV_MET_CTRL_PULSE_CTRL_0", None)
    srvMetRegPulse0Ctrl.setLabel("PULSE0_CTRL")
    srvMetRegPulse0Ctrl.setVisible(True)
    srvMetRegPulse0Ctrl.setDefaultValue(0x81009100)
    srvMetRegPulse0Ctrl.setReadOnly(True)
    srvMetRegPulse0Ctrl.setDependencies(updateConfigPulseXCtrl, ["DRV_MET_CONF_EN_P0", "DRV_MET_CONF_DET_P0", "DRV_MET_CONF_POL_P0", "DRV_MET_CONF_TYP_P0", "DRV_MET_CONF_WID_P0"])

    srvMetRegPulse1Ctrl = metComponentCommon.createHexSymbol("DRV_MET_CTRL_PULSE_CTRL_1", None)
    srvMetRegPulse1Ctrl.setLabel("PULSE1_CTRL")
    srvMetRegPulse1Ctrl.setVisible(True)
    srvMetRegPulse1Ctrl.setDefaultValue(0x81029100)
    srvMetRegPulse1Ctrl.setReadOnly(True)
    srvMetRegPulse1Ctrl.setDependencies(updateConfigPulseXCtrl, ["DRV_MET_CONF_EN_P1", "DRV_MET_CONF_DET_P1", "DRV_MET_CONF_POL_P1", "DRV_MET_CONF_TYP_P1", "DRV_MET_CONF_WID_P1"])

    srvMetRegPulse2Ctrl = metComponentCommon.createHexSymbol("DRV_MET_CTRL_PULSE_CTRL_2", None)
    srvMetRegPulse2Ctrl.setLabel("PULSE2_CTRL")
    srvMetRegPulse2Ctrl.setVisible(True)
    srvMetRegPulse2Ctrl.setDefaultValue(0x0)
    srvMetRegPulse2Ctrl.setReadOnly(True)
    srvMetRegPulse2Ctrl.setDependencies(updateConfigPulseXCtrl, ["DRV_MET_CONF_EN_P2", "DRV_MET_CONF_DET_P2", "DRV_MET_CONF_POL_P2", "DRV_MET_CONF_TYP_P2", "DRV_MET_CONF_WID_P2"])
    
    #####################################################################################################################################
    # METROLOGY FILES 

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
    srvMetHeaderFileDefs.setSourcePath("driver/metrology/drv_metrology_definitions.h.ftl")
    srvMetHeaderFileDefs.setOutputName("drv_metrology_definitions.h")
    srvMetHeaderFileDefs.setDestPath("driver/metrology")
    srvMetHeaderFileDefs.setProjectPath("config/" + configName + "/driver/metrology/")
    srvMetHeaderFileDefs.setType("HEADER")
    srvMetHeaderFileDefs.setOverwrite(True)
    srvMetHeaderFileDefs.setMarkup(True)

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
