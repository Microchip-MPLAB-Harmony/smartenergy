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
#### Component ####
################################################################################
def instantiateComponent(usiComponent, index):
    global srvUsiInstanceSpace
    global isDMAPresent

    Log.writeInfoMessage("Loading PLC USI Service...instance:" + str(index))

    srvUsiInstanceSpace = "srv_usi_" + str(index)

    # Enable "ENABLE_SYS_DMA" option in MHC
    if Database.getSymbolValue("core", "DMA_ENABLE") == None:
        isDMAPresent = False
        Log.writeInfoMessage("isDMAPresent = False")
    else:
        isDMAPresent = True
        Log.writeInfoMessage("isDMAPresent = False")

        # Enable "Enable System DMA" option in MHC
        if (Database.getSymbolValue("HarmonyCore", "ENABLE_SYS_DMA") == False):
            Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_DMA", True)

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
    usiSymCdcAPI.setVisible(False)

    usiSymTcpAPI = usiComponent.createBooleanSymbol("SRV_USI_TCP_API_INDEX", None)
    usiSymTcpAPI.setLabel("TCP API INDEX")
    usiSymTcpAPI.setReadOnly(True)
    usiSymTcpAPI.setDefaultValue(False)
    usiSymTcpAPI.setVisible(False)

    usiSymCommAPI = usiComponent.createStringSymbol("SRV_USI_PLIB", None)
    usiSymCommAPI.setLabel("Communication API Used")
    usiSymCommAPI.setReadOnly(True)
    usiSymCommAPI.setDefaultValue("")

    usiSymReadBufferSize = usiComponent.createIntegerSymbol("SRV_USI_RD_BUF_SIZE", None)
    usiSymReadBufferSize.setLabel("Read Buffer Size")
    usiSymReadBufferSize.setDefaultValue(1024)

    usiSymReadBufferSize = usiComponent.createIntegerSymbol("SRV_USI_WR_BUF_SIZE", None)
    usiSymReadBufferSize.setLabel("Write Buffer Size")
    usiSymReadBufferSize.setDefaultValue(1024)

    global usiTXDMA
    usiTXDMA = usiComponent.createBooleanSymbol("SRV_USI_TX_DMA", None)
    usiTXDMA.setLabel("Use DMA for Transmit ?")
    usiTXDMA.setVisible(isDMAPresent)
    usiTXDMA.setDefaultValue(isDMAPresent)
    usiTXDMA.setReadOnly(True)            
    
    global usiUsartTXDMAChannel
    usiUsartTXDMAChannel = usiComponent.createIntegerSymbol("SRV_USI_USART_TX_DMA_CHANNEL", None)
    usiUsartTXDMAChannel.setLabel("DMA Channel For Transmit")
    usiUsartTXDMAChannel.setDefaultValue(0)
    usiUsartTXDMAChannel.setVisible(False)
    usiUsartTXDMAChannel.setReadOnly(True)
    usiUsartTXDMAChannel.setDependencies(requestAndAssignTxDMAChannel, ["SRV_USI_TX_DMA"])

    global usiUsartTXDMAChannelComment
    usiUsartTXDMAChannelComment = usiComponent.createCommentSymbol("SRV_USI_USART_TX_DMA_CH_COMMENT", None)
    usiUsartTXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Transmit. Check DMA Manager. !!!")
    usiUsartTXDMAChannelComment.setVisible(False)
    usiUsartTXDMAChannelComment.setDependencies(requestDMAComment, ["SRV_USI_USART_TX_DMA_CHANNEL"])

    usiUsartDependencyDMAComment = usiComponent.createCommentSymbol("SRV_USI_USART_DEPENDENCY_DMA_COMMENT", None)
    usiUsartDependencyDMAComment.setLabel("!!! Satisfy PLIB Dependency to Allocate DMA Channel !!!")
    usiUsartDependencyDMAComment.setVisible(isDMAPresent)

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
    global isDMAPresent

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    if connectID == "srv_usi_USART_dependency":
        plibUsed = localComponent.getSymbolByID("SRV_USI_PLIB")
        plibUsed.clearValue()
        plibUsed.setValue(remoteID.upper())
        
        # Set USART USI API index
        apiUsedIndex = localComponent.getSymbolByID("SRV_USI_USART_API_INDEX")
        apiUsedIndex.clearValue()
        apiUsedIndex.setValue(True)

        # localComponent.setSymbolValue("SRV_USI_USART_API", True)
        Database.setSymbolValue("srv_usi", "SRV_USI_USART_API", True)

        dmaChannelSym = Database.getSymbolValue("core", "DMA_CH_FOR_" + remoteID.upper() + "_Transmit")
        dmaRequestSym = Database.getSymbolValue("core", "DMA_CH_NEEDED_FOR_" + remoteID.upper() + "_Transmit")

        # Do not change the order as DMA Channels needs to be allocated
        # after setting the plibUsed symbol
        # Both device and connected plib should support DMA
        if isDMAPresent == True and dmaChannelSym != None and dmaRequestSym != None:
            localComponent.getSymbolByID("SRV_USI_TX_DMA").setValue(1)
            localComponent.getSymbolByID("SRV_USI_USART_DEPENDENCY_DMA_COMMENT").setVisible(False)
        else:
            print("[CHIRS_dbg] : USI connect USI DMA ERROR")
            localComponent.getSymbolByID("SRV_USI_USART_TX_DMA_CH_COMMENT").setVisible(True)

def onAttachmentDisconnected(source, target):
    global isDMAPresent

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    print("[CHIRS_dbg] : USI onAttachmentDisconnected")

    if connectID == "srv_usi_USART_dependency":

        print("[CHIRS_dbg] : USI connect srv_usi_USART_dependency")

        dmaChannelSym = Database.getSymbolValue("core", "DMA_CH_FOR_" + remoteID.upper() + "_Transmit")
        dmaRequestSym = Database.getSymbolValue("core", "DMA_CH_NEEDED_FOR_" + remoteID.upper() + "_Transmit")

        # Do not change the order as DMA Channels needs to be cleared
        # before clearing the plibUsed symbol
        # Both device and connected plib should support DMA
        if isDMAPresent == True and dmaChannelSym != None and dmaRequestSym != None:
            print("[CHIRS_dbg] : USI connect srv_usi_USART_dependency remoteID.upper " + remoteID.upper())
            localComponent.getSymbolByID("SRV_USI_TX_DMA").setValue(0)
            localComponent.getSymbolByID("SRV_USI_USART_DEPENDENCY_DMA_COMMENT").setVisible(True)
        else:
            print("[CHIRS_dbg] : USI disconnect DMA ERROR")
            localComponent.getSymbolByID("DRV_PLC_TX_DMA_CH_COMMENT").setVisible(True)

        localComponent.getSymbolByID("SRV_USI_PLIB").clearValue()
        localComponent.getSymbolByID("SRV_USI_USART_API_INDEX").clearValue()
        Database.setSymbolValue("srv_usi", "SRV_USI_USART_API", False)
        # localComponent.setSymbolValue("SRV_USI_USART_API", False)

def requestAndAssignTxDMAChannel(symbol, event):
    global srvUsiInstanceSpace
    global usiUsartTXDMAChannelComment

    usartPeripheral = Database.getSymbolValue(srvUsiInstanceSpace, "SRV_USI_PLIB")

    dmaChannelID = "DMA_CH_FOR_" + str(usartPeripheral) + "_Transmit"
    dmaRequestID = "DMA_CH_NEEDED_FOR_" + str(usartPeripheral) + "_Transmit"

    # Clear the DMA symbol. Done for backward compatibility.
    Database.clearSymbolValue("core", dmaRequestID)

    dummyDict = {}

    if event["value"] == False:
        print("[CHIRS_dbg] : USI requestAndAssignTxDMAChannel event False" + dmaRequestID)
        dummyDict = Database.sendMessage("core", "DMA_CHANNEL_DISABLE", {"dma_channel":dmaRequestID})
        usiUsartTXDMAChannelComment.setVisible(False)
        symbol.setVisible(False)
    else:
        print("[CHIRS_dbg] : USI requestAndAssignTxDMAChannel event True" + dmaRequestID)
        symbol.setVisible(True)
        dummyDict = Database.sendMessage("core", "DMA_CHANNEL_ENABLE", {"dma_channel":dmaRequestID})

    # Get the allocated channel and assign it
    channel = Database.getSymbolValue("core", dmaChannelID)
    symbol.setValue(channel)

def requestDMAComment(symbol, event):
    global usiTXDMA

    if(event["value"] == -2) and (usiTXDMA.getValue() == True):
        symbol.setVisible(True)
        event["symbol"].setVisible(False)
    else:
        symbol.setVisible(False)
