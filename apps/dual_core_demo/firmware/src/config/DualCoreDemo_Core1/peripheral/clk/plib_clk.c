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
                        Check Peripheral clock status
*********************************************************************************/
static bool PeripheralClockStatus(uint32_t periph_id)
{
    bool retval = false;
    uint32_t status = 0U;
    const uint32_t csr_offset[] = { PMC_CSR0_REG_OFST,
                                    PMC_CSR1_REG_OFST,
                                    PMC_CSR2_REG_OFST,
                                    PMC_CSR3_REG_OFST
                                    };
    uint32_t index = periph_id / 32U;
    if (index < (sizeof(csr_offset)/sizeof(csr_offset[0])))
    {
        status = (*(volatile uint32_t* const)((PMC_BASE_ADDRESS +
                                                        csr_offset[index])));
        retval = ((status & (1 << (periph_id % 32U))) != 0U);
    }
    return retval;
}


/*********************************************************************************
                        Initialize Peripheral clocks
*********************************************************************************/
static void PeripheralClockInitialize(void)
{
    struct {
        uint8_t id;
        uint8_t clken;
        uint8_t gclken;
        uint8_t css;
        uint8_t divs;
    } periphList[] =
    {
        { ID_TC2_CHANNEL0, 1U, 0U, 0U, 0U},

        { ID_PIOD, 1U, 0U, 0U, 0U},

        { ID_IPC1, 1U, 0U, 0U, 0U},

        { ID_PERIPH_MAX + 1, 0, 0, 0, 0}//end of list marker
    };
    uint32_t count = sizeof(periphList)/sizeof(periphList[0]);
    uint32_t i = 0U;

    while((i < count) && (periphList[i].id != ((uint32_t)ID_PERIPH_MAX + 1U)))
    {
        PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk |\
                            PMC_PCR_GCLKEN(periphList[i].gclken) |\
                            PMC_PCR_EN(periphList[i].clken) |\
                            PMC_PCR_GCLKDIV(periphList[i].divs) |\
                            PMC_PCR_GCLKCSS(periphList[i].css) |\
                            PMC_PCR_PID(periphList[i].id);
        while(PeripheralClockStatus(periphList[i].id) == false)
        {
            /* Wait for clock to be initialized */
        }
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