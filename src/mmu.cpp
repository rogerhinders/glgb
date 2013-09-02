//
// TODO:
// fix updateKeyReg and resetTimer
#include <cstdio>
#include "mmu.h"
#include "regs.h"

using namespace std;

MMU::MMU( Cart* cartridge ) {
    cart = cartridge;
    initMem();
}


MMU::~MMU() {

}

void MMU::setInput( Input* keys ) {
    this->keys = keys;
}

void MMU::setCPU( CPU* cpu ) {
    this->cpu = cpu;
}


void MMU::initMem() {
    mem[REG_TIMA] = 0x00;
    mem[REG_TMA]  = 0x00;
    mem[REG_TAC]  = 0x00;
    mem[REG_NR10] = 0x80;
    mem[REG_NR11] = 0xBF;
    mem[REG_NR12] = 0xF3;
    mem[REG_NR14] = 0xBF;
    mem[REG_NR21] = 0x3F;
    mem[REG_NR22] = 0x00;
    mem[REG_NR24] = 0xBF;
    mem[REG_NR30] = 0x7F;
    mem[REG_NR31] = 0xFF;
    mem[REG_NR32] = 0x9F;
    mem[REG_NR34] = 0xBF;
    mem[REG_NR41] = 0xFF;
    mem[REG_NR42] = 0x00;
    mem[REG_NR43] = 0x00;
    mem[REG_NR44] = 0xBF;
    mem[REG_NR50] = 0x77;
    mem[REG_NR51] = 0xF3;
    mem[REG_NR52] = 0xF1;
    mem[REG_LCDC] = 0x91;
    mem[REG_SCY]  = 0x00;
    mem[REG_SCX]  = 0x00;
    mem[REG_LYC]  = 0x00;
    mem[REG_BGP]  = 0xFC;
    mem[REG_OBP0] = 0xFF;
    mem[REG_OBP1] = 0xFF;
    mem[REG_WY]   = 0x00;
    mem[REG_WX]   = 0x00;
    mem[REG_IE]   = 0x00;
    
    mbc1RomBank = 0;
    mbc1RamBank = 0;
}

void MMU::memWrite8( u16 addr, u8 value ) {
    u8 *p = &mem[0];
    if(addr<=0x3FFF ) { // 0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
        p = &cart->rom[0];

        //MBC1 set rom bank
        if( ( ( cart->hdr->cartridgeType==CART_MBC1 )||( cart->hdr->cartridgeType==CART_MBC1_RAM )||( cart->hdr->cartridgeType==CART_MBC1_RAM_BATTERY ) ) && ( addr>=0x2000 )&&( addr<=0x3fff ) ) {
            mbc1RomBank = value&0x1F;
     //       printf("setting rombank\n");
        }

        return;
    }
    if( addr>=0x4000 && addr<=0x7FFF ) { // 4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
        p = &cart->rom[0];

        //MBC1 set ram bank
        if( ( ( cart->hdr->cartridgeType==CART_MBC1_RAM )||( cart->hdr->cartridgeType==CART_MBC1_RAM_BATTERY ) ) && ( addr>=0x4000 )&&( addr<=0x5fff ) ) {
            mbc1RamBank = value&3;
        }

        return;
    }
    if( addr>=0x8000 && addr<=0x9fff ) { //8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
        p = &mem[0];
    }
    if( addr>=0xA000 && addr<=0xBFFF ) { // A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
        p = &mem[0];
    }
    if( addr>=0xC000 && addr<=0xCFFF ) { // C000-CFFF   4KB Work RAM Bank 0 (WRAM)
        p = &mem[0];
    }
    if( addr>=0xD000 && addr<=0xDFFF ) { // D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
        p = &mem[0];
    }
    if( addr>=0xE000 && addr<=0xFDFF ) { // E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
        p = &mem[0];
        addr-=0x2000;
    }
    if( addr>=0xFE00 && addr<=0xFE9F ) { // FE00-FE9F   Sprite Attribute Table (OAM)
        p = &mem[0];
    }
    if( addr>=0xFEA0 && addr<=0xFEFF ) { // FEA0-FEFF   Not Usable
        p = &mem[0];
        return;
    }
    if( addr>=0xFF00 && addr<=0xFF7F ) { // FF00-FF7F   I/O Ports
        p = &mem[0];
        if( addr==REG_JOYP ) {
            keys->updateKeyReg( value );
            return;
        }
        if( addr==REG_DMA ) {
            dma();
        }
        if(addr==REG_SCX&&mem[REG_LY]!=145) printf("%d -> %d\n",value,mem[REG_LY]);
    }
    if( addr>=0xFF80 && addr<=0xFFFE ) { // FF80-FFFE   High RAM (HRAM)
        p = &mem[0];
    }
    if( addr==0xFFFF ) { // FFFF        Interrupt Enable Register
        p = &mem[0];
    }
    
    p[addr]=value;
    //reset timer if started or stopped:
    if( addr==REG_TAC ) cpu->resetTimer();
}
u8 MMU::memRead8( u16 addr ) {
    u8 *p = &mem[0];
    u32 offset = 0x0;
    if(addr<=0x3FFF ) { // 0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
        p = &cart->rom[0];
    }

    if( addr>=0x4000 && addr<=0x7FFF ) { // 4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
        p = &cart->rom[0];

        //mbc1 rom bank
        if( ( cart->hdr->cartridgeType==CART_MBC1 )||( cart->hdr->cartridgeType==CART_MBC1_RAM )||( cart->hdr->cartridgeType==CART_MBC1_RAM_BATTERY ) ) {
            if( ( mbc1RomBank!=0x0 )&&( mbc1RomBank!=0x1 ) ) {
                offset = ( mbc1RomBank*0x4000 )-0x4000;
            }

        }
    }
    if( addr>=0x8000 && addr<=0x9fff ) { //8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
        p = &mem[0];
    }
    if( addr>=0xA000 && addr<=0xBFFF ) { // A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
        p = &mem[0];

        //mbc1 ram bank
        if( cart->hdr->cartridgeType==CART_MBC1 ) {

        }
    }
    if( addr>=0xC000 && addr<=0xCFFF ) { // C000-CFFF   4KB Work RAM Bank 0 (WRAM)
        p = &mem[0];
    }
    if( addr>=0xD000 && addr<=0xDFFF ) { // D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
        p = &mem[0];
    }
    if( addr>=0xE000 && addr<=0xFDFF ) { // E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
        p = &mem[0];
        addr-=0x2000;
    }
    if( addr>=0xFE00 && addr<=0xFE9F ) { // FE00-FE9F   Sprite Attribute Table (OAM)
        p = &mem[0];
    }
    if( addr>=0xFEA0 && addr<=0xFEFF ) { // FEA0-FEFF   Not Usable
        p = &mem[0];
        return 0;
    }
    if( addr>=0xFF00 && addr<=0xFF7F ) { // FF00-FF7F   I/O Ports
        p = &mem[0];
    }
    if( addr>=0xFF80 && addr<=0xFFFE ) { // FF80-FFFE   High RAM (HRAM)
        p = &mem[0];
    }
    if( addr==0xFFFF ) { // FFFF        Interrupt Enable Register
        p = &mem[0];
    }

    return p[addr+offset];
}
void MMU::memWrite16( u16 addr, u16 value ) {
    memWrite8( addr,value&0xff );
    memWrite8( addr+1,value>>8 );
}

u16 MMU::memRead16( u16 addr ) {
    return memRead8( addr+1 )<<8|memRead8( addr );
}
void MMU::printMem( u16 start, u16 end ) {
    int i;
    for( i=start; i<end; i++ ) {
        printf( " 0x%02x",memRead8(i) );
    }
}
void MMU::dma() {
    int i;
    u16 addr = mem[REG_DMA]<<8;
    for( i=0; i<0xA0; i++ ) {
        memWrite8( 0xFE00+i,memRead8( addr+i ) );
    }
}