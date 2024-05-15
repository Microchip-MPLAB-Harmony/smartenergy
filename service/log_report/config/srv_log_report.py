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

srv_log_report_helpkeyword = "srv_log_report_configurations"

def instantiateComponent(srvLogReportComponent):

    Log.writeInfoMessage("Loading SE Log Report service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")
    processor = Variables.get("__PROCESSOR")

    # Boolean value to control traces enabled/disabled
    pTracesEnabled = srvLogReportComponent.createBooleanSymbol("ENABLE_TRACES", None)
    if Database.getSymbolValue("g3_config", "G3_DEBUG_ENABLE") != None:
        enabledValue = Database.getSymbolValue("g3_config", "G3_DEBUG_ENABLE")
    elif (Database.getComponentByID("prime_config") != None):
        enabledValue = True
    else:
        enabledValue = False
    pTracesEnabled.setValue(enabledValue)
    pTracesEnabled.setVisible(False)

    # Create a checkbox to report in display
    pLogReportDisplay = srvLogReportComponent.createBooleanSymbol("enableReportDisplay", None)
    pLogReportDisplay.setLabel("Report errors in display?")
    pLogReportDisplay.setDefaultValue(False)
    pLogReportDisplay.setVisible(True)
    pLogReportDisplay.setHelp(srv_log_report_helpkeyword)
    if (("MTSH" in processor) or ("MTC" in processor)):
        pLogReportDisplay.setReadOnly(False)
    else:
        pLogReportDisplay.setReadOnly(True)

    # Log Report Files
    global pLogReportSource
    pLogReportSource = srvLogReportComponent.createFileSymbol("SRV_LOG_REPORT_SOURCE", None)
    pLogReportSource.setSourcePath("service/log_report/templates/srv_log_report.c.ftl")
    pLogReportSource.setOutputName("srv_log_report.c")
    pLogReportSource.setDestPath("service/log_report")
    pLogReportSource.setProjectPath("config/" + configName + "/service/log_report/")
    pLogReportSource.setType("SOURCE")
    pLogReportSource.setMarkup(True)
    pLogReportSource.setEnabled(enabledValue)
    pLogReportSource.setDependencies(enableTracesChange, ["ENABLE_TRACES"])

    pLogReportHeader = srvLogReportComponent.createFileSymbol("SRV_LOG_REPORT_HEADER", None)
    pLogReportHeader.setSourcePath("service/log_report/templates/srv_log_report.h.ftl")
    pLogReportHeader.setOutputName("srv_log_report.h")
    pLogReportHeader.setDestPath("service/log_report")
    pLogReportHeader.setProjectPath("config/" + configName + "/service/log_report/")
    pLogReportHeader.setType("HEADER")
    pLogReportHeader.setMarkup(True)

    pLogReportSystemDefIncFile = srvLogReportComponent.createFileSymbol("SRV_LOG_REPORT_SYSTEM_DEF", None)
    pLogReportSystemDefIncFile.setType("STRING")
    pLogReportSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    pLogReportSystemDefIncFile.setSourcePath("service/log_report/templates/system/system_definitions.h.ftl")
    pLogReportSystemDefIncFile.setMarkup(True)

def enableTracesChange(symbol, event):
    print(str(event["value"]))
    if event["value"] == True:
        # Enable LogReport source
        pLogReportSource.setEnabled(True)
    else:
        # Disable LogReport source
        pLogReportSource.setEnabled(False)

#Handle messages from other components
def handleMessage(messageID, args):
    retDict= {}

    if (messageID == "SET_SYMBOL"):
        retDict= {"Return": "Success"}
        Database.setSymbolValue(args["Component"], args["Id"], args["Value"])
    else:
        retDict= {"Return": "UnImplemented Command"}

    return retDict
