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

################################################################################
#### Component ####
################################################################################

global sort_alphanumeric

global plcSourceBinFileG3CENA
global plcSourceBinFileG3CENB
global plcSourceBinFileG3FCC
global plcSourceBinFilePRIME
global plcSourceBinFileARIB
global plcSourceBinFileMM
global plcAssemblyBinFile

global plcBandInUse

global plcStaticAddressing
global plcBinaryAddress
global plcBinarySize

plc_phy_helpkeyword = "mcc_h3_plc_phy_driver_configurations"
gPlcBand = ""

PLC_PROFILE_G3_CEN_A = 1
PLC_PROFILE_G3_FCC = 2
PLC_PROFILE_G3_ARIB = 3
PLC_PROFILE_G3_CEN_B = 4
PLC_PROFILE_PRIME = 5
PLC_PROFILE_G3_FCC_CEN_A = 6
PLC_PROFILE_G3_FCC_CEN_B = 7
PLC_PROFILE_G3_ARIB_CEN_A = 8
PLC_PROFILE_G3_ARIB_CEN_B = 9
PLC_PROFILE_MM_CEN_A = 10

# PL360 Bin reserved region: 0x20000 = 128KB
PLC_PHY_BIN_RESERVED_SIZE  = 131072

plcCoupPRIMECH = []
plcCoupPRIME2CH = []

def getFlashMemoryDescription():
    nodeIFLASH = ATDF.getNode("/avr-tools-device-file/devices/device/address-spaces/address-space/memory-segment@[type=\"flash\"]")
    if nodeIFLASH is not None:
        return (nodeIFLASH.getAttribute("start"), nodeIFLASH.getAttribute("size"))

    return (0, 0)

def configureSpiPlib(localComponent):
    global currentNPCS
    global spiNumNPCS

    plibUsed = localComponent.getSymbolByID("DRV_PLC_PLIB").getValue().lower()

    if plibUsed == '':
        return

    if plibUsed == "srv_spisplit":
        plibUsed = localComponent.getSymbolByID("DRV_PLC_PLIB_SPISPLIT").getValue().lower()

    remoteComponent = Database.getComponentByID(plibUsed)
    if remoteComponent == None:
        return

    if (spiNumNPCS > 0) and (plibUsed.startswith("flexcom") or plibUsed.startswith("spi")):
        if plibUsed.startswith("flexcom"):
            prefix = "FLEXCOM_SPI_"
        else:
            prefix = "SPI_"

        # Set NPCSx enabled
        spiSymbol = remoteComponent.getSymbolByID(prefix + "EN_NPCS" + str(currentNPCS))
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(True)
            spiSymbol.setReadOnly(True)

        # Set CSSAT to 0
        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_CSAAT")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(0)
            spiSymbol.setReadOnly(True)

        # Set CSNAAT to 0
        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_CSNAAT")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(0)
            spiSymbol.setReadOnly(True)

        # Set baudrate
        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_BAUD_RATE")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(8000000)

        # Set polarity to 0
        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_CPOL")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(0)
            spiSymbol.setReadOnly(True)

        # Set phase to 1
        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_NCPHA")
        if spiSymbol != None:
            spiSymbol.clearValues()
            if plibUsed.startswith("flexcom"):
                spiSymbol.setValue(1)
            else:
                spiSymbol.setValue(0)
            spiSymbol.setReadOnly(True)

        # Set DLYBS
        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_DLYBS")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(0)

        # Set DLYBCT
        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_DLYBCT")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(0)

    elif plibUsed.startswith("usart"):
        # Set polarity to 0
        spiSymbol = remoteComponent.getSymbolByID("USART_SPI_CLOCK_POLARITY")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(0)
            spiSymbol.setReadOnly(True)

        # Set phase to 1
        spiSymbol = remoteComponent.getSymbolByID("USART_SPI_CLOCK_PHASE")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(1)
            spiSymbol.setReadOnly(True)

        # Set baudrate
        spiSymbol = remoteComponent.getSymbolByID("USART_SPI_BAUD_RATE")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(8000000)

    elif plibUsed.startswith("sercom"):
        # Set polarity to 0
        spiSymbol = remoteComponent.getSymbolByID("SPI_CLOCK_POLARITY")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(0)
            spiSymbol.setReadOnly(True)

        # Set phase to 0
        spiSymbol = remoteComponent.getSymbolByID("SPI_CLOCK_PHASE")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(0)
            spiSymbol.setReadOnly(True)

        # Set baudrate
        spiSymbol = remoteComponent.getSymbolByID("SPI_BAUD_RATE")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(8000000)

    elif plibUsed.startswith("spi"):
        # Set baudrate
        spiSymbol = remoteComponent.getSymbolByID("SPI_BAUD_RATE")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(8000000)

def deconfigureSpiPlib(localComponent):
    global currentNPCS

    plibUsed = localComponent.getSymbolByID("DRV_PLC_PLIB").getValue().lower()

    if plibUsed == '':
        return

    if plibUsed == "srv_spisplit":
        plibUsed = localComponent.getSymbolByID("DRV_PLC_PLIB_SPISPLIT").getValue().lower()

    remoteComponent = Database.getComponentByID(plibUsed)
    if remoteComponent == None:
        return

    if (spiNumNPCS > 0) and (plibUsed.startswith("flexcom") or plibUsed.startswith("spi")):
        if plibUsed.startswith("flexcom"):
            prefix = "FLEXCOM_SPI_"
        else:
            prefix = "SPI_"

        # Disable read-only
        spiSymbol = remoteComponent.getSymbolByID(prefix + "EN_NPCS" + str(currentNPCS))
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(False)
            spiSymbol.setReadOnly(False)

        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_CSAAT")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_CSNAAT")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_CPOL")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_NCPHA")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

    elif plibUsed.startswith("usart"):
        # Disable read-only
        spiSymbol = remoteComponent.getSymbolByID("USART_SPI_CLOCK_POLARITY")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        spiSymbol = remoteComponent.getSymbolByID("USART_SPI_CLOCK_PHASE")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

    elif plibUsed.startswith("sercom"):
        # Disable read-only
        spiSymbol = remoteComponent.getSymbolByID("SPI_CLOCK_POLARITY")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        spiSymbol = remoteComponent.getSymbolByID("SPI_CLOCK_PHASE")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

def npcsChanged(symbol, event):
    global currentNPCS

    localComponent = event["source"]

    deconfigureSpiPlib(localComponent)
    currentNPCS = event["value"]
    configureSpiPlib(localComponent)

def getIndexFromPinId(pinId):
    availablePinDictionary = {}
    availablePinDictionary = Database.sendMessage("core", "PIN_LIST", availablePinDictionary)
    index = 0
    for pad in sort_alphanumeric(availablePinDictionary.values()):
        if pad == pinId:
            return index
        index += 1

    return None

def handleMessage(messageID, args):
    global plcStaticAddressing
    global plcBinaryAddress
    global plcBinarySize

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

    elif (messageID == "SPI_SPLITTER_CONNECTED"):
        spiPlib = args.get("plibUsed")
        plibUsedSpiSplit.setValue(spiPlib)
        plcDependencyDMAComment.setVisible(False)

        # Show/hide NPCS depending on SPI PLIB connected
        plibConfigComment.setVisible(True)
        if (spiNumNPCS > 0) and (spiPlib.startswith("FLEXCOM") or spiPlib.startswith("SPI")):
            # The SPI connected supports multiple NPCS/CSR
            spiNumCSR.setValue(spiNumNPCS)
            spiNpcsUsed.setVisible(True)
            plibConfigComment.setLabel("***Selected NPCS must be properly configured in the PLIB used***")
        else:
            spiNumCSR.setValue(0)
            plibConfigComment.setLabel("***The PLIB used must be properly configured***")
            if spiNumNPCS > 0:
                spiNpcsUsed.setVisible(False)

        # Configure SPI PLIB
        configureSpiPlib(plibConfigComment.getComponent())

        if (isDMAPresent == True):
            # Get DMA channel for Transmit
            plcTXDMAChannel.setVisible(True)
            dmaChannel = args.get("dmaTxChannel")
            if (dmaChannel == None) or (dmaChannel < 0) or (dmaChannel >= dmaChannelCount):
                # Error in DMA channel allocation
                plcTXDMAChannelComment.setVisible(True)
                plcTXDMAChannel.setValue(-1)
            else:
                plcTXDMAChannel.setValue(dmaChannel)

            # Get DMA channel for Receive
            dmaChannel = args.get("dmaRxChannel")
            plcRXDMAChannel.setVisible(True)
            if (dmaChannel == None) or (dmaChannel < 0) or (dmaChannel >= dmaChannelCount):
                # Error in DMA channel allocation
                plcRXDMAChannelComment.setVisible(True)
                plcRXDMAChannel.setValue(-1)
            else:
                plcRXDMAChannel.setValue(dmaChannel)

    elif (messageID == "SPI_SPLITTER_DISCONNECTED"):
        deconfigureSpiPlib(plibUsedSpiSplit.getComponent())
        plibUsedSpiSplit.clearValues()
        if spiNumNPCS > 0:
            spiNpcsUsed.setVisible(False)

        if isDMAPresent:
            # Hide DMA comments
            plcTXDMAChannel.setVisible(False)
            plcRXDMAChannel.setVisible(False)
            plcTXDMAChannelComment.setVisible(False)
            plcRXDMAChannelComment.setVisible(False)
            plcDependencyDMAComment.setVisible(True)

    elif (messageID == "DRVPLC_CONFIG_HW_IO"):
        global plcDriverMode
        
        result_dict = {"Result": "Fail"}
        signalId, pinId, functionValue, nameValue, enable = args['config']

        symbolName = None
        symbolValue = None

        if (functionValue.lower() == "gpio") or (signalId.lower() in ["int", "irq"]):
            plcDevice = nameValue.split("_")[-2].upper()
            if plcDevice == "PL360" or plcDevice =="PL460":
                currentValue = plcDriverMode.getValue()
                if currentValue != plcDevice:
                    plcDriverMode.setValue(plcDevice)
                
            pinDescr = nameValue.split("_")[-1].lower()
            if "enable" in pinDescr:
                symbolName = "DRV_PLC_LDO_EN_PIN"
            elif "extint" in pinDescr:
                symbolName = "DRV_PLC_EXT_INT_PIN"
            elif "nthw0" in pinDescr:
                symbolName = "DRV_PLC_THMON_PIN"
            elif "nrst" in pinDescr:
                symbolName = "DRV_PLC_RESET_PIN"
            elif "txen" in pinDescr:
                symbolName = "DRV_PLC_TX_ENABLE_PIN"
            elif "stby" in pinDescr:
                symbolName = "DRV_PLC_STBY_PIN"

            if symbolName != None:
                # Get index from pinId
                symbolValue = getIndexFromPinId(pinId)
                
        elif signalId.lower() == "cs":
            pinDescr = functionValue.split("_")[-1].lower()
            if "io3" in pinDescr:
                symbolName = "DRV_PLC_SPI_NPCS"
                symbolValue = 0 # NPCS0
            elif "io4" in pinDescr:
                symbolName = "DRV_PLC_SPI_NPCS"
                symbolValue = 1 # NPCS1
            elif "pad" in pinDescr:
                symbolName = "DRV_PLC_SPI_CS_PIN"
                symbolValue = getIndexFromPinId(pinId)

        if symbolValue != None:
            res = Database.setSymbolValue('drvPlcPhy', symbolName, symbolValue)
            if res == True:
                result_dict = {"Result": "Success"}
                if symbolName == "DRV_PLC_EXT_INT_PIN":
                    eic = Database.getSymbolValue("drvPlcPhy", "PLC_EIC_ID")
                    if (eic != "0") and (functionValue.split("_")[0].upper() == "EIC"):
                        intNum = int("".join(filter(lambda x: x.isdigit(), functionValue.split("_")[-1])))
                        res = Database.setSymbolValue('drvPlcPhy', "DRV_PLC_EIC_SIGNAL", "EIC_PIN_{}".format(intNum))
                        if res == True:
                            result_dict = {"Result": "Success"}
                        else:
                            result_dict = {"Result": "Fail"}

    elif (messageID == "SET_STATIC_ADDRESSING"):
        plcStaticAddressing.setValue(args["enable"])
        plcBinaryAddress.setValue(int(args["address"]))
        plcBinarySize.setValue(int(args["size"]))
        
    return result_dict

def sort_alphanumeric(l):
    import re
    convert = lambda text: int(text) if text.isdigit() else text.lower()
    alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ]
    return sorted(l, key = alphanum_key)

def plcExternalInterruptTrigger(symbol, event):
    key = event["symbol"].getKeyDescription(event["value"])
    intSrc = "PIO" + key[1] + "_IRQn"
    symbol.setValue(intSrc, 1)

def plcPhyExternalInterruptPort(symbol, event):
    key = event["symbol"].getKeyDescription(event["value"])
    intPort = "PIO_PORT_" + key[1]
    symbol.setValue(intPort)

def plcPhyEnableEncScript(symbol, event):
    if (event["value"] == True):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False)

def plcPhyVisibleEncComment(symbol, event):
    if (event["value"] == True):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def setPlcBandInUse(plcBand):
    dict = {}

    if (plcBand == "CEN-A"):
        plcBandInUse.setValue(PLC_PROFILE_G3_CEN_A)
        dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_G3_CENA", {})
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENA", {})
        dict = Database.sendMessage("srv_rsniffer", "SRV_RSNIFFER_G3", {})
    elif (plcBand == "CEN-B"):
        plcBandInUse.setValue(PLC_PROFILE_G3_CEN_B)
        dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_G3_CENB", {})
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENB", {})
        dict = Database.sendMessage("srv_rsniffer", "SRV_RSNIFFER_G3", {})
    elif (plcBand == "FCC"):
        plcBandInUse.setValue(PLC_PROFILE_G3_FCC)
        dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_G3_FCC", {})
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_FCC", {})
        dict = Database.sendMessage("srv_rsniffer", "SRV_RSNIFFER_G3", {})
    elif (plcBand == "ARIB"):
        plcBandInUse.setValue(PLC_PROFILE_G3_ARIB)
        dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_G3_ARIB", {})
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_ARIB", {})
        dict = Database.sendMessage("srv_rsniffer", "SRV_RSNIFFER_G3", {})
    elif (plcBand == "PRIME"):
        plcBandInUse.setValue(PLC_PROFILE_PRIME)
        dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_PRIME", {})
        dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_PRIME", {})
        dict = Database.sendMessage("srv_rsniffer", "SRV_RSNIFFER_PRIME", {})
    elif (plcBand == "MM"):
        plcBandInUse.setValue(PLC_PROFILE_MM_CEN_A)

def setPlcMultiBandInUse(g3_band, g3_aux_band):
    dict = {}

    if (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_G3_BAND_AUX_ACTIVE") == True) :
        g3_aux_band_default = True
    else:
        g3_aux_band_default = False

    if (g3_band == "FCC"):
        if (g3_aux_band == "CEN-A"):
            plcBandInUse.setValue(PLC_PROFILE_G3_FCC_CEN_A)
            if (g3_aux_band_default == True):
                dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_G3_CENA", {})
                dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENA", {})
        elif (g3_aux_band == "CEN-B"):
            plcBandInUse.setValue(PLC_PROFILE_G3_FCC_CEN_B)
            if (g3_aux_band_default == True):
                dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_G3_CENB", {})
                dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENB", {})
    else:
        if (g3_aux_band == "CEN-A"):
            plcBandInUse.setValue(PLC_PROFILE_G3_ARIB_CEN_A)
            if (g3_aux_band_default == True):
                dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_G3_CENA", {})
                dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENA", {})
        elif (g3_aux_band == "CEN-B"):
            plcBandInUse.setValue(PLC_PROFILE_G3_ARIB_CEN_B)
            if (g3_aux_band_default == True):
                dict = Database.sendMessage("srv_pserial", "SRV_PSERIAL_G3_CENB", {})
                dict = Database.sendMessage("srv_psniffer", "SRV_PSNIFFER_G3_CENB", {})

def removeAllBinFiles():
    plcSourceBinFilePRIME.setEnabled(False)
    plcSourceBinFileG3CENA.setEnabled(False)
    plcSourceBinFileG3CENB.setEnabled(False)
    plcSourceBinFileG3FCC.setEnabled(False)
    plcSourceBinFileG3ARIB.setEnabled(False)
    plcSourceBinFileMM.setEnabled(False)

def includeBinFile(plcBand):
    if (plcBand == "PRIME"):
        plcSourceBinFilePRIME.setEnabled(True)
        plcSourceBinFileG3CENA.setEnabled(False)
        plcSourceBinFileG3CENB.setEnabled(False)
        plcSourceBinFileG3FCC.setEnabled(False)
        plcSourceBinFileG3ARIB.setEnabled(False)
        plcSourceBinFileMM.setEnabled(False)
    elif (plcBand == "MM"):
        plcSourceBinFilePRIME.setEnabled(False)
        plcSourceBinFileG3CENA.setEnabled(False)
        plcSourceBinFileG3CENB.setEnabled(False)
        plcSourceBinFileG3FCC.setEnabled(False)
        plcSourceBinFileG3ARIB.setEnabled(False)
        plcSourceBinFileMM.setEnabled(True)
    else:
        # G3-PLC
        plcSourceBinFilePRIME.setEnabled(False)
        plcSourceBinFileMM.setEnabled(False)
        if (plcBand == "CEN-A"):
            plcSourceBinFileG3CENA.setEnabled(True)
        elif (plcBand == "CEN-B"):
            plcSourceBinFileG3CENB.setEnabled(True)
        elif (plcBand == "FCC"):
            plcSourceBinFileG3FCC.setEnabled(True)
        elif (plcBand == "ARIB"):
            plcSourceBinFileG3ARIB.setEnabled(True)

def updateBinFiles():
    dict = {}
    removeAllBinFiles()
    drvPlcProfile = Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PROFILE")
    if (drvPlcProfile == "G3-PLC") :
        g3_band = Database.getSymbolValue("drvPlcPhy", "DRV_PLC_G3_BAND")
        includeBinFile(g3_band)
        setPlcBandInUse(g3_band)
        if (g3_band == "FCC" or g3_band =="ARIB") and (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_COUP_G3_MULTIBAND") == True) :
            g3_aux_band = Database.getSymbolValue("drvPlcPhy", "DRV_PLC_G3_BAND_AUX")
            includeBinFile(g3_aux_band)
            setPlcMultiBandInUse(g3_band, g3_aux_band)
        dict = Database.sendMessage("srv_pcoup", "SRV_PCOUP_UPDATE_G3_PARAMETERS", {})
    elif (drvPlcProfile == "Meters&More") :
        includeBinFile("MM")
        setPlcBandInUse("MM")
        dict = Database.sendMessage("srv_pcoup", "SRV_PCOUP_UPDATE_MM_PARAMETERS", {})
    else:
        includeBinFile("PRIME")
        setPlcBandInUse("PRIME")
        dict = Database.sendMessage("srv_pcoup", "SRV_PCOUP_UPDATE_PRIME_PARAMETERS", {})

    # Check Internal/External Addressing
    if (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_BIN_STATIC_ADDRESSING") == False) :
        # Internal Addressing
        plcAssemblyBinFile.setEnabled(True)
    else:
        # External Addressing
        removeAllBinFiles()
        plcAssemblyBinFile.setEnabled(False)

def updateG3PLCBandInUse(symbol, event):
    updateBinFiles()

def plcShowAddressingMode(symbol, event):
    if (event["value"] == "PRIME"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)
    
def plcBinAddressingMode(symbol, event):
    symbol.setVisible(event["value"])
    updateBinFiles()

def setPlcProfile(symbol, event):
    global plcProfileFile
    global plcProfileDefFile
    global plcProfileHeaderLocalFile

    eventValue = event["value"]
    if (eventValue == "PRIME"):
        plcProfileFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_prime.c.ftl")
        plcProfileDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_prime.h.ftl")
        plcProfileHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local_prime.h")
    elif (eventValue == "G3-PLC"):
        plcProfileFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_g3.c.ftl")
        plcProfileDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_g3.h.ftl")
        plcProfileHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local_g3.h")
    else:
        # MM
        plcProfileFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_mm.c.ftl")
        plcProfileDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_mm.h.ftl")
        plcProfileHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local_mm.h")

def showPL460Pins(symbol, event):
    if (event["value"] == "PL460"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def showSleepPin(symbol, event):
    symbol.setVisible(event["value"])

def showThermalMonitorPin(symbol, event):
    symbol.setVisible(event["value"])

def showG3HighAttenuation(symbol, event):
    global plcDriverMode
    global plcG3Band

    if (plcDriverMode.getValue() == "PL360"):
        symbol.setVisible(False)
    else:
        if (plcG3Band.getValue() == "FCC") or (plcG3Band.getValue() == "ARIB"):
            symbol.setVisible(True)
        else:
            symbol.setVisible(False)

def showG3InternalDriver(symbol, event):
    global plcDriverMode

    if (event["symbol"].getValue() == "CEN-B") and plcDriverMode.getValue() == "PL360":
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)
        # symbol.setReadOnly(True)
        # symbol.setValue(False)
        # symbol.setReadOnly(False)

def showG3Multiband(symbol, event):
    if (event["symbol"].getValue() == "FCC") or (event["symbol"].getValue() == "ARIB"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)
        # symbol.setReadOnly(True)
        # symbol.setValue(False)
        # symbol.setReadOnly(False)

def showG3AuxBand(symbol, event):
    if (event["value"] == True):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def showChannelSelect(symbol, event):
    symbol.setVisible(event["value"])

def showPrime2ChannelSelect(symbol, event):
    if (event["value"] == "DOUBLE CHANNEL"):
        symbol.setVisible(True)
        plcCoupPRIME2Channel.setVisible(True)
    else:
        symbol.setVisible(False)
        plcCoupPRIME2Channel.setVisible(False)

def checkPrimeChannelConf(symbol, event):
    global plcDriverMode
    global plcCoupPRIMEDefChannel
    global plcCoupPRIMEHighAttenuation
    global plcCoupPRIMEBandAux

    channels_selected = 0
    if (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_1CHN_MODE") == True):
        channels_selected = channels_selected | Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_CH1")
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_CH2") << 1)
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_CH3") << 2)
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_CH4") << 3)
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_CH5") << 4)
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_CH6") << 5)
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_CH7") << 6)
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_CH8") << 7)

    if (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_2CHN_MODE") == True):
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_2CH1") << 8)
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_2CH2") << 9)
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_2CH3") << 10)
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_2CH4") << 11)
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_2CH5") << 12)
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_2CH6") << 13)
        channels_selected = channels_selected | (Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_2CH7") << 14)

    channel_value = plcCoupPRIMEDefChannel.getValue()
    if (channel_value.rfind("_") > 0):
        channel_default_idx = int(channel_value[3]) + 7
    else:
        channel_default_idx = int(channel_value[-1]) - 1

    # Set visibility of Warning Selection channel Comment
    if ((((1 << channel_default_idx) & channels_selected)) == 0):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

    # Set visibility of Auxiliary Band
    if (plcDriverMode.getValue() == "PL360"):
        plcCoupPRIMEBandAux.setVisible(False)
    else:
        if(channels_selected & 1):
            plcCoupPRIMEBandAux.setVisible(True)
        else:
            plcCoupPRIMEBandAux.setVisible(False)

    # Set visibility of High Attenuation
    if (plcDriverMode.getValue() == "PL360"):
        plcCoupPRIMEHighAttenuation.setVisible(False)
    else:
        if (channels_selected <= 1):
            plcCoupPRIMEHighAttenuation.setVisible(False)
        elif ((channels_selected & 1) and (channels_selected > 1) and Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_BAND_AUX") == False):
            plcCoupPRIMEHighAttenuation.setVisible(False)
        else:
            plcCoupPRIMEHighAttenuation.setVisible(True)

    # Update internal Channels Selected Symbol
    Database.setSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_CHANNELS_SELECTED", channels_selected)

    # Send message to PLC COUP to update PRIME configuration
    dict = {}
    dict = Database.sendMessage("srv_pcoup", "SRV_PCOUP_UPDATE_PRIME_PARAMETERS", {})

def checkPrime2ChannelConf(symbol, event):
    global plcCoupPRIME2Channel

    channel_def = int(plcCoupPRIME2Channel.getValue()[2])
    channel_sel = Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PRIME_2CH" + str(channel_def))

    if (channel_sel == False):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def updateShowCoupSettings(symbol, event):
    global plcCoupPRIMESettings
    global plcCoupG3Settings
    
    if (event["value"] == "G3-PLC"):
        plcCoupG3Settings.setVisible(True)
        plcCoupPRIMESettings.setVisible(False)
    elif (event["value"] == "PRIME"):
        plcCoupG3Settings.setVisible(False)
        plcCoupPRIMESettings.setVisible(True)
    else :
        plcCoupG3Settings.setVisible(False)
        plcCoupPRIMESettings.setVisible(False)

def resetPlcBand(symbol, event):
    symbol.setReadOnly(True)
    symbol.setValue("CEN-A")
    symbol.setReadOnly(False)

def enablePL460Capabilities(symbol, event):
    if (event["value"] == "PL460"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def showRTOSMenu(symbol, event):
    symbol.setVisible(event["value"] != "BareMetal")

def genRtosTask(symbol, event):
    symbol.setEnabled(event["value"] != "BareMetal")

def commandRtosMicriumOSIIIAppTaskVisibility(symbol, event):
    symbol.setVisible(event["value"] == "MicriumOSIII")

def commandRtosMicriumOSIIITaskOptVisibility(symbol, event):
    symbol.setVisible(event["value"])

def getActiveRtos():
    return Database.getSymbolValue("HarmonyCore", "SELECT_RTOS")

def identifyPeripherals(component):
    periphNode = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals")
    peripherals = periphNode.getChildren()

    flexcomIdCreated = False
    adcIdCreated = False
    pioIdCreated = False
    eicIdCreated = False

    for module in range (0, len(peripherals)):
        if str(peripherals[module].getAttribute("name")) == "FLEXCOM":
            plcFlexcomId = component.createStringSymbol("PLC_FLEXCOM_ID", None)
            plcFlexcomId.setDefaultValue(str(peripherals[module].getAttribute("id")))
            flexcomIdCreated = True
        elif str(peripherals[module].getAttribute("name")) == "ADC":
            plcAdcId = component.createStringSymbol("PLC_ADC_ID", None)
            plcAdcId.setDefaultValue(str(peripherals[module].getAttribute("id")))
            adcIdCreated = True
        elif str(peripherals[module].getAttribute("name")) == "PIO":
            plcPioId = component.createStringSymbol("PLC_PIO_ID", None)
            plcPioId.setDefaultValue(str(peripherals[module].getAttribute("id")))
            pioIdCreated = True
        elif str(peripherals[module].getAttribute("name")) == "EIC":
            plcEicId = component.createStringSymbol("PLC_EIC_ID", None)
            plcEicId.setDefaultValue(str(peripherals[module].getAttribute("id")))
            eicIdCreated = True

    if not flexcomIdCreated:
        plcFlexcomId = component.createStringSymbol("PLC_FLEXCOM_ID", None)
        plcFlexcomId.setDefaultValue("0")
    if not adcIdCreated:
        plcAdcId = component.createStringSymbol("PLC_ADC_ID", None)
        plcAdcId.setDefaultValue("0")
    if not pioIdCreated:
        plcPioId = component.createStringSymbol("PLC_PIO_ID", None)
        plcPioId.setDefaultValue("0")
    if not eicIdCreated:
        plcEicId = component.createStringSymbol("PLC_EIC_ID", None)
        plcEicId.setDefaultValue("0")

    plcFlexcomId.setVisible(False)
    plcAdcId.setVisible(False)
    plcPioId.setVisible(False)
    plcEicId.setVisible(False)

def getEICSignals():
    eicSignalsList = []
    for signal in eicSignalsATDF:
        index = signal.getAttribute("index")
        if (index != None):
            eicPin = "EIC_PIN_{}".format(index)
            if eicPin not in eicSignalsList:
                eicSignalsList.append(eicPin)

    if (eicSignalsList == []):
        eicSignalsList = ["None"]

    return eicSignalsList

def getEICSignalsFromPin(pin):
    eicList = []
    eicPad = "{}{:02d}".format(pin.replace("R","P")[:2],int(pin[2:]))
    for signal in eicSignalsATDF:
        pad = signal.getAttribute("pad")
        index = signal.getAttribute("index")
        if ((pad in eicPad) and (index!=None)):
            eicList.append("EIC_PIN_" + index)

    return eicList

def checkEICSignals(symbol, event):
    index = plcExtIntPin.getValue()
    pinDesc = plcExtIntPin.getKeyDescription(index)
    eic = plcEICSignal.getValue()

    eicSignalsFromPin = getEICSignalsFromPin(pinDesc)

    if eic in eicSignalsFromPin:
        symbol.setVisible(False)
    else:
        symbol.setVisible(True)

def showSymbol(symbol, event):
    symbol.setVisible(event["value"])

def instantiateComponent(plcComponent):
    global isDMAPresent

    res = Database.activateComponents(["HarmonyCore"])

    # Enable "Generate Harmony Driver Common Files" option in MHC
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":True})

    # Enable "Generate Harmony System Service Common Files" option in MHC
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":True})

    # Enable "Generate Harmony System Port Files" option in MHC
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_PORTS", {"isEnabled":True})

    # Enable "Generate Harmony System DMA Files" option in MHC
    if Database.getSymbolValue("core", "DMA_ENABLE") != None:
        Database.sendMessage("HarmonyCore", "ENABLE_SYS_DMA", {"isEnabled":True})

    if Database.getSymbolValue("core", "DMA_ENABLE") == None:
        isDMAPresent = False
    else:
        isDMAPresent = True

    identifyPeripherals(plcComponent)

    global plcDriverMode
    plcDriverMode = plcComponent.createComboSymbol("DRV_PLC_MODE", None, ["PL360", "PL460"])
    plcDriverMode.setLabel("PLC Driver Mode")
    plcDriverMode.setDefaultValue("PL460")
    plcDriverMode.setHelp(plc_phy_helpkeyword)

    # RTOS CONFIG
    plcRTOSMenu = plcComponent.createMenuSymbol("DRV_PLC_RTOS_MENU", None)
    plcRTOSMenu.setLabel("RTOS settings")
    plcRTOSMenu.setDescription("RTOS settings")
    plcRTOSMenu.setVisible(getActiveRtos() != "BareMetal")
    plcRTOSMenu.setDependencies(showRTOSMenu, ["HarmonyCore.SELECT_RTOS"])
    plcRTOSMenu.setHelp(plc_phy_helpkeyword)

    plcRTOSStackSize = plcComponent.createIntegerSymbol("DRV_PLC_RTOS_STACK_SIZE", plcRTOSMenu)
    plcRTOSStackSize.setLabel("Stack Size (in bytes)")
    plcRTOSStackSize.setDefaultValue(1024)
    plcRTOSStackSize.setMin(256)
    plcRTOSStackSize.setMax(16*1024)
    plcRTOSStackSize.setHelp(plc_phy_helpkeyword)

    plcRTOSTaskPriority = plcComponent.createIntegerSymbol("DRV_PLC_RTOS_TASK_PRIORITY", plcRTOSMenu)
    plcRTOSTaskPriority.setLabel("Task Priority")
    plcRTOSTaskPriority.setDefaultValue(1)
    plcRTOSTaskPriority.setMin(0)
    plcRTOSTaskPriority.setHelp(plc_phy_helpkeyword)

    plcRTOSMsgQSize = plcComponent.createIntegerSymbol("DRV_PLC_RTOS_TASK_MSG_QTY", plcRTOSMenu)
    plcRTOSMsgQSize.setLabel("Maximum Message Queue Size")
    plcRTOSMsgQSize.setDescription("A µC/OS-III task contains an optional internal message queue (if OS_CFG_TASK_Q_EN is set to DEF_ENABLED in os_cfg.h). This argument specifies the maximum number of messages that the task can receive through this message queue. The user may specify that the task is unable to receive messages by setting this argument to 0")
    plcRTOSMsgQSize.setDefaultValue(0)
    plcRTOSMsgQSize.setMin(0)
    plcRTOSMsgQSize.setVisible(getActiveRtos() == "MicriumOSIII")
    plcRTOSMsgQSize.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])
    plcRTOSMsgQSize.setHelp(plc_phy_helpkeyword)

    plcRTOSTaskTimeQuanta = plcComponent.createIntegerSymbol("DRV_PLC_RTOS_TASK_TIME_QUANTA", plcRTOSMenu)
    plcRTOSTaskTimeQuanta.setLabel("Task Time Quanta")
    plcRTOSTaskTimeQuanta.setDescription("The amount of time (in clock ticks) for the time quanta when Round Robin is enabled. If you specify 0, then the default time quanta will be used which is the tick rate divided by 10.")
    plcRTOSTaskTimeQuanta.setDefaultValue(0)
    plcRTOSTaskTimeQuanta.setMin(0)
    plcRTOSTaskTimeQuanta.setVisible(getActiveRtos() == "MicriumOSIII")
    plcRTOSTaskTimeQuanta.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])
    plcRTOSTaskTimeQuanta.setHelp(plc_phy_helpkeyword)

    plcRTOSTaskSpecificOpt = plcComponent.createBooleanSymbol("DRV_PLC_RTOS_TASK_OPT_NONE", plcRTOSMenu)
    plcRTOSTaskSpecificOpt.setLabel("Task Specific Options")
    plcRTOSTaskSpecificOpt.setDescription("Contains task-specific options. Each option consists of one bit. The option is selected when the bit is set. The current version of µC/OS-III supports the following options:")
    plcRTOSTaskSpecificOpt.setDefaultValue(True)
    plcRTOSTaskSpecificOpt.setVisible(getActiveRtos() == "MicriumOSIII")
    plcRTOSTaskSpecificOpt.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])
    plcRTOSTaskSpecificOpt.setHelp(plc_phy_helpkeyword)

    plcRTOSTaskStkChk = plcComponent.createBooleanSymbol("DRV_PLC_RTOS_TASK_OPT_STK_CHK", plcRTOSTaskSpecificOpt)
    plcRTOSTaskStkChk.setLabel("Stack checking is allowed for the task")
    plcRTOSTaskStkChk.setDescription("Specifies whether stack checking is allowed for the task")
    plcRTOSTaskStkChk.setDefaultValue(True)
    plcRTOSTaskStkChk.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_PLC_RTOS_TASK_OPT_NONE"])
    plcRTOSTaskStkChk.setHelp(plc_phy_helpkeyword)

    plcRTOSTaskStkClr = plcComponent.createBooleanSymbol("DRV_PLC_RTOS_TASK_OPT_STK_CLR", plcRTOSTaskSpecificOpt)
    plcRTOSTaskStkClr.setLabel("Stack needs to be cleared")
    plcRTOSTaskStkClr.setDescription("Specifies whether the stack needs to be cleared")
    plcRTOSTaskStkClr.setDefaultValue(True)
    plcRTOSTaskStkClr.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_PLC_RTOS_TASK_OPT_NONE"])
    plcRTOSTaskStkClr.setHelp(plc_phy_helpkeyword)

    plcRTOSTaskSaveFp = plcComponent.createBooleanSymbol("DRV_PLC_RTOS_TASK_OPT_SAVE_FP", plcRTOSTaskSpecificOpt)
    plcRTOSTaskSaveFp.setLabel("Floating-point registers needs to be saved")
    plcRTOSTaskSaveFp.setDescription("Specifies whether floating-point registers are saved. This option is only valid if the processor has floating-point hardware and the processor-specific code saves the floating-point registers")
    plcRTOSTaskSaveFp.setDefaultValue(False)
    plcRTOSTaskSaveFp.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_PLC_RTOS_TASK_OPT_NONE"])
    plcRTOSTaskSaveFp.setHelp(plc_phy_helpkeyword)

    plcRTOSTaskNoTls = plcComponent.createBooleanSymbol("DRV_PLC_RTOS_TASK_OPT_NO_TLS", plcRTOSTaskSpecificOpt)
    plcRTOSTaskNoTls.setLabel("TLS (Thread Local Storage) support needed for the task")
    plcRTOSTaskNoTls.setDescription("If the caller doesn’t want or need TLS (Thread Local Storage) support for the task being created. If you do not include this option, TLS will be supported by default. TLS support was added in V3.03.00")
    plcRTOSTaskNoTls.setDefaultValue(False)
    plcRTOSTaskNoTls.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_PLC_RTOS_TASK_OPT_NONE"])
    plcRTOSTaskNoTls.setHelp(plc_phy_helpkeyword)

    plcPLIB = plcComponent.createStringSymbol("DRV_PLC_PLIB", None)
    plcPLIB.setLabel("PLIB Used")
    plcPLIB.setReadOnly(True)
    plcPLIB.setHelp(plc_phy_helpkeyword)

    global plibUsedSpiSplit
    plibUsedSpiSplit = plcComponent.createStringSymbol("DRV_PLC_PLIB_SPISPLIT", None)
    plibUsedSpiSplit.setLabel("PLIB Used by SPI Splitter")
    plibUsedSpiSplit.setDescription("PLIB connected to SPI dependency in SPI Splitter")
    plibUsedSpiSplit.setReadOnly(True)
    plibUsedSpiSplit.setVisible(False)
    plibUsedSpiSplit.setHelp(plc_phy_helpkeyword)

    # Get number of CSR registers (number of NPCS pins) in SPI/FLEXCOM peripheral
    global spiNumNPCS
    spiNumNPCS = 0
    spiCsrNode = ATDF.getNode("/avr-tools-device-file/modules/module@[name=\"SPI\"]/register-group/register@[name=\"SPI_CSR\"]")
    if spiCsrNode != None:
        spiNumNPCS = int(spiCsrNode.getAttribute('count'),10)
    else:
        flexcomSpiCsrNode = ATDF.getNode("/avr-tools-device-file/modules/module@[name=\"FLEXCOM\"]/register-group/register@[name=\"FLEX_SPI_CSR\"]")
        if flexcomSpiCsrNode != None:
            spiNumNPCS = int(flexcomSpiCsrNode.getAttribute('count'),10)

    # If 0, the SPI peripheral doesn't support multiple NPCS/CSR
    global spiNpcsUsed
    global currentNPCS
    if spiNumNPCS > 0:
        spiNpcsUsed = plcComponent.createKeyValueSetSymbol("DRV_PLC_SPI_NPCS", None)
        spiNpcsUsed.setLabel("SPI NPCS Used")
        spiNpcsUsed.setDescription("Select SPI NPCS (peripheral chip select) connected to PLC Device")
        spiNpcsUsed.setOutputMode("Value")
        spiNpcsUsed.setDisplayMode("Key")
        spiNpcsUsed.setDefaultValue(0)
        spiNpcsUsed.setVisible(False)
        spiNpcsUsed.setHelp(plc_phy_helpkeyword)

        currentNPCS = 0

        for npcs in range(0, spiNumNPCS):
            spiNpcsUsed.addKey("NPCS" + str(npcs), str(npcs), "SPI NPCS" + str(npcs) + " used by RF215 Driver")

    global spiNumCSR
    spiNumCSR = plcComponent.createIntegerSymbol("DRV_PLC_SPI_NUM_CSR", None)
    spiNumCSR.setVisible(False)
    spiNumCSR.setDependencies(npcsChanged, ["DRV_PLC_SPI_NPCS"])

    global plibConfigComment
    plibConfigComment = plcComponent.createCommentSymbol("DRV_PLC_PLIB_CONFIG_COMMENT", None)
    plibConfigComment.setVisible(False)

    global plcExtIntPin
    plcExtIntPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_EXT_INT_PIN", None)
    plcExtIntPin.setLabel("External Interrupt Pin")
    plcExtIntPin.setDefaultValue(0)
    plcExtIntPin.setOutputMode("Key")
    plcExtIntPin.setDisplayMode("Description")
    plcExtIntPin.setHelp(plc_phy_helpkeyword)

    plcExtIntSource = plcComponent.createStringSymbol("DRV_PLC_EXT_INT_SRC", None)
    plcExtIntSource.setLabel("External Interrupt Source")
    plcExtIntSource.setDefaultValue("PIOA_IRQn")
    plcExtIntSource.setVisible(False)
    plcExtIntSource.setReadOnly(True)
    plcExtIntSource.setHelp(plc_phy_helpkeyword)
    plcExtIntSource.setDependencies(plcExternalInterruptTrigger, ["DRV_PLC_EXT_INT_PIN"])

    plcPhyExtIntPioPort = plcComponent.createStringSymbol("DRV_PLC_EXT_INT_PIO_PORT", None)
    plcPhyExtIntPioPort.setLabel("External Interrupt Port")
    plcPhyExtIntPioPort.setDefaultValue("PIO_PORT_A")
    plcPhyExtIntPioPort.setVisible(False)
    plcPhyExtIntPioPort.setReadOnly(True)
    plcPhyExtIntPioPort.setHelp(plc_phy_helpkeyword)
    plcPhyExtIntPioPort.setDependencies(plcPhyExternalInterruptPort, ["DRV_PLC_EXT_INT_PIN"])

    plcResetPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_RESET_PIN", None)
    plcResetPin.setLabel("Reset Pin")
    plcResetPin.setDefaultValue(0)
    plcResetPin.setOutputMode("Key")
    plcResetPin.setHelp(plc_phy_helpkeyword)
    plcResetPin.setDisplayMode("Description")

    plcLDOEnPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_LDO_EN_PIN", None)
    plcLDOEnPin.setLabel("LDO Enable Pin")
    plcLDOEnPin.setDefaultValue(0)
    plcLDOEnPin.setOutputMode("Key")
    plcLDOEnPin.setHelp(plc_phy_helpkeyword)
    plcLDOEnPin.setDisplayMode("Description")

    plcTxEnablePin = plcComponent.createKeyValueSetSymbol("DRV_PLC_TX_ENABLE_PIN", None)
    plcTxEnablePin.setLabel("TX Enable Pin")
    plcTxEnablePin.setDefaultValue(0)
    plcTxEnablePin.setOutputMode("Key")
    plcTxEnablePin.setDisplayMode("Description")
    plcTxEnablePin.setHelp(plc_phy_helpkeyword)
    plcTxEnablePin.setDependencies(enablePL460Capabilities, ["DRV_PLC_MODE"]);

    plcSleepMode = plcComponent.createBooleanSymbol("DRV_PLC_SLEEP_MODE", None)
    plcSleepMode.setLabel("Sleep Mode")
    plcSleepMode.setDefaultValue(False)
    plcSleepMode.setHelp(plc_phy_helpkeyword)
    plcSleepMode.setVisible(True)

    plcStbyPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_STBY_PIN", plcSleepMode)
    plcStbyPin.setLabel("Stand By Pin")
    plcStbyPin.setDefaultValue(0)
    plcStbyPin.setOutputMode("Key")
    plcStbyPin.setDisplayMode("Description")
    plcStbyPin.setVisible(False)
    plcStbyPin.setHelp(plc_phy_helpkeyword)
    plcStbyPin.setDependencies(showSleepPin, ["DRV_PLC_SLEEP_MODE"])

    plcThermalMonitor = plcComponent.createBooleanSymbol("DRV_PLC_THERMAL_MONITOR", None)
    plcThermalMonitor.setLabel("Thermal Monitor")
    plcThermalMonitor.setDefaultValue(False)
    plcThermalMonitor.setHelp(plc_phy_helpkeyword)
    plcThermalMonitor.setDependencies(enablePL460Capabilities, ["DRV_PLC_MODE"]);

    plcThMonPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_THMON_PIN", plcThermalMonitor)
    plcThMonPin.setLabel("Thermal Monitor Pin")
    plcThMonPin.setDefaultValue(0)
    plcThMonPin.setOutputMode("Key")
    plcThMonPin.setDisplayMode("Description")
    plcThMonPin.setVisible(False)
    plcThMonPin.setHelp(plc_phy_helpkeyword)
    plcThMonPin.setDependencies(showThermalMonitorPin, ["DRV_PLC_THERMAL_MONITOR"])

    plcSPICSPin = plcComponent.createKeyValueSetSymbol("DRV_PLC_SPI_CS_PIN", None)  # Only used with SERCOM
    plcSPICSPin.setLabel("SPI Chip Select Pin")
    plcSPICSPin.setDefaultValue(0)
    plcSPICSPin.setOutputMode("Key")
    plcSPICSPin.setHelp(plc_phy_helpkeyword)
    plcSPICSPin.setDisplayMode("Description")
    plcSPICSPin.setVisible(False)

    availablePinDictionary = {}

    eic = Database.getSymbolValue("drvPlcPhy", "PLC_EIC_ID")

    # Send message to core to get available pins
    availablePinDictionary = Database.sendMessage("core", "PIN_LIST", availablePinDictionary)

    for pad in sort_alphanumeric(availablePinDictionary.values()):
        key = "SYS_PORT_PIN_" + pad
        value = list(availablePinDictionary.keys())[list(availablePinDictionary.values()).index(pad)]
        description = pad
        plcResetPin.addKey(key, value, description)
        plcLDOEnPin.addKey(key, value, description)
        plcTxEnablePin.addKey(key, value, description)
        plcStbyPin.addKey(key, value, description)
        plcThMonPin.addKey(key, value, description)
        plcExtIntPin.addKey(key, value, description)
        if eic != "0":
            plcSPICSPin.addKey(key, value, description)
            plcSPICSPin.setVisible(True) # Set SPI CS as Visible

    plcSymPinConfigComment = plcComponent.createCommentSymbol("DRV_PLC_PINS_CONFIG_COMMENT", None)
    plcSymPinConfigComment.setVisible(True)
    plcSymPinConfigComment.setLabel("***Above selected pins must be properly configured by Pin Manager***")

    if (eic != "0"):
        global eicSignalsATDF
        eicSignalsATDF = ATDF.getNode('/avr-tools-device-file/devices/device/peripherals/module@[name="EIC"]/instance/signals').getChildren()

        global plcEICSignal
        eicSignals = getEICSignals()
        plcEICSignal = plcComponent.createComboSymbol("DRV_PLC_EIC_SIGNAL", plcExtIntPin, eicSignals)
        plcEICSignal.setLabel("EIC signal")
        plcEICSignal.setDefaultValue(eicSignals[0])

        plcEICSignalComment = plcComponent.createCommentSymbol("DRV_PLC_EIC_SIGNAL_COMMENT", plcExtIntPin)
        plcEICSignalComment.setVisible(False)
        plcEICSignalComment.setLabel("***Selected EIC signal cannot be assigned to PIN value. Please check it with the PIN Manager***")
        plcEICSignalComment.setDependencies(checkEICSignals, ["DRV_PLC_EXT_INT_PIN" ,"DRV_PLC_EIC_SIGNAL"])

    ##### Do not modify below symbol names as they are used by Memory Driver #####

    global dmaChannelCount
    global plcTXDMAChannel
    global plcTXDMAChannelComment
    global plcRXDMAChannel
    global plcRXDMAChannelComment
    if isDMAPresent:
        plcTXDMAChannel = plcComponent.createIntegerSymbol("DRV_PLC_TX_DMA_CHANNEL", None)
        plcTXDMAChannel.setLabel("DMA Channel For Transmit")
        plcTXDMAChannel.setDescription("Allocated DMA channel for SPI Transmit")
        plcTXDMAChannel.setDefaultValue(0)
        plcTXDMAChannel.setMin(-1)
        plcTXDMAChannel.setVisible(False)
        plcTXDMAChannel.setReadOnly(True)

        plcTXDMAChannelComment = plcComponent.createCommentSymbol("DRV_PLC_TX_DMA_CH_COMMENT", None)
        plcTXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Transmit. Check DMA manager.")
        plcTXDMAChannelComment.setVisible(False)

        plcRXDMAChannel = plcComponent.createIntegerSymbol("DRV_PLC_RX_DMA_CHANNEL", None)
        plcRXDMAChannel.setLabel("DMA Channel For Receive")
        plcRXDMAChannel.setDescription("Allocated DMA channel for SPI Receive")
        plcRXDMAChannel.setDefaultValue(1)
        plcRXDMAChannel.setMin(-1)
        plcRXDMAChannel.setVisible(False)
        plcRXDMAChannel.setReadOnly(True)

        plcRXDMAChannelComment = plcComponent.createCommentSymbol("DRV_PLC_RX_DMA_CH_COMMENT", None)
        plcRXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Receive. Check DMA manager.")
        plcRXDMAChannelComment.setVisible(False)

        # Get number of DMA channels
        dmaChannelCount = Database.getSymbolValue("core", "DMA_CHANNEL_COUNT")
        if dmaChannelCount != None:
            plcTXDMAChannel.setMax(dmaChannelCount - 1)
            plcRXDMAChannel.setMax(dmaChannelCount - 1)

    global plcDependencyDMAComment
    plcDependencyDMAComment = plcComponent.createCommentSymbol("DRV_PLC_DEPENDENCY_DMA_COMMENT", None)
    plcDependencyDMAComment.setLabel("!!! Satisfy PLIB Dependency to Allocate DMA Channel !!!")
    plcDependencyDMAComment.setVisible(isDMAPresent)

    ############################################################################
    #### Code Generation ####
    ############################################################################

    configName = Variables.get("__CONFIGURATION_NAME")

    #### Common Files ##########################################################

    plcPhyHalHeaderFile = plcComponent.createFileSymbol("PLC_HAL_HEADER", None)
    plcPhyHalHeaderFile.setSourcePath("driver/common/plcHal/drv_plc_hal.h.ftl")
    plcPhyHalHeaderFile.setOutputName("drv_plc_hal.h")
    plcPhyHalHeaderFile.setDestPath("driver/plc/common")
    plcPhyHalHeaderFile.setProjectPath("config/" + configName + "/driver/plc/common")
    plcPhyHalHeaderFile.setType("HEADER")
    plcPhyHalHeaderFile.setMarkup(True)

    plcPhyHalFile = plcComponent.createFileSymbol("PLC_HAL", None)
    plcPhyHalFile.setSourcePath("driver/common/plcHal/drv_plc_hal.c.ftl")
    plcPhyHalFile.setOutputName("drv_plc_hal.c")
    plcPhyHalFile.setDestPath("driver/plc/common")
    plcPhyHalFile.setProjectPath("config/" + configName + "/driver/plc/common")
    plcPhyHalFile.setType("SOURCE")
    plcPhyHalFile.setMarkup(True)

    plcPhyHALInitDataFile = plcComponent.createFileSymbol("DRV_HAL_INIT_DATA", None)
    plcPhyHALInitDataFile.setType("STRING")
    plcPhyHALInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    plcPhyHALInitDataFile.setSourcePath("driver/common/plcHal/drv_plc_hal_initialization_data.c.ftl")
    plcPhyHALInitDataFile.setMarkup(True)

    plcPhyBootFile = plcComponent.createFileSymbol("PLC_BOOT", None)
    plcPhyBootFile.setSourcePath("driver/common/plcBoot/drv_plc_boot.c.ftl")
    plcPhyBootFile.setOutputName("drv_plc_boot.c")
    plcPhyBootFile.setDestPath("driver/plc/common")
    plcPhyBootFile.setProjectPath("config/" + configName + "/driver/plc/common")
    plcPhyBootFile.setType("SOURCE")
    plcPhyBootFile.setMarkup(True)

    plcPhyBootHeaderFile = plcComponent.createFileSymbol("PLC_BOOT_HEADER", None)
    plcPhyBootHeaderFile.setSourcePath("driver/common/plcBoot/drv_plc_boot.h")
    plcPhyBootHeaderFile.setOutputName("drv_plc_boot.h")
    plcPhyBootHeaderFile.setDestPath("driver/plc/common")
    plcPhyBootHeaderFile.setProjectPath("config/" + configName + "/driver/plc/common")
    plcPhyBootHeaderFile.setType("HEADER")

    #### ASM Path Settings #####################################################

    plcAsmPathSetting = plcComponent.createSettingSymbol("DRV_PLC_ASM_PATH_SETTING", None)
    plcAsmPathSetting.setCategory("C32-AS")
    plcAsmPathSetting.setKey("extra-include-directories-for-assembler")
    plcAsmPathSetting.setValue("../src/config/" + configName + "/driver/plc/phy/bin")
    plcAsmPathSetting.setAppend(True, ";")

    #### Miscellanea ###########################################################

    plcPhySecureScript = plcComponent.createFileSymbol("DRV_PLC_PHY_SECURE_SCRIPT", None)
    plcPhySecureScript.setSourcePath("driver/plcPhy/src/bin/plcPhy_encfile.py")
    plcPhySecureScript.setOutputName("plcPhy_encfile.py")
    plcPhySecureScript.setDestPath("driver/plc/phy/bin/")
    plcPhySecureScript.setProjectPath("config/" + configName + "/driver/plc/phy/bin/")
    plcPhySecureScript.setType("SOURCE")
    plcPhySecureScript.setEnabled(False)
    plcPhySecureScript.setDependencies(plcPhyEnableEncScript, ["DRV_PLC_SECURE_MODE"])

    #### Source and header PLC PHY Files #######################################

    plcHeaderFile = plcComponent.createFileSymbol("PLC_HEADER", None)
    plcHeaderFile.setSourcePath("driver/plcPhy/drv_plc_phy.h.ftl")
    plcHeaderFile.setOutputName("drv_plc_phy.h")
    plcHeaderFile.setDestPath("driver/plc/phy")
    plcHeaderFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcHeaderFile.setType("HEADER")
    plcHeaderFile.setMarkup(True)

    plcSymHeaderDefFile = plcComponent.createFileSymbol("DRV_PLC_DEF", None)
    plcSymHeaderDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_definitions.h")
    plcSymHeaderDefFile.setOutputName("drv_plc_phy_definitions.h")
    plcSymHeaderDefFile.setDestPath("driver/plc/phy")
    plcSymHeaderDefFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcSymHeaderDefFile.setType("HEADER")

    plcHeaderLocalFile = plcComponent.createFileSymbol("PLC_HEADER_LOCAL", None)
    plcHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local.h.ftl")
    plcHeaderLocalFile.setOutputName("drv_plc_phy_local.h")
    plcHeaderLocalFile.setDestPath("driver/plc/phy")
    plcHeaderLocalFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcHeaderLocalFile.setType("HEADER")
    plcHeaderLocalFile.setMarkup(True)

    global plcProfileFile
    plcProfileFile = plcComponent.createFileSymbol("DRV_PLC_PROF_FILE", None)
    plcProfileFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_g3.c.ftl")
    plcProfileFile.setOutputName("drv_plc_phy_comm.c")
    plcProfileFile.setDestPath("driver/plc/phy")
    plcProfileFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcProfileFile.setType("SOURCE")
    plcProfileFile.setDependencies(setPlcProfile, ["DRV_PLC_PROFILE"])
    plcProfileFile.setMarkup(True)

    global plcProfileDefFile
    plcProfileDefFile = plcComponent.createFileSymbol("DRV_PLC_PROFILE_DEF", None)
    plcProfileDefFile.setSourcePath("driver/plcPhy/drv_plc_phy_g3.h.ftl")
    plcProfileDefFile.setOutputName("drv_plc_phy_comm.h")
    plcProfileDefFile.setDestPath("driver/plc/phy")
    plcProfileDefFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcProfileDefFile.setType("HEADER")
    plcProfileDefFile.setMarkup(True)

    plcSourceFile = plcComponent.createFileSymbol("PLC_SOURCE", None)
    plcSourceFile.setSourcePath("driver/plcPhy/src/drv_plc_phy.c.ftl")
    plcSourceFile.setOutputName("drv_plc_phy.c")
    plcSourceFile.setDestPath("driver/plc/phy")
    plcSourceFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcSourceFile.setType("SOURCE")
    plcSourceFile.setMarkup(True)

    global plcProfileHeaderLocalFile
    plcProfileHeaderLocalFile = plcComponent.createFileSymbol("DRV_PLC_PROFILE_LOCAL", None)
    plcProfileHeaderLocalFile.setSourcePath("driver/plcPhy/src/drv_plc_phy_local_g3.h")
    plcProfileHeaderLocalFile.setOutputName("drv_plc_phy_local_comm.h")
    plcProfileHeaderLocalFile.setDestPath("driver/plc/phy")
    plcProfileHeaderLocalFile.setProjectPath("config/" + configName + "/driver/plc/phy")
    plcProfileHeaderLocalFile.setType("SOURCE")
    plcProfileHeaderLocalFile.setEnabled(True)

    #### Binary PHY Files ######################################################

    global plcSourceBinFileG3CENA
    plcSourceBinFileG3CENA = plcComponent.createLibrarySymbol("PLC_SOURCE_BIN_G3_CENA", None)
    plcSourceBinFileG3CENA.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_G3_CENA.bin")
    plcSourceBinFileG3CENA.setOutputName("PLC_PHY_G3_CENA.bin")
    plcSourceBinFileG3CENA.setDestPath("driver/plc/phy/bin")
    plcSourceBinFileG3CENA.setEnabled(True)

    global plcSourceBinFileG3CENB
    plcSourceBinFileG3CENB = plcComponent.createLibrarySymbol("PLC_SOURCE_BIN_G3_CENB", None)
    plcSourceBinFileG3CENB.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_G3_CENB.bin")
    plcSourceBinFileG3CENB.setOutputName("PLC_PHY_G3_CENB.bin")
    plcSourceBinFileG3CENB.setDestPath("driver/plc/phy/bin")
    plcSourceBinFileG3CENB.setEnabled(False)

    global plcSourceBinFileG3FCC
    plcSourceBinFileG3FCC = plcComponent.createLibrarySymbol("PLC_SOURCE_BIN_G3_FCC", None)
    plcSourceBinFileG3FCC.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_G3_FCC.bin")
    plcSourceBinFileG3FCC.setOutputName("PLC_PHY_G3_FCC.bin")
    plcSourceBinFileG3FCC.setDestPath("driver/plc/phy/bin")
    plcSourceBinFileG3FCC.setEnabled(False)

    global plcSourceBinFileG3ARIB
    plcSourceBinFileG3ARIB = plcComponent.createLibrarySymbol("PLC_SOURCE_BIN_G3_ARIB", None)
    plcSourceBinFileG3ARIB.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_G3_ARIB.bin")
    plcSourceBinFileG3ARIB.setOutputName("PLC_PHY_G3_ARIB.bin")
    plcSourceBinFileG3ARIB.setDestPath("driver/plc/phy/bin")
    plcSourceBinFileG3ARIB.setEnabled(False)

    global plcSourceBinFilePRIME
    plcSourceBinFilePRIME = plcComponent.createLibrarySymbol("PLC_SOURCE_BIN_PRIME", None)
    plcSourceBinFilePRIME.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_PRIME_2CHN.bin")
    plcSourceBinFilePRIME.setOutputName("PLC_PHY_PRIME_2CHN.bin")
    plcSourceBinFilePRIME.setDestPath("driver/plc/phy/bin")
    plcSourceBinFilePRIME.setEnabled(False)

    global plcSourceBinFileMM
    plcSourceBinFileMM = plcComponent.createLibrarySymbol("PLC_SOURCE_BIN_MM", None)
    plcSourceBinFileMM.setSourcePath("driver/plcPhy/src/bin/PLC_PHY_MM.bin")
    plcSourceBinFileMM.setOutputName("PLC_PHY_MM.bin")
    plcSourceBinFileMM.setDestPath("driver/plc/phy/bin")
    plcSourceBinFileMM.setEnabled(False)

    global plcAssemblyBinFile
    plcAssemblyBinFile = plcComponent.createFileSymbol("PLC_ASSEMBLY_BIN", None)
    plcAssemblyBinFile.setSourcePath("driver/plcPhy/src/bin/plc_phy_bin.S.ftl")
    plcAssemblyBinFile.setOutputName("plc_phy_bin.S")
    plcAssemblyBinFile.setDestPath("driver/plc/phy/bin")
    plcAssemblyBinFile.setProjectPath("config/" + configName + "/driver/plc/phy/bin")
    plcAssemblyBinFile.setType("SOURCE")
    plcAssemblyBinFile.setMarkup(True)
    plcAssemblyBinFile.setOverwrite(True)

    ##### PLC Profile Selector  ####################################################

    plcProfile = plcComponent.createComboSymbol("DRV_PLC_PROFILE", None, ["G3-PLC", "PRIME", "Meters&More"])
    plcProfile.setLabel("PLC Profile")
    plcProfile.setDefaultValue("G3-PLC")
    plcProfile.setHelp(plc_phy_helpkeyword)

    global plcStaticAddressing
    plcStaticAddressing = plcComponent.createBooleanSymbol("DRV_PLC_BIN_STATIC_ADDRESSING", plcProfile)
    plcStaticAddressing.setLabel("Static Bin file Addressing")
    plcStaticAddressing.setVisible(False)
    plcStaticAddressing.setDefaultValue(False)
    plcStaticAddressing.setDependencies(plcShowAddressingMode, ["DRV_PLC_PROFILE"])

    # Start address
    memStartAddressHex, memSizeHex = getFlashMemoryDescription()
    memStartAddressInt = int(memStartAddressHex, 0)
    memSizeInt = int(memSizeHex, 0)
    pl360BinStartAddressInt = memStartAddressInt + memSizeInt - PLC_PHY_BIN_RESERVED_SIZE

    global plcBinaryAddress
    plcBinaryAddress = plcComponent.createHexSymbol("DRV_PLC_BIN_ADDRESS", plcStaticAddressing)
    plcBinaryAddress.setLabel("PLC Bin Address")
    plcBinaryAddress.setVisible(False)
    plcBinaryAddress.setDefaultValue(pl360BinStartAddressInt)
    plcBinaryAddress.setDependencies(plcBinAddressingMode, ["DRV_PLC_BIN_STATIC_ADDRESSING"])

    global plcBinarySize
    plcBinarySize = plcComponent.createIntegerSymbol("DRV_PLC_BIN_SIZE", plcStaticAddressing)
    plcBinarySize.setLabel("PLC Bin Size (bytes)")
    plcBinarySize.setVisible(False)
    plcBinarySize.setDefaultValue(PLC_PHY_BIN_RESERVED_SIZE)
    plcBinarySize.setDependencies(plcBinAddressingMode, ["DRV_PLC_BIN_STATIC_ADDRESSING"])

    plcSecureMode = plcComponent.createBooleanSymbol("DRV_PLC_SECURE_MODE", None)
    plcSecureMode.setLabel("PLC Secure Mode")
    plcSecureMode.setVisible(False)
    plcSecureMode.setDefaultValue(False)

    updBinFilesCtrl = plcComponent.createBooleanSymbol("DRV_PLC_UPD_BIN_FILES", None)
    updBinFilesCtrl.setLabel("updBinFilesCtrl")
    updBinFilesCtrl.setDescription("Auxiliary control")
    updBinFilesCtrl.setVisible(False)
    updBinFilesCtrl.setDefaultValue(False)
    updBinFilesCtrl.setDependencies(updateShowCoupSettings, ["DRV_PLC_PROFILE"])

    ##### Coupling Settings : G3-PLC ###############################################

    global plcCoupG3Settings
    plcCoupG3Settings = plcComponent.createMenuSymbol("DRV_PLC_COUP_G3_SETTING", None)
    plcCoupG3Settings.setLabel("PLC Coupling Settings")
    plcCoupG3Settings.setDescription("Coupling Settings")
    plcCoupG3Settings.setVisible(True)
    plcCoupG3Settings.setHelp(plc_phy_helpkeyword)

    global plcG3Band
    # plcG3Band = plcComponent.createComboSymbol("DRV_PLC_G3_BAND", plcCoupG3Settings, ["CEN-A", "CEN-B", "FCC", "ARIB"])
    plcG3Band = plcComponent.createComboSymbol("DRV_PLC_G3_BAND", plcCoupG3Settings, ["CEN-A", "CEN-B", "FCC"])
    plcG3Band.setLabel("Main Branch")
    plcG3Band.setDefaultValue("CEN-A")
    plcG3Band.setHelp(plc_phy_helpkeyword)
    # plcG3Band.setDependencies(resetPlcBand, ["DRV_PLC_MODE"])

    plcCoupG3Internal = plcComponent.createBooleanSymbol("DRV_PLC_COUP_G3_INTERNAL", plcCoupG3Settings)
    plcCoupG3Internal.setLabel("Internal Driver")
    plcCoupG3Internal.setDescription("Internal Driver")
    plcCoupG3Internal.setVisible(False)
    plcCoupG3Internal.setDefaultValue(False)
    plcCoupG3Internal.setHelp(plc_phy_helpkeyword)
    plcCoupG3Internal.setDependencies(showG3InternalDriver, ["DRV_PLC_G3_BAND"])

    plcCoupGMultiBand = plcComponent.createBooleanSymbol("DRV_PLC_COUP_G3_MULTIBAND", plcCoupG3Settings)
    plcCoupGMultiBand.setLabel("Multiband")
    plcCoupGMultiBand.setDescription("Multiband")
    plcCoupGMultiBand.setVisible(False)
    plcCoupGMultiBand.setDefaultValue(False)
    plcCoupGMultiBand.setHelp(plc_phy_helpkeyword)
    plcCoupGMultiBand.setDependencies(showG3Multiband, ["DRV_PLC_G3_BAND"])

    plcG3BandAux = plcComponent.createComboSymbol("DRV_PLC_G3_BAND_AUX", plcCoupGMultiBand, ["CEN-A", "CEN-B"])
    plcG3BandAux.setLabel("Auxiliary Branch")
    plcG3BandAux.setDefaultValue("CEN-A")
    plcG3BandAux.setVisible(False)
    plcG3BandAux.setHelp(plc_phy_helpkeyword)
    plcG3BandAux.setDependencies(showG3AuxBand, ["DRV_PLC_COUP_G3_MULTIBAND"])

    plcG3BandAuxActive = plcComponent.createBooleanSymbol("DRV_PLC_G3_BAND_AUX_ACTIVE", plcCoupGMultiBand)
    plcG3BandAuxActive.setLabel("Set as default branch")
    plcG3BandAuxActive.setDefaultValue(False)
    plcG3BandAuxActive.setVisible(False)
    plcG3BandAuxActive.setHelp(plc_phy_helpkeyword)
    plcG3BandAuxActive.setDependencies(showG3AuxBand, ["DRV_PLC_COUP_G3_MULTIBAND"])

    plcCoupG3HighAttenuation = plcComponent.createBooleanSymbol("DRV_PLC_COUP_G3_HIGH_ATTENUATION", plcG3Band)
    plcCoupG3HighAttenuation.setLabel("FCC high attenuation branch")
    plcCoupG3HighAttenuation.setDescription("FCC high attenuation")
    plcCoupG3HighAttenuation.setVisible(False)
    plcCoupG3HighAttenuation.setDefaultValue(False)
    plcCoupG3HighAttenuation.setHelp(plc_phy_helpkeyword)
    plcCoupG3HighAttenuation.setDependencies(showG3HighAttenuation, ["DRV_PLC_G3_BAND", "DRV_PLC_MODE"])

    ##### Coupling Settings : PRIME  ####################################################
    global plcCoupPRIMESettings
    plcCoupPRIMESettings = plcComponent.createMenuSymbol("DRV_PLC_COUP_PRIME_SETTING", None)
    plcCoupPRIMESettings.setLabel("PLC Coupling Settings")
    plcCoupPRIMESettings.setDescription("Coupling Settings")
    plcCoupPRIMESettings.setVisible(False)
    plcCoupPRIMESettings.setHelp(plc_phy_helpkeyword)

    plcCoupPRIME1ChnMode = plcComponent.createBooleanSymbol("DRV_PLC_PRIME_1CHN_MODE", plcCoupPRIMESettings)
    plcCoupPRIME1ChnMode.setLabel("Single Channel Mode")
    plcCoupPRIME1ChnMode.setDescription("Single Channel Mode")
    plcCoupPRIME1ChnMode.setDefaultValue(False)
    plcCoupPRIME1ChnMode.setHelp(plc_phy_helpkeyword)

    plcCoupPRIME2ChnMode = plcComponent.createBooleanSymbol("DRV_PLC_PRIME_2CHN_MODE", plcCoupPRIMESettings)
    plcCoupPRIME2ChnMode.setLabel("Double Channel Mode")
    plcCoupPRIME2ChnMode.setDescription("Double Channel Mode")
    plcCoupPRIME2ChnMode.setDefaultValue(False)
    plcCoupPRIME2ChnMode.setHelp(plc_phy_helpkeyword)

    for idx in range(8):
        plcCoupPRIMECH.append(plcComponent.createBooleanSymbol("DRV_PLC_PRIME_CH" + str(idx + 1), plcCoupPRIME1ChnMode))
        plcCoupPRIMECH[idx].setLabel("Channel " + str(idx + 1))
        plcCoupPRIMECH[idx].setDescription("Channel " + str(idx + 1))
        plcCoupPRIMECH[idx].setVisible(False)
        plcCoupPRIMECH[idx].setDefaultValue(False)
        plcCoupPRIMECH[idx].setDependencies(showChannelSelect, ["DRV_PLC_PRIME_1CHN_MODE"])
        plcCoupPRIMECH[idx].setHelp(plc_phy_helpkeyword)

    for idx in range(7):
        plcCoupPRIME2CH.append(plcComponent.createBooleanSymbol("DRV_PLC_PRIME_2CH" + str(idx + 1), plcCoupPRIME2ChnMode))
        plcCoupPRIME2CH[idx].setLabel("Channels " + str(idx + 1) + " - " + str(idx + 2))
        plcCoupPRIME2CH[idx].setDescription("Channel " + str(idx + 1))
        plcCoupPRIME2CH[idx].setDefaultValue(False)
        plcCoupPRIME2CH[idx].setVisible(False)
        plcCoupPRIME2CH[idx].setHelp(plc_phy_helpkeyword)
        plcCoupPRIME2CH[idx].setDependencies(showChannelSelect, ["DRV_PLC_PRIME_2CHN_MODE"])

    global plcCoupPRIMEDefChannel
    plcCoupPRIMEDefChannel = plcComponent.createComboSymbol("DRV_PLC_PRIME_DEF_CHN", plcCoupPRIMESettings, ["CHN1", "CHN2", "CHN3", "CHN4", "CHN5", "CHN6", "CHN7", "CHN8", "CHN1_CHN2", "CHN2_CHN3", "CHN3_CHN4", "CHN4_CHN5", "CHN5_CHN6", "CHN6_CHN7", "CHN7_CHN8"])
    plcCoupPRIMEDefChannel.setLabel("Default Channel")
    plcCoupPRIMEDefChannel.setDefaultValue("CHN1")
    plcCoupPRIMEDefChannel.setVisible(True)
    plcCoupPRIMEDefChannel.setHelp(plc_phy_helpkeyword)

    global plcCoupPRIMEBandAux
    plcCoupPRIMEBandAux = plcComponent.createBooleanSymbol("DRV_PLC_PRIME_BAND_AUX", plcCoupPRIMECH[0])
    plcCoupPRIMEBandAux.setLabel("Use Auxiliary Branch")
    plcCoupPRIMEBandAux.setDefaultValue(False)
    plcCoupPRIMEBandAux.setVisible(False)
    plcCoupPRIMEBandAux.setHelp(plc_phy_helpkeyword)

    global plcCoupPRIMEHighAttenuation
    plcCoupPRIMEHighAttenuation = plcComponent.createBooleanSymbol("DRV_PLC_PRIME_HIGH_ATTENUATION", plcCoupPRIMESettings)
    plcCoupPRIMEHighAttenuation.setLabel("High attenuation branch")
    plcCoupPRIMEHighAttenuation.setDescription("High attenuation branch")
    plcCoupPRIMEHighAttenuation.setVisible(False)
    plcCoupPRIMEHighAttenuation.setDefaultValue(False)
    plcCoupPRIMEHighAttenuation.setHelp(plc_phy_helpkeyword)

    plcCoupPRIMEChannelWarning = plcComponent.createCommentSymbol("DRV_PLC_PRIME_CHANNEL_WARN", plcCoupPRIMEDefChannel)
    plcCoupPRIMEChannelWarning.setLabel("Warning!!! Default channel is not included in available channels")
    plcCoupPRIMEChannelWarning.setVisible(True)
    plcCoupPRIMEChannelWarning.setDependencies(checkPrimeChannelConf, ["DRV_PLC_PRIME_CH1", "DRV_PLC_PRIME_CH2", "DRV_PLC_PRIME_CH3", \
        "DRV_PLC_PRIME_CH4", "DRV_PLC_PRIME_CH5", "DRV_PLC_PRIME_CH6", "DRV_PLC_PRIME_CH7", "DRV_PLC_PRIME_CH8", "DRV_PLC_PRIME_1CHN_MODE", \
        "DRV_PLC_PRIME_2CH1", "DRV_PLC_PRIME_2CH2", "DRV_PLC_PRIME_2CH3", "DRV_PLC_PRIME_2CH4", "DRV_PLC_PRIME_2CH5", "DRV_PLC_PRIME_2CH6", \
        "DRV_PLC_PRIME_2CH7", "DRV_PLC_PRIME_2CHN_MODE", "DRV_PLC_PRIME_DEF_CHN", "DRV_PLC_PRIME_BAND_AUX", "DRV_PLC_PRIME_HIGH_ATTENUATION", "DRV_PLC_MODE"])

    pCoupPRIMEChannelsSelected = plcComponent.createIntegerSymbol("DRV_PLC_PRIME_CHANNELS_SELECTED", plcCoupPRIMESettings)
    pCoupPRIMEChannelsSelected.setLabel("Channels Selected")
    pCoupPRIMEChannelsSelected.setVisible(False)
    pCoupPRIMEChannelsSelected.setDefaultValue(0)

    ##### Coupling Settings : Meters&More ###############################################

    global plcCoupMMSettings
    plcCoupMMSettings = plcComponent.createMenuSymbol("DRV_PLC_COUP_MM_SETTING", None)
    plcCoupMMSettings.setLabel("PLC Coupling Settings")
    plcCoupMMSettings.setDescription("Coupling Settings")
    plcCoupMMSettings.setVisible(False)
    plcCoupMMSettings.setHelp(plc_phy_helpkeyword)

    global plcMMBand
    plcMMBand = plcComponent.createComboSymbol("DRV_PLC_MM_BAND", plcCoupMMSettings, ["CEN-A"])
    plcMMBand.setLabel("Main Branch")
    plcMMBand.setDefaultValue("CEN-A")
    plcMMBand.setReadOnly(True)
    plcMMBand.setHelp(plc_phy_helpkeyword)

    ##### Coupling Settings : Generic  ####################################################

    plcPhyKeyCortex = plcComponent.createHexSymbol("DRV_PLC_CORE_KEY", None)
    plcPhyKeyCortex.setLabel("PLC Phy Key Cortex")
    plcPhyKeyCortex.setDefaultValue(0x1122)
    plcPhyKeyCortex.setVisible(False)
    plcPhyKeyCortex.setReadOnly(True)

    global plcBandInUse
    plcBandInUse = plcComponent.createIntegerSymbol("DRV_PLC_BAND_IN_USE", None)
    plcBandInUse.setLabel("PLC Band in use")
    plcBandInUse.setDefaultValue(PLC_PROFILE_G3_CEN_A)
    plcBandInUse.setVisible(False)
    plcBandInUse.setReadOnly(True)
    plcBandInUse.setDependencies(updateG3PLCBandInUse, ["DRV_PLC_PROFILE", "DRV_PLC_G3_BAND", "DRV_PLC_G3_BAND_AUX", "DRV_PLC_COUP_G3_MULTIBAND", "DRV_PLC_G3_BAND_AUX_ACTIVE"])

    #### FreeMaker Files ######################################################

    plcSystemDefFile = plcComponent.createFileSymbol("PLC_DEF", None)
    plcSystemDefFile.setType("STRING")
    plcSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    plcSystemDefFile.setSourcePath("driver/plcPhy/templates/system/definitions.h.ftl")
    plcSystemDefFile.setMarkup(True)

    plcSymSystemDefObjFile = plcComponent.createFileSymbol("DRV_PLC_SYSTEM_DEF_OBJECT", None)
    plcSymSystemDefObjFile.setType("STRING")
    plcSymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    plcSymSystemDefObjFile.setSourcePath("driver/plcPhy/templates/system/definitions_objects.h.ftl")
    plcSymSystemDefObjFile.setMarkup(True)

    plcSymSystemConfigFile = plcComponent.createFileSymbol("DRV_PLC_CONFIGIRUTION", None)
    plcSymSystemConfigFile.setType("STRING")
    plcSymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    plcSymSystemConfigFile.setSourcePath("driver/plcPhy/templates/system/configuration.h.ftl")
    plcSymSystemConfigFile.setMarkup(True)

    plcSymSystemInitDataFile = plcComponent.createFileSymbol("DRV_PLC_INIT_DATA", None)
    plcSymSystemInitDataFile.setType("STRING")
    plcSymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    plcSymSystemInitDataFile.setSourcePath("driver/plcPhy/templates/system/initialize_data.c.ftl")
    plcSymSystemInitDataFile.setMarkup(True)

    plcSystemInitFile = plcComponent.createFileSymbol("PLC_INIT", None)
    plcSystemInitFile.setType("STRING")
    plcSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    plcSystemInitFile.setSourcePath("driver/plcPhy/templates/system/initialize.c.ftl")
    plcSystemInitFile.setMarkup(True)

    plcSystemTasksFile = plcComponent.createFileSymbol("DRV_PLC_SYS_TASK", None)
    plcSystemTasksFile.setType("STRING")
    plcSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    plcSystemTasksFile.setSourcePath("driver/plcPhy/templates/system/system_tasks.c.ftl")
    plcSystemTasksFile.setMarkup(True)

    plcSystemRtosTasksFile = plcComponent.createFileSymbol("DRV_PLC_SYS_RTOS_TASK", None)
    plcSystemRtosTasksFile.setType("STRING")
    plcSystemRtosTasksFile.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
    plcSystemRtosTasksFile.setSourcePath("driver/plcPhy/templates/system/system_rtos_tasks.c.ftl")
    plcSystemRtosTasksFile.setMarkup(True)
    plcSystemRtosTasksFile.setEnabled(getActiveRtos() != "BareMetal")
    plcSystemRtosTasksFile.setDependencies(genRtosTask, ["HarmonyCore.SELECT_RTOS"])

    updateBinFiles()

################################################################################
#### Business Logic ####
################################################################################
def onAttachmentConnected(source, target):
    global isDMAPresent

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    if connectID == "drvPlcPhy_SPI_dependency" :
        plibUsed = localComponent.getSymbolByID("DRV_PLC_PLIB")
        plibUsed.clearValues()
        plibUsed.setValue(remoteID.upper())

        if (remoteID == "srv_spisplit"):
            # Connected to SPI Splitter
            plibUsedSpiSplit.clearValues()
            plibUsedSpiSplit.setVisible(True)

        else:
            # Show/hide NPCS depending on SPI PLIB connected
            plibConfigComment.setVisible(True)
            if (spiNumNPCS > 0) and (remoteID.startswith("flexcom") or remoteID.startswith("spi")):
                # The SPI connected supports multiple NPCS/CSR
                spiNumCSR.setValue(spiNumNPCS)
                spiNpcsUsed.setVisible(True)
                plibConfigComment.setLabel("***Selected NPCS must be properly configured in the PLIB used***")
            else:
                spiNumCSR.setValue(0)
                plibConfigComment.setLabel("***The PLIB used must be properly configured***")
                if spiNumNPCS > 0:
                    spiNpcsUsed.setVisible(False)

            # Configure SPI PLIB
            configureSpiPlib(localComponent)

            if (isDMAPresent == True):
                plcDependencyDMAComment.setVisible(False)

                # Request DMA channels for Transmit
                dmaChannelID = "DMA_CH_FOR_" + remoteID.upper() + "_Transmit"
                dmaRequestID = "DMA_CH_NEEDED_FOR_" + remoteID.upper() + "_Transmit"
                Database.sendMessage("core", "DMA_CHANNEL_ENABLE", {"dma_channel":dmaRequestID})
                dmaChannel = Database.getSymbolValue("core", dmaChannelID)
                plcTXDMAChannel.setVisible(True)
                if (dmaChannel == None) or (dmaChannel < 0) or (dmaChannel >= dmaChannelCount):
                    # Error in DMA channel allocation
                    plcTXDMAChannelComment.setVisible(True)
                    plcTXDMAChannel.setValue(-1)
                else:
                    plcTXDMAChannel.setValue(dmaChannel)

                # Request DMA channels for Receive
                dmaChannelID = "DMA_CH_FOR_" + remoteID.upper() + "_Receive"
                dmaRequestID = "DMA_CH_NEEDED_FOR_" + remoteID.upper() + "_Receive"
                Database.sendMessage("core", "DMA_CHANNEL_ENABLE", {"dma_channel":dmaRequestID})
                dmaChannel = Database.getSymbolValue("core", dmaChannelID)
                plcRXDMAChannel.setVisible(True)
                if (dmaChannel == None) or (dmaChannel < 0) or (dmaChannel >= dmaChannelCount):
                    # Error in DMA channel allocation
                    plcRXDMAChannelComment.setVisible(True)
                    plcRXDMAChannel.setValue(-1)
                else:
                    plcRXDMAChannel.setValue(dmaChannel)

            # Set symbols read-only
            Database.sendMessage(remoteID, "SPI_MASTER_MODE", {"isReadOnly":True})

            remoteComponent = Database.getComponentByID(remoteID)
            if not isDMAPresent and (remoteID.startswith("flexcom") or remoteID.startswith("spi")):
                plibUseSpiDma = remoteComponent.getSymbolByID("USE_SPI_DMA")
                if plibUseSpiDma != None:
                    plibUseSpiDma.setReadOnly(True)

            Database.sendMessage(remoteID, "SPI_MASTER_INTERRUPT_MODE", {"isReadOnly":True})

            if remoteID.startswith("flexcom"):
                plibFIFO = remoteComponent.getSymbolByID("FLEXCOM_SPI_FIFO_ENABLE")
                if plibFIFO != None:
                    plibFIFO.setReadOnly(True)
            elif remoteID.startswith("sercom"):
                plibReceiver = remoteComponent.getSymbolByID("SPI_RECIEVER_ENABLE")
                if plibReceiver != None:
                    plibReceiver.setReadOnly(True)

def onAttachmentDisconnected(source, target):
    global isDMAPresent

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    if connectID == "drvPlcPhy_SPI_dependency":
        # Disable read-only in PLIB
        deconfigureSpiPlib(localComponent)

        localComponent.getSymbolByID("DRV_PLC_PLIB").clearValues()
        plibUsedSpiSplit.setVisible(False)
        plibConfigComment.setVisible(False)
        if spiNumNPCS > 0:
            spiNpcsUsed.setVisible(False)

        if isDMAPresent:
            # Hide DMA comments
            plcTXDMAChannel.setVisible(False)
            plcRXDMAChannel.setVisible(False)
            plcTXDMAChannelComment.setVisible(False)
            plcRXDMAChannelComment.setVisible(False)
            plcDependencyDMAComment.setVisible(True)

        if (remoteID != "srv_spisplit"):
            dummyDict = {}
            dummyDict = Database.sendMessage(remoteID, "SPI_MASTER_MODE", {"isReadOnly":False})
            dummyDict = Database.sendMessage(remoteID, "SPI_MASTER_INTERRUPT_MODE", {"isReadOnly":False})

            if (isDMAPresent == True):
                # Deactivate requested DMA channels for transmit and receive
                dmaChannelID = "DMA_CH_FOR_" + remoteID.upper() + "_Transmit"
                dmaRequestID = "DMA_CH_NEEDED_FOR_" + remoteID.upper() + "_Transmit"
                Database.sendMessage("core", "DMA_CHANNEL_DISABLE", {"dma_channel":dmaRequestID})

                dmaChannelID = "DMA_CH_FOR_" + remoteID.upper() + "_Receive"
                dmaRequestID = "DMA_CH_NEEDED_FOR_" + remoteID.upper() + "_Receive"
                Database.sendMessage("core", "DMA_CHANNEL_DISABLE", {"dma_channel":dmaRequestID})

            if not isDMAPresent and ("FLEXCOM" in remoteID.upper() or "SPI" in remoteID.upper()):
                remoteSym = remoteComponent.getSymbolByID("USE_SPI_DMA")
                if remoteSym != None:
                    remoteSym.setReadOnly(False)

            if "FLEXCOM" in remoteID.upper():
                remoteSym = remoteComponent.getSymbolByID("FLEXCOM_SPI_FIFO_ENABLE")
                if remoteSym != None:
                    remoteSym.setReadOnly(False)
            elif "SERCOM" in remoteID.upper():
                remoteSym = remoteComponent.getSymbolByID("SPI_RECIEVER_ENABLE")
                if remoteSym != None:
                    remoteSym.setReadOnly(False)

def destroyComponent(plcComponent):
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_PORTS", {"isEnabled":False})

    if isDMAPresent:
        Database.sendMessage("HarmonyCore", "ENABLE_SYS_DMA", {"isEnabled":False})
