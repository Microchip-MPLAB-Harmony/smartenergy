![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

## Smart Energy Release v1.3.0

### New Features

- **Applications**
  - Applications updated to use the code from new releases of dependent Harmony 3 modules.

- **New Features and Enhancements**
  - Added support for Meters And More PLC standard on both Drivers and Services.
  - Reviewed and updated support for PRIME (PLC and RF) standard on both Drivers and Services.

### Bug fixes

- None.

### Known Issues

- "Image Copy Mode" setting value of the Secondary Project is not persisting in PIC32CX-MT Dual Core 0 Demo applications. Once the project has been opened, it is necessary to make a modification to replace the "default" value with the "manual" value. For further information, please check the project configuration following chapter 1.3.2.3 Linking Dual Core Demo Applications in the documentation provided in the docs folder.

### Development Tools

- [MPLAB® X IDE v6.20](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.45](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator 5.5.1 or higher
- PIC32CX-MT family (MCUs):
  - PIC32CX-MT DFP 1.3.132 or higher

### Notes

- Metrology Driver and Demo App not available anymore in this repository. This has moved to a new smartenergy_metrology repository also available on this same platform.

## Smart Energy Release v1.2.1

### New Features

- **Applications**
  - Applications updated to use the code from new releases of dependent Harmony 3 modules.

- **New Features and Enhancements**
  - None.

### Bug fixes

- Added missing parameter on Embedded RF Sniffer function.
- Restored FreeRTOS folder on RTOS-based projects.

### Known Issues

- "Image Copy Mode" setting value of the Secondary Project is not persisting in PIC32CX-MT Dual Core 0 Demo applications. Once the project has been opened, it is necessary to make a modification to replace the "default" value with the "manual" value. For further information, please check the project configuration following chapter 1.3.2.3 Linking Dual Core Demo Applications in the documentation provided in the docs folder.
- On example applications for SAMD20 platform, file *\src\config\sam_d20_xpro_pl460\peripheral\sercom\spi_master\plib_sercom0_spi_master.c* has to be reverted after regeneration, to preserve some required initialization values which are deleted when file is regenerated.

### Development Tools

- [MPLAB® X IDE v6.20](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.40](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator 5.5.1 or higher
- PIC32CX-MT family (MCUs):
  - PIC32CX-MT DFP 1.3.132 or higher

### Notes

- None

## Smart Energy Release v1.2.0

### New Features

- **Applications**
  - Metering Demo and Dual Core Demo applications updated to use latest Smart Energy Drivers and Services.

- **New Features and Enhancements**
  - Added *rf215* driver which provides support to [AT86RF215 radio transceiver](https://www.microchip.com/en-us/product/at86rf215).
  - Added new Services:
    - log_report - Provides unified access to logging system from Smart Energy stacks.
    - queue - Implements a queing system for Smart Energy stacks.
    - random - Provides unified access to Random Number Generation from Smart Energy stacks.
    - rserial - Implements RF transceiver serialization.
    - rsniffer - Implements RF transceiver sniffer capabilities.
    - security - Provides unified access to Cryptographic functions from Smart Energy stacks.
    - spisplit - Allows to connect both PLC and RF transceiver to the same SPI Peripheral.
  - Updated PL360/PL460 binaries to latest versions.
  - Added RTOS support to Drivers implementation.
  - Extended support on PLC Driver to new parts:
    - [WBZ451](https://www.microchip.com/en-us/products/wireless-connectivity/bluetooth-low-energy/pic32cx-bz2-family).
    - [SAMD20](https://www.microchip.com/en-us/product/ATSAMD20J18).
  - Updated Metrology Driver to latest Metrology binaries.

### Bug fixes

- None

### Known Issues

- "Image Copy Mode" setting value of the Secondary Project is not persisting in PIC32CX-MT Dual Core 0 Demo applications. Once the project has been opened, it is necessary to make a modification to replace the "default" value with the "manual" value. For further information, please check the project configuration following chapter 1.3.2.3 Linking Dual Core Demo Applications in the documentation provided in the docs folder.

### Development Tools

- [MPLAB® X IDE v6.20](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.35](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator 5.5.0 or higher
- PIC32CX-MT family (MCUs):
  - PIC32CX-MT DFP 1.3.132 or higher

### Notes

- None

## Smart Energy Release v1.1.1

### New Features

- None.

### Bug fixes

- Solve dependency issues in Harmony content. Replace repository name harmony_gfx by gfx.

### Known Issues

- None.

### Development Tools

- [MPLAB® X IDE v6.05](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.21](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator 5.2.2 or higher
- PIC32CX-MT family (MCUs):
  - PIC32CX-MT DFP 1.1.115 or higher

### Notes

- None

## Smart Energy Release v1.1.0

### New Features

- **New part support** - This release introduces initial support for PIC32CX-MTSH/MTC Core 0 devices providing functionality for metrology and dual core applications.

- **Applications**
  - New Demo metering application has been introduced in this release as an example of the most common functionalities included in an electricity meter. It allows the user to evaluate the features and to test the high accuracy that can be achieved with the Microchip Smart Metering solutions.
  - Smart Energy Demo Dual Core Applications: a pair of examples showing how to run and communicate 2 separate applications independently on both cores of the PIC32CXMTSH dual core device.

- **New Features and Enhancements**
  - Updated and improved output format on HTML documentation.

### Bug fixes

  - None

### Known Issues

- "Image Copy Mode" setting value of the Secondary Project is not persisting in MISTRAL Dual Core 0 Demo application. Once the project has been opened, it is necessary to make a modification to replace the "default" value with the "manual" value. For further information, please check the project configuration following chapter 1.3.2.3 Linking Dual Core Demo Applications in the documentation provided in the docs folder.

### Development Tools

- [MPLAB® X IDE v6.05](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.21](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator 5.2.2 or higher
- PIC32CX-MT family (MCUs):
  - PIC32CX-MT DFP 1.1.115 or higher

### Notes

- None

## Smart Energy Release v1.0.0

### NEW FEATURES

- **New part support** - This release introduces initial support for [PL360B](https://www.microchip.com/wwwproducts/en/PL360B) and [MPL460A](https://www.microchip.com/en-us/product/mpl460a) providing functionality for G3 PLC/PRIME physical layer and G3 PLC MAC RT layer.

- **Smart Energy Library** - The following table provides the list of Smart Energy Library components and Applications.

| Type | Module Name |  Module Caption |
| --- | --- | --- |
| Driver | PLC\_PHY | PLC PHY Driver |
| Driver | G3\_MAC\_RT | G3 MAC RT Driver |
| Service | USI | Universal Serial Interface Service  |
| Service | PCRC| PLC CRC Function Service|
| Service | PCOUP| PLC PHY Coupling Service|
| Service | PVDDMON| PLC PVDD Monitor Service|
| Service | PSNIFFER| PLC Sniffer Tool Service|
| Service | PSERIAL| PLC Tester Tool Service|

- **Applications**
  - MPLAB Harmony provides large number of application examples to accelerate learning and reduce the development cycles for your embedded systems with reusable software components. The application examples are available in the respective [product family specific repository](apps/readme.md).

### KNOWN ISSUES

- None.

### DEVELOPMENT TOOLS

- [MPLAB® X IDE v6.00](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.10](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator 5.1.4

For additional information, see the following links and release notes.
* [Read-me File](./readme.md)
* [Microchip MPLAB Harmony](https://www.microchip.com/mplab/mplab-harmony)
* [Microchip MPLAB Harmony Pages](https://microchip-mplab-harmony.github.io/)
* [Microchip Smart Energy products](https://www.microchip.com/design-centers/smart-energy-products/power-line-communications)