# SRV_USI_Open Function

## C

```c
SRV_USI_HANDLE SRV_USI_Open( const SYS_MODULE_INDEX index )
```

## Summary

Opens the specified USI service instance and returns a handle to it.

## Description

This routine opens the specified USI service instance and provides a handle that must be provided to all other client-level operations to identify the caller and the instance of the service.

## Precondition

Function [*SRV_USI_Initialize*](GUID-621D854B-54BF-4473-91C6-B37A25840333.html) must have been called before calling this function.

## Parameters

| Param | Description |
|:----- |:----------- |
| index | Index for the instance to be opened |

## Returns

If successful, the routine returns a valid open-instance handle ([*SRV_USI_HANDLE Typedef*](GUID-9C3076C9-0171-469B-B059-048A45B45047.html)).

If an error occurs, the return value is *[*SRV_USI_HANDLE_INVALID*](GUID-ABD93EB0-3099-42B8-9559-30F072F02739.html)*. Error can occur

- if the specified instance has been already opened.
- if the instance being opened is not initialized or is invalid.
- if there is an error opening the serial interface (USB CDC instances only)

## Example

```c
SRV_USI_HANDLE handle;

handle = SRV_USI_Open(SRV_USI_INDEX_0);
if (handle == SRV_USI_HANDLE_INVALID)
{
    // Unable to open USI instance. Maybe it is not initialized
}
```

## Remarks

The handle returned is valid until [*SRV_USI_Close*](GUID-B3A181FF-1B0E-4CF1-8EBB-0D9000DB82EF.html) is called.
