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

def instantiateComponent(srvLogReportComponent):
    
    Log.writeInfoMessage("Loading Log Report service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")
 
    # Create a checkbox to report in display
    pLogReportDisplay = srvLogReportComponent.createBooleanSymbol("enableReportDisplay", None)
    pLogReportDisplay.setLabel("Report errors in display?")
    pLogReportDisplay.setDefaultValue(False)
    pLogReportDisplay.setVisible(True)
   
    # Log Report Files
    pLogReportSource = srvLogReportComponent.createFileSymbol("SRV_LOG_REPORT_SOURCE", None)
    pLogReportSource.setSourcePath("service/templates/log_report/srv_log_report.c.ftl")
    pLogReportSource.setOutputName("srv_log_report.c")
    pLogReportSource.setDestPath("service/log_report")
    pLogReportSource.setProjectPath("config/" + configName + "/service/log_report/")
    pLogReportSource.setType("SOURCE")
    pLogReportSource.setMarkup(True)
    
    pLogReportHeader = srvLogReportComponent.createFileSymbol("SRV_LOG_REPORT_HEADER", None)
    pLogReportHeader.setSourcePath("service/log_report/srv_log_report.h")
    pLogReportHeader.setOutputName("srv_log_report.h")
    pLogReportHeader.setDestPath("service/log_report")
    pLogReportHeader.setProjectPath("config/" + configName + "/service/log_report/")
    pLogReportHeader.setType("HEADER")

    pLogReportSystemDefIncFile = srvLogReportComponent.createFileSymbol("SRV_LOG_REPORT_SYSTEM_DEF", None)
    pLogReportSystemDefIncFile.setType("STRING")
    pLogReportSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    pLogReportSystemDefIncFile.setSourcePath("service/log_report/templates/system/system_definitions.h.ftl")
    pLogReportSystemDefIncFile.setMarkup(True)
