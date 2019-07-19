/*******************************************************************************
  PLC Driver Local G3 Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_g3_macrt_local_comm.h

  Summary:
    G3 MAC RT Driver Local G3 MAC RT Data Structures

  Description:
    Driver Local G3 MAC RT Data Structures
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

#ifndef _DRV_G3_MACRT_LOCAL_COMM_H
#define _DRV_G3_MACRT_LOCAL_COMM_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "driver/plc/g3MacRt/drv_g3_macrt_comm.h"
#include "driver/plc/g3MacRt/src/drv_g3_macrt_local.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// ***************************************************************************** 
#define PLC_STATUS_LENGTH                  8
#define DRV_G3_MACRT_DATA_MAX_SIZE         512
#define DRV_G3_MACRT_MLME_SET_SIZE         sizeof(MAC_RT_TX_PARAMETERS_OBJ)
#define DRV_G3_MACRT_MLME_GET_SIZE         sizeof(MAC_RT_RX_PARAMETERS_OBJ)
#define DRV_G3_MACRT_TX_CFM_SIZE           sizeof(MAC_RT_TX_CFM_OBJ)
#define DRV_G3_MACRT_TONEMAP_RSP_SIZE      sizeof(MAC_RT_TONE_MAP_RSP)
#define DRV_G3_MACRT_REG_PKT_SIZE          156

/* FLAG MASKs for set events */
#define DRV_G3_MACRT_EV_TX_CFM_MASK            0x0001
#define DRV_G3_MACRT_EV_MLME_SET_CFM_MASK      0x0002
#define DRV_G3_MACRT_EV_DATA_IND_MASK          0x0004
#define DRV_G3_MACRT_EV_MLME_GET_CFM_MASK      0x0008
#define DRV_G3_MACRT_EV_REG_RSP_MASK           0x0010
#define DRV_G3_MACRT_EV_TONMAP_RSP_MASK        0x0020
#define DRV_G3_MACRT_EV_SNIFFER_MASK           0x0040

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* G3 MACRT Communication Events

  Summary:
    Object used to track communications events from G3 MACRT.

  Description:
    This object is used to keep the data necessary to communicate with G3 MACRT.

  Remarks:
    None.
*/

typedef struct {
    /* HW Timer reference */
    uint32_t timerRef;
    /* Received Data length */
    uint16_t rcvDataLength;
    /* Received Register Response length */
    uint16_t regRspLength;
    /* Flag to indicate if TX CONFIRMATION MSG event is enable */
    bool evTxCfm;
    /* Flag to indicate if DATA INDICATION MSG event is enable */
    bool evDataInd;
    /* Flag to indicate if MLME SET CONFIRM MSG event is enable */
    bool evMlmeSetCfm;
    /* Flag to indicate if MLME GET CONFIRM MSG event is enable */
    bool evMlmeGetCfm;
    /* Flag to indicate if TONE MAP RESPONSE MSG event is enable */
    bool evToneMapRsp;
    /* Flag to indicate if REGISTER DATA RESPONSE event is enable */
    bool evReg;
    /* Flag to indicate if SNIFFER MSG event is enable */
    bool evSniffer;
} DRV_G3_MACRT_EVENTS_OBJ;

/* G3 MACRT Internal Memory Map

  Summary:
    Object used to refer to data regions inside G3 MACRT.

  Description:
    This object is used to refer to the data necessary to communicate with 
    G3 MACRT.

  Remarks:
    None.
*/

typedef enum {
    STATUS_ID = 0,
    SET_COORD_ID,
    SET_SPEC15_ID,
    TONE_MAP_REQ_ID,
    TX_REQ_ID,
    TX_CFM_ID,
    MLME_SET_ID,
    MLME_SET_CMF_ID,
    DATA_IND_ID,
    MLME_GET_CFM_ID,
    REG_INFO_ID,
    SNIFFER_ID,
    IDS,
} DRV_G3_MACRT_MEM_ID;

/****************************** DRV_PLC Interface ***************************/

void DRV_G3_MACRT_Init(DRV_G3_MACRT_OBJ *g3MacRt);
void DRV_G3_MACRT_Task(void);

#endif //#ifndef _DRV_G3_MACRT_LOCAL_COMM_H
