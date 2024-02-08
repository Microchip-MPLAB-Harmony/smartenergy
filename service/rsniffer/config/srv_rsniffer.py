# coding: utf-8
"""
Copyright (C) 2024, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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
