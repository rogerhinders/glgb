#include <iostream>
#include <cstdio>
#include "gpu.h"
#include "regs.h"

using namespace std;

const u32 GPU::lcdShades[] = { 0x00E0F8D0, 0x0088C070, 0x00346856, 0x00081820 };
const u16 GPU::tileDataLoc[] = { ADDR_TILEDATA_0, ADDR_TILEDATA_1 };

GPU::GPU( MMU* mmu, CPU* cpu, u32* screen ) {
    this->mmu = mmu;
    this->cpu = cpu;
    this->screen = screen;
    mclock=0;
    mode=0;
    line=0;
    initGPU();
    updateStat();
}

GPU::~GPU() {

}

void GPU::initGPU() {
    for( int i=0; i<NUM_SPRITES; i++ ) {
        oam.push_back( OAM( i,mmu ) );
    }
}


u8 GPU::update() {
    if( ( mmu->mem[REG_LCDC]&LCDC_DISPLAY_ENABLE )==0 ) return 0;
    u8 retVal = 0;
    mclock+=cpu->T;


    switch( mode ) {
        case MODE_HBL:
            if( mclock>=204 ) {
                mclock=0;

                line++;
                mmu->mem[REG_LY]=line;
                if( line==144 ) {
                    mode=MODE_VBL;
                    retVal = 1;
                } else {
                    mode=MODE_R_OAM;
                }
                updateStat();
            }
            break;
        case MODE_VBL:
            if( mclock>=456 ) {
                u8 irqe = mmu->mem[REG_IE]&1;

                if( line==144&&cpu->IME==1&&irqe==1 ) {

                    mmu->mem[REG_IF]|=1; //irq fired

                }
                mclock=0;

                line++;

                mmu->mem[REG_LY]=line;
                if( line>153 ) {
                    mode=MODE_R_OAM;
                    updateStat();
                    line=0;
                    mmu->mem[REG_LY]=line;
                }
            }
            break;
        case MODE_R_OAM:
            if( mclock>=80 ) {
                mclock=0;
                mode=MODE_R_VRAM;
                updateStat();
            }
            break;
        case MODE_R_VRAM:
            if( mclock>=172 ) {
                drawLine( screen );
                mclock=0;
                mode=MODE_HBL;
                updateStat();
            }
            break;
    }

    return retVal;
}
void GPU::updateStat() {
    u8 tmpStat = mmu->mem[REG_STAT];
    tmpStat &= 0xFC;
    tmpStat |= mode;
    mmu->mem[REG_STAT] = tmpStat;
    //stat irq:
    if( mmu->mem[REG_LY]==mmu->mem[REG_LYC] ) {
        //fire irq
        mmu->mem[REG_IF] |= IRQ_LCD_STAT;
        cpu->releaseHalt();
        //enable coincidence bit in stat reg:
        mmu->mem[REG_STAT] |= 0x40;
    }
}

void GPU::drawLine( u32 *screen ) {
    //update bg line
    fillBGLine();

    //update wnd line
    fillWNDLine();
    
    //draw BG to screenbuffer
    for( int i=0; i<SCREEN_W; i++ ) {

        screen[i+SCREEN_W*line]=tempLineBG[( i+mmu->mem[REG_SCX] )%BG_W];
        if(tempLineWND[i]!=0) screen[i+SCREEN_W*line] = tempLineWND[i];
    }

    //draw OAM to screenbuffer
    u32 sprColor;

    for( int i=0; i<NUM_SPRITES; i++ ) {
        oam[i].update( line );
        for( s16 x=oam[i].x; x<( oam[i].x+oam[i].w ); x++ ) {
            if( ( x>=0 )&&( x<SCREEN_W ) ) {
                if(!(oam[i].priority&&(screen[x+SCREEN_W*line]!=GPU::lcdShades[mmu->mem[REG_BGP]&3]))) { //check for obj-to-bg priority
                    sprColor = oam[i].tempLine[x-oam[i].x];
                    if( sprColor!=0 ) {
                        screen[x+SCREEN_W*line] = sprColor;
                    }
                }
            }
        }
    }

}

void GPU::fillBGLine() {
    if( !( mmu->mem[REG_LCDC]&LCDC_BG_ENABLE ) ) {
        for( int i=0; i<BG_W; i++ )
            tempLineBG[i] = GPU::lcdShades[0];

        return;
    }

    u8 mapX,mapY,tileX,tileY;
    u8 oLine = ( line+mmu->mem[REG_SCY] );
    mapY=oLine/8;
    tileY=oLine&7;
    u16 mapOfs,mapBase;
    
    if(mmu->mem[REG_LCDC]&LCDC_BG_TILE_MAP)
        mapBase = 0x9c00;
    else
        mapBase = 0x9800;

    u32 tileOfs;
    u8 tileSelect = ( mmu->mem[REG_LCDC]>>4 )&1;
    for( int i=0; i<BG_W; i++ ) {
        mapX = i/8;
        tileX = i&7;
        mapOfs = mapBase + ( mapX+32*mapY );
        if( tileSelect==1 ) {
            tileOfs=GPU::tileDataLoc[tileSelect]+mmu->mem[mapOfs]*16;
        } else { //select tiles using signed offset..
            tileOfs=GPU::tileDataLoc[tileSelect]+( ( s8 )mmu->mem[mapOfs]+0x80 )*16;
        }

        tileOfs+=tileY*2;
        u8 shift = ( ( ~tileX )&7 );

        u8 colU=( mmu->mem[tileOfs+1]>>shift )&1;
        u8 colL=( mmu->mem[tileOfs]>>shift )&1;
        tempLineBG[i] = GPU::lcdShades[( mmu->mem[REG_BGP]>>( ( colU<<1|colL )*2 ) )&3];
    }
}

void GPU::fillWNDLine() {
    if( !( mmu->mem[REG_LCDC]&LCDC_WND_ENABLE ) || line<mmu->mem[REG_WY]) {
        for( int i=0; i<BG_W; i++ )
            tempLineWND[i] = 0;

        return;
    }

    u8 mapX,mapY,tileX,tileY;
    u8 oLine = ( line-mmu->mem[REG_WY] );

    mapY=oLine/8;
    tileY=oLine&7;
    u16 mapOfs,mapBase;
    
    if(mmu->mem[REG_LCDC]&LCDC_WND_TILE_MAP)
        mapBase = 0x9c00;
    else
        mapBase = 0x9800;

    u32 tileOfs;
    u8 tileSelect = ( mmu->mem[REG_LCDC]>>4 )&1;
    for( int i=0; i<BG_W; i++ ) {
        mapX = i/8;
        tileX = i&7;
        mapOfs = mapBase + ( mapX+32*mapY );
        if( tileSelect==1 ) {
            tileOfs=GPU::tileDataLoc[tileSelect]+mmu->mem[mapOfs]*16;
        } else { //select tiles using signed offset..
            tileOfs=GPU::tileDataLoc[tileSelect]+( ( s8 )mmu->mem[mapOfs]+0x80 )*16;
        }

        tileOfs+=tileY*2;
        u8 shift = ( ( ~tileX )&7 );

        u8 colU=( mmu->mem[tileOfs+1]>>shift )&1;
        u8 colL=( mmu->mem[tileOfs]>>shift )&1;
        tempLineWND[i] = GPU::lcdShades[( mmu->mem[REG_BGP]>>( ( colU<<1|colL )*2 ) )&3];
    }
}
