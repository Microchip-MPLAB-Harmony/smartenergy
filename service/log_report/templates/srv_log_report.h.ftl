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

<#if (ENABLE_TRACES == true)>
// *****************************************************************************
// *****************************************************************************
// Section: API Functions
// *****************************************************************************
// *****************************************************************************

//******************************************************************************
/* Function:
    void SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_LEVEL logLevel,
                                          uint32_t code,
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
	The following error code ranges are already reserved:
	- SRV_USI: 100 - 124
	- SRV_QUEUE: 125 - 149
	- PRIME PAL: 200 - 219
	- PRIME Stack: 220 - 9999
*/

void SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_LEVEL logLevel,
                                      uint32_t code,
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
