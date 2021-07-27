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
        print ("pSniffer as G3 mode")

def instantiateComponent(pSnifferComponentCommon):
    
    Log.writeInfoMessage("Loading PLC Phy Sniffer Service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")
    
    global pSnifferPLCProfile
    pSnifferPLCProfile = pSnifferComponentCommon.createKeyValueSetSymbol("SRV_PSNF_PLC_PROFILE", None)
    pSnifferPLCProfile.setLabel("PLC Profile")
    pSnifferPLCProfile.addKey("G3_CEN_A", "0", "G3_CEN_A")
    pSnifferPLCProfile.addKey("G3_CEN_B", "1", "G3_CEN_B")
    pSnifferPLCProfile.addKey("G3_FCC", "2", "G3_FCC")
    pSnifferPLCProfile.addKey("G3_ARIB", "3", "G3_ARIB")
    pSnifferPLCProfile.addKey("PRIME", "4", "PRIME")
    pSnifferPLCProfile.setDisplayMode("Description")
    pSnifferPLCProfile.setOutputMode("Value")
    pSnifferPLCProfile.setDefaultValue(0)

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
    pSnifferHeaderFile.setSourcePath("service/psniffer/srv_psniffer.h.ftl")
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
