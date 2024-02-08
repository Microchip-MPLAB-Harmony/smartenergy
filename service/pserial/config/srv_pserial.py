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

pserial_helpkeyword = "srv_pserial_configurations"

def isKeyPresent(symbol, key):
    for i in range(symbol.getKeyCount()):
        if symbol.getKey(i) == key:
            return True
    return False

def handleMessage(messageID, args):
    global pSerialPLCProfile

    result_dict = {}

    # print ("[CHRIS_dbg]: SRV_PSERIAL handleMessage:" + messageID)

    if (messageID == "SRV_PSERIAL_G3_CENA"):
        if isKeyPresent(pSerialPLCProfile, "G3_CEN_A") == True:
            pSerialPLCProfile.setSelectedKey("G3_CEN_A")

    elif (messageID == "SRV_PSERIAL_G3_CENB"):
        if isKeyPresent(pSerialPLCProfile, "G3_CEN_B") == True:
            pSerialPLCProfile.setSelectedKey("G3_CEN_B")

    elif (messageID == "SRV_PSERIAL_G3_FCC"):
        if isKeyPresent(pSerialPLCProfile, "G3_FCC") == True:
            pSerialPLCProfile.setSelectedKey("G3_FCC")

    elif (messageID == "SRV_PSERIAL_G3_ARIB"):
        if isKeyPresent(pSerialPLCProfile, "G3_ARIB") == True:
            pSerialPLCProfile.setSelectedKey("G3_ARIB")

    elif (messageID == "SRV_PSERIAL_PRIME"):
        if isKeyPresent(pSerialPLCProfile, "PRIME") == True:
            pSerialPLCProfile.setSelectedKey("PRIME")

    return result_dict

def setProfile(symbol, event):
    global pSerialSourceFileG3
    global pSerialSourceFilePrime

    print ("setProfile:" + str(event["value"]))

    if (event["symbol"].getKeyDescription(event["value"]) == "PRIME"):
        # PRIME
        pSerialSourceFilePrime.setEnabled(True)
        pSerialSourceFileG3.setEnabled(False)
        print ("pSerial as PRIME mode")
    else:
        pSerialSourceFilePrime.setEnabled(False)
        pSerialSourceFileG3.setEnabled(True)
        print ("pSerial as G3-PLC mode")

def instantiateComponent(pSerialComponentCommon):

    Log.writeInfoMessage("Loading PLC Phy Serial Common Service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    global pSerialPLCProfile
    pSerialPLCProfile = pSerialComponentCommon.createKeyValueSetSymbol("SRV_PSER_PLC_PROFILE", None)
    pSerialPLCProfile.setLabel("PLC Profile")
    pSerialPLCProfile.addKey("G3_CEN_A", "G3_CEN_A", "G3-PLC CENELEC-A")
    pSerialPLCProfile.addKey("G3_CEN_B", "G3_CEN_B", "G3-PLC CENELEC-B")
    pSerialPLCProfile.addKey("G3_FCC", "G3_FCC", "G3-PLC FCC")
    # pSerialPLCProfile.addKey("G3_ARIB", "G3_ARIB", "G3-PLC ARIB")
    pSerialPLCProfile.addKey("PRIME", "PRIME", "PRIME")
    pSerialPLCProfile.setDisplayMode("Description")
    pSerialPLCProfile.setOutputMode("Value")
    pSerialPLCProfile.setDefaultValue(0)
    pSerialPLCProfile.setHelp(pserial_helpkeyword)

    # Phy Serial Files
    global pSerialSourceFileG3
    pSerialSourceFileG3 = pSerialComponentCommon.createFileSymbol("SRV_PSER_SOURCE_G3", None)
    pSerialSourceFileG3.setSourcePath("service/pserial/srv_pserial_g3.c.ftl")
    pSerialSourceFileG3.setOutputName("srv_pserial.c")
    pSerialSourceFileG3.setDestPath("service/pserial")
    pSerialSourceFileG3.setProjectPath("config/" + configName + "/service/pserial/")
    pSerialSourceFileG3.setType("SOURCE")
    pSerialSourceFileG3.setMarkup(True)
    pSerialSourceFileG3.setOverwrite(True)
    pSerialSourceFileG3.setEnabled(True)
    pSerialSourceFileG3.setDependencies(setProfile, ["SRV_PSER_PLC_PROFILE"])

    global pSerialSourceFilePrime
    pSerialSourceFilePrime = pSerialComponentCommon.createFileSymbol("SRV_PSER_SOURCE_PRIME", None)
    pSerialSourceFilePrime.setSourcePath("service/pserial/srv_pserial_prime.c")
    pSerialSourceFilePrime.setOutputName("srv_pserial.c")
    pSerialSourceFilePrime.setDestPath("service/pserial")
    pSerialSourceFilePrime.setProjectPath("config/" + configName + "/service/pserial/")
    pSerialSourceFilePrime.setType("SOURCE")
    pSerialSourceFilePrime.setMarkup(False)
    pSerialSourceFilePrime.setOverwrite(True)
    pSerialSourceFilePrime.setEnabled(False)

    pSerialHeaderFile = pSerialComponentCommon.createFileSymbol("SRV_PSER_HEADER", None)
    pSerialHeaderFile.setSourcePath("service/pserial/templates/srv_pserial.h.ftl")
    pSerialHeaderFile.setOutputName("srv_pserial.h")
    pSerialHeaderFile.setDestPath("service/pserial")
    pSerialHeaderFile.setProjectPath("config/" + configName + "/service/pserial/")
    pSerialHeaderFile.setType("HEADER")
    pSerialHeaderFile.setMarkup(True)
    pSerialHeaderFile.setOverwrite(True)
    pSerialHeaderFile.setEnabled(True)

    pSerialSystemDefIncFile = pSerialComponentCommon.createFileSymbol("SRV_PSER_SYSTEM_DEF", None)
    pSerialSystemDefIncFile.setType("STRING")
    pSerialSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    pSerialSystemDefIncFile.setSourcePath("service/pserial/templates/system/system_definitions.h.ftl")
    pSerialSystemDefIncFile.setMarkup(True)
