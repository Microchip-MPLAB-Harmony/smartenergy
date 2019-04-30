/*******************************************************************************
  User Configuration Header

  File Name:
    user.h

  Summary:
    Build-time configuration header for the user defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    It only provides macro definitions for build-time configuration options

*******************************************************************************/

#ifndef USER_H
#define USER_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: User Configuration macros
// *****************************************************************************
// *****************************************************************************
#define DACC_CFG_TABLE_CH1      {0x00000000, 0x00002120, 0x0000073F, 0x00003F3F, \
                                 0x00000333, 0x00000000, 0x546000FF, 0x1A1A1A1A, \
                                 0x00002020, 0x00000044, 0x0FD20005, 0x00000355, \
                                 0x0F000000, 0x001020F0, 0x00000355, 0x0F000000, \
                                 0x001020FF}
    
#define DACC_CFG_CH2_8_COUP06   {0x00000000, 0x10102120, 0x033F073F, 0x3F3F3F3F, \
                                 0x00000FFF, 0x00000000, 0xA8C000FF, 0x1B1B1B1B, \
                                 0x10101010, 0x00001111, 0x04380006, 0x000003AA, \
                                 0xF0000000, 0x001020F0, 0x00000355, 0x0F000000, \
                                 0x001020FF}

#define DACC_CFG_CH2_8_COUP11   {0x00000000, 0x21202120, 0x073F073F, 0x3F3F3F3F, \
                                 0x00000FFF, 0x00000000, 0xA8C000FF, 0x1B1B1B1B, \
                                 0x10101010, 0x00001111, 0x04380006, 0x000003AA, \
                                 0xF0000000, 0x001020FF, 0x000003AA, 0xF0000000, \
                                 0x001020FF}

#define DACC_CFG_TABLE_CH2_8    DACC_CFG_CH2_8_COUP11  

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
