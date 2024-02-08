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

################################################################################
#### Business Logic ####
################################################################################

rf09Band863OpModes = {"value": "863",
                    "description": "863-870 MHz band. Europe, India",
                    "fskModes": 3, "fskMinChannel": [0, 0, 0], "fskMaxChannel": [68, 34, 34],
                    "ofdmOptions": 1, "ofdmMinChannel": [0], "ofdmMaxChannel": [34]}

rf09Band866OpModes = {"value": "866",
                    "description": "865-867 MHz band",
                    "fskModes": 3, "fskMinChannel": [0, 0, 0], "fskMaxChannel": [18, 9, 9],
                    "ofdmOptions": 1, "ofdmMinChannel": [0], "ofdmMaxChannel": [9]}

rf09Band870OpModes = {"value": "870",
                    "description": "870-876 MHz band. Europe",
                    "fskModes": 3, "fskMinChannel": [0, 0, 0], "fskMaxChannel": [58, 29, 29],
                    "ofdmOptions": 1, "ofdmMinChannel": [0], "ofdmMaxChannel": [29]}

rf09Band915OpModes = {"value": "915",
                    "description": "902-928 MHz band. USA, Canada, Mexico, Colombia",
                    "fskModes": 3, "fskMinChannel": [0, 0, 0], "fskMaxChannel": [128, 63, 63],
                    "ofdmOptions": 4, "ofdmMinChannel": [0, 0, 0, 0], "ofdmMaxChannel": [128, 63, 30, 19]}

rf09Band915aOpModes = {"value": "915A",
                    "description": "902-928 MHz band (alternate). USA, Canada, Mexico",
                    "fskModes": 5, "fskMinChannel": [0, 0, 0, 0, 0], "fskMaxChannel": [128, 128, 128, 63, 63],
                    "ofdmOptions": 4, "ofdmMinChannel": [0, 0, 0, 0], "ofdmMaxChannel": [128, 63, 30, 19]}

rf09Band915bOpModes = {"value": "915B",
                    "description": "902-907.5 & 915-928 MHz band. Brazil",
                    "fskModes": 5, "fskMinChannel": [0, 0, 0, 0, 0], "fskMaxChannel": [128, 128, 128, 63, 63],
                    "fskMaxChannelFirstRange": [26, 26, 26, 12, 12], "fskMinChannelSecondRange": [65, 65, 65, 32, 32],
                    "ofdmOptions": 4, "ofdmMinChannel": [0, 0, 0, 0], "ofdmMaxChannel": [128, 63, 30, 19],
                    "ofdmMaxChannelFirstRange": [26, 12, 5, 3], "ofdmMinChannelSecondRange": [65, 32, 16, 11]}

rf09Band915cOpModes = {"value": "915C",
                    "description": "915-928 MHz band. Argentina, Australia, New Zeland",
                    "fskModes": 5, "fskMinChannel": [65, 65, 65, 32, 32], "fskMaxChannel": [128, 128, 128, 63, 63],
                    "ofdmOptions": 4, "ofdmMinChannel": [65, 32, 16, 11], "ofdmMaxChannel": [128, 63, 30, 19]}

rf09Band919OpModes = {"value": "919",
                    "description": "919-923 MHz band. Malaysia",
                    "fskModes": 5, "fskMinChannel": [85, 85, 85, 42, 42], "fskMaxChannel": [103, 103, 103, 51, 51],
                    "ofdmOptions": 4, "ofdmMinChannel": [85, 42, 21, 14], "ofdmMaxChannel": [103, 51, 24, 16]}

rf09Band920OpModes = {"value": "920",
                    "description": "920-928 MHz band. Japan",
                    "fskModes": 12, "fskMinChannel": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], "fskMaxChannel": [36, 17, 11, 11, 17, 17, 17, 11, 6, 6, 6, 6],
                    "ofdmOptions": 4, "ofdmMinChannel": [0, 0, 0, 0], "ofdmMaxChannel": [38, 18, 8, 4]}

rf09Band920bIndonesiaOpModes = {"value": "920B_INDONESIA",
                    "description": "920-923 MHz band. Indonesia",
                    "fskModes": 5, "fskMinChannel": [90, 90, 90, 45, 45], "fskMaxChannel": [103, 103, 103, 51, 51],
                    "ofdmOptions": 4, "ofdmMinChannel": [90, 45, 22, 15], "ofdmMaxChannel": [103, 51, 24, 16]}

rf09BandsOpModes = {
    "863": rf09Band863OpModes,
    "866": rf09Band866OpModes,
    "870": rf09Band870OpModes,
    "915": rf09Band915OpModes,
    "915-a": rf09Band915aOpModes,
    "915-b": rf09Band915bOpModes,
    "915-c": rf09Band915cOpModes,
    "919": rf09Band919OpModes,
    "920": rf09Band920OpModes,
    "920-b (Indonesia)": rf09Band920bIndonesiaOpModes,
}

rf24Band2450OpModes = {"value": "2450",
                    "description": "2400-2483.5 MHz band",
                    "fskModes": 3, "fskMinChannel": [0, 0, 0], "fskMaxChannel": [415, 206, 206],
                    "ofdmOptions": 4, "ofdmMinChannel": [0, 0, 0, 0], "ofdmMaxChannel": [415, 206, 96, 63]}

rf24BandsOpModes = {
    "2450": rf24Band2450OpModes,
}

rfBandsOpModes = {
    "RF09": rf09BandsOpModes,
    "RF24": rf24BandsOpModes,
}

rf215_helpkeyword = "mcc_h3_rf215_driver_configurations"

def sort_alphanumeric(l):
    import re
    convert = lambda text: int(text) if text.isdigit() else text.lower()
    alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ]
    return sorted(l, key = alphanum_key)

def getSpiFrequency(peripheralClock, maxDLYBCT):
    result_dict = {}

    # Compute minimum number of peripheral clock cycles between LSB last byte to MSB next byte
    cyclesBtwBytesMin = math.ceil(peripheralClock * 125 / 1e9)
    # Compute minimum number of peripheral clock cycles between LSB last byte to LSB next byte
    cyclesByteDurationMin = math.ceil(peripheralClock * 875 / 1e9)
    # Compute needed number of peripheral clock cycles between between CS falling edge to SCLK rising edge. This is directly DLYBS
    result_dict["DLYBS"] = int(math.ceil(peripheralClock * 50 / 1e9))
    # Compute minimum clock divider (maximum SPI CLK frequency)
    freqDivMin = math.ceil(peripheralClock / 25000000)

    if freqDivMin >= cyclesBtwBytesMin:
        # One SPCK cycle at maximum frequency is enough. No additional delay between bytes is needed
        maxDLYBCT = 0
    elif maxDLYBCT > 0:
        # With maximum frequency, additional delay between bytes is needed. Compute that maximum DLYBCT needed. Delay is computed as 32*DLYCBT/FperiphClk
        maxDLYBCT =  math.ceil((cyclesBtwBytesMin - freqDivMin) / 32)

    # Find the best combination of divider/DLYBCT
    cyclesBest = 0xFFFF
    for dlybct in range(0, int(maxDLYBCT) + 1):
        # Number of peripheral clock cycles for this DLYBCT
        cyclesDLYBCT = dlybct * 32

        # Minimum divider allowed for this DLYBCT (1)
        if cyclesBtwBytesMin > (freqDivMin + cyclesDLYBCT):
            freqDiv1 = cyclesBtwBytesMin - cyclesDLYBCT
        else:
            freqDiv1 = freqDivMin

        # Minimum divider allowed for this DLYBCT (1)
        if cyclesByteDurationMin > ((freqDivMin * 8) + cyclesDLYBCT):
            freqDiv2 = math.ceil((cyclesByteDurationMin - cyclesDLYBCT) / 8)
        else:
            freqDiv2 = freqDivMin

        # Minimum divider allowed for this DLYBCT (worts case)
        freqDiv = max(freqDiv1, freqDiv2)

        if (freqDiv < 4) and (dlybct == 0):
            # CPU/DMA need >=32 cycles between bytes (DIV>=4 or DLYBCT>=1)
            freqDiv = 4

        # Total number of peripheral clock cycles for one byte. 8 SPCK cycles + DLYBCT cycles
        totalCycles = freqDiv * 8 + cyclesDLYBCT

        if totalCycles < cyclesBest:
            # This combination is the best one up to now
            result_dict["DLYBCT"] = dlybct
            result_dict["frequency"] = int(round(peripheralClock / freqDiv))

    return result_dict

def configureSpiPlib(localComponent):
    global currentNPCS
    global spiNumNPCS

    plibUsed = localComponent.getSymbolByID("DRV_RF215_PLIB").getValue().lower()

    if plibUsed == '':
        return

    if plibUsed == "srv_spisplit":
        plibUsed = localComponent.getSymbolByID("DRV_RF215_PLIB_SPISPLIT").getValue().lower()

    remoteComponent = Database.getComponentByID(plibUsed)
    if remoteComponent == None:
        return

    if (spiNumNPCS > 0) and (plibUsed.startswith("flexcom") or plibUsed.startswith("spi")):
        if plibUsed.startswith("flexcom"):
            prefix = "FLEXCOM_SPI_"
            peripheralClock = remoteComponent.getSymbolByID("FLEXCOM_SPI_PERIPHERAL_CLOCK").getValue()
        else:
            prefix = "SPI_"
            peripheralClock = remoteComponent.getSymbolByID("SPI_MASTER_CLOCK").getValue()

        # Set NPCSx enabled
        spiSymbol = remoteComponent.getSymbolByID(prefix + "EN_NPCS" + str(currentNPCS))
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(True)
            spiSymbol.setReadOnly(True)

        # Set 8 bits
        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_BITS")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(0)
            spiSymbol.setReadOnly(True)

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
        spiFrequency = getSpiFrequency(peripheralClock, 0xFF)
        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_BAUD_RATE")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(spiFrequency["frequency"])

        # Set DLYBS
        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_DLYBS")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(spiFrequency["DLYBS"])

        # Set DLYBCT
        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_DLYBCT")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(spiFrequency["DLYBCT"])

    elif plibUsed.startswith("usart"):
        # Set 8 bits
        spiSymbol = remoteComponent.getSymbolByID("USART_SPI_BITS_PER_TRANSFER")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(0)
            spiSymbol.setReadOnly(True)

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
        peripheralClock = remoteComponent.getSymbolByID("USART_CLOCK_FREQ").getValue()
        spiFrequency = getSpiFrequency(peripheralClock, 0)
        spiSymbol = remoteComponent.getSymbolByID("USART_SPI_BAUD_RATE")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(spiFrequency["frequency"])

    elif plibUsed.startswith("sercom"):
        # Set 8 bits
        spiSymbol = remoteComponent.getSymbolByID("SPI_CHARSIZE_BITS")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(0)
            spiSymbol.setReadOnly(True)

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
        peripheralClock = Database.getSymbolValue("core", plibUsed.upper() + "_CORE_CLOCK_FREQUENCY")
        spiFrequency = getSpiFrequency(peripheralClock, 0)
        spiSymbol = remoteComponent.getSymbolByID("SPI_BAUD_RATE")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(spiFrequency["frequency"])

    elif plibUsed.startswith("spi"):
        # Set baudrate
        peripheralClock = Database.getSymbolValue("core", plibUsed.upper() + "_CLOCK_FREQUENCY")
        spiFrequency = getSpiFrequency(peripheralClock, 0)
        spiSymbol = remoteComponent.getSymbolByID("SPI_BAUD_RATE")
        if spiSymbol != None:
            spiSymbol.clearValues()
            spiSymbol.setValue(spiFrequency["frequency"])

def deconfigureSpiPlib(localComponent):
    global currentNPCS

    plibUsed = localComponent.getSymbolByID("DRV_RF215_PLIB").getValue().lower()

    if plibUsed == '':
        return

    if plibUsed == "srv_spisplit":
        plibUsed = localComponent.getSymbolByID("DRV_RF215_PLIB_SPISPLIT").getValue().lower()

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

        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_BITS")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_CPOL")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_NCPHA")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_CSAAT")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        spiSymbol = remoteComponent.getSymbolByID(prefix + "CSR" + str(currentNPCS) + "_CSNAAT")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

    elif plibUsed.startswith("usart"):
        # Disable read-only
        spiSymbol = remoteComponent.getSymbolByID("USART_SPI_BITS_PER_TRANSFER")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        spiSymbol = remoteComponent.getSymbolByID("USART_SPI_CLOCK_POLARITY")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        spiSymbol = remoteComponent.getSymbolByID("USART_SPI_CLOCK_PHASE")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

    elif plibUsed.startswith("sercom"):
        # Disable read-only
        spiSymbol = remoteComponent.getSymbolByID("SPI_CHARSIZE_BITS")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        # Set polarity to 0
        spiSymbol = remoteComponent.getSymbolByID("SPI_CLOCK_POLARITY")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

        # Set phase to 0
        spiSymbol = remoteComponent.getSymbolByID("SPI_CLOCK_PHASE")
        if spiSymbol != None:
            spiSymbol.setReadOnly(False)

def npcsChanged(symbol, event):
    global currentNPCS

    localComponent = event["source"]

    deconfigureSpiPlib(localComponent)
    currentNPCS = event["value"]
    configureSpiPlib(localComponent)

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

def showSymbol(symbol, event):
    # Show/hide configuration symbol depending on parent enabled/disabled
    symbol.setVisible(event["value"])

def enableIeeeSunFile(symbol, event):
    # Enable/disable IEEE SUN FSK/OFDM file depending on FSK/OFDM PHY enabled/disabled
    symbol.setEnabled(event["value"])

def numClientsHandler(symbol, event):
    eventID = event["id"]
    eventValue = event["value"]
    numTrx = 0
    if eventValue == True:
        numTrx = 1

    if eventID == "DRV_RF215_TRX09_EN":
        # Check if RF24 is enabled
        rf24En = event["source"].getSymbolByID("DRV_RF215_TRX24_EN")
        if rf24En.getValue() == True:
            numTrx += 1
    elif eventID == "DRV_RF215_TRX24_EN":
        # Check if RF09 is enabled
        rf09En = event["source"].getSymbolByID("DRV_RF215_TRX09_EN")
        if rf09En.getValue() == True:
            numTrx += 1

    if numTrx == 2:
        symbol.setMin(2)
    else:
        symbol.setMin(1)

def numTxBuffersHandler(symbol, event):
    eventID = event["id"]
    eventValue = event["value"]
    if eventID == "DRV_RF215_TXRX_TIME_SUPPORT":
        # Only one buffer if no TX time support
        if eventValue == False:
            symbol.setValue(symbol.getMin())
            symbol.setReadOnly(True)
        else:
            symbol.setReadOnly(False)
            symbol.clearValues()
    else:
        numTrx = 0
        if eventValue == True:
            numTrx = 1

        if eventID == "DRV_RF215_TRX09_EN":
            # Check if RF24 is enabled
            rf24En = event["source"].getSymbolByID("DRV_RF215_TRX24_EN")
            if rf24En.getValue() == True:
                numTrx += 1
        elif eventID == "DRV_RF215_TRX24_EN":
            # Check if RF09 is enabled
            rf09En = event["source"].getSymbolByID("DRV_RF215_TRX09_EN")
            if rf09En.getValue() == True:
                numTrx += 1

        if numTrx == 2:
            symbol.setMin(2)
        else:
            symbol.setMin(1)

def ccaContentionWindowHandler(symbol, event):
    eventID = event["id"]
    eventValue = event["value"]

    if eventID == "DRV_RF215_TXRX_TIME_SUPPORT":
        # Only available if TX time support enabled
        if eventValue == False:
            symbol.setValue(False)
            symbol.setReadOnly(True)
            symbol.setVisible(False)
        else:
            symbol.setVisible(True)
            symbol.setReadOnly(False)
            symbol.clearValues()

def rf09EnHandler(symbol, event):
    eventID = event["id"]
    eventValue = event["value"]
    if eventID == "DRV_RF215_DEVICE":
        if eventValue == "RF215M":
            # Only RF09 available in RF215M
            symbol.setReadOnly(True)
    elif eventID == "DRV_RF215_TRX24_EN":
        # Force to enable at least one TRX
        if eventValue == True:
            symbol.setReadOnly(False)
        else:
            symbol.setReadOnly(True)

def rf24EnHandler(symbol, event):
    eventID = event["id"]
    eventValue = event["value"]
    if eventID == "DRV_RF215_DEVICE":
        if eventValue == "RF215":
            # RF09 and RF24 available in RF215
            symbol.setReadOnly(False)
            symbol.clearValues()
            symbol.setVisible(True)
        elif eventValue == "RF215M":
            # RF24 not available in RF215M
            symbol.setVisible(False)
            symbol.setValue(False)
            symbol.setReadOnly(True)
    elif eventID == "DRV_RF215_TRX09_EN":
        # Force to enable at least one TRX
        if eventValue == True:
            rfDevice = event["source"].getSymbolByID("DRV_RF215_DEVICE")
            if rfDevice.getValue() == "RF215":
                symbol.setReadOnly(False)
        else:
            symbol.setValue(True)
            symbol.setReadOnly(True)

def rfPhyEnHandler(symbol, event):
    # Force to enable at least one PHY
    if event["value"] == False:
        symbol.setReadOnly(True)
    else:
        symbol.setReadOnly(False)

def rfPhyTypeHandler(symbol, event):
    eventID = event["id"]
    eventValue = event["value"]
    if eventID == "DRV_RF215_FSK_EN":
        if eventValue == False:
            symbol.setSelectedKey("OFDM")
            symbol.setReadOnly(True)
        else:
            phyOfdmEn = event["source"].getSymbolByID("DRV_RF215_OFDM_EN")
            if phyOfdmEn.getValue() == True:
                symbol.setReadOnly(False)
    elif eventID == "DRV_RF215_OFDM_EN":
        if eventValue == False:
            symbol.setSelectedKey("FSK")
            symbol.setReadOnly(True)
        else:
            phyFskEn = event["source"].getSymbolByID("DRV_RF215_FSK_EN")
            if phyFskEn.getValue() == True:
                symbol.setReadOnly(False)
    elif (eventID == "DRV_RF215_TRX09_EN") or (eventID == "DRV_RF215_TRX24_EN"):
        # Show/hide symbol depending on RF09/RF24 enabled
        symbol.setVisible(eventValue)

def updateOpModeRange(symbol, phyBand, phyType, rfTrx):
    dictBand = rfBandsOpModes[rfTrx][phyBand]
    if phyType == "FSK":
        symbol.setLabel("Default FSK Operating Mode   ")
        if rfTrx == "RF09":
            symbol.setDescription("Select the default FSK Operating Mode for RF215 Sub-GHz Transceiver (RF09)")
        else:
            symbol.setDescription("Select the default FSK Operating Mode for RF215 2.4 GHz Transceiver (RF24)")
        symbol.setMin(1)
        symbol.setMax(dictBand["fskModes"])
    elif phyType == "OFDM":
        symbol.setLabel("Default OFDM Bandwidth Option")
        if rfTrx == "RF09":
            symbol.setDescription("Select the default OFDM Bandwidth Option for RF215 Sub-GHz Transceiver (RF09)")
        else:
            symbol.setDescription("Select the default OFDM Bandwidth Option for RF215 2.4 GHz Transceiver (RF24)")
        symbol.setMax(4)
        symbol.setMin(5 - dictBand["ofdmOptions"])

def rf09PhyOpModeHandler(symbol, event):
    eventID = event["id"]
    eventValue = event["value"]
    eventSymbol = event["symbol"]
    if eventID == "DRV_RF215_TRX09_PHY_BAND":
        phyBand = eventSymbol.getSelectedKey()
        phyType = event["source"].getSymbolByID("DRV_RF215_TRX09_PHY_TYPE").getSelectedKey()
        updateOpModeRange(symbol, phyBand, phyType, "RF09")
    elif eventID == "DRV_RF215_TRX09_PHY_TYPE":
        phyType = eventSymbol.getSelectedKey()
        phyBand = event["source"].getSymbolByID("DRV_RF215_TRX09_PHY_BAND").getSelectedKey()
        updateOpModeRange(symbol, phyBand, phyType, "RF09")
    elif eventID == "DRV_RF215_TRX09_EN":
        # Show/hide symbol depending on RF09 enabled
        symbol.setVisible(eventValue)

def rf24PhyOpModeHandler(symbol, event):
    eventID = event["id"]
    eventValue = event["value"]
    eventSymbol = event["symbol"]
    if eventID == "DRV_RF215_TRX24_PHY_BAND":
        phyBand = eventSymbol.getSelectedKey()
        phyType = event["source"].getSymbolByID("DRV_RF215_TRX24_PHY_TYPE").getSelectedKey()
        updateOpModeRange(symbol, phyBand, phyType, "RF24")
    elif eventID == "DRV_RF215_TRX24_PHY_TYPE":
        phyType = eventSymbol.getSelectedKey()
        phyBand = event["source"].getSymbolByID("DRV_RF215_TRX24_PHY_BAND").getSelectedKey()
        updateOpModeRange(symbol, phyBand, phyType, "RF24")
    elif eventID == "DRV_RF215_TRX24_EN":
        # Show/hide symbol depending on RF24 enabled
        symbol.setVisible(eventValue)

def updateChnNumRange(symbol, phyBand, phyType, phyOpMode, rfTrx):
    dictBand = rfBandsOpModes[rfTrx][phyBand]
    if phyType == "FSK":
        if (phyOpMode >= 1) and (phyOpMode <= dictBand["fskModes"]):
            symbol.setMin(dictBand["fskMinChannel"][phyOpMode - 1])
            symbol.setMax(dictBand["fskMaxChannel"][phyOpMode - 1])
    elif phyType == "OFDM":
        if (phyOpMode <= 4) and (phyOpMode >= (5 - dictBand["ofdmOptions"])):
            symbol.setMin(dictBand["ofdmMinChannel"][4 - phyOpMode])
            symbol.setMax(dictBand["ofdmMaxChannel"][4 - phyOpMode])

def rf09PhyChnNumHandler(symbol, event):
    eventID = event["id"]
    eventValue = event["value"]
    eventSymbol = event["symbol"]
    eventSource = event["source"]
    if eventID == "DRV_RF215_TRX09_PHY_BAND":
        phyBand = eventSymbol.getSelectedKey()
        phyType = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_TYPE").getSelectedKey()
        phyOpMode = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_OPM").getValue()
        updateChnNumRange(symbol, phyBand, phyType, phyOpMode, "RF09")
    elif eventID == "DRV_RF215_TRX09_PHY_TYPE":
        phyType = eventSymbol.getSelectedKey()
        phyBand = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_BAND").getSelectedKey()
        phyOpMode = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_OPM").getValue()
        updateChnNumRange(symbol, phyBand, phyType, phyOpMode, "RF09")
    elif eventID == "DRV_RF215_TRX09_PHY_OPM":
        phyOpMode = eventValue
        phyBand = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_BAND").getSelectedKey()
        phyType = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_TYPE").getSelectedKey()
        updateChnNumRange(symbol, phyBand, phyType, phyOpMode, "RF09")
    elif eventID == "DRV_RF215_TRX09_EN":
        # Show/hide symbol depending on RF09 enabled
        symbol.setVisible(eventValue)

def rf24PhyChnNumHandler(symbol, event):
    eventID = event["id"]
    eventValue = event["value"]
    eventSymbol = event["symbol"]
    eventSource = event["source"]
    if eventID == "DRV_RF215_TRX24_PHY_BAND":
        phyBand = eventSymbol.getSelectedKey()
        phyType = eventSource.getSymbolByID("DRV_RF215_TRX24_PHY_TYPE").getSelectedKey()
        phyOpMode = eventSource.getSymbolByID("DRV_RF215_TRX24_PHY_OPM").getValue()
        updateChnNumRange(symbol, phyBand, phyType, phyOpMode, "RF24")
    elif eventID == "DRV_RF215_TRX24_PHY_TYPE":
        phyType = eventSymbol.getSelectedKey()
        phyBand = eventSource.getSymbolByID("DRV_RF215_TRX24_PHY_BAND").getSelectedKey()
        phyOpMode = eventSource.getSymbolByID("DRV_RF215_TRX24_PHY_OPM").getValue()
        updateChnNumRange(symbol, phyBand, phyType, phyOpMode, "RF24")
    elif eventID == "DRV_RF215_TRX24_PHY_OPM":
        phyOpMode = eventValue
        phyBand = eventSource.getSymbolByID("DRV_RF215_TRX24_PHY_BAND").getSelectedKey()
        phyType = eventSource.getSymbolByID("DRV_RF215_TRX24_PHY_TYPE").getSelectedKey()
        updateChnNumRange(symbol, phyBand, phyType, phyOpMode, "RF24")
    elif eventID == "DRV_RF215_TRX24_EN":
        # Show/hide symbol depending on RF24 enabled
        symbol.setVisible(eventValue)

def updateChnNumComment(symbol, phyBand, phyType, phyOpMode, phyChnNum, rfTrx):
    dictBand = rfBandsOpModes[rfTrx][phyBand]
    if phyType == "FSK":
        if (phyOpMode >= 1) and (phyOpMode <= dictBand["fskModes"]):
            if "fskMaxChannelFirstRange" in dictBand.keys():
                maxChannelFirstRange = dictBand["fskMaxChannelFirstRange"][phyOpMode - 1]
                minChannelSecondRange = dictBand["fskMinChannelSecondRange"][phyOpMode - 1]
                if (phyChnNum > maxChannelFirstRange) and (phyChnNum < minChannelSecondRange):
                    symbol.setLabel("!!! Channel numbers from %d to %d are not allowed !!!"%(maxChannelFirstRange+1, minChannelSecondRange-1))
                    symbol.setVisible(True)
                else:
                    symbol.setVisible(False)
            else:
                symbol.setVisible(False)
    elif phyType == "OFDM":
        if (phyOpMode <= 4) and (phyOpMode >= (5 - dictBand["ofdmOptions"])):
            if "ofdmMaxChannelFirstRange" in dictBand.keys():
                maxChannelFirstRange = dictBand["ofdmMaxChannelFirstRange"][4 - phyOpMode]
                minChannelSecondRange = dictBand["ofdmMinChannelSecondRange"][4 - phyOpMode]
                if (phyChnNum > maxChannelFirstRange) and (phyChnNum < minChannelSecondRange):
                    symbol.setLabel("!!! Channel numbers from %d to %d are not allowed !!!"%(maxChannelFirstRange+1, minChannelSecondRange-1))
                    symbol.setVisible(True)
                else:
                    symbol.setVisible(False)
            else:
                symbol.setVisible(False)

def rf09PhyChnNumCommentHandler(symbol, event):
    eventID = event["id"]
    eventValue = event["value"]
    eventSymbol = event["symbol"]
    eventSource = event["source"]
    if eventID == "DRV_RF215_TRX09_PHY_BAND":
        phyBand = eventSymbol.getSelectedKey()
        phyType = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_TYPE").getSelectedKey()
        phyOpMode = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_OPM").getValue()
        phyChnNum = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_CHANNEL").getValue()
        updateChnNumComment(symbol, phyBand, phyType, phyOpMode, phyChnNum, "RF09")
    elif eventID == "DRV_RF215_TRX09_PHY_TYPE":
        phyType = eventSymbol.getSelectedKey()
        phyBand = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_BAND").getSelectedKey()
        phyOpMode = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_OPM").getValue()
        phyChnNum = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_CHANNEL").getValue()
        updateChnNumComment(symbol, phyBand, phyType, phyOpMode, phyChnNum, "RF09")
    elif eventID == "DRV_RF215_TRX09_PHY_OPM":
        phyOpMode = eventValue
        phyBand = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_BAND").getSelectedKey()
        phyType = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_TYPE").getSelectedKey()
        phyChnNum = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_CHANNEL").getValue()
        updateChnNumComment(symbol, phyBand, phyType, phyOpMode, phyChnNum, "RF09")
    elif eventID == "DRV_RF215_TRX09_PHY_CHANNEL":
        phyChnNum = eventValue
        phyBand = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_BAND").getSelectedKey()
        phyType = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_TYPE").getSelectedKey()
        phyOpMode = eventSource.getSymbolByID("DRV_RF215_TRX09_PHY_OPM").getValue()
        updateChnNumComment(symbol, phyBand, phyType, phyOpMode, phyChnNum, "RF09")
    elif eventID == "DRV_RF215_TRX09_EN":
        # Hide comment if RF09 disabled
        if eventValue == False:
            symbol.setVisible(False)

################################################################################
#### Component ####
################################################################################

def instantiateComponent(rf215Component):
    # Activate needed modules
    Database.activateComponents(["HarmonyCore", "sys_time"])

    global isDMAPresent
    if Database.getSymbolValue("core", "DMA_ENABLE") != None:
        # DMA is present (XDMAC for SAME70)
        isDMAPresent = True
    else:
        # DMA is not present (PDC is used for SAMG55/PIC32CXMT)
        isDMAPresent = False

    rfDevice = rf215Component.createComboSymbol("DRV_RF215_DEVICE", None, ["RF215", "RF215M"])
    rfDevice.setLabel("RF Device Used")
    rfDevice.setDefaultValue("RF215")
    rfDevice.setHelp(rf215_helpkeyword)

    numClients = rf215Component.createIntegerSymbol("DRV_RF215_NUM_CLIENTS", None)
    numClients.setLabel("Number of clients")
    numClients.setDescription("Maximum number of clients (each single client for a single transceiver)")
    numClients.setDefaultValue(1)
    numClients.setMin(1)
    numClients.setMax(16)
    numClients.setDependencies(numClientsHandler, ["DRV_RF215_TRX09_EN", "DRV_RF215_TRX24_EN"])
    numClients.setHelp(rf215_helpkeyword)

    numTxBuffers = rf215Component.createIntegerSymbol("DRV_RF215_NUM_TX_BUFFERS", None)
    numTxBuffers.setLabel("Number of TX Buffers")
    numTxBuffers.setDescription("Select the maximum PSDU data length, including FCS")
    numTxBuffers.setDefaultValue(1)
    numTxBuffers.setMin(1)
    numTxBuffers.setMax(16)
    numTxBuffers.setDependencies(numTxBuffersHandler, ["DRV_RF215_TRX09_EN", "DRV_RF215_TRX24_EN", "DRV_RF215_TXRX_TIME_SUPPORT"])
    numTxBuffers.setHelp(rf215_helpkeyword)

    fcsMode = rf215Component.createKeyValueSetSymbol("DRV_RF215_FCS_MODE", None)
    fcsMode.setLabel("Automatic FCS (Frame Check Sequence)")
    fcsMode.setDescription("Select the automatic FCS mode for PSDU TX and RX")
    fcsMode.setOutputMode("Value")
    fcsMode.setDisplayMode("Key")
    fcsMode.setDefaultValue(2)
    fcsMode.addKey("Disabled", "0", "Automatic FCS disabled")
    fcsMode.addKey("16-bit FCS", "2", "Automatic 16-bit FCS TX and RX")
    fcsMode.addKey("32-bit FCS", "4", "Automatic 32-bit FCS TX and RX")
    fcsMode.setHelp(rf215_helpkeyword)

    maxPsduLen = rf215Component.createIntegerSymbol("DRV_RF215_MAX_PSDU_LEN", None)
    maxPsduLen.setLabel("Maximum PSDU data length")
    maxPsduLen.setDescription("Select the maximum PSDU data length, including FCS")
    maxPsduLen.setDefaultValue(576)
    maxPsduLen.setMin(8)
    maxPsduLen.setMax(2047)
    maxPsduLen.setHelp(rf215_helpkeyword)

    txrxTimeSupport = rf215Component.createBooleanSymbol("DRV_RF215_TXRX_TIME_SUPPORT", None)
    txrxTimeSupport.setLabel("PHY TX/RX time support")
    txrxTimeSupport.setDescription("Enable/disable support for PHY TX/RX time. If enabled, RX time is provided, and it is allowed to schedule TX for a specific time.")
    txrxTimeSupport.setDefaultValue(True)
    txrxTimeSupport.setHelp(rf215_helpkeyword)

    ccaContentionWindow = rf215Component.createBooleanSymbol("DRV_RF215_CCA_CONTENTION_WINDOW", txrxTimeSupport)
    ccaContentionWindow.setLabel("CCA Contention Window support")
    ccaContentionWindow.setDescription("Enable/disable support for CCA (Clear Channel Assessment) Contention Window.")
    ccaContentionWindow.setDefaultValue(False)
    ccaContentionWindow.setDependencies(ccaContentionWindowHandler, ["DRV_RF215_TXRX_TIME_SUPPORT"])
    ccaContentionWindow.setHelp(rf215_helpkeyword)

    freqHoppingSupport = rf215Component.createBooleanSymbol("DRV_RF215_FREQ_HOPPING_SUPPORT", txrxTimeSupport)
    freqHoppingSupport.setLabel("Frequency hopping support")
    freqHoppingSupport.setDescription("Enable/disable support for frequency hopping. If enabled, channel is TX parameter and it is possible to schedule RX channel switch.")
    freqHoppingSupport.setDefaultValue(False)
    freqHoppingSupport.setDependencies(showSymbol, ["DRV_RF215_TXRX_TIME_SUPPORT"])
    freqHoppingSupport.setHelp(rf215_helpkeyword)

    txTimeError = rf215Component.createIntegerSymbol("DRV_RF215_TX_TIME_ERROR", txrxTimeSupport)
    txTimeError.setLabel("Max. TX time delay error (us)")
    txTimeError.setDescription("Select the maximum TX time delay error allowed in microseconds.")
    txTimeError.setDefaultValue(9000)
    txTimeError.setMin(4)
    txTimeError.setMax(20000)
    txTimeError.setDependencies(showSymbol, ["DRV_RF215_TXRX_TIME_SUPPORT"])
    txTimeError.setHelp(rf215_helpkeyword)

    # RTOS CONFIG
    rf215RTOSMenu = rf215Component.createMenuSymbol("DRV_RF215_RTOS_MENU", None)
    rf215RTOSMenu.setLabel("RTOS settings")
    rf215RTOSMenu.setDescription("RTOS settings")
    rf215RTOSMenu.setVisible(getActiveRtos() != "BareMetal")
    rf215RTOSMenu.setDependencies(showRTOSMenu, ["HarmonyCore.SELECT_RTOS"])
    rf215RTOSMenu.setHelp(rf215_helpkeyword)

    rf215RTOSStackSize = rf215Component.createIntegerSymbol("DRV_RF215_RTOS_STACK_SIZE", rf215RTOSMenu)
    rf215RTOSStackSize.setLabel("Stack Size (in bytes)")
    rf215RTOSStackSize.setDefaultValue(1792)
    rf215RTOSStackSize.setMin(512)
    rf215RTOSStackSize.setMax(16*1024)
    rf215RTOSStackSize.setHelp(rf215_helpkeyword)

    rf215RTOSTaskPriority = rf215Component.createIntegerSymbol("DRV_RF215_RTOS_TASK_PRIORITY", rf215RTOSMenu)
    rf215RTOSTaskPriority.setLabel("Task Priority")
    rf215RTOSTaskPriority.setDefaultValue(1)
    rf215RTOSTaskPriority.setMin(0)
    rf215RTOSTaskPriority.setHelp(rf215_helpkeyword)

    rf215RTOSMsgQSize = rf215Component.createIntegerSymbol("DRV_RF215_RTOS_TASK_MSG_QTY", rf215RTOSMenu)
    rf215RTOSMsgQSize.setLabel("Maximum Message Queue Size")
    rf215RTOSMsgQSize.setDescription("A µC/OS-III task contains an optional internal message queue (if OS_CFG_TASK_Q_EN is set to DEF_ENABLED in os_cfg.h). This argument specifies the maximum number of messages that the task can receive through this message queue. The user may specify that the task is unable to receive messages by setting this argument to 0")
    rf215RTOSMsgQSize.setDefaultValue(0)
    rf215RTOSMsgQSize.setMin(0)
    rf215RTOSMsgQSize.setVisible(getActiveRtos() == "MicriumOSIII")
    rf215RTOSMsgQSize.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])
    rf215RTOSMsgQSize.setHelp(rf215_helpkeyword)

    rf215RTOSTaskTimeQuanta = rf215Component.createIntegerSymbol("DRV_RF215_RTOS_TASK_TIME_QUANTA", rf215RTOSMenu)
    rf215RTOSTaskTimeQuanta.setLabel("Task Time Quanta")
    rf215RTOSTaskTimeQuanta.setDescription("The amount of time (in clock ticks) for the time quanta when Round Robin is enabled. If you specify 0, then the default time quanta will be used which is the tick rate divided by 10.")
    rf215RTOSTaskTimeQuanta.setDefaultValue(0)
    rf215RTOSTaskTimeQuanta.setMin(0)
    rf215RTOSTaskTimeQuanta.setVisible(getActiveRtos() == "MicriumOSIII")
    rf215RTOSTaskTimeQuanta.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])
    rf215RTOSTaskTimeQuanta.setHelp(rf215_helpkeyword)

    rf215RTOSTaskSpecificOpt = rf215Component.createBooleanSymbol("DRV_RF215_RTOS_TASK_OPT_NONE", rf215RTOSMenu)
    rf215RTOSTaskSpecificOpt.setLabel("Task Specific Options")
    rf215RTOSTaskSpecificOpt.setDescription("Contains task-specific options. Each option consists of one bit. The option is selected when the bit is set. The current version of µC/OS-III supports the following options:")
    rf215RTOSTaskSpecificOpt.setDefaultValue(True)
    rf215RTOSTaskSpecificOpt.setVisible(getActiveRtos() == "MicriumOSIII")
    rf215RTOSTaskSpecificOpt.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])
    rf215RTOSTaskSpecificOpt.setHelp(rf215_helpkeyword)

    rf215RTOSTaskStkChk = rf215Component.createBooleanSymbol("DRV_RF215_RTOS_TASK_OPT_STK_CHK", rf215RTOSTaskSpecificOpt)
    rf215RTOSTaskStkChk.setLabel("Stack checking is allowed for the task")
    rf215RTOSTaskStkChk.setDescription("Specifies whether stack checking is allowed for the task")
    rf215RTOSTaskStkChk.setDefaultValue(True)
    rf215RTOSTaskStkChk.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_RF215_RTOS_TASK_OPT_NONE"])
    rf215RTOSTaskStkChk.setHelp(rf215_helpkeyword)

    rf215RTOSTaskStkClr = rf215Component.createBooleanSymbol("DRV_RF215_RTOS_TASK_OPT_STK_CLR", rf215RTOSTaskSpecificOpt)
    rf215RTOSTaskStkClr.setLabel("Stack needs to be cleared")
    rf215RTOSTaskStkClr.setDescription("Specifies whether the stack needs to be cleared")
    rf215RTOSTaskStkClr.setDefaultValue(True)
    rf215RTOSTaskStkClr.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_RF215_RTOS_TASK_OPT_NONE"])
    rf215RTOSTaskStkClr.setHelp(rf215_helpkeyword)

    rf215RTOSTaskSaveFp = rf215Component.createBooleanSymbol("DRV_RF215_RTOS_TASK_OPT_SAVE_FP", rf215RTOSTaskSpecificOpt)
    rf215RTOSTaskSaveFp.setLabel("Floating-point registers needs to be saved")
    rf215RTOSTaskSaveFp.setDescription("Specifies whether floating-point registers are saved. This option is only valid if the processor has floating-point hardware and the processor-specific code saves the floating-point registers")
    rf215RTOSTaskSaveFp.setDefaultValue(False)
    rf215RTOSTaskSaveFp.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_RF215_RTOS_TASK_OPT_NONE"])
    rf215RTOSTaskSaveFp.setHelp(rf215_helpkeyword)

    rf215RTOSTaskNoTls = rf215Component.createBooleanSymbol("DRV_RF215_RTOS_TASK_OPT_NO_TLS", rf215RTOSTaskSpecificOpt)
    rf215RTOSTaskNoTls.setLabel("TLS (Thread Local Storage) support needed for the task")
    rf215RTOSTaskNoTls.setDescription("If the caller doesn’t want or need TLS (Thread Local Storage) support for the task being created. If you do not include this option, TLS will be supported by default. TLS support was added in V3.03.00")
    rf215RTOSTaskNoTls.setDefaultValue(False)
    rf215RTOSTaskNoTls.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_RF215_RTOS_TASK_OPT_NONE"])
    rf215RTOSTaskNoTls.setHelp(rf215_helpkeyword)

    plibUsed = rf215Component.createStringSymbol("DRV_RF215_PLIB", None)
    plibUsed.setLabel("PLIB Used")
    plibUsed.setDescription("PLIB connected to SPI dependency")
    plibUsed.setReadOnly(True)
    plibUsed.setHelp(rf215_helpkeyword)

    global plibUsedSpiSplit
    plibUsedSpiSplit = rf215Component.createStringSymbol("DRV_RF215_PLIB_SPISPLIT", None)
    plibUsedSpiSplit.setLabel("PLIB Used by SPI Splitter")
    plibUsedSpiSplit.setDescription("PLIB connected to SPI dependency in SPI Splitter")
    plibUsedSpiSplit.setReadOnly(True)
    plibUsedSpiSplit.setVisible(False)
    plibUsedSpiSplit.setHelp(rf215_helpkeyword)

    global spiDependencyComment
    spiDependencyComment = rf215Component.createCommentSymbol("DRV_RF215_SPI_DEPENDENCY_COMMENT", None)
    spiDependencyComment.setLabel("!!! Satisfy SPI Dependency !!!")

    global dmaChannelCount
    global dmaTxChannel
    global dmaTxChannelComment
    global dmaRxChannel
    global dmaRxChannelComment
    if isDMAPresent:
        dmaTxChannel = rf215Component.createIntegerSymbol("DRV_RF215_TX_DMA_CHANNEL", None)
        dmaTxChannel.setLabel("DMA Channel For Transmit")
        dmaTxChannel.setDescription("Allocated DMA channel for SPI Transmit")
        dmaTxChannel.setMin(-1)
        dmaTxChannel.setReadOnly(True)
        dmaTxChannel.setVisible(False)
        dmaTxChannel.setHelp(rf215_helpkeyword)

        dmaTxChannelComment = rf215Component.createCommentSymbol("DRV_RF215_TX_DMA_CH_COMMENT", None)
        dmaTxChannelComment.setLabel("!!! Couldn't Allocate DMA Channel for Transmit. Check DMA manager !!!")
        dmaTxChannelComment.setVisible(False)

        dmaRxChannel = rf215Component.createIntegerSymbol("DRV_RF215_RX_DMA_CHANNEL", None)
        dmaRxChannel.setLabel("DMA Channel For Receive")
        dmaRxChannel.setDescription("Allocated DMA channel for SPI Receive")
        dmaRxChannel.setMin(-1)
        dmaRxChannel.setReadOnly(True)
        dmaRxChannel.setVisible(False)
        dmaRxChannel.setHelp(rf215_helpkeyword)

        dmaRxChannelComment = rf215Component.createCommentSymbol("DRV_RF215_RX_DMA_CH_COMMENT", None)
        dmaRxChannelComment.setLabel("!!! Couldn't Allocate DMA Channel for Receive. Check DMA manager !!!")
        dmaRxChannelComment.setVisible(False)

        # Get number of DMA channels
        dmaChannelCount = Database.getSymbolValue("core", "DMA_CHANNEL_COUNT")
        if dmaChannelCount != None:
            dmaTxChannel.setMax(dmaChannelCount - 1)
            dmaRxChannel.setMax(dmaChannelCount - 1)

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
        spiNpcsUsed = rf215Component.createKeyValueSetSymbol("DRV_RF215_SPI_NPCS", None)
        spiNpcsUsed.setLabel("SPI NPCS Used")
        spiNpcsUsed.setDescription("Select SPI NPCS (peripheral chip select) connected to RF Device")
        spiNpcsUsed.setOutputMode("Value")
        spiNpcsUsed.setDisplayMode("Key")
        spiNpcsUsed.setDefaultValue(0)
        spiNpcsUsed.setVisible(False)
        spiNpcsUsed.setHelp(rf215_helpkeyword)

        currentNPCS = 0

        for npcs in range(0, spiNumNPCS):
            spiNpcsUsed.addKey("NPCS" + str(npcs), str(npcs), "SPI NPCS" + str(npcs) + " used by RF215 Driver")

    global spiNumCSR
    spiNumCSR = rf215Component.createIntegerSymbol("DRV_RF215_SPI_NUM_CSR", None)
    spiNumCSR.setVisible(False)
    spiNumCSR.setDependencies(npcsChanged, ["DRV_RF215_SPI_NPCS"])

    global plibConfigComment
    plibConfigComment = rf215Component.createCommentSymbol("DRV_RF215_PLIB_CONFIG_COMMENT", None)
    plibConfigComment.setVisible(False)

    spiPinsComment = rf215Component.createCommentSymbol("DRV_RF215_SPI_PINS_COMMENT", None)
    spiPinsComment.setLabel("***SPI pins must be properly configured in Pin Manager***")

    pinExtInt = rf215Component.createKeyValueSetSymbol("DRV_RF215_EXT_INT_PIN", None)
    pinExtInt.setLabel("External Interrupt Pin")
    pinExtInt.setDescription("Select pin connected to RF device's IRQ pin. Configure in Pin Manager as Input")
    pinExtInt.setDefaultValue(0)
    pinExtInt.setOutputMode("Key")
    pinExtInt.setDisplayMode("Description")
    pinExtInt.setHelp(rf215_helpkeyword)

    pinReset = rf215Component.createKeyValueSetSymbol("DRV_RF215_RESET_PIN", None)
    pinReset.setLabel("Reset Pin")
    pinReset.setDescription("Select pin connected to RF device's RSTN pin. Configure in Pin Manager as Output Low")
    pinReset.setDefaultValue(0)
    pinReset.setOutputMode("Key")
    pinReset.setDisplayMode("Description")
    pinReset.setHelp(rf215_helpkeyword)

    useLedTxPin = rf215Component.createBooleanSymbol("DRV_RF215_USE_LED_TX", None)
    useLedTxPin.setLabel("Use LED TX Pin?")
    useLedTxPin.setDescription("Enable/diable pin to indicate RX in progress")
    useLedTxPin.setDefaultValue(False)
    useLedTxPin.setHelp(rf215_helpkeyword)

    pinLedTx = rf215Component.createKeyValueSetSymbol("DRV_RF215_LED_TX_PIN", useLedTxPin)
    pinLedTx.setLabel("LED TX Pin")
    pinLedTx.setDescription("Select pin used to indicate TX in progress. Configure in Pin Manager as Output Low")
    pinLedTx.setDefaultValue(0)
    pinLedTx.setOutputMode("Key")
    pinLedTx.setDisplayMode("Description")
    pinLedTx.setVisible(False)
    pinLedTx.setDependencies(showSymbol, ["DRV_RF215_USE_LED_TX"])
    pinLedTx.setHelp(rf215_helpkeyword)

    useLedRxPin = rf215Component.createBooleanSymbol("DRV_RF215_USE_LED_RX", None)
    useLedRxPin.setLabel("Use LED RX Pin?")
    useLedRxPin.setDescription("Enable/diable pin to indicate RX in progress")
    useLedRxPin.setDefaultValue(False)
    useLedRxPin.setHelp(rf215_helpkeyword)

    pinLedRx = rf215Component.createKeyValueSetSymbol("DRV_RF215_LED_RX_PIN", useLedRxPin)
    pinLedRx.setLabel("LED RX Pin")
    pinLedRx.setDescription("Select pin used to indicate RX in progress. Configure in Pin Manager as Output Low")
    pinLedRx.setDefaultValue(0)
    pinLedRx.setOutputMode("Key")
    pinLedRx.setDisplayMode("Description")
    pinLedRx.setVisible(False)
    pinLedRx.setDependencies(showSymbol, ["DRV_RF215_USE_LED_RX"])
    pinLedRx.setHelp(rf215_helpkeyword)

    # Send message to core to get available pins
    availablePinDictionary = {}
    availablePinDictionary = Database.sendMessage("core", "PIN_LIST", availablePinDictionary)

    for pad in sort_alphanumeric(availablePinDictionary.values()):
        key = "SYS_PORT_PIN_" + pad
        value = list(availablePinDictionary.keys())[list(availablePinDictionary.values()).index(pad)]
        description = pad
        pinExtInt.addKey(key, value, description)
        pinReset.addKey(key, value, description)
        pinLedTx.addKey(key, value, description)
        pinLedRx.addKey(key, value, description)

    rf215PinsComment = rf215Component.createCommentSymbol("DRV_RF215_PINS_CONFIG_COMMENT", None)
    rf215PinsComment.setLabel("***Above selected pins must be properly configured in Pin Manager***")

    phyFskEn = rf215Component.createBooleanSymbol("DRV_RF215_FSK_EN", None)
    phyFskEn.setLabel("Use FSK PHY?")
    phyFskEn.setDescription("Enable/disable 802.15.4 SUN FSK PHY")
    phyFskEn.setDefaultValue(True)
    phyFskEn.setDependencies(rfPhyEnHandler, ["DRV_RF215_OFDM_EN"])
    phyFskEn.setHelp(rf215_helpkeyword)

    phyOfdmEn = rf215Component.createBooleanSymbol("DRV_RF215_OFDM_EN", None)
    phyOfdmEn.setLabel("Use OFDM PHY?")
    phyOfdmEn.setDescription("Enable/disable 802.15.4 SUN OFDM PHY")
    phyOfdmEn.setDefaultValue(True)
    phyOfdmEn.setDependencies(rfPhyEnHandler, ["DRV_RF215_FSK_EN"])
    phyOfdmEn.setHelp(rf215_helpkeyword)

    rf09En = rf215Component.createBooleanSymbol("DRV_RF215_TRX09_EN", None)
    rf09En.setLabel("Use Sub-GHz Transceiver?")
    rf09En.setDescription("Enable/disable RF215 Sub-GHz Transceiver (RF09)")
    rf09En.setDefaultValue(True)
    rf09En.setReadOnly(True)
    rf09En.setDependencies(rf09EnHandler, ["DRV_RF215_DEVICE", "DRV_RF215_TRX24_EN"])
    rf09En.setHelp(rf215_helpkeyword)

    rf09Band = rf215Component.createKeyValueSetSymbol("DRV_RF215_TRX09_PHY_BAND", rf09En)
    rf09Band.setLabel("Default PHY Frequency Band")
    rf09Band.setDescription("Select the default PHY frequency band for RF215 Sub-GHz Transceiver (RF09)")
    rf09Band.setOutputMode("Value")
    rf09Band.setDisplayMode("Key")
    rf09Band.setDefaultValue(0)
    rf09Band.setDependencies(showSymbol, ["DRV_RF215_TRX09_EN"])
    rf09Band.setHelp(rf215_helpkeyword)
    for keyBand in sort_alphanumeric(rf09BandsOpModes.keys()):
        dictBand = rf09BandsOpModes[keyBand]
        rf09Band.addKey(keyBand, dictBand["value"], dictBand["description"])

    rf09PhyType = rf215Component.createKeyValueSetSymbol("DRV_RF215_TRX09_PHY_TYPE", rf09En)
    rf09PhyType.setLabel("Default PHY Type")
    rf09PhyType.setDescription("Select the default PHY type for RF215 Sub-GHz Transceiver (RF09)")
    rf09PhyType.setOutputMode("Key")
    rf09PhyType.setDisplayMode("Key")
    rf09PhyType.setDefaultValue(0)
    rf09PhyType.setDependencies(rfPhyTypeHandler, ["DRV_RF215_TRX09_EN", "DRV_RF215_FSK_EN", "DRV_RF215_OFDM_EN"])
    rf09PhyType.addKey("FSK", "FSK", "FSK")
    rf09PhyType.addKey("OFDM", "OFDM", "OFDM")
    rf09PhyType.setHelp(rf215_helpkeyword)

    rf09PhyOpMode = rf215Component.createIntegerSymbol("DRV_RF215_TRX09_PHY_OPM", rf09En)
    rf09PhyOpMode.setLabel("Default FSK Operating Mode")
    rf09PhyOpMode.setDescription("Select the default FSK Operating Mode for RF215 Sub-GHz Transceiver (RF09)")
    rf09PhyOpMode.setDefaultValue(1)
    rf09PhyOpMode.setMin(1)
    rf09PhyOpMode.setMax(rf09BandsOpModes[rf09Band.getSelectedKey()]["fskModes"])
    rf09PhyOpMode.setDependencies(rf09PhyOpModeHandler, ["DRV_RF215_TRX09_EN", "DRV_RF215_TRX09_PHY_BAND", "DRV_RF215_TRX09_PHY_TYPE"])
    rf09PhyOpMode.setHelp(rf215_helpkeyword)

    rf09PhyChannel = rf215Component.createIntegerSymbol("DRV_RF215_TRX09_PHY_CHANNEL", rf09En)
    rf09PhyChannel.setLabel("Default PHY Frequency Channel Number")
    rf09PhyChannel.setDescription("Select the default PHY Frequency Channel Number for RF215 Sub-GHz Transceiver (RF09)")
    rf09PhyChannel.setDefaultValue(0)
    rf09PhyChannel.setMin(0)
    rf09PhyChannel.setMax(rf09BandsOpModes[rf09Band.getSelectedKey()]["fskMaxChannel"][0])
    rf09PhyChannel.setDependencies(rf09PhyChnNumHandler, ["DRV_RF215_TRX09_EN", "DRV_RF215_TRX09_PHY_BAND", "DRV_RF215_TRX09_PHY_TYPE", "DRV_RF215_TRX09_PHY_OPM"])
    rf09PhyChannel.setHelp(rf215_helpkeyword)

    rf09PhyChannelComment = rf215Component.createCommentSymbol("DRV_RF215_TRX09_CHN_COMMENT", rf09En)
    rf09PhyChannelComment.setVisible(False)
    rf09PhyChannelComment.setDependencies(rf09PhyChnNumCommentHandler,
    ["DRV_RF215_TRX09_EN", "DRV_RF215_TRX09_PHY_BAND", "DRV_RF215_TRX09_PHY_TYPE", "DRV_RF215_TRX09_PHY_OPM", "DRV_RF215_TRX09_PHY_CHANNEL"])
    rf09PhyChannelComment.setHelp(rf215_helpkeyword)

    rf24En = rf215Component.createBooleanSymbol("DRV_RF215_TRX24_EN", None)
    rf24En.setLabel("Use 2.4 GHz Transceiver?")
    rf24En.setDescription("Enable/disable RF215 2.4 GHz Transceiver (RF24)")
    rf24En.setDefaultValue(False)
    rf24En.setDependencies(rf24EnHandler, ["DRV_RF215_DEVICE", "DRV_RF215_TRX09_EN"])
    rf24En.setHelp(rf215_helpkeyword)

    rf24Band = rf215Component.createKeyValueSetSymbol("DRV_RF215_TRX24_PHY_BAND", rf24En)
    rf24Band.setLabel("Default PHY Frequency Band")
    rf24Band.setDescription("Select the default PHY frequency band for RF215 2.4 GHz Transceiver (RF24)")
    rf24Band.setOutputMode("Value")
    rf24Band.setDisplayMode("Key")
    rf24Band.setDefaultValue(0)
    rf24Band.setVisible(False)
    rf24Band.setDependencies(showSymbol, ["DRV_RF215_TRX24_EN"])
    rf24Band.setHelp(rf215_helpkeyword)
    for keyBand in sort_alphanumeric(rf24BandsOpModes.keys()):
        dictBand = rf24BandsOpModes[keyBand]
        rf24Band.addKey(keyBand, dictBand["value"], dictBand["description"])

    rf24PhyType = rf215Component.createKeyValueSetSymbol("DRV_RF215_TRX24_PHY_TYPE", rf24En)
    rf24PhyType.setLabel("Default PHY Type")
    rf24PhyType.setDescription("Select the default PHY type for RF215 Sub-GHz Transceiver (RF24)")
    rf24PhyType.setOutputMode("Key")
    rf24PhyType.setDisplayMode("Key")
    rf24PhyType.setDefaultValue(0)
    rf24PhyType.setVisible(False)
    rf24PhyType.setDependencies(rfPhyTypeHandler, ["DRV_RF215_TRX24_EN", "DRV_RF215_FSK_EN", "DRV_RF215_OFDM_EN"])
    rf24PhyType.addKey("FSK", "FSK", "FSK")
    rf24PhyType.addKey("OFDM", "OFDM", "OFDM")
    rf24PhyType.setHelp(rf215_helpkeyword)

    rf24PhyOpMode = rf215Component.createIntegerSymbol("DRV_RF215_TRX24_PHY_OPM", rf24En)
    rf24PhyOpMode.setLabel("Default FSK Operating Mode")
    rf24PhyOpMode.setDescription("Select the default FSK Operating Mode for RF215 Sub-GHz Transceiver (RF24)")
    rf24PhyOpMode.setDefaultValue(1)
    rf24PhyOpMode.setMin(1)
    rf24PhyOpMode.setMax(rf24BandsOpModes[rf24Band.getSelectedKey()]["fskModes"])
    rf24PhyOpMode.setVisible(False)
    rf24PhyOpMode.setDependencies(rf24PhyOpModeHandler, ["DRV_RF215_TRX24_EN", "DRV_RF215_TRX24_PHY_BAND", "DRV_RF215_TRX24_PHY_TYPE"])
    rf24PhyOpMode.setHelp(rf215_helpkeyword)

    rf24PhyChannel = rf215Component.createIntegerSymbol("DRV_RF215_TRX24_PHY_CHANNEL", rf24En)
    rf24PhyChannel.setLabel("Default PHY Frequency Channel Number")
    rf24PhyChannel.setDescription("Select the default PHY Frequency Channel Number for RF215 Sub-GHz Transceiver (RF24)")
    rf24PhyChannel.setDefaultValue(0)
    rf24PhyChannel.setMin(0)
    rf24PhyChannel.setMax(rf24BandsOpModes[rf24Band.getSelectedKey()]["fskMaxChannel"][0])
    rf24PhyChannel.setVisible(False)
    rf24PhyChannel.setDependencies(rf24PhyChnNumHandler, ["DRV_RF215_TRX24_EN", "DRV_RF215_TRX24_PHY_BAND", "DRV_RF215_TRX24_PHY_TYPE", "DRV_RF215_TRX24_PHY_OPM"])
    rf24PhyChannel.setHelp(rf215_helpkeyword)

    ############################################################################
    #### Code Generation ####
    ############################################################################

    configName = Variables.get("__CONFIGURATION_NAME")

    #### Source/Header RF215 API Files ########################################

    rf215HeaderFile = rf215Component.createFileSymbol("DRV_RF215_HEADER", None)
    rf215HeaderFile.setSourcePath("driver/rf215/drv_rf215.h.ftl")
    rf215HeaderFile.setOutputName("drv_rf215.h")
    rf215HeaderFile.setDestPath("driver/rf215")
    rf215HeaderFile.setProjectPath("config/" + configName + "/driver/rf215")
    rf215HeaderFile.setType("HEADER")
    rf215HeaderFile.setMarkup(True)
    rf215HeaderFile.setOverwrite(True)

    rf215DefHeaderFile = rf215Component.createFileSymbol("DRV_RF215_DEF", None)
    rf215DefHeaderFile.setSourcePath("driver/rf215/src/drv_rf215_definitions.h.ftl")
    rf215DefHeaderFile.setOutputName("drv_rf215_definitions.h")
    rf215DefHeaderFile.setDestPath("driver/rf215")
    rf215DefHeaderFile.setProjectPath("config/" + configName + "/driver/rf215")
    rf215DefHeaderFile.setType("HEADER")
    rf215DefHeaderFile.setMarkup(True)
    rf215DefHeaderFile.setOverwrite(True)

    rf215SourceFile = rf215Component.createFileSymbol("DRV_RF215_SOURCE", None)
    rf215SourceFile.setSourcePath("driver/rf215/src/drv_rf215.c.ftl")
    rf215SourceFile.setOutputName("drv_rf215.c")
    rf215SourceFile.setDestPath("driver/rf215")
    rf215SourceFile.setProjectPath("config/" + configName + "/driver/rf215")
    rf215SourceFile.setType("SOURCE")
    rf215SourceFile.setMarkup(True)
    rf215SourceFile.setOverwrite(True)

    #### Source/Header RF215 Internal Files ###################################

    rf215LocalHeaderFile = rf215Component.createFileSymbol("DRV_RF215_LOCAL_HEADER", None)
    rf215LocalHeaderFile.setSourcePath("driver/rf215/src/drv_rf215_local.h.ftl")
    rf215LocalHeaderFile.setOutputName("drv_rf215_local.h")
    rf215LocalHeaderFile.setDestPath("driver/rf215")
    rf215LocalHeaderFile.setProjectPath("config/" + configName + "/driver/rf215")
    rf215LocalHeaderFile.setType("HEADER")
    rf215LocalHeaderFile.setMarkup(True)
    rf215LocalHeaderFile.setOverwrite(True)

    rf215HalHeaderFile = rf215Component.createFileSymbol("DRV_RF215_HAL_HEADER", None)
    rf215HalHeaderFile.setSourcePath("driver/rf215/src/hal/rf215_hal.h.ftl")
    rf215HalHeaderFile.setOutputName("rf215_hal.h")
    rf215HalHeaderFile.setDestPath("driver/rf215/hal")
    rf215HalHeaderFile.setProjectPath("config/" + configName + "/driver/rf215/hal")
    rf215HalHeaderFile.setType("HEADER")
    rf215HalHeaderFile.setMarkup(True)
    rf215HalHeaderFile.setOverwrite(True)

    rf215HalSourceFile = rf215Component.createFileSymbol("DRV_RF215_HAL_SOURCE", None)
    rf215HalSourceFile.setSourcePath("driver/rf215/src/hal/rf215_hal.c.ftl")
    rf215HalSourceFile.setOutputName("rf215_hal.c")
    rf215HalSourceFile.setDestPath("driver/rf215/hal")
    rf215HalSourceFile.setProjectPath("config/" + configName + "/driver/rf215/hal")
    rf215HalSourceFile.setType("SOURCE")
    rf215HalSourceFile.setMarkup(True)
    rf215HalSourceFile.setOverwrite(True)

    rf215PhyHeaderFile = rf215Component.createFileSymbol("DRV_RF215_PHY_HEADER", None)
    rf215PhyHeaderFile.setSourcePath("driver/rf215/src/phy/rf215_phy.h.ftl")
    rf215PhyHeaderFile.setOutputName("rf215_phy.h")
    rf215PhyHeaderFile.setDestPath("driver/rf215/phy")
    rf215PhyHeaderFile.setProjectPath("config/" + configName + "/driver/rf215/phy")
    rf215PhyHeaderFile.setType("HEADER")
    rf215PhyHeaderFile.setMarkup(True)
    rf215PhyHeaderFile.setOverwrite(True)

    rf215PhySourceFile = rf215Component.createFileSymbol("DRV_RF215_PHY_SOURCE", None)
    rf215PhySourceFile.setSourcePath("driver/rf215/src/phy/rf215_phy.c.ftl")
    rf215PhySourceFile.setOutputName("rf215_phy.c")
    rf215PhySourceFile.setDestPath("driver/rf215/phy")
    rf215PhySourceFile.setProjectPath("config/" + configName + "/driver/rf215/phy")
    rf215PhySourceFile.setType("SOURCE")
    rf215PhySourceFile.setMarkup(True)
    rf215PhySourceFile.setOverwrite(True)

    ieeeSunFskHeaderFile = rf215Component.createFileSymbol("DRV_RF215_IEEE_SUN_FSK", None)
    ieeeSunFskHeaderFile.setSourcePath("driver/rf215/src/phy/ieee_15_4_sun_fsk.h.ftl")
    ieeeSunFskHeaderFile.setOutputName("ieee_15_4_sun_fsk.h")
    ieeeSunFskHeaderFile.setDestPath("driver/rf215/phy")
    ieeeSunFskHeaderFile.setProjectPath("config/" + configName + "/driver/rf215/phy")
    ieeeSunFskHeaderFile.setType("HEADER")
    ieeeSunFskHeaderFile.setOverwrite(True)
    ieeeSunFskHeaderFile.setMarkup(True)
    ieeeSunFskHeaderFile.setDependencies(enableIeeeSunFile, ["DRV_RF215_FSK_EN"])

    ieeeSunOfdmHeaderFile = rf215Component.createFileSymbol("DRV_RF215_IEEE_SUN_OFDM", None)
    ieeeSunOfdmHeaderFile.setSourcePath("driver/rf215/src/phy/ieee_15_4_sun_ofdm.h.ftl")
    ieeeSunOfdmHeaderFile.setOutputName("ieee_15_4_sun_ofdm.h")
    ieeeSunOfdmHeaderFile.setDestPath("driver/rf215/phy")
    ieeeSunOfdmHeaderFile.setProjectPath("config/" + configName + "/driver/rf215/phy")
    ieeeSunOfdmHeaderFile.setType("HEADER")
    ieeeSunOfdmHeaderFile.setOverwrite(True)
    ieeeSunOfdmHeaderFile.setMarkup(True)
    ieeeSunOfdmHeaderFile.setDependencies(enableIeeeSunFile, ["DRV_RF215_OFDM_EN"])

    #### System Template Files #################################################

    rf215SystemDefFile = rf215Component.createFileSymbol("DRV_RF215_SYSTEM_DEF", None)
    rf215SystemDefFile.setType("STRING")
    rf215SystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    rf215SystemDefFile.setSourcePath("driver/rf215/templates/system/definitions.h.ftl")
    rf215SystemDefFile.setMarkup(True)

    rf215SymSystemDefObjFile = rf215Component.createFileSymbol("DRV_RF215_SYSTEM_DEF_OBJECT", None)
    rf215SymSystemDefObjFile.setType("STRING")
    rf215SymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    rf215SymSystemDefObjFile.setSourcePath("driver/rf215/templates/system/definitions_objects.h.ftl")
    rf215SymSystemDefObjFile.setMarkup(True)

    rf215SymSystemConfigFile = rf215Component.createFileSymbol("DRV_RF215_SYSTEM_CONFIG", None)
    rf215SymSystemConfigFile.setType("STRING")
    rf215SymSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    rf215SymSystemConfigFile.setSourcePath("driver/rf215/templates/system/configuration.h.ftl")
    rf215SymSystemConfigFile.setMarkup(True)

    rf215SymSystemInitDataFile = rf215Component.createFileSymbol("DRV_RF215_INIT_DATA", None)
    rf215SymSystemInitDataFile.setType("STRING")
    rf215SymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    rf215SymSystemInitDataFile.setSourcePath("driver/rf215/templates/system/initialize_data.c.ftl")
    rf215SymSystemInitDataFile.setMarkup(True)

    rf215SystemInitFile = rf215Component.createFileSymbol("DRV_RF215_SYS_INIT", None)
    rf215SystemInitFile.setType("STRING")
    rf215SystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    rf215SystemInitFile.setSourcePath("driver/rf215/templates/system/initialize.c.ftl")
    rf215SystemInitFile.setMarkup(True)

    rf215SystemTasksFile = rf215Component.createFileSymbol("DRV_RF215_SYS_TASK", None)
    rf215SystemTasksFile.setType("STRING")
    rf215SystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    rf215SystemTasksFile.setSourcePath("driver/rf215/templates/system/system_tasks.c.ftl")
    rf215SystemTasksFile.setMarkup(True)

    rf215SystemRtosTasksFile = rf215Component.createFileSymbol("DRV_RF215_SYS_RTOS_TASK", None)
    rf215SystemRtosTasksFile.setType("STRING")
    rf215SystemRtosTasksFile.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
    rf215SystemRtosTasksFile.setSourcePath("driver/rf215/templates/system/system_rtos_tasks.c.ftl")
    rf215SystemRtosTasksFile.setMarkup(True)
    rf215SystemRtosTasksFile.setEnabled(getActiveRtos() != "BareMetal")
    rf215SystemRtosTasksFile.setDependencies(genRtosTask, ["HarmonyCore.SELECT_RTOS"])

def destroyComponent(rf215Component):
    Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":False})
    Database.sendMessage("HarmonyCore", "ENABLE_SYS_PORTS", {"isEnabled":False})

    if isDMAPresent:
        Database.sendMessage("HarmonyCore", "ENABLE_SYS_DMA", {"isEnabled":False})

def onAttachmentConnected(source, target):
    localComponent = source["component"]
    localConnectID = source["id"]
    remoteComponent = target["component"]
    remoteComponentID = remoteComponent.getID()

    if localConnectID == "drv_rf215_HarmonyCoreDependency":
        # Enable "Generate Harmony Driver Common Files" option in MCC
        Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":True})

        # Enable "Generate Harmony System Service Common Files" option in MCC
        Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":True})

        # Enable "Generate Harmony System Port Files" option in MCC
        Database.sendMessage("HarmonyCore", "ENABLE_SYS_PORTS", {"isEnabled":True})

        if isDMAPresent:
            # Enable "Generate Harmony System DMA Files" option in MCC
            Database.sendMessage("HarmonyCore", "ENABLE_SYS_DMA", {"isEnabled":True})

    elif localConnectID == "drv_rf215_SPI_dependency":
        # Set PLIB Used
        plibUsed = localComponent.getSymbolByID("DRV_RF215_PLIB")
        plibUsed.setValue(remoteComponentID.upper())
        spiDependencyComment.setVisible(False)

        if (remoteComponentID == "srv_spisplit"):
            # Connected to SPI Splitter
            plibUsedSpiSplit.clearValues()
            plibUsedSpiSplit.setVisible(True)
        else:
            # Show/hide NPCS depending on SPI PLIB connected
            plibConfigComment.setVisible(True)
            if (spiNumNPCS > 0) and (remoteComponentID.startswith("flexcom") or remoteComponentID.startswith("spi")):
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

            if isDMAPresent:
                # Request DMA channels for Transmit
                dmaChannelID = "DMA_CH_FOR_" + remoteComponentID.upper() + "_Transmit"
                dmaRequestID = "DMA_CH_NEEDED_FOR_" + remoteComponentID.upper() + "_Transmit"
                Database.sendMessage("core", "DMA_CHANNEL_ENABLE", {"dma_channel":dmaRequestID})
                dmaChannel = Database.getSymbolValue("core", dmaChannelID)
                dmaTxChannel.setVisible(True)
                if (dmaChannel == None) or (dmaChannel < 0) or (dmaChannel >= dmaChannelCount):
                    # Error in DMA channel allocation
                    dmaTxChannelComment.setVisible(True)
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
                    dmaRxChannelComment.setVisible(True)
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

def onAttachmentDisconnected(source, target):
    localComponent = source["component"]
    localConnectID = source["id"]
    remoteComponent = target["component"]
    remoteComponentID = remoteComponent.getID()

    if localConnectID == "drv_rf215_SPI_dependency":
        # Disable read-only in PLIB
        deconfigureSpiPlib(localComponent)

        # Clear PLIB used
        localComponent.getSymbolByID("DRV_RF215_PLIB").clearValues()
        spiDependencyComment.setVisible(True)
        plibUsedSpiSplit.setVisible(False)
        plibConfigComment.setVisible(False)
        if spiNumNPCS > 0:
            spiNpcsUsed.setVisible(False)

        if isDMAPresent:
            # Hide DMA comments
            dmaTxChannel.setVisible(False)
            dmaRxChannel.setVisible(False)
            dmaTxChannelComment.setVisible(False)
            dmaRxChannelComment.setVisible(False)

        if (remoteComponentID != "srv_spisplit"):
            # Clear read-only configurations in SPI PLIB
            Database.sendMessage(remoteComponentID, "SPI_MASTER_MODE", {"isReadOnly":False})
            Database.sendMessage(remoteComponentID, "SPI_MASTER_INTERRUPT_MODE", {"isReadOnly":False})

            if isDMAPresent:
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

        if isDMAPresent:
            # Get DMA channel for Transmit
            dmaTxChannel.setVisible(True)
            dmaChannel = args.get("dmaTxChannel")
            if (dmaChannel == None) or (dmaChannel < 0) or (dmaChannel >= dmaChannelCount):
                # Error in DMA channel allocation
                dmaTxChannelComment.setVisible(True)
                dmaTxChannel.setValue(-1)
            else:
                dmaTxChannel.setValue(dmaChannel)

            # Get DMA channel for Receive
            dmaChannel = args.get("dmaRxChannel")
            dmaRxChannel.setVisible(True)
            if (dmaChannel == None) or (dmaChannel < 0) or (dmaChannel >= dmaChannelCount):
                # Error in DMA channel allocation
                dmaRxChannelComment.setVisible(True)
                dmaRxChannel.setValue(-1)
            else:
                dmaRxChannel.setValue(dmaChannel)

    elif (messageID == "SPI_SPLITTER_DISCONNECTED"):
        deconfigureSpiPlib(plibUsedSpiSplit.getComponent())
        plibUsedSpiSplit.clearValues()
        spiDependencyComment.setVisible(True)
        plibConfigComment.setVisible(False)
        if spiNumNPCS > 0:
            spiNpcsUsed.setVisible(False)

        if isDMAPresent:
            # Hide DMA comments
            dmaTxChannel.setVisible(False)
            dmaRxChannel.setVisible(False)
            dmaTxChannelComment.setVisible(False)
            dmaRxChannelComment.setVisible(False)

    return result_dict
