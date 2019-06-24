/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <string.h>
#include "app.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

#define APP_TX_DELAY_US           1000000 /* 1 second */

#define div_ceil(a, b)            (((a) + (b) - 1) / (b))

static bool _rxCheckCrc(DRV_PL360_RECEPTION_OBJ *indObj);

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void Timer1_Callback ( uintptr_t context )
{
    /* RX Led off */
    LED_Set();
}

static void _pl360ExceptionCb(DRV_PL360_EXCEPTION exceptionObj, uintptr_t context )
{
    switch (exceptionObj) 
    {
        case DRV_PL360_EXCEPTION_UNEXPECTED_KEY:
            appData.pl360_err_unexpected++;
            SYS_DEBUG_Print("Exception: DRV_PL360_EXCEPTION_UNEXPECTED_KEY [%u]\r\n", appData.pl360_err_unexpected);
            break;

        case DRV_PL360_EXCEPTION_CRITICAL_ERROR:
            appData.pl360_err_critical++;
            SYS_DEBUG_Print("Exception: DRV_PL360_EXCEPTION_CRITICAL_ERROR [%u]\r\n", appData.pl360_err_critical);
            break;

        case DRV_PL360_EXCEPTION_RESET:
            appData.pl360_err_reset++;
            SYS_DEBUG_Print("Exception: DRV_PL360_EXCEPTION_RESET [%u]\r\n", appData.pl360_err_reset);
            break;

        default:
            appData.pl360_err_unknow++;
            SYS_DEBUG_Print("Exception: UNKNOWN [%u]\r\n", appData.pl360_err_unknow);
	}

	appData.pl360_exception = true;
}

static void _pl360DataCfmCb(DRV_PL360_TRANSMISSION_CFM_OBJ *cfmObj, uintptr_t context )
{
    appData.state = APP_STATE_TX;
    
    /* Handle result of transmission : Show it through Console */
    switch(cfmObj->result)
    {
        case DRV_PL360_TX_RESULT_PROCESS:
            SYS_DEBUG_Print("...DRV_PL360_TX_RESULT_PROCESS\r\n");
            break;   
        case DRV_PL360_TX_RESULT_SUCCESS:
            SYS_DEBUG_Print("...DRV_PL360_TX_RESULT_PROCESS\r\n");
            break;   
        case DRV_PL360_TX_RESULT_INV_LENGTH:
            SYS_DEBUG_Print("...DRV_PL360_TX_RESULT_INV_LENGTH\r\n");
            break;
        case DRV_PL360_TX_RESULT_BUSY_CH:
            SYS_DEBUG_Print("...DRV_PL360_TX_RESULT_BUSY_CH\r\n");
            break;    
        case DRV_PL360_TX_RESULT_BUSY_TX:
            SYS_DEBUG_Print("...DRV_PL360_TX_RESULT_BUSY_TX\r\n");
            break;    
        case DRV_PL360_TX_RESULT_BUSY_RX:
            SYS_DEBUG_Print("...DRV_PL360_TX_RESULT_BUSY_RX\r\n");
            break;   
        case DRV_PL360_TX_RESULT_INV_SCHEME:
            SYS_DEBUG_Print("...DRV_PL360_TX_RESULT_INV_SCHEME\r\n");
            break; 
        case DRV_PL360_TX_RESULT_TIMEOUT:
            SYS_DEBUG_Print("...DRV_PL360_TX_RESULT_TIMEOUT\r\n");
            break;   
        case DRV_PL360_TX_RESULT_INV_TONEMAP:
            SYS_DEBUG_Print("...DRV_PL360_TX_RESULT_INV_TONEMAP\r\n");
            break;
        case DRV_PL360_TX_RESULT_INV_MODE:
            SYS_DEBUG_Print("...DRV_PL360_TX_RESULT_INV_MODE\r\n");
            break;   
        case DRV_PL360_TX_RESULT_NO_TX:
            SYS_DEBUG_Print("...DRV_PL360_TX_RESULT_NO_TX\r\n");
            break;   
    }
    
}

static void _pl360DataIndCb(DRV_PL360_RECEPTION_OBJ *indObj, uintptr_t context )
{   
    /* Shows Received PLC message */
	if (indObj->dataLength) {
		/* Check CRC and extract padding */
		if (_rxCheckCrc(indObj)) {
            /* CRC Ok */
            /* Start Timer: LED blinking for each received message */
            LED_Clear();
            appData.tmr1Handle = SYS_TIME_CallbackRegisterMS(Timer1_Callback, 0, LED_RX_OFF_RATE_MS, SYS_TIME_SINGLE);
            /* Copy data to Application buffer, excluding CRC info */
            memcpy(appData.pDataRx, indObj->pReceivedData, indObj->dataLength - 2);
            
			/* Show received message */
            appData.pDataRx[indObj->dataLength - 2] = 0;
			SYS_DEBUG_Print("<- Received new msg (%u bytes)\r\n", (unsigned int)indObj->dataLength - 2);
			SYS_DEBUG_Print("\tData msg: %s", appData.pDataRx);
		} else {
			/* CRC Error */
			SYS_DEBUG_Print("<- Received new msg : CRC error\r\n");
		}
	}
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/
#define DRV_PL360_INDEX_0   0

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
static const uint16_t gCrc16Table[256] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};

static uint16_t _crc16Ccitt(const uint8_t *pData, uint32_t length, uint16_t initValue)
{
	/* polynom(16): X16 + X12 + X5 + 1 = 0x1021 */
	while (length--) {
		initValue = gCrc16Table[(initValue >> 8) ^ (*pData++)] ^ ((initValue & 0xFF) << 8);
	}
	return initValue;
}

static uint8_t _getPaddingLength(DRV_PL360_MOD_TYPE modType, uint16_t dataLength)
{
	uint32_t interleaverInputSize;
	uint32_t interleaverMaxSize;
	uint32_t padding;
	uint8_t repCode;
	uint8_t rsParity;
	uint8_t bitsPerCarrier;
	uint8_t numCarriers;
	uint8_t flBand;
	uint8_t paddingBytes;
    
#if (DRV_PL360_PLC_PROFILE == G3_CEN_A)
	numCarriers = NUM_CARRIERS_CENELEC_A;
	flBand = 4;
#elif (DRV_PL360_PLC_PROFILE == G3_CEN_B)
	numCarriers = NUM_CARRIERS_CENELEC_B;
	flBand = 4;
#elif (DRV_PL360_PLC_PROFILE == G3_FCC)
	numCarriers = NUM_CARRIERS_FCC;
	flBand = 1;
#else
#error ERROR in PHY band definition
#endif
    
	if (modType == MOD_TYPE_BPSK_ROBO) {
		bitsPerCarrier = 1;
		repCode = 4;
		rsParity = 8;
	} else {
		bitsPerCarrier = modType + 1;
		repCode = 1;
		rsParity = 16;
	}

	/* Get interleaver input size */
	interleaverInputSize = (dataLength + 2 + rsParity) << 3; /* data len + CRC + RS parity */
	interleaverInputSize += 6; /* 6 bits of Convolutional Code */
	interleaverInputSize *= (repCode << 1); /* In case of ROBO mode */

	/* Get interleaver maximum buffer */
	interleaverMaxSize = div_ceil(interleaverInputSize, numCarriers * bitsPerCarrier * flBand * repCode);
	interleaverMaxSize = interleaverMaxSize * numCarriers * bitsPerCarrier * flBand * repCode;

	/* Get padding in bytes */
	padding = interleaverMaxSize - interleaverInputSize;
	paddingBytes = padding / (repCode << 4);

	return paddingBytes;
}

static void _txInsertPaddingCcrc(void)
{
	uint8_t *pDataEnd;
	uint16_t crc;
	uint8_t padLength;

	pDataEnd = &appData.pDataTx[appData.pl360Tx.dataLength];

	/* Get padding length */
	padLength = _getPaddingLength(appData.pl360Tx.modType, appData.pl360Tx.dataLength);

	/* Encode the padding */
	memset(pDataEnd, 0, padLength);
	pDataEnd += padLength;

	/* Calculate and append FCS */
	crc = _crc16Ccitt(appData.pDataTx, appData.pl360Tx.dataLength + padLength, 0);
	*pDataEnd++ = (uint8_t)crc;
	*pDataEnd = (uint8_t)(crc >> 8);

	/* Adjust data length */
	appData.pl360Tx.dataLength += (2 + padLength);
}

static void _setupTransmissionParameters (void)
{
    /* Configure Tone Map, it depends on PLC profile */
#if (DRV_PL360_PLC_PROFILE == G3_CEN_A)
	appData.pl360Tx.toneMap[0] = 0x3F;
#elif (DRV_PL360_PLC_PROFILE == G3_CEN_B)
	appData.pl360Tx.toneMap[0] = 0x0F;
#elif (DRV_PL360_PLC_PROFILE == G3_FCC)
	appData.pl360Tx.toneMap[0] = 0xFF;
    appData.pl360Tx.toneMap[1] = 0xFF;
    appData.pl360Tx.toneMap[2] = 0xFF;
#else
#error ERROR in PHY band definition
#endif    
    
    /* Set pre-emphasis filter */
	memset(appData.pl360Tx.preemphasis, 0, sizeof(appData.pl360Tx.preemphasis));
	/* Use only 1 RS block */
	appData.pl360Tx.rs2Blocks = 0;
	/* ACK management is not needed. */
	appData.pl360Tx.delimiterType = DT_SOF_NO_RESP;
	/* Set modulation scheme. */
	appData.pl360Tx.modScheme = MOD_SCHEME_DIFFERENTIAL;
	/* Set modulation type. */
	appData.pl360Tx.modType = MOD_TYPE_BPSK;
	/* Set phase detector counter */
	appData.pl360Tx.pdc = 0;
	/* Set Transmission Mode. */
	appData.pl360Tx.mode = TX_MODE_FORCED | TX_MODE_RELATIVE;
	/* Value depends on TX time mode. Relative + TX_Time = 0 to set instantaneous transmission */
	appData.pl360Tx.time = APP_TX_DELAY_US;
	/* Set transmission power. It represents 3dBs of Attenuation signal per Unit. 0 value for maximum signal level. */
	appData.pl360Tx.attenuation = 0;
    
}

static void _setupTransmissionData (void)
{
    uint16_t dataLength;

    /* Set data content */
	dataLength = sprintf((char *)appData.pDataTx, "G3 PLC Message num:%u\r\n", (unsigned int)appData.counterTx);
    appData.pl360Tx.dataLength = dataLength;
    
    /* Insert padding and CRC */
	_txInsertPaddingCcrc();
    
}

static bool _rxCheckCrc(DRV_PL360_RECEPTION_OBJ *indObj)
{
	uint8_t *pDataEnd;
	uint16_t crc;
	uint16_t crcRcv;

	/* Extract CRC */
	pDataEnd = &indObj->pReceivedData[indObj->dataLength];
	crcRcv = *(pDataEnd - 2);
	crcRcv += (*(pDataEnd - 1) << 8);

	/* Calculate FCS */
	crc = _crc16Ccitt(indObj->pReceivedData, indObj->dataLength - 2, 0);

	/* Check CRC */
	if (crc != crcRcv) {
		/* CRC Error */
		return false;
	}

	return true;
}

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_IDLE;
    //appData.mode = APP_MODE;
    
    /* Init Timer handler */
    appData.tmr1Handle = SYS_TIME_HANDLE_INVALID;
    
    /* Reset PL360 exceptions statistics */
    appData.pl360_err_unexpected = 0;
    appData.pl360_err_critical = 0;
    appData.pl360_err_reset = 0;
    appData.pl360_err_unknow = 0;
    
    /* Init PLC statistics */
    appData.counterTx = 0;
    appData.counterRx = 0;
    appData.counterTxCfm = 0;
    
    /* Update state machine */
    appData.state = APP_STATE_INIT;
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
            /* Open PL360 driver */
            appData.drvPl360Handle = DRV_PL360_Open(DRV_PL360_INDEX_0, NULL);
            
            if (appData.drvPl360Handle != DRV_HANDLE_INVALID)
            {
                appData.state = APP_STATE_DEV_OPEN;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }
            break;
            
        case APP_STATE_DEV_OPEN:
            /* Check PL360 device */
            if (DRV_PL360_Status(DRV_PL360_INDEX_0) == SYS_STATUS_READY)
            {
                /* Configure PL360 callbacks */
                DRV_PL360_ExceptionCallbackRegister(appData.drvPl360Handle, _pl360ExceptionCb, DRV_PL360_INDEX_0);

                if (appData.mode == APP_MODE_TRANSMISION)
                {
                    SYS_DEBUG_Print("PL360 device ready to transmit data...\r\n");
                    DRV_PL360_DataCfmCallbackRegister(appData.drvPl360Handle, _pl360DataCfmCb, DRV_PL360_INDEX_0);
                    appData.state = APP_STATE_TX_SETUP;
                    appData.pl360Tx.pTransmitData = appData.pDataTx;
                }
                else
                {
                    SYS_DEBUG_Print("PL360 device ready to receive data...\r\n");
                    DRV_PL360_DataIndCallbackRegister(appData.drvPl360Handle, _pl360DataIndCb, DRV_PL360_INDEX_0);
                    appData.state = APP_STATE_WAITING_RX;
                }

            }                
            break;

        case APP_STATE_TX_SETUP:
        {
            _setupTransmissionParameters();
            appData.state = APP_STATE_TX;
            break;
        }

        case APP_STATE_TX:
        {
            /* update application counters */
            appData.counterTx++;
            /* update data message */
            _setupTransmissionData();
            
            SYS_DEBUG_Print("\r\n-> Sending new PLC msg(%u)...size:%u bytes\r\n", appData.counterTx, appData.pl360Tx.dataLength);
            
            DRV_PL360_Send(appData.drvPl360Handle, &appData.pl360Tx);
            appData.state = APP_STATE_WAITING_TX_CFM;
            break;
        }

        case APP_STATE_WAITING_TX_CFM:
        {
            /* Waiting */
            break;
        }

        case APP_STATE_RX:
        {
            appData.state = APP_STATE_WAITING_RX;
            break;
        }

        case APP_STATE_WAITING_RX:
        {
            /* Waiting */
            break;
        }

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
