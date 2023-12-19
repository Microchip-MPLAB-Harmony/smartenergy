 /*******************************************************************************
  Header for the cipher wrapper between G3 stack and Crypto

  Company:
    Microchip Technology Inc.

  File Name:
    cipher_wrapper.h

  Summary:
    Interface definition of the wrapper between G3 and Crypto.

  Description:
    This file defines the interface for the wrapper between G3 and Crypto.
    It includes calls to handle CCM, CMAC and EAX.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
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
*/
//DOM-IGNORE-END

#ifndef CIPHER_WRAPPER_H
#define CIPHER_WRAPPER_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>

#ifdef __cplusplus // Provide C++ Compatibility
 extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Constant Definitions
// *****************************************************************************
// *****************************************************************************

/* Successful function return */
#define CIPHER_WRAPPER_RETURN_GOOD    0

// *****************************************************************************
// *****************************************************************************
// Section: Service Interface Functions
// *****************************************************************************
// *****************************************************************************

//******************************************************************************
/* Function:
    int32_t CIPHER_Wrapper_AesCmacDirect(uint8_t *input, uint32_t inputLen,
                                         uint8_t *outputMac, uint8_t *key)

  Summary:
    Performs AES-CMAC to generate the MAC in single step without initialization.

  Description:
    This function performs AES-CMAC to generate the MAC in single step without
    initialization. To verify, the user can compare the generated MAC with the
    MAC received from sender with message. The size of the key is 16 bytes. The
    size of the MAC is equal to the AES block size (16 bytes).

  Precondition:
    None.

  Parameters:
    input     - Pointer to buffer holding the input data.
    inputLen  - Length of the input data in bytes.
    outputMac - Pointer to store the output data (MAC). The size of the buffer
                must be equal or larger than the AES block size (16 bytes).
    key       - Pointer to buffer holding the 16-byte key itself.

  Returns:
    - CIPHER_WRAPPER_RETURN_GOOD: Successful process.
    - Any other value: Error in the process.

  Example:
    <code>
    int32_t ret;
    uint8_t in1[] = { some input data };
    uint8_t out1[16];
    uint8_t key[16] = { some key };

    ret = CIPHER_Wrapper_AesCmacDirect(in1, sizeof(in1), out1, key);
    </code>

  Remarks:
    None.
*/

int32_t CIPHER_Wrapper_AesCmacDirect(uint8_t *input, uint32_t inputLen,
                                     uint8_t *outputMac, uint8_t *key);

//******************************************************************************
/* Function:
    int32_t CIPHER_Wrapper_AesCcmSetkey(uint8_t *key)

  Summary:
    Initializes the AES-CCM context and sets the encryption key.

  Description:
    This function initializes the AES-CCM context and sets the 16-byte
    encryption key.

  Precondition:
    None.

  Parameters:
    key - Pointer to buffer holding the 16-byte key itself.

  Returns:
    - CIPHER_WRAPPER_RETURN_GOOD: Successful initialization.
    - Any other value: Error in the initialization.

  Example:
    <code>
    int32_t ret;
    uint8_t key[16] = { some key };
    ret = CIPHER_Wrapper_AesCcmSetkey(key);
    </code>

  Remarks:
    None.
*/

int32_t CIPHER_Wrapper_AesCcmSetkey(uint8_t *key);

//******************************************************************************
/* Function:
    int32_t CIPHER_Wrapper_AesCcmAuthDecrypt(uint32_t length,
                                             uint8_t *iv, uint32_t ivLen,
                                             uint8_t *add, uint32_t addLen,
                                             uint8_t *input, uint8_t *output,
                                             uint8_t *tag, uint32_t tagLen)

  Summary:
    Performs AES-CCM authenticated decryption of a buffer.

  Description:
    This function performs a AES-CCM authenticated decryption of a buffer.

  Precondition:
    Key must be set earlier with a call to CIPHER_Wrapper_AesCcmSetkey.

  Parameters:
    length - Length of the input data in bytes.
    iv     - Pointer to initialization vector (nonce).
    ivLen  - Length of the nonce in bytes.
    add    - Pointer to additional data field.
    addLen - Length of additional data in bytes.
    input  - Pointer to buffer holding the input ciphered data.
    output - Pointer to store the output plain data. The size of the buffer
             must be equal or larger than the input data.
    tag    - Pointer to buffer holding the authentication tag.
    tagLen - Length of the authentication tag in bytes.

  Returns:
    - CIPHER_WRAPPER_RETURN_GOOD: Successful decryption.
    - Any other value: Error in the decryption.

  Example:
    <code>
    int32_t ret;
    uint8_t nonce[] = { some initialization nonce };
    uint8_t cipher[] = { encrypted message };
    uint8_t plain[sizeof(cipher)];
    uint8_t authIn[] = { some authentication input };
    uint8_t tag[] = { authentication tag received for verification };
    uint8_t key[16] = { some key };

    ret = CIPHER_Wrapper_AesCcmSetkey(key, sizeof(key));
    ret = CIPHER_Wrapper_AesCcmAuthDecrypt(sizeof(plain), nonce, sizeof(nonce),
                                           authIn, sizeof(authIn),
                                           cipher, plain,
                                           tag, sizeof(tag));
    </code>

  Remarks:
    None.
*/

int32_t CIPHER_Wrapper_AesCcmAuthDecrypt(uint32_t length,
                                         uint8_t *iv, uint32_t ivLen,
                                         uint8_t *add, uint32_t addLen,
                                         uint8_t *input, uint8_t *output,
                                         uint8_t *tag, uint32_t tagLen);

//******************************************************************************
/* Function:
    int32_t CIPHER_Wrapper_AesCcmEncryptAndTag(uint32_t length,
                                               uint8_t *iv, uint32_t ivLen,
                                               uint8_t *add, uint32_t addLen,
                                               uint8_t *input, uint8_t *output,
                                               uint8_t *tag, uint32_t tagLen)

  Summary:
    Performs AES-CCM authenticated encryption of a buffer.

  Description:
    This function performs AES-CCM authenticated encryption of a buffer.

  Precondition:
    Key must be set earlier with a call to CIPHER_Wrapper_AesCcmSetkey.

  Parameters:
    length - Length of the input data in bytes.
    iv     - Pointer to initialization vector (nonce).
    ivLen  - Length of the nonce in bytes.
    add    - Pointer to additional data field.
    addLen - Length of additional data in bytes.
    input  - Pointer to buffer holding the input plain data.
    output - Pointer to store the output ciphered data. The size of the buffer
             must be equal or larger than the input data.
    tag    - Pointer to sotore the authentication tag.
    tagLen - Length of the authentication tag in bytes.

  Returns:
    - CIPHER_WRAPPER_RETURN_GOOD: Successful encryption.
    - Any other value: Error in the encryption.

  Example:
    <code>
    int32_t ret;
    uint8_t nonce[] = { some initialization nonce };
    uint8_t plain[] = { some plain message };
    uint8_t cipher[sizeof(plain)];
    uint8_t authIn[] = { some authentication input };
    uint8_t tag[];
    uint8_t key[16] = { some key };

    ret = CIPHER_Wrapper_AesCcmSetkey(key);
    ret = CIPHER_Wrapper_AesCcmEncryptAndTag(sizeof(cipher), nonce, sizeof(nonce),
                                             authIn, sizeof(authIn),
                                             plain, cipher,
                                             tag, sizeof(tag));
    </code>

  Remarks:
    None.
*/

int32_t CIPHER_Wrapper_AesCcmEncryptAndTag(uint32_t length,
                                           uint8_t *iv, uint32_t ivLen,
                                           uint8_t *add, uint32_t addLen,
                                           uint8_t *input, uint8_t *output,
                                           uint8_t *tag, uint32_t tagLen);

//******************************************************************************
/* Function:
    int32_t CIPHER_Wrapper_EaxInitKey(const uint8_t *key, uint32_t keyLen)

  Summary:
    Initializes the EAX context and sets the key.

  Description:
    This function initializes the EAX context and sets the key.

  Precondition:
    The crypto initialization routines should be called before calling this
    routine (in "SYS_Initialize").

  Parameters:
    key    - Pointer to buffer holding the key itself.
    keyLen - Length of key in bytes.

  Returns:
    - CIPHER_WRAPPER_RETURN_GOOD: Successful initialization.
    - Any other value: Error in the initialization.

  Example:
    <code>
    int32_t ret;
    uint8_t key[] = { some key };
    ret = CIPHER_Wrapper_EaxInitKey(key, sizeof(key));
    </code>

  Remarks:
    None.
*/

int32_t CIPHER_Wrapper_EaxInitKey(const uint8_t *key, uint32_t keyLen);

//******************************************************************************
/* Function:
    int32_t CIPHER_Wrapper_EaxEncrypt(const uint8_t *iv, uint32_t ivLen,
                                      const uint8_t *hdr, uint32_t hdrLen,
                                      uint8_t *msg, uint32_t msgLen,
                                      uint8_t *tag, uint32_t tagLen)

  Summary:
    Encrypts a message using the EAX context created earlier.

  Description:
    This function encrypts a message using the EAX context created earlier.

  Precondition:
    Key must be set earlier with a call to CIPHER_Wrapper_EaxInitKey.

  Parameters:
    iv      - Pointer to initialization vector (nonce).
    ivLen   - Length of the nonce in bytes.
    hdr     - Pointer to buffer holding the header.
    hdrLen  - Length of header in bytes.
    msg     - Pointer to buffer holding the message data.
    msgLen  - Length of the message data.
    tag     - Pointer to buffer holding the tag.
    tagLen  - Length of the tag in bytes.

  Returns:
    - CIPHER_WRAPPER_RETURN_GOOD: Successful encryption.
    - Any other value: Error in the encryption.

  Example:
    <code>
  int32_t ret;
    nonce[] = { some initialization nonce };
    header[] = { some header };
    message[] = { some plain text message };
    tag[AES_BLOCK_SIZE];
    uint8_t key[] = { some key };

    ret = CIPHER_Wrapper_EaxInitKey(key, sizeof(key));
    ret = CIPHER_Wrapper_EaxEncrypt(nonce, sizeof(nonce), hdr, sizeof(hdr),
                                    message, sizeof(message), tag, sizeof(tag));
    </code>

  Remarks:
    None.
*/

int32_t CIPHER_Wrapper_EaxEncrypt(const uint8_t *iv, uint32_t ivLen,
                                  const uint8_t *hdr, uint32_t hdrLen,
                                  uint8_t *msg, uint32_t msgLen,
                                  uint8_t *tag, uint32_t tagLen);

//******************************************************************************
/* Function:
    int32_t CIPHER_Wrapper_EaxDecrypt(const uint8_t *iv, uint32_t ivLen,
                                      const uint8_t *hdr, uint32_t hdrLen,
                                      uint8_t *msg, uint32_t msgLen,
                                      const uint8_t *tag, uint32_t tagLen)

  Summary:
    Decrypts a message using the EAX context created earlier.

  Description:
    This function decrypts a message using the EAX context created earlier.

  Precondition:
    Key must be set earlier with a call to CIPHER_Wrapper_EaxInitKey.

  Parameters:
    iv      - Pointer to initialization vector (nonce).
    ivLen   - Length of the nonce in bytes.
    hdr     - Pointer to buffer holding the header.
    hdrLen  - Length of header in bytes.
    msg     - Pointer to buffer holding the message data.
    msgLen  - Length of the message data.
    tag     - Pointer to buffer holding the tag.
    tagLen  - Length of the tag in bytes.

  Returns:
    - CIPHER_WRAPPER_RETURN_GOOD: Successful decryption.
    - Any other value: Error in the decryption.

  Example:
    <code>
    int32_t ret;
    nonce[] = { some initialization nonce };
    header[] = { some header };
    message[] = { some cipher text message };
    uint8_t tag[AES_BLOCK_SIZE] = { authentication tag received for verification };
    uint8_t key[] = { some key };

    ret = CIPHER_Wrapper_EaxInitKey(key, sizeof(key));
    ret = CIPHER_Wrapper_EaxDecrypt(nonce, sizeof(nonce), hdr, sizeof(hdr),
                                    message, sizeof(message), tag, sizeof(tag));
    </code>

  Remarks:
    None.
*/

int32_t CIPHER_Wrapper_EaxDecrypt(const uint8_t *iv, uint32_t ivLen,
                                  const uint8_t *hdr, uint32_t hdrLen,
                                  uint8_t *msg, uint32_t msgLen,
                                  const uint8_t *tag, uint32_t tagLen);

//******************************************************************************
/* Function:
    int32_t CIPHER_Wrapper_EaxEnd(void)

  Summary:
    Cleans up the EAX context and ends the EAX operation.

  Description:
    This function cleans up the EAX context and ends the EAX operation.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    - CIPHER_WRAPPER_RETURN_GOOD: Successful termination.
    - Any other value: Error in the termination.

  Example:
    <code>
    int32_t ret;
    ret = CIPHER_Wrapper_EaxEnd();
    </code>

  Remarks:
    None.
*/

int32_t CIPHER_Wrapper_EaxEnd(void);

#ifdef __cplusplus // Provide C++ Compatibility
 }
#endif

#endif /* CIPHER_WRAPPER_H */
