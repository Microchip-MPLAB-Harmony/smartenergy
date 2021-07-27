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
def handleMessage(messageID, args):
    result_dict = {}

    return result_dict

def instantiateComponent(pTrngComponent):
    
    Log.writeInfoMessage("Loading PLC TRNG Service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    trng_node = ATDF.getNode('/avr-tools-device-file/devices/device/peripherals/module@[name="TRNG"]')

    pTrngHWSupport = pTrngComponent.createBooleanSymbol("SRV_PTRNG_HW_SUPPORT", None)
    pTrngHWSupport.setLabel("HW support")
    if (trng_node == None):
        pTrngHWSupport.setDefaultValue(False)
    else:
        pTrngHWSupport.setDefaultValue(True)
    pTrngHWSupport.setReadOnly(True)

    # PLC TRNG Files
    pTrngHeaderFile = pTrngComponent.createFileSymbol("SRV_PTRNG_HEADER", None)
    pTrngHeaderFile.setSourcePath("service/ptrng/srv_ptrng.h.ftl")
    pTrngHeaderFile.setOutputName("srv_ptrng.h")
    pTrngHeaderFile.setDestPath("service/ptrng")
    pTrngHeaderFile.setProjectPath("config/" + configName + "/service/ptrng/")
    pTrngHeaderFile.setType("HEADER")
    pTrngHeaderFile.setMarkup(True)

    pTrngSourceFile = pTrngComponent.createFileSymbol("SRV_PTRNG_SOURCE", None)
    pTrngSourceFile.setSourcePath("service/ptrng/srv_ptrng.c.ftl")
    pTrngSourceFile.setOutputName("srv_ptrng.c")
    pTrngSourceFile.setDestPath("service/ptrng")
    pTrngSourceFile.setProjectPath("config/" + configName + "/service/ptrng/")
    pTrngSourceFile.setType("SOURCE")
    pTrngSourceFile.setMarkup(True)

    pTrngSystemDefIncFile = pTrngComponent.createFileSymbol("SRV_PTRNG_SYSTEM_DEF", None)
    pTrngSystemDefIncFile.setType("STRING")
    pTrngSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    pTrngSystemDefIncFile.setSourcePath("service/ptrng/templates/system/system_definitions.h.ftl")
    pTrngSystemDefIncFile.setMarkup(True)
