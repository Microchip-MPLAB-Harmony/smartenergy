/*******************************************************************************
  Header for log report service

  Company:
    Microchip Technology Inc.

  File Name:
    srv_log_report.h

  Summary:
    Interface definition for the log report service.

  Description:
    This file defines the interface for the log report service.
    Debug messages and log information is printed on the console.
    If a display is available, debug code errors will be shown.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2024, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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

#ifndef SRV_LOG_REPORT_H
#define SRV_LOG_REPORT_H

<#if (ENABLE_TRACES == true)>
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include "system/debug/sys_debug.h"

</#if>
#ifdef __cplusplus // Provide C++ Compatibility
 extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Log level enumeration

   Summary:
    Log message priority levels.

   Description:
    This enumeration maps the log levels to the supported system error
    message priority values.

   Remarks:
    Used to add the right individual message priority before reporting through
    SYS_DEBUG.
*/

<#if (ENABLE_TRACES == true)>
typedef enum
{
    /* Errors that have the potential to cause a system crash. */
    SRV_LOG_REPORT_FATAL = SYS_ERROR_FATAL,

    /* Errors that have the potential to cause incorrect behavior. */
    SRV_LOG_REPORT_ERROR = SYS_ERROR_ERROR,

    /* Warnings about potentially unexpected behavior or side effects. */
    SRV_LOG_REPORT_WARNING = SYS_ERROR_WARNING,

    /* Information helpful to understanding potential errors and warnings. */
    SRV_LOG_REPORT_INFO = SYS_ERROR_INFO,

    /* Verbose information helpful during debugging and testing. */
    SRV_LOG_REPORT_DEBUG = SYS_ERROR_DEBUG

} SRV_LOG_REPORT_LEVEL;

<#else>
typedef enum
{
    /* Errors that have the potential to cause a system crash. */
    SRV_LOG_REPORT_FATAL = 0,

    /* Errors that have the potential to cause incorrect behavior. */
    SRV_LOG_REPORT_ERROR = 1,

    /* Warnings about potentially unexpected behavior or side effects. */
    SRV_LOG_REPORT_WARNING = 2,

    /* Information helpful to understanding potential errors and warnings. */
    SRV_LOG_REPORT_INFO = 3,

    /* Verbose information helpful during debugging and testing. */
    SRV_LOG_REPORT_DEBUG = 4

} SRV_LOG_REPORT_LEVEL;

</#if>

// *****************************************************************************
/* Error/warning code enumeration

   Summary:
    Error/warning codes.

   Description:
    This enumeration lists the error and warning codes.

   Remarks:
    None.
*/

typedef enum
{
    /* PRIME CL_432 Errors */
    CL_432_BAD_HANDLER_RECEIVED         =   100,
    CL_432_UNKNOWN_HANDLER_IN_CONFIRM   =   101,

    /* PRIME DB Errors */
    DB_GET_PARAM_BAD_ID                 =   200,
    DB_GET_PARAM_BAD_SIZE               =   201,
    DB_SET_PARAM_BAD_ID                 =   202,
    DB_SET_PARAM_BAD_SIZE               =   203,
    DB_GET_ELEM_BAD_ID                  =   204,
    DB_GET_ELEM_BAD_SIZE                =   205,
    DB_GET_ELEM_BAD_INDEX               =   206,
    DB_SET_ELEM_BAD_ID                  =   207,
    DB_SET_ELEM_BAD_SIZE                =   208,
    DB_SET_ELEM_BAD_INDEX               =   209,

    /* PRIME DISPATCHER Errors */
    DIS_ERROR_BUFF_ADDRESS_SEND         =   300,
    DIS_EMPTY_ELEM_INSERT               =   301,
    DIS_MODULE_NOT_REGISTERED           =   302,
    DIS_ADD_MSG_ALREADY_ADDED           =   303,
    DIS_MSG_GEN_ERROR_MSG_DEL           =   304,
    DIS_TRY_SEND_MSG_DEL                =   305,    
    DIS_NULL_BUFF_QUEUE                 =   306,

    /* PRIME NOTIFICATIONS Errors */
    NOT_NOT_VALID_SIGNAL                =   400,
    NOT_TABLE_FULL                      =   401,
    NOT_TOO_MANY_EVENTS                 =   402,
    NOT_WRONG_POINTER                   =   403,

    /* PRIME BMM Errors */
    BMM_ALLOC_BUFF_BIGGER_THAN_AVAILABLE=   500,
    BMM_ALLOC_NO_FREE_BUFF              =   501,
    BMM_FREE_FREE_BUFF                  =   502,  
    BMM_FREE_ERROR                      =   503,
    BMM_BUFFER_INTEGRITY_BROKEN         =   504,

    /* Queue Service Errors */
    QUEUE_FULL_INSERT_END               =   600,
    QUEUE_FULL_INSERT_BEFORE            =   601,
    QUEUE_FULL_INSERT_AFTER             =   602,
    QUEUE_FULL_INSERT_FIRST             =   603,
    QUEUE_EMPTY_REMOVE_HEAD             =   604,
    QUEUE_EMPTY_READ_REMOVE             =   605,
    QUEUE_FULL_APPEND                   =   606,
    QUEUE_APPEND_BAD_ELEMENT            =   607,
    QUEUE_BAD_ELEMENT                   =   608,
    QUEUE_BAD_TAIL                      =   609,
    QUEUE_APPEND_PRIO_SINGLE            =   610,
    QUEUE_NOT_INIT_NULL_SIZE            =   611,
    QUEUE_FIRST_BAD_INIT                =   612,
    QUEUE_LAST_NOT_TAIL                 =   613,
    QUEUE_TOO_BIG                       =   614,
    QUEUE_BAD_NEXT_ELEMENT              =   615,
    QUEUE_WRONG_CHAIN                   =   616,
    QUEUE_APPEND_AGAIN                  =   617,
    QUEUE_APPEND_AGAIN_ONE_ELEMENT      =   618,

    /* PRIME TMM Errors */
    TMM_NO_FREE_TIMER                   =   700,
    TMM_QUEUE_EMPTY_REMOVE              =   701,
    TMM_START_NULL_CB_FUNCTION          =   702,
    TMM_START_INVALID_TIMER_VALUE       =   703,
    TMM_START_NO_TIME                   =   704,

    /* PRIME BCN Errors */
    BCN_BAD_VALUES_ADJUSTING            =   800,

    /* PRIME IQ errors */
    IQ_PRO_BAD_QUEUE                    =   900,
    IQ_PRO_START_PRO                    =   901,
    IQ_PRO_CONT_PRO                     =   902,
    IQ_PRO_END_PRO                      =   903,
    IQ_PRO_BUSY_START_PRO               =   904,
    IQ_PRO_SERVICE_BAD_STATE            =   905,
    IQ_PRO_MAX_TIME_PROMOTING           =   906,
    IQ_NETWORK_NODE_DISCONNECTED_STATE  =   907,

    /* PRIME BSI Errors */
    BSI_ERROR                           =   1000,

    /* PRIME PRO Errors */
    PRO_START_PRO                       =   1100,
    PRO_END_PRO_OK                      =   1101,
    PRO_END_PRO_BAD                     =   1102,
    PRO_START_DEM                       =   1103,
    PRO_END_NODE_DISCONNECTED           =   1104,
    PRO_FIRST_DEM                       =   1105,
    PRO_NO_VBS_FREE                     =   1106,

    /* PRIME PHY Wrapper Errors */
    PHY_WRP_CRC_WRONG_HEADER_TYPE       =   1200,
    PHY_WRP_WRONG_LEN                   =   1201,
    PHY_WRP_WRONG_MSG_TYPE              =   1202,
    PHY_WRP_MUL_LCID_NOT_FOUND          =   1203,

    /* PRIME PNPDU Errors */
    PNU_ENHANCED_NOT_AVAILABLE          =   1300,

    /* PRIME SWI Errors */
    SWI_INVALID_CHANNEL                 =   1400,
    
    /* PRIME Management Plane Errors */
    MNGP_FU_ST_MACHINE_CONFIRM_TIMEOUT  =   1500,
    MNGP_FU_ST_MACHINE_WRONG_BUFFER     =   1501,
    MNGP_FU_NODE_MNG_CONFIRM_TIMEOUT    =   1502,
    
    /* PRIME DATA Errors */
    DATA_TX_EXTRA_QUEUE_FULL            =   1600,
    DATA_CONN_CLOSED_INVALID_HANDLER    =   1601,

    /* PRIME CON Errors */
    CON_UNICAST_NO_COMMON_ANCESTOR      =   1700,
    CON_UNICAST_BAD_RESPONSE            =   1701,
    CON_MULTICAST_SW_INCREASE_OVERFLOW  =   1702,
    CON_MULTICAST_SW_DECREASE_OVERFLOW  =   1703,    
    CON_MULTICAST_SW_SONS_INCREASE_OVERFLOW  =   1704,
    CON_MULTICAST_SW_SONS_DECREASE_OVERFLOW  =   1705,  
    CON_MULTICAST_NO_CONNECTIONS        =   1706,
    
    /* USI Service Errors */
    USI_BAD_LENGTH                      =   9001,
    USI_BAD_PROTOCOL                    =   9002,
    USI_BAD_CRC                         =   9003,
    USI_INVALID_LENGTH                  =   9004,
    USI_ERROR_ESCAPE                    =   9005,

   /* PRIME PHY Layer Errors */
    PHY_LAYER_PLC_NOT_AVAILABLE         =   9100,
    PHY_LAYER_RF_NOT_AVAILABLE          =   9101,
    PAL_PLC_TIMER_SYNC_ERROR            =   9102,

    /* PRIME Critical System Error */
    CRITICAL_ERROR_PRIME_NO_PHY         =   9995,
    CRITICAL_ERROR_PRIME_NO_PLC         =   9996,
    CRITICAL_ERROR_PL360_FAILURE        =   9997,
    CRITICAL_ERROR_RF215_FAILURE        =   9998,
    CRITICAL_ERROR                      =   9999


} SRV_LOG_REPORT_CODE;

<#if (ENABLE_TRACES == true)>
// *****************************************************************************
// *****************************************************************************
// Section: API Functions
// *****************************************************************************
// *****************************************************************************

//******************************************************************************
/* Function:
    void SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_LEVEL logLevel,
                                          SRV_LOG_REPORT_CODE code,
                                          const char *info, ...)
  Summary:
    Reports an error/warning code with related information.

  Description:
    This function reports an error/warning code with related information.

  Precondition:
    The SYS_DEBUG initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    logLevel       - Log priority level
    code           - Code of the reported error/warning
    info           - Formatted description of the reported error/warning

  Returns:
    None.

  Example:
    <code>
    SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_WARNING, 100,
                                     "Wrong input\r\n");
    </code>

  Remarks:
    The function does not add a newline after printing the information.
*/

void SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_LEVEL logLevel,
                                      SRV_LOG_REPORT_CODE code,
                                      const char *info, ...);

//******************************************************************************
/* Function:
    void SRV_LOG_REPORT_Message(SRV_LOG_REPORT_LEVEL logLevel,
                                const char *info, ...)
  Summary:
    Reports log information.

  Description:
    This function reports log information.

  Precondition:
    The SYS_DEBUG initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    logLevel       - Log priority level
    info           - Formatted description of the information

  Returns:
    None.

  Example:
    <code>
    SRV_LOG_REPORT_Message(SRV_LOG_REPORT_DEBUG, "No address\r\n");
    </code>

  Remarks:
    The function does not add a newline after printing the information.
*/

void SRV_LOG_REPORT_Message(SRV_LOG_REPORT_LEVEL logLevel,
                            const char *info, ...);

//******************************************************************************
/* Function:
    void SRV_LOG_REPORT_Buffer(SRV_LOG_REPORT_LEVEL logLevel,
                               const uint8_t *buffer, uint32_t bufferLength,
                               const char *info, ...)

  Summary:
    Reports log information with a buffer.

  Description:
    This function reports log information with a buffer.

  Precondition:
    The SYS_DEBUG initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    logLevel          - Log priority level
    buffer            - Buffer to be reported
    bufferLength      - Length of the buffer
    info              - Formatted description of the information

  Returns:
    None.

  Example:
    <code>
    uint8_t macAddress[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0x00, 0x01};
    uint8_t maxNumberNodes = 100;

    SRV_LOG_REPORT_Buffer(SRV_LOG_REPORT_INFO, macAddress, 6, "MAC Address: ");
    </code>

  Remarks:
    The function automatically adds a newline after printing the buffer.
*/

void SRV_LOG_REPORT_Buffer(SRV_LOG_REPORT_LEVEL logLevel,
                           const uint8_t *buffer, uint32_t bufferLength,
                           const char *info, ...);

<#else>
#define SRV_LOG_REPORT_Message_With_Code(logLevel, code, info, ...)
#define SRV_LOG_REPORT_Message(logLevel, info, ...)
#define SRV_LOG_REPORT_Buffer(logLevel, buffer, bufferLength, info, ...)

</#if>
#ifdef __cplusplus // Provide C++ Compatibility
 }
#endif

#endif /* SRV_LOG_REPORT_H */
