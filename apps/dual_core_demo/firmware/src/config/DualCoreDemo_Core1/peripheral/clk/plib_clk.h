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

#ifndef PLIB_CLK_H
#define PLIB_CLK_H

#include <stddef.h>
#include <stdbool.h>
#include "device.h"

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

// *****************************************************************************
// *****************************************************************************
// Section: CLK Module System Interface Routines
// *****************************************************************************
// *****************************************************************************

typedef uint32_t ID_PERIPH;


typedef enum
{
    GCLK_SRC_MD_SLOW_CLK = PMC_PCR_GCLKCSS_MD_SLOW_CLK_Val,
	GCLK_SRC_TD_SLOW_CLOCK = PMC_PCR_GCLKCSS_TD_SLOW_CLOCK_Val,
	GCLK_SRC_MAINCK = PMC_PCR_GCLKCSS_MAINCK_Val,
	GCLK_SRC_MCK0 = PMC_PCR_GCLKCSS_MCK0_Val,
	GCLK_SRC_PLLACK1 = PMC_PCR_GCLKCSS_PLLACK1_Val,
	GCLK_SRC_PLLBCK = PMC_PCR_GCLKCSS_PLLBCK_Val,
	GCLK_SRC_PLLCCK = PMC_PCR_GCLKCSS_PLLCCK_Val,
	GCLK_SRC_PLLCSRC = PMC_PCR_GCLKCSS_PLLCSRC_Val,
	
}GCLK_SRC;




// *****************************************************************************
/* Function:
    void CLK_Initialize ( void )

  Summary:
    Initializes hardware of the System Clock and Peripheral Clock.

  Description:
    This function initializes the hardware of System Clock and Peripheral Clocks.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    Example 1: Do not alter the configuration bit settings
    CLK_Initialize ( );

    </code>

  Remarks:
    None.
*/

void CLK_Initialize ( void );

#ifdef __cplusplus
}
#endif

#endif //PLIB_CLK_H