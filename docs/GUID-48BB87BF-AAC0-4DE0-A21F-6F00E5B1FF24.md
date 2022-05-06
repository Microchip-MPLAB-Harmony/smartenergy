# SRV_USI_CALLBACK Typedef

## C

```c
typedef void ( * SRV_USI_CALLBACK ) ( uint8_t *pData, size_t length );
```

## Summary

Pointer to an USI service callback function.

## Description

This data type defines a pointer to an USI service callback function, thus defining the function signature. Clients of the USI service can register callback functions for a specific USI protocol with [*SRV_USI_CallbackRegister*](GUID-C0B40EB5-8C77-4961-8932-C0C247B4FA29.html) function. That function will be called back when a new message is received and it belongs to the specified USI protocol.

## Parameters

| Param | Description |
|:----- |:----------- |
| pData | Pointer to data payload of the received USI message |
| length | Length in bytes of data payload of the received USI message |

## Returns

None.  

## Example

```c
void APP_USIPhyProtocolEventHandler(uint8_t *pData, size_t length)
{
    /* Message received from external tool */
}

// 'handle', returned from SRV_USI_Open previously called
SRV_USI_CallbackRegister(handle, SRV_USI_PROT_ID_PHY, APP_USIPhyProtocolEventHandler);
```

## Remarks

None.
