/*******************************************************************************
  PLC PVDD Monitor Service Library

  Company:
    Microchip Technology Inc.

  File Name:
    srv_pvddmon.c

  Summary:
    PLC PVDD Monitor Service File

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
<#if core.CoreSysIntFile == true>
#include "interrupts.h"
</#if>
#include "srv_pvddmon.h"
#include "peripheral/${PVDD_MON_MASK_PREFIX?lower_case}/plib_${PVDD_MON_ADC_INSTANCE?lower_case}.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data
// *****************************************************************************
// *****************************************************************************

static SRV_PVDDMON_CMP_MODE srv_pvddmon_mode;
static SRV_PVDDMON_CALLBACK ${PVDD_MON_ADC_INSTANCE}_CompareCallback = NULL;


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************

<#if (PLC_ADC_ID??) && (PLC_ADC_ID == 44134)>
static void l${PVDD_MON_ADC_INSTANCE}_PVDDMONCallback( uint32_t status, uint32_t eocStatus, uintptr_t context )
{
    /* Avoid warning */
    (void)eocStatus;
    
<#else>
static void l${PVDD_MON_ADC_INSTANCE}_PVDDMONCallback( uint32_t status, uintptr_t context )
{
</#if>
    if ((status & ${PVDD_MON_MASK_PREFIX}_ISR_COMPE_Msk) != 0U)
    {
        if (${PVDD_MON_ADC_INSTANCE}_CompareCallback != NULL)
        {
            ${PVDD_MON_ADC_INSTANCE}_CompareCallback(srv_pvddmon_mode, context);
        }
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: PLC PVDD Monitor Service Interface Implementation
// *****************************************************************************
// *****************************************************************************

void SRV_PVDDMON_Initialize (void)
{
    ${PVDD_MON_MASK_PREFIX}_CHANNEL_MASK channelMsk = ${PVDD_MON_MASK_PREFIX}_CH${SRV_PVDDMON_ADC_CHANNEL}_MASK;

    /* Disable ${PVDD_MON_ADC_INSTANCE} channel */
    ${PVDD_MON_ADC_INSTANCE}_ChannelsDisable(channelMsk);

    /* Disable channel interrupt */
<#if (PLC_ADC_ID??) && (PLC_ADC_ID == 44134)>
    ${PVDD_MON_ADC_INSTANCE}_ChannelsInterruptDisable((${PVDD_MON_MASK_PREFIX}_INTERRUPT_EOC_MASK)channelMsk);
<#else>
    ${PVDD_MON_ADC_INSTANCE}_ChannelsInterruptDisable((${PVDD_MON_MASK_PREFIX}_INTERRUPT_MASK)channelMsk);
</#if>
}

void SRV_PVDDMON_Start (SRV_PVDDMON_CMP_MODE cmpMode)
{
    uint32_t emr = 0;
    ${PVDD_MON_MASK_PREFIX}_CHANNEL_MASK channelMsk = ${PVDD_MON_MASK_PREFIX}_CH${SRV_PVDDMON_ADC_CHANNEL}_MASK;

    /* Set Free Run reset */
<#if (PLC_ADC_ID??) && (PLC_ADC_ID == 44134)>
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_TRGR |= ${PVDD_MON_MASK_PREFIX}_TRGR_TRGMOD_CONTINUOUS;
<#else>
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_MR |= ${PVDD_MON_MASK_PREFIX}_MR_FREERUN_Msk;
</#if>

    /* Set Comparison Mode */
    if (cmpMode == SRV_PVDDMON_CMP_MODE_OUT)
    {
        srv_pvddmon_mode = SRV_PVDDMON_CMP_MODE_OUT;
        emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_OUT;
        /* Set Compare Window Register */
        ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_CWR = ${PVDD_MON_MASK_PREFIX}_CWR_HIGHTHRES(SRV_PVDDMON_HIGH_TRESHOLD) | ${PVDD_MON_MASK_PREFIX}_CWR_LOWTHRES(SRV_PVDDMON_LOW_TRESHOLD);
    }
    else
    {
        srv_pvddmon_mode = SRV_PVDDMON_CMP_MODE_IN;
        emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_IN;
        /* Set Compare Window Register */
        ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_CWR = ${PVDD_MON_MASK_PREFIX}_CWR_HIGHTHRES(SRV_PVDDMON_HIGH_TRESHOLD_HYST) | ${PVDD_MON_MASK_PREFIX}_CWR_LOWTHRES(SRV_PVDDMON_LOW_TRESHOLD_HYST); 
    }

    /* Set Comparison Selected Channel */
    emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPSEL(${SRV_PVDDMON_ADC_CHANNEL});

<#if (adc.ADC_INSTANCE_NAME)?has_content>
    /* Set Compare Type */
    emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPTYPE_Msk;

</#if>
    /* Set Filter */
    emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPFILTER(3);
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_EMR = emr;

    /* Enable Comparison Event Interrupt */
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_IER |= ${PVDD_MON_MASK_PREFIX}_IER_COMPE_Msk;

    /* Enable ${PVDD_MON_ADC_INSTANCE} channel */
    ${PVDD_MON_ADC_INSTANCE}_ChannelsEnable(channelMsk);

<#if (adc.ADC_INSTANCE_NAME)?has_content>
    /* Comparison Restart */
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_CR = 0x1U << ${PVDD_MON_MASK_PREFIX}_CR_CMPRST_Pos;

</#if>
    /* Start ${PVDD_MON_ADC_INSTANCE} conversion */
    ${PVDD_MON_ADC_INSTANCE}_ConversionStart();
}

void SRV_PVDDMON_Restart (SRV_PVDDMON_CMP_MODE cmpMode)
{
    uint32_t emr;
    ${PVDD_MON_MASK_PREFIX}_CHANNEL_MASK channelMsk = ${PVDD_MON_MASK_PREFIX}_CH${SRV_PVDDMON_ADC_CHANNEL}_MASK;

    /* Disable channel COMPE interrupt */
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_IDR |= ${PVDD_MON_MASK_PREFIX}_IER_COMPE_Msk;

    /* Disable ${PVDD_MON_ADC_INSTANCE} channel */
    ${PVDD_MON_ADC_INSTANCE}_ChannelsDisable(channelMsk);

    /* Set Comparison Mode */
    if (cmpMode == SRV_PVDDMON_CMP_MODE_OUT)
    {
        srv_pvddmon_mode = SRV_PVDDMON_CMP_MODE_OUT;
        emr = ${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_OUT;
        /* Set Compare Window Register */
        ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_CWR = ${PVDD_MON_MASK_PREFIX}_CWR_HIGHTHRES(SRV_PVDDMON_HIGH_TRESHOLD) | ${PVDD_MON_MASK_PREFIX}_CWR_LOWTHRES(SRV_PVDDMON_LOW_TRESHOLD);
    }
    else
    {
        srv_pvddmon_mode = SRV_PVDDMON_CMP_MODE_IN;
        emr = ${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_IN;
        /* Set Compare Window Register */
        ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_CWR = ${PVDD_MON_MASK_PREFIX}_CWR_HIGHTHRES(SRV_PVDDMON_HIGH_TRESHOLD_HYST) | ${PVDD_MON_MASK_PREFIX}_CWR_LOWTHRES(SRV_PVDDMON_LOW_TRESHOLD_HYST); 
    }
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_EMR &= ~${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_Msk;
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_EMR |= emr;

    while((${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_ISR & ${PVDD_MON_MASK_PREFIX}_ISR_COMPE_Msk) != 0U){}

<#if (adc.ADC_INSTANCE_NAME)?has_content>
    /* Comparison Restart */
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_CR = 0x1U << ${PVDD_MON_MASK_PREFIX}_CR_CMPRST_Pos;

</#if>
    /* Enable ${PVDD_MON_ADC_INSTANCE} channel */
    ${PVDD_MON_ADC_INSTANCE}_ChannelsEnable(channelMsk);

    /* Enable Comparison Event Interrupt */
    ${PVDD_MON_ADC_INSTANCE}_REGS->${PVDD_MON_MASK_PREFIX}_IER |= ${PVDD_MON_MASK_PREFIX}_IER_COMPE_Msk;
}

void SRV_PVDDMON_CallbackRegister (SRV_PVDDMON_CALLBACK callback, uintptr_t context)
{
    /* Register ${PVDD_MON_ADC_INSTANCE} Callback */
    ${PVDD_MON_ADC_INSTANCE}_CallbackRegister(l${PVDD_MON_ADC_INSTANCE}_PVDDMONCallback, context);
    ${PVDD_MON_ADC_INSTANCE}_CompareCallback = callback;
}

bool SRV_PVDDMON_CheckWindow(void)
{
    uint32_t adcValue;
    
    adcValue = ${PVDD_MON_ADC_INSTANCE}_ChannelResultGet(${PVDD_MON_MASK_PREFIX}_CH${SRV_PVDDMON_ADC_CHANNEL});
    while(adcValue == 0U)
    {
        adcValue = ${PVDD_MON_ADC_INSTANCE}_ChannelResultGet(${PVDD_MON_MASK_PREFIX}_CH${SRV_PVDDMON_ADC_CHANNEL});
    }
    
    if ((adcValue <= SRV_PVDDMON_HIGH_TRESHOLD) && (adcValue >= SRV_PVDDMON_LOW_TRESHOLD))
    {
        return true;
    }
    else
    {
        return false;
    }
}