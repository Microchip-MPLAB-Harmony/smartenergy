/*******************************************************************************
  PL360 Driver Local Communication Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_pl360_local_comm.h

  Summary:
    PL360 Driver Local Communication Data Structures

  Description:
    Driver Local Communication Data Structures
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

#ifndef _DRV_PL360_LOCAL_COMM_H
#define _DRV_PL360_LOCAL_COMM_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "driver/phy/pl360/drv_pl360_comm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************
#define PL360_STATUS_LENGTH     8
#define PL360_DATA_PKT_SIZE     512
#define PL360_TX_PAR_SIZE       sizeof(DRV_PL360_TRANSMISSION_OBJ)
#define PL360_RX_PAR_SIZE       sizeof(DRV_PL360_RECEPTION_OBJ)
#define PL360_CMF_PKT_SIZE      sizeof(DRV_PL360_TRANSMISSION_CFM_OBJ)
#define PL360_REG_PKT_SIZE      PL360_DATA_PKT_SIZE

/* Number of transmission buffers */
#define NUM_TX_BUFFERS                      1

// *****************************************************************************
/* PRIME Structure defining PLC events */
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
} DRV_PL360_EVENTS_OBJ;

/* Internal Memory Map */
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
} DRV_PL360_MEM_ID;

/* ! FLAG MASKs for set events */
#define DRV_PL360_EV_FLAG_TX0_CFM_MASK           0x0001
#define DRV_PL360_EV_FLAG_TX1_CFM_MASK           0x0002
#define DRV_PL360_EV_FLAG_RX_DAT_MASK            0x0004
#define DRV_PL360_EV_FLAG_CD_MASK                0x0008
#define DRV_PL360_EV_FLAG_REG_MASK               0x0010
#define DRV_PL360_EV_FLAG_RX_PAR_MASK            0x0020

/* ! FLAG MASKs for register events */
#define DRV_PL360_REG_LEN_MASK                   0x1FF
#define DRV_PL360_REG_ID_MASK                    0xF000
#define DRV_PL360_REG_OFFSET_MASK                0x0FFF
#define DRV_PL360_REG_ADC_MASK                   0x1000
#define DRV_PL360_REG_DAC_MASK                   0x2000
#define DRV_PL360_REG_MASK                       0x4000
#define DRV_PL360_FUSES_MASK                     0x8000
#define DRV_PL360_REG_ADC_BASE                   0x40000000
#define DRV_PL360_REG_DAC_BASE                   0x40004000
#define DRV_PL360_REG_BASE                       0x80000000
#define DRV_PL360_FUSES_BASE                     0x400E1800

void drv_pl360_comm_init(DRV_PL360_OBJ *pl360);
void drv_pl360_comm_task(void);

#endif //#ifndef _DRV_PL360_LOCAL_COMM_H
