/*******************************************************************************
  PL360 Binary file Assembler

  Summary:
    Library for Microchip PLC Stack
    
  Description:
    - Join PL360 PHY binary file together PLC application
*******************************************************************************/

/*****************************************************************************
 Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.

Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software 
and any derivatives exclusively with Microchip products. It is your 
responsibility to comply with third party license terms applicable to your 
use of third party software (including open source software) that may 
accompany Microchip software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR 
PURPOSE.

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE 
FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN 
ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, 
THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************/

  .section .rodata
  .global pl360_bin_start
  .global pl360_bin_end
<#if DRV_PL360_PLC_PROFILE == "5">
  .global pl360_bin2_start
  .global pl360_bin2_end
</#if>

  .align 8
pl360_bin_start:
<#if (DRV_PL360_PLC_PROFILE == "0") || (DRV_PL360_PLC_PROFILE == "5")>
  .incbin "./PL360_G3_CENA.bin"
<#elseif DRV_PL360_PLC_PROFILE == "1">
  .incbin "./PL360_G3_CENB.bin"
<#elseif DRV_PL360_PLC_PROFILE == "2">
  .incbin "./PL360_G3_FCC.bin"
<#elseif DRV_PL360_PLC_PROFILE == "4">
  .incbin "./PL360_PRIME.bin"
</#if>
  .align 8
pl360_bin_end:
<#if DRV_PL360_PLC_PROFILE == "5">
pl360_bin2_start:
  .incbin "./PL360_G3_FCC.bin"
  .align 8
pl360_bin2_end:
</#if>