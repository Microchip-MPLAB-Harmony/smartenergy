/*******************************************************************************
  FLEXCOM4 USART PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_flexcom4_usart.c

  Summary:
    FLEXCOM4 USART PLIB Implementation File

  Description
    This file defines the interface to the FLEXCOM4 USART peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.
*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
/* This section lists the other files that are included in this file.
*/
#include "plib_flexcom4_usart.h"

// *****************************************************************************
// *****************************************************************************
// Section: FLEXCOM4 USART Implementation
// *****************************************************************************
// *****************************************************************************

FLEXCOM_USART_OBJECT flexcom4UsartObj;

void static FLEXCOM4_USART_ISR_RX_Handler( void )
{
    if(flexcom4UsartObj.rxBusyStatus == true)
    {
        while((US_CSR_RXRDY_Msk == (USART4_REGS->US_CSR & US_CSR_RXRDY_Msk)) && (flexcom4UsartObj.rxSize > flexcom4UsartObj.rxProcessedSize) )
        {
            flexcom4UsartObj.rxBuffer[flexcom4UsartObj.rxProcessedSize++] = (USART4_REGS->US_RHR & US_RHR_RXCHR_Msk);
        }

        /* Check if the buffer is done */
        if(flexcom4UsartObj.rxProcessedSize >= flexcom4UsartObj.rxSize)
        {
            flexcom4UsartObj.rxBusyStatus = false;

            /* Disable Read, Overrun, Parity and Framing error interrupts */
            USART4_REGS->US_IDR = (US_IDR_RXRDY_Msk | US_IDR_FRAME_Msk | US_IDR_PARE_Msk | US_IDR_OVRE_Msk);

            if(flexcom4UsartObj.rxCallback != NULL)
            {
                flexcom4UsartObj.rxCallback(flexcom4UsartObj.rxContext);
            }
        }
    }
    else
    {
        /* Nothing to process */
        ;
    }
}


void FLEXCOM4_InterruptHandler( void )
{
    /* Channel status */
    uint32_t channelStatus = USART4_REGS->US_CSR;

    USART4_REGS->US_PTCR = US_PTCR_ERRCLR_Msk;

    /* Error status */
    uint32_t errorStatus = (channelStatus & (US_CSR_OVRE_Msk | US_CSR_FRAME_Msk | US_CSR_PARE_Msk));

    if(errorStatus != 0)
    {
        flexcom4UsartObj.rxBusyStatus = false;

        /* Disable Read, Overrun, Parity and Framing error interrupts */
        USART4_REGS->US_IDR = (US_IDR_RXRDY_Msk | US_IDR_FRAME_Msk | US_IDR_PARE_Msk | US_IDR_OVRE_Msk);

        /* Client must call FLEXCOM4_USART_ErrorGet() function to clear the errors */

        /* USART errors are normally associated with the receiver, hence calling
         * receiver context */
        if( flexcom4UsartObj.rxCallback != NULL )
        {
            flexcom4UsartObj.rxCallback(flexcom4UsartObj.rxContext);
        }
    }

    /* Receiver status */
    if(US_CSR_RXRDY_Msk == (channelStatus & US_CSR_RXRDY_Msk))
    {
        FLEXCOM4_USART_ISR_RX_Handler();
    }

    if ((USART4_REGS->US_PTSR & US_PTSR_TXTEN_Msk) && (channelStatus & US_CSR_ENDTX_Msk))
    {
        if(flexcom4UsartObj.txBusyStatus == true)
        {
            flexcom4UsartObj.txBusyStatus = false;
            USART4_REGS->US_PTCR = US_PTCR_TXTDIS_Msk;
            USART4_REGS->US_IDR = US_IDR_ENDTX_Msk;

            if( flexcom4UsartObj.txCallback != NULL )
            {
                flexcom4UsartObj.txCallback(flexcom4UsartObj.txContext);
            }
        }
    }
}

void static FLEXCOM4_USART_ErrorClear( void )
{
    uint8_t dummyData = 0u;

    USART4_REGS->US_CR = US_CR_RSTSTA_Msk;

    /* Flush existing error bytes from the RX FIFO */
    while( US_CSR_RXRDY_Msk == (USART4_REGS->US_CSR& US_CSR_RXRDY_Msk) )
    {
        dummyData = (USART4_REGS->US_RHR& US_RHR_RXCHR_Msk);
    }

    /* Ignore the warning */
    (void)dummyData;
}

void FLEXCOM4_USART_Initialize( void )
{
    /* Set FLEXCOM USART operating mode */
    FLEXCOM4_REGS->FLEXCOM_MR = FLEXCOM_MR_OPMODE_USART;

    /* Reset FLEXCOM4 USART */
    USART4_REGS->US_CR = (US_CR_RSTRX_Msk | US_CR_RSTTX_Msk | US_CR_RSTSTA_Msk);

    /* Enable FLEXCOM4 USART */
    USART4_REGS->US_CR = (US_CR_TXEN_Msk | US_CR_RXEN_Msk);

    /* Configure FLEXCOM4 USART mode */
    USART4_REGS->US_MR = ((US_MR_USCLKS_MCK) | US_MR_CHRL_8_BIT | US_MR_PAR_NO | US_MR_NBSTOP_1_BIT | (0 << US_MR_OVER_Pos));

    /* Configure FLEXCOM4 USART Baud Rate */
    USART4_REGS->US_BRGR = US_BRGR_CD(32);

    /* Initialize instance object */
    flexcom4UsartObj.rxBuffer = NULL;
    flexcom4UsartObj.rxSize = 0;
    flexcom4UsartObj.rxProcessedSize = 0;
    flexcom4UsartObj.rxBusyStatus = false;
    flexcom4UsartObj.rxCallback = NULL;
    flexcom4UsartObj.txBuffer = NULL;
    flexcom4UsartObj.txSize = 0;
    flexcom4UsartObj.txProcessedSize = 0;
    flexcom4UsartObj.txBusyStatus = false;
    flexcom4UsartObj.txCallback = NULL;
}

FLEXCOM_USART_ERROR FLEXCOM4_USART_ErrorGet( void )
{
    FLEXCOM_USART_ERROR errors = FLEXCOM_USART_ERROR_NONE;
    uint32_t status = USART4_REGS->US_CSR;

    /* Collect all errors */
    if(status & US_CSR_OVRE_Msk)
    {
        errors = FLEXCOM_USART_ERROR_OVERRUN;
    }

    if(status & US_CSR_PARE_Msk)
    {
        errors |= FLEXCOM_USART_ERROR_PARITY;
    }

    if(status & US_CSR_FRAME_Msk)
    {
        errors |= FLEXCOM_USART_ERROR_FRAMING;
    }

    if(errors != FLEXCOM_USART_ERROR_NONE)
    {
        FLEXCOM4_USART_ErrorClear();
    }

    /* All errors are cleared, but send the previous error state */
    return errors;
}

bool FLEXCOM4_USART_SerialSetup( FLEXCOM_USART_SERIAL_SETUP *setup, uint32_t srcClkFreq )
{
    uint32_t baud = 0;
    uint32_t brgVal = 0;
    uint32_t overSampVal = 0;
    uint32_t usartMode;
    bool status = false;

    if((flexcom4UsartObj.rxBusyStatus == true) || (flexcom4UsartObj.txBusyStatus == true))
    {
        /* Transaction is in progress, so return without updating settings */
        return false;
    }

    if (setup != NULL)
    {
        baud = setup->baudRate;

        if(srcClkFreq == 0)
        {
            srcClkFreq = FLEXCOM4_USART_FrequencyGet();
        }

        /* Calculate BRG value */
        if (srcClkFreq >= (16 * baud))
        {
            brgVal = (srcClkFreq / (16 * baud));
        }
        else
        {
            brgVal = (srcClkFreq / (8 * baud));
            overSampVal = (1 << US_MR_OVER_Pos) & US_MR_OVER_Msk;
        }

        /* Configure FLEXCOM4 USART mode */
        usartMode = USART4_REGS->US_MR;
        usartMode &= ~(US_MR_CHRL_Msk | US_MR_MODE9_Msk | US_MR_PAR_Msk | US_MR_NBSTOP_Msk | US_MR_OVER_Msk);
        USART4_REGS->US_MR = usartMode | ((uint32_t)setup->dataWidth | (uint32_t)setup->parity | (uint32_t)setup->stopBits | overSampVal);

        /* Configure FLEXCOM4 USART Baud Rate */
        USART4_REGS->US_BRGR = US_BRGR_CD(brgVal);
        status = true;
    }

    return status;
}

bool FLEXCOM4_USART_Read( void *buffer, const size_t size )
{
    bool status = false;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(lBuffer != NULL)
    {
        /* Clear errors before submitting the request.
         * ErrorGet clears errors internally. */
        FLEXCOM4_USART_ErrorGet();

        /* Check if receive request is in progress */
        if(flexcom4UsartObj.rxBusyStatus == false)
        {
            status = true;

            flexcom4UsartObj.rxBuffer = lBuffer;
            flexcom4UsartObj.rxSize = size;
            flexcom4UsartObj.rxProcessedSize = 0;
            flexcom4UsartObj.rxBusyStatus = true;


            /* Enable Read, Overrun, Parity and Framing error interrupts */
            USART4_REGS->US_IER = (US_IER_RXRDY_Msk | US_IER_FRAME_Msk | US_IER_PARE_Msk | US_IER_OVRE_Msk);
        }
    }

    return status;
}

bool FLEXCOM4_USART_Write( void *buffer, const size_t size )
{
    bool status = false;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(lBuffer != NULL)
    {
        /* Check if transmit request is in progress */
        if(flexcom4UsartObj.txBusyStatus == false)
        {
            status = true;

            flexcom4UsartObj.txBuffer = lBuffer;
            flexcom4UsartObj.txSize = size;
            flexcom4UsartObj.txProcessedSize = 0;
            flexcom4UsartObj.txBusyStatus = true;

            USART4_REGS->US_TPR = (uint32_t) buffer;
            USART4_REGS->US_TCR = (uint32_t) size;
            USART4_REGS->US_PTCR = US_PTCR_TXTEN_Msk;
            USART4_REGS->US_IER = US_IER_ENDTX_Msk;
        }
    }

    return status;
}

void FLEXCOM4_USART_WriteCallbackRegister( FLEXCOM_USART_CALLBACK callback, uintptr_t context )
{
    flexcom4UsartObj.txCallback = callback;
    flexcom4UsartObj.txContext = context;
}

void FLEXCOM4_USART_ReadCallbackRegister( FLEXCOM_USART_CALLBACK callback, uintptr_t context )
{
    flexcom4UsartObj.rxCallback = callback;
    flexcom4UsartObj.rxContext = context;
}

bool FLEXCOM4_USART_WriteIsBusy( void )
{
    return flexcom4UsartObj.txBusyStatus;
}

bool FLEXCOM4_USART_ReadIsBusy( void )
{
    return flexcom4UsartObj.rxBusyStatus;
}

size_t FLEXCOM4_USART_WriteCountGet( void )
{
    return (flexcom4UsartObj.txSize - USART4_REGS->US_TCR);

}

size_t FLEXCOM4_USART_ReadCountGet( void )
{
    return flexcom4UsartObj.rxProcessedSize;
}

