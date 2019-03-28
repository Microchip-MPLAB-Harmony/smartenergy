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
        localComponent.setDependencyEnabled("srv_usi_CDC_dependency", False)
        localComponent.setDependencyEnabled("srv_usi_TCP_AUX_dependency", False)
        print ("setDependencies True")
    else:
        localComponent.setDependencyEnabled("srv_usi_CDC_dependency", True)
        localComponent.setDependencyEnabled("srv_usi_TCP_AUX_dependency", True)

def enableUSIProtocols(symbol, event):
    symbol.setVisible(event["value"])

################################################################################
#### Component ####
################################################################################
def instantiateComponent(usiComponent, index):

    Log.writeInfoMessage("Loading PLC USI Service...instance:" + str(index))

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

    usiSymConn = usiComponent.createBooleanSymbol("SRV_USI_CONN", None)
    usiSymConn.setLabel("USI connection")
    usiSymConn.setReadOnly(True)
    usiSymConn.setVisible(True)
    usiSymConn.setDefaultValue(False)
    #usiSymConn.setDependencies(setDepencies, ["SRV_USI_CONN"])

    usiSymConnPHY = usiComponent.createBooleanSymbol("SRV_USI_CONN_PHY", usiSymConn)
    usiSymConnPHY.setLabel("PHY")
    usiSymConnPHY.setVisible(False)
    usiSymConnPHY.setDefaultValue(False)
    usiSymConnPHY.setDependencies(enableUSIProtocols, ["SRV_USI_CONN"])

    usiSymConnPRIMEMNGP = usiComponent.createBooleanSymbol("SRV_USI_CONN_PRIMEMNGP", usiSymConn)
    usiSymConnPRIMEMNGP.setLabel("PRIME MNGP")
    usiSymConnPRIMEMNGP.setVisible(False)
    usiSymConnPRIMEMNGP.setDefaultValue(False)
    usiSymConnPRIMEMNGP.setDependencies(enableUSIProtocols, ["SRV_USI_CONN"])

    usiSymConnPRIMESNIF = usiComponent.createBooleanSymbol("SRV_USI_CONN_PRIMESNIF", usiSymConn)
    usiSymConnPRIMESNIF.setLabel("PRIME SNIFFER")
    usiSymConnPRIMESNIF.setVisible(False)
    usiSymConnPRIMESNIF.setDefaultValue(False)
    usiSymConnPRIMESNIF.setDependencies(enableUSIProtocols, ["SRV_USI_CONN"])

    usiSymConnPRIMEPHYSerial = usiComponent.createBooleanSymbol("SRV_USI_CONN_PRIMEPHYSERIAL", usiSymConn)
    usiSymConnPRIMEPHYSerial.setLabel("PRIME PHY SERIAL")
    usiSymConnPRIMEPHYSerial.setVisible(False)
    usiSymConnPRIMEPHYSerial.setDefaultValue(False)
    usiSymConnPRIMEPHYSerial.setDependencies(enableUSIProtocols, ["SRV_USI_CONN"])

    usiSymConnPRIMEAPI = usiComponent.createBooleanSymbol("SRV_USI_CONN_PRIMEAPI", usiSymConn)
    usiSymConnPRIMEAPI.setLabel("PRIME API")
    usiSymConnPRIMEAPI.setVisible(False)
    usiSymConnPRIMEAPI.setDefaultValue(False)
    usiSymConnPRIMEAPI.setDependencies(enableUSIProtocols, ["SRV_USI_CONN"])

    usiSymConnG3SNIF = usiComponent.createBooleanSymbol("SRV_USI_CONN_G3SNIF", usiSymConn)
    usiSymConnG3SNIF.setLabel("G3 SNIFFER")
    usiSymConnG3SNIF.setVisible(False)
    usiSymConnG3SNIF.setDefaultValue(False)
    usiSymConnG3SNIF.setDependencies(enableUSIProtocols, ["SRV_USI_CONN"])

    usiSymConnG3MAC = usiComponent.createBooleanSymbol("SRV_USI_CONN_G3MAC", usiSymConn)
    usiSymConnG3MAC.setLabel("G3 MAC")
    usiSymConnG3MAC.setVisible(False)
    usiSymConnG3MAC.setDefaultValue(False)
    usiSymConnG3MAC.setDependencies(enableUSIProtocols, ["SRV_USI_CONN"])

    usiSymConnG3ADP = usiComponent.createBooleanSymbol("SRV_USI_CONN_G3ADP", usiSymConn)
    usiSymConnG3ADP.setLabel("G3 ADP")
    usiSymConnG3ADP.setVisible(False)
    usiSymConnG3ADP.setDefaultValue(False)
    usiSymConnG3ADP.setDependencies(enableUSIProtocols, ["SRV_USI_CONN"])

    usiSymConnG3COORD = usiComponent.createBooleanSymbol("SRV_USI_CONN_G3COORDINATOR", usiSymConn)
    usiSymConnG3COORD.setLabel("G3 COORDINATOR")
    usiSymConnG3COORD.setVisible(False)
    usiSymConnG3COORD.setDefaultValue(False)
    usiSymConnG3COORD.setDependencies(enableUSIProtocols, ["SRV_USI_CONN"])

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

    updateDependencies = False

    if connectID == "srv_usi_USART_dependency" :
        print("OnConnection...srv_usi_USART_dependency")
#        localComponent.setDependencyEnabled("srv_usi_CDC_dependency", False)
#        localComponent.setDependencyEnabled("srv_usi_TCP_AUX_dependency", False)
        updateDependencies = True
        Database.setSymbolValue("srv_usi", "SRV_USI_USART_API", True, 1)
        apiUsedIndex = localComponent.getSymbolByID("SRV_USI_USART_API_INDEX")
        apiUsedIndex.setValue(True, 0)

    if connectID == "srv_usi_TCP_AUX_dependency" :
        print("OnConnection...srv_usi_TCP_AUX_dependency")
#        localComponent.setDependencyEnabled("srv_usi_USART_dependency", False)
#        localComponent.setDependencyEnabled("srv_usi_CDC_dependency", False)
        updateDependencies = True
        Database.setSymbolValue("srv_usi", "SRV_USI_TCP_API", True, 1)
        apiUsedIndex = localComponent.getSymbolByID("SRV_USI_TCP_API_INDEX")
        apiUsedIndex.setValue(True, 0)

    if connectID == "srv_usi_CDC_dependency" :
        print("OnConnection...srv_usi_CDC_dependency")
#        localComponent.setDependencyEnabled("srv_usi_USART_dependency", False)
#        localComponent.setDependencyEnabled("srv_usi_TCP_AUX_dependency", False)
        updateDependencies = True
        Database.setSymbolValue("srv_usi", "SRV_USI_CDC_API", True, 1)
        apiUsedIndex = localComponent.getSymbolByID("SRV_USI_CDC_API_INDEX")
        apiUsedIndex.setValue(True, 0)

    if updateDependencies:
        print("Set PLIB connection")
        deviceUsed = localComponent.getSymbolByID("SRV_USI_CONN")
        deviceUsed.clearValue()
        deviceUsed.setValue(True, 0)
        plibUsed = localComponent.getSymbolByID("SRV_USI_COMM_API")
        plibUsed.setValue(remoteID.upper(), 0)


def onAttachmentDisconnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    targetID = target["id"]

    print("OnDisconnection..." + connectID)
    updateDependencies = False

    if connectID == "srv_usi_USART_dependency" :
        print("OnDisconnection...srv_usi_USART_dependency")
#        localComponent.setDependencyEnabled("srv_usi_CDC_dependency", True)
#        localComponent.setDependencyEnabled("srv_usi_TCP_AUX_dependency", True)
        updateDependencies = True
        Database.setSymbolValue("srv_usi", "SRV_USI_USART_API", False, 1)
        apiUsedIndex = localComponent.getSymbolByID("SRV_USI_USART_API_INDEX")
        apiUsedIndex.setValue(False, 0)

    if connectID == "srv_usi_TCP_AUX_dependency" :
        print("OnDisconnection...srv_usi_TCP_AUX_dependency")
#        localComponent.setDependencyEnabled("srv_usi_USART_dependency", True)
#        localComponent.setDependencyEnabled("srv_usi_CDC_dependency", True)
        updateDependencies = True
        Database.setSymbolValue("srv_usi", "SRV_USI_TCP_API", False, 1)
        apiUsedIndex = localComponent.getSymbolByID("SRV_USI_TCP_API_INDEX")
        apiUsedIndex.setValue(False, 0)

    if connectID == "srv_usi_CDC_dependency" :
        print("OnDisconnection...srv_usi_CDC_dependency")
#        localComponent.setDependencyEnabled("srv_usi_USART_dependency", True)
#        localComponent.setDependencyEnabled("srv_usi_TCP_AUX_dependency", True)
        updateDependencies = True
        Database.setSymbolValue("srv_usi", "SRV_USI_CDC_API", False, 1)
        apiUsedIndex = localComponent.getSymbolByID("SRV_USI_CDC_API_INDEX")
        apiUsedIndex.setValue(False, 0)

    if updateDependencies:
        print("Clear PLIB connection")
        deviceUsed = localComponent.getSymbolByID("SRV_USI_CONN")
        deviceUsed.clearValue()
        plibUsed = localComponent.getSymbolByID("SRV_USI_COMM_API")
        plibUsed.setValue("", 0)
