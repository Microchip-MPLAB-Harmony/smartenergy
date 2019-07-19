/*******************************************************************************
  G3 MAC RT Driver Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_g3_macrt_local.h

  Summary:
    G3 MAC RT Driver Local Data Structures

  Description:
    G3 MAC Real Time Driver Local Data Structures
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _DRV_G3_MACRT_LOCAL_H
#define _DRV_G3_MACRT_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "system/system.h"
#include "driver/plc/g3MacRt/drv_g3_macrt.h"
#include "driver/plc/g3MacRt/drv_g3_macrt_definitions.h"
#include "driver/plc/g3MacRt/drv_g3_macrt_comm.h"
#include "driver/plc/common/drv_plc_boot.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* DRV_G3_MACRT Command set

  Summary:
    Enumeration listing the DRV_G3_MACRT commands.

  Description:
    This enumeration defines the commands used to interact with the DRV_G3_MACRT
    series of devices.

  Remarks:
    None
*/

typedef enum
{
    /* Write command */
    DRV_G3_MACRT_CMD_WRITE              = (1 << 10),

    /* Read command */
    DRV_G3_MACRT_CMD_READ               = (0 << 10)

} DRV_G3_MACRT_CMD;

// *****************************************************************************
/* DRV_G3_MACRT Transfer Object State

  Summary:
    Defines the status of the DRV_G3_MACRT Transfer Object.

  Description:
    This enumeration defines the status of the DRV_G3_MACRT Transfer Object.

  Remarks:
    None.
*/

typedef enum
{
    DRV_G3_MACRT_STATE_IDLE,
    DRV_G3_MACRT_STATE_TX,
    DRV_G3_MACRT_STATE_WAITING_TX_CFM,
    DRV_G3_MACRT_STATE_ERROR,
}DRV_G3_MACRT_STATE;

// *****************************************************************************
/* PLC Driver Instance Object

  Summary:
    Object used to keep any data required for an instance of the PLC driver.

  Description:
    None.

  Remarks:
    None.
*/

typedef struct
{
    /* Flag to indicate this object is in use  */
    bool                                      inUse;

    /* State of the MAC RT driver */
    DRV_G3_MACRT_STATE                        state;

    /* Keep track of the number of clients that have opened this driver */
    size_t                                    nClients;

    /* Maximum number of clients */
    size_t                                    nClientsMax;

    /* The status of the driver */
    SYS_STATUS                                status;

    /* HAL API list that will be used by the driver to access the hardware */
    DRV_PLC_HAL_INTERFACE                     *plcHal;

    /* PLC Profile */
    uint8_t                                   plcProfile;

    /* PLC Specification Compliance */
    uint8_t                                   plcSpecification;
    
    /* Size (in Bytes) of the PLC binary file */
    uint32_t                                  binSize;

    /* Address where PLC binary file is located */
    uint32_t                                  binStartAddress;

    /* Secure mode */
    bool                                      secure;

    /* Application Bootloader Data Callback */
    DRV_PLC_BOOT_DATA_CALLBACK                bootDataCallback;

    /* Application Transmission Confirm Callback */
    DRV_G3_MACRT_TX_CFM_CALLBACK              txCfmCallback;

    /* Application Data Indication Callback */
    DRV_G3_MACRT_DATA_IND_CALLBACK            dataIndCallback;

    /* Application MLME Get Confirm Callback */
    DRV_G3_MACRT_MLME_GET_CFM_CALLBACK        mlmeGetCfmcallback;

    /* Application Exception Callback */
    DRV_G3_MACRT_EXCEPTION_CALLBACK           exceptionCallback;

    /* Application PLC Sniffer Callback */
    DRV_G3_MACRT_SNIFFER_CALLBACK             snifferDataCallback;

    /* Application context for Transmission Confirm Callback */
    uintptr_t                                 contextTxCfm;

    /* Application context for Data Indication Callback */
    uintptr_t                                 contextDataInd;

    /* Application context for MLME Get Confirm Callback */
    uintptr_t                                 contextMlmeGetCfm;

    /* Application context for Exception Callback */
    uintptr_t                                 contextExc;

    /* Application context for Bootloader Data Callback */
    uintptr_t                                 contextBoot;

    /* Application context for PLC Sniffer Data Callback */
    uintptr_t                                 contextSniffer;

    /* Event detection flag: confirmation of the previous transmission */
    volatile bool                             evTxCfm;

    /* Event detection flag: data of new reception */
    volatile uint16_t                         evDataIndLength;

    /* Event detection flag: parameters of new reception */
    volatile bool                             evMlmeGetCfm;

    /* Event detection flag: tone map response */
    volatile bool                             evToneMapRsp;

    /* Event detection flag: length of the response with register content */
    volatile uint16_t                         evRegRspLength;

    /* Event detection flag: new PLC sniffer packet */
    volatile bool                             evSniffer;

    /* Event detection flag: reset waiting tx cfm */
    volatile bool                             evResetTxCfm;
    
    /* Pointer to Sniffer Data Buffer */
    uint8_t                                   *pDataSniffer;

} DRV_G3_MACRT_OBJ;


#endif //#ifndef _DRV_G3_MACRT_LOCAL_H
