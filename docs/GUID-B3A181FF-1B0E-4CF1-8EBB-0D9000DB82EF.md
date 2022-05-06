# SRV_USI_Close Function

## C

```c
void SRV_USI_Close( const SRV_USI_HANDLE handle )
```

## Summary

Closes an opened-instance of the USI service.

## Description

This routine closes an opened-instance of the USI service, invalidating the handle.

A new handle must be obtained by calling [*SRV_USI_Open*](GUID-7464B9B1-E30E-4C83-BFF4-C8FBC8AE2B97.html) before the caller uses the service again.

## Precondition

[*SRV_USI_Open*](GUID-7464B9B1-E30E-4C83-BFF4-C8FBC8AE2B97.html) must have been called to obtain a valid opened service handle.

## Parameters

| Param | Description |
|:----- |:----------- |
| handle | A valid open-instance handle, returned from [*SRV_USI_Open*](GUID-7464B9B1-E30E-4C83-BFF4-C8FBC8AE2B97.html) |

## Returns

None.

## Example

```c
// 'handle', returned from SRV_USI_Open previously called

SRV_USI_Close(handle);
```

## Remarks

None.
