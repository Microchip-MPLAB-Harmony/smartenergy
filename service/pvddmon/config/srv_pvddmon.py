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
import math 

global pPVDDMonHighThrsHex
global pPVDDMonLowThrsHex
global pPVDDMonHighThrsHexHyst
global pPVDDMonLowThrsHexHyst

srv_pvddmon_helpkeyword = "mcc_h3_srv_pvddmon_configurations"

def handleMessage(messageID, args):
    result_dict = {}

    return result_dict

def getNewADCThresholds(symbol, event):
    print("PLC PVDD Monitor Service: Getting new threshold levels")

    maxValue = pow(2, Database.getSymbolValue("srv_pvddmon", "SRV_PVDDMON_ADC_BITS")) - 1

    HighThrs = Database.getSymbolValue("srv_pvddmon", "SRV_PVDDMON_HIGH_TH")
    LowThrs = Database.getSymbolValue("srv_pvddmon", "SRV_PVDDMON_LOW_TH")
    ResUp = Database.getSymbolValue("srv_pvddmon", "SRV_PVDDMON_RES_UP")
    ResDown = Database.getSymbolValue("srv_pvddmon", "SRV_PVDDMON_RES_DOWN")
    Hysteresis = Database.getSymbolValue("srv_pvddmon", "SRV_PVDDMON_HIST")

    VoutHigh = HighThrs * ResDown / (ResUp + ResDown)
    VoutLow = LowThrs * ResDown / (ResUp + ResDown)
    VoutHighHyst = (HighThrs - Hysteresis) * ResDown / (ResUp + ResDown)
    VoutLowHyst = (LowThrs + Hysteresis) * ResDown / (ResUp + ResDown)
    
    HighThresholdHex = hex(int(math.ceil(VoutHigh * maxValue / 3300)))
    LowThresholdHex = hex(int(math.ceil(VoutLow * maxValue / 3300)))
    HighThresholdHexHyst = hex(int(math.ceil(VoutHighHyst * maxValue / 3300)))
    LowThresholdHexHyst = hex(int(math.ceil(VoutLowHyst * maxValue / 3300)))
    
    pPVDDMonHighThrsHex.setValue(HighThresholdHex)
    pPVDDMonLowThrsHex.setValue(LowThresholdHex)
    pPVDDMonHighThrsHexHyst.setValue(HighThresholdHexHyst)
    pPVDDMonLowThrsHexHyst.setValue(LowThresholdHexHyst)


def setPlibValue(symbol, event):
    symbol.setValue(event["value"])

def identifyPeripherals(component):
    periphNode = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals")
    peripherals = periphNode.getChildren()

    flexcomIdCreated = False
    adcIdCreated = False
    pioIdCreated = False

    for module in range (0, len(peripherals)):
        if str(peripherals[module].getAttribute("name")) == "FLEXCOM":
            plcFlexcomId = component.createIntegerSymbol("PLC_FLEXCOM_ID", None)
            plcFlexcomId.setDefaultValue(int(peripherals[module].getAttribute("id")))
            plcFlexcomId.setVisible(False)
            flexcomIdCreated = True
        elif str(peripherals[module].getAttribute("name")) == "ADC":
            plcAdcId = component.createIntegerSymbol("PLC_ADC_ID", None)
            plcAdcId.setDefaultValue(int(peripherals[module].getAttribute("id")))
            plcAdcId.setVisible(False)
            adcIdCreated = True
        elif str(peripherals[module].getAttribute("name")) == "PIO":
            plcPioId = component.createIntegerSymbol("PLC_PIO_ID", None)
            plcPioId.setDefaultValue(int(peripherals[module].getAttribute("id")))
            plcPioId.setVisible(False)
            pioIdCreated = True
    
    if not flexcomIdCreated:
        plcFlexcomId = component.createIntegerSymbol("PLC_FLEXCOM_ID", None)
        plcFlexcomId.setDefaultValue(0)
        plcFlexcomId.setVisible(False)
    if not adcIdCreated:
        plcAdcId = component.createIntegerSymbol("PLC_ADC_ID", None)
        plcAdcId.setDefaultValue(0)
        plcAdcId.setVisible(False)
    if not pioIdCreated:
        plcPioId = component.createIntegerSymbol("PLC_PIO_ID", None)
        plcPioId.setDefaultValue(0)
        plcPioId.setVisible(False)

def instantiateComponent(pPVDDMonComponent):
    
    print("Loading PLC PVDD Monitor Service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    pPVDDMonPlib = pPVDDMonComponent.createStringSymbol("SRV_PVDDMON_PLIB", None)
    pPVDDMonPlib.setLabel("Peripheral lib")
    pPVDDMonPlib.setReadOnly(True)
    pPVDDMonPlib.setHelp(srv_pvddmon_helpkeyword)

    pVddHighThrs = 13000
    pVddLowThrs = 10000
    pVddResolution = 12
    ResUp = 36000
    ResDown = 10000
    Hysteresis = 100

    pvddmon_node = ATDF.getNode('/avr-tools-device-file/devices/device/peripherals/module@[name="ADC"]')
    if (pvddmon_node == None):
        pvddmon_node = ATDF.getNode('/avr-tools-device-file/devices/device/peripherals/module@[name="AFEC"]')
        if (pvddmon_node == None):
            pvddmon_plib = ""
        else:
            pvddmon_plib = "AFEC"
    else:            
        pvddmon_plib = "ADC"

    identifyPeripherals(pPVDDMonComponent)

    pPVDDMonADCChannel = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_ADC_CHANNEL", pPVDDMonPlib)
    pPVDDMonADCChannel.setLabel("Channel")
    pPVDDMonADCChannel.setDefaultValue(0)
    pPVDDMonADCChannel.setHelp(srv_pvddmon_helpkeyword)

    pPVDDMonADCResolution = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_ADC_BITS", pPVDDMonPlib)
    pPVDDMonADCResolution.setLabel("Result Resolution bits")
    pPVDDMonADCResolution.setDefaultValue(pVddResolution)
    pPVDDMonADCChannel.setHelp(srv_pvddmon_helpkeyword)

    pPVDDMonADCComment1 = pPVDDMonComponent.createCommentSymbol("SRV_PVDDMON_ADC_COMMENT1", pPVDDMonPlib)
    pPVDDMonADCComment1.setLabel("**** ADC must be manually configured in FreeRun mode.")
    pPVDDMonADCComment1.setVisible(True)

    pPVDDMonADCComment2 = pPVDDMonComponent.createCommentSymbol("SRV_PVDDMON_ADC_COMMENT2", pPVDDMonPlib)
    pPVDDMonADCComment2.setLabel("**** Enable manually ADC End of conversion interrupt in the channel configuration.")
    pPVDDMonADCComment2.setVisible(True)

    if (pvddmon_plib == ""):
        pPVDDMonADCComment3 = pPVDDMonComponent.createCommentSymbol("SRV_PVDDMON_ADC_COMMENT3", None)
        pPVDDMonADCComment3.setLabel("**** CRITICAL ERROR. ADC peripheral has not been detected.")
        pPVDDMonADCComment3.setVisible(True)
    
    maxValue = pow(2, pVddResolution) - 1
    VoutHigh = pVddHighThrs * ResDown / (ResUp + ResDown)
    VoutLow = pVddLowThrs * ResDown / (ResUp + ResDown)
    VoutHighHyst = (pVddHighThrs - Hysteresis) * ResDown / (ResUp + ResDown)
    VoutLowHyst = (pVddLowThrs + Hysteresis) * ResDown / (ResUp + ResDown)

    HighThresholdHex = hex(int(math.ceil(VoutHigh * maxValue / 3300)))
    LowThresholdHex = hex(int(math.ceil(VoutLow * maxValue / 3300)))
    HighThresholdHexHyst = hex(int(math.ceil(VoutHighHyst * maxValue / 3300)))
    LowThresholdHexHyst = hex(int(math.ceil(VoutLowHyst * maxValue / 3300)))

    global pPVDDMonHighThrsHex
    pPVDDMonHighThrsHex = pPVDDMonComponent.createStringSymbol("SRV_PVDDMON_HIGH_THRESHOLD_HEX", None)
    pPVDDMonHighThrsHex.setLabel("High Threshold [Hex]")
    pPVDDMonHighThrsHex.setDefaultValue(HighThresholdHex)
    pPVDDMonHighThrsHex.setDependencies(getNewADCThresholds, ["SRV_PVDDMON_HIGH_TH", "SRV_PVDDMON_LOW_TH", "SRV_PVDDMON_ADC_BITS"])
    pPVDDMonHighThrsHex.setVisible(False)
    
    global pPVDDMonLowThrsHex
    pPVDDMonLowThrsHex = pPVDDMonComponent.createStringSymbol("SRV_PVDDMON_LOW_THRESHOLD_HEX", None)
    pPVDDMonLowThrsHex.setLabel("Low Threshold [Hex]")
    pPVDDMonLowThrsHex.setDefaultValue(LowThresholdHex)
    pPVDDMonLowThrsHex.setVisible(False)

    global pPVDDMonHighThrsHexHyst
    pPVDDMonHighThrsHexHyst = pPVDDMonComponent.createStringSymbol("SRV_PVDDMON_HIGH_THRESHOLD_HEX_HYST", None)
    pPVDDMonHighThrsHexHyst.setVisible(False)
    pPVDDMonHighThrsHexHyst.setDefaultValue(HighThresholdHexHyst)
    
    global pPVDDMonLowThrsHexHyst
    pPVDDMonLowThrsHexHyst = pPVDDMonComponent.createStringSymbol("SRV_PVDDMON_LOW_THRESHOLD_HEX_HYST", None)
    pPVDDMonLowThrsHexHyst.setVisible(False)
    pPVDDMonLowThrsHexHyst.setDefaultValue(LowThresholdHexHyst)

    pPVDDMonComWinRes = pPVDDMonComponent.createCommentSymbol("SRV_PVDDMON_RES", None)
    pPVDDMonComWinRes.setLabel("Voltage Monitor scheme")
    pPVDDMonComWinRes.setVisible(True)
    pPVDDMonComWinRes.setHelp(srv_pvddmon_helpkeyword)

    pPVDDMonComWinResUp = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_RES_UP", pPVDDMonComWinRes)
    pPVDDMonComWinResUp.setLabel("Resistor Up [Ohms]")
    pPVDDMonComWinResUp.setDefaultValue(ResUp)
    pPVDDMonComWinResUp.setHelp(srv_pvddmon_helpkeyword)

    pPVDDMonComWinResDown = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_RES_DOWN", pPVDDMonComWinRes)
    pPVDDMonComWinResDown.setLabel("Resistor Down [Ohms]")
    pPVDDMonComWinResDown.setDefaultValue(ResDown)
    pPVDDMonComWinResDown.setHelp(srv_pvddmon_helpkeyword)

    pPVDDMonComWinThrs = pPVDDMonComponent.createCommentSymbol("SRV_PVDDMON_PVDD", None)
    pPVDDMonComWinThrs.setLabel("Comparison Window thresholds")
    pPVDDMonComWinThrs.setVisible(True)
    pPVDDMonComWinThrs.setHelp(srv_pvddmon_helpkeyword)

    pPVDDMonHighThreshold = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_HIGH_TH", pPVDDMonComWinThrs)
    pPVDDMonHighThreshold.setLabel("High Threshold [mV]")
    pPVDDMonHighThreshold.setDefaultValue(pVddHighThrs)
    pPVDDMonHighThreshold.setHelp(srv_pvddmon_helpkeyword)

    pPVDDMonLowThreshold = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_LOW_TH", pPVDDMonComWinThrs)
    pPVDDMonLowThreshold.setLabel("Low Threshold [mV]")
    pPVDDMonLowThreshold.setDefaultValue(pVddLowThrs)
    pPVDDMonLowThreshold.setHelp(srv_pvddmon_helpkeyword)

    pPVDDMonHisteresys = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_HIST", pPVDDMonComWinThrs)
    pPVDDMonHisteresys.setLabel("Histeresys level [mV]")
    pPVDDMonHisteresys.setDefaultValue(Hysteresis)
    pPVDDMonHisteresys.setHelp(srv_pvddmon_helpkeyword)

    # PLC PVDD Monitor Files
    pPVDDMonHeaderFile = pPVDDMonComponent.createFileSymbol("SRV_PVDDMON_HEADER", None)
    pPVDDMonHeaderFile.setSourcePath("service/pvddmon/templates/srv_pvddmon.h.ftl")
    pPVDDMonHeaderFile.setOutputName("srv_pvddmon.h")
    pPVDDMonHeaderFile.setDestPath("service/pvddmon")
    pPVDDMonHeaderFile.setProjectPath("config/" + configName + "/service/pvddmon/")
    pPVDDMonHeaderFile.setType("HEADER")
    pPVDDMonHeaderFile.setMarkup(True)

    pPVDDMonSourceFile = pPVDDMonComponent.createFileSymbol("SRV_PVDDMON_SOURCE", None)
    pPVDDMonSourceFile.setSourcePath("service/pvddmon/templates/srv_pvddmon.c.ftl")
    pPVDDMonSourceFile.setOutputName("srv_pvddmon.c")
    pPVDDMonSourceFile.setDestPath("service/pvddmon")
    pPVDDMonSourceFile.setProjectPath("config/" + configName + "/service/pvddmon/")
    pPVDDMonSourceFile.setType("SOURCE")
    pPVDDMonSourceFile.setMarkup(True)

    pPVDDMonSystemDefIncFile = pPVDDMonComponent.createFileSymbol("SRV_PVDDMON_SYSTEM_DEF", None)
    pPVDDMonSystemDefIncFile.setType("STRING")
    pPVDDMonSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    pPVDDMonSystemDefIncFile.setSourcePath("service/pvddmon/templates/system/system_definitions.h.ftl")
    pPVDDMonSystemDefIncFile.setMarkup(True)

    pPVDDMonSystemInitFile = pPVDDMonComponent.createFileSymbol("SRV_PVDDMON_INIT", None)
    pPVDDMonSystemInitFile.setType("STRING")
    pPVDDMonSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    pPVDDMonSystemInitFile.setSourcePath("service/pvddmon/templates/system/initialize.c.ftl")
    pPVDDMonSystemInitFile.setMarkup(True)

################################################################################
#### Business Logic ####
################################################################################  
def onAttachmentConnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    if connectID == "srv_pvddmon_ADC_dependency" :
        plibUsed = localComponent.getSymbolByID("SRV_PVDDMON_PLIB")
        plibUsed.clearValue()
        plibUsed.setValue(remoteID.upper())

def onAttachmentDisconnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    if connectID == "srv_pvddmon_ADC_dependency" :
        localComponent.getSymbolByID("SRV_PVDDMON_PLIB").clearValue()
        dummyDict = {}