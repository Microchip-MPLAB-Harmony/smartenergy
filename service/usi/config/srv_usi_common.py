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

global srv_usi_helpkeyword
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

def instantiateComponent(usiComponentCommon):

    Log.writeInfoMessage("Loading PLC USI Service...common")

    res = Database.activateComponents(["HarmonyCore"])

    # Enable "Generate Harmony Driver Common Files" option in MHC
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":True})

    # Enable "Generate Harmony System Service Common Files" option in MHC
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":True})

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

    global usiSymUsartAPI
    usiSymUsartAPI = usiComponentCommon.createBooleanSymbol("SRV_USI_USART_API", None)
    usiSymUsartAPI.setLabel("UART API")
    usiSymUsartAPI.setReadOnly(True)
    usiSymUsartAPI.setDefaultValue(False)
    usiSymUsartAPI.setVisible(True)
    usiSymUsartAPI.setHelp(srv_usi_helpkeyword)

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
    usiSourceFile.setSourcePath("service/usi/src/srv_usi.c")
    usiSourceFile.setOutputName("srv_usi.c")
    usiSourceFile.setDestPath("service/usi")
    usiSourceFile.setProjectPath("config/" + configName + "/service/usi/")
    usiSourceFile.setType("SOURCE")
    
    usiHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_HEADER", None)
    usiHeaderFile.setSourcePath("service/usi/srv_usi.h")
    usiHeaderFile.setOutputName("srv_usi.h")
    usiHeaderFile.setDestPath("service/usi")
    usiHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiHeaderFile.setType("HEADER")
    
    usiDefHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_DEF_HEADER", None)
    usiDefHeaderFile.setSourcePath("service/usi/srv_usi_definitions.h")
    usiDefHeaderFile.setOutputName("srv_usi_definitions.h")
    usiDefHeaderFile.setDestPath("service/usi")
    usiDefHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiDefHeaderFile.setType("HEADER")
    
    usiLocalHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_LOCAL_HEADER", None)
    usiLocalHeaderFile.setSourcePath("service/usi/src/srv_usi_local.h")
    usiLocalHeaderFile.setOutputName("srv_usi_local.h")
    usiLocalHeaderFile.setDestPath("service/usi")
    usiLocalHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiLocalHeaderFile.setType("SOURCE")
    
    usiUsartSourceFile = usiComponentCommon.createFileSymbol("SRV_USI_USART_SOURCE", None)
    usiUsartSourceFile.setSourcePath("service/usi/src/srv_usi_usart.c")
    usiUsartSourceFile.setOutputName("srv_usi_usart.c")
    usiUsartSourceFile.setDestPath("service/usi")
    usiUsartSourceFile.setProjectPath("config/" + configName + "/service/usi/")
    usiUsartSourceFile.setType("SOURCE")
    usiUsartSourceFile.setEnabled(False)
    usiUsartSourceFile.setDependencies(enableFiles, ["SRV_USI_USART_API"])
    
    usiUsartHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_USART_HEADER", None)
    usiUsartHeaderFile.setSourcePath("service/usi/src/srv_usi_usart.h")
    usiUsartHeaderFile.setOutputName("srv_usi_usart.h")
    usiUsartHeaderFile.setDestPath("service/usi")
    usiUsartHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiUsartHeaderFile.setType("HEADER")
    usiUsartHeaderFile.setEnabled(False)
    usiUsartHeaderFile.setDependencies(enableFiles, ["SRV_USI_USART_API"])
    
    usiCDCSourceFile = usiComponentCommon.createFileSymbol("SRV_USI_CDC_SOURCE", None)
    usiCDCSourceFile.setSourcePath("service/usi/src/srv_usi_cdc.c")
    usiCDCSourceFile.setOutputName("srv_usi_cdc.c")
    usiCDCSourceFile.setDestPath("service/usi")
    usiCDCSourceFile.setProjectPath("config/" + configName + "/service/usi/")
    usiCDCSourceFile.setType("SOURCE")
    usiCDCSourceFile.setEnabled(False)
    usiCDCSourceFile.setDependencies(enableFiles, ["SRV_USI_CDC_API"])
    
    usiCDCHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_CDC_HEADER", None)
    usiCDCHeaderFile.setSourcePath("service/usi/src/srv_usi_cdc.h")
    usiCDCHeaderFile.setOutputName("srv_usi_cdc.h")
    usiCDCHeaderFile.setDestPath("service/usi")
    usiCDCHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiCDCHeaderFile.setType("HEADER")
    usiCDCHeaderFile.setOverwrite(True)
    usiCDCHeaderFile.setEnabled(False)
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

