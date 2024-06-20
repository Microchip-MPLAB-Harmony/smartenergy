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
    pSecurityAesWrpSource.setSourcePath("service/security/src/aes_wrapper.c.ftl")
    pSecurityAesWrpSource.setOutputName("aes_wrapper.c")
    pSecurityAesWrpSource.setDestPath("service/security")
    pSecurityAesWrpSource.setProjectPath("config/" + configName + "/service/security/")
    pSecurityAesWrpSource.setType("SOURCE")
    pSecurityAesWrpSource.setMarkup(True)

    pSecurityAesWrpHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_AES_WRP_HEADER", None)
    pSecurityAesWrpHeader.setSourcePath("service/security/aes_wrapper.h.ftl")
    pSecurityAesWrpHeader.setOutputName("aes_wrapper.h")
    pSecurityAesWrpHeader.setDestPath("service/security")
    pSecurityAesWrpHeader.setProjectPath("config/" + configName + "/service/security/")
    pSecurityAesWrpHeader.setType("HEADER")
    pSecurityAesWrpHeader.setMarkup(True)

    pSecurityCipherWrpSource = srvSecurityComponent.createFileSymbol("SRV_SECURITY_CIPHER_WRP_SOURCE", None)
    pSecurityCipherWrpSource.setSourcePath("service/security/src/cipher_wrapper.c.ftl")
    pSecurityCipherWrpSource.setOutputName("cipher_wrapper.c")
    pSecurityCipherWrpSource.setDestPath("service/security")
    pSecurityCipherWrpSource.setProjectPath("config/" + configName + "/service/security/")
    pSecurityCipherWrpSource.setType("SOURCE")
    pSecurityCipherWrpSource.setMarkup(True)

    pSecurityCipherWrpHeader = srvSecurityComponent.createFileSymbol("SRV_SECURITY_CIPHER_WRP_HEADER", None)
    pSecurityCipherWrpHeader.setSourcePath("service/security/cipher_wrapper.h.ftl")
    pSecurityCipherWrpHeader.setOutputName("cipher_wrapper.h")
    pSecurityCipherWrpHeader.setDestPath("service/security")
    pSecurityCipherWrpHeader.setProjectPath("config/" + configName + "/service/security/")
    pSecurityCipherWrpHeader.setType("HEADER")
    pSecurityCipherWrpHeader.setMarkup(True)
