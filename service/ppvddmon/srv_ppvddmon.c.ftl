/*******************************************************************************
  PLC PVDD Monitor Service Library

  Company:
    Microchip Technology Inc.

  File Name:
    srv_ppvddmon.c

  Summary:
    PLC PVDD Monitor Service File

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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
<#compress> <#-- To remove unwanted new lines -->
<#assign PVDD_MON_ADC_INSTANCE = "">
<#assign PVDD_MON_MASK_PREFIX = "">

<#if (afec0.AFEC_INSTANCE_NAME)?has_content>
    <#assign PVDD_MON_ADC_INSTANCE = "AFEC0">
    <#assign PVDD_MON_MASK_PREFIX = "AFEC">
<#elseif (afec1.AFEC_INSTANCE_NAME)?has_content>
    <#assign PVDD_MON_ADC_INSTANCE = "AFEC1">
    <#assign PVDD_MON_MASK_PREFIX = "AFEC">
<#elseif (adc.ADC_INSTANCE_NAME)?has_content>
    <#assign PVDD_MON_ADC_INSTANCE = "ADC">
    <#assign PVDD_MON_MASK_PREFIX = "ADC">
</#if>

</#compress>

#include "device.h"
<#if core.CoreSysIntFile == true>
#include "interrupts.h"
</#if>
#include "srv_ppvddmon.h"
#include "peripheral/${PVDD_MON_ADC_INSTANCE?lower_case}/plib_${PVDD_MON_ADC_INSTANCE?lower_case}.h"

static SRV_PVDDMON_CMP_MODE srv_pvddmon_mode;

<#-- *********************************************************************************************** -->
// *****************************************************************************
// *****************************************************************************
// Section: PLC PVDD Monitor Service Implementation
// *****************************************************************************
// *****************************************************************************
/* Objects to hold callback function and context */
static SRV_PVDDMON_CALLBACK ${PVDD_MON_ADC_INSTANCE}_CompareCallback = NULL;

static void _${PVDD_MON_ADC_INSTANCE}_PVDDMONCallback( uint32_t status, uintptr_t context )
{
    if (status & ${PVDD_MON_MASK_PREFIX}_ISR_COMPE_Msk)
    {
        if (${PVDD_MON_ADC_INSTANCE}_CompareCallback)
        {
            ${PVDD_MON_ADC_INSTANCE}_CompareCallback(srv_pvddmon_mode, context);
        }
    }
}

/* Start PLC PVDD Monitor */
void SRV_PPVDDMON_Initialize (void)
{
    ${PVDD_MON_MASK_PREFIX}_CHANNEL_MASK channelMsk = (1 << ${SRV_PPVDDMON_ADC_CHANNEL});

    /* Disable ${PVDD_MON_ADC_INSTANCE} channel */
    ${PVDD_MON_ADC_INSTANCE}_ChannelsDisable(channelMsk);

    /* Disable channel interrupt */
    ${PVDD_MON_ADC_INSTANCE}_ChannelsInterruptDisable(channelMsk);
}

/* Start PLC PVDD Monitor */
void SRV_PPVDDMON_Start (SRV_PVDDMON_CMP_MODE cmpMode)
{
    uint32_t emr = 0;
    ${PVDD_MON_MASK_PREFIX}_CHANNEL_MASK channelMsk = (1 << ${SRV_PPVDDMON_ADC_CHANNEL});

    /* Set Free Run reset */
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_MR |= ${PVDD_MON_MASK_PREFIX}_MR_FREERUN_Msk;

    /* Set Compare Window Register */
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_CWR = ${PVDD_MON_MASK_PREFIX}_CWR_HIGHTHRES(SRV_PVDDMON_HIGH_TRESHOLD) | ${PVDD_MON_MASK_PREFIX}_CWR_LOWTHRES(SRV_PVDDMON_LOW_TRESHOLD); 

    /* Set Comparison Mode */
    if (cmpMode == SRV_PVDDMON_CMP_MODE_OUT)
    {
        srv_pvddmon_mode = SRV_PVDDMON_CMP_MODE_OUT;
        emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_OUT;
    }
    else
    {
        srv_pvddmon_mode = SRV_PVDDMON_CMP_MODE_IN;
        emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_IN;
    }

    /* Set Comparison Selected Channel */
    emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPSEL(${SRV_PPVDDMON_ADC_CHANNEL});

    /* Set Compare Type */
    emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPTYPE_Msk;

    /* Set Filter */
    emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPFILTER(3);
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_EMR = emr;

    /* Enable Comparison Event Interrupt */
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_IER |= ${PVDD_MON_MASK_PREFIX}_IER_COMPE_Msk;

    /* Enable ${PVDD_MON_ADC_INSTANCE} channel */
    ${PVDD_MON_ADC_INSTANCE}_ChannelsEnable(channelMsk);

    /* Comparison Restart */
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_CR = 0x1U << ${PVDD_MON_MASK_PREFIX}_CR_CMPRST_Pos;

    /* Start ${PVDD_MON_ADC_INSTANCE} conversion */
    ${PVDD_MON_ADC_INSTANCE}_ConversionStart();
}

/* Restart PLC PVDD Monitor */
void SRV_PPVDDMON_Restart (SRV_PVDDMON_CMP_MODE cmpMode)
{
    uint32_t emr;
    ${PVDD_MON_MASK_PREFIX}_CHANNEL_MASK channelMsk = (1 << ${SRV_PPVDDMON_ADC_CHANNEL});

    /* Disable channel COMPE interrupt */
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_IDR |= ${PVDD_MON_MASK_PREFIX}_IER_COMPE_Msk;

    /* Disable ${PVDD_MON_ADC_INSTANCE} channel */
    ${PVDD_MON_ADC_INSTANCE}_ChannelsDisable(channelMsk);

    /* Set Comparison Mode */
    if (cmpMode == SRV_PVDDMON_CMP_MODE_OUT)
    {
      srv_pvddmon_mode = SRV_PVDDMON_CMP_MODE_OUT;
      emr = ${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_OUT;
    }
    else
    {
      srv_pvddmon_mode = SRV_PVDDMON_CMP_MODE_IN;
      emr = ${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_IN;
    }
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_EMR &= ~${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_Msk;
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_EMR |= emr;

    /* Comparison Restart */
    while(${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_ISR & ${PVDD_MON_MASK_PREFIX}_ISR_COMPE_Msk);
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_CR = 0x1U << ${PVDD_MON_MASK_PREFIX}_CR_CMPRST_Pos;

    /* Enable ${PVDD_MON_ADC_INSTANCE} channel */
    ${PVDD_MON_ADC_INSTANCE}_ChannelsEnable(channelMsk);

    /* Enable Comparison Event Interrupt */
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_IER |= ${PVDD_MON_MASK_PREFIX}_IER_COMPE_Msk;
}

void SRV_PPVDDMON_RegisterCallback (SRV_PVDDMON_CALLBACK callback_fn, uintptr_t context)
{
    /* Register ${PVDD_MON_ADC_INSTANCE} Callback */
    ${PVDD_MON_ADC_INSTANCE}_CallbackRegister(_${PVDD_MON_ADC_INSTANCE}_PVDDMONCallback, context);
    ${PVDD_MON_ADC_INSTANCE}_CompareCallback = callback_fn;
}

bool SRV_PPVDDMON_CheckComparisonInWindow(void)
{
    uint16_t adcData;
    
    adcData = (uint16_t) ((${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_LCDR & ${PVDD_MON_MASK_PREFIX}_LCDR_LDATA_Msk) >> ${PVDD_MON_MASK_PREFIX}_LCDR_LDATA_Pos);
    
    if (adcData > SRV_PVDDMON_HIGH_TRESHOLD)
    {
        return false;
    }
    
    if (adcData < SRV_PVDDMON_LOW_TRESHOLD)
    {
        return false;
    }
    
    return true;
}