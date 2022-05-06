# Configuring The Library

USI Service is configured via MCC. The following snapshots show the MCC configuration window for USI Service and brief description.

![SRV_USI_mcc_usi](GUID-4723EBC7-E7BD-45DF-A415-877591516D28-low.png "USI Service common configuration options for all instances")

- **Received Message Pool Size**
  - Specifies the maximum number of received USI messages (of any length) that can be stored, without calling [*SRV_USI_Tasks*](GUID-E0E5E6A7-18F0-4315-B9C1-E4D3011230A8.html), for all instances using UART (not used for instances using USB CDC)
- **UART API**
  - **UART Connections**
    - Indicates the number of USI instances using UART
- **CDC API**
  - **CDC Connections**
    - Indicates the number of USI instances using USB CDC

Each USI instance can be connected to:

- Peripheral Library with **UART** capability (UART/USART/FLEXCOM/SERCOM)
- USB CDC Function Driver (**USB_DEVICE_CDC** capability)

The user must connect only one dependency to each USI instance. Altough it is allowed in MCC to connect an instance to both UART and USB_DEVICE_CDC, it is not supported by the library and the code will not be generated properly.

![SRV_USI_mcc_usi_uart](GUID-CACA8649-7E51-4726-8F73-96A32701B8FE-low.png "USI Service configuration options for UART instance")

- **Device Used**
  - Indicates the hardware PLIB instance used by the corresponding instance of USI Service.
  - The underlying PLIB is automatically configured in **Non-blocking mode**, as shown in the figure below
- **Read Buffer Size**
  - Size in bytes of the buffer used by the corresponding USI instance for message reception
  - Note that the buffer can store more than one received message (**Received Message Pool Size**) if a new message is received before calling [*SRV_USI_Tasks*](GUID-E0E5E6A7-18F0-4315-B9C1-E4D3011230A8.html)
- **Write Buffer Size**
  - Size in bytes of the buffer used by the corresponding USI instance for message transmission
  - Note that USI adds overhead (start/end marks, USI header, escaped characters) to the message sent to [*SRV_USI_Send_Message*](GUID-0A58D291-CDCA-4C22-BAE0-D55E4517D530.html)

![SRV_USI_mcc_uart](GUID-83E2EA1C-492C-4DF0-8466-E9C5410CBBBD-low.png "UART Peripheral configuration")

![SRV_USI_mcc_usi_cdc](GUID-536D4849-94B1-4399-9F71-7331A8D82351-low.png "USI Service configuration options for USB CDC instance")

- **Device Used**
  - Indicates the USB CDC Instance used by the corresponding instance of USI Service.
  - The user should configure **cdc_com_port_single_demo** in the USB Device Layer module, as shown in the figure below
- **Read Buffer Size**
  - Size in bytes of the buffer used by the corresponding USI instance for message reception
- **Write Buffer Size**
  - Size in bytes of the buffer used by the corresponding USI instance for message transmission
  - Note that USI adds overhead (start/end marks, USI header, escaped characters) to the message sent to [*SRV_USI_Send_Message*](GUID-0A58D291-CDCA-4C22-BAE0-D55E4517D530.html)

![SRV_USI_mcc_cdc](GUID-6CF62397-359E-4201-8207-A593F05DC261-low.png "USB Device Layer configuration")
