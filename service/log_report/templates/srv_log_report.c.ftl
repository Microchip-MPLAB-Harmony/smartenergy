/*******************************************************************************
  Source for the log report service

  Company:
    Microchip Technology Inc.

  File Name:
    srv_log_report.c

  Summary:
    Interface implementation for the log report service.

  Description:
    This file implements the interface for the debug report service.
    Debug messages and log information is printed on the console.
    If a display is available, debug code errors will be shown. 
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdarg.h>
#include "configuration.h"
#include "srv_log_report.h"

static char message[SYS_CONSOLE_PRINT_BUFFER_SIZE];

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 17.1 deviated 6 times. Deviation record ID - H3_MISRAC_2012_R_17_1_DR_1 */
/* MISRA C-2012 Rule 21.6 deviated 4 times. Deviation record ID - H3_MISRAC_2012_R_21_1_DR_6 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block deviate "MISRA C-2012 Rule 17.1" "H3_MISRAC_2012_R_17_1_DR_1"
#pragma coverity compliance block deviate "MISRA C-2012 Rule 21.6" "H3_MISRAC_2012_R_21_1_DR_6"
</#if>

static va_list srvLogReportArgs;

void SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_LEVEL logLevel,
                                      SRV_LOG_REPORT_CODE code,
                                      const char *info, ...)
{
    /* Format the information */
    va_start(srvLogReportArgs, info);
    (void) vsnprintf(message, SYS_CONSOLE_PRINT_BUFFER_SIZE, info, srvLogReportArgs);
    va_end(srvLogReportArgs);

    SYS_DEBUG_MESSAGE((SYS_ERROR_LEVEL)logLevel, message);

<#if enableReportDisplay == true>
//  #if (BOARD == PIC32CXMTSH_DB) || (BOARD == PIC32CXMTC_DB)
//  uint32_t errorType;
//  errorType = code % 100000000; /* Only eight digits in the display */
//  cl010_show_numeric_string(CL010_LINE_UP, (const uint8_t *)&errorType);
//  #endif
</#if>
}

void SRV_LOG_REPORT_Message(SRV_LOG_REPORT_LEVEL logLevel,
                            const char *info, ...)
{
    /* Format the information */
    va_start(srvLogReportArgs, info);
    (void) vsnprintf(message, SYS_CONSOLE_PRINT_BUFFER_SIZE, info, srvLogReportArgs);
    va_end(srvLogReportArgs);
    
    SYS_DEBUG_PRINT((SYS_ERROR_LEVEL)logLevel, message);
}

void SRV_LOG_REPORT_Buffer(SRV_LOG_REPORT_LEVEL logLevel,
                           const uint8_t *buffer, uint32_t bufferLength,
                           const char *info, ...)
{
    uint32_t blockLength;
    uint32_t lastBlockLength;
    uint32_t lastPosition;
    uint32_t blockNumber;
    uint32_t lastBlock;
    
    /* Format the information */
    va_start(srvLogReportArgs, info);
    (void) vsnprintf(message, SYS_CONSOLE_PRINT_BUFFER_SIZE, info, srvLogReportArgs);
    va_end(srvLogReportArgs);
    
    SYS_DEBUG_PRINT((SYS_ERROR_LEVEL)logLevel, message);
    
    /* The buffer will be printed in hex, so 1 byte turns into 2 chars */
    blockLength = SYS_CONSOLE_PRINT_BUFFER_SIZE / 2U;
    blockNumber = bufferLength / blockLength;
    lastBlockLength = bufferLength % blockLength;
    if (lastBlockLength)
    {
        ++blockNumber;
    } 
    else
    {
        lastBlockLength = blockLength;
    }
    
    /* Format the buffer in hex */
    lastBlock = 0U;
    while (lastBlock < blockNumber)
    {
        if (lastBlock == (blockNumber - 1U))
        {
            blockLength = lastBlockLength;
        }
    
        if (blockLength)
        {
            lastPosition = lastBlock * (SYS_CONSOLE_PRINT_BUFFER_SIZE / 2U);
            for (uint32_t i = 0U; i < blockLength; i++)
            {
                (void) sprintf(&message[i << 1], "%02x", buffer[lastPosition + i]);
            }

            SYS_DEBUG_PRINT((SYS_ERROR_LEVEL)logLevel, message);
        }
        
        ++lastBlock;
    }
    
    SYS_DEBUG_PRINT((SYS_ERROR_LEVEL)logLevel, "\r\n");
}

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 17.1"
#pragma coverity compliance end_block "MISRA C-2012 Rule 21.6"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRA C-2012 deviation block end */
