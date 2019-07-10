# Microchip MPLAB Harmony 3 Release Notes
## Smart Energy - PLC Release v3.0.0
### NEW FEATURES
- **New part support** - This release introduces initial support for [PLCB](https://www.microchip.com/wwwproducts/en/PLCB) providing functionality for G3/PRIME physical layer.

- **USB Library** - The following table provides the list of PLC Library components and Applications. 

| Type | Module Name |  Module Caption |
| --- | --- | --- |
| Driver | driver\pl360 | Driver for Power Line Communication (PLC) modem |
| Service | Universal Serial Interface (USI) | Universal Serial Interface (USI) Library  |
| Service | PLC CRC Function Driver (PCRC)| PLC CRC Function Driver (PCRC) Library|
| Service | PLC Phy Tester Tool Serial Interface (PSERIAL) | PLC Phy Tester Tool Serial Interface (PSERIAL) Library  |
| Service | PLC Phy Sniffer Tool Serial Interface (PSERIAL) | PLC Phy Sniffer Tool Serial Interface (PSERIAL) Library  |
| PLC Driver App | g3_getting_started  | PLC G3 Getting Started Demonstration for CEN A, CEN B and FCC bands |
| PLC Driver App | prime_getting_started  | PLC PRIME Getting Started Demonstration  |
| PLC Driver App | g3_tx_console  | PLC G3 Transmission Console Demonstration for CEN B band and multi-band (CEN A, FCC)  |
| PLC Driver App | prime_tx_console  | PLC PRIME Transmission Console Demonstration for channels from 1 to 8 |
| PLC Tools App | g3_phy_sniffer_tool  | PLC G3 Physical Sniffer application to connect with MCHP PLC Sniffer PC Tool for CEN A, CEN B and FCC bands |
| PLC Tools App | prime_phy_sniffer_tool  | PLC PRIME Physical Sniffer application to connect with MCHP PLC Sniffer PC Tool |
| PLC Tools App | g3_phy_tester_tool  | PLC G3 Physical Tester application to connect with MCHP PLC Phy Tester PC Tool for CEN A, CEN B and FCC bands |
| PLC Tools App | prime_phy_tester_tool  | PLC PRIME Physical Tester application to connect with MCHP PLC Tester PC Tool |


- **Development kit and demo application support** - The following table provides number of PLC applications available for different development kits

| Development Kits | Number of applications |
| --- | --- |
| [SAM E70 Xplained Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame70-xpld) | 15 |

### KNOWN ISSUES

The current known issues are as follows:

* No known issues

### DEVELOPMENT TOOLS

* [MPLAB X IDE v5.15](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB XC32 C/C++ Compiler v2.15](https://www.microchip.com/mplab/compilers)
* MPLAB X IDE plug-ins:
    * MPLAB Harmony Configurator (MHC) v3.3.0.1.

For additional information, see the following links and release notes.
* [Read-me File](./readme.md)
* [Microchip MPLAB Harmony](https://www.microchip.com/mplab/mplab-harmony)
* [Microchip MPLAB Harmony Pages](https://microchip-mplab-harmony.github.io/)
* [Microchip PLC products](https://www.microchip.com/design-centers/smart-energy-products/power-line-communications)