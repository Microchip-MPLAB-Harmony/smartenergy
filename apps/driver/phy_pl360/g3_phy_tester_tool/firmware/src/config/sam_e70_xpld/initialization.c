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
#pragma config TCM_CONFIGURATION = 0
#pragma config SECURITY_BIT = CLEAR
#pragma config BOOT_MODE = SET




// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="DRV_USART Instance 0 Initialization Data">

static DRV_USART_CLIENT_OBJ drvUSART0ClientObjPool[DRV_USART_CLIENTS_NUMBER_IDX0];

/* USART transmit/receive transfer objects pool */
static DRV_USART_BUFFER_OBJ drvUSART0BufferObjPool[DRV_USART_QUEUE_SIZE_IDX0];

const DRV_USART_PLIB_INTERFACE drvUsart0PlibAPI = {
    .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG)USART1_ReadCallbackRegister,
    .read = (DRV_USART_PLIB_READ)USART1_Read,
    .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY)USART1_ReadIsBusy,
    .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET)USART1_ReadCountGet,
    .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG)USART1_WriteCallbackRegister,
    .write = (DRV_USART_PLIB_WRITE)USART1_Write,
    .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY)USART1_WriteIsBusy,
    .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET)USART1_WriteCountGet,
    .errorGet = (DRV_USART_PLIB_ERROR_GET)USART1_ErrorGet,
    .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP)USART1_SerialSetup
};

const uint32_t drvUsart0remapDataWidth[] = { 0x0, 0x40, 0x80, 0xC0, 0x20000 };
const uint32_t drvUsart0remapParity[] = { 0x800, 0x0, 0x200, 0x600, 0x400, 0xC00 };
const uint32_t drvUsart0remapStopBits[] = { 0x0, 0x1000, 0x2000 };
const uint32_t drvUsart0remapError[] = { 0x20, 0x80, 0x40 };

const DRV_USART_INTERRUPT_SOURCES drvUSART0InterruptSources =
{
    /* Peripheral has single interrupt vector */
    .isSingleIntSrc                        = true,

    /* Peripheral interrupt line */
    .intSources.usartInterrupt             = USART1_IRQn,
    /* DMA interrupt line */
    .intSources.dmaInterrupt               = XDMAC_IRQn,
};

const DRV_USART_INIT drvUsart0InitData =
{
    .usartPlib = &drvUsart0PlibAPI,

    /* USART Number of clients */
    .numClients = DRV_USART_CLIENTS_NUMBER_IDX0,

    /* USART Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvUSART0ClientObjPool[0],

    .dmaChannelTransmit = DRV_USART_XMIT_DMA_CH_IDX0,

    .usartTransmitAddress = (void *)&(USART1_REGS->US_THR),

    .dmaChannelReceive = SYS_DMA_CHANNEL_NONE,

    /* Combined size of transmit and receive buffer objects */
    .bufferObjPoolSize = DRV_USART_QUEUE_SIZE_IDX0,

    /* USART transmit and receive buffer buffer objects pool */
    .bufferObjPool = (uintptr_t)&drvUSART0BufferObjPool[0],

    .interruptSources = &drvUSART0InterruptSources,

    .remapDataWidth = drvUsart0remapDataWidth,

    .remapParity = drvUsart0remapParity,

    .remapStopBits = drvUsart0remapStopBits,

    .remapError = drvUsart0remapError,
};

// </editor-fold>
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
    .init = (DRV_PL360_HAL_INIT)drv_pl360_hal_init,

    /* PL360 HAL setup */
    .setup = (DRV_PL360_HAL_SETUP)drv_pl360_hal_setup,

    /* PL360 PL360 reset */
    .reset = (DRV_PL360_HAL_RESET)drv_pl360_hal_reset,

    /* PL360 HAL Get Carrier Detect or PLC Line Status */
    .getCd = (DRV_PL360_HAL_GET_CD)drv_pl360_hal_get_cd,

    /* PL360 HAL Enable/Disable external interrupt */
    .enableExtInt = (DRV_PL360_HAL_ENABLE_EXT_INT)drv_pl360_hal_enable_interrupt,

    /* PL360 HAL delay function */
    .delay = (DRV_PL360_HAL_DELAY)drv_pl360_hal_delay,

    /* PL360 HAL Transfer Bootloader Command */
    .sendBootCmd = (DRV_PL360_HAL_SEND_BOOT_CMD)drv_pl360_hal_send_boot_cmd,

    /* PL360 HAL Transfer Write/Read Command */
    .sendWrRdCmd = (DRV_PL360_HAL_SEND_WRRD_CMD)drv_pl360_hal_send_wrrd_cmd,
};

/* PL360 Driver Initialization Data */
DRV_PL360_INIT drvPL360InitData =
{
    /* SPI PLIB API  interface*/
    .pl360Hal = &drvPL360HalAPI,

    /* PL360 Number of clients */
    .numClients = DRV_PL360_CLIENTS_NUMBER_IDX,  

    /* PL360 PLC profile */
    .plcProfile = DRV_PL360_PLC_PROFILE,

    /* Secure Mode */
    .secure = DRV_PL360_SECURE,
    
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SRV_USI Instance 0 Initialization Data">

const SRV_USI_USART_INTERFACE srvUSIUsartApi = {
    .open = (SRV_USI_USART_OPEN)DRV_USART_Open,
    .read = (SRV_USI_USART_READ)DRV_USART_ReadBufferAdd,
    .write = (SRV_USI_USART_WRITE)DRV_USART_WriteBufferAdd,
    .eventHandlerSet = (SRV_USI_USART_EVENT_HANDLER_SET)DRV_USART_BufferEventHandlerSet,
    .close = (SRV_USI_USART_CLOSE)DRV_USART_Close
};

const SRV_USI_INIT srvUSI0InitData =
{
    .usiInterfaceApi = SRV_USI_USART_API,

    .usiApi = (void*)&srvUSIUsartApi,

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
  
    CLK_Initialize();
	PIO_Initialize();


    XDMAC_Initialize();

	RSWDT_REGS->RSWDT_MR = RSWDT_MR_WDDIS_Msk;	// Disable RSWDT 

	WDT_REGS->WDT_MR = WDT_MR_WDDIS_Msk; 		// Disable WDT 

 
    TC0_CH0_TimerInitialize(); 
     
    
	BSP_Initialize();
	SPI0_Initialize();

	USART1_Initialize();


    sysObj.drvUsart0 = DRV_USART_Initialize(DRV_USART_INDEX_0, (SYS_MODULE_INIT *)&drvUsart0InitData);

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
