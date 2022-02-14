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

def handleMessage(messageID, args):
    result_dict = {}

    return result_dict

def getNewADCThresholds(symbol, event):
    print("PLC PVDD Monitor Service: Getting new threshold levels")

    maxValue = pow(2, Database.getSymbolValue("srv_pvddmon", "SRV_PVDDMON_ADC_BITS")) - 1

    HighThrs = Database.getSymbolValue("srv_pvddmon", "SRV_PVDDMON_HIGH_TH")
    LowThrs = Database.getSymbolValue("srv_pvddmon", "SRV_PVDDMON_LOW_TH")

    VoutHigh = HighThrs * 10.0 / (10 + 36)
    VoutLow = LowThrs * 10.0 / (10 + 36)
    VoutHighHyst = (HighThrs - 100) * 10.0 / (10 + 36)
    VoutLowHyst = (LowThrs + 100) * 10.0 / (10 + 36)

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

def instantiateComponent(pPVDDMonComponent):
    
    print("Loading PLC PVDD Monitor Service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    pVddValue = 12000
    pVddHighThrs = 13000
    pVddLowThrs = 10000
    pVddResolution = 12

    pvddmon_node = ATDF.getNode('/avr-tools-device-file/devices/device/peripherals/module@[name="ADC"]')
    if (pvddmon_node == None):
        pvddmon_node = ATDF.getNode('/avr-tools-device-file/devices/device/peripherals/module@[name="AFEC"]')
        if (pvddmon_node == None):
            pvddmon_plib = ""
        else:
            pvddmon_plib = "AFEC"
    else:            
        pvddmon_plib = "ADC"

    pPVDDMonVdd = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_PVDD", None)
    pPVDDMonVdd.setLabel("Vdd [mV]")
    pPVDDMonVdd.setDefaultValue(pVddValue) 
    pPVDDMonVdd.setReadOnly(True)

    pPVDDMonHighThreshold = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_HIGH_TH", pPVDDMonVdd)
    pPVDDMonHighThreshold.setLabel("High Threshold [mV]")
    pPVDDMonHighThreshold.setDefaultValue(pVddHighThrs)

    pPVDDMonLowThreshold = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_LOW_TH", pPVDDMonVdd)
    pPVDDMonLowThreshold.setLabel("Low Threshold [mV]")
    pPVDDMonLowThreshold.setDefaultValue(pVddLowThrs)

    pPVDDMonADCPlib = pPVDDMonComponent.createStringSymbol("SRV_PVDDMON_ADC_PLIB", None)
    pPVDDMonADCPlib.setLabel("Peripheral lib")
    pPVDDMonADCPlib.setDefaultValue(pvddmon_plib)
    pPVDDMonADCPlib.setReadOnly(True)

    pPVDDMonADCChannel = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_ADC_CHANNEL", pPVDDMonADCPlib)
    pPVDDMonADCChannel.setLabel("Channel")
    pPVDDMonADCChannel.setDefaultValue(0)

    pPVDDMonADCResolution = pPVDDMonComponent.createIntegerSymbol("SRV_PVDDMON_ADC_BITS", pPVDDMonADCPlib)
    pPVDDMonADCResolution.setLabel("Result Resolution bits")
    pPVDDMonADCResolution.setDefaultValue(pVddResolution)

    pPVDDMonADCComment1 = pPVDDMonComponent.createCommentSymbol("SRV_PVDDMON_ADC_COMMENT1", pPVDDMonADCPlib)
    pPVDDMonADCComment1.setLabel("**** " + pvddmon_plib + " will be configured in FreeRun mode.")
    pPVDDMonADCComment1.setVisible(True)

    pPVDDMonADCComment2 = pPVDDMonComponent.createCommentSymbol("SRV_PVDDMON_ADC_COMMENT2", pPVDDMonADCPlib)
    pPVDDMonADCComment2.setLabel("**** Enable manually " + pvddmon_plib + " End of conversion interrupt in the " + pvddmon_plib + " channel configuration.")
    pPVDDMonADCComment2.setVisible(True)

    if (pvddmon_plib == ""):
        pPVDDMonADCComment3 = pPVDDMonComponent.createCommentSymbol("SRV_PVDDMON_ADC_COMMENT2", None)
        pPVDDMonADCComment3.setLabel("**** CRITICAL ERROR. ADC peripheral has not been detected.")
        pPVDDMonADCComment3.setVisible(True)
    
    maxValue = pow(2, pVddResolution) - 1
    VoutHigh = pVddHighThrs * 10.0 / (10 + 36)
    VoutLow = pVddLowThrs * 10.0 / (10 + 36)
    VoutHighHyst = (pVddHighThrs - 100) * 10.0 / (10 + 36)
    VoutLowHyst = (pVddLowThrs + 100) * 10.0 / (10 + 36)

    HighThresholdHex = hex(int(math.ceil(VoutHigh * maxValue / 3300)))
    LowThresholdHex = hex(int(math.ceil(VoutLow * maxValue / 3300)))
    HighThresholdHexHyst = hex(int(math.ceil(VoutHighHyst * maxValue / 3300)))
    LowThresholdHexHyst = hex(int(math.ceil(VoutLowHyst * maxValue / 3300)))

    global pPVDDMonHighThrsHex
    pPVDDMonHighThrsHex = pPVDDMonComponent.createStringSymbol("SRV_PVDDMON_HIGH_THRESHOLD_HEX", None)
    pPVDDMonHighThrsHex.setLabel("High Threshold [Hex]")
    pPVDDMonHighThrsHex.setDefaultValue(HighThresholdHex)
    pPVDDMonHighThrsHex.setDependencies(getNewADCThresholds, ["SRV_PVDDMON_HIGH_TH", "SRV_PVDDMON_LOW_TH", "SRV_PVDDMON_ADC_BITS"])
    
    global pPVDDMonLowThrsHex
    pPVDDMonLowThrsHex = pPVDDMonComponent.createStringSymbol("SRV_PVDDMON_LOW_THRESHOLD_HEX", None)
    pPVDDMonLowThrsHex.setLabel("Low Threshold [Hex]")
    pPVDDMonLowThrsHex.setDefaultValue(LowThresholdHex)

    global pPVDDMonHighThrsHexHyst
    pPVDDMonHighThrsHexHyst = pPVDDMonComponent.createStringSymbol("SRV_PVDDMON_HIGH_THRESHOLD_HEX_HYST", None)
    pPVDDMonHighThrsHexHyst.setVisible(False)
    pPVDDMonHighThrsHexHyst.setDefaultValue(HighThresholdHexHyst)
    
    global pPVDDMonLowThrsHexHyst
    pPVDDMonLowThrsHexHyst = pPVDDMonComponent.createStringSymbol("SRV_PVDDMON_LOW_THRESHOLD_HEX_HYST", None)
    pPVDDMonLowThrsHexHyst.setVisible(False)
    pPVDDMonLowThrsHexHyst.setDefaultValue(LowThresholdHexHyst)

    # PLC PVDD Monitor Files
    pPVDDMonHeaderFile = pPVDDMonComponent.createFileSymbol("SRV_PVDDMON_HEADER", None)
    pPVDDMonHeaderFile.setSourcePath("service/pvddmon/srv_pvddmon.h.ftl")
    pPVDDMonHeaderFile.setOutputName("srv_pvddmon.h")
    pPVDDMonHeaderFile.setDestPath("service/pvddmon")
    pPVDDMonHeaderFile.setProjectPath("config/" + configName + "/service/pvddmon/")
    pPVDDMonHeaderFile.setType("HEADER")
    pPVDDMonHeaderFile.setMarkup(True)

    pPVDDMonSourceFile = pPVDDMonComponent.createFileSymbol("SRV_PVDDMON_SOURCE", None)
    pPVDDMonSourceFile.setSourcePath("service/pvddmon/srv_pvddmon.c.ftl")
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
