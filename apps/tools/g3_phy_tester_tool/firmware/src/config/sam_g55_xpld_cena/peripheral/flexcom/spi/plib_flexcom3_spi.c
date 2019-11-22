/*******************************************************************************
  FLEXCOM3 SPI PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_flexcom3_spi.c

  Summary:
    FLEXCOM3 SPI PLIB Implementation File.

  Description:
    This file defines the interface to the FLEXCOM SPI peripheral library.
    This library provides access to and control of the associated
    peripheral instance.

  Remarks:
    None.

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#include "plib_flexcom3_spi.h"

// *****************************************************************************
// *****************************************************************************
// Section: FLEXCOM3 SPI Implementation
// *****************************************************************************
// *****************************************************************************


/* Global object to save FLEXCOM SPI Exchange related data */
FLEXCOM_SPI_OBJECT flexcom3SpiObj;

static uint8_t dummyDataBuffer[512];

static void setupDMA( void* pTransmitData, void* pReceiveData, size_t size )
{
    /* Always set up the rx channel first */
    SPI3_REGS->SPI_RPR = (uint32_t) pReceiveData;
    SPI3_REGS->SPI_RCR = (uint32_t) size;
    SPI3_REGS->SPI_TPR = (uint32_t) pTransmitData;
    SPI3_REGS->SPI_TCR = (uint32_t) size;
    SPI3_REGS->SPI_PTCR = SPI_PTCR_RXTEN_Msk | SPI_PTCR_TXTEN_Msk;
    SPI3_REGS->SPI_IER = SPI_IER_ENDRX_Msk;
}

void FLEXCOM3_SPI_Initialize( void )
{
    /* Set FLEXCOM SPI operating mode */
    FLEXCOM3_REGS->FLEXCOM_MR = FLEXCOM_MR_OPMODE_SPI;

    /* Disable and Reset the FLEXCOM SPI */
    SPI3_REGS->SPI_CR = SPI_CR_SPIDIS_Msk | SPI_CR_SWRST_Msk;

    /* Enable Master mode, select clock source, select particular NPCS line for chip select and disable mode fault detection */
    SPI3_REGS->SPI_MR = SPI_MR_MSTR_Msk | SPI_MR_BRSRCCLK_PERIPH_CLK | SPI_MR_PCS(0) | SPI_MR_MODFDIS_Msk;

    /* Set up clock Polarity, data phase, Communication Width, Baud Rate and Chip select active after transfer */
    SPI3_REGS->SPI_CSR[0] = SPI_CSR_CPOL(0) | SPI_CSR_NCPHA(1) | SPI_CSR_BITS_8_BIT | SPI_CSR_SCBR(119) | SPI_CSR_CSAAT_Msk;

    /* Initialize global variables */
    flexcom3SpiObj.transferIsBusy = false;
    flexcom3SpiObj.callback = NULL;

    /* Enable FLEXCOM3 SPI */
    SPI3_REGS->SPI_CR = SPI_CR_SPIEN_Msk;
}

bool FLEXCOM3_SPI_WriteRead( void* pTransmitData, size_t txSize, void* pReceiveData, size_t rxSize )
{
    bool isRequestAccepted = false;
    uint32_t size = 0;

    /* Verify the request */
    if((((txSize > 0) && (pTransmitData != NULL)) || ((rxSize > 0) && (pReceiveData != NULL))) && (flexcom3SpiObj.transferIsBusy == false))
    {
        isRequestAccepted = true;

        flexcom3SpiObj.transferIsBusy = true;

        flexcom3SpiObj.txBuffer = pTransmitData;
        flexcom3SpiObj.rxBuffer = pReceiveData;
        flexcom3SpiObj.txCount = txSize;
        flexcom3SpiObj.rxCount = rxSize;

        if ((txSize > 0) && (rxSize > 0))
        {
            /* Find the lower value among txSize and rxSize */
            (txSize >= rxSize) ? (size = rxSize) : (size = txSize);

            /* Calculate the remaining tx/rx bytes and total bytes transferred */
            flexcom3SpiObj.rxCount -= size;
            flexcom3SpiObj.txCount -= size;
            flexcom3SpiObj.nBytesTransferred = size;

            setupDMA(pTransmitData, pReceiveData, size);
        }
        else
        {
            if (rxSize > 0)
            {
                /* txSize is 0. Need to use the dummy data buffer for transmission.
                 * Find out the max data that can be received, given the limited size of the dummy data buffer.
                 */
                (rxSize > sizeof(dummyDataBuffer)) ?
                    (size = sizeof(dummyDataBuffer)): (size = rxSize);

                /* Calculate the remaining rx bytes and total bytes transferred */
                flexcom3SpiObj.rxCount -= size;
                flexcom3SpiObj.nBytesTransferred = size;

                setupDMA(dummyDataBuffer, pReceiveData, size);
            }
            else
            {
                /* rxSize is 0. Need to use the dummy data buffer for reception.
                 * Find out the max data that can be transmitted, given the limited size of the dummy data buffer.
                 */
                (txSize > sizeof(dummyDataBuffer)) ?
                    (size = sizeof(dummyDataBuffer)): (size = txSize);

                /* Calculate the remaining tx bytes and total bytes transferred */
                flexcom3SpiObj.txCount -= size;
                flexcom3SpiObj.nBytesTransferred = size;

                setupDMA(pTransmitData, dummyDataBuffer, size);
            }
        }
    }

    return isRequestAccepted;
}

bool FLEXCOM3_SPI_TransferSetup( FLEXCOM_SPI_TRANSFER_SETUP * setup, uint32_t spiSourceClock )
{
    uint32_t scbr;

    if ((setup == NULL) || (setup->clockFrequency == 0))
    {
        return false;
    }
    if(spiSourceClock == 0)
    {
        // Fetch Master Clock Frequency directly
        spiSourceClock = 119996416;
    }

    scbr = spiSourceClock/setup->clockFrequency;

    if(scbr == 0)
    {
        scbr = 1;
    }
    else if(scbr > 255)
    {
        scbr = 255;
    }

    SPI3_REGS->SPI_CSR[0] = (uint32_t)setup->clockPolarity | (uint32_t)setup->clockPhase | (uint32_t)setup->dataBits | SPI_CSR_SCBR(scbr);

    return true;
}

bool FLEXCOM3_SPI_Write( void* pTransmitData, size_t txSize )
{
    return (FLEXCOM3_SPI_WriteRead(pTransmitData, txSize, NULL, 0));
}

bool FLEXCOM3_SPI_Read( void* pReceiveData, size_t rxSize )
{
    return (FLEXCOM3_SPI_WriteRead(NULL, 0, pReceiveData, rxSize));
}

void FLEXCOM3_SPI_CallbackRegister( FLEXCOM_SPI_CALLBACK callback, uintptr_t context )
{
    flexcom3SpiObj.callback = callback;
    flexcom3SpiObj.context = context;
}

bool FLEXCOM3_SPI_IsBusy( void )
{
    return ((flexcom3SpiObj.transferIsBusy) || ((SPI3_REGS->SPI_SR & SPI_SR_TXEMPTY_Msk) == 0));
}

void FLEXCOM3_InterruptHandler( void )
{
    uint32_t size;
    uint32_t index;

    /* save the status in global object before it gets cleared */
    flexcom3SpiObj.status = SPI3_REGS->SPI_SR;

    SPI3_REGS->SPI_PTCR = SPI_PTCR_ERRCLR_Msk;

    if(flexcom3SpiObj.rxCount > 0)
    {
        /* txPending is 0. Need to use the dummy data buffer for transmission.
         * Find out the max data that can be received, given the limited size of the dummy data buffer.
         */
        (flexcom3SpiObj.rxCount > sizeof(dummyDataBuffer)) ?
            (size = sizeof(dummyDataBuffer)): (size = flexcom3SpiObj.rxCount);

        index = flexcom3SpiObj.nBytesTransferred;

        /* Calculate the remaining rx bytes and total bytes transferred */
        flexcom3SpiObj.rxCount -= size;
        flexcom3SpiObj.nBytesTransferred += size;

        setupDMA(dummyDataBuffer,(void *)&((uint8_t*)flexcom3SpiObj.rxBuffer)[index],size);
    }
    else if(flexcom3SpiObj.txCount > 0)
    {
        /* rxSize is 0. Need to use the dummy data buffer for reception.
         * Find out the max data that can be transmitted, given the limited size of the dummy data buffer.
         */
        (flexcom3SpiObj.txCount > sizeof(dummyDataBuffer)) ?
            (size = sizeof(dummyDataBuffer)): (size = flexcom3SpiObj.txCount);

        index = flexcom3SpiObj.nBytesTransferred;

        /* Calculate the remaining rx bytes and total bytes transferred */
        flexcom3SpiObj.txCount -= size;
        flexcom3SpiObj.nBytesTransferred += size;

        setupDMA((void *)&((uint8_t*)flexcom3SpiObj.txBuffer)[index], dummyDataBuffer, size);
    }
    else
    {
        flexcom3SpiObj.transferIsBusy = false;

        /* Set Last transfer to deassert NPCS after the last byte written in TDR has been transferred. */
        SPI3_REGS->SPI_CR = SPI_CR_LASTXFER_Msk;
        SPI3_REGS->SPI_PTCR = SPI_PTCR_RXTDIS_Msk | SPI_PTCR_TXTDIS_Msk;
        SPI3_REGS->SPI_IDR = SPI_IDR_ENDRX_Msk;

        if( flexcom3SpiObj.callback != NULL )
        {
            flexcom3SpiObj.callback(flexcom3SpiObj.context);
        }
    }
}
