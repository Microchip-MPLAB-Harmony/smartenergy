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

/*
Copyright (C) 2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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
<#compress> <#-- To remove unwanted new lines -->
<#assign PVDD_MON_MASK_PREFIX = "">

<#if (afec0.AFEC_INSTANCE_NAME)?has_content>
    <#assign PVDD_MON_MASK_PREFIX = "AFEC">
<#elseif (afec1.AFEC_INSTANCE_NAME)?has_content>
    <#assign PVDD_MON_MASK_PREFIX = "AFEC">
<#elseif (adc.ADC_INSTANCE_NAME)?has_content>
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
#include "peripheral/${PVDD_MON_MASK_PREFIX?lower_case}/plib_${SRV_PVDDMON_PLIB?lower_case}.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data
// *****************************************************************************
// *****************************************************************************

static SRV_PVDDMON_CMP_MODE srv_pvddmon_mode;
static SRV_PVDDMON_CALLBACK ${SRV_PVDDMON_PLIB}_CompareCallback = NULL;


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************

<#if (PLC_ADC_ID??) && (PLC_ADC_ID == "44134")>
static void l${SRV_PVDDMON_PLIB}_PVDDMONCallback( uint32_t status, uint32_t eocStatus, uintptr_t context )
{
    /* Avoid warning */
    (void)eocStatus;

<#else>
static void l${SRV_PVDDMON_PLIB}_PVDDMONCallback( uint32_t status, uintptr_t context )
{
</#if>
    if ((status & ${PVDD_MON_MASK_PREFIX}_ISR_COMPE_Msk) != 0U)
    {
        if (${SRV_PVDDMON_PLIB}_CompareCallback != NULL)
        {
            ${SRV_PVDDMON_PLIB}_CompareCallback(srv_pvddmon_mode, context);
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

    /* Disable ${SRV_PVDDMON_PLIB} channel */
    ${SRV_PVDDMON_PLIB}_ChannelsDisable(channelMsk);

    /* Disable channel interrupt */
<#if (PLC_ADC_ID??) && (PLC_ADC_ID == "44134")>
    ${SRV_PVDDMON_PLIB}_ChannelsInterruptDisable((${PVDD_MON_MASK_PREFIX}_INTERRUPT_EOC_MASK)channelMsk);
<#else>
    ${SRV_PVDDMON_PLIB}_ChannelsInterruptDisable((${PVDD_MON_MASK_PREFIX}_INTERRUPT_MASK)channelMsk);
</#if>
}

void SRV_PVDDMON_Start (SRV_PVDDMON_CMP_MODE cmpMode)
{
    uint32_t emr = 0;
    ${PVDD_MON_MASK_PREFIX}_CHANNEL_MASK channelMsk = ${PVDD_MON_MASK_PREFIX}_CH${SRV_PVDDMON_ADC_CHANNEL}_MASK;

    /* Set Free Run reset */
<#if (PLC_ADC_ID??) && (PLC_ADC_ID == "44134")>
    ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_TRGR |= ${PVDD_MON_MASK_PREFIX}_TRGR_TRGMOD_CONTINUOUS;
<#else>
    ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_MR |= ${PVDD_MON_MASK_PREFIX}_MR_FREERUN_Msk;
</#if>

    /* Set Comparison Mode */
    if (cmpMode == SRV_PVDDMON_CMP_MODE_OUT)
    {
        srv_pvddmon_mode = SRV_PVDDMON_CMP_MODE_OUT;
        emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_OUT;
        /* Set Compare Window Register */
        ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_CWR = ${PVDD_MON_MASK_PREFIX}_CWR_HIGHTHRES(SRV_PVDDMON_HIGH_TRESHOLD) | ${PVDD_MON_MASK_PREFIX}_CWR_LOWTHRES(SRV_PVDDMON_LOW_TRESHOLD);
    }
    else
    {
        srv_pvddmon_mode = SRV_PVDDMON_CMP_MODE_IN;
        emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_IN;
        /* Set Compare Window Register */
        ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_CWR = ${PVDD_MON_MASK_PREFIX}_CWR_HIGHTHRES(SRV_PVDDMON_HIGH_TRESHOLD_HYST) | ${PVDD_MON_MASK_PREFIX}_CWR_LOWTHRES(SRV_PVDDMON_LOW_TRESHOLD_HYST);
    }

    /* Set Comparison Selected Channel */
    emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPSEL(${SRV_PVDDMON_ADC_CHANNEL});

<#if (adc.ADC_INSTANCE_NAME)?has_content>
    /* Set Compare Type */
    emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPTYPE_Msk;

</#if>
    /* Set Filter */
    emr |= ${PVDD_MON_MASK_PREFIX}_EMR_CMPFILTER(3);
    ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_EMR = emr;

    /* Enable Comparison Event Interrupt */
    ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_IER |= ${PVDD_MON_MASK_PREFIX}_IER_COMPE_Msk;

    /* Enable ${SRV_PVDDMON_PLIB} channel */
    ${SRV_PVDDMON_PLIB}_ChannelsEnable(channelMsk);

<#if (adc.ADC_INSTANCE_NAME)?has_content>
    /* Comparison Restart */
    ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_CR = 0x1U << ${PVDD_MON_MASK_PREFIX}_CR_CMPRST_Pos;

</#if>
    /* Start ${SRV_PVDDMON_PLIB} conversion */
    ${SRV_PVDDMON_PLIB}_ConversionStart();
}

void SRV_PVDDMON_Restart (SRV_PVDDMON_CMP_MODE cmpMode)
{
    uint32_t emr;
    ${PVDD_MON_MASK_PREFIX}_CHANNEL_MASK channelMsk = ${PVDD_MON_MASK_PREFIX}_CH${SRV_PVDDMON_ADC_CHANNEL}_MASK;

    /* Disable channel COMPE interrupt */
    ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_IDR |= ${PVDD_MON_MASK_PREFIX}_IER_COMPE_Msk;

    /* Disable ${SRV_PVDDMON_PLIB} channel */
    ${SRV_PVDDMON_PLIB}_ChannelsDisable(channelMsk);

    /* Set Comparison Mode */
    if (cmpMode == SRV_PVDDMON_CMP_MODE_OUT)
    {
        srv_pvddmon_mode = SRV_PVDDMON_CMP_MODE_OUT;
        emr = ${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_OUT;
        /* Set Compare Window Register */
        ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_CWR = ${PVDD_MON_MASK_PREFIX}_CWR_HIGHTHRES(SRV_PVDDMON_HIGH_TRESHOLD) | ${PVDD_MON_MASK_PREFIX}_CWR_LOWTHRES(SRV_PVDDMON_LOW_TRESHOLD);
    }
    else
    {
        srv_pvddmon_mode = SRV_PVDDMON_CMP_MODE_IN;
        emr = ${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_IN;
        /* Set Compare Window Register */
        ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_CWR = ${PVDD_MON_MASK_PREFIX}_CWR_HIGHTHRES(SRV_PVDDMON_HIGH_TRESHOLD_HYST) | ${PVDD_MON_MASK_PREFIX}_CWR_LOWTHRES(SRV_PVDDMON_LOW_TRESHOLD_HYST);
    }
    ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_EMR &= ~${PVDD_MON_MASK_PREFIX}_EMR_CMPMODE_Msk;
    ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_EMR |= emr;

    while((${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_ISR & ${PVDD_MON_MASK_PREFIX}_ISR_COMPE_Msk) != 0U){}

<#if (adc.ADC_INSTANCE_NAME)?has_content>
    /* Comparison Restart */
    ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_CR = 0x1U << ${PVDD_MON_MASK_PREFIX}_CR_CMPRST_Pos;

</#if>
    /* Enable ${SRV_PVDDMON_PLIB} channel */
    ${SRV_PVDDMON_PLIB}_ChannelsEnable(channelMsk);

    /* Enable Comparison Event Interrupt */
    ${SRV_PVDDMON_PLIB}_REGS->${PVDD_MON_MASK_PREFIX}_IER |= ${PVDD_MON_MASK_PREFIX}_IER_COMPE_Msk;
}

void SRV_PVDDMON_CallbackRegister (SRV_PVDDMON_CALLBACK callback, uintptr_t context)
{
    /* Register ${SRV_PVDDMON_PLIB} Callback */
    ${SRV_PVDDMON_PLIB}_CallbackRegister(l${SRV_PVDDMON_PLIB}_PVDDMONCallback, context);
    ${SRV_PVDDMON_PLIB}_CompareCallback = callback;
}

bool SRV_PVDDMON_CheckWindow(void)
{
    uint32_t adcValue;

    adcValue = ${SRV_PVDDMON_PLIB}_ChannelResultGet(${PVDD_MON_MASK_PREFIX}_CH${SRV_PVDDMON_ADC_CHANNEL});
    while(adcValue == 0U)
    {
        adcValue = ${SRV_PVDDMON_PLIB}_ChannelResultGet(${PVDD_MON_MASK_PREFIX}_CH${SRV_PVDDMON_ADC_CHANNEL});
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