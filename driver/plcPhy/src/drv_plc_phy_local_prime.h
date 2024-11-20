/*******************************************************************************
  PLC Driver Local PRIME Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_plc_phy_local_comm.h

  Summary:
    PLC Driver Local PRIME Data Structures

  Description:
    Driver Local PRIME Data Structures
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
//DOM-IGNORE-END

#ifndef DRV_PLC_PHY_LOCAL_COMM_H
#define DRV_PLC_PHY_LOCAL_COMM_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "driver/plc/phy/drv_plc_phy_comm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

#define PLC_STATUS_LENGTH     8U
#define PLC_DATA_PKT_SIZE     512U
#define PLC_TX_PAR_SIZE       (uint16_t)(sizeof(DRV_PLC_PHY_TRANSMISSION_OBJ))
#define PLC_RX_PAR_SIZE       (uint16_t)(sizeof(DRV_PLC_PHY_RECEPTION_OBJ))
#define PLC_CMF_PKT_SIZE      (uint16_t)(sizeof(DRV_PLC_PHY_TRANSMISSION_CFM_OBJ))
#define PLC_REG_PKT_SIZE      PLC_DATA_PKT_SIZE

/* Number of transmission buffers */
#define NUM_TX_BUFFERS                             2U

/* FLAG MASKs for set events */
#define DRV_PLC_PHY_EV_FLAG_TX0_CFM_MASK           0x0001U
#define DRV_PLC_PHY_EV_FLAG_TX1_CFM_MASK           0x0002U
#define DRV_PLC_PHY_EV_FLAG_RX_DAT_MASK            0x0004U
#define DRV_PLC_PHY_EV_FLAG_CD_MASK                0x0008U
#define DRV_PLC_PHY_EV_FLAG_REG_MASK               0x0010U
#define DRV_PLC_PHY_EV_FLAG_RX_PAR_MASK            0x0020U

/* FLAG MASKs for register events */
#define DRV_PLC_PHY_REG_LEN_MASK                   0x1FFU
#define DRV_PLC_PHY_REG_ID_MASK                    0xF000U
#define DRV_PLC_PHY_REG_OFFSET_MASK                0x0FFFU
#define DRV_PLC_PHY_REG_ADC_MASK                   0x1000U
#define DRV_PLC_PHY_REG_DAC_MASK                   0x2000U
#define DRV_PLC_PHY_REG_MASK                       0x4000U
#define DRV_PLC_PHY_FUSES_MASK                     0x8000U
#define DRV_PLC_PHY_REG_ADC_BASE                   0x40000000UL
#define DRV_PLC_PHY_REG_DAC_BASE                   0x40004000UL
#define DRV_PLC_PHY_REG_BASE                       0x80000000UL
#define DRV_PLC_PHY_FUSES_BASE                     0x400E1800UL

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* PLC Communication Events

  Summary:
    Object used to track communications events from PLC.

  Description:
    This object is used to keep the data necessary to communicate with PLC.

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
    /* Flag to indicate if CONFIRMATION MSG from buffer 0 event is enable */
    bool evCfm[2];
    /* Flag to indicate if DATA INDICATION MSG event is enable */
    bool evRxDat;
    /* Flag to indicate if QPAR INDICATION MSG event is enable */
    bool evRxPar;
    /* Flag to indicate if REGISTER DATA RESPONSE event is enable */
    bool evReg;
} DRV_PLC_PHY_EVENTS_OBJ;

/* PLC Internal Memory Map

  Summary:
    Object used to refer to data regions inside PLC.

  Description:
    This object is used to refer to the data necessary to communicate with
    PLC.

  Remarks:
    None.
*/

typedef enum {
    STATUS_ID = 0,
    TX0_PAR_ID,
    TX0_DAT_ID,
    TX0_CFM_ID,
    TX1_PAR_ID,
    TX1_DAT_ID,
    TX1_CFM_ID,
    RX_PAR_ID,
    RX_DAT_ID,
    REG_INFO_ID,
    IDS,
} DRV_PLC_PHY_MEM_ID;

/****************************** DRV_PLC_PHY Interface ***************************/

void DRV_PLC_PHY_Init(DRV_PLC_PHY_OBJ *plcPhyObj);
void DRV_PLC_PHY_Task(void);

#endif //#ifndef DRV_PLC_PHY_LOCAL_COMM_H
