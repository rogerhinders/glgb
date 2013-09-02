#include <cstdio>
#include "oam.h"
#include "regs.h"
#include "gpu.h"
OAM::OAM( u8 oamNum, MMU* mmu ) {
    this->oamNumber = oamNum;
    this->mmu = mmu;
    w = 8;
}

OAM::~OAM() {

}

void OAM::update( u8 currentLine ) {
    y = mmu->mem[SPR_ATTR_TABLE_0+( oamNumber*SPR_TBL_SZ )];
    y -= 16;
    x = mmu->mem[SPR_ATTR_TABLE_1+( oamNumber*SPR_TBL_SZ )];
    x -= 8;
    tileNum = mmu->mem[SPR_ATTR_TABLE_2+( oamNumber*SPR_TBL_SZ )];
    attributes = mmu->mem[SPR_ATTR_TABLE_3+( oamNumber*SPR_TBL_SZ )];

    if( ( mmu->mem[REG_LCDC]&4 )==0 )
        h = 8;
    else
        h = 16;

    if( ( attributes&SPRITE_ATTR_PAL_DMG )==0 )
        pal = REG_OBP0;
    else
        pal = REG_OBP1;
    
    priority = (attributes>>7)&1; //obj-to-bg priority bit

    updateLine( currentLine );
}

void OAM::updateLine( u8 currentLine ) {

    u8 shift,colL,colU,tileY;
    u32 tileOfs;

    //clear line:
    for(u8 i=0;i<8;i++)
        tempLine[i] = 0;

    if( ( y==-16 )||( y>=SCREEN_H )||( x==-8 )||( x>=SCREEN_W ) || !(mmu->mem[REG_LCDC]&LCDC_OBJ_ENABLE))  //if sprite is disabled, ret
        return;

    for( s16 iy=y; iy<( y+h ); iy++ ) {
        if( iy==currentLine ) {

            for( u8 oX = 0; oX<w; oX++ ) {
                tileOfs = ADDR_TILEDATA_1; //tiles at 0x8000
                tileOfs += tileNum*16; //a tile is 16bytes in size
             
             if( ( attributes&SPRITE_ATTR_Y_FLIP )>0 )
                    tileY = (~(iy-y))&7;
                else
                    tileY = iy-y;
                
                tileOfs += tileY*2; //get y line in chosen tile, 1 line is 2 bytes wide
              
                if( ( attributes&SPRITE_ATTR_X_FLIP )>0 )
                    shift = oX;
                else
                    shift = ( ( ~oX )&7 );

                colU=( mmu->mem[tileOfs+1]>>shift )&1;
                colL=( mmu->mem[tileOfs]>>shift )&1;

                if(( colU<<1|colL )!=0 )tempLine[oX] = GPU::lcdShades[( mmu->mem[pal]>>( ( colU<<1|colL )*2 ) )&3];
            }
        }
    }
}
