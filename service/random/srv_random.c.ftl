/*******************************************************************************
  Source for the pseudo-random numbers generation service

  Company:
    Microchip Technology Inc.

  File Name:
    srv_random.c

  Summary:
    Interface implementation for the pseudo-random numbers generation service.

  Description:
    This file implements the interface for the pseudo-random numbers generation 
    service.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
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
*******************************************************************************/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "definitions.h"
<#if (trng)??>
<#elseif (lib_crypto)??>
#include "crypto/crypto.h"
<#else>
#include <rand.h>
</#if>
#include "srv_random.h"

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

uint8_t SRV_RANDOM_Get8bits(void)
{
  uint8_t retValue;

<#if (trng)??>
  retValue = (uint8_t)TRNG_ReadData();
<#elseif (lib_crypto)??>
  CRYPT_RNG_CTX rngCtx;

  CRYPT_RNG_Initialize(&rngCtx);
  CRYPT_RNG_Get(&rngCtx, (unsigned char*)&retValue);
  CRYPT_RNG_Deinitialize(&rngCtx);
<#else>
  uint32_t seed; // use uninitialized value

  srand(seed);
  retValue = (uint8_t)rand();
</#if>

  return retValue;
}

uint16_t SRV_RANDOM_Get16bits(void)
{
  uint16_t retValue;

<#if (trng)??>
  retValue = (uint16_t)TRNG_ReadData();
<#elseif (lib_crypto)??>
  CRYPT_RNG_CTX rngCtx;

  CRYPT_RNG_Initialize(&rngCtx);
  CRYPT_RNG_BlockGenerate(&rngCtx, (unsigned char*)&retValue, 2);
  CRYPT_RNG_Deinitialize(&rngCtx);
<#else>
  uint32_t seed; // use uninitialized value

  srand(seed);
  retValue = (uint16_t)rand();
</#if>

  return retValue;
}

uint16_t  SRV_RANDOM_Get16bitsInRange(uint16_t min, uint16_t max)
{
  uint16_t localMin = min;

  if (max < min) 
  {
    localMin = max;
    max = min;
  }

  return (SRV_RANDOM_Get16bits() % (max - localMin + 1) + localMin);
}

uint32_t SRV_RANDOM_Get32bits(void)
{
  uint32_t retValue;

<#if (trng)??>
  retValue = TRNG_ReadData();
<#elseif (lib_crypto)??>
  CRYPT_RNG_CTX rngCtx;

  CRYPT_RNG_Initialize(&rngCtx);
  CRYPT_RNG_BlockGenerate(&rngCtx, (unsigned char*)&retValue, 4);
  CRYPT_RNG_Deinitialize(&rngCtx);
<#else>
  uint32_t seed; // use uninitialized value

  srand(seed);
  retValue = rand();
</#if>

  return retValue;
}

uint32_t SRV_RANDOM_Get32bitsInRange(uint32_t min, uint32_t max)
{
  uint32_t localMin = min;

  if (max < min) 
  {
    localMin = max;
    max = min;
  }

  return (SRV_RANDOM_Get32bits() % (max - localMin + 1) + localMin);
}

void SRV_RANDOM_Get128bits(uint8_t *rndValue)
{
<#if (trng)??>
  uint32_t randNum;
  uint8_t n;

  for (n = 0; n < 4; n ++)
  {
    randNum = TRNG_ReadData();

    *rndValue++ = (uint8_t)(randNum >> 24);
    *rndValue++ = (uint8_t)(randNum >> 16);
    *rndValue++ = (uint8_t)(randNum >> 8);
    *rndValue++ = (uint8_t)(randNum);
  }
<#elseif (lib_crypto)??>
  CRYPT_RNG_CTX rngCtx;

  CRYPT_RNG_Initialize(&rngCtx);
  CRYPT_RNG_BlockGenerate(&rngCtx, (unsigned char*)rndValue, 16);
  CRYPT_RNG_Deinitialize(&rngCtx);
<#else>
  uint32_t seed; // use uninitialized value
  uint32_t randNum;
  uint8_t n;

  srand(seed);

  for (n = 0; n < 4; n ++)
  {
    randNum = rand();

    *rndValue++ = (uint8_t)(randNum >> 24);
    *rndValue++ = (uint8_t)(randNum >> 16);
    *rndValue++ = (uint8_t)(randNum >> 8);
    *rndValue++ = (uint8_t)(randNum);
  }
</#if>
}