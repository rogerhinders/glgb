#ifndef OAM_H
#define OAM_H

#include "types.h"
#include "mmu.h"

#define SPR_ATTR_TABLE_0 0xFE00
#define SPR_ATTR_TABLE_1 0xFE01
#define SPR_ATTR_TABLE_2 0xFE02
#define SPR_ATTR_TABLE_3 0xFE03
#define SPR_TBL_SZ 4

#define SPRITE_ATTR_PAL_DMG  0x10
#define SPRITE_ATTR_X_FLIP   0x20
#define SPRITE_ATTR_Y_FLIP   0x40
#define SPRITE_ATTR_PRIORITY 0x80

class OAM {

public:
    OAM( u8 oamNum, MMU* mmu );
    ~OAM();
    void update(u8 currentLine);
    u8 oamNumber;
    u32 tempLine[8];
    s16 y;
    s16 x;
    u8 w;
    u8 h;
    u8 priority;
private:
    u16 pal;
    u8 tileNum;
    u8 attributes;
    
    MMU* mmu;
    
    void updateLine(u8 currentLine);
};

#endif // OAM_H
