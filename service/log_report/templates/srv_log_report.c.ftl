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

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdarg.h>
#include "configuration.h"
#include "srv_log_report.h"
<#if enableReportDisplay == true>
<#if __PROCESSOR?matches("PIC32CX.*MT.*")>
<#if le_gfx_slcdc.BoardSelect?has_content && ((le_gfx_slcdc.BoardSelect == "PIC32CXMTSH_DB") || (le_gfx_slcdc.BoardSelect == "PIC32CXMTC_DB"))>
#include "gfx/driver/controller/slcdc/cl010.h"
</#if>
</#if>
</#if>

<#if sys_debug?? && sys_console??>
<#assign SERIAL_DEBUG_ENABLE = true>
<#else>
<#assign SERIAL_DEBUG_ENABLE = false>
</#if>
<#if SERIAL_DEBUG_ENABLE = true>
static char message[SYS_CONSOLE_PRINT_BUFFER_SIZE];

</#if>
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

<#if enableReportDisplay == true>
<#if __PROCESSOR?matches("PIC32CX.*MT.*")>
<#if le_gfx_slcdc.BoardSelect?has_content && ((le_gfx_slcdc.BoardSelect == "PIC32CXMTSH_DB") || (le_gfx_slcdc.BoardSelect == "PIC32CXMTC_DB"))>
static void lSRV_LOG_REPORT_show_error(enum cl010_line disp_line, SRV_LOG_REPORT_CODE code_type)
{
    uint32_t errorType;

    errorType = code_type % 100000000; /* Only eight digits in the display */
    cl010_show_numeric_string(disp_line, (const uint8_t *)&errorType);
}
</#if>
</#if>
</#if>

void SRV_LOG_REPORT_Message_With_Code(SRV_LOG_REPORT_LEVEL logLevel,
                                      SRV_LOG_REPORT_CODE code,
                                      const char *info, ...)
{
<#if SERIAL_DEBUG_ENABLE = true>
    /* Format the information */
    va_start(srvLogReportArgs, info);
    (void) vsnprintf(message, SYS_CONSOLE_PRINT_BUFFER_SIZE, info, srvLogReportArgs);
    va_end(srvLogReportArgs);

    SYS_DEBUG_MESSAGE((SYS_ERROR_LEVEL)logLevel, message);
<#else>
    (void)logLevel;
    (void)info;
</#if>

<#if enableReportDisplay == true>
<#if __PROCESSOR?matches("PIC32CX.*MT.*")>
<#if le_gfx_slcdc.BoardSelect?has_content && ((le_gfx_slcdc.BoardSelect == "PIC32CXMTSH_DB") || (le_gfx_slcdc.BoardSelect == "PIC32CXMTC_DB"))>
    lSRV_LOG_REPORT_show_error(CL010_LINE_UP,code);
</#if>
</#if>
</#if>

}

void SRV_LOG_REPORT_Message(SRV_LOG_REPORT_LEVEL logLevel,
                            const char *info, ...)
{
<#if SERIAL_DEBUG_ENABLE = true>
    /* Format the information */
    va_start(srvLogReportArgs, info);
    (void) vsnprintf(message, SYS_CONSOLE_PRINT_BUFFER_SIZE, info, srvLogReportArgs);
    va_end(srvLogReportArgs);

    SYS_DEBUG_PRINT((SYS_ERROR_LEVEL)logLevel, message);
<#else>
    (void)logLevel;
    (void)info;
</#if>
}

void SRV_LOG_REPORT_Buffer(SRV_LOG_REPORT_LEVEL logLevel,
                           const uint8_t *buffer, uint32_t bufferLength,
                           const char *info, ...)
{
<#if SERIAL_DEBUG_ENABLE = true>
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
    if (lastBlockLength != 0U)
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

        if (blockLength != 0U)
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
<#else>
    (void)logLevel;
    (void)buffer;
    (void)bufferLength;
    (void)info;
</#if>
}

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 17.1"
#pragma coverity compliance end_block "MISRA C-2012 Rule 21.6"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRA C-2012 deviation block end */
