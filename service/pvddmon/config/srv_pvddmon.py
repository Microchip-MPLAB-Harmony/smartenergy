# coding: utf-8
"""
Copyright (C) 2021, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
"""
import math

global pPVDDMonHighThrsHex
global pPVDDMonLowThrsHex
global pPVDDMonHighThrsHexHyst
global pPVDDMonLowThrsHexHyst

global plcAdcId

srv_pvddmon_helpkeyword = "mcc_h3_srv_pvddmon_configurations"

def handleMessage(messageID, args):
    result_dict = {}

    return result_dict

def getNewADCThresholds(symbol, event):
    global plcAdcId

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

    adcPeriphId = plcAdcId.getValue()
    if (adcPeriphId == "U2204"):
        vref = 1650
    else:
        vref = 3300

    HighThresholdHex = hex(int(math.ceil(VoutHigh * maxValue / vref)))
    LowThresholdHex = hex(int(math.ceil(VoutLow * maxValue / vref)))
    HighThresholdHexHyst = hex(int(math.ceil(VoutHighHyst * maxValue / vref)))
    LowThresholdHexHyst = hex(int(math.ceil(VoutLowHyst * maxValue / vref)))

    pPVDDMonHighThrsHex.setValue(HighThresholdHex)
    pPVDDMonLowThrsHex.setValue(LowThresholdHex)
    pPVDDMonHighThrsHexHyst.setValue(HighThresholdHexHyst)
    pPVDDMonLowThrsHexHyst.setValue(LowThresholdHexHyst)


def setPlibValue(symbol, event):
    symbol.setValue(event["value"])

def identifyPeripherals(component):
    global plcAdcId

    periphNode = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals")
    peripherals = periphNode.getChildren()

    flexcomIdCreated = False
    adcIdCreated = False
    pioIdCreated = False
    eicIdCreated = False

    for module in range (0, len(peripherals)):
        if str(peripherals[module].getAttribute("name")) == "FLEXCOM":
            plcFlexcomId = component.createStringSymbol("PLC_FLEXCOM_ID", None)
            plcFlexcomId.setDefaultValue(str(peripherals[module].getAttribute("id")))
            flexcomIdCreated = True
        elif str(peripherals[module].getAttribute("name")) in ["ADC", "AFEC"]:
            plcAdcId = component.createStringSymbol("PLC_ADC_ID", None)
            plcAdcId.setDefaultValue(str(peripherals[module].getAttribute("id")))
            adcIdCreated = True
        elif str(peripherals[module].getAttribute("name")) == "PIO":
            plcPioId = component.createStringSymbol("PLC_PIO_ID", None)
            plcPioId.setDefaultValue(str(peripherals[module].getAttribute("id")))
            pioIdCreated = True
        elif str(peripherals[module].getAttribute("name")) == "EIC":
            plcEicId = component.createStringSymbol("PLC_EIC_ID", None)
            plcEicId.setDefaultValue(str(peripherals[module].getAttribute("id")))
            eicIdCreated = True

    if not flexcomIdCreated:
        plcFlexcomId = component.createStringSymbol("PLC_FLEXCOM_ID", None)
        plcFlexcomId.setDefaultValue("0")
    if not adcIdCreated:
        plcAdcId = component.createStringSymbol("PLC_ADC_ID", None)
        plcAdcId.setDefaultValue("0")
    if not pioIdCreated:
        plcPioId = component.createStringSymbol("PLC_PIO_ID", None)
        plcPioId.setDefaultValue("0")
    if not eicIdCreated:
        plcEicId = component.createStringSymbol("PLC_EIC_ID", None)
        plcEicId.setDefaultValue("0")

    plcFlexcomId.setVisible(False)
    plcAdcId.setVisible(False)
    plcPioId.setVisible(False)
    plcEicId.setVisible(False)

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

    adcPeriphId = plcAdcId.getValue()
    if not (adcPeriphId in ["U2204", "44134", "11147"]):
        print("PLC PVDD Monitor Service is not supported for " + pvddmon_plib + " Plib")
        print("CHRIS dbg-> adcPeriphId: {}".format(adcPeriphId))
        return

    if (adcPeriphId != "U2204"):
        pPVDDMonADCChannel = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_ADC_CHANNEL", pPVDDMonPlib)
        pPVDDMonADCChannel.setLabel("Channel")
        pPVDDMonADCChannel.setDefaultValue(0)
        pPVDDMonADCChannel.setHelp(srv_pvddmon_helpkeyword)

    pPVDDMonADCResolution = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_ADC_BITS", pPVDDMonPlib)
    pPVDDMonADCResolution.setLabel("Result Resolution bits")
    pPVDDMonADCResolution.setDefaultValue(pVddResolution)
    pPVDDMonADCResolution.setHelp(srv_pvddmon_helpkeyword)

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

    if (adcPeriphId == "U2204"):
        vref = 1650
    else:
        vref = 3300

    HighThresholdHex = hex(int(math.ceil(VoutHigh * maxValue / vref)))
    LowThresholdHex = hex(int(math.ceil(VoutLow * maxValue / vref)))
    HighThresholdHexHyst = hex(int(math.ceil(VoutHighHyst * maxValue / vref)))
    LowThresholdHexHyst = hex(int(math.ceil(VoutLowHyst * maxValue / vref)))

    global pPVDDMonHighThrsHex
    pPVDDMonHighThrsHex = pPVDDMonComponent.createStringSymbol("SRV_PVDDMON_HIGH_THRESHOLD_HEX", None)
    pPVDDMonHighThrsHex.setDefaultValue(HighThresholdHex)
    pPVDDMonHighThrsHex.setDependencies(getNewADCThresholds, ["SRV_PVDDMON_HIGH_TH", "SRV_PVDDMON_LOW_TH", "SRV_PVDDMON_ADC_BITS", "SRV_PVDDMON_RES_UP", "SRV_PVDDMON_RES_DOWN"])
    pPVDDMonHighThrsHex.setVisible(False)

    global pPVDDMonLowThrsHex
    pPVDDMonLowThrsHex = pPVDDMonComponent.createStringSymbol("SRV_PVDDMON_LOW_THRESHOLD_HEX", None)
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
    if (adcPeriphId == "U2204"):
        pPVDDMonHeaderFile = pPVDDMonComponent.createFileSymbol("SRV_PVDDMON_HEADER", None)
        pPVDDMonHeaderFile.setSourcePath("service/pvddmon/templates/srv_pvddmon_u2204.h.ftl")
        pPVDDMonHeaderFile.setOutputName("srv_pvddmon.h")
        pPVDDMonHeaderFile.setDestPath("service/pvddmon")
        pPVDDMonHeaderFile.setProjectPath("config/" + configName + "/service/pvddmon/")
        pPVDDMonHeaderFile.setType("HEADER")
        pPVDDMonHeaderFile.setMarkup(True)

        pPVDDMonSourceFile = pPVDDMonComponent.createFileSymbol("SRV_PVDDMON_SOURCE", None)
        pPVDDMonSourceFile.setSourcePath("service/pvddmon/srv_pvddmon_u2204.c")
        pPVDDMonSourceFile.setOutputName("srv_pvddmon.c")
        pPVDDMonSourceFile.setDestPath("service/pvddmon")
        pPVDDMonSourceFile.setProjectPath("config/" + configName + "/service/pvddmon/")
        pPVDDMonSourceFile.setType("SOURCE")
    else:
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