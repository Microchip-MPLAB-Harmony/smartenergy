/*******************************************************************************
 System Tasks File

  File Name:
    tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled tasks.

  Description:
    This file contains source code necessary to maintain system's polled tasks.
    It implements the "SYS_Tasks" function that calls the individual "Tasks"
    functions for all polled MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"
#include "sys_tasks.h"


// *****************************************************************************
// *****************************************************************************
// Section: RTOS "Tasks" Routine
// *****************************************************************************
// *****************************************************************************
TaskHandle_t xSYS_CMD_Tasks;
void lSYS_CMD_Tasks(  void *pvParameters  )
{
    while(1)
    {
        SYS_CMD_Tasks();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


static void lDRV_MEMORY_0_Tasks(  void *pvParameters  )
{
    while(true)
    {
        DRV_MEMORY_Tasks(sysObj.drvMemory0);
        vTaskDelay(DRV_MEMORY_RTOS_DELAY_IDX0 / portTICK_PERIOD_MS);
    }
}


static void lSYS_FS_Tasks(  void *pvParameters  )
{
    while(true)
    {
        SYS_FS_Tasks();
        vTaskDelay(100U / portTICK_PERIOD_MS);
    }
}


void _SLCDC_Tasks(  void *pvParameters  )
{
    while(1)
    {
        DRV_SLCDC_Update();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

static void lDRV_METROLOGY_Tasks(  void *pvParameters  )
{
    while(true)
    {
        DRV_METROLOGY_Tasks(sysObj.drvMet);
    }
}

/* Handle for the APP_METROLOGY_Tasks. */
TaskHandle_t xAPP_METROLOGY_Tasks;

static void lAPP_METROLOGY_Tasks(  void *pvParameters  )
{
    while(true)
    {
        APP_METROLOGY_Tasks();
    }
}
/* Handle for the APP_CONSOLE_Tasks. */
TaskHandle_t xAPP_CONSOLE_Tasks;

static void lAPP_CONSOLE_Tasks(  void *pvParameters  )
{
    while(true)
    {
        APP_CONSOLE_Tasks();
    }
}
/* Handle for the APP_DATALOG_Tasks. */
TaskHandle_t xAPP_DATALOG_Tasks;

static void lAPP_DATALOG_Tasks(  void *pvParameters  )
{
    while(true)
    {
        APP_DATALOG_Tasks();
        vTaskDelay(100U / portTICK_PERIOD_MS);
    }
}
/* Handle for the APP_DISPLAY_Tasks. */
TaskHandle_t xAPP_DISPLAY_Tasks;

static void lAPP_DISPLAY_Tasks(  void *pvParameters  )
{
    while(true)
    {
        APP_DISPLAY_Tasks();
    }
}
/* Handle for the APP_ENERGY_Tasks. */
TaskHandle_t xAPP_ENERGY_Tasks;

static void lAPP_ENERGY_Tasks(  void *pvParameters  )
{
    while(true)
    {
        APP_ENERGY_Tasks();
    }
}
/* Handle for the APP_EVENTS_Tasks. */
TaskHandle_t xAPP_EVENTS_Tasks;

static void lAPP_EVENTS_Tasks(  void *pvParameters  )
{
    while(true)
    {
        APP_EVENTS_Tasks();
    }
}




// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/
void SYS_Tasks ( void )
{
    /* Maintain system services */

    (void) xTaskCreate( lSYS_CMD_Tasks,
        "SYS_CMD_TASKS",
        SYS_CMD_RTOS_STACK_SIZE,
        (void*)NULL,
        SYS_CMD_RTOS_TASK_PRIORITY,
        &xSYS_CMD_Tasks
    );



    (void) xTaskCreate( lSYS_FS_Tasks,
        "SYS_FS_TASKS",
        SYS_FS_STACK_SIZE,
        (void*)NULL,
        SYS_FS_PRIORITY,
        (TaskHandle_t*)NULL
    );




    /* Maintain Device Drivers */
        (void)xTaskCreate( lDRV_MEMORY_0_Tasks,
        "DRV_MEM_0_TASKS",
        DRV_MEMORY_STACK_SIZE_IDX0,
        (void*)NULL,
        DRV_MEMORY_PRIORITY_IDX0,
        (TaskHandle_t*)NULL
    );

    xTaskCreate( _SLCDC_Tasks,
        "SLCDC_Tasks",
        2048,
        (void*)NULL,
        1,
        (TaskHandle_t*)NULL
    );


    (void) xTaskCreate(lDRV_METROLOGY_Tasks,
        "DRV_METROLOGY_TASKS",
        DRV_METROLOGY_RTOS_STACK_SIZE,
        (void*)NULL,
        DRV_METROLOGY_RTOS_TASK_PRIORITY,
        (TaskHandle_t*)NULL
    );



    /* Maintain Middleware & Other Libraries */


    /* Maintain the application's state machine. */
        /* Create OS Thread for APP_METROLOGY_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_METROLOGY_Tasks,
                "APP_METROLOGY_Tasks",
                1024,
                NULL,
                1,
                &xAPP_METROLOGY_Tasks);

    /* Create OS Thread for APP_CONSOLE_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_CONSOLE_Tasks,
                "APP_CONSOLE_Tasks",
                1024,
                NULL,
                1,
                &xAPP_CONSOLE_Tasks);

    /* Create OS Thread for APP_DATALOG_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_DATALOG_Tasks,
                "APP_DATALOG_Tasks",
                1024,
                NULL,
                1,
                &xAPP_DATALOG_Tasks);

    /* Create OS Thread for APP_DISPLAY_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_DISPLAY_Tasks,
                "APP_DISPLAY_Tasks",
                1024,
                NULL,
                1,
                &xAPP_DISPLAY_Tasks);

    /* Create OS Thread for APP_ENERGY_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_ENERGY_Tasks,
                "APP_ENERGY_Tasks",
                1024,
                NULL,
                1,
                &xAPP_ENERGY_Tasks);

    /* Create OS Thread for APP_EVENTS_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_EVENTS_Tasks,
                "APP_EVENTS_Tasks",
                1024,
                NULL,
                1,
                &xAPP_EVENTS_Tasks);




    /* Start RTOS Scheduler. */

     /**********************************************************************
     * Create all Threads for APP Tasks before starting FreeRTOS Scheduler *
     ***********************************************************************/
    vTaskStartScheduler(); /* This function never returns. */

}

/*******************************************************************************
 End of File
 */

