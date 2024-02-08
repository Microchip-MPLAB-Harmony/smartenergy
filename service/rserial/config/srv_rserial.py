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
