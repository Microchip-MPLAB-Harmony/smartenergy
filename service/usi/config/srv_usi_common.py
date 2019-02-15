# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

def instantiateComponent(usiComponentCommon):
    
    Log.writeInfoMessage("Loading PLC USI Common Service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    usiSourceFile = usiComponentCommon.createFileSymbol("SRV_USI_SOURCE", None)
    usiSourceFile.setSourcePath("services/plc/usi/usi.c")
    usiSourceFile.setOutputName("srv_usi.c")
    usiSourceFile.setDestPath("services/plc/usi")
    usiSourceFile.setProjectPath("config/" + configName + "services/plc/usi/")
    usiSourceFile.setType("SOURCE")
    usiSourceFile.setOverwrite(True)

    usiHeaderFile = usiComponentCommon.createFileSymbol("SRV_USI_HEADER", None)
    usiHeaderFile.setSourcePath("services/plc/usi/usi.h")
    usiHeaderFile.setOutputName("srv_usi.h")
    usiSourceFile.setDestPath("services/plc/usi")
    usiSourceFile.setProjectPath("config/" + configName + "services/plc/usi/")
    usiHeaderFile.setType("HEADER")
    usiHeaderFile.setOverwrite(True)

