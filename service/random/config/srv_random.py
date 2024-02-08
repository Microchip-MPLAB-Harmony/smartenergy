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

def instantiateComponent(srvRandomComponent):

    Log.writeInfoMessage("Loading SE Random service")

    # Activate TRNG if exists
    Database.activateComponents(["trng"])

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    # Random Files
    pRandomSource = srvRandomComponent.createFileSymbol("SRV_RANDOM_SOURCE", None)
    pRandomSource.setSourcePath("service/random/srv_random.c.ftl")
    pRandomSource.setOutputName("srv_random.c")
    pRandomSource.setDestPath("service/random")
    pRandomSource.setProjectPath("config/" + configName + "/service/random/")
    pRandomSource.setType("SOURCE")
    pRandomSource.setMarkup(True)

    pRandomHeader = srvRandomComponent.createFileSymbol("SRV_RANDOM_HEADER", None)
    pRandomHeader.setSourcePath("service/random/srv_random.h")
    pRandomHeader.setOutputName("srv_random.h")
    pRandomHeader.setDestPath("service/random")
    pRandomHeader.setProjectPath("config/" + configName + "/service/random/")
    pRandomHeader.setType("HEADER")