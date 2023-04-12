/*******************************************************************************
  Header for the queue management module 

  Company:
    Microchip Technology Inc.

  File Name:
    srv_queue.h

  Summary:
    Interface definition of the queue management module.

  Description:
    This file defines the interface for the queue management module.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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

#ifndef SRV_QUEUE_H
#define SRV_QUEUE_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility
 extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Queue types

 Summary:
    List of possible queue types.

 Description:
    This type defines the possible queue types.

 Remarks:
    None.
*/

typedef enum 
{
    SRV_QUEUE_TYPE_SINGLE     = 0,
    SRV_QUEUE_TYPE_PRIORITY   = 1,
} SRV_QUEUE_TYPE;

// *****************************************************************************
/* Queue access modes

 Summary:
    List of possible queue access modes.

 Description:
    This type defines the possible queue access modes.

 Remarks:
    None.
*/

typedef enum 
{
    SRV_QUEUE_MODE_REMOVE   = 0,
    SRV_QUEUE_MODE_READ     = 1,
} SRV_QUEUE_MODE;

// *****************************************************************************
/* Queue positions

 Summary:
    List of possible queue positions.

 Description:
    This type defines the possible queue positions.

 Remarks:
    None.
*/

typedef enum 
{
    SRV_QUEUE_POSITION_HEAD   = 0,
    SRV_QUEUE_POSITION_TAIL   = 1,
} SRV_QUEUE_POSITION;

// *****************************************************************************
/* Queue element

  Summary:
    Queue element.

  Description:
    This structure contains the data stored in each queue element.

  Remarks:
    None.
*/
typedef struct _queue_element_tag
{
    /* Pointer to the previous object of the queue */
    struct _queue_element_tag *prev;                
    
    /* Pointer to the next object of the queue */
    struct _queue_element_tag *next;                

    /* Element priority (only with priority queues) */  
    uint32_t priority;                              
} SRV_QUEUE_ELEMENT;

// *****************************************************************************
/* Queue data

  Summary:
    Queue data.

  Description:
    This structure contains the common data stored in a queue.

  Remarks:
    None.
*/
typedef struct  
{
    /* Pointer to the head of the queue */
    SRV_QUEUE_ELEMENT *head;                           
    
    /* Pointer to the tail of the queue */
    SRV_QUEUE_ELEMENT *tail;                           
    
    /* Queue capacity */
    uint16_t capacity;                               
    
    /* Queue size */
    uint16_t size;                                   
    
    /* Queue mode (single or priority queue) */
    SRV_QUEUE_TYPE type;                                    
} SRV_QUEUE;

// *****************************************************************************
// *****************************************************************************
// Section: Service Interface Functions
// *****************************************************************************
// *****************************************************************************

/***************************************************************************
  Function:
    void SRV_QUEUE_Init(SRV_QUEUE *queue, uint16_t capacity, SRV_QUEUE_TYPE type)
    
  Summary:
    Initializes a queue.
    
  Description:
    This function initializes a queue.

  Precondition:
    None.

  Parameters:
    queue          - Queue to be initialized.
    capacity       - Maximum number of elements in the queue.
    type           - Queue type (single or priority queue).

  Returns:
    None.

  Example:
    <code>
    #define NUM_MAX_NODES    750
    static SRV_QUEUE nodeQueue;
   
    SRV_QUEUE_Init(&nodeQueue, NUM_MAX_NODES, SRV_QUEUE_TYPE_SINGLE);
    </code>

  Remarks:
    This function must be called before invoking any other functionality of 
    the queue management module.
*/
void SRV_QUEUE_Init(SRV_QUEUE *queue, uint16_t capacity, SRV_QUEUE_TYPE type);

/***************************************************************************
  Function:
    void SRV_QUEUE_Append(SRV_QUEUE *queue, SRV_QUEUE_ELEMENT *element)
    
  Summary:
    Appends an element into a queue.
    
  Description:
    This function appends an element into a queue.

  Precondition:
    The queue must have been initialized previously with 
    function SRV_QUEUE_Init.

  Parameters:
    queue          - Queue where to append the element.
    element        - Pointer to the element to be appended.

  Returns:
    None.

  Example:
    <code>
    #define NUM_MAX_NODES    750
    typedef struct node_info
    {
        struct node_info *prev;
        struct node_info *next;
        uint8_t macAddress[8]
    } node_info_t;
    static SRV_QUEUE nodeQueue;
    node_info_t nodeInfo;

    SRV_QUEUE_Init(&nodeQueue, NUM_MAX_NODES, SRV_QUEUE_TYPE_SINGLE);
    memset(nodeinfo.macAddress, 0xFF, 8);
    SRV_QUEUE_Append(&nodeQueue, (SRV_QUEUE_ELEMENT *)nodeInfo);
    </code>

  Remarks:
    None.
*/
void SRV_QUEUE_Append(SRV_QUEUE *queue, SRV_QUEUE_ELEMENT *element);

/***************************************************************************
  Function:
    void SRV_QUEUE_Append_With_Priority(SRV_QUEUE *queue, uint32_t priority, 
                                        SRV_QUEUE_ELEMENT *element)
    
  Summary:
    Appends an element into a priority queue.
    
  Description:
    This function appends an element into a priority queue.

  Precondition:
    The queue must have been initialized previously with 
    function SRV_QUEUE_Init.

  Parameters:
    queue          - Queue where to append the element.
    priority       - Priority to be appended.
    element        - Pointer to the element to be appended.

  Returns:
    None.

  Example:
    <code>
    #define NUM_MAX_NODES    750
    typedef struct node_info
    {
        struct node_info *prev;
        struct node_info *next;
        uinbt32_t priority;
        uint8_t macAddress[8]
    } node_info_t;
    static SRV_QUEUE nodeQueue;
    node_info_t nodeInfo;

    SRV_QUEUE_Init(&nodeQueue, NUM_MAX_NODES, SRV_QUEUE_TYPE_PRIORITY);
    memset(nodeinfo.macAddress, 0xFF, 8);
    SRV_QUEUE_Append_With_Priority(&nodeQueue, 3, (SRV_QUEUE_ELEMENT *)nodeInfo);
    </code>

  Remarks:
    None.
*/
void SRV_QUEUE_Append_With_Priority(SRV_QUEUE *queue, uint32_t priority, 
                                    SRV_QUEUE_ELEMENT *element);

/***************************************************************************
  Function:
    void SRV_QUEUE_Insert_Before(SRV_QUEUE *queue, 
                                 SRV_QUEUE_ELEMENT *currentElement, 
                                 SRV_QUEUE_ELEMENT *element)
    
  Summary:
    Inserts an element into a queue before a given element.
    
  Description:
    This function inserts an element into a queue before a given element.

  Precondition:
    The queue must have been initialized previously with 
    function SRV_QUEUE_Init.

  Parameters:
    queue          - Queue where to insert the element.
    currentElement - Pointer to an element in the queue.
    element        - Pointer to the element to be inserted.

  Returns:
    None.

  Example:
    <code>
    #define NUM_MAX_NODES    750
    typedef struct node_info
    {
        struct node_info *prev;
        struct node_info *next;
        uint8_t macAddress[8]
    } node_info_t;
    static SRV_QUEUE nodeQueue;
    node_info_t nodeInfo1;
    node_info_t nodeInfo2;

    SRV_QUEUE_Init(&nodeQueue, NUM_MAX_NODES, SRV_QUEUE_TYPE_SINGLE);
    memset(nodeinfo1.macAddress, 0xFF, 8);
    SRV_QUEUE_Append(&nodeQueue, (SRV_QUEUE_ELEMENT *)nodeInfo1);
    memset(nodeinfo2.macAddress, 0xEE, 8);
    SRV_QUEUE_Insert_Before(&nodeQueue, (SRV_QUEUE_ELEMENT *)nodeInfo1, 
                            (SRV_QUEUE_ELEMENT *)nodeInfo2);
    </code>

  Remarks:
    None.
*/
void SRV_QUEUE_Insert_Before(SRV_QUEUE *queue, 
                             SRV_QUEUE_ELEMENT *currentElement, 
                             SRV_QUEUE_ELEMENT *element);

/***************************************************************************
  Function:
    void SRV_QUEUE_Insert_After(SRV_QUEUE *queue, 
                                SRV_QUEUE_ELEMENT *currentElement, 
                                SRV_QUEUE_ELEMENT *element)
    
  Summary:
    Inserts an element into a queue after a given element.
    
  Description:
    This function inserts an element into a queue after a given element.

  Precondition:
    The queue must have been initialized previously with 
    function SRV_QUEUE_Init.

  Parameters:
    queue          - Queue where to insert the element.
    currentElement - Pointer to an element in the queue.
    element        - Pointer to the element to be inserted.

  Returns:
    None.

  Example:
    <code>
    #define NUM_MAX_NODES    750
    typedef struct node_info
    {
        struct node_info *prev;
        struct node_info *next;
        uint8_t macAddress[8]
    } node_info_t;
    static SRV_QUEUE nodeQueue;
    node_info_t nodeInfo1;
    node_info_t nodeInfo2;

    SRV_QUEUE_Init(&nodeQueue, NUM_MAX_NODES, SRV_QUEUE_TYPE_SINGLE);
    memset(nodeinfo1.macAddress, 0xFF, 8);
    SRV_QUEUE_Append(&nodeQueue, (SRV_QUEUE_ELEMENT *)nodeInfo1);
    memset(nodeinfo2.macAddress, 0xEE, 8);
    SRV_QUEUE_Insert_After(&nodeQueue, (SRV_QUEUE_ELEMENT *)nodeInfo1, 
                           (SRV_QUEUE_ELEMENT *)nodeInfo2);
    </code>

  Remarks:
    None.
*/
void SRV_QUEUE_Insert_After(SRV_QUEUE *queue, 
                            SRV_QUEUE_ELEMENT *currentElement, 
                            SRV_QUEUE_ELEMENT *element);

/***************************************************************************
  Function:
    SRV_QUEUE_ELEMENT *SRV_QUEUE_Read_Or_Remove(SRV_QUEUE *queue, 
                                                SRV_QUEUE_MODE accessMode, 
                                                SRV_QUEUE_POSITION position)
    
  Summary:
    Reads or removes an element from a queue.
    
  Description:
    This function reads or removes an element from a queue.

  Precondition:
    The queue must have been initialized previously with 
    function SRV_QUEUE_Init.

  Parameters:
    queue          - Queue from which the element must be read or removed.
    access         - Access mode (read or remove).
    position       - Position in the queue (head or tail).

  Returns:
    In case of remove, the element will be removed from queue and returned. 
    In case of read, the element will be returned without removing it from 
    the queue.
    If the queue is empty, NULL is returned.

  Example:
    <code>
    #define NUM_MAX_NODES    750
    typedef struct node_info
    {
        struct node_info *prev;
        struct node_info *next;
        uint8_t macAddress[8]
    } node_info_t;
    static SRV_QUEUE nodeQueue;
    node_info_t nodeInfo;
    node_info_t *removedNode;

    SRV_QUEUE_Init(&nodeQueue, NUM_MAX_NODES, SRV_QUEUE_TYPE_SINGLE);
    memset(nodeinfo1.macAddress, 0xFF, 8);
    SRV_QUEUE_Append(&nodeQueue, (SRV_QUEUE_ELEMENT *)nodeInfo);
    removedNode = SRV_QUEUE_Read_Or_Remove(&nodeQueue, SRV_QUEUE_MODE_REMOVE, 
                                           SRV_QUEUE_POSITION_HEAD);
    </code>

  Remarks:
    None.
*/
SRV_QUEUE_ELEMENT *SRV_QUEUE_Read_Or_Remove(SRV_QUEUE *queue, 
                                            SRV_QUEUE_MODE accessMode, 
                                            SRV_QUEUE_POSITION position);

/***************************************************************************
  Function:
    SRV_QUEUE_ELEMENT *SRV_QUEUE_Read_Element(SRV_QUEUE *queue, 
                                              uint16_t elementIndex)
    
  Summary:
    Reads an element from a queue at the given index.
    
  Description:
    This function reads an element from a queue at the given index.

  Precondition:
    The queue must have been initialized previously with 
    function SRV_QUEUE_Init.

  Parameters:
    queue          - Queue from which the element must be read.
    elementIndex   - Position of the element in the queue.

  Returns:
    Pointer to the queue element.
    If the queue is empty, NULL is returned.

  Example:
    <code>
    #define NUM_MAX_NODES    750
    typedef struct node_info
    {
        struct node_info *prev;
        struct node_info *next;
        uint8_t macAddress[8]
    } node_info_t;
    static SRV_QUEUE nodeQueue;
    node_info_t nodeInfo1;
    node_info_t nodeInfo2;
    node_info_t *nodeInfo3;

    SRV_QUEUE_Init(&nodeQueue, NUM_MAX_NODES, SRV_QUEUE_TYPE_SINGLE);
    memset(nodeinfo1.macAddress, 0xFF, 8);
    SRV_QUEUE_Append(&nodeQueue, (SRV_QUEUE_ELEMENT *)nodeInfo1);
    memset(nodeinfo2.macAddress, 0xEE, 8);
    SRV_QUEUE_Insert_After(&nodeQueue, (SRV_QUEUE_ELEMENT *)nodeInfo1, 
                           (SRV_QUEUE_ELEMENT *)nodeInfo2);
    nodeInfo3 = SRV_QUEUE_Read_Element(&nodeQueue, 2);
    </code>

  Remarks:
    None.
*/
SRV_QUEUE_ELEMENT *SRV_QUEUE_Read_Element(SRV_QUEUE *queue, 
                                          uint16_t elementIndex);

/***************************************************************************
  Function:
    void SRV_QUEUE_Remove_Element(SRV_QUEUE *queue, SRV_QUEUE_ELEMENT *element)
    
  Summary:
    Removes the given element from a queue.
    
  Description:
    This function removes a given element from a queue.

  Precondition:
    The queue must have been initialized previously with 
    function SRV_QUEUE_Init.

  Parameters:
    queue          - Queue from which the element must be removed.
    element        - Element to be removed.

  Returns:
    None.

  Example:
    <code>
    #define NUM_MAX_NODES    750
    typedef struct node_info
    {
        struct node_info *prev;
        struct node_info *next;
        uint8_t macAddress[8]
    } node_info_t;
    static SRV_QUEUE nodeQueue;
    node_info_t nodeInfo;

    SRV_QUEUE_Init(&nodeQueue, NUM_MAX_NODES, SRV_QUEUE_TYPE_SINGLE);
    memset(nodeinfo.macAddress, 0xFF, 8);
    SRV_QUEUE_Append(&nodeQueue, (SRV_QUEUE_ELEMENT *)nodeInfo);
    SRV_QUEUE_Remove_Element(&nodeQueue, (SRV_QUEUE_ELEMENT *)nodeInfo);
    </code>

  Remarks:
    None.
*/
void SRV_QUEUE_Remove_Element(SRV_QUEUE *queue, SRV_QUEUE_ELEMENT *element);

/***************************************************************************
  Function:
    void SRV_QUEUE_Flush(SRV_QUEUE *queue)
    
  Summary:
    Flushes the given queue.
    
  Description:
    This function flushes the given queue.

  Precondition:
    The queue must have been initialized previously with 
    function SRV_QUEUE_Init.

  Parameters:
    queue          - Queue to be flushed.

  Returns:
    None.

  Example:
    <code>
    #define NUM_MAX_NODES    750
    typedef struct node_info
    {
        struct node_info *prev;
        struct node_info *next;
        uint8_t macAddress[8]
    } node_info_t;
    static SRV_QUEUE nodeQueue;
    node_info_t nodeInfo;

    SRV_QUEUE_Init(&nodeQueue, NUM_MAX_NODES, SRV_QUEUE_TYPE_SINGLE);
    memset(nodeinfo.macAddress, 0xFF, 8);
    SRV_QUEUE_Append(&nodeQueue, (SRV_QUEUE_ELEMENT *)nodeInfo);
    SRV_QUEUE_Flush(&nodeQueue);

  Remarks:
    None.
*/
void SRV_QUEUE_Flush(SRV_QUEUE *queue);

/***************************************************************************
  Function:
    void SRV_QUEUE_Set_Capacity(SRV_QUEUE *queue, uint16_t capacity)
    
  Summary:
    Modifies the total capacity of a queue.
    
  Description:
    This function modifies the total capacity of a queue.

  Precondition:
    The queue must have been initialized previously with 
    function SRV_QUEUE_Init.

  Parameters:
    queue          - Queue.
    capacity       - New capacity of the queue.

  Returns:
    None.

  Example:
    <code>
    #define NUM_MAX_NODES    750
    typedef struct node_info
    {
        struct node_info *prev;
        struct node_info *next;
        uint8_t macAddress[8]
    } node_info_t;
    static SRV_QUEUE nodeQueue;
    node_info_t nodeInfo;

    SRV_QUEUE_Init(&nodeQueue, NUM_MAX_NODES, SRV_QUEUE_TYPE_SINGLE);
    memset(nodeinfo.macAddress, 0xFF, 8);
    SRV_QUEUE_Append(&nodeQueue, (SRV_QUEUE_ELEMENT *)nodeInfo);
    SRV_QUEUE_Set_Capacity(&nodeQueue, (NUM_MAX_NODES + 10));

  Remarks:
    None.
*/
void SRV_QUEUE_Set_Capacity(SRV_QUEUE *queue, uint16_t capacity);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility
 }
#endif
// DOM-IGNORE-END

#endif /* SRV_QUEUE_H */
