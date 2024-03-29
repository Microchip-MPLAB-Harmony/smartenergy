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

psniffer_helpkeyword = "srv_psniffer_configurations"

def isKeyPresent(symbol, key):
    for i in range(symbol.getKeyCount()):
        if symbol.getKey(i) == key:
            return True
    return False

def handleMessage(messageID, args):
    global pSnifferPLCProfile

    result_dict = {}

    if (messageID == "SRV_PSNIFFER_G3_CENA"):
        if isKeyPresent(pSnifferPLCProfile, "G3_CEN_A") == True:
            pSnifferPLCProfile.setSelectedKey("G3_CEN_A")

    elif (messageID == "SRV_PSNIFFER_G3_CENB"):
        if isKeyPresent(pSnifferPLCProfile, "G3_CEN_B") == True:
            pSnifferPLCProfile.setSelectedKey("G3_CEN_B")

    elif (messageID == "SRV_PSNIFFER_G3_FCC"):
        if isKeyPresent(pSnifferPLCProfile, "G3_FCC") == True:
            pSnifferPLCProfile.setSelectedKey("G3_FCC")

    elif (messageID == "SRV_PSNIFFER_G3_ARIB"):
        if isKeyPresent(pSnifferPLCProfile, "G3_ARIB") == True:
            pSnifferPLCProfile.setSelectedKey("G3_ARIB")

    elif (messageID == "SRV_PSNIFFER_PRIME"):
        if isKeyPresent(pSnifferPLCProfile, "PRIME") == True:
            pSnifferPLCProfile.setSelectedKey("PRIME")

    return result_dict

def setProfile(symbol, event):
    global pSnifferSourceFileG3
    global pSnifferSourceFilePrime

    print ("setProfile:" + str(event["value"]))

    if (event["symbol"].getKeyDescription(event["value"]) == "PRIME"):
        # PRIME
        pSnifferSourceFilePrime.setEnabled(True)
        pSnifferSourceFileG3.setEnabled(False)
        print ("pSniffer as PRIME mode")
    else:
        pSnifferSourceFilePrime.setEnabled(False)
        pSnifferSourceFileG3.setEnabled(True)
        print ("pSniffer as G3-PLC mode")

def instantiateComponent(pSnifferComponentCommon):

    Log.writeInfoMessage("Loading PLC Phy Sniffer Service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    global pSnifferPLCProfile
    pSnifferPLCProfile = pSnifferComponentCommon.createKeyValueSetSymbol("SRV_PSNF_PLC_PROFILE", None)
    pSnifferPLCProfile.setLabel("PLC Profile")
    pSnifferPLCProfile.addKey("G3_CEN_A", "G3_CEN_A", "G3-PLC CENELEC-A")
    pSnifferPLCProfile.addKey("G3_CEN_B", "G3_CEN_B", "G3-PLC CENELEC-B")
    pSnifferPLCProfile.addKey("G3_FCC", "G3_FCC", "G3-PLC FCC")
    # pSnifferPLCProfile.addKey("G3_ARIB", "G3_ARIB", "G3-PLC ARIB")
    pSnifferPLCProfile.addKey("PRIME", "PRIME", "PRIME")
    pSnifferPLCProfile.setDisplayMode("Description")
    pSnifferPLCProfile.setOutputMode("Value")
    pSnifferPLCProfile.setDefaultValue(0)
    pSnifferPLCProfile.setHelp(psniffer_helpkeyword)

    # Phy Serial Files
    global pSnifferSourceFileG3
    pSnifferSourceFileG3 = pSnifferComponentCommon.createFileSymbol("SRV_PSNF_SOURCE_G3", None)
    pSnifferSourceFileG3.setSourcePath("service/psniffer/srv_psniffer_g3.c")
    pSnifferSourceFileG3.setOutputName("srv_psniffer.c")
    pSnifferSourceFileG3.setDestPath("service/psniffer")
    pSnifferSourceFileG3.setProjectPath("config/" + configName + "/service/psniffer/")
    pSnifferSourceFileG3.setType("SOURCE")
    pSnifferSourceFileG3.setMarkup(False)
    pSnifferSourceFileG3.setOverwrite(True)
    pSnifferSourceFileG3.setEnabled(True)
    pSnifferSourceFileG3.setDependencies(setProfile, ["SRV_PSNF_PLC_PROFILE"])

    global pSnifferSourceFilePrime
    pSnifferSourceFilePrime = pSnifferComponentCommon.createFileSymbol("SRV_PSNF_SOURCE_PRIME", None)
    pSnifferSourceFilePrime.setSourcePath("service/psniffer/srv_psniffer_prime.c")
    pSnifferSourceFilePrime.setOutputName("srv_psniffer.c")
    pSnifferSourceFilePrime.setDestPath("service/psniffer")
    pSnifferSourceFilePrime.setProjectPath("config/" + configName + "/service/psniffer/")
    pSnifferSourceFilePrime.setType("SOURCE")
    pSnifferSourceFilePrime.setMarkup(False)
    pSnifferSourceFilePrime.setOverwrite(True)
    pSnifferSourceFilePrime.setEnabled(False)

    pSnifferHeaderFile = pSnifferComponentCommon.createFileSymbol("SRV_PSNF_HEADER", None)
    pSnifferHeaderFile.setSourcePath("service/psniffer/templates/srv_psniffer.h.ftl")
    pSnifferHeaderFile.setOutputName("srv_psniffer.h")
    pSnifferHeaderFile.setDestPath("service/psniffer")
    pSnifferHeaderFile.setProjectPath("config/" + configName + "/service/psniffer/")
    pSnifferHeaderFile.setType("HEADER")
    pSnifferHeaderFile.setMarkup(True)
    pSnifferHeaderFile.setOverwrite(True)
    pSnifferHeaderFile.setEnabled(True)

    pSnifferSystemDefIncFile = pSnifferComponentCommon.createFileSymbol("SRV_PSNF_SYSTEM_DEF", None)
    pSnifferSystemDefIncFile.setType("STRING")
    pSnifferSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    pSnifferSystemDefIncFile.setSourcePath("service/psniffer/templates/system/system_definitions.h.ftl")
    pSnifferSystemDefIncFile.setMarkup(True)
