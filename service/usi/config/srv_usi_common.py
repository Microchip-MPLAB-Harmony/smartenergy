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

srv_usi_helpkeyword = "mcc_h3_srv_usi_configurations"

def handleMessage(messageID, args):
    global usiSymUsartAPI
    global usiSymCdcAPI
    global usiSymNumUsart
    global usiSymNumCdc

    result_dict = {}

    usiInstances = filter(lambda k: "srv_usi_" in k, Database.getActiveComponentIDs())

    uart_count = 0
    usb_count = 0
    for usiInstance in sorted(usiInstances):
        if Database.getSymbolValue(usiInstance, "SRV_USI_DEVICE_SET") == "UART":
            Database.setSymbolValue(usiInstance, "SRV_USI_USART_API_INDEX", uart_count)
            uart_count += 1
        if Database.getSymbolValue(usiInstance, "SRV_USI_DEVICE_SET") == "USB_CDC":
            Database.setSymbolValue(usiInstance, "SRV_USI_CDC_API_INDEX", usb_count)
            usb_count += 1

    usiSymNumUsart.setValue(uart_count)
    usiSymNumCdc.setValue(usb_count)

    if (uart_count > 0):
        usiSymUsartAPI.setValue(True)
    else:
        usiSymUsartAPI.setValue(False)

    if (usb_count > 0):
        usiSymCdcAPI.setValue(True)
    else:
        usiSymCdcAPI.setValue(False)

    return result_dict

def enableFiles(symbol, event):
    symbol.setEnabled(event["value"])
    # if(event["value"]):
        # print("Enable Src/Hdr files")
    # else:
        # print("Disable Src/Hdr files")

def errorLogReportHandler(symbol, event):
    eventValue = event["value"]
    component = event["source"]

    # Enable/Disable Log Report dependency
    component.setDependencyEnabled("srv_usi_logreport_dependency", eventValue)

    if eventValue == True:
        Database.activateComponents(["srvLogReport"])

def instantiateComponent(usiComponentCommon):

    Log.writeInfoMessage("Loading PLC USI Service...common")

    res = Database.activateComponents(["HarmonyCore"])

    # Enable "Generate Harmony Driver Common Files" option in MHC
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":True})

    # Enable "Generate Harmony System Service Common Files" option in MHC
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":True})

    # Disable Log Report dependency by default
    usiComponentCommon.setDependencyEnabled("srv_usi_logreport_dependency", False)

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    usiSymMsgPoolSize = usiComponentCommon.createIntegerSymbol("SRV_USI_MSG_POOL_SIZE", None)
    usiSymMsgPoolSize.setLabel("Received Message Pool Size")
    usiSymMsgPoolSize.setMin(1)
    usiSymMsgPoolSize.setMax(10)
    usiSymMsgPoolSize.setDefaultValue(5)
    usiSymMsgPoolSize.setHelp(srv_usi_helpkeyword)

    usiErrorLogReport = usiComponentCommon.createBooleanSymbol("SRV_USI_ERROR_LOG_REPORT", None)
    usiErrorLogReport.setLabel("Enable Error Log Report")
    usiErrorLogReport.setReadOnly(False)
    usiErrorLogReport.setDefaultValue(False)
    usiErrorLogReport.setVisible(True)
    usiErrorLogReport.setHelp(srv_usi_helpkeyword)

    global usiSymUsartAPI
    usiSymUsartAPI = usiComponentCommon.createBooleanSymbol("SRV_USI_USART_API", None)
    usiSymUsartAPI.setLabel("UART API")
    usiSymUsartAPI.setReadOnly(True)
    usiSymUsartAPI.setDefaultValue(False)
    usiSymUsartAPI.setVisible(True)
    usiSymUsartAPI.setHelp(srv_usi_helpkeyword)
    usiSymUsartAPI.setDependencies(errorLogReportHandler, ["SRV_USI_ERROR_LOG_REPORT"])

    global usiSymCdcAPI
    usiSymCdcAPI = usiComponentCommon.createBooleanSymbol("SRV_USI_CDC_API", None)
    usiSymCdcAPI.setLabel("CDC API")
    usiSymCdcAPI.setReadOnly(True)
    usiSymCdcAPI.setDefaultValue(False)
    usiSymCdcAPI.setVisible(True)
    usiSymCdcAPI.setHelp(srv_usi_helpkeyword)

    global usiSymNumUsart
    usiSymNumUsart = usiComponentCommon.createIntegerSymbol("SRV_USI_NUM_USART", usiSymUsartAPI)
    usiSymNumUsart.setLabel("UART Connections")
    usiSymNumUsart.setDefaultValue(0)
    usiSymNumUsart.setVisible(True)
    usiSymNumUsart.setReadOnly(True)
    usiSymNumUsart.setHelp(srv_usi_helpkeyword)

    global usiSymNumCdc
    usiSymNumCdc = usiComponentCommon.createIntegerSymbol("SRV_USI_NUM_CDC", usiSymCdcAPI)
    usiSymNumCdc.setLabel("CDC Connections")
    usiSymNumCdc.setDefaultValue(0)
    usiSymNumCdc.setVisible(True)
    usiSymNumCdc.setReadOnly(True)
    usiSymNumCdc.setHelp(srv_usi_helpkeyword)

    ##### USI Files  ####################################################

    usiSourceFile = usiComponentCommon.createFileSymbol("SRV_USI_SOURCE", None)
    usiSourceFile.setSourcePath("service/usi/src/srv_usi.c.ftl")
    usiSourceFile.setOutputName("srv_usi.c")
    usiSourceFile.setDestPath("service/usi")
    usiSourceFile.setProjectPath("config/" + configName + "/service/usi/")
    usiSourceFile.setType("SOURCE")
    usiSourceFile.setMarkup(True)

    usiHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_HEADER", None)
    usiHeaderFile.setSourcePath("service/usi/srv_usi.h")
    usiHeaderFile.setOutputName("srv_usi.h")
    usiHeaderFile.setDestPath("service/usi")
    usiHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiHeaderFile.setType("HEADER")

    usiDefHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_DEF_HEADER", None)
    usiDefHeaderFile.setSourcePath("service/usi/srv_usi_definitions.h.ftl")
    usiDefHeaderFile.setOutputName("srv_usi_definitions.h")
    usiDefHeaderFile.setDestPath("service/usi")
    usiDefHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiDefHeaderFile.setType("HEADER")
    usiDefHeaderFile.setMarkup(True)

    usiLocalHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_LOCAL_HEADER", None)
    usiLocalHeaderFile.setSourcePath("service/usi/src/srv_usi_local.h")
    usiLocalHeaderFile.setOutputName("srv_usi_local.h")
    usiLocalHeaderFile.setDestPath("service/usi")
    usiLocalHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiLocalHeaderFile.setType("SOURCE")

    usiUsartSourceFile = usiComponentCommon.createFileSymbol("SRV_USI_USART_SOURCE", None)
    usiUsartSourceFile.setSourcePath("service/usi/src/srv_usi_usart.c.ftl")
    usiUsartSourceFile.setOutputName("srv_usi_usart.c")
    usiUsartSourceFile.setDestPath("service/usi")
    usiUsartSourceFile.setProjectPath("config/" + configName + "/service/usi/")
    usiUsartSourceFile.setType("SOURCE")
    usiUsartSourceFile.setEnabled(False)
    usiUsartSourceFile.setMarkup(True)
    usiUsartSourceFile.setDependencies(enableFiles, ["SRV_USI_USART_API"])

    usiUsartHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_USART_HEADER", None)
    usiUsartHeaderFile.setSourcePath("service/usi/src/srv_usi_usart.h.ftl")
    usiUsartHeaderFile.setOutputName("srv_usi_usart.h")
    usiUsartHeaderFile.setDestPath("service/usi")
    usiUsartHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiUsartHeaderFile.setType("HEADER")
    usiUsartHeaderFile.setEnabled(False)
    usiUsartHeaderFile.setMarkup(True)
    usiUsartHeaderFile.setDependencies(enableFiles, ["SRV_USI_USART_API"])

    usiCDCSourceFile = usiComponentCommon.createFileSymbol("SRV_USI_CDC_SOURCE", None)
    usiCDCSourceFile.setSourcePath("service/usi/src/srv_usi_cdc.c.ftl")
    usiCDCSourceFile.setOutputName("srv_usi_cdc.c")
    usiCDCSourceFile.setDestPath("service/usi")
    usiCDCSourceFile.setProjectPath("config/" + configName + "/service/usi/")
    usiCDCSourceFile.setType("SOURCE")
    usiCDCSourceFile.setEnabled(False)
    usiCDCSourceFile.setMarkup(True)
    usiCDCSourceFile.setDependencies(enableFiles, ["SRV_USI_CDC_API"])

    usiCDCHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_CDC_HEADER", None)
    usiCDCHeaderFile.setSourcePath("service/usi/src/srv_usi_cdc.h.ftl")
    usiCDCHeaderFile.setOutputName("srv_usi_cdc.h")
    usiCDCHeaderFile.setDestPath("service/usi")
    usiCDCHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiCDCHeaderFile.setType("HEADER")
    usiCDCHeaderFile.setOverwrite(True)
    usiCDCHeaderFile.setEnabled(False)
    usiCDCHeaderFile.setMarkup(True)
    usiCDCHeaderFile.setDependencies(enableFiles, ["SRV_USI_CDC_API"])

    #### FreeMaker Files ######################################################
    usiSymCommonSysCfgFile = usiComponentCommon.createFileSymbol("SRV_USI_SYS_CFG_COMMON", None)
    usiSymCommonSysCfgFile.setType("STRING")
    usiSymCommonSysCfgFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    usiSymCommonSysCfgFile.setSourcePath("service/usi/templates/system/system_config_common.h.ftl")
    usiSymCommonSysCfgFile.setMarkup(True)

    usiSymSystemDefIncFile = usiComponentCommon.createFileSymbol("SRV_USI_SYSTEM_DEF", None)
    usiSymSystemDefIncFile.setType("STRING")
    usiSymSystemDefIncFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    usiSymSystemDefIncFile.setSourcePath("service/usi/templates/system/system_definitions.h.ftl")
    usiSymSystemDefIncFile.setMarkup(True)

def destroyComponent(usiComponentCommon):
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":False})

