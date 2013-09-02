#ifndef HEADER_H
#define HEADER_H

#include "types.h"
#define NINTENDO_LOGO_SIZE 0x30
#define TITLE_SIZE 0x10

#define HDR_ENTRY_POINT 0x100
#define HDR_NINTENDO_LOGO 0x104
#define HDR_TITLE 0x134
#define HDR_MANUFACTURER_CODE 0x13F
#define HDR_CGB_FLAG 0x143
#define HDR_NEW_LICENSEE_CODE 0x144
#define HDR_SGB_FLAG 0x146
#define HDR_CARTRIDGE_TYPE 0x147
#define HDR_ROM_SIZE 0x148
#define HDR_RAM_SIZE 0x149
#define HDR_DESTINATION_CODE 0x14A
#define HDR_OLD_LICENSEE_CODE 0x14B
#define HDR_MASK_ROM_VERSION_NUMBER 0x14C
#define HDR_HEADER_CHECKSUM 0x14D
#define HDR_GLOBAL_CHECKSUM 0x14E

#define CART_NO_MBC           0x0
#define CART_MBC1             0x1
#define CART_MBC1_RAM         0x2
#define CART_MBC1_RAM_BATTERY 0x3

class Header {

public:

    u32 entryPoint;
    u8 nintendoLogo[NINTENDO_LOGO_SIZE];
    u8 title[TITLE_SIZE];
    u32 manufacturerCode;
    u8 cgbFlag;
    u16 newLicenseeCode;
    u8 sgbFlag;
    u8 cartridgeType;
    u8 romSize;
    u8 ramSize;
    u8 destinationCode;
    u8 oldLicenseeCode;
    u8 maskRomVersionNumber;
    u8 headerChecksum;
    u16 globalChecksum;

    Header();
    void readHeader( u8* rom );
};

#endif // HEADER_H
