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

global rsniffer_helpkeyword
global protocolValues

rsniffer_helpkeyword = "srv_rsniffer_configurations"

def isKeyPresent(symbol, key):
    for i in range(symbol.getKeyCount()):
        if symbol.getKey(i) == key:
            return True
    return False

def handleMessage(messageID, args):
    global rSnifferProtocol

    result_dict = {}

    if (messageID == "SRV_RSNIFFER_G3"):
        rSnifferProtocol.setValue("G3")

    elif (messageID == "SRV_RSNIFFER_PRIME"):
        if ("PRIME" in protocolValues):
            rSnifferProtocol.setValue("PRIME")

    return result_dict

def instantiateComponent(rSnifferComponentCommon):
    
    Log.writeInfoMessage("Loading RF Phy Sniffer Service")

    processor = Variables.get("__PROCESSOR")
    if ("WBZ45" in processor) or ("PIC32CX1012BZ" in processor):
        protocolValues = ["G3"]
    else:
        protocolValues = ["G3", "PRIME"]

    global rSnifferProtocol
    rSnifferProtocol = rSnifferComponentCommon.createComboSymbol("SRV_RSNF_PROTOCOL", None, protocolValues)
    rSnifferProtocol.setLabel("PLC-RF Hybrid Protocol")
    rSnifferProtocol.setDefaultValue("G3")
    rSnifferProtocol.setHelp(rsniffer_helpkeyword)

    # Check PLC protocol
    plcComponent = Database.getComponentByID("drvPlcPhy")
    if plcComponent != None:
        protocol = Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PROFILE")
        if protocol == "G3-PLC":
            rSnifferProtocol.setValue("G3")
        else:
            rSnifferProtocol.setValue("PRIME")
    else:
        plcComponent = Database.getComponentByID("drvG3MacRt")
        if plcComponent != None:
            rSnifferProtocol.setValue("G3")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    # Phy Serial Files
    rSnifferSourceFile = rSnifferComponentCommon.createFileSymbol("SRV_RSNF_SOURCE_G3", None)
    rSnifferSourceFile.setOutputName("srv_rsniffer.c")
    rSnifferSourceFile.setDestPath("service/rsniffer")
    rSnifferSourceFile.setProjectPath("config/" + configName + "/service/rsniffer/")
    rSnifferSourceFile.setType("SOURCE")
    rSnifferSourceFile.setOverwrite(True)

    rSnifferHeaderFile = rSnifferComponentCommon.createFileSymbol("SRV_RSNF_HEADER", None)
    rSnifferHeaderFile.setOutputName("srv_rsniffer.h")
    rSnifferHeaderFile.setDestPath("service/rsniffer")
    rSnifferHeaderFile.setProjectPath("config/" + configName + "/service/rsniffer/")
    rSnifferHeaderFile.setType("HEADER")
    rSnifferHeaderFile.setOverwrite(True)

    if ("WBZ45" in processor) or ("PIC32CX1012BZ" in processor):
        rSnifferSourceFile.setSourcePath("service/rsniffer/srv_rsniffer_wbz451.c")
        rSnifferSourceFile.setMarkup(False)
        rSnifferHeaderFile.setSourcePath("service/rsniffer/srv_rsniffer_wbz451.h")
        rSnifferHeaderFile.setMarkup(False)
    else:
        rSnifferSourceFile.setSourcePath("service/rsniffer/srv_rsniffer.c.ftl")
        rSnifferSourceFile.setMarkup(True)
        rSnifferHeaderFile.setSourcePath("service/rsniffer/srv_rsniffer.h.ftl")
        rSnifferHeaderFile.setMarkup(True)
        
    rSnifferSystemDefIncFile = rSnifferComponentCommon.createFileSymbol("SRV_RSNF_SYSTEM_DEF", None)
    rSnifferSystemDefIncFile.setType("STRING")
    rSnifferSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    rSnifferSystemDefIncFile.setSourcePath("service/rsniffer/templates/system/system_definitions.h.ftl")
    rSnifferSystemDefIncFile.setMarkup(True)
