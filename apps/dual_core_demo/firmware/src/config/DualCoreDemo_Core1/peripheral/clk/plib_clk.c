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
#include <stdbool.h>
#include "device.h"
#include "plib_clk.h"
/*********************************************************************************
                        Initialize Peripheral clocks
*********************************************************************************/
static uint32_t PeripheralClockStatus(uint32_t periphId)
{
    if (periphId < 32) {
		return ((PMC_REGS->PMC_CSR0 & PMC_CSR0_Msk) & (1 << periphId));
	} else if (periphId < 64) {
		return ((PMC_REGS->PMC_CSR1 & PMC_CSR1_Msk) & (1 << (periphId - 32)));
	} else if (periphId < 96) {
		return ((PMC_REGS->PMC_CSR2 & PMC_CSR2_Msk) & (1 << (periphId - 64)));
	} else if (periphId <= ID_PERIPH_MAX) {
		return ((PMC_REGS->PMC_CSR3 & PMC_CSR3_Msk) & (1 << (periphId - 96)));
	} else {
		return 0;
	}
}

static void PeripheralClockInitialize(void)
{
    struct {
        uint8_t id;
        uint8_t clken;
        uint8_t gclken;
        uint8_t css;
        uint8_t div;
    } periphList[] =
    {
        { ID_TC2_CHANNEL0, 1U, 0U, 0U, 0U},

        { ID_PIOD, 1U, 0U, 0U, 0U},

        { ID_IPC1, 1U, 0U, 0U, 0U},

        { ID_PERIPH_MAX + 1, 0, 0, 0, 0}//end of list marker
    };
    uint32_t count = sizeof(periphList)/sizeof(periphList[0]);
    uint32_t i = 0U;

    while((i < count) && (periphList[i].id != (ID_PERIPH_MAX + 1U)))
    {
        PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk |\
                            PMC_PCR_GCLKEN(periphList[i].gclken) |\
                            PMC_PCR_EN(periphList[i].clken) |\
                            PMC_PCR_GCLKDIV(periphList[i].div) |\
                            PMC_PCR_GCLKCSS(periphList[i].css) |\
                            PMC_PCR_PID(periphList[i].id);
        
        while(PeripheralClockStatus(periphList[i].id) == 0);
            
        i++;
    }
}

/*********************************************************************************
                                Clock Initialize
*********************************************************************************/
void CLK_Initialize( void )
{
    /* Initialize Peripheral clock */
    PeripheralClockInitialize();
}