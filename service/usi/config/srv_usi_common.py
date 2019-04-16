# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
def setConnections(symbol, event):
    symbol.setVisible(event["value"]) 
    if(event["value"] == False):
        symbol.clearValue()
    else:
        symbol.setValue(1, 0)        

def enableUsartFiles(symbol, event):
    symbol.setEnabled(event["value"])  
    if(event["value"]):
        print("Enable USART files")
    else:
        print("Disable USART files")

def instantiateComponent(usiComponentCommon):
    
    Log.writeInfoMessage("Loading PLC USI Common Service")

    res = Database.activateComponents(["HarmonyCore"])

    # Enable "Generate Harmony Driver Common Files" option in MHC
    Database.setSymbolValue("HarmonyCore", "ENABLE_DRV_COMMON", True, 1)

    # Enable "Generate Harmony System Service Common Files" option in MHC
    Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_COMMON", True, 1)

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    usiSymMsgPoolSize = usiComponentCommon.createIntegerSymbol("SRV_USI_MSG_POOL_SIZE", None)
    usiSymMsgPoolSize.setLabel("Received Message Pool Size")
    usiSymMsgPoolSize.setMin(1)
    usiSymMsgPoolSize.setMax(10)
    usiSymMsgPoolSize.setDefaultValue(5)

    usiSymUsartAPI = usiComponentCommon.createBooleanSymbol("SRV_USI_USART_API", None)
    usiSymUsartAPI.setLabel("USART API")
    usiSymUsartAPI.setReadOnly(True)
    usiSymUsartAPI.setDefaultValue(False)
    usiSymUsartAPI.setVisible(True)

    usiSymCdcAPI = usiComponentCommon.createBooleanSymbol("SRV_USI_CDC_API", None)
    usiSymCdcAPI.setLabel("CDC API")
    usiSymCdcAPI.setReadOnly(True)
    usiSymCdcAPI.setDefaultValue(False)
    usiSymCdcAPI.setVisible(True)

    usiSymTcpAPI = usiComponentCommon.createBooleanSymbol("SRV_USI_TCP_API", None)
    usiSymTcpAPI.setLabel("TCP API")
    usiSymTcpAPI.setReadOnly(True)
    usiSymTcpAPI.setDefaultValue(False)
    usiSymTcpAPI.setVisible(True)

    usiSymNumUsart = usiComponentCommon.createIntegerSymbol("SRV_USI_NUM_USART", usiSymUsartAPI)
    usiSymNumUsart.setLabel("USART Connections")
    usiSymNumUsart.setDefaultValue(0)
    usiSymNumUsart.setVisible(True)
    usiSymNumUsart.setDependencies(setConnections, ["SRV_USI_USART_API"])

    usiSymNumCdc = usiComponentCommon.createIntegerSymbol("SRV_USI_NUM_CDC", usiSymCdcAPI)
    usiSymNumCdc.setLabel("CDC Connections")
    usiSymNumCdc.setDefaultValue(0)
    usiSymNumCdc.setVisible(True)
    usiSymNumCdc.setDependencies(setConnections, ["SRV_USI_CDC_API"])

    usiSymNumTcp = usiComponentCommon.createIntegerSymbol("SRV_USI_NUM_TCP", usiSymTcpAPI)
    usiSymNumTcp.setLabel("TCP Connections")
    usiSymNumTcp.setDefaultValue(0)
    usiSymNumTcp.setVisible(True)
    usiSymNumTcp.setDependencies(setConnections, ["SRV_USI_TCP_API"])

    usiHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_HEADER", None)
    usiHeaderFile.setSourcePath("service/usi/srv_usi.h")
    usiHeaderFile.setOutputName("srv_usi.h")
    usiHeaderFile.setDestPath("service/usi")
    usiHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiHeaderFile.setType("HEADER")
    usiHeaderFile.setOverwrite(True)

    # Source Files
    usiSourceFile = usiComponentCommon.createFileSymbol("SRV_USI_SOURCE", None)
    usiSourceFile.setSourcePath("service/usi/src/srv_usi.c.ftl")
    usiSourceFile.setOutputName("srv_usi.c")
    usiSourceFile.setDestPath("service/usi")
    usiSourceFile.setProjectPath("config/" + configName + "/service/usi/")
    usiSourceFile.setType("SOURCE")
    usiSourceFile.setMarkup(True)
    usiSourceFile.setOverwrite(True)
    usiSourceFile.setEnabled(True)

    usiHeaderLocalFile = usiComponentCommon.createFileSymbol("SRV_USI_SOURCE_LOCAL", None)
    usiHeaderLocalFile.setSourcePath("service/usi/src/srv_usi_local.h.ftl")
    usiHeaderLocalFile.setOutputName("srv_usi_local.h")
    usiHeaderLocalFile.setDestPath("service/usi")
    usiHeaderLocalFile.setProjectPath("config/" + configName + "/service/usi/")
    usiHeaderLocalFile.setType("SOURCE")
    usiHeaderLocalFile.setMarkup(True)
    usiHeaderLocalFile.setOverwrite(True)
    usiHeaderLocalFile.setEnabled(True)

    usiSymHeaderDefFile = usiComponentCommon.createFileSymbol("DRV_USI_FILE_DEF_HEADER", None)
    usiSymHeaderDefFile.setSourcePath("service/usi/templates/srv_usi_definitions.h.ftl")
    usiSymHeaderDefFile.setOutputName("srv_usi_definitions.h")
    usiSymHeaderDefFile.setDestPath("service/usi/")
    usiSymHeaderDefFile.setProjectPath("config/" + configName + "/service/usi/")
    usiSymHeaderDefFile.setType("HEADER")
    usiSymHeaderDefFile.setMarkup(True)
    usiSymHeaderDefFile.setOverwrite(True)

    # USART wrapper Files
    usiUsartSourceFile = usiComponentCommon.createFileSymbol("SRV_USI_USART_SOURCE", None)
    usiUsartSourceFile.setSourcePath("service/usi/src/srv_usi_usart.c")
    usiUsartSourceFile.setOutputName("srv_usi_usart.c")
    usiUsartSourceFile.setDestPath("service/usi")
    usiUsartSourceFile.setProjectPath("config/" + configName + "/service/usi/")
    usiUsartSourceFile.setType("SOURCE")
    usiUsartSourceFile.setMarkup(False)
    usiUsartSourceFile.setOverwrite(True)
    usiUsartSourceFile.setEnabled(False)
    usiUsartSourceFile.setDependencies(enableUsartFiles, ["SRV_USI_USART_API"])

    usiUsartHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_USART_HEADER", None)
    usiUsartHeaderFile.setSourcePath("service/usi/src/srv_usi_usart.h")
    usiUsartHeaderFile.setOutputName("srv_usi_usart.h")
    usiUsartHeaderFile.setDestPath("service/usi")
    usiUsartHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiUsartHeaderFile.setType("SOURCE")
    usiUsartHeaderFile.setMarkup(False)
    usiUsartHeaderFile.setOverwrite(True)
    usiUsartHeaderFile.setEnabled(False)
    usiUsartHeaderFile.setDependencies(enableUsartFiles, ["SRV_USI_USART_API"])

    # System Template Files
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

