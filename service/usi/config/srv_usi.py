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

def selectDeviceSet(symbol, event):
    symbol.clearValue()
    if ("USART" in event["value"]) or ("UART" in event["value"]) or ("SERCOM" in event["value"]) or ("FLEXCOM" in event["value"]):
        symbol.setValue("UART")
    elif "USB" in event["value"]:
        symbol.setValue("USB_CDC")
    else:
        symbol.setValue("")

################################################################################
#### Component ####
################################################################################
def instantiateComponent(usiComponent, index):
    global srvUsiInstanceSpace

    Log.writeInfoMessage("Loading PLC USI Service...instance:" + str(index))

    srvUsiInstanceSpace = "srv_usi_" + str(index)

    usiSymIndex = usiComponent.createIntegerSymbol("INDEX", None)
    usiSymIndex.setVisible(False)
    usiSymIndex.setDefaultValue(index)

    usiSymDevice = usiComponent.createStringSymbol("SRV_USI_DEVICE", None)
    usiSymDevice.setLabel("Device Used")
    usiSymDevice.setReadOnly(True)
    usiSymDevice.setDefaultValue("")
    usiSymDevice.setHelp(srv_usi_helpkeyword)

    usiSymDeviceSet = usiComponent.createStringSymbol("SRV_USI_DEVICE_SET", None)
    usiSymDeviceSet.setLabel("Device Set")
    usiSymDeviceSet.setDefaultValue("")
    usiSymDeviceSet.setDependencies(selectDeviceSet, ["SRV_USI_DEVICE"])
    usiSymDeviceSet.setVisible(False)

    usiSymUsartAPI = usiComponent.createIntegerSymbol("SRV_USI_USART_API_INDEX", None)
    usiSymUsartAPI.setLabel("USART API INDEX")
    usiSymUsartAPI.setDefaultValue(0)
    usiSymUsartAPI.setVisible(False)

    usiSymCdcAPI = usiComponent.createIntegerSymbol("SRV_USI_CDC_API_INDEX", None)
    usiSymCdcAPI.setLabel("CDC API INDEX")
    usiSymCdcAPI.setDefaultValue(0)
    usiSymCdcAPI.setVisible(False)

    usiSymDeviceIndex = usiComponent.createIntegerSymbol("SRV_USI_CDC_DEVICE_INDEX", None)
    usiSymDeviceIndex.setLabel("CDC Device Index")
    usiSymDeviceIndex.setVisible(False)
    usiSymDeviceIndex.setDefaultValue(0)

    usiSymReadBufferSize = usiComponent.createIntegerSymbol("SRV_USI_RD_BUF_SIZE", None)
    usiSymReadBufferSize.setLabel("Read Buffer Size")
    usiSymReadBufferSize.setDefaultValue(1024)
    usiSymReadBufferSize.setMin(8)
    usiSymReadBufferSize.setMax(32*1024)
    usiSymReadBufferSize.setHelp(srv_usi_helpkeyword)

    usiSymReadBufferSize = usiComponent.createIntegerSymbol("SRV_USI_WR_BUF_SIZE", None)
    usiSymReadBufferSize.setLabel("Write Buffer Size")
    usiSymReadBufferSize.setDefaultValue(1024)
    usiSymReadBufferSize.setMin(8)
    usiSymReadBufferSize.setMax(32*1024)
    usiSymReadBufferSize.setHelp(srv_usi_helpkeyword)

    ############################################################################
    #### Code Generation ####
    ############################################################################
    
    # System Template Files
    usiSymSystemDefObjFile = usiComponent.createFileSymbol("SRV_USI_FILE_SYS_DEF_OBJ", None)
    usiSymSystemDefObjFile.setType("STRING")
    usiSymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    usiSymSystemDefObjFile.setSourcePath("service/usi/templates/system/system_definitions_objects.h.ftl")
    usiSymSystemDefObjFile.setMarkup(True)

    usiSymSystemConfigFile = usiComponent.createFileSymbol("SRV_USI_FILE_SYS_CONFIG", None)
    usiSymSystemConfigFile.setType("STRING")
    usiSymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    usiSymSystemConfigFile.setSourcePath("service/usi/templates/system/system_config.h.ftl")
    usiSymSystemConfigFile.setMarkup(True)

    usiSymSystemInitFile = usiComponent.createFileSymbol("SRV_USI_FILE_SYS_INIT", None)
    usiSymSystemInitFile.setType("STRING")
    usiSymSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    usiSymSystemInitFile.setSourcePath("service/usi/templates/system/system_initialize.c.ftl")
    usiSymSystemInitFile.setMarkup(True)

    usiSymSystemInitDataFile = usiComponent.createFileSymbol("SRV_USI_FILE_SYS_INIT_DATA", None)
    usiSymSystemInitDataFile.setType("STRING")
    usiSymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    usiSymSystemInitDataFile.setSourcePath("service/usi/templates/system/system_initialize_data.c.ftl")
    usiSymSystemInitDataFile.setMarkup(True)

    usiSystemTasksFile = usiComponent.createFileSymbol("SRV_USI_SYS_TASK", None)
    usiSystemTasksFile.setType("STRING")
    usiSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
    usiSystemTasksFile.setSourcePath("service/usi/templates/system/system_tasks.c.ftl")
    usiSystemTasksFile.setMarkup(True)

################################################################################
#### Business Logic ####
################################################################################  
def onAttachmentConnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    dict = {}

    if connectID == "srv_usi_USART_dependency":
        plibUsed = localComponent.getSymbolByID("SRV_USI_DEVICE")
        plibUsed.clearValue()
        plibUsed.setValue(remoteID.upper())
        dict = Database.sendMessage("srv_usi", "USI_ADD_USART_API", {})
    
    if connectID == "srv_usi_CDC_dependency":
        plibUsed = localComponent.getSymbolByID("SRV_USI_DEVICE")
        plibUsed.clearValue()
        plibUsed.setValue(remoteID.upper())
        cdcSymDeviceIndex = localComponent.getSymbolByID("SRV_USI_CDC_DEVICE_INDEX")
        cdcSymDeviceIndex.setValue(Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_FUNCTION_INDEX"))
        dict = Database.sendMessage("srv_usi", "USI_ADD_CDC_API", {})

def onAttachmentDisconnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    dict = {}

    if connectID == "srv_usi_USART_dependency":
        localComponent.getSymbolByID("SRV_USI_DEVICE").clearValue()
        dict = Database.sendMessage("srv_usi", "USI_REMOVE_USART_API", {})
        dict = Database.sendMessage(remoteID, "UART_NON_BLOCKING_MODE", {"isReadOnly":False})
    
    if connectID == "srv_usi_CDC_dependency":
        localComponent.getSymbolByID("SRV_USI_DEVICE").clearValue()
        dict = Database.sendMessage("srv_usi", "USI_REMOVE_CDC_API", {})

def handleMessage(messageID, args):

    result_dict = {}

    if (messageID == "REQUEST_CONFIG_PARAMS"):
        if args.get("localComponentID") != None:
            result_dict = Database.sendMessage(args["localComponentID"], "UART_NON_BLOCKING_MODE", {"isEnabled":True, "isReadOnly":True})

    return result_dict

