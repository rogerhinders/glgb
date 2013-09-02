#include <string.h>
#include <cstdio>
#include "header.h"

Header::Header() {

}
void Header::readHeader( u8* rom ) {
    entryPoint = rom[HDR_ENTRY_POINT]<<24|rom[HDR_ENTRY_POINT+1]<<16|rom[HDR_ENTRY_POINT+2]<<8|rom[HDR_ENTRY_POINT+3];
    memcpy( nintendoLogo,rom+HDR_NINTENDO_LOGO,NINTENDO_LOGO_SIZE );
    memcpy( title,rom+HDR_TITLE,TITLE_SIZE );
    manufacturerCode = rom[HDR_MANUFACTURER_CODE]<<24|rom[HDR_MANUFACTURER_CODE+1]<<16|rom[HDR_MANUFACTURER_CODE+2]<<8|rom[HDR_MANUFACTURER_CODE+3];
    cgbFlag = rom[HDR_CGB_FLAG];
    newLicenseeCode = rom[HDR_NEW_LICENSEE_CODE]<<8|rom[HDR_NEW_LICENSEE_CODE+1];
    sgbFlag = rom[HDR_SGB_FLAG];
    cartridgeType = rom[HDR_CARTRIDGE_TYPE];
    romSize = rom[HDR_ROM_SIZE];
    ramSize = rom[HDR_RAM_SIZE];
    destinationCode = rom[HDR_DESTINATION_CODE];
    oldLicenseeCode = rom[HDR_OLD_LICENSEE_CODE];
    maskRomVersionNumber = rom[HDR_MASK_ROM_VERSION_NUMBER];
    headerChecksum = rom[HDR_HEADER_CHECKSUM];
    globalChecksum = rom[HDR_GLOBAL_CHECKSUM]<<8|rom[HDR_GLOBAL_CHECKSUM+1];
}

