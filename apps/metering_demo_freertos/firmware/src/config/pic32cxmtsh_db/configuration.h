/*******************************************************************************
  System Configuration Header

  File Name:
    configuration.h

  Summary:
    Build-time configuration header for the system defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/

#include "user.h"
#include "device.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: System Configuration
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************

#define SYS_CMD_ENABLE
#define SYS_CMD_DEVICE_MAX_INSTANCES       SYS_CONSOLE_DEVICE_MAX_INSTANCES
#define SYS_CMD_PRINT_BUFFER_SIZE          512U
#define SYS_CMD_BUFFER_DMA_READY

/* Command System Service RTOS Configurations*/
#define SYS_CMD_RTOS_STACK_SIZE                512
#define SYS_CMD_RTOS_TASK_PRIORITY             1

/* TIME System Service Configuration Options */
#define SYS_TIME_INDEX_0                            (0)
#define SYS_TIME_MAX_TIMERS                         (5)
#define SYS_TIME_HW_COUNTER_WIDTH                   (32)
#define SYS_TIME_HW_COUNTER_PERIOD                  (4294967295U)
#define SYS_TIME_HW_COUNTER_HALF_PERIOD             (SYS_TIME_HW_COUNTER_PERIOD>>1)
#define SYS_TIME_CPU_CLOCK_FREQUENCY                (200000000)
#define SYS_TIME_COMPARE_UPDATE_EXECUTION_CYCLES    (232)


/* File System Service Configuration */

#define SYS_FS_MEDIA_NUMBER               (1U)
#define SYS_FS_VOLUME_NUMBER              (1U)

#define SYS_FS_AUTOMOUNT_ENABLE           false
#define SYS_FS_MAX_FILES                  (5U)
#define SYS_FS_MAX_FILE_SYSTEM_TYPE       (1U)
#define SYS_FS_MEDIA_MAX_BLOCK_SIZE       (512U)
#define SYS_FS_MEDIA_MANAGER_BUFFER_SIZE  (2048U)
#define SYS_FS_USE_LFN                    (1)
#define SYS_FS_FILE_NAME_LEN              (255U)
#define SYS_FS_CWD_STRING_LEN             (1024)

/* File System RTOS Configurations*/
#define SYS_FS_STACK_SIZE                 256
#define SYS_FS_PRIORITY                   1

#define SYS_FS_FAT_VERSION                "v0.15"
#define SYS_FS_FAT_READONLY               false
#define SYS_FS_FAT_CODE_PAGE              437
#define SYS_FS_FAT_MAX_SS                 SYS_FS_MEDIA_MAX_BLOCK_SIZE






#define SYS_CONSOLE_DEVICE_MAX_INSTANCES   			(1U)
#define SYS_CONSOLE_UART_MAX_INSTANCES 	   			(1U)
#define SYS_CONSOLE_USB_CDC_MAX_INSTANCES 	   		(0U)
#define SYS_CONSOLE_PRINT_BUFFER_SIZE        		(512U)


#define SYS_CONSOLE_INDEX_0                       0






// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************
/* Memory Driver Global Configuration Options */
#define DRV_MEMORY_INSTANCES_NUMBER          (1U)
/* SST26 Driver Instance Configuration */
#define DRV_SST26_INDEX                 (0U)
#define DRV_SST26_CLIENTS_NUMBER        (1U)
#define DRV_SST26_START_ADDRESS         (0x0U)
#define DRV_SST26_PAGE_SIZE             (256U)
#define DRV_SST26_ERASE_BUFFER_SIZE     (4096U)


/* Memory Driver Instance 0 Configuration */
#define DRV_MEMORY_INDEX_0                   0
#define DRV_MEMORY_CLIENTS_NUMBER_IDX0       1
#define DRV_MEMORY_BUF_Q_SIZE_IDX0    1
/* Memory Driver Instance 0 RTOS Configurations*/
#define DRV_MEMORY_STACK_SIZE_IDX0               1024
#define DRV_MEMORY_PRIORITY_IDX0                 1
#define DRV_MEMORY_RTOS_DELAY_IDX0               10U

/* Metrology Configuration Options */
#define DRV_METROLOGY_REG_BASE_ADDRESS        0x20088000UL
/* Metrology Default Config: Meter Constant */
#define DRV_METROLOGY_CONF_PKT                0x500000UL
/* Metrology Default Config: Meter Type */
#define DRV_METROLOGY_CONF_MT                 0xcccUL
/* Metrology Default Config: Current conversion factor */
#define DRV_METROLOGY_CONF_KI                 0x9a523UL
/* Metrology Default Config: Voltage conversion factor */
#define DRV_METROLOGY_CONF_KV                 0x19cc00UL
/* Metrology Default Config: ATSENSE CTRL 20 23 */
#define DRV_METROLOGY_CONF_ATS2023            0x1010103UL
/* Metrology Default Config: ATSENSE CTRL 24 27 */
#define DRV_METROLOGY_CONF_ATS2427            0x7000001UL
/* Metrology Default Config: SWELL */
#define DRV_METROLOGY_CONF_SWELL              0x5eab918UL
/* Metrology Default Config: SAG */
#define DRV_METROLOGY_CONF_SAG                0x1a2ec26UL
/* Metrology Default Config: CREEP P */
#define DRV_METROLOGY_CONF_CREEP_P            0x2e9aUL
/* Metrology Default Config: CREEP Q */
#define DRV_METROLOGY_CONF_CREEP_Q            0x2e9aUL
/* Metrology Default Config: CREEP I */
#define DRV_METROLOGY_CONF_CREEP_I            0x212dUL
/* Metrology Default Config: FEATURE_CTRL0 */
#define DRV_METROLOGY_CONF_FCTRL0             0x300UL
/* Metrology Default Config: FEATURE_CTRL1 */
#define DRV_METROLOGY_CONF_FCTRL1             0x0UL
/* Metrology Default Config: PULSE0_CTRL */
#define DRV_METROLOGY_CONF_PULSE0_CTRL        0x81009100UL
/* Metrology Default Config: PULSE1_CTRL */
#define DRV_METROLOGY_CONF_PULSE1_CTRL        0x81029100UL
/* Metrology Default Config: PULSE2_CTRL */
#define DRV_METROLOGY_CONF_PULSE2_CTRL        0x0UL
/* Metrology Default Config: Waveform Capture */
#define DRV_METROLOGY_CONF_WAVEFORM           0xf00UL
/* Metrology Default Config: Capture Buffer Size */
#define DRV_METROLOGY_CAPTURE_BUF_SIZE        32000UL

/* Metrology Driver RTOS Configurations */
#define DRV_METROLOGY_RTOS_STACK_SIZE         256U
#define DRV_METROLOGY_RTOS_TASK_PRIORITY      1U



// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // CONFIGURATION_H
/*******************************************************************************
 End of File
*/
