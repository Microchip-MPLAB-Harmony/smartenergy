/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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

/*******************************************************************************
  MPLAB Harmony SLCDC Generated Driver Implementation File

  File Name:
    drv_gfx_SLCDC.c

  Summary:
    Build-time generated implementation for the SLCDC Driver for PIC32C MPU/MCUs.

  Description:
    Build-time generated implementation for the SLCDC Driver for PIC32C MPU/MCUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "drv_gfx_slcdc.h"
#include <stddef.h>


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************


#define CONF_COMM_SEL				8
#define CONF_SEG_SEL				20
#define CONF_BUFFER_TIME_SEL		SLCKx64
#define CONF_FRAME_PRESC_SEL		64
#define CONF_FRAME_DIV_SEL			2

#define BOARD_FREQ_SLCK_XTAL        (32768U)


slcdc_callback_t slcdc_callback_pointer = NULL;

/** The actual frame rate value */
static uint32_t g_frame_rate = 0;







void slcdc_get_config_defaults(struct slcdc_config *slcdc_cfg)
{
	slcdc_cfg->bias_buffer_duration = SLCDC_BIAS_1_3;
	slcdc_cfg->buf_time = SLCDC_BUFTIME_X64_SCLK;
	slcdc_cfg->framePrescaler = SLCDC_FRAME_PRE_64;
	slcdc_cfg->disp_mode = SLCDC_DISPMODE_NORMAL;
	slcdc_cfg->controller_power_mode = SLCDC_POWER_MODE_LCDON_INVR;
	slcdc_cfg->frame_rate = 64;
}




status_code_t slcdc_init(slcdc_registers_t *p_slcdc, struct slcdc_config *slcdc_cfg)
{
	if (slcdc_get_status(p_slcdc) == SLCDC_SR_ENA_Msk) {
		return ERR_BUSY;
	}

	/* SLCDC basic configuration */
	p_slcdc->SLCDC_MR = 
			((slcdc_cfg->bias_buffer_duration)<< SLCDC_MR_BIAS_Pos) |
			((CONF_COMM_SEL-1)<< SLCDC_MR_COMSEL_Pos) |
			SLCDC_MR_SEGSEL(CONF_SEG_SEL-1) |
			slcdc_cfg->buf_time;

	if (slcdc_set_frame_rate(p_slcdc, slcdc_cfg->frame_rate) != STATUS_OK) {
		return ERR_INVALID_ARG;
	}

	slcdc_set_display_mode(p_slcdc, slcdc_cfg->disp_mode);

	return STATUS_OK;
}

status_code_t slcdc_set_frame_rate(slcdc_registers_t *p_slcdc, uint32_t frame_rate)
{
	uint32_t ul_divisors[SLCDC_CLOCK_PRE_MAX] = {8, 16, 32, 64, 128, 256, 512, 1024};
	uint32_t ul_pre = 0;
	uint32_t ul_div;

	/* Find prescaler and divisor values */
	ul_div = (BOARD_FREQ_SLCK_XTAL / ul_divisors[ul_pre])
			/ frame_rate
			/ SLCDC_NCOM_VALUE;

	while ((ul_div > SLCDC_CLOCK_DIV_MAX) && (ul_pre < SLCDC_CLOCK_PRE_MAX)) {
		ul_pre++;
		ul_div = (BOARD_FREQ_SLCK_XTAL / ul_divisors[ul_pre])
				/ frame_rate
				/ SLCDC_NCOM_VALUE;
	}

	/* Return result */
	if (ul_pre < SLCDC_CLOCK_PRE_MAX) {
		p_slcdc->SLCDC_FRR = 0;
		p_slcdc->SLCDC_FRR = ((ul_div - 1) << SLCDC_FRR_DIV_Pos) |
				(ul_pre << SLCDC_FRR_PRESC_Pos);
		g_frame_rate = (BOARD_FREQ_SLCK_XTAL / ul_divisors[ul_pre])
				/ ul_div / SLCDC_NCOM_VALUE;
		return STATUS_OK;
	} else {
		return ERR_INVALID_ARG;
	}
}

status_code_t slcdc_set_blink_freq(slcdc_registers_t *p_slcdc, uint32_t value,
		bool is_period_freq)
{
	uint32_t ul_div;

	if (value == 0) {
		ul_div = 0;
	} else {
		if (is_period_freq) {
			ul_div = (g_frame_rate / value);
		} else {
			ul_div = (g_frame_rate * value);
		}
	}

	if (ul_div < 256) {
		p_slcdc->SLCDC_DR &= ~SLCDC_DR_LCDBLKFREQ_Msk;
		p_slcdc->SLCDC_DR |= SLCDC_DR_LCDBLKFREQ(ul_div);
		return STATUS_OK;
	} else {
		return ERR_INVALID_ARG;
	}
}

void slcdc_enable(slcdc_registers_t *p_slcdc)
{
	//sleepmgr_lock_mode(SLEEPMGR_BACKUP);
	p_slcdc->SLCDC_CR = SLCDC_CR_LCDEN_Msk;
}

void slcdc_disable(slcdc_registers_t *p_slcdc)
{
	//sleepmgr_unlock_mode(SLEEPMGR_BACKUP);
	p_slcdc->SLCDC_CR = SLCDC_CR_LCDDIS_Msk;
}

void slcdc_set_display_memory(slcdc_registers_t *p_slcdc)
{
	uint8_t tmp = 8; /* SLCDC_COM_NUM */

	while (tmp--) {
		slcdc_set_lsb_memory(p_slcdc, tmp, ~0UL);
	}
}

void slcdc_clear_display_memory(slcdc_registers_t *p_slcdc)
{
	uint8_t tmp = 8; /* SLCDC_COM_NUM */

	while (tmp--) {
		slcdc_clear_lsb_memory(p_slcdc, tmp, ~0UL);
	}
}

void slcdc_set_callback(slcdc_registers_t *p_slcdc, slcdc_callback_t callback,
		uint32_t ul_sources, uint8_t irq_line, uint8_t irq_level)
{
	slcdc_callback_pointer = callback;
	NVIC_ClearPendingIRQ((IRQn_Type)irq_line);
	NVIC_SetPriority((IRQn_Type)irq_line, irq_level);
	NVIC_EnableIRQ((IRQn_Type)irq_line);
	slcdc_enable_interrupt(p_slcdc, ul_sources);
}

void SLCDC_Handler(void)
{
	/* Interrupt handler */
	if (slcdc_callback_pointer != NULL) {
		slcdc_callback_pointer();
	}
}

/*******************************************************************************
 End of File
*/
