# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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
    global pSerialPLCProfile

    result_dict = {}

    print ("[CHRIS_dbg]: SRV_PSERIAL handleMessage:" + messageID)

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
        print ("pSerial as G3 mode")

def instantiateComponent(pSerialComponentCommon):
    
    Log.writeInfoMessage("Loading PLC Phy Serial Common Service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")
    
    global pSerialPLCProfile
    pSerialPLCProfile = pSerialComponentCommon.createKeyValueSetSymbol("SRV_PSER_PLC_PROFILE", None)
    pSerialPLCProfile.setLabel("PLC Profile")
    pSerialPLCProfile.addKey("G3_CEN_A", "0", "G3_CEN_A")
    pSerialPLCProfile.addKey("G3_CEN_B", "1", "G3_CEN_B")
    pSerialPLCProfile.addKey("G3_FCC", "2", "G3_FCC")
    pSerialPLCProfile.addKey("G3_ARIB", "3", "G3_ARIB")
    pSerialPLCProfile.addKey("PRIME", "4", "PRIME")
    pSerialPLCProfile.setDisplayMode("Description")
    pSerialPLCProfile.setOutputMode("Value")
    pSerialPLCProfile.setDefaultValue(0)

    # Phy Serial Files
    global pSerialSourceFileG3
    pSerialSourceFileG3 = pSerialComponentCommon.createFileSymbol("SRV_PSER_SOURCE_G3", None)
    pSerialSourceFileG3.setSourcePath("service/pserial/srv_pserial_g3.c")
    pSerialSourceFileG3.setOutputName("srv_pserial.c")
    pSerialSourceFileG3.setDestPath("service/pserial")
    pSerialSourceFileG3.setProjectPath("config/" + configName + "/service/pserial/")
    pSerialSourceFileG3.setType("SOURCE")
    pSerialSourceFileG3.setMarkup(False)
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
    pSerialHeaderFile.setSourcePath("service/pserial/srv_pserial.h.ftl")
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
