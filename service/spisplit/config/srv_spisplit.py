# coding: utf-8
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
import math

global clientList
clientList = []

def instantiateComponent(spiSplitterComponent):
    
    print("Loading PLC-RF SPI Splitter Service")

    global isDMAPresent
    if Database.getSymbolValue("core", "DMA_ENABLE") != None:
        # DMA is present (XDMAC for SAME70)
        isDMAPresent = True
    else:
        # DMA is not present (PDC is used for SAMG55/PIC32CXMT)
        isDMAPresent = False

    plibUsed = spiSplitterComponent.createStringSymbol("SRV_SPISPLIT_PLIB", None)
    plibUsed.setLabel("PLIB Used")
    plibUsed.setDescription("PLIB connected to SPI dependency")
    plibUsed.setReadOnly(True)

    spiDependencyComment = spiSplitterComponent.createCommentSymbol("SRV_SPISPLIT_SPI_DEPENDENCY_COMMENT", None)
    spiDependencyComment.setLabel("!!! Satisfy SPI Dependency !!!")

    global dmaChannelCount
    if isDMAPresent:    
        dmaTxChannel = spiSplitterComponent.createIntegerSymbol("SRV_SPISPLIT_TX_DMA_CHANNEL", None)
        dmaTxChannel.setLabel("DMA Channel For Transmit")
        dmaTxChannel.setDescription("Allocated DMA channel for SPI Transmit")
        dmaTxChannel.setMin(-1)
        dmaTxChannel.setReadOnly(True)
        dmaTxChannel.setVisible(False)

        dmaTxChannelComment = spiSplitterComponent.createCommentSymbol("SRV_SPISPLIT_TX_DMA_CH_COMMENT", None)
        dmaTxChannelComment.setLabel("!!! Couldn't Allocate DMA Channel for Transmit. Check DMA manager !!!")
        dmaTxChannelComment.setVisible(False)

        dmaRxChannel = spiSplitterComponent.createIntegerSymbol("SRV_SPISPLIT_RX_DMA_CHANNEL", None)
        dmaRxChannel.setLabel("DMA Channel For Receive")
        dmaRxChannel.setDescription("Allocated DMA channel for SPI Receive")
        dmaRxChannel.setMin(-1)
        dmaRxChannel.setReadOnly(True)
        dmaRxChannel.setVisible(False)

        dmaRxChannelComment = spiSplitterComponent.createCommentSymbol("SRV_SPISPLIT_RX_DMA_CH_COMMENT", None)
        dmaRxChannelComment.setLabel("!!! Couldn't Allocate DMA Channel for Receive. Check DMA manager !!!")
        dmaRxChannelComment.setVisible(False)

        # Get number of DMA channels
        dmaChannelCount = Database.getSymbolValue("core", "DMA_CHANNEL_COUNT")
        if dmaChannelCount != None:
            dmaTxChannel.setMax(dmaChannelCount - 1)
            dmaRxChannel.setMax(dmaChannelCount - 1)

################################################################################
#### Business Logic ####
################################################################################  
def destroyComponent(spiSplitterComponent):
    pass

def onAttachmentConnected(source, target):
    localComponent = source["component"]
    localConnectID = source["id"]
    remoteComponent = target["component"]
    remoteComponentID = remoteComponent.getID()

    plibUsed = localComponent.getSymbolByID("SRV_SPISPLIT_PLIB")
    dmaTxChannel = localComponent.getSymbolByID("SRV_SPISPLIT_TX_DMA_CHANNEL")
    dmaRxChannel = localComponent.getSymbolByID("SRV_SPISPLIT_RX_DMA_CHANNEL")

    if localConnectID == "srv_spiplit_SPI_dependency":
        # Set PLIB Used
        plibUsed.setValue(remoteComponentID.upper())
        localComponent.getSymbolByID("SRV_SPISPLIT_SPI_DEPENDENCY_COMMENT").setVisible(False)

        if isDMAPresent:
            # Request DMA channels for Transmit
            dmaChannelID = "DMA_CH_FOR_" + remoteComponentID.upper() + "_Transmit"
            dmaRequestID = "DMA_CH_NEEDED_FOR_" + remoteComponentID.upper() + "_Transmit"
            Database.sendMessage("core", "DMA_CHANNEL_ENABLE", {"dma_channel":dmaRequestID})
            dmaChannel = Database.getSymbolValue("core", dmaChannelID)
            
            dmaTxChannel.setVisible(True)
            if (dmaChannel == None) or (dmaChannel < 0) or (dmaChannel >= dmaChannelCount):
                # Error in DMA channel allocation
                localComponent.getSymbolByID("SRV_SPISPLIT_TX_DMA_CH_COMMENT").setVisible(True)
                dmaTxChannel.setValue(-1)
            else:
                dmaTxChannel.setValue(dmaChannel)

            # Request DMA channels for Receive
            dmaChannelID = "DMA_CH_FOR_" + remoteComponentID.upper() + "_Receive"
            dmaRequestID = "DMA_CH_NEEDED_FOR_" + remoteComponentID.upper() + "_Receive"
            Database.sendMessage("core", "DMA_CHANNEL_ENABLE", {"dma_channel":dmaRequestID})
            dmaChannel = Database.getSymbolValue("core", dmaChannelID)
            
            dmaRxChannel.setVisible(True)
            if (dmaChannel == None) or (dmaChannel < 0) or (dmaChannel >= dmaChannelCount):
                # Error in DMA channel allocation
                localComponent.getSymbolByID("SRV_SPISPLIT_RX_DMA_CH_COMMENT").setVisible(True)
                dmaRxChannel.setValue(-1)
            else:
                dmaRxChannel.setValue(dmaChannel)

        else:
            # Enable PDC DMA on PLIB
            plibUseSpiDma = remoteComponent.getSymbolByID("USE_SPI_DMA")
            if plibUseSpiDma != None:
                plibUseSpiDma.clearValue()
                plibUseSpiDma.setValue(True)
                plibUseSpiDma.setReadOnly(True)

        # Notify clients
        for clientID in clientList:
            if isDMAPresent:
                Database.sendMessage(clientID, "SPI_SPLITTER_CONNECTED", {"plibUsed":plibUsed.getValue(), "dmaTxChannel": dmaTxChannel.getValue(), "dmaRxChannel": dmaRxChannel.getValue()})
            else:
                Database.sendMessage(clientID, "SPI_SPLITTER_CONNECTED", {"plibUsed":plibUsed.getValue()})

    elif localConnectID == "srv_spiplit_SPI_capability":
        # Add to client list
        clientList.append(remoteComponentID)

        if plibUsed.getValue() != "":
            # Notify client
            if isDMAPresent:
                Database.sendMessage(remoteComponentID, "SPI_SPLITTER_CONNECTED", {"plibUsed":plibUsed.getValue(), "dmaTxChannel": dmaTxChannel.getValue(), "dmaRxChannel": dmaRxChannel.getValue()})
            else:
                Database.sendMessage(remoteComponentID, "SPI_SPLITTER_CONNECTED", {"plibUsed":plibUsed.getValue()})

def onAttachmentDisconnected(source, target):
    localComponent = source["component"]
    localConnectID = source["id"]
    remoteComponent = target["component"]
    remoteComponentID = remoteComponent.getID()

    if localConnectID == "srv_spiplit_SPI_dependency":
        # Clear read-only configurations in SPI PLIB
        Database.sendMessage(remoteComponentID, "SPI_MASTER_MODE", {"isReadOnly":False})
        Database.sendMessage(remoteComponentID, "SPI_MASTER_INTERRUPT_MODE", {"isReadOnly":False})

        # Clear PLIB used
        localComponent.getSymbolByID("SRV_SPISPLIT_PLIB").clearValue()
        localComponent.getSymbolByID("SRV_SPISPLIT_SPI_DEPENDENCY_COMMENT").setVisible(True)

        if isDMAPresent:
            # Hide DMA comments
            localComponent.getSymbolByID("SRV_SPISPLIT_TX_DMA_CHANNEL").setVisible(False)
            localComponent.getSymbolByID("SRV_SPISPLIT_RX_DMA_CHANNEL").setVisible(False)
            localComponent.getSymbolByID("SRV_SPISPLIT_TX_DMA_CH_COMMENT").setVisible(False)
            localComponent.getSymbolByID("SRV_SPISPLIT_RX_DMA_CH_COMMENT").setVisible(False)
            
            # Deactivate requested DMA channels for transmit and receive
            dmaChannelID = "DMA_CH_FOR_" + remoteComponentID.upper() + "_Transmit"
            dmaRequestID = "DMA_CH_NEEDED_FOR_" + remoteComponentID.upper() + "_Transmit"
            Database.sendMessage("core", "DMA_CHANNEL_DISABLE", {"dma_channel":dmaRequestID})

            dmaChannelID = "DMA_CH_FOR_" + remoteComponentID.upper() + "_Receive"
            dmaRequestID = "DMA_CH_NEEDED_FOR_" + remoteComponentID.upper() + "_Receive"
            Database.sendMessage("core", "DMA_CHANNEL_DISABLE", {"dma_channel":dmaRequestID})

        # Notify clients
        for clientID in clientList:
            result_dict = Database.sendMessage(clientID, "SPI_SPLITTER_DISCONNECTED", {})
        
        else:
            # Disable PDC DMA on PLIB
            plibUseSpiDma = remoteComponent.getSymbolByID("USE_SPI_DMA")
            if plibUseSpiDma != None:
                plibUseSpiDma.setReadOnly(False)
    elif localConnectID == "srv_spiplit_SPI_capability":
        # Remove from client list
        clientList.remove(remoteComponentID)

def handleMessage(messageID, args):
    result_dict = {}

    if (messageID == "REQUEST_CONFIG_PARAMS"):
        remoteComponentID = args.get("localComponentID")
        if remoteComponentID != None:
            # Set SPI in master mode
            result_dict = Database.sendMessage(remoteComponentID, "SPI_MASTER_MODE", {"isReadOnly":True, "isEnabled":True})
            # DMA Mode: Disable interrupt mode in PLIB
            # PDC Mode: Enable interrupt mode in PLIB (needed to enable PDC DMA)
            result_dict = Database.sendMessage(remoteComponentID, "SPI_MASTER_INTERRUPT_MODE", {"isReadOnly":True, "isEnabled":not isDMAPresent})

    return result_dict