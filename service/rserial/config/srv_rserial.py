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

global rserial_helpkeyword

rserial_helpkeyword = "srv_rserial_configurations"

def instantiateComponent(rSerialComponentCommon):
    
    Log.writeInfoMessage("Loading RF Phy Serial Common Service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    # Phy Serial Files
    rSerialSourceFile = rSerialComponentCommon.createFileSymbol("SRV_RSER_SOURCE", None)
    rSerialSourceFile.setSourcePath("service/rserial/templates/srv_rserial.c.ftl")
    rSerialSourceFile.setOutputName("srv_rserial.c")
    rSerialSourceFile.setDestPath("service/rserial")
    rSerialSourceFile.setProjectPath("config/" + configName + "/service/rserial/")
    rSerialSourceFile.setType("SOURCE")
    rSerialSourceFile.setMarkup(True)
    rSerialSourceFile.setOverwrite(True)
    rSerialSourceFile.setEnabled(True)

    rSerialHeaderFile = rSerialComponentCommon.createFileSymbol("SRV_RSER_HEADER", None)
    rSerialHeaderFile.setSourcePath("service/rserial/srv_rserial.h")
    rSerialHeaderFile.setOutputName("srv_rserial.h")
    rSerialHeaderFile.setDestPath("service/rserial")
    rSerialHeaderFile.setProjectPath("config/" + configName + "/service/rserial/")
    rSerialHeaderFile.setType("HEADER")
    rSerialHeaderFile.setMarkup(False)
    rSerialHeaderFile.setOverwrite(True)
    rSerialHeaderFile.setEnabled(True)

    rSerialSystemDefIncFile = rSerialComponentCommon.createFileSymbol("SRV_RSER_SYSTEM_DEF", None)
    rSerialSystemDefIncFile.setType("STRING")
    rSerialSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    rSerialSystemDefIncFile.setSourcePath("service/rserial/templates/system/system_definitions.h.ftl")
    rSerialSystemDefIncFile.setMarkup(True)
