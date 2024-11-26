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

def instantiateComponent(crcComponentCommon):

    Log.writeInfoMessage("Loading SE CRC Common Service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    # CRC wrapper Files
    srvCrcSourceFile = crcComponentCommon.createFileSymbol("SRV_PCRC_SOURCE", None)
    srvCrcSourceFile.setSourcePath("service/pcrc/srv_pcrc.c.ftl")
    srvCrcSourceFile.setOutputName("srv_pcrc.c")
    srvCrcSourceFile.setDestPath("service/pcrc")
    srvCrcSourceFile.setProjectPath("config/" + configName + "/service/pcrc/")
    srvCrcSourceFile.setType("SOURCE")
    srvCrcSourceFile.setMarkup(True)
    srvCrcSourceFile.setOverwrite(True)
    srvCrcSourceFile.setEnabled(True)

    srvCrcHeaderFile = crcComponentCommon.createFileSymbol("SRV_PCRC_HEADER", None)
    srvCrcHeaderFile.setSourcePath("service/pcrc/srv_pcrc.h")
    srvCrcHeaderFile.setOutputName("srv_pcrc.h")
    srvCrcHeaderFile.setDestPath("service/pcrc")
    srvCrcHeaderFile.setProjectPath("config/" + configName + "/service/pcrc/")
    srvCrcHeaderFile.setType("HEADER")
    srvCrcHeaderFile.setMarkup(False)
    srvCrcHeaderFile.setOverwrite(True)
    srvCrcHeaderFile.setEnabled(True)

    #### FreeMaker System Files ######################################################

    srvCrcSystemDefFile = crcComponentCommon.createFileSymbol("SRV_PCRC_SYSTEM_DEF", None)
    srvCrcSystemDefFile.setType("STRING")
    srvCrcSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    srvCrcSystemDefFile.setSourcePath("service/pcrc/templates/system/system_definitions.h.ftl")
    srvCrcSystemDefFile.setMarkup(True)
