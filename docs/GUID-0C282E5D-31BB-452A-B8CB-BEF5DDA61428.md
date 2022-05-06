# How the Library Works

USI service is a wrapper that provides the interface between the Smart Energy stacks and the serial communications channel (UART or USB CDC).

For USI message transmission, [*SRV_USI_Send_Message*](GUID-0A58D291-CDCA-4C22-BAE0-D55E4517D530.html) function is provided, which packs the data into USI frame format and sends the message through serial interface.

For USI message reception, the service unpacks the data and passes it through callback ([*SRV_USI_CallbackRegister*](GUID-C0B40EB5-8C77-4961-8932-C0C247B4FA29.html)) to the corresponding client depending on the USI protocol.

In the other side of the serial interface, a similar implementation is needed in an external device to pack/unpack USI messages for the corresponding USI protocol. Microchip provides PC tools and Python libraries, which are used to manage different layers of the Smart Energy stacks.

## USI Frame Format

The USI frame format is based on the HDLC specification used along with DLMS. The Serial Communications Profile of the Management Plane defined in the PRIME specification also uses the same format. Below is the structure of the USI frame and field description.

![USI general frame format](GUID-58E362D0-40B9-4360-B2B7-7610E2404FE0-low.png "USI Frame Format")

- **0x7E**: Frame start/end identifier.
- **MSG LENGTH**: Message payload length in bytes (**MESSAGE DATA** field).
- **PROTOCOL ID**: USI protocol identifier.
- **MESSAGE DATA**: Message payload (variable length). The format is defined by each protocol.
- **CRC**:  Error detection code for the message. The CRC size is defined by each protocol (see the table below). For more information about CRC format, see [*PLC CRC Service*](GUID-4E896D85-86B1-4721-880B-214CEE3819BE.html)

| USI Protocol ([*SRV_USI_PROTOCOL_ID Enum*](GUID-0C07CB6E-5CF9-4C7E-AC48-965198152AAF.html)) | PROTOCOL ID | CRC size (bits) |
|:----|:----:|:----:|
| SRV_USI_PROT_ID_MNGP_PRIME | 0x00-0x07 | 32 |
| SRV_USI_PROT_ID_SNIF_PRIME | 0x13 | 16 |
| SRV_USI_PROT_ID_PHY_SERIAL_PRIME | 0x1F | 16 |
| SRV_USI_PROT_ID_PHY | 0x22 | 16 |
| SRV_USI_PROT_ID_SNIFF_G3 | 0x23 | 16 |
| SRV_USI_PROT_ID_MAC_G3 | 0x24 | 16 |
| SRV_USI_PROT_ID_ADP_G3 | 0x25 | 16 |
| SRV_USI_PROT_ID_COORD_G3 | 0x26 | 16 |
| SRV_USI_PROT_ID_PRIME_API | 0x30 | 8 |

In order to detect the end of the message properly, it is needed to add escape bytes to all fields described above:

- **0x7E** bytes are replaced by **0x7D5E** (2 bytes)
- **0x7D** bytes are replaced by **0x7D5D** (2 bytes)

This adds overhead and has to be considered when configuring the [*size of USI service buffers*](GUID-A3663EE4-FB02-4CF3-BD0E-2BE3FED24123.html). The size of transmission buffers must be at least twice the size of the biggest message payload used (for the very worst case, all bytes 0x7E or 0x7D).

## USI Protocols

Each protocol defines its own frame format for the USI payload data. A specific PC tool or Python library is provided for each protocol.

### PHY Sniffer

There are two USI protocols used to communicate with *Microchip Hybrid Sniffer Tool*, one for PRIME (*SRV_USI_PROT_ID_SNIF_PRIME*) and one for G3 (*SRV_USI_PROT_ID_SNIFF_G3*).

For more information, see [*PLC PHY Sniffer Service*](GUID-BF84994F-1D02-4912-91AB-DCB62487A732.html).

### PLC PHY Serial

There is one USI protocol used to serialize PLC PHY API (*SRV_USI_PROT_ID_PHY*) and communicate with *Microchip PLC PHY Tester Tool* or *Microchip PLC PHY Python Libraries*. The protocol defines different sub-format for G3 and PRIME.

For more information, see [*PLC PHY Serial Service*](GUID-9CCE1919-6FB5-41BE-90E8-9A12F57D6D61.html).
