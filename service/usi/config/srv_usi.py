# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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

global srv_usi_helpkeyword
srv_usi_helpkeyword = "mcc_h3_srv_usi_configurations"

def selectDeviceSet(symbol, event):
    symbol.clearValue()
    if ("USART" in event["value"]) or ("UART" in event["value"]) or ("SERCOM" in event["value"]) or ("FLEXCOM" in event["value"]):
        symbol.setValue("UART")
    elif "USB" in event["value"]:
        symbol.setValue("USB_CDC")
    else:
        symbol.setValue("")

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

    usiSymDevice = usiComponent.createStringSymbol("SRV_USI_DEVICE", None)
    usiSymDevice.setLabel("Device Used")
    usiSymDevice.setReadOnly(True)
    usiSymDevice.setDefaultValue("")
    usiSymDevice.setHelp(srv_usi_helpkeyword)

    usiSymDeviceSet = usiComponent.createStringSymbol("SRV_USI_DEVICE_SET", None)
    usiSymDeviceSet.setLabel("Device Set")
    usiSymDeviceSet.setDefaultValue("")
    usiSymDeviceSet.setDependencies(selectDeviceSet, ["SRV_USI_DEVICE"])
    usiSymDeviceSet.setVisible(False)

    usiSymUsartAPI = usiComponent.createIntegerSymbol("SRV_USI_USART_API_INDEX", None)
    usiSymUsartAPI.setLabel("USART API INDEX")
    usiSymUsartAPI.setDefaultValue(0)
    usiSymUsartAPI.setVisible(False)

    usiSymCdcAPI = usiComponent.createIntegerSymbol("SRV_USI_CDC_API_INDEX", None)
    usiSymCdcAPI.setLabel("CDC API INDEX")
    usiSymCdcAPI.setDefaultValue(0)
    usiSymCdcAPI.setVisible(False)

    usiSymDeviceIndex = usiComponent.createIntegerSymbol("SRV_USI_CDC_DEVICE_INDEX", None)
    usiSymDeviceIndex.setLabel("CDC Device Index")
    usiSymDeviceIndex.setVisible(False)
    usiSymDeviceIndex.setDefaultValue(0)

    usiSymReadBufferSize = usiComponent.createIntegerSymbol("SRV_USI_RD_BUF_SIZE", None)
    usiSymReadBufferSize.setLabel("Read Buffer Size")
    usiSymReadBufferSize.setDefaultValue(1024)
    usiSymReadBufferSize.setMin(8)
    usiSymReadBufferSize.setMax(32*1024)
    usiSymReadBufferSize.setHelp(srv_usi_helpkeyword)

    usiSymReadBufferSize = usiComponent.createIntegerSymbol("SRV_USI_WR_BUF_SIZE", None)
    usiSymReadBufferSize.setLabel("Write Buffer Size")
    usiSymReadBufferSize.setDefaultValue(1024)
    usiSymReadBufferSize.setMin(8)
    usiSymReadBufferSize.setMax(32*1024)
    usiSymReadBufferSize.setHelp(srv_usi_helpkeyword)

    # RTOS CONFIG
    usiRTOSMenu = usiComponent.createMenuSymbol("SRV_USI_RTOS_MENU", None)
    usiRTOSMenu.setLabel("RTOS settings")
    usiRTOSMenu.setDescription("RTOS settings")
    usiRTOSMenu.setVisible(getActiveRtos() != "BareMetal")
    usiRTOSMenu.setDependencies(showRTOSMenu, ["HarmonyCore.SELECT_RTOS"])
    usiRTOSMenu.setHelp(srv_usi_helpkeyword)

    usiRTOSStackSize = usiComponent.createIntegerSymbol("SRV_USI_RTOS_STACK_SIZE", usiRTOSMenu)
    usiRTOSStackSize.setLabel("Stack Size (in bytes)")
    usiRTOSStackSize.setDefaultValue(1024)
    usiRTOSStackSize.setMin(1024)
    usiRTOSStackSize.setMax(16*1024)
    usiRTOSStackSize.setHelp(srv_usi_helpkeyword)

    usiRTOSTaskPriority = usiComponent.createIntegerSymbol("SRV_USI_RTOS_TASK_PRIORITY", usiRTOSMenu)
    usiRTOSTaskPriority.setLabel("Task Priority")
    usiRTOSTaskPriority.setDefaultValue(1)
    usiRTOSTaskPriority.setMin(0)
    usiRTOSTaskPriority.setHelp(srv_usi_helpkeyword)

    usiRTOSMsgQSize = usiComponent.createIntegerSymbol("SRV_USI_RTOS_TASK_MSG_QTY", usiRTOSMenu)
    usiRTOSMsgQSize.setLabel("Maximum Message Queue Size")
    usiRTOSMsgQSize.setDescription("A µC/OS-III task contains an optional internal message queue (if OS_CFG_TASK_Q_EN is set to DEF_ENABLED in os_cfg.h). This argument specifies the maximum number of messages that the task can receive through this message queue. The user may specify that the task is unable to receive messages by setting this argument to 0")
    usiRTOSMsgQSize.setDefaultValue(0)
    usiRTOSMsgQSize.setMin(0)
    usiRTOSMsgQSize.setVisible(getActiveRtos() == "MicriumOSIII")
    usiRTOSMsgQSize.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])
    usiRTOSMsgQSize.setHelp(srv_usi_helpkeyword)

    usiRTOSTaskTimeQuanta = usiComponent.createIntegerSymbol("SRV_USI_RTOS_TASK_TIME_QUANTA", usiRTOSMenu)
    usiRTOSTaskTimeQuanta.setLabel("Task Time Quanta")
    usiRTOSTaskTimeQuanta.setDescription("The amount of time (in clock ticks) for the time quanta when Round Robin is enabled. If you specify 0, then the default time quanta will be used which is the tick rate divided by 10.")
    usiRTOSTaskTimeQuanta.setDefaultValue(0)
    usiRTOSTaskTimeQuanta.setMin(0)
    usiRTOSTaskTimeQuanta.setVisible(getActiveRtos() == "MicriumOSIII")
    usiRTOSTaskTimeQuanta.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])
    usiRTOSTaskTimeQuanta.setHelp(srv_usi_helpkeyword)

    usiRTOSTaskSpecificOpt = usiComponent.createBooleanSymbol("SRV_USI_RTOS_TASK_OPT_NONE", usiRTOSMenu)
    usiRTOSTaskSpecificOpt.setLabel("Task Specific Options")
    usiRTOSTaskSpecificOpt.setDescription("Contains task-specific options. Each option consists of one bit. The option is selected when the bit is set. The current version of µC/OS-III supports the following options:")
    usiRTOSTaskSpecificOpt.setDefaultValue(True)
    usiRTOSTaskSpecificOpt.setVisible(getActiveRtos() == "MicriumOSIII")
    usiRTOSTaskSpecificOpt.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])
    usiRTOSTaskSpecificOpt.setHelp(srv_usi_helpkeyword)

    usiRTOSTaskStkChk = usiComponent.createBooleanSymbol("SRV_USI_RTOS_TASK_OPT_STK_CHK", usiRTOSTaskSpecificOpt)
    usiRTOSTaskStkChk.setLabel("Stack checking is allowed for the task")
    usiRTOSTaskStkChk.setDescription("Specifies whether stack checking is allowed for the task")
    usiRTOSTaskStkChk.setDefaultValue(True)
    usiRTOSTaskStkChk.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["SRV_USI_RTOS_TASK_OPT_NONE"])
    usiRTOSTaskStkChk.setHelp(srv_usi_helpkeyword)

    usiRTOSTaskStkClr = usiComponent.createBooleanSymbol("SRV_USI_RTOS_TASK_OPT_STK_CLR", usiRTOSTaskSpecificOpt)
    usiRTOSTaskStkClr.setLabel("Stack needs to be cleared")
    usiRTOSTaskStkClr.setDescription("Specifies whether the stack needs to be cleared")
    usiRTOSTaskStkClr.setDefaultValue(True)
    usiRTOSTaskStkClr.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["SRV_USI_RTOS_TASK_OPT_NONE"])
    usiRTOSTaskStkClr.setHelp(srv_usi_helpkeyword)

    usiRTOSTaskSaveFp = usiComponent.createBooleanSymbol("SRV_USI_RTOS_TASK_OPT_SAVE_FP", usiRTOSTaskSpecificOpt)
    usiRTOSTaskSaveFp.setLabel("Floating-point registers needs to be saved")
    usiRTOSTaskSaveFp.setDescription("Specifies whether floating-point registers are saved. This option is only valid if the processor has floating-point hardware and the processor-specific code saves the floating-point registers")
    usiRTOSTaskSaveFp.setDefaultValue(False)
    usiRTOSTaskSaveFp.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["SRV_USI_RTOS_TASK_OPT_NONE"])
    usiRTOSTaskSaveFp.setHelp(srv_usi_helpkeyword)

    usiRTOSTaskNoTls = usiComponent.createBooleanSymbol("SRV_USI_RTOS_TASK_OPT_NO_TLS", usiRTOSTaskSpecificOpt)
    usiRTOSTaskNoTls.setLabel("TLS (Thread Local Storage) support needed for the task")
    usiRTOSTaskNoTls.setDescription("If the caller doesn’t want or need TLS (Thread Local Storage) support for the task being created. If you do not include this option, TLS will be supported by default. TLS support was added in V3.03.00")
    usiRTOSTaskNoTls.setDefaultValue(False)
    usiRTOSTaskNoTls.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["SRV_USI_RTOS_TASK_OPT_NONE"])
    usiRTOSTaskNoTls.setHelp(srv_usi_helpkeyword)

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

    plcSystemRtosTasksFile = usiComponent.createFileSymbol("SRV_USI_SYS_RTOS_TASK", None)
    plcSystemRtosTasksFile.setType("STRING")
    plcSystemRtosTasksFile.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
    plcSystemRtosTasksFile.setSourcePath("service/usi/templates/system/system_rtos_tasks.c.ftl")
    plcSystemRtosTasksFile.setMarkup(True)
    plcSystemRtosTasksFile.setEnabled(getActiveRtos() != "BareMetal")
    plcSystemRtosTasksFile.setDependencies(genRtosTask, ["HarmonyCore.SELECT_RTOS"])

################################################################################
#### Business Logic ####
################################################################################  
def onAttachmentConnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    dict = {}

    if connectID == "srv_usi_USART_dependency":
        plibUsed = localComponent.getSymbolByID("SRV_USI_DEVICE")
        plibUsed.clearValue()
        plibUsed.setValue(remoteID.upper())
        dict = Database.sendMessage("srv_usi", "USI_ADD_USART_API", {})
    
    if connectID == "srv_usi_CDC_dependency":
        plibUsed = localComponent.getSymbolByID("SRV_USI_DEVICE")
        plibUsed.clearValue()
        plibUsed.setValue(remoteID.upper())
        cdcSymDeviceIndex = localComponent.getSymbolByID("SRV_USI_CDC_DEVICE_INDEX")
        cdcSymDeviceIndex.setValue(Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_FUNCTION_INDEX"))
        dict = Database.sendMessage("srv_usi", "USI_ADD_CDC_API", {})

def onAttachmentDisconnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]

    dict = {}

    if connectID == "srv_usi_USART_dependency":
        localComponent.getSymbolByID("SRV_USI_DEVICE").clearValue()
        dict = Database.sendMessage("srv_usi", "USI_REMOVE_USART_API", {})
        dict = Database.sendMessage(remoteID, "UART_NON_BLOCKING_MODE", {"isReadOnly":False})
    
    if connectID == "srv_usi_CDC_dependency":
        localComponent.getSymbolByID("SRV_USI_DEVICE").clearValue()
        dict = Database.sendMessage("srv_usi", "USI_REMOVE_CDC_API", {})

def handleMessage(messageID, args):

    result_dict = {}

    if (messageID == "REQUEST_CONFIG_PARAMS"):
        if args.get("localComponentID") != None:
            result_dict = Database.sendMessage(args["localComponentID"], "UART_NON_BLOCKING_MODE", {"isEnabled":True, "isReadOnly":True})

    return result_dict

