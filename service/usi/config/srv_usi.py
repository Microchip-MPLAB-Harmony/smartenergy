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
def setDepencies(symbol, event):
    localComponent = symbol.getComponent()
    print ("setDependencies:" + str(event["value"]))

    if event["value"] == 1:
        localComponent.setDependencyEnabled("srv_usi_USB_dependency", False)
        localComponent.setDependencyEnabled("srv_usi_ETH_dependency", False)
        print ("setDependencies True")
    else:
        localComponent.setDependencyEnabled("srv_usi_USB_dependency", True)
        localComponent.setDependencyEnabled("srv_usi_ETH_dependency", True)

#def enableUSIProtocols(localComponent):
#    deviceUsed = localComponent.getSymbolByID("SRV_USI_CONN")


################################################################################
#### Component ####
################################################################################
def instantiateComponent(usiComponent, index):

    Log.writeInfoMessage("Loading PLC USI Service...instance:" + str(index))

    usiSymIndex = usiComponent.createIntegerSymbol("INDEX", None)
    usiSymIndex.setVisible(False)
    usiSymIndex.setDefaultValue(index)

    usiSymPLIB = usiComponent.createStringSymbol("SRV_USI_PLIB", None)
    usiSymPLIB.setLabel("PLIB Used")
    usiSymPLIB.setReadOnly(True)
    usiSymPLIB.setDefaultValue("")

    usiSymConn = usiComponent.createBooleanSymbol("SRV_USI_CONN", None)
    usiSymConn.setLabel("USI connection")
    usiSymConn.setReadOnly(True)
    usiSymConn.setVisible(True)
    usiSymConn.setDefaultValue(False)
    usiSymConn.setDependencies(setDepencies, ["SRV_USI_CONN"])

    usiSymConnPHY = usiComponent.createBooleanSymbol("SRV_USI_CONN_PHY", usiSymConn)
    usiSymConnPHY.setLabel("PHY")
    usiSymConnPHY.setVisible(True)
    usiSymConnPHY.setDefaultValue(False)

    usiSymConnPRIMEMNGP = usiComponent.createBooleanSymbol("SRV_USI_CONN_PRIMEMNGP", usiSymConn)
    usiSymConnPRIMEMNGP.setLabel("PRIME MNGP")
    usiSymConnPRIMEMNGP.setVisible(True)
    usiSymConnPRIMEMNGP.setDefaultValue(False)

    usiSymConnPRIMESNIF = usiComponent.createBooleanSymbol("SRV_USI_CONN_PRIMESNIF", usiSymConn)
    usiSymConnPRIMESNIF.setLabel("PRIME SNIFFER")
    usiSymConnPRIMESNIF.setVisible(True)
    usiSymConnPRIMESNIF.setDefaultValue(False)

    usiSymConnPRIMEPHYSerial = usiComponent.createBooleanSymbol("SRV_USI_CONN_PRIMEPHYSERIAL", usiSymConn)
    usiSymConnPRIMEPHYSerial.setLabel("PRIME PHY SERIAL")
    usiSymConnPRIMEPHYSerial.setVisible(True)
    usiSymConnPRIMEPHYSerial.setDefaultValue(False)

    usiSymConnPRIMEAPI = usiComponent.createBooleanSymbol("SRV_USI_CONN_PRIMEAPI", usiSymConn)
    usiSymConnPRIMEAPI.setLabel("PRIME API")
    usiSymConnPRIMEAPI.setVisible(True)
    usiSymConnPRIMEAPI.setDefaultValue(False)

    usiSymConnG3SNIF = usiComponent.createBooleanSymbol("SRV_USI_CONN_G3SNIF", usiSymConn)
    usiSymConnG3SNIF.setLabel("G3 SNIFFER")
    usiSymConnG3SNIF.setVisible(True)
    usiSymConnG3SNIF.setDefaultValue(False)

    usiSymConnG3MAC = usiComponent.createBooleanSymbol("SRV_USI_CONN_G3MAC", usiSymConn)
    usiSymConnG3MAC.setLabel("G3 MAC")
    usiSymConnG3MAC.setVisible(True)
    usiSymConnG3MAC.setDefaultValue(False)

    usiSymConnG3ADP = usiComponent.createBooleanSymbol("SRV_USI_CONN_G3ADP", usiSymConn)
    usiSymConnG3ADP.setLabel("G3 ADP")
    usiSymConnG3ADP.setVisible(True)
    usiSymConnG3ADP.setDefaultValue(False)

    usiSymConnG3COORD = usiComponent.createBooleanSymbol("SRV_USI_CONN_G3COORDINATOR", usiSymConn)
    usiSymConnG3COORD.setLabel("G3 COORDINATOR")
    usiSymConnG3COORD.setVisible(True)
    usiSymConnG3COORD.setDefaultValue(False)

    usiSymConnMICROPHY = usiComponent.createBooleanSymbol("SRV_USI_CONN_MICROPHY", usiSymConn)
    usiSymConnMICROPHY.setLabel("MICROPHY")
    usiSymConnMICROPHY.setVisible(True)
    usiSymConnMICROPHY.setDefaultValue(False)

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    usiSourceFile = usiComponent.createFileSymbol("SRV_USI_SOURCE", None)
    usiSourceFile.setSourcePath("services/plc/usi/usi.c")
    usiSourceFile.setOutputName("srv_usi.c")
    usiSourceFile.setDestPath("services/plc/usi")
    usiSourceFile.setProjectPath("config/" + configName + "services/plc/usi/")
    usiSourceFile.setType("SOURCE")
    usiSourceFile.setOverwrite(True)

    usiHeaderFile = usiComponent.createFileSymbol("SRV_USI_HEADER", None)
    usiHeaderFile.setSourcePath("services/plc/usi/usi.h")
    usiHeaderFile.setOutputName("srv_usi.h")
    usiSourceFile.setDestPath("services/plc/usi")
    usiSourceFile.setProjectPath("config/" + configName + "services/plc/usi/")
    usiHeaderFile.setType("HEADER")
    usiHeaderFile.setOverwrite(True)

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

    if connectID == "srv_usi_UART_dependency" :
        deviceUsed = localComponent.getSymbolByID("SRV_USI_CONN")
        deviceUsed.clearValue()
        deviceUsed.setValue(True, 0)
        plibUsed = localComponent.getSymbolByID("SRV_USI_PLIB")
        plibUsed.setValue(remoteID.upper(), 0)
        ##localComponent.setDependencyEnabled("srv_usi_USB_dependency", False)
        ##localComponent.setDependencyEnabled("srv_usi_ETH_dependency", False)

        #enableUSIProtocols(localComponent)

def onAttachmentDisconnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    targetID = target["id"]

    print("OnDisconnection..." + connectID)

    if connectID == "srv_usi_UART_dependency" :
        deviceUsed = localComponent.getSymbolByID("SRV_USI_CONN")
        deviceUsed.clearValue()
        plibUsed = localComponent.getSymbolByID("SRV_USI_PLIB")
        plibUsed.setValue("", 0)
        ##localComponent.setDependencyEnabled("srv_usi_USB_dependency", True)
        ##localComponent.setDependencyEnabled("srv_usi_ETH_dependency", True)
        
        #disableUSIProtocols(localComponent)
