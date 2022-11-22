![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

## Smart Energy Release v1.1.0

### New Features

- **New part support** - This release introduces initial support for PIC32CX-MTSH/MTC Core 0 devices providing functionality for metrology applications.

- **Applications**
  - New Demo metering application has been introduced in this release as an example of the most common functionalities included in an electricity meter. It allows the user to evaluate the features and to test the high accuracy that can be achieved with the Microchip Smart Metering solutions.

- **New Features and Enhancements**
  - Updated and improved output format on HTML documentation.
  
### Known Issues

The current known issues are as follows:
- The following product families specifically requires the below mentioned DFP versions to be [installed](https://microchipdeveloper.com/mplabx:projects-packs) with MPLABX v6.05. It is always recommended to use the latest version of DFPs for all products provided by Microchip.
  - **PIC32CX-MT family of smart energy microcontrollers (MCUs)**: PIC32CX-MT DFP 1.0.6 or higher
- Core 1 applications and Dual debug on PIC32CX-MT devices is not supported.
- The generation of the project via MCC could generate compilation errors due to dependencies with future versions of some H3 repositories. Demo metering application will be updated accordingly when the affected H3 repositories are updated.

### Development Tools

- [MPLAB® X IDE v6.05](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.20](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator 5.2.1 or higher

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