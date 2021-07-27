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
def handleMessage(messageID, args):
    result_dict = {}

    return result_dict

def setPlibValue(symbol, event):
    symbol.setValue(event["value"])

def instantiateComponent(pPVDDMonComponent):
    
    Log.writeInfoMessage("Loading PLC PVDD Monitor Service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    ppvddmon_node = ATDF.getNode('/avr-tools-device-file/devices/device/peripherals/module@[name="ADC"]')
    print("[CHRIS_dbg]: ppvddmon_node ADC...")
    print(ppvddmon_node)

    pHWSupport = pPVDDMonComponent.createBooleanSymbol("SRV_PPVDDMON_ADC_HW_SUPPORT", None)
    pHWSupport.setLabel("ADC PLIB support")
    if (ppvddmon_node == None):
        pHWSupport.setDefaultValue(False)
    else:
        pHWSupport.setDefaultValue(True)
    pHWSupport.setReadOnly(True)

    ppvddmon_node = ATDF.getNode('/avr-tools-device-file/devices/device/peripherals/module@[name="AFEC"]')
    print("[CHRIS_dbg]: ppvddmon_node AFEC...")
    print(ppvddmon_node)

    pHWSupport = pPVDDMonComponent.createBooleanSymbol("SRV_PPVDDMON_AFEC_HW_SUPPORT", None)
    pHWSupport.setLabel("AFEC PLIB support")
    if (ppvddmon_node == None):
        pHWSupport.setDefaultValue(False)
    else:
        pHWSupport.setDefaultValue(True)
    pHWSupport.setReadOnly(True)

    # PLC PVDD Monitor Files
    pPVDDMonHeaderFile = pPVDDMonComponent.createFileSymbol("SRV_PPVDDMON_HEADER", None)
    pPVDDMonHeaderFile.setSourcePath("service/ppvddmon/srv_ppvddmon.h.ftl")
    pPVDDMonHeaderFile.setOutputName("srv_ppvddmon.h")
    pPVDDMonHeaderFile.setDestPath("service/ppvddmon")
    pPVDDMonHeaderFile.setProjectPath("config/" + configName + "/service/ppvddmon/")
    pPVDDMonHeaderFile.setType("HEADER")
    pPVDDMonHeaderFile.setMarkup(True)

    pPVDDMonSourceFile = pPVDDMonComponent.createFileSymbol("SRV_PPVDDMON_SOURCE", None)
    pPVDDMonSourceFile.setSourcePath("service/ppvddmon/srv_ppvddmon.c.ftl")
    pPVDDMonSourceFile.setOutputName("srv_ppvddmon.c")
    pPVDDMonSourceFile.setDestPath("service/ppvddmon")
    pPVDDMonSourceFile.setProjectPath("config/" + configName + "/service/ppvddmon/")
    pPVDDMonSourceFile.setType("SOURCE")
    pPVDDMonSourceFile.setMarkup(True)

    pPVDDMonSystemDefIncFile = pPVDDMonComponent.createFileSymbol("SRV_PPVDDMON_SYSTEM_DEF", None)
    pPVDDMonSystemDefIncFile.setType("STRING")
    pPVDDMonSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    pPVDDMonSystemDefIncFile.setSourcePath("service/ppvddmon/templates/system/system_definitions.h.ftl")
    pPVDDMonSystemDefIncFile.setMarkup(True)
