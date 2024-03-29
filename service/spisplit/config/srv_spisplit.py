# coding: utf-8
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
import math

global clientList
clientList = []

srv_spisplit_helpkeyword = "srv_spisplitter_configurations"

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
    plibUsed.setHelp(srv_spisplit_helpkeyword)

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
        dmaTxChannel.setHelp(srv_spisplit_helpkeyword)

        dmaTxChannelComment = spiSplitterComponent.createCommentSymbol("SRV_SPISPLIT_TX_DMA_CH_COMMENT", None)
        dmaTxChannelComment.setLabel("!!! Couldn't Allocate DMA Channel for Transmit. Check DMA manager !!!")
        dmaTxChannelComment.setVisible(False)

        dmaRxChannel = spiSplitterComponent.createIntegerSymbol("SRV_SPISPLIT_RX_DMA_CHANNEL", None)
        dmaRxChannel.setLabel("DMA Channel For Receive")
        dmaRxChannel.setDescription("Allocated DMA channel for SPI Receive")
        dmaRxChannel.setMin(-1)
        dmaRxChannel.setReadOnly(True)
        dmaRxChannel.setVisible(False)
        dmaRxChannel.setHelp(srv_spisplit_helpkeyword)

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

        # Set symbols read-only
        Database.sendMessage(remoteComponentID, "SPI_MASTER_MODE", {"isReadOnly":True})

        remoteComponent = Database.getComponentByID(remoteComponentID)
        if not isDMAPresent and (remoteComponentID.startswith("flexcom") or remoteComponentID.startswith("spi")):
            plibUseSpiDma = remoteComponent.getSymbolByID("USE_SPI_DMA")
            if plibUseSpiDma != None:
                plibUseSpiDma.setReadOnly(True)

        Database.sendMessage(remoteComponentID, "SPI_MASTER_INTERRUPT_MODE", {"isReadOnly":True})

        if remoteComponentID.startswith("flexcom"):
            plibFIFO = remoteComponent.getSymbolByID("FLEXCOM_SPI_FIFO_ENABLE")
            if plibFIFO != None:
                plibFIFO.setReadOnly(True)
        elif remoteComponentID.startswith("sercom"):
            plibReceiver = remoteComponent.getSymbolByID("SPI_RECIEVER_ENABLE")
            if plibReceiver != None:
                plibReceiver.setReadOnly(True)

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
        localComponent.getSymbolByID("SRV_SPISPLIT_PLIB").clearValues()
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

        if not isDMAPresent and (remoteComponentID.startswith("flexcom") or remoteComponentID.startswith("spi")):
                # Disable read-only in PDC DMA
                plibUseSpiDma = remoteComponent.getSymbolByID("USE_SPI_DMA")
                if plibUseSpiDma != None:
                    plibUseSpiDma.setReadOnly(False)

        if remoteComponentID.startswith("flexcom"):
            # Disable read-only in FIFO
            plibFIFO = remoteComponent.getSymbolByID("FLEXCOM_SPI_FIFO_ENABLE")
            if plibUseSpiDma != None:
                plibFIFO.setReadOnly(False)

        elif remoteComponentID.startswith("sercom"):
            # Disable read-only in Enable receiver
            plibReceiver = remoteComponent.getSymbolByID("SPI_RECIEVER_ENABLE")
            if plibReceiver != None:
                plibReceiver.setReadOnly(False)

        # Notify clients
        for clientID in clientList:
            result_dict = Database.sendMessage(clientID, "SPI_SPLITTER_DISCONNECTED", {})

    elif localConnectID == "srv_spiplit_SPI_capability":
        # Remove from client list
        clientList.remove(remoteComponentID)

def handleMessage(messageID, args):
    result_dict = {}

    if (messageID == "REQUEST_CONFIG_PARAMS"):
        remoteComponentID = args.get("localComponentID")
        if remoteComponentID != None:
            # Set SPI in master mode
            result_dict = Database.sendMessage(remoteComponentID, "SPI_MASTER_MODE", {"isEnabled":True})
            # DMA Mode: Disable interrupt mode in PLIB
            # PDC Mode: Enable interrupt mode in PLIB (needed to enable PDC DMA)
            result_dict = Database.sendMessage(remoteComponentID, "SPI_MASTER_INTERRUPT_MODE", {"isEnabled":not isDMAPresent})

            remoteComponent = Database.getComponentByID(remoteComponentID)

            if not isDMAPresent and (remoteComponentID.startswith("flexcom") or remoteComponentID.startswith("spi")):
                # Enable PDC DMA on PLIB
                plibUseSpiDma = remoteComponent.getSymbolByID("USE_SPI_DMA")
                if plibUseSpiDma != None:
                    plibUseSpiDma.clearValues()
                    plibUseSpiDma.setValue(True)

            if remoteComponentID.startswith("flexcom"):
                # Disable FIFO
                plibFIFO = remoteComponent.getSymbolByID("FLEXCOM_SPI_FIFO_ENABLE")
                if plibFIFO != None:
                    plibFIFO.clearValues()
                    plibFIFO.setValue(False)

            elif remoteComponentID.startswith("sercom"):
                # Enable receiver
                plibReceiver = remoteComponent.getSymbolByID("SPI_RECIEVER_ENABLE")
                if plibReceiver != None:
                    plibReceiver.clearValues()
                    plibReceiver.setValue(True)

            # Set symbols read-only
            result_dict = Database.sendMessage(remoteComponentID, "SPI_MASTER_MODE", {"isReadOnly":True})

            if not isDMAPresent and (remoteComponentID.startswith("flexcom") or remoteComponentID.startswith("spi")):
                if plibUseSpiDma != None:
                    plibUseSpiDma.setReadOnly(True)

            result_dict = Database.sendMessage(remoteComponentID, "SPI_MASTER_INTERRUPT_MODE", {"isReadOnly":True})

            if remoteComponentID.startswith("flexcom"):
                if plibFIFO != None:
                    plibFIFO.setReadOnly(True)
            elif remoteComponentID.startswith("sercom"):
                if plibReceiver != None:
                    plibReceiver.setReadOnly(True)

    return result_dict