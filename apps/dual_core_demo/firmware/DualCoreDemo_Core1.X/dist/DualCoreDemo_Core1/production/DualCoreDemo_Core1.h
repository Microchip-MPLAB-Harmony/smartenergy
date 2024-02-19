#ifndef __BIN2HEX_XC32_SECONDARY_RECORD_H
#define __BIN2HEX_XC32_SECONDARY_RECORD_H
typedef struct __xc32_section_record {
    void *sectionDestinationAddress;
    const unsigned char *sectionData;
} __xc32_section_record;

extern const __xc32_section_record __attribute__((space(prog))) __secondary_image_sections[];
extern const void __xc32_LoadSecondarySection(__xc32_section_record *section_to_load);
extern const void __xc32_LoadAllSecondarySections();

#endif //__BIN2HEX_XC32_SECONDARY_RECORD_H