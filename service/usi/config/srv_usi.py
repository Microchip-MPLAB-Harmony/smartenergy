# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

################################################################################
#### Global Variables ####
################################################################################

################################################################################
#### Business Logic ####
################################################################################


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

    usiSymUsartAPI = usiComponent.createBooleanSymbol("SRV_USI_USART_API_INDEX", None)
    usiSymUsartAPI.setLabel("USART API INDEX")
    usiSymUsartAPI.setReadOnly(True)
    usiSymUsartAPI.setDefaultValue(False)
    usiSymUsartAPI.setVisible(True)

    usiSymCdcAPI = usiComponent.createBooleanSymbol("SRV_USI_CDC_API_INDEX", None)
    usiSymCdcAPI.setLabel("CDC API INDEX")
    usiSymCdcAPI.setReadOnly(True)
    usiSymCdcAPI.setDefaultValue(False)
    usiSymCdcAPI.setVisible(True)

    usiSymTcpAPI = usiComponent.createBooleanSymbol("SRV_USI_TCP_API_INDEX", None)
    usiSymTcpAPI.setLabel("TCP API INDEX")
    usiSymTcpAPI.setReadOnly(True)
    usiSymTcpAPI.setDefaultValue(False)
    usiSymTcpAPI.setVisible(True)

    usiSymCommAPI = usiComponent.createStringSymbol("SRV_USI_COMM_API", None)
    usiSymCommAPI.setLabel("Communication API Used")
    usiSymCommAPI.setReadOnly(True)
    usiSymCommAPI.setDefaultValue("")

    global usiUsartTXDMAChannel
    usiUsartTXDMAChannel = usiComponent.createIntegerSymbol("SRV_USI_USART_TX_DMA_CHANNEL", None)
    usiUsartTXDMAChannel.setLabel("DMA Channel For Transmit")
    usiUsartTXDMAChannel.setDefaultValue(0)
    usiUsartTXDMAChannel.setVisible(False)
    usiUsartTXDMAChannel.setReadOnly(True)
    usiUsartTXDMAChannel.setDependencies(requestAndAssignTxDMAChannel, ["SRV_USI_USART_API_INDEX"])

    usiSymReadBufferSize = usiComponent.createIntegerSymbol("SRV_USI_RD_BUF_SIZE", None)
    usiSymReadBufferSize.setLabel("Read Buffer Size")
    usiSymReadBufferSize.setDefaultValue(1024)

    usiSymReadBufferSize = usiComponent.createIntegerSymbol("SRV_USI_WR_BUF_SIZE", None)
    usiSymReadBufferSize.setLabel("Write Buffer Size")
    usiSymReadBufferSize.setDefaultValue(1024)

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

    # System Files
    configName = Variables.get("__CONFIGURATION_NAME")

    usiUsartSourceFile = usiComponent.createFileSymbol("SRV_USI_USART_SOURCE", None)
    usiUsartSourceFile.setSourcePath("service/usi/src/srv_usi_usart.c")
    usiUsartSourceFile.setOutputName("srv_usi_usart.c")
    usiUsartSourceFile.setDestPath("service/usi")
    usiUsartSourceFile.setProjectPath("config/" + configName + "/service/usi/")
    usiUsartSourceFile.setType("SOURCE")
    usiUsartSourceFile.setOverwrite(True)
    usiUsartSourceFile.setEnabled(False)

    usiUsartHeaderFile = usiComponent.createFileSymbol("SRV_USI_USART_HEADER", None)
    usiUsartHeaderFile.setSourcePath("service/usi/src/srv_usi_usart.h")
    usiUsartHeaderFile.setOutputName("srv_usi_usart.h")
    usiUsartHeaderFile.setDestPath("service/usi")
    usiUsartHeaderFile.setProjectPath("config/" + configName + "/service/usi/")
    usiUsartHeaderFile.setType("SOURCE")
    usiUsartHeaderFile.setOverwrite(True)
    usiUsartHeaderFile.setEnabled(False)



############################################################################
#### Dependency ####
############################################################################
def onAttachmentConnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    targetID = target["id"]

    print("OnConnection..." + connectID)
    print(remoteID)

    if connectID == "srv_usi_USART_dependency" :
        print("OnConnection...srv_usi_USART_dependency")

        print("Set PLIB connection")
        plibUsed = localComponent.getSymbolByID("SRV_USI_COMM_API")
        plibUsed.clearValue()
        plibUsed.setValue(remoteID.upper(), 0)

        apiUsedIndex = localComponent.getSymbolByID("SRV_USI_USART_API_INDEX")
        apiUsedIndex.clearValue()
        apiUsedIndex.setValue(True, 1)

        Database.setSymbolValue("srv_usi", "SRV_USI_USART_API", True, 1)

        # Do not change the order as DMA Channels needs to be allocated
        # after setting the Usart Api index symbol
        usiUsartTXDMAChannel.setVisible(True)

#    if connectID == "srv_usi_TCP_AUX_dependency" :
#        print("OnConnection...srv_usi_TCP_AUX_dependency")
#        
#        print("Set PLIB connection")
#        plibUsed = localComponent.getSymbolByID("SRV_USI_COMM_API")
#        plibUsed.clearValue()
#        plibUsed.setValue(remoteID.upper(), 0)
#
#        Database.setSymbolValue("srv_usi", "SRV_USI_TCP_API", True, 1)
#        apiUsedIndex = localComponent.getSymbolByID("SRV_USI_TCP_API_INDEX")
#        apiUsedIndex.clearValue()
#        apiUsedIndex.setValue(True, 0)
#
#    if connectID == "srv_usi_CDC_dependency" :
#        print("OnConnection...srv_usi_CDC_dependency")
#        
#        print("Set PLIB connection")
#        plibUsed = localComponent.getSymbolByID("SRV_USI_COMM_API")
#        plibUsed.clearValue()
#        plibUsed.setValue(remoteID.upper(), 0)
#
#        Database.setSymbolValue("srv_usi", "SRV_USI_CDC_API", True, 1)
#        apiUsedIndex = localComponent.getSymbolByID("SRV_USI_CDC_API_INDEX")
#        apiUsedIndex.clearValue()
#        apiUsedIndex.setValue(True, 0)


def onAttachmentDisconnected(source, target):
    global isDMAPresent

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    targetID = target["id"]

    print("OnDisconnection..." + connectID)

    if connectID == "srv_usi_USART_dependency" :
        print("OnDisconnection...srv_usi_USART_dependency")

        # Do not change the order as DMA Channels needs to be cleared
        # before clearing the Usart Api Index symbol
        usiUsartTXDMAChannel.setVisible(False)

        Database.setSymbolValue("srv_usi", "SRV_USI_USART_API", False, 1)
        apiUsedIndex = localComponent.getSymbolByID("SRV_USI_USART_API_INDEX").clearValue()

        print("Clear PLIB connection")
        plibUsed = localComponent.getSymbolByID("SRV_USI_COMM_API")
        plibUsed.clearValue()

#    if connectID == "srv_usi_TCP_AUX_dependency" :
#        print("OnDisconnection...srv_usi_TCP_AUX_dependency")
#
#        Database.setSymbolValue("srv_usi", "SRV_USI_TCP_API", False, 1)
#        apiUsedIndex = localComponent.getSymbolByID("SRV_USI_TCP_API_INDEX").clearValue()
#
#        print("Clear PLIB connection")
#        plibUsed = localComponent.getSymbolByID("SRV_USI_COMM_API")
#        plibUsed.clearValue()
#
#    if connectID == "srv_usi_CDC_dependency" :
#        print("OnDisconnection...srv_usi_CDC_dependency")
#
#        Database.setSymbolValue("srv_usi", "SRV_USI_CDC_API", False, 1)
#        localComponent.getSymbolByID("SRV_USI_CDC_API_INDEX").clearValue()
#
#        print("Clear PLIB connection")
#        plibUsed = localComponent.getSymbolByID("SRV_USI_COMM_API")
#        plibUsed.clearValue()

def requestAndAssignTxDMAChannel(symbol, event):
    global srvUsiInstanceSpace

    usiService = Database.getSymbolValue(srvUsiInstanceSpace, "SRV_USI_COMM_API")

    dmaChannelID = "DMA_CH_FOR_" + str(usiService) + "_Transmit"
    dmaRequestID = "DMA_CH_NEEDED_FOR_" + str(usiService) + "_Transmit"

    print("requestAndAssignTxDMAChannel : " + dmaChannelID)
    print("requestAndAssignTxDMAChannel : " + dmaRequestID)

    if event["value"] == False:
        Database.setSymbolValue("core", dmaRequestID, False, 2)
        symbol.setVisible(False)
    else:
        symbol.setVisible(True)
        Database.setSymbolValue("core", dmaRequestID, True, 2)

    # Get the allocated channel and assign it
    channel = Database.getSymbolValue("core", dmaChannelID)
    symbol.setValue(channel, 2)

def destroyComponent(spiComponent):
    global srvUsiInstanceSpace

    usiService = Database.getSymbolValue(srvUsiInstanceSpace, "SRV_USI_COMM_API")
    dmaRequestID = "DMA_CH_NEEDED_FOR_" + str(usiService) + "_Transmit"
    Database.setSymbolValue("core", dmaRequestID, False, 2) 