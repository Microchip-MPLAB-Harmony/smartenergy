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

global g3MacRtBinFileCENA
global g3MacRtBinFileCENB
global g3MacRtBinFileFCC
global g3MacRtBinFileARIB
global g3MacRtAsmBinFile

global plcBandInUse

plc_mac_rt_helpkeyword = "drv_g3_macrt_configurations"
gPlcBand = ""

PLC_PROFILE_G3_CEN_A = 1
PLC_PROFILE_G3_FCC = 2
PLC_PROFILE_G3_ARIB = 3
PLC_PROFILE_G3_CEN_B = 4
PLC_PROFILE_G3_FCC_CEN_A = 5
PLC_PROFILE_G3_FCC_CEN_B = 6
PLC_PROFILE_G3_ARIB_CEN_A = 7
PLC_PROFILE_G3_ARIB_CEN_B = 8

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
        # print("DRVPLC handleMessage pad: {} pinId:{}".format(pad, pinId))
        if pad == pinId:
            return index
        index += 1

    return None

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
        
        # print("DRVPLC handleMessage: {} args: {}".format(messageID, args))
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

            # print("DRVPLC handleMessage GPIO: {} - pinDescr: {}".format(symbolName, pinDescr))

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
            # print("DRVPLC handleMessage CS: {} - pinDescr: {}".format(symbolName, pinDescr))

        if symbolValue != None:
            # print("CHRIS dbg >> drvG3MacRt DRVPLC_CONFIG_HW_IO set {}: {}".format(symbolName, symbolValue))
            res = Database.setSymbolValue('drvG3MacRt', symbolName, symbolValue)
            if res == True:
                result_dict = {"Result": "Success"}
                if symbolName == "DRV_PLC_EXT_INT_PIN":
                    eic = Database.getSymbolValue("drvG3MacRt", "PLC_EIC_ID")
                    if (eic != "0") and (functionValue.split("_")[0].upper() == "EIC"):
                        intNum = int("".join(filter(lambda x: x.isdigit(), functionValue.split("_")[-1])))
                        res = Database.setSymbolValue('drvG3MacRt', "DRV_PLC_EIC_SIGNAL", "EIC_PIN_{}".format(intNum))
                        if res == True:
                            result_dict = {"Result": "Success"}
                        else:
                            result_dict = {"Result": "Fail"}

    return result_dict

def sort_alphanumeric(l):
    import re
    convert = lambda text: int(text) if text.isdigit() else text.lower()
    alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ]
    return sorted(l, key = alphanum_key)

def g3MacRtExternalInterruptTrigger(symbol, event):
    key = event["symbol"].getKeyDescription(event["value"])
    intSrc = "PIO" + key[1] + "_IRQn"
    symbol.setValue(intSrc, 1)

def g3MacRtExternalInterruptPort(symbol, event):
    key = event["symbol"].getKeyDescription(event["value"])
    intPort = "PIO_PORT_" + key[1]
    symbol.setValue(intPort)

def g3MacRtEnableEncScript(symbol, event):
    if (event["value"] == True):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False)

def g3MacRtVisibleEncComment(symbol, event):
    if (event["value"] == True):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def setPlcBandInUse(plcBand):
    dict = {}

    if (plcBand == "CEN-A"):
        plcBandInUse.setValue(PLC_PROFILE_G3_CEN_A)
    elif (plcBand == "CEN-B"):
        plcBandInUse.setValue(PLC_PROFILE_G3_CEN_B)
    elif (plcBand == "FCC"):
        plcBandInUse.setValue(PLC_PROFILE_G3_FCC)
    elif (plcBand == "ARIB"):
        plcBandInUse.setValue(PLC_PROFILE_G3_ARIB)

def setPlcMultiBandInUse(g3_band, g3_aux_band):
    if (g3_band == "FCC"):
        if (g3_aux_band == "CEN-A"):
            plcBandInUse.setValue(PLC_PROFILE_G3_FCC_CEN_A)
        elif (g3_aux_band == "CEN-B"):
            plcBandInUse.setValue(PLC_PROFILE_G3_FCC_CEN_B)
    else:
        if (g3_aux_band == "CEN-A"):
            plcBandInUse.setValue(PLC_PROFILE_G3_ARIB_CEN_A)
        elif (g3_aux_band == "CEN-B"):
            plcBandInUse.setValue(PLC_PROFILE_G3_ARIB_CEN_B)

def removeAllBinFiles():
    g3MacRtBinFileCENA.setEnabled(False)
    g3MacRtBinFileCENB.setEnabled(False)
    g3MacRtBinFileFCC.setEnabled(False)
    g3MacRtBinFileARIB.setEnabled(False)

def includeBinFile(plcBand):
    if (plcBand == "CEN-A"):
        g3MacRtBinFileCENA.setEnabled(True)
    elif (plcBand == "CEN-B"):
        g3MacRtBinFileCENB.setEnabled(True)
    elif (plcBand == "FCC"):
        g3MacRtBinFileFCC.setEnabled(True)
    elif (plcBand == "ARIB"):
        g3MacRtBinFileARIB.setEnabled(True)

def updateBinFiles():
    dict = {}
    removeAllBinFiles()
    g3_band = Database.getSymbolValue("drvG3MacRt", "DRV_PLC_G3_BAND")
    includeBinFile(g3_band)
    setPlcBandInUse(g3_band)
    if (g3_band == "FCC" or g3_band =="ARIB") and (Database.getSymbolValue("drvG3MacRt", "DRV_PLC_COUP_G3_MULTIBAND") == True) :
        g3_aux_band = Database.getSymbolValue("drvG3MacRt", "DRV_PLC_G3_BAND_AUX")
        includeBinFile(g3_aux_band)
        setPlcMultiBandInUse(g3_band, g3_aux_band)

    # Update Coupling Parameters
    dict = Database.sendMessage("srv_pcoup", "SRV_PCOUP_UPDATE_G3_PARAMETERS", {})

    # Check Internal/External Addressing
    if (Database.getSymbolValue("drvG3MacRt", "DRV_PLC_BIN_STATIC_ADDRESSING") == False) :
        # Internal Addressing
        g3MacRtAsmBinFile.setEnabled(True)
    else:
        # External Addressing
        removeAllBinFiles()
        g3MacRtAsmBinFile.setEnabled(False)

def updateG3PLCBandInUse(symbol, event):
    updateBinFiles()

def plcBinAddressingMode(symbol, event):
    symbol.setVisible(event["value"])
    updateBinFiles()

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

def resetPlcBand(symbol, event):
    symbol.setReadOnly(True)
    symbol.setValue("CEN-A")
    symbol.setReadOnly(False)

def enablePL460Capabilities(symbol, event):
    if (event["value"] == "PL460"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

    # Send PL360/PL460 to PLC PAL (if present) to set PVDD Monitor as read-only
    Database.sendMessage("g3PalPlc", "PLC_DEVICE", {"Device": event["value"]})

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


def instantiateComponent(g3MacRtComponent):
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

    # Set G3 mode in RSNIFFER (if present)
    Database.sendMessage("srv_rsniffer", "SRV_RSNIFFER_G3", {})

    identifyPeripherals(g3MacRtComponent)
    deviceNode = ATDF.getNode("/avr-tools-device-file/devices/device")
    architecture = deviceNode.getAttribute("architecture")

    architectureDevice = g3MacRtComponent.createStringSymbol("DRV_PLC_ARCH", None)
    architectureDevice.setDefaultValue(str(architecture))
    architectureDevice.setVisible(False)

    global plcDriverMode
    plcDriverMode = g3MacRtComponent.createComboSymbol("DRV_PLC_MODE", None, ["PL360", "PL460"])
    plcDriverMode.setLabel("PLC Driver Mode")
    plcDriverMode.setDefaultValue("PL460")
    plcDriverMode.setHelp(plc_mac_rt_helpkeyword)
    Database.sendMessage("g3PalPlc", "PLC_DEVICE", {"Device": "PL460"})

    # RTOS CONFIG
    plcRTOSMenu = g3MacRtComponent.createMenuSymbol("DRV_PLC_RTOS_MENU", None)
    plcRTOSMenu.setLabel("RTOS settings")
    plcRTOSMenu.setDescription("RTOS settings")
    plcRTOSMenu.setVisible(getActiveRtos() != "BareMetal")
    plcRTOSMenu.setDependencies(showRTOSMenu, ["HarmonyCore.SELECT_RTOS"])
    plcRTOSMenu.setHelp(plc_mac_rt_helpkeyword)

    plcRTOSStackSize = g3MacRtComponent.createIntegerSymbol("DRV_PLC_RTOS_STACK_SIZE", plcRTOSMenu)
    plcRTOSStackSize.setLabel("Stack Size (in bytes)")
    plcRTOSStackSize.setDefaultValue(1280)
    plcRTOSStackSize.setMin(512)
    plcRTOSStackSize.setMax(16*1024)
    plcRTOSStackSize.setHelp(plc_mac_rt_helpkeyword)

    plcRTOSTaskPriority = g3MacRtComponent.createIntegerSymbol("DRV_PLC_RTOS_TASK_PRIORITY", plcRTOSMenu)
    plcRTOSTaskPriority.setLabel("Task Priority")
    plcRTOSTaskPriority.setDefaultValue(1)
    plcRTOSTaskPriority.setMin(0)
    plcRTOSTaskPriority.setHelp(plc_mac_rt_helpkeyword)

    plcRTOSMsgQSize = g3MacRtComponent.createIntegerSymbol("DRV_PLC_RTOS_TASK_MSG_QTY", plcRTOSMenu)
    plcRTOSMsgQSize.setLabel("Maximum Message Queue Size")
    plcRTOSMsgQSize.setDescription("A µC/OS-III task contains an optional internal message queue (if OS_CFG_TASK_Q_EN is set to DEF_ENABLED in os_cfg.h). This argument specifies the maximum number of messages that the task can receive through this message queue. The user may specify that the task is unable to receive messages by setting this argument to 0")
    plcRTOSMsgQSize.setDefaultValue(0)
    plcRTOSMsgQSize.setMin(0)
    plcRTOSMsgQSize.setVisible(getActiveRtos() == "MicriumOSIII")
    plcRTOSMsgQSize.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])
    plcRTOSMsgQSize.setHelp(plc_mac_rt_helpkeyword)

    plcRTOSTaskTimeQuanta = g3MacRtComponent.createIntegerSymbol("DRV_PLC_RTOS_TASK_TIME_QUANTA", plcRTOSMenu)
    plcRTOSTaskTimeQuanta.setLabel("Task Time Quanta")
    plcRTOSTaskTimeQuanta.setDescription("The amount of time (in clock ticks) for the time quanta when Round Robin is enabled. If you specify 0, then the default time quanta will be used which is the tick rate divided by 10.")
    plcRTOSTaskTimeQuanta.setDefaultValue(0)
    plcRTOSTaskTimeQuanta.setMin(0)
    plcRTOSTaskTimeQuanta.setVisible(getActiveRtos() == "MicriumOSIII")
    plcRTOSTaskTimeQuanta.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])
    plcRTOSTaskTimeQuanta.setHelp(plc_mac_rt_helpkeyword)

    plcRTOSTaskSpecificOpt = g3MacRtComponent.createBooleanSymbol("DRV_PLC_RTOS_TASK_OPT_NONE", plcRTOSMenu)
    plcRTOSTaskSpecificOpt.setLabel("Task Specific Options")
    plcRTOSTaskSpecificOpt.setDescription("Contains task-specific options. Each option consists of one bit. The option is selected when the bit is set. The current version of µC/OS-III supports the following options:")
    plcRTOSTaskSpecificOpt.setDefaultValue(True)
    plcRTOSTaskSpecificOpt.setVisible(getActiveRtos() == "MicriumOSIII")
    plcRTOSTaskSpecificOpt.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])
    plcRTOSTaskSpecificOpt.setHelp(plc_mac_rt_helpkeyword)

    plcRTOSTaskStkChk = g3MacRtComponent.createBooleanSymbol("DRV_PLC_RTOS_TASK_OPT_STK_CHK", plcRTOSTaskSpecificOpt)
    plcRTOSTaskStkChk.setLabel("Stack checking is allowed for the task")
    plcRTOSTaskStkChk.setDescription("Specifies whether stack checking is allowed for the task")
    plcRTOSTaskStkChk.setDefaultValue(True)
    plcRTOSTaskStkChk.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_PLC_RTOS_TASK_OPT_NONE"])
    plcRTOSTaskStkChk.setHelp(plc_mac_rt_helpkeyword)

    plcRTOSTaskStkClr = g3MacRtComponent.createBooleanSymbol("DRV_PLC_RTOS_TASK_OPT_STK_CLR", plcRTOSTaskSpecificOpt)
    plcRTOSTaskStkClr.setLabel("Stack needs to be cleared")
    plcRTOSTaskStkClr.setDescription("Specifies whether the stack needs to be cleared")
    plcRTOSTaskStkClr.setDefaultValue(True)
    plcRTOSTaskStkClr.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_PLC_RTOS_TASK_OPT_NONE"])
    plcRTOSTaskStkClr.setHelp(plc_mac_rt_helpkeyword)

    plcRTOSTaskSaveFp = g3MacRtComponent.createBooleanSymbol("DRV_PLC_RTOS_TASK_OPT_SAVE_FP", plcRTOSTaskSpecificOpt)
    plcRTOSTaskSaveFp.setLabel("Floating-point registers needs to be saved")
    plcRTOSTaskSaveFp.setDescription("Specifies whether floating-point registers are saved. This option is only valid if the processor has floating-point hardware and the processor-specific code saves the floating-point registers")
    plcRTOSTaskSaveFp.setDefaultValue(False)
    plcRTOSTaskSaveFp.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_PLC_RTOS_TASK_OPT_NONE"])
    plcRTOSTaskSaveFp.setHelp(plc_mac_rt_helpkeyword)

    plcRTOSTaskNoTls = g3MacRtComponent.createBooleanSymbol("DRV_PLC_RTOS_TASK_OPT_NO_TLS", plcRTOSTaskSpecificOpt)
    plcRTOSTaskNoTls.setLabel("TLS (Thread Local Storage) support needed for the task")
    plcRTOSTaskNoTls.setDescription("If the caller doesn’t want or need TLS (Thread Local Storage) support for the task being created. If you do not include this option, TLS will be supported by default. TLS support was added in V3.03.00")
    plcRTOSTaskNoTls.setDefaultValue(False)
    plcRTOSTaskNoTls.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_PLC_RTOS_TASK_OPT_NONE"])
    plcRTOSTaskNoTls.setHelp(plc_mac_rt_helpkeyword)

    plcPLIB = g3MacRtComponent.createStringSymbol("DRV_PLC_PLIB", None)
    plcPLIB.setLabel("PLIB Used")
    plcPLIB.setReadOnly(True)
    plcPLIB.setHelp(plc_mac_rt_helpkeyword)

    global plibUsedSpiSplit
    plibUsedSpiSplit = g3MacRtComponent.createStringSymbol("DRV_PLC_PLIB_SPISPLIT", None)
    plibUsedSpiSplit.setLabel("PLIB Used by SPI Splitter")
    plibUsedSpiSplit.setDescription("PLIB connected to SPI dependency in SPI Splitter")
    plibUsedSpiSplit.setReadOnly(True)
    plibUsedSpiSplit.setVisible(False)
    plibUsedSpiSplit.setHelp(plc_mac_rt_helpkeyword)

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
        spiNpcsUsed = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_SPI_NPCS", None)
        spiNpcsUsed.setLabel("SPI NPCS Used")
        spiNpcsUsed.setDescription("Select SPI NPCS (peripheral chip select) connected to PLC Device")
        spiNpcsUsed.setOutputMode("Value")
        spiNpcsUsed.setDisplayMode("Key")
        spiNpcsUsed.setDefaultValue(0)
        spiNpcsUsed.setVisible(False)
        spiNpcsUsed.setHelp(plc_mac_rt_helpkeyword)

        currentNPCS = 0

        for npcs in range(0, spiNumNPCS):
            spiNpcsUsed.addKey("NPCS" + str(npcs), str(npcs), "SPI NPCS" + str(npcs) + " used by RF215 Driver")

    global spiNumCSR
    spiNumCSR = g3MacRtComponent.createIntegerSymbol("DRV_PLC_SPI_NUM_CSR", None)
    spiNumCSR.setVisible(False)
    spiNumCSR.setDependencies(npcsChanged, ["DRV_PLC_SPI_NPCS"])

    global plibConfigComment
    plibConfigComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_PLIB_CONFIG_COMMENT", None)
    plibConfigComment.setVisible(False)

    global plcExtIntPin
    plcExtIntPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_EXT_INT_PIN", None)
    plcExtIntPin.setLabel("External Interrupt Pin")
    plcExtIntPin.setDefaultValue(0)
    plcExtIntPin.setOutputMode("Key")
    plcExtIntPin.setDisplayMode("Description")
    plcExtIntPin.setHelp(plc_mac_rt_helpkeyword)

    plcExtIntSource = g3MacRtComponent.createStringSymbol("DRV_PLC_EXT_INT_SRC", None)
    plcExtIntSource.setLabel("External Interrupt Source")
    plcExtIntSource.setDefaultValue("PIOA_IRQn")
    plcExtIntSource.setVisible(False)
    plcExtIntSource.setReadOnly(True)
    plcExtIntSource.setHelp(plc_mac_rt_helpkeyword)
    plcExtIntSource.setDependencies(g3MacRtExternalInterruptTrigger, ["DRV_PLC_EXT_INT_PIN"])

    plcExtIntPioPort = g3MacRtComponent.createStringSymbol("DRV_PLC_EXT_INT_PIO_PORT", None)
    plcExtIntPioPort.setLabel("External Interrupt Port")
    plcExtIntPioPort.setDefaultValue("PIO_PORT_A")
    plcExtIntPioPort.setVisible(False)
    plcExtIntPioPort.setReadOnly(True)
    plcExtIntPioPort.setHelp(plc_mac_rt_helpkeyword)
    plcExtIntPioPort.setDependencies(g3MacRtExternalInterruptPort, ["DRV_PLC_EXT_INT_PIN"])

    plcResetPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_RESET_PIN", None)
    plcResetPin.setLabel("Reset Pin")
    plcResetPin.setDefaultValue(0)
    plcResetPin.setOutputMode("Key")
    plcResetPin.setDisplayMode("Description")
    plcResetPin.setHelp(plc_mac_rt_helpkeyword)

    plcLDOEnPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_LDO_EN_PIN", None)
    plcLDOEnPin.setLabel("LDO Enable Pin")
    plcLDOEnPin.setDefaultValue(0)
    plcLDOEnPin.setOutputMode("Key")
    plcLDOEnPin.setDisplayMode("Description")
    plcLDOEnPin.setHelp(plc_mac_rt_helpkeyword)

    plcTxEnablePin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_TX_ENABLE_PIN", None)
    plcTxEnablePin.setLabel("TX Enable Pin")
    plcTxEnablePin.setDefaultValue(0)
    plcTxEnablePin.setOutputMode("Key")
    plcTxEnablePin.setDisplayMode("Description")
    plcTxEnablePin.setHelp(plc_mac_rt_helpkeyword)
    plcTxEnablePin.setDependencies(enablePL460Capabilities, ["DRV_PLC_MODE"]);

    plcSleepMode = g3MacRtComponent.createBooleanSymbol("DRV_PLC_SLEEP_MODE", None)
    plcSleepMode.setLabel("Sleep Mode")
    plcSleepMode.setDefaultValue(False)
    plcSleepMode.setVisible(True)
    plcSleepMode.setHelp(plc_mac_rt_helpkeyword)

    plcStbyPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_STBY_PIN", plcSleepMode)
    plcStbyPin.setLabel("Stand By Pin")
    plcStbyPin.setDefaultValue(0)
    plcStbyPin.setOutputMode("Key")
    plcStbyPin.setDisplayMode("Description")
    plcStbyPin.setVisible(False)
    plcStbyPin.setHelp(plc_mac_rt_helpkeyword)
    plcStbyPin.setDependencies(showSleepPin, ["DRV_PLC_SLEEP_MODE"])

    plcThermalMonitor = g3MacRtComponent.createBooleanSymbol("DRV_PLC_THERMAL_MONITOR", None)
    plcThermalMonitor.setLabel("Thermal Monitor")
    plcThermalMonitor.setDefaultValue(False)
    plcThermalMonitor.setHelp(plc_mac_rt_helpkeyword)
    plcThermalMonitor.setDependencies(enablePL460Capabilities, ["DRV_PLC_MODE"]);

    plcThMonPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_THMON_PIN", plcThermalMonitor)
    plcThMonPin.setLabel("Thermal Monitor Pin")
    plcThMonPin.setDefaultValue(0)
    plcThMonPin.setOutputMode("Key")
    plcThMonPin.setDisplayMode("Description")
    plcThMonPin.setVisible(False)
    plcThMonPin.setHelp(plc_mac_rt_helpkeyword)
    plcThMonPin.setDependencies(showThermalMonitorPin, ["DRV_PLC_THERMAL_MONITOR"])

    plcSPICSPin = g3MacRtComponent.createKeyValueSetSymbol("DRV_PLC_SPI_CS_PIN", None)  # Only used with SERCOM
    plcSPICSPin.setLabel("SPI Chip Select Pin")
    plcSPICSPin.setDefaultValue(0)
    plcSPICSPin.setOutputMode("Key")
    plcSPICSPin.setHelp(plc_mac_rt_helpkeyword)
    plcSPICSPin.setDisplayMode("Description")
    plcSPICSPin.setVisible(False)

    availablePinDictionary = {}

    # Send message to core to get available pins
    availablePinDictionary = Database.sendMessage("core", "PIN_LIST", availablePinDictionary)

    eic = Database.getSymbolValue("drvG3MacRt", "PLC_EIC_ID")

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

    plcSymPinConfigComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_PINS_CONFIG_COMMENT", None)
    plcSymPinConfigComment.setVisible(True)
    plcSymPinConfigComment.setLabel("***Above selected pins must be properly configured by Pin Manager***")

    if (eic != "0"):
        global eicSignalsATDF
        eicSignalsATDF = ATDF.getNode('/avr-tools-device-file/devices/device/peripherals/module@[name="EIC"]/instance/signals').getChildren()

        global plcEICSignal
        eicSignals = getEICSignals()
        plcEICSignal = g3MacRtComponent.createComboSymbol("DRV_PLC_EIC_SIGNAL", plcExtIntPin, eicSignals)
        plcEICSignal.setLabel("EIC signal")
        plcEICSignal.setDefaultValue(eicSignals[0])

        plcEICSignalComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_EIC_SIGNAL_COMMENT", plcExtIntPin)
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
        plcTXDMAChannel = g3MacRtComponent.createIntegerSymbol("DRV_PLC_TX_DMA_CHANNEL", None)
        plcTXDMAChannel.setLabel("DMA Channel For Transmit")
        plcTXDMAChannel.setDescription("Allocated DMA channel for SPI Transmit")
        plcTXDMAChannel.setDefaultValue(0)
        plcTXDMAChannel.setMin(-1)
        plcTXDMAChannel.setVisible(False)
        plcTXDMAChannel.setReadOnly(True)

        plcTXDMAChannelComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_TX_DMA_CH_COMMENT", None)
        plcTXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Transmit. Check DMA manager.")
        plcTXDMAChannelComment.setVisible(False)

        plcRXDMAChannel = g3MacRtComponent.createIntegerSymbol("DRV_PLC_RX_DMA_CHANNEL", None)
        plcRXDMAChannel.setLabel("DMA Channel For Receive")
        plcRXDMAChannel.setDescription("Allocated DMA channel for SPI Receive")
        plcRXDMAChannel.setDefaultValue(1)
        plcRXDMAChannel.setMin(-1)
        plcRXDMAChannel.setVisible(False)
        plcRXDMAChannel.setReadOnly(True)

        plcRXDMAChannelComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_RX_DMA_CH_COMMENT", None)
        plcRXDMAChannelComment.setLabel("Warning!!! Couldn't Allocate DMA Channel for Receive. Check DMA manager.")
        plcRXDMAChannelComment.setVisible(False)

        # Get number of DMA channels
        dmaChannelCount = Database.getSymbolValue("core", "DMA_CHANNEL_COUNT")
        if dmaChannelCount != None:
            plcTXDMAChannel.setMax(dmaChannelCount - 1)
            plcRXDMAChannel.setMax(dmaChannelCount - 1)

    global plcDependencyDMAComment
    plcDependencyDMAComment = g3MacRtComponent.createCommentSymbol("DRV_PLC_DEPENDENCY_DMA_COMMENT", None)
    plcDependencyDMAComment.setLabel("!!! Satisfy PLIB Dependency to Allocate DMA Channel !!!")
    plcDependencyDMAComment.setVisible(isDMAPresent)

    plcStaticAddressing = g3MacRtComponent.createBooleanSymbol("DRV_PLC_BIN_STATIC_ADDRESSING", None)
    plcStaticAddressing.setLabel("Static Bin file Addressing")
    plcStaticAddressing.setVisible(False)
    plcStaticAddressing.setDefaultValue(False)

    plcBinaryAddress = g3MacRtComponent.createHexSymbol("DRV_PLC_PLC_BIN_ADDRESS", plcStaticAddressing)
    plcBinaryAddress.setLabel("PLC Bin Address")
    plcBinaryAddress.setVisible(False)
    plcBinaryAddress.setDefaultValue(0x004A0000)
    plcBinaryAddress.setDependencies(plcBinAddressingMode, ["DRV_PLC_BIN_STATIC_ADDRESSING"])

    plcBinarySize = g3MacRtComponent.createHexSymbol("DRV_PLC_PLC_BIN_SIZE", plcStaticAddressing)
    plcBinarySize.setLabel("PLC Bin Size (bytes)")
    plcBinarySize.setVisible(False)
    plcBinarySize.setDefaultValue(0x10000)
    plcBinarySize.setDependencies(plcBinAddressingMode, ["DRV_PLC_BIN_STATIC_ADDRESSING"])

    plcSecureMode = g3MacRtComponent.createBooleanSymbol("DRV_PLC_SECURE_MODE", None)
    plcSecureMode.setLabel("PLC Secure Mode")
    plcSecureMode.setVisible(False)
    plcSecureMode.setDefaultValue(False)

    ############################################################################
    #### Code Generation ####
    ############################################################################

    configName = Variables.get("__CONFIGURATION_NAME")

    #### Common Files ##########################################################

    g3MacRtHalHeaderFile = g3MacRtComponent.createFileSymbol("PLC_HAL_HEADER", None)
    g3MacRtHalHeaderFile.setSourcePath("driver/common/plcHal/drv_plc_hal.h.ftl")
    g3MacRtHalHeaderFile.setOutputName("drv_plc_hal.h")
    g3MacRtHalHeaderFile.setDestPath("driver/plc/common")
    g3MacRtHalHeaderFile.setProjectPath("config/" + configName + "/driver/plc/common")
    g3MacRtHalHeaderFile.setType("HEADER")
    g3MacRtHalHeaderFile.setMarkup(True)

    g3MacRtHalFile = g3MacRtComponent.createFileSymbol("PLC_HAL", None)
    g3MacRtHalFile.setSourcePath("driver/common/plcHal/drv_plc_hal.c.ftl")
    g3MacRtHalFile.setOutputName("drv_plc_hal.c")
    g3MacRtHalFile.setDestPath("driver/plc/common")
    g3MacRtHalFile.setProjectPath("config/" + configName + "/driver/plc/common")
    g3MacRtHalFile.setType("SOURCE")
    g3MacRtHalFile.setMarkup(True)

    g3MacRtHALInitDataFile = g3MacRtComponent.createFileSymbol("DRV_HAL_INIT_DATA", None)
    g3MacRtHALInitDataFile.setType("STRING")
    g3MacRtHALInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    g3MacRtHALInitDataFile.setSourcePath("driver/common/plcHal/drv_plc_hal_initialization_data.c.ftl")
    g3MacRtHALInitDataFile.setMarkup(True)

    g3MacRtBootFile = g3MacRtComponent.createFileSymbol("PLC_BOOT", None)
    g3MacRtBootFile.setSourcePath("driver/common/plcBoot/drv_plc_boot.c.ftl")
    g3MacRtBootFile.setOutputName("drv_plc_boot.c")
    g3MacRtBootFile.setDestPath("driver/plc/common")
    g3MacRtBootFile.setProjectPath("config/" + configName + "/driver/plc/common")
    g3MacRtBootFile.setType("SOURCE")
    g3MacRtBootFile.setMarkup(True)

    g3MacRtBootHeaderFile = g3MacRtComponent.createFileSymbol("PLC_BOOT_HEADER", None)
    g3MacRtBootHeaderFile.setSourcePath("driver/common/plcBoot/drv_plc_boot.h")
    g3MacRtBootHeaderFile.setOutputName("drv_plc_boot.h")
    g3MacRtBootHeaderFile.setDestPath("driver/plc/common")
    g3MacRtBootHeaderFile.setProjectPath("config/" + configName + "/driver/plc/common")
    g3MacRtBootHeaderFile.setType("HEADER")

    #### ASM Path Settings #####################################################

    plcAsmPathSetting = g3MacRtComponent.createSettingSymbol("DRV_PLC_ASM_PATH_SETTING", None)
    plcAsmPathSetting.setCategory("C32-AS")
    plcAsmPathSetting.setKey("extra-include-directories-for-assembler")
    plcAsmPathSetting.setValue("../src/config/" + configName + "/driver/plc/g3MacRt/bin")
    plcAsmPathSetting.setAppend(True, ";")

    #### Miscellanea ###########################################################

    g3MacRtSecureScript = g3MacRtComponent.createFileSymbol("DRV_PLC_PHY_SECURE_SCRIPT", None)
    g3MacRtSecureScript.setSourcePath("driver/g3MacRt/src/bin/plc_encfile.py")
    g3MacRtSecureScript.setOutputName("plc_encfile.py")
    g3MacRtSecureScript.setDestPath("driver/plc/g3MacRt/bin/")
    g3MacRtSecureScript.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin/")
    g3MacRtSecureScript.setType("SOURCE")
    g3MacRtSecureScript.setEnabled(False)
    g3MacRtSecureScript.setDependencies(g3MacRtEnableEncScript, ["DRV_PLC_SECURE_MODE"])

    g3MacRtKeyCortex = g3MacRtComponent.createHexSymbol("DRV_PLC_CORE_KEY", None)
    g3MacRtKeyCortex.setLabel("PLC Mac Rt Key Cortex")
    g3MacRtKeyCortex.setDefaultValue(0x5A5A)
    g3MacRtKeyCortex.setVisible(False)
    g3MacRtKeyCortex.setReadOnly(True)

    #### Source and header PLC PHY Files #######################################

    g3MacRtHdrFile = g3MacRtComponent.createFileSymbol("MACRT_HEADER", None)
    g3MacRtHdrFile.setSourcePath("driver/g3MacRt/drv_g3_macrt.h.ftl")
    g3MacRtHdrFile.setOutputName("drv_g3_macrt.h")
    g3MacRtHdrFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtHdrFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtHdrFile.setType("HEADER")
    g3MacRtHdrFile.setMarkup(True)

    g3MacRtHdrCommFile = g3MacRtComponent.createFileSymbol("MACRT_HEADER_COMM", None)
    g3MacRtHdrCommFile.setSourcePath("driver/g3MacRt/drv_g3_macrt_comm.h.ftl")
    g3MacRtHdrCommFile.setOutputName("drv_g3_macrt_comm.h")
    g3MacRtHdrCommFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtHdrCommFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtHdrCommFile.setType("HEADER")
    g3MacRtHdrCommFile.setMarkup(True)

    g3MacRtHdrDefFile = g3MacRtComponent.createFileSymbol("MACRT_HEADER_DEF", None)
    g3MacRtHdrDefFile.setSourcePath("driver/g3MacRt/drv_g3_macrt_definitions.h")
    g3MacRtHdrDefFile.setOutputName("drv_g3_macrt_definitions.h")
    g3MacRtHdrDefFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtHdrDefFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtHdrDefFile.setType("HEADER")

    g3MacRtSrcFile = g3MacRtComponent.createFileSymbol("MACRT_SOURCE", None)
    g3MacRtSrcFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt.c.ftl")
    g3MacRtSrcFile.setOutputName("drv_g3_macrt.c")
    g3MacRtSrcFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtSrcFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtSrcFile.setType("SOURCE")
    g3MacRtSrcFile.setMarkup(True)

    g3MacRtSrcCommFile = g3MacRtComponent.createFileSymbol("MACRT_SOURCE_COMM", None)
    g3MacRtSrcCommFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt_local_comm.c.ftl")
    g3MacRtSrcCommFile.setOutputName("drv_g3_macrt_local_comm.c")
    g3MacRtSrcCommFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtSrcCommFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtSrcCommFile.setType("SOURCE")
    g3MacRtSrcCommFile.setMarkup(True)

    g3MacRtSrcLocalFile = g3MacRtComponent.createFileSymbol("MACRT_SOURCE_LOCAL", None)
    g3MacRtSrcLocalFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt_local.h.ftl")
    g3MacRtSrcLocalFile.setOutputName("drv_g3_macrt_local.h")
    g3MacRtSrcLocalFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtSrcLocalFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtSrcLocalFile.setType("SOURCE")
    g3MacRtSrcLocalFile.setMarkup(True)

    g3MacRtSrcLocalCommFile = g3MacRtComponent.createFileSymbol("MACRT_SOURCE_LOCAL_COMM", None)
    g3MacRtSrcLocalCommFile.setSourcePath("driver/g3MacRt/src/drv_g3_macrt_local_comm.h")
    g3MacRtSrcLocalCommFile.setOutputName("drv_g3_macrt_local_comm.h")
    g3MacRtSrcLocalCommFile.setDestPath("driver/plc/g3MacRt")
    g3MacRtSrcLocalCommFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt")
    g3MacRtSrcLocalCommFile.setType("SOURCE")

    #### Binary PHY Files ######################################################

    global g3MacRtBinFileCENA
    g3MacRtBinFileCENA = g3MacRtComponent.createLibrarySymbol("MACRT_SOURCE_BIN_G3_CENA", None)
    g3MacRtBinFileCENA.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_CENA.bin")
    g3MacRtBinFileCENA.setOutputName("G3_MAC_RT_CENA.bin")
    g3MacRtBinFileCENA.setDestPath("driver/plc/g3MacRt/bin")
    g3MacRtBinFileCENA.setEnabled(True)

    global g3MacRtBinFileCENB
    g3MacRtBinFileCENB = g3MacRtComponent.createLibrarySymbol("MACRT_SOURCE_BIN_G3_CENB", None)
    g3MacRtBinFileCENB.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_CENB.bin")
    g3MacRtBinFileCENB.setOutputName("G3_MAC_RT_CENB.bin")
    g3MacRtBinFileCENB.setDestPath("driver/plc/g3MacRt/bin")
    g3MacRtBinFileCENB.setEnabled(False)

    global g3MacRtBinFileFCC
    g3MacRtBinFileFCC = g3MacRtComponent.createLibrarySymbol("MACRT_SOURCE_BIN_G3_FCC", None)
    g3MacRtBinFileFCC.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_FCC.bin")
    g3MacRtBinFileFCC.setOutputName("G3_MAC_RT_FCC.bin")
    g3MacRtBinFileFCC.setDestPath("driver/plc/g3MacRt/bin")
    g3MacRtBinFileFCC.setEnabled(False)

    global g3MacRtBinFileARIB
    g3MacRtBinFileARIB = g3MacRtComponent.createLibrarySymbol("MACRT_SOURCE_BIN_G3_ARIB", None)
    g3MacRtBinFileARIB.setSourcePath("driver/g3MacRt/src/bin/G3_MAC_RT_ARIB.bin")
    g3MacRtBinFileARIB.setOutputName("G3_MAC_RT_ARIB.bin")
    g3MacRtBinFileARIB.setDestPath("driver/plc/g3MacRt/bin")
    g3MacRtBinFileARIB.setEnabled(False)

    global g3MacRtAsmBinFile
    g3MacRtAsmBinFile = g3MacRtComponent.createFileSymbol("MACRT_ASM_BIN", None)
    g3MacRtAsmBinFile.setSourcePath("driver/g3MacRt/src/bin/drv_g3_macrt_bin.S.ftl")
    g3MacRtAsmBinFile.setOutputName("drv_g3_macrt_bin.S")
    g3MacRtAsmBinFile.setDestPath("driver/plc/g3MacRt/bin")
    g3MacRtAsmBinFile.setProjectPath("config/" + configName + "/driver/plc/g3MacRt/bin")
    g3MacRtAsmBinFile.setType("SOURCE")
    g3MacRtAsmBinFile.setMarkup(True)

    ##### Coupling Settings : G3  ####################################################

    updBinFilesCtrl = g3MacRtComponent.createBooleanSymbol("DRV_PLC_UPD_BIN_FILES", None)
    updBinFilesCtrl.setLabel("updBinFilesCtrl")
    updBinFilesCtrl.setDescription("Auxiliary control")
    updBinFilesCtrl.setVisible(False)
    updBinFilesCtrl.setDefaultValue(False)

    global plcCoupG3Settings
    plcCoupG3Settings = g3MacRtComponent.createMenuSymbol("DRV_PLC_COUP_G3_SETTING", None)
    plcCoupG3Settings.setLabel("PLC Coupling Settings")
    plcCoupG3Settings.setDescription("Coupling Settings")
    plcCoupG3Settings.setVisible(True)
    plcCoupG3Settings.setHelp(plc_mac_rt_helpkeyword)

    global plcG3Band
    # plcG3Band = g3MacRtComponent.createComboSymbol("DRV_PLC_G3_BAND", plcCoupG3Settings, ["CEN-A", "CEN-B", "FCC", "ARIB"])
    plcG3Band = g3MacRtComponent.createComboSymbol("DRV_PLC_G3_BAND", plcCoupG3Settings, ["CEN-A", "CEN-B", "FCC"])
    plcG3Band.setLabel("Main Branch")
    plcG3Band.setDefaultValue("CEN-A")
    plcG3Band.setHelp(plc_mac_rt_helpkeyword)
    # plcG3Band.setDependencies(resetPlcBand, ["DRV_PLC_MODE"])

    plcCoupG3Internal = g3MacRtComponent.createBooleanSymbol("DRV_PLC_COUP_G3_INTERNAL", plcCoupG3Settings)
    plcCoupG3Internal.setLabel("Internal Driver")
    plcCoupG3Internal.setDescription("Internal Driver")
    plcCoupG3Internal.setVisible(False)
    plcCoupG3Internal.setDefaultValue(False)
    plcCoupG3Internal.setHelp(plc_mac_rt_helpkeyword)
    plcCoupG3Internal.setDependencies(showG3InternalDriver, ["DRV_PLC_G3_BAND"])

    plcCoupGMultiBand = g3MacRtComponent.createBooleanSymbol("DRV_PLC_COUP_G3_MULTIBAND", plcCoupG3Settings)
    plcCoupGMultiBand.setLabel("Multiband")
    plcCoupGMultiBand.setDescription("Multiband")
    plcCoupGMultiBand.setVisible(False)
    plcCoupGMultiBand.setDefaultValue(False)
    plcCoupGMultiBand.setHelp(plc_mac_rt_helpkeyword)
    plcCoupGMultiBand.setDependencies(showG3Multiband, ["DRV_PLC_G3_BAND"])

    plcG3BandAux = g3MacRtComponent.createComboSymbol("DRV_PLC_G3_BAND_AUX", plcCoupGMultiBand, ["CEN-A", "CEN-B"])
    plcG3BandAux.setLabel("Auxiliary Branch")
    plcG3BandAux.setDefaultValue("CEN-A")
    plcG3BandAux.setVisible(False)
    plcG3BandAux.setHelp(plc_mac_rt_helpkeyword)
    plcG3BandAux.setDependencies(showG3AuxBand, ["DRV_PLC_COUP_G3_MULTIBAND"])

    plcG3BandAuxActive = g3MacRtComponent.createBooleanSymbol("DRV_PLC_G3_BAND_AUX_ACTIVE", plcCoupGMultiBand)
    plcG3BandAuxActive.setLabel("Set as default branch")
    plcG3BandAuxActive.setDefaultValue(False)
    plcG3BandAuxActive.setVisible(False)
    plcG3BandAuxActive.setHelp(plc_mac_rt_helpkeyword)
    plcG3BandAuxActive.setDependencies(showG3AuxBand, ["DRV_PLC_COUP_G3_MULTIBAND"])

    plcCoupG3HighAttenuation = g3MacRtComponent.createBooleanSymbol("DRV_PLC_COUP_G3_HIGH_ATTENUATION", plcG3Band)
    plcCoupG3HighAttenuation.setLabel("FCC high attenuation branch")
    plcCoupG3HighAttenuation.setDescription("FCC high attenuation")
    plcCoupG3HighAttenuation.setVisible(False)
    plcCoupG3HighAttenuation.setDefaultValue(False)
    plcCoupG3HighAttenuation.setHelp(plc_mac_rt_helpkeyword)
    plcCoupG3HighAttenuation.setDependencies(showG3HighAttenuation, ["DRV_PLC_G3_BAND", "DRV_PLC_MODE"])

    ##### Coupling Settings : Generic  ####################################################

    global plcBandInUse
    plcBandInUse = g3MacRtComponent.createIntegerSymbol("DRV_PLC_BAND_IN_USE", None)
    plcBandInUse.setLabel("PLC Band in use")
    plcBandInUse.setDefaultValue(PLC_PROFILE_G3_CEN_A)
    plcBandInUse.setVisible(False)
    plcBandInUse.setReadOnly(True)
    plcBandInUse.setHelp(plc_mac_rt_helpkeyword)
    plcBandInUse.setDependencies(updateG3PLCBandInUse, ["DRV_PLC_G3_BAND", "DRV_PLC_G3_BAND_AUX", "DRV_PLC_COUP_G3_MULTIBAND"])

    #### FreeMaker Files ######################################################

    plcSystemDefFile = g3MacRtComponent.createFileSymbol("PLC_DEF", None)
    plcSystemDefFile.setType("STRING")
    plcSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    plcSystemDefFile.setSourcePath("driver/g3MacRt/templates/system/definitions.h.ftl")
    plcSystemDefFile.setMarkup(True)

    plcSymSystemDefObjFile = g3MacRtComponent.createFileSymbol("DRV_PLC_SYSTEM_DEF_OBJECT", None)
    plcSymSystemDefObjFile.setType("STRING")
    plcSymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    plcSymSystemDefObjFile.setSourcePath("driver/g3MacRt/templates/system/definitions_objects.h.ftl")
    plcSymSystemDefObjFile.setMarkup(True)

    plcSymSystemConfigFile = g3MacRtComponent.createFileSymbol("DRV_PLC_CONFIGURATION", None)
    plcSymSystemConfigFile.setType("STRING")
    plcSymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    plcSymSystemConfigFile.setSourcePath("driver/g3MacRt/templates/system/configuration.h.ftl")
    plcSymSystemConfigFile.setMarkup(True)

    plcSymSystemInitDataFile = g3MacRtComponent.createFileSymbol("DRV_PLC_INIT_DATA", None)
    plcSymSystemInitDataFile.setType("STRING")
    plcSymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    plcSymSystemInitDataFile.setSourcePath("driver/g3MacRt/templates/system/initialize_data.c.ftl")
    plcSymSystemInitDataFile.setMarkup(True)

    plcSystemInitFile = g3MacRtComponent.createFileSymbol("PLC_INIT", None)
    plcSystemInitFile.setType("STRING")
    plcSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    plcSystemInitFile.setSourcePath("driver/g3MacRt/templates/system/initialize.c.ftl")
    plcSystemInitFile.setMarkup(True)

    plcSystemTasksFile = g3MacRtComponent.createFileSymbol("DRV_PLC_SYS_TASK", None)
    plcSystemTasksFile.setType("STRING")
    plcSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    plcSystemTasksFile.setSourcePath("driver/g3MacRt/templates/system/system_tasks.c.ftl")
    plcSystemTasksFile.setMarkup(True)

    plcSystemRtosTasksFile = g3MacRtComponent.createFileSymbol("DRV_PLC_SYS_RTOS_TASK", None)
    plcSystemRtosTasksFile.setType("STRING")
    plcSystemRtosTasksFile.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
    plcSystemRtosTasksFile.setSourcePath("driver/g3MacRt/templates/system/system_rtos_tasks.c.ftl")
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

    if connectID == "drvG3MacRt_SPI_dependency" :
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

    if connectID == "drvG3MacRt_SPI_dependency":
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

def destroyComponent(g3MacRtComponent):
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_PORTS", {"isEnabled":False})

    if isDMAPresent:
        Database.sendMessage("HarmonyCore", "ENABLE_SYS_DMA", {"isEnabled":False})
