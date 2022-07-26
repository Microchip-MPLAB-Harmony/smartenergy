/*******************************************************************************
  Supply Controller (SUPC) Peripheral Library (PLIB)

  Company:
    Microchip Technology Inc.

  File Name:
    plib_supc.c

  Summary:
    SUPC Source File

  Description:
    None

*******************************************************************************/

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "plib_supc.h"
#include "peripheral/clk/plib_clk.h"
#include "peripheral/sefc/plib_sefc0.h"
#include "peripheral/sefc/plib_sefc1.h"
#include "interrupts.h"


static void WaitEntryClockSetup(bool xtal_disable)
{
    uint8_t count = 0U;

    /* Enable the RC Oscillator */
    PMC_REGS->CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCEN_Msk;

    /* Wait until the RC oscillator clock is ready. */
    while((PMC_REGS->PMC_SR & PMC_SR_MOSCRCS_Msk) != PMC_SR_MOSCRCS_Msk);

    /* Switch Main Clock (MAINCK) to the RC Oscillator clock */
    PMC_REGS->CKGR_MOR = (PMC_REGS->CKGR_MOR & ~CKGR_MOR_MOSCSEL_Msk) | CKGR_MOR_KEY_PASSWD;

    /* Wait for Main Clock Selection Status */
    while((PMC_REGS->PMC_SR & PMC_SR_MOSCSELS_Msk) != PMC_SR_MOSCSELS_Msk);

    /* Disable PLL Clock */
    for (count = 0U; count < 3U; count++)
    {
        PMC_REGS->PMC_PLL_UPDT = (PMC_REGS->PMC_PLL_UPDT & ~PMC_PLL_UPDT_Msk) | PMC_PLL_UPDT_ID(count);
        PMC_REGS->PMC_PLL_UPDT |= PMC_PLL_UPDT_UPDATE_Msk;
        PMC_REGS->PMC_PLL_CTRL0 &= ~PMC_PLL_CTRL0_ENPLL_Msk;
    }

    /* Disable Crystal  */
    if(xtal_disable)
    {
        PMC_REGS->CKGR_MOR = (PMC_REGS->CKGR_MOR & ~CKGR_MOR_MOSCXTEN_Msk) | CKGR_MOR_KEY_PASSWD;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: SUPC Implementation
// *****************************************************************************
// *****************************************************************************

void SUPC_Initialize(void)
{
    SUPC_REGS->SUPC_SMMR = SUPC_SMMR_VDD3V3SMSMPL(0x1) | SUPC_SMMR_VDD3V3SMTH(10) | SUPC_SMMR_VDD3V3SMPWRM(0) ;

    SUPC_REGS->SUPC_MR = (SUPC_REGS->SUPC_MR & ~SUPC_MR_Msk) | (SUPC_REGS->SUPC_MR & SUPC_MR_OSCBYPASS_Msk) | SUPC_MR_KEY_PASSWD | SUPC_MR_IO_BACKUP_ISO_Msk | SUPC_MR_CORSMRSTEN_Msk | SUPC_MR_VREGDIS_Msk | SUPC_MR_CORSMM_Msk;

    SUPC_REGS->SUPC_BMR = SUPC_BMR_KEY_PASSWD | SUPC_BMR_FWUPEN_Msk | SUPC_BMR_MRTCOUT_Msk ;

    SUPC_REGS->SUPC_WUMR = SUPC_WUMR_LPDBC0(0x1) | SUPC_WUMR_LPDBC1(0x0) | SUPC_WUMR_LPDBC2(0x0) | SUPC_WUMR_LPDBC3(0x0) | SUPC_WUMR_LPDBC4(0x0) | SUPC_WUMR_WKUPDBC(0x2) | SUPC_WUMR_FWUPDBC(0x2) | SUPC_WUMR_LPDBCEN0_Msk ;

    SUPC_REGS->SUPC_IER = SUPC_IER_LPDBC0_Msk | SUPC_IER_VDD3V3SMEV_Msk;
    
    SUPC_REGS->SUPC_WUIR = SUPC_WUIR_WKUPEN0_Msk;

}

void SUPC_SleepModeEnter(void)
{
    SCB->SCR &= (uint32_t)~SCB_SCR_SLEEPDEEP_Msk;

    /* Enable Interrupt */
    __DMB();
    __enable_irq();

    /* Enter Sleep  */
    __DSB();
    __WFI();
}

void SUPC_WaitModeEnter(WAITMODE_FLASH_STATE flash_lpm, WAITMODE_WKUP_SOURCE source)
{
    uint32_t i;

    /* Disable CPU Interrupt */
    __disable_irq();
    __DMB();

    /* Setup Clock for wait mode entry */
    WaitEntryClockSetup((flash_lpm == WAITMODE_FLASH_DEEPSLEEP));

    /* Enable CPU Interrupt */
    __DMB();
    __enable_irq();

    /* FLASH  Low power mode and Wakeup source */
    PMC_REGS->PMC_FSMR = ((uint32_t) flash_lpm | (uint32_t) source);

    /* Set Flash Wait State at 0 */
    SEFC0_REGS->SEFC_EEFC_FMR = SEFC_EEFC_FMR_FWS(0) | SEFC_EEFC_FMR_CLOE_Msk;
    SEFC1_REGS->SEFC_EEFC_FMR = SEFC_EEFC_FMR_FWS(0) | SEFC_EEFC_FMR_CLOE_Msk;

    /* Set the WAITMODE bit */
    PMC_REGS->CKGR_MOR |= (CKGR_MOR_KEY_PASSWD | CKGR_MOR_WAITMODE_Msk);

    /* Waiting for Master Clock Ready MCKRDY = 1 */
    while((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);

    /* Waiting for MOSCRCEN bit is cleared is strongly recommended
     * to ensure that the core will not execute undesired instructions
     */
    for (i = 0; i < 500; i++)
    {
       __NOP();
    }

    while((PMC_REGS->CKGR_MOR & CKGR_MOR_MOSCRCEN_Msk) != CKGR_MOR_MOSCRCEN_Msk);

    /* Disable CPU Interrupt */
    __disable_irq();
    __DMB();

    /* Restore Clock Setting */
    SEFC0_Initialize();
    SEFC1_Initialize();
    CLK_Initialize();

    /* Enable CPU Interrupt */
    __DMB();
    __enable_irq();
}

void SUPC_BackupModeEnter(void)
{
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* Switch off voltage regulator */
    SUPC_REGS->SUPC_CR |= SUPC_CR_KEY_PASSWD | SUPC_CR_VROFF_Msk;

    /* Enable CPU Interrupt */
    __DMB();
    __enable_irq();

    /* Enter Backup */
    __DSB();
    __WFI();
}

SUPC_OBJECT supcObj;

void SUPC_CallbackRegister(SUPC_CALLBACK callback, uintptr_t context)
{
    supcObj.callback = callback;
    supcObj.context = context;
}

void SUPC_InterruptHandler(void)
{
    uint32_t supc_status = SUPC_REGS->SUPC_ISR;

    /* Callback user function */
    if(supcObj.callback != NULL)
    {
        supcObj.callback(supc_status, supcObj.context);
    }
}

uint32_t SUPC_GPBRRead(GPBR_REGS_INDEX reg)
{
    return GPBR_REGS->SYS_GPBR[reg];
}

void SUPC_GPBRWrite(GPBR_REGS_INDEX reg, uint32_t data)
{
    GPBR_REGS->SYS_GPBR[reg] = data;
}
