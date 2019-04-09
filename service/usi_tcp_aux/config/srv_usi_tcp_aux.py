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
def instantiateComponent(usiTcpAuxComponent):

    Log.writeInfoMessage("Loading PLC USI TCP Aux Service")

    usiTcpAuxSymPLIB = usiTcpAuxComponent.createStringSymbol("SRV_USI_TCP_AUX_PLIB", None)
    usiTcpAuxSymPLIB.setLabel("Virtual PLIB")
    usiTcpAuxSymPLIB.setReadOnly(True)
    usiTcpAuxSymPLIB.setDefaultValue("TCP")





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
