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

def instantiateComponent(crcComponentCommon):
    
    Log.writeInfoMessage("Loading PLC CRC Common Service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    # USART wrapper Files
    srvCrcSourceFile = crcComponentCommon.createFileSymbol("SRV_PCRC_SOURCE", None)
    srvCrcSourceFile.setSourcePath("service/pcrc/srv_pcrc.c")
    srvCrcSourceFile.setOutputName("srv_pcrc.c")
    srvCrcSourceFile.setDestPath("service/pcrc")
    srvCrcSourceFile.setProjectPath("config/" + configName + "/service/pcrc/")
    srvCrcSourceFile.setType("SOURCE")
    srvCrcSourceFile.setMarkup(False)
    srvCrcSourceFile.setOverwrite(True)
    srvCrcSourceFile.setEnabled(True)

    srvCrcHeaderFile = crcComponentCommon.createFileSymbol("SRV_PCRC_HEADER", None)
    srvCrcHeaderFile.setSourcePath("service/pcrc/srv_pcrc.h")
    srvCrcHeaderFile.setOutputName("srv_pcrc.h")
    srvCrcHeaderFile.setDestPath("service/pcrc")
    srvCrcHeaderFile.setProjectPath("config/" + configName + "/service/pcrc/")
    srvCrcHeaderFile.setType("HEADER")
    srvCrcHeaderFile.setMarkup(False)
    srvCrcHeaderFile.setOverwrite(True)
    srvCrcHeaderFile.setEnabled(True)
