/*******************************************************************************
  G3 MAC Real Time Binary file Assembler

  Summary:
    Library for Microchip PLC Stack
    
  Description:
    - Join G3 MAC Real Time binary file together PLC application
*******************************************************************************/

/*****************************************************************************
 Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.

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
  .global g3_mac_rt_bin_start
  .global g3_mac_rt_bin_end
  .global g3_mac_rt_bin2_start
  .global g3_mac_rt_bin2_end

  .align 8
g3_mac_rt_bin_start:
<#if DRV_PLC_BAND_IN_USE == 1>
  .incbin "./G3_MAC_RT_CENA.bin"
<#elseif DRV_PLC_BAND_IN_USE == 2 || DRV_PLC_BAND_IN_USE == 5 || DRV_PLC_BAND_IN_USE == 6>
  .incbin "./G3_MAC_RT_FCC.bin"
<#elseif DRV_PLC_BAND_IN_USE == 3 || DRV_PLC_BAND_IN_USE == 7 || DRV_PLC_BAND_IN_USE == 8>
  .incbin "./G3_MAC_RT_G3_ARIB.bin"
<#elseif DRV_PLC_BAND_IN_USE == 4>
  .incbin "./G3_MAC_RT_CENB.bin"
</#if>
  .align 8
g3_mac_rt_bin_end:
<#if (DRV_PLC_BAND_IN_USE == 5) || (DRV_PLC_BAND_IN_USE == 7)>
g3_mac_rt_bin2_start:
  .incbin "./G3_MAC_RT_CENA.bin"
  .align 8
g3_mac_rt_bin2_end:
<#elseif (DRV_PLC_BAND_IN_USE == 6) || (DRV_PLC_BAND_IN_USE == 8)>
g3_mac_rt_bin2_start:
  .incbin "./G3_MAC_RT_CENB.bin"
  .align 8
g3_mac_rt_bin2_end:
<#else>
g3_mac_rt_bin2_start:
g3_mac_rt_bin2_end:
</#if>
