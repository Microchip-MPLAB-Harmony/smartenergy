"""*****************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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

def instantiateComponent(srvSecurityComponent):
    
    Log.writeInfoMessage("Loading Security service for G3")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")
    
    # Security Files
    pSecurityAesWrpSource = srvSecurityComponent.createFileSymbol("SRV_SECURITY_AES_WRP_SOURCE", None)
    pSecurityAesWrpSource.setSourcePath("service/security/src/aes_wrapper.c")
    pSecurityAesWrpSource.setOutputName("aes_wrapper.c")
    pSecurityAesWrpSource.setDestPath("stack/service/security")
    pSecurityAesWrpSource.setProjectPath("config/" + configName + "/stack/service/security/")
    pSecurityAesWrpSource.setType("SOURCE")
    
    pSecurityAesWrpHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_AES_WRP_HEADER", None)
    pSecurityAesWrpHeader.setSourcePath("service/security/aes_wrapper.h")
    pSecurityAesWrpHeader.setOutputName("aes_wrapper.h")
    pSecurityAesWrpHeader.setDestPath("stack/service/security")
    pSecurityAesWrpHeader.setProjectPath("config/" + configName + "/stack/service/security/")
    pSecurityAesWrpHeader.setType("HEADER")
    
    pSecurityCipherWrpSource = srvSecurityComponent.createFileSymbol("SRV_SECURITY_CIPHER_WRP_SOURCE", None)
    pSecurityCipherWrpSource.setSourcePath("service/security/src/cipher_wrapper.c")
    pSecurityCipherWrpSource.setOutputName("cipher_wrapper.c")
    pSecurityCipherWrpSource.setDestPath("stack/service/security")
    pSecurityCipherWrpSource.setProjectPath("config/" + configName + "/stack/service/security/")
    pSecurityCipherWrpSource.setType("SOURCE")
    
    pSecurityCipherWrpHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_CIPHER_WRP_HEADER", None)
    pSecurityCipherWrpHeader.setSourcePath("service/security/cipher_wrapper.h")
    pSecurityCipherWrpHeader.setOutputName("cipher_wrapper.h")
    pSecurityCipherWrpHeader.setDestPath("stack/service/security")
    pSecurityCipherWrpHeader.setProjectPath("config/" + configName + "/stack/service/security/")
    pSecurityCipherWrpHeader.setType("HEADER")
    
    pSecurityEaxSource = srvSecurityComponent.createFileSymbol("SRV_SECURITY_EAX_SOURCE", None)
    pSecurityEaxSource.setSourcePath("service/security/src/eax.c")
    pSecurityEaxSource.setOutputName("eax.c")
    pSecurityEaxSource.setDestPath("stack/service/security")
    pSecurityEaxSource.setProjectPath("config/" + configName + "/stack/service/security/")
    pSecurityEaxSource.setType("SOURCE")
    
    pSecurityEaxHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_EAX_HEADER", None)
    pSecurityEaxHeader.setSourcePath("service/security/src/eax.h")
    pSecurityEaxHeader.setOutputName("eax.h")
    pSecurityEaxHeader.setDestPath("stack/service/security")
    pSecurityEaxHeader.setProjectPath("config/" + configName + "/stack/service/security/")
    pSecurityEaxHeader.setType("HEADER")
    
    pSecurityBrgEndianHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_BRG_ENDIAN_HEADER", None)
    pSecurityBrgEndianHeader.setSourcePath("service/security/src/brg_endian.h")
    pSecurityBrgEndianHeader.setOutputName("brg_endian.h")
    pSecurityBrgEndianHeader.setDestPath("stack/service/security")
    pSecurityBrgEndianHeader.setProjectPath("config/" + configName + "/stack/service/security/")
    pSecurityBrgEndianHeader.setType("HEADER")
    
    pSecurityBrgTypesHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_BRG_TYPES_HEADER", None)
    pSecurityBrgTypesHeader.setSourcePath("service/security/src/brg_types.h")
    pSecurityBrgTypesHeader.setOutputName("brg_types.h")
    pSecurityBrgTypesHeader.setDestPath("stack/service/security")
    pSecurityBrgTypesHeader.setProjectPath("config/" + configName + "/stack/service/security/")
    pSecurityBrgTypesHeader.setType("HEADER")
    
    pSecurityModeHdrHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_MODE_HDR_HEADER", None)
    pSecurityModeHdrHeader.setSourcePath("service/security/src/mode_hdr.h")
    pSecurityModeHdrHeader.setOutputName("mode_hdr.h")
    pSecurityModeHdrHeader.setDestPath("stack/service/security")
    pSecurityModeHdrHeader.setProjectPath("config/" + configName + "/stack/service/security/")
    pSecurityModeHdrHeader.setType("HEADER")    
