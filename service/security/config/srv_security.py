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

def instantiateComponent(srvSecurityComponent):

    Log.writeInfoMessage("Loading SE Security service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    # Security Files
    pSecurityAesWrpSource = srvSecurityComponent.createFileSymbol("SRV_SECURITY_AES_WRP_SOURCE", None)
    pSecurityAesWrpSource.setSourcePath("service/security/src/aes_wrapper.c")
    pSecurityAesWrpSource.setOutputName("aes_wrapper.c")
    pSecurityAesWrpSource.setDestPath("service/security")
    pSecurityAesWrpSource.setProjectPath("config/" + configName + "/service/security/")
    pSecurityAesWrpSource.setType("SOURCE")

    pSecurityAesWrpHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_AES_WRP_HEADER", None)
    pSecurityAesWrpHeader.setSourcePath("service/security/aes_wrapper.h")
    pSecurityAesWrpHeader.setOutputName("aes_wrapper.h")
    pSecurityAesWrpHeader.setDestPath("service/security")
    pSecurityAesWrpHeader.setProjectPath("config/" + configName + "/service/security/")
    pSecurityAesWrpHeader.setType("HEADER")

    pSecurityCipherWrpSource = srvSecurityComponent.createFileSymbol("SRV_SECURITY_CIPHER_WRP_SOURCE", None)
    pSecurityCipherWrpSource.setSourcePath("service/security/src/cipher_wrapper.c")
    pSecurityCipherWrpSource.setOutputName("cipher_wrapper.c")
    pSecurityCipherWrpSource.setDestPath("service/security")
    pSecurityCipherWrpSource.setProjectPath("config/" + configName + "/service/security/")
    pSecurityCipherWrpSource.setType("SOURCE")

    pSecurityCipherWrpHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_CIPHER_WRP_HEADER", None)
    pSecurityCipherWrpHeader.setSourcePath("service/security/cipher_wrapper.h")
    pSecurityCipherWrpHeader.setOutputName("cipher_wrapper.h")
    pSecurityCipherWrpHeader.setDestPath("service/security")
    pSecurityCipherWrpHeader.setProjectPath("config/" + configName + "/service/security/")
    pSecurityCipherWrpHeader.setType("HEADER")

    pSecurityEaxSource = srvSecurityComponent.createFileSymbol("SRV_SECURITY_EAX_SOURCE", None)
    pSecurityEaxSource.setSourcePath("service/security/src/eax.c")
    pSecurityEaxSource.setOutputName("eax.c")
    pSecurityEaxSource.setDestPath("service/security")
    pSecurityEaxSource.setProjectPath("config/" + configName + "/service/security/")
    pSecurityEaxSource.setType("SOURCE")

    pSecurityEaxHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_EAX_HEADER", None)
    pSecurityEaxHeader.setSourcePath("service/security/src/eax.h")
    pSecurityEaxHeader.setOutputName("eax.h")
    pSecurityEaxHeader.setDestPath("service/security")
    pSecurityEaxHeader.setProjectPath("config/" + configName + "/service/security/")
    pSecurityEaxHeader.setType("HEADER")

    pSecurityBrgEndianHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_BRG_ENDIAN_HEADER", None)
    pSecurityBrgEndianHeader.setSourcePath("service/security/src/brg_endian.h")
    pSecurityBrgEndianHeader.setOutputName("brg_endian.h")
    pSecurityBrgEndianHeader.setDestPath("service/security")
    pSecurityBrgEndianHeader.setProjectPath("config/" + configName + "/service/security/")
    pSecurityBrgEndianHeader.setType("HEADER")

    pSecurityBrgTypesHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_BRG_TYPES_HEADER", None)
    pSecurityBrgTypesHeader.setSourcePath("service/security/src/brg_types.h")
    pSecurityBrgTypesHeader.setOutputName("brg_types.h")
    pSecurityBrgTypesHeader.setDestPath("service/security")
    pSecurityBrgTypesHeader.setProjectPath("config/" + configName + "/service/security/")
    pSecurityBrgTypesHeader.setType("HEADER")

    pSecurityModeHdrHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_MODE_HDR_HEADER", None)
    pSecurityModeHdrHeader.setSourcePath("service/security/src/mode_hdr.h")
    pSecurityModeHdrHeader.setOutputName("mode_hdr.h")
    pSecurityModeHdrHeader.setDestPath("service/security")
    pSecurityModeHdrHeader.setProjectPath("config/" + configName + "/service/security/")
    pSecurityModeHdrHeader.setType("HEADER")
