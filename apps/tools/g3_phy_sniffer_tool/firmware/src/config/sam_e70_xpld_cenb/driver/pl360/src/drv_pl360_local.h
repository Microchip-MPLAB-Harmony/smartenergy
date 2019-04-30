/*******************************************************************************
  PL360 Driver Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360_local.h

  Summary:
    PL360 Driver Local Data Structures

  Description:
    Driver Local Data Structures
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _DRV_PL360_LOCAL_H
#define _DRV_PL360_LOCAL_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "system/system.h"
#include "driver/pl360/drv_pl360.h"
#include "driver/pl360/drv_pl360_definitions.h"
#include "driver/pl360/drv_pl360_comm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* DRV_PL360 Command set

  Summary:
    Enumeration listing the DRV_PL360 commands.

  Description:
    This enumeration defines the commands used to interact with the DRV_PL360
    series of devices.

  Remarks:
    None
*/

typedef enum
{
    /* Write command */
    DRV_PL360_CMD_WRITE              = (1 << 10),

    /* Read command */
    DRV_PL360_CMD_READ               = (0 << 10)

} DRV_PL360_CMD;

typedef enum
{
    DRV_PL360_STATE_IDLE,
    DRV_PL360_STATE_TX,
    DRV_PL360_STATE_WAITING_TX_CFM,
    DRV_PL360_STATE_ERROR,
}DRV_PL360_STATE;

// *****************************************************************************
/* PL360 Driver Instance Object

  Summary:
    Object used to keep any data required for an instance of the PL360 driver.

  Description:
    None.

  Remarks:
    None.
*/

typedef struct
{
    /* Flag to indicate this object is in use  */
    bool                            inUse;

    DRV_PL360_STATE                 state;

    /* Keep track of the number of clients
      that have opened this driver */
    size_t                          nClients;

    /* Maximum number of clients */
    size_t                          nClientsMax;

    /* The status of the driver */
    SYS_STATUS                      status;

    /* HAL API list that will be used by the driver to access the hardware */
    DRV_PL360_HAL_INTERFACE        *pl360Hal;

    /* PLC Profile */
    uint8_t                         plcProfile;

    /* Size (in Bytes) of the PLC binary file */
    uint32_t                        binSize;

    /* Address where PLC binary file is located */
    uint32_t                        binStartAddress;

    /* Secure mode */
    bool                            secure;

    /* Application Data Confirm Callback */
    DRV_PL360_DATA_CFM_CALLBACK     dataCfmCallback;

    /* Application Data Indication Callback */
    DRV_PL360_DATA_IND_CALLBACK     dataIndCallback;

    /* Application Exception Callback */
    DRV_PL360_EXCEPTION_CALLBACK    exceptionCallback;

    /* Application context */
    uintptr_t                       context;

    /* Event detection flag: confirmation of the previous transmission */
    volatile bool                   evTxCfm[2];

    /* Event detection flag: parameters of new reception */
    volatile bool                   evRxPar;

    /* Event detection flag: data of new reception */
    volatile bool                   evRxDat;

    /* Event detection flag: length of the response with register content */
    volatile uint16_t               evRegRspLength;

    /* Event detection flag: reset waiting tx cfm */
    volatile bool                   evResetTxCfm;

} DRV_PL360_OBJ;


#endif //#ifndef _DRV_PL360_LOCAL_H
