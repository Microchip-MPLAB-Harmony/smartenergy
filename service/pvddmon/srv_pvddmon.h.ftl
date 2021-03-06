/*******************************************************************************
  Interface definition of TRNG PLIB.

  Company:
    Microchip Technology Inc.

  File Name:
    srv_pvddmon.h

  Summary:
    Interface definition of the True Random Number Generator Plib (TRNG).

  Description:
    This file defines the interface for the TRNG Plib.
    It allows user to generate true random numbers.
*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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

#ifndef SRV_PVDDMON_H    // Guards against multiple inclusion
#define SRV_PVDDMON_H

<#compress> <#-- To remove unwanted new lines -->
<#assign PVDD_MON_ADC_INSTANCE = "">
<#assign PVDD_MON_MASK_PREFIX = "">

<#if (afec0.AFEC_INSTANCE_NAME)?has_content>
    <#assign PVDD_MON_ADC_INSTANCE = "AFEC0">
    <#assign PVDD_MON_MASK_PREFIX = "AFEC">
<#elseif (afec1.AFEC_INSTANCE_NAME)?has_content>
    <#assign PVDD_MON_ADC_INSTANCE = "AFEC1">
    <#assign PVDD_MON_MASK_PREFIX = "AFEC">
<#elseif (adc.ADC_INSTANCE_NAME)?has_content>
    <#assign PVDD_MON_ADC_INSTANCE = "ADC">
    <#assign PVDD_MON_MASK_PREFIX = "ADC">
</#if>

</#compress>
#include <stdint.h>
#include "device.h"
<#if core.CoreSysIntFile == true>
#include "interrupts.h"
</#if>
#include "peripheral/${PVDD_MON_MASK_PREFIX?lower_case}/plib_${PVDD_MON_MASK_PREFIX?lower_case}_common.h"


#ifdef __cplusplus // Provide C++ Compatibility
 extern "C" {
#endif

/* List of PVDD Monitor comparison modes

 Summary:
    Defines two comparison modes.

 Description:
    This will be used to set the comparison mode. 

*/
typedef enum 
{
    /* Notify when PVDD level enters the comparison window */
    SRV_PVDDMON_CMP_MODE_IN,

    /* Notify when PVDD level leaves the comparison window */
    SRV_PVDDMON_CMP_MODE_OUT,

} SRV_PVDDMON_CMP_MODE;

// *****************************************************************************
/* PVDD Monitor Event Handler Function Pointer

   Summary
    Pointer to a PVDD Monitor Event handler function

   Description
    This data type defines the required function signature for the PVDD Monitor Event
    handling callback function. A client must register a pointer using the callback 
    register function whose function signature match the types specified by this 
    function pointer in order to receive a notification related the comparison event.

  Parameters:
    cmpMode - Comparison mode.

    context - Pointer to parameters to be passed to Handler function.                       

  Returns:
    None.

  Example:
    <code>
    static void APP_PLC_PVDDMonitorCb( SRV_PVDDMON_CMP_MODE cmpMode, uintptr_t context )
    {
        (void)context;
        
        if (cmpMode == SRV_PVDDMON_CMP_MODE_OUT)
        {
            // PLC Transmission is not permitted
            DRV_PLC_PHY_EnableTX(appPlc.drvPl360Handle, false);
            appPlc.pvddMonTxEnable = false;
            // Restart PVDD Monitor to check when VDD is within the comparison window
            SRV_PVDDMON_Restart(SRV_PVDDMON_CMP_MODE_IN);
        }
        else
        {
            // PLC Transmission is permitted again
            DRV_PLC_PHY_EnableTX(appPlc.drvPl360Handle, true);
            appPlc.pvddMonTxEnable = true;
            // Restart PVDD Monitor to check when VDD is out of the comparison window
            SRV_PVDDMON_Restart(SRV_PVDDMON_CMP_MODE_OUT);
        }
    }
    </code>

  Remarks:
    - See SRV_PVDDMON_CMP_MODE for more details.

*/
typedef void (*SRV_PVDDMON_CALLBACK)( SRV_PVDDMON_CMP_MODE cmpMode, uintptr_t context );

/* High and Low threshold ADC values */
#define SRV_PVDDMON_HIGH_TRESHOLD              ${SRV_PVDDMON_HIGH_THRESHOLD_HEX}
#define SRV_PVDDMON_LOW_TRESHOLD               ${SRV_PVDDMON_LOW_THRESHOLD_HEX}
#define SRV_PVDDMON_HIGH_TRESHOLD_HYST         ${SRV_PVDDMON_HIGH_THRESHOLD_HEX_HYST}
#define SRV_PVDDMON_LOW_TRESHOLD_HYST          ${SRV_PVDDMON_LOW_THRESHOLD_HEX_HYST}

/************************ SRV PLC PVDD MONITOR API *****************************/
// *****************************************************************************
/* Function:
    void SRV_PVDDMON_Initialize ( void )

  Summary:
    Initializes the PVDD Monitor service.

  Description:
    This routine initializes the PVDD Monitor service, making it ready for 
    clients to start and use it.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    // Init PVDD Monitor service
    SRV_PVDDMON_Initialize();
    </code>

  Remarks:
    None
*/
void SRV_PVDDMON_Initialize (void);

// *****************************************************************************
/* Function:
    void SRV_PVDDMON_Start
    (
        SRV_PVDDMON_CMP_MODE cmpMode
    )

  Summary:
    Start the PVDD Monitor service.

  Description:
    This routine starts the PVDD Monitor service, configuring the ADC plib and 
    setting the comparison mode.

  Precondition:
    Function SRV_PVDDMON_Initialize must have been called before calling this function.
    PVDD Monitor event handler must have been set before calling this function to 
    be able to receive the PVDD Monitor events.

  Parameters:
    cmpMode - .

  Returns:
    None.

  Example:
    <code>
    // Enable PLC PVDD Monitor Service 
    SRV_PVDDMON_CallbackRegister(APP_PLC_PVDDMonitorCb, 0);
    // Notify when PVDD level leaves the comparison window 
    SRV_PVDDMON_Start(SRV_PVDDMON_CMP_MODE_OUT);
    </code>

  Remarks:
    None
*/
void SRV_PVDDMON_Start (SRV_PVDDMON_CMP_MODE cmpMode);

// *****************************************************************************
/* Function:
    void SRV_PVDDMON_Restart
    (
        SRV_PVDDMON_CMP_MODE cmpMode
    )

  Summary:
    Restart the PVDD Monitor service.

  Description:
    This routine restarts the PVDD Monitor service, and allows clients to modify 
    the comparison mode in runtime. This routine is commonly used in the PVDD Monitor 
    event handler to switch the comparison mode once ADC has been previously configured
    and is running.

  Precondition:
    Function SRV_PVDDMON_Start must have been called before calling this function.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    static void APP_PLC_PVDDMonitorCb( SRV_PVDDMON_CMP_MODE cmpMode, uintptr_t context )
    {
        (void)context;
        
        if (cmpMode == SRV_PVDDMON_CMP_MODE_OUT)
        {
            // PLC Transmission is not permitted 
            DRV_PLC_PHY_EnableTX(appPlc.drvPl360Handle, false);
            appPlc.pvddMonTxEnable = false;
            // Restart PVDD Monitor to check when VDD is within the comparison window 
            SRV_PVDDMON_Restart(SRV_PVDDMON_CMP_MODE_IN);
        }
        else
        {
            // PLC Transmission is permitted again 
            DRV_PLC_PHY_EnableTX(appPlc.drvPl360Handle, true);
            appPlc.pvddMonTxEnable = true;
            // Restart PVDD Monitor to check when VDD is out of the comparison window 
            SRV_PVDDMON_Restart(SRV_PVDDMON_CMP_MODE_OUT);
        }
    }
    </code>

  Remarks:
    None
*/
void SRV_PVDDMON_Restart (SRV_PVDDMON_CMP_MODE cmpMode);

// *****************************************************************************
/* Function:
    void SRV_PVDDMON_CallbackRegister
    (
        SRV_PVDDMON_CALLBACK callback,
        uintptr_t context
    )

  Summary:
    Allows a client to identify a PVDD Monitor event handling function to call 
    back when the PVDD level is in/out comparison window.

  Description:
    The callback once set, persists until the client closes the sets another 
    callback (which could be a "NULL" pointer to indicate no callback).

  Precondition:
    None.

  Parameters:
    callback - Pointer to the callback function.

    context - The value of parameter will be passed back to the client unchanged, 
    when the callback function is called.

  Returns:
    None.

  Example:
    <code>
    // Enable PLC PVDD Monitor Service 
    SRV_PVDDMON_CallbackRegister(APP_PLC_PVDDMonitorCb, 0);
    // Notify when PVDD level leaves the comparison window
    SRV_PVDDMON_Start(SRV_PVDDMON_CMP_MODE_OUT);
    </code>

  Remarks:
    None
*/
void SRV_PVDDMON_CallbackRegister (SRV_PVDDMON_CALLBACK callback, uintptr_t context);

#ifdef __cplusplus // Provide C++ Compatibility
 }
#endif

#endif 
