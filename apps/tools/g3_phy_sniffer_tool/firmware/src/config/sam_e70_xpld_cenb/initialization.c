/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "definitions.h"
#include "device.h"



// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
#pragma config TCM_CONFIGURATION = 0
#pragma config SECURITY_BIT = CLEAR
#pragma config BOOT_MODE = SET




// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="DRV_PL360 Initialization Data">

/* HAL Interface Initialization for PL360 Driver */
DRV_PL360_PLIB_INTERFACE drvPL360Plib = {

    /* PL360 SPI PLIB */
    .spiPlibTransferSetup = (DRV_PL360_SPI_PLIB_TRANSFER_SETUP)SPI0_TransferSetup,

    /* DMA Channel for Transmit */
    .dmaChannelTx = SYS_DMA_CHANNEL_1,

    /* DMA Channel for Receive */
    .dmaChannelRx  = SYS_DMA_CHANNEL_0,

    /* SPI Transmit Register */
    .spiAddressTx =  (void *)&(SPI0_REGS->SPI_TDR),

    /* SPI Receive Register */
    .spiAddressRx  = (void *)&(SPI0_REGS->SPI_RDR),
    
    /* SPI clock frequency */
    .spiClockFrequency = DRV_PL360_SPI_CLK,
    
    /* PL360 LDO Enable Pin */
    .ldoPin = DRV_PL360_LDO_EN_PIN, 
    
    /* PL360 Reset Pin */
    .resetPin = DRV_PL360_RESET_PIN,
       
    /* PL360 External Interrupt Pin */
    .extIntPin = DRV_PL360_EXT_INT_PIN,
};

/* HAL Interface Initialization for PL360 Driver */
DRV_PL360_HAL_INTERFACE drvPL360HalAPI = {

    /* PL360 PLIB */
    .pl360Plib = &drvPL360Plib,

    /* PL360 HAL init */
    .init = (DRV_PL360_HAL_INIT)DRV_PL360_HAL_Init,

    /* PL360 HAL setup */
    .setup = (DRV_PL360_HAL_SETUP)DRV_PL360_HAL_Setup,

    /* PL360 PL360 reset */
    .reset = (DRV_PL360_HAL_RESET)DRV_PL360_HAL_Reset,

    /* PL360 HAL Get Carrier Detect or PLC Line Status */
    .getCd = (DRV_PL360_HAL_GET_CD)DRV_PL360_HAL_GetCarrierDetect,

    /* PL360 HAL Enable/Disable external interrupt */
    .enableExtInt = (DRV_PL360_HAL_ENABLE_EXT_INT)DRV_PL360_HAL_EnableInterrupts,

    /* PL360 HAL delay function */
    .delay = (DRV_PL360_HAL_DELAY)DRV_PL360_HAL_Delay,

    /* PL360 HAL Transfer Bootloader Command */
    .sendBootCmd = (DRV_PL360_HAL_SEND_BOOT_CMD)DRV_PL360_HAL_SendBootCmd,

    /* PL360 HAL Transfer Write/Read Command */
    .sendWrRdCmd = (DRV_PL360_HAL_SEND_WRRD_CMD)DRV_PL360_HAL_SendWrRdCmd,
};

/* PL360 Binary file addressing */
extern uint8_t pl360_bin_start;
extern uint8_t pl360_bin_end;

/* PL360 Driver Initialization Data */
DRV_PL360_INIT drvPL360InitData = {

    /* SPI PLIB API  interface*/
    .pl360Hal = &drvPL360HalAPI,

    /* PL360 Number of clients */
    .numClients = DRV_PL360_CLIENTS_NUMBER_IDX,  

    /* PL360 PLC profile */
    .plcProfile = DRV_PL360_PLC_PROFILE,
 
    /* PLC Binary start address */
    .binStartAddress = (uint32_t)&pl360_bin_start,
    
    /* PLC Binary end address */
    .binEndAddress = (uint32_t)&pl360_bin_end,

    /* Secure Mode */
    .secure = DRV_PL360_SECURE,
    
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SRV_USI Instance 0 Initialization Data">

uint8_t gSrvUSIUSART1ReadBuffer[SRV_USI0_RD_BUF_SIZE] = {0};
uint8_t gSrvUSIUSART1WriteBuffer[SRV_USI0_WR_BUF_SIZE] = {0};

const SRV_USI_USART_INTERFACE srvUsiUSART1PlibAPI = {
    .readCallbackRegister = (USI_USART_PLIB_READ_CALLBACK_REG)USART1_ReadCallbackRegister,
    .read = (USI_USART_PLIB_READ)USART1_Read,
    .writeCallbackRegister = (USI_USART_PLIB_WRITE_CALLBACK_REG)USART1_WriteCallbackRegister,
    .dmaChannelTx = SYS_DMA_CHANNEL_18,
    .usartAddressTx = (void *)&(USART1_REGS->US_THR)
};

const SRV_USI_INIT srvUSI0InitData =
{
    .usiInterfaceApi = SRV_USI_USART_API,

    .usiApi = (SRV_USI_USART_INTERFACE *)&srvUsiUSART1PlibAPI,

    .readBuffer = (void*)gSrvUSIUSART1ReadBuffer,

    .readSizeMax = SRV_USI0_RD_BUF_SIZE,

    .writeBuffer = (void*)gSrvUSIUSART1WriteBuffer,

    .writeSizeMax = SRV_USI0_WR_BUF_SIZE,

};

// </editor-fold>


// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************
/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="SYS_TIME Initialization Data">

const SYS_TIME_PLIB_INTERFACE sysTimePlibAPI = {
    .timerCallbackSet = (SYS_TIME_PLIB_CALLBACK_REGISTER)TC0_CH0_TimerCallbackRegister,
    .timerCounterGet = (SYS_TIME_PLIB_COUNTER_GET)TC0_CH0_TimerCounterGet,
    .timerPeriodSet = (SYS_TIME_PLIB_PERIOD_SET)TC0_CH0_TimerPeriodSet,
    .timerFrequencyGet = (SYS_TIME_PLIB_FREQUENCY_GET)TC0_CH0_TimerFrequencyGet,
    .timerCompareSet = (SYS_TIME_PLIB_COMPARE_SET)TC0_CH0_TimerCompareSet,
    .timerStart = (SYS_TIME_PLIB_START)TC0_CH0_TimerStart,
    .timerStop = (SYS_TIME_PLIB_STOP)TC0_CH0_TimerStop 
};

const SYS_TIME_INIT sysTimeInitData =
{
    .timePlib = &sysTimePlibAPI,
    .hwTimerIntNum = TC0_CH0_IRQn,
};

// </editor-fold>



/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{

    EFC_Initialize();
  
    CLK_Initialize();
	PIO_Initialize();

    XDMAC_Initialize();

	RSWDT_REGS->RSWDT_MR = RSWDT_MR_WDDIS_Msk;	// Disable RSWDT 

	WDT_REGS->WDT_MR = WDT_MR_WDDIS_Msk; 		// Disable WDT 

 
    TC0_CH0_TimerInitialize(); 
     
    
	BSP_Initialize();
	SPI0_Initialize();

	USART1_Initialize();


 
    /* Initialize PL360 Driver Instance */
    sysObj.drvPL360 = DRV_PL360_Initialize(DRV_PL360_INDEX, (SYS_MODULE_INIT *)&drvPL360InitData);
    /* Register Callback function to handle PL360 interruption */
    PIO_PinInterruptCallbackRegister(DRV_PL360_EXT_INT_PIN, DRV_PL360_ExternalInterruptHandler, sysObj.drvPL360);
   
    /* Initialize USI Service Instance 0 */
    sysObj.srvUSI0 = SRV_USI_Initialize(SRV_USI_INDEX_0, (SYS_MODULE_INIT *)&srvUSI0InitData);


    sysObj.sysTime = SYS_TIME_Initialize(SYS_TIME_INDEX_0, (SYS_MODULE_INIT *)&sysTimeInitData);


    APP_Initialize();


    NVIC_Initialize();

}


/*******************************************************************************
 End of File
*/
