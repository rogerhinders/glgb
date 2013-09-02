#include <string>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include "emulator.h"
#include "disassembler.h"
#include "regs.h"

using namespace std;

Emulator::Emulator( u32* screen ) {
    //load rom from disk
    //string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/individual/01-special.gb" );
    // -PASS- string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/individual/02-interrupts.gb" );
    // -PASS- string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/individual/03-op sp,hl.gb" );
    // -PASS- string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/individual/04-op r,imm.gb" );
    // -PASS- string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/individual/05-op rp.gb" );
    // -PASS- string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/individual/06-ld r,r.gb" );
    // -PASS- string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb" );
    // -PASS- string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/individual/08-misc instrs.gb" );
    // -PASS- string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/individual/09-op r,r.gb" );
    // -PASS- string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/individual/10-bit ops.gb" );
    // -PASS- string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/individual/11-op a,(hl).gb" );
    //string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/cpu_instrs.gb" );
    //string path( "/home/goebbels/Documents/emulation/proj/helloz/tbp.cgb" );
    // string path( "/home/goebbels/Documents/emulation/test_roms/cpu_instrs/individual/03-op sp,hl.gb" );
    string path( "/home/mengele/Documents/emulation/bgb/sml2.gb" );
    this->screen = screen;
    rom = new Cart( path.c_str() );
    mmu = new MMU( rom );
    cpu = new CPU( mmu );
    gpu = new GPU( mmu, cpu, screen );
    keys = new Input( mmu );

    mmu->setInput( keys );
    mmu->setCPU( cpu );

    debugger = new Debugger(cpu,mmu);
    
    cycles = 0;
    setbuf(stdout, NULL);

}

Emulator::~Emulator() {
    delete rom;
    delete cpu;
    delete mmu;
    delete gpu;
    delete keys;
    delete debugger;
}


void Emulator::run() {
    //while( cycles<REFRESH_CYCLES ) {
    while(1) {
        cpu->execInstr( mmu->memRead8( cpu->PC.w ) );
        if(gpu->update()) break;
        debugger->getCommand();
        cycles += cpu->T;
    }
    //cycles = 0;
    if( ( mmu->mem[REG_LCDC]&LCDC_DISPLAY_ENABLE )==0 ) { //if display is off, clear it
        memset(screen,0xff,SCREEN_W*SCREEN_H*4);
    };
    
}

void Emulator::setKey( u8 key, u8 event ) {
    keys->setKey( key,event );
}

void Emulator::activateDebugger() {
    debugger->activateDebugger();
}
