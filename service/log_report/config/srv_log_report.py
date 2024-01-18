"""*****************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
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

srv_log_report_helpkeyword = "srv_log_report_configurations"

def instantiateComponent(srvLogReportComponent):

    Log.writeInfoMessage("Loading SE Log Report service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    # Boolean value to control traces enabled/disabled
    pTracesEnabled = srvLogReportComponent.createBooleanSymbol("ENABLE_TRACES", None)
    if Database.getSymbolValue("g3_config", "G3_DEBUG_ENABLE") != None:
        enabledValue = Database.getSymbolValue("g3_config", "G3_DEBUG_ENABLE")
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
