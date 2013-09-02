#include <iostream>
#include <iomanip>
#include <cstdio>
#include "cpu.h"
#include "tables.h"
#include "regs.h"

using namespace std;

CPU::CPU(MMU* mmu) {
    this->mmu = mmu;
    initCpu();
}

void CPU::initCpu() {

    AF.w=0x01b0;
    BC.w=0x0013;
    DE.w=0x00d8;
    HL.w=0x014d;
    SP.w=0xfffe;
    PC.w=CPU_START_ADDRESS;
    IME = 0;
    HALT = 0;
    HALT_IRQ = 0;
    timer = getFreq();
    rdiv = DIV_FREQ_HZ;
}

void CPU::resetTimer() {
    timer = getFreq();
}

s16 CPU::getFreq() {
    u8 whichFreq = mmu->mem[REG_TAC]&3;
    s16 freq;

    switch(whichFreq) {
        case TIMER_FREQ_0:
                freq = TIMER_FREQ_0_HZ;
        break;
        case TIMER_FREQ_1:
            freq = TIMER_FREQ_1_HZ;
        break;
        case TIMER_FREQ_2:
            freq = TIMER_FREQ_2_HZ;
        break;
        case TIMER_FREQ_3:
            freq = TIMER_FREQ_3_HZ;
        break;
    }
    return freq;
}

void CPU::updateTimer() {

    rdiv -=(s8)T;
    if(rdiv<=0) {
        mmu->mem[REG_DIV]++;
        rdiv = DIV_FREQ_HZ;
    }
    if((mmu->mem[REG_TAC]>>2)==TIMER_START) {
        timer -= (s8)T;
        if(timer<=0) {

            resetTimer();
            mmu->mem[REG_TIMA]++;
            if(mmu->mem[REG_TIMA]==0) {
                mmu->mem[REG_TIMA] = mmu->mem[REG_TMA];
                mmu->mem[REG_IF] |= IRQ_TIMER;
                releaseHalt();
            }
        }
    }
}

void CPU::updateIrq() {
    if((IME==1)&&((mmu->mem[REG_IF]&IRQ_VBL)>0)&&((mmu->mem[REG_IE]&IRQ_VBL)>0)) {
        //printf("VBL IRQ FIRED!\n");
        IME=0;
        mmu->mem[REG_IF] ^= IRQ_VBL;
        execIrq(IRQ_VBL_ADDR);

    }
    if((IME==1)&&((mmu->mem[REG_IF]&IRQ_LCD_STAT)>0)&&((mmu->mem[REG_IE]&IRQ_LCD_STAT)>0)) {
        //printf("STAT IRQ FIRED!\n");
        IME=0;
        mmu->mem[REG_IF] ^= IRQ_LCD_STAT;
        execIrq(IRQ_LCD_STAT_ADDR);
    }
    if((IME==1)&&((mmu->mem[REG_IF]&IRQ_TIMER)>0)&&((mmu->mem[REG_IE]&IRQ_TIMER)>0)) {
        //printf("TIMER IRQ FIRED!\n");
        IME=0;
        mmu->mem[REG_IF] ^= IRQ_TIMER;
        execIrq(IRQ_TIMER_ADDR);
    }
    if((IME==1)&&((mmu->mem[REG_IF]&IRQ_SERIAL)>0)&&((mmu->mem[REG_IE]&IRQ_SERIAL)>0)) {
         //printf("SERIAL IRQ FIRED!\n");
        IME=0;
        mmu->mem[REG_IF] ^= IRQ_SERIAL;
        execIrq(IRQ_SERIAL_ADDR);
    }
    if((IME==1)&&((mmu->mem[REG_IF]&IRQ_JOYPAD)>0)&&((mmu->mem[REG_IE]&IRQ_JOYPAD)>0)) {
        //printf("JOYPAD IRQ FIRED!\n");
        IME=0;
        mmu->mem[REG_IF] ^= IRQ_JOYPAD;
        execIrq(IRQ_JOYPAD_ADDR);
    }

}

void CPU::execIrq( u16 addr ) {
    releaseHalt();
    SP.w-=2;
    mmu->memWrite16(SP.w,PC.w);
    PC.w=addr;
}

void CPU::releaseHalt() {
    if(HALT==1) {
        HALT_IRQ = 1;
    }
}

void CPU::execInstr( u8 opcode ) {
  //  cout << "executing: "<< hex << (int)opcode << " d: " << (int)PC.w << endl;
//if(opcode==0xe4) for(;;) int u;
    switch( opcode ) {
        
        case 0x00: nop(); break;
        case 0x01: load16(BC.w); break;
        case 0x02: loadBC_A(); break;
        case 0x03: incReg16(BC.w); break;
        case 0x04: incReg(BC.b.hi); break;
        case 0x05: decReg(BC.b.hi); break;
        case 0x06: load8(BC.b.hi); break;
        case 0x07: rlca(); break;
        case 0x08: loadNN_SP(); break;
        case 0x09: addReg16(BC.w); break;
        case 0x0a: loadBC(); break;
        case 0x0b: decReg16(BC.w); break;
        case 0x0c: incReg(BC.b.lo); break;
        case 0x0d: decReg(BC.b.lo); break;
        case 0x0e: load8(BC.b.lo); break;
        case 0x0f: rrca(); break;
        case 0x10: stop(); break;
        case 0x11: load16(DE.w); break;
        case 0x12: loadDE_A(); break;
        case 0x13: incReg16(DE.w); break;
        case 0x14: incReg(DE.b.hi); break;
        case 0x15: decReg(DE.b.hi); break;
        case 0x16: load8(DE.b.hi); break;
        case 0x17: rla(); break;
        case 0x18: jumpR(); break;
        case 0x19: addReg16(DE.w); break;
        case 0x1a: loadDE(); break;
        case 0x1b: decReg16(DE.w); break;
        case 0x1c: incReg(DE.b.lo); break;
        case 0x1d: decReg(DE.b.lo); break;
        case 0x1e: load8(DE.b.lo); break;
        case 0x1f: rra(); break;
        case 0x20: jumpRC(opcode); break;
        case 0x21: load16(HL.w); break;
        case 0x22: loadHLInc_A(); break;
        case 0x23: incReg16(HL.w); break;
        case 0x24: incReg(HL.b.hi); break;
        case 0x25: decReg(HL.b.hi); break;
        case 0x26: load8(HL.b.hi); break;
        case 0x27: daa(); break;
        case 0x28: jumpRC(opcode); break;
        case 0x29: addReg16(HL.w); break;
        case 0x2a: loadHLInc(); break;
        case 0x2b: decReg16(HL.w); break;
        case 0x2c: incReg(HL.b.lo); break;
        case 0x2d: decReg(HL.b.lo); break;
        case 0x2e: load8(HL.b.lo); break;
        case 0x2f: cpl(); break;
        case 0x30: jumpRC(opcode); break;
        case 0x31: load16(SP.w); break;
        case 0x32: loadHLDec_A(); break;
        case 0x33: incReg16(SP.w); break;
        case 0x34: incHL(); break;
        case 0x35: decHL(); break;
        case 0x36: loadHL8(); break;
        case 0x37: scf(); break;
        case 0x38: jumpRC(opcode); break;
        case 0x39: addReg16(SP.w); break;
        case 0x3a: loadHLDec();break;
        case 0x3b: decReg16(SP.w); break;
        case 0x3c: incReg(AF.b.hi); break;
        case 0x3d: decReg(AF.b.hi); break;
        case 0x3e: load8(AF.b.hi); break;
        case 0x3f: ccf(); break;
        case 0x40: loadRR(BC.b.hi,BC.b.hi); break;
        case 0x41: loadRR(BC.b.hi,BC.b.lo); break;
        case 0x42: loadRR(BC.b.hi,DE.b.hi); break;
        case 0x43: loadRR(BC.b.hi,DE.b.lo); break;
        case 0x44: loadRR(BC.b.hi,HL.b.hi); break;
        case 0x45: loadRR(BC.b.hi,HL.b.lo); break;
        case 0x46: loadRHL(BC.b.hi); break;
        case 0x47: loadRR(BC.b.hi,AF.b.hi); break;
        case 0x48: loadRR(BC.b.lo,BC.b.hi); break;
        case 0x49: loadRR(BC.b.lo,BC.b.lo); break;
        case 0x4a: loadRR(BC.b.lo,DE.b.hi); break;
        case 0x4b: loadRR(BC.b.lo,DE.b.lo); break;
        case 0x4c: loadRR(BC.b.lo,HL.b.hi); break;
        case 0x4d: loadRR(BC.b.lo,HL.b.lo); break;
        case 0x4e: loadRHL(BC.b.lo); break;
        case 0x4f: loadRR(BC.b.lo,AF.b.hi); break;
        case 0x50: loadRR(DE.b.hi,BC.b.hi); break;
        case 0x51: loadRR(DE.b.hi,BC.b.lo); break;
        case 0x52: loadRR(DE.b.hi,DE.b.hi); break;
        case 0x53: loadRR(DE.b.hi,DE.b.lo); break;
        case 0x54: loadRR(DE.b.hi,HL.b.hi); break;
        case 0x55: loadRR(DE.b.hi,HL.b.lo); break;
        case 0x56: loadRHL(DE.b.hi); break;
        case 0x57: loadRR(DE.b.hi,AF.b.hi); break;
        case 0x58: loadRR(DE.b.lo,BC.b.hi); break;
        case 0x59: loadRR(DE.b.lo,BC.b.lo); break;
        case 0x5a: loadRR(DE.b.lo,DE.b.hi); break;
        case 0x5b: loadRR(DE.b.lo,DE.b.lo); break;
        case 0x5c: loadRR(DE.b.lo,HL.b.hi); break;
        case 0x5d: loadRR(DE.b.lo,HL.b.lo); break;
        case 0x5e: loadRHL(DE.b.lo); break;
        case 0x5f: loadRR(DE.b.lo,AF.b.hi); break;
        case 0x60: loadRR(HL.b.hi,BC.b.hi); break;
        case 0x61: loadRR(HL.b.hi,BC.b.lo); break;
        case 0x62: loadRR(HL.b.hi,DE.b.hi); break;
        case 0x63: loadRR(HL.b.hi,DE.b.lo); break;
        case 0x64: loadRR(HL.b.hi,HL.b.hi); break;
        case 0x65: loadRR(HL.b.hi,HL.b.lo); break;
        case 0x66: loadRHL(HL.b.hi); break;
        case 0x67: loadRR(HL.b.hi,AF.b.hi); break;
        case 0x68: loadRR(HL.b.lo,BC.b.hi); break;
        case 0x69: loadRR(HL.b.lo,BC.b.lo); break;
        case 0x6a: loadRR(HL.b.lo,DE.b.hi); break;
        case 0x6b: loadRR(HL.b.lo,DE.b.lo); break;
        case 0x6c: loadRR(HL.b.lo,HL.b.hi); break;
        case 0x6d: loadRR(HL.b.lo,HL.b.lo); break;
        case 0x6e: loadRHL(HL.b.lo); break;
        case 0x6f: loadRR(HL.b.lo,AF.b.hi); break;
        case 0x70: loadHLR(BC.b.hi); break;
        case 0x71: loadHLR(BC.b.lo); break;
        case 0x72: loadHLR(DE.b.hi); break;
        case 0x73: loadHLR(DE.b.lo); break;
        case 0x74: loadHLR(HL.b.hi); break;
        case 0x75: loadHLR(HL.b.lo); break;
        case 0x76: halt(); break;
        case 0x77: loadHLR(AF.b.hi); break;
        case 0x78: loadRR(AF.b.hi,BC.b.hi); break;
        case 0x79: loadRR(AF.b.hi,BC.b.lo); break;
        case 0x7a: loadRR(AF.b.hi,DE.b.hi); break;
        case 0x7b: loadRR(AF.b.hi,DE.b.lo); break;
        case 0x7c: loadRR(AF.b.hi,HL.b.hi); break;
        case 0x7d: loadRR(AF.b.hi,HL.b.lo); break;
        case 0x7e: loadRHL(AF.b.hi); break;
        case 0x7f: loadRR(AF.b.hi,AF.b.hi); break;
        case 0x80: addReg(BC.b.hi); break;
        case 0x81: addReg(BC.b.lo); break;
        case 0x82: addReg(DE.b.hi); break;
        case 0x83: addReg(DE.b.lo); break;
        case 0x84: addReg(HL.b.hi); break;
        case 0x85: addReg(HL.b.lo); break;
        case 0x86: addHL(); break;
        case 0x87: addReg(AF.b.hi); break;
        case 0x88: adcReg(BC.b.hi); break;
        case 0x89: adcReg(BC.b.lo); break;
        case 0x8A: adcReg(DE.b.hi); break;
        case 0x8B: adcReg(DE.b.lo); break;
        case 0x8C: adcReg(HL.b.hi); break;
        case 0x8D: adcReg(HL.b.lo); break;
        case 0x8E: adcHL(); break;
        case 0x8F: adcReg(AF.b.hi); break;
        case 0x90: subReg(BC.b.hi); break;
        case 0x91: subReg(BC.b.lo); break;
        case 0x92: subReg(DE.b.hi); break;
        case 0x93: subReg(DE.b.lo); break;
        case 0x94: subReg(HL.b.hi); break;
        case 0x95: subReg(HL.b.lo); break;
        case 0x96: subHL(); break;
        case 0x97: subReg(AF.b.hi); break;
        case 0x98: sbcReg(BC.b.hi); break;
        case 0x99: sbcReg(BC.b.lo); break;
        case 0x9A: sbcReg(DE.b.hi); break;
        case 0x9B: sbcReg(DE.b.lo); break;
        case 0x9C: sbcReg(HL.b.hi); break;
        case 0x9D: sbcReg(HL.b.lo); break;
        case 0x9E: sbcHL(); break;
        case 0x9F: sbcReg(AF.b.hi); break;
        case 0xa0: andReg(BC.b.hi); break;
        case 0xa1: andReg(BC.b.lo); break;
        case 0xa2: andReg(DE.b.hi); break;
        case 0xa3: andReg(DE.b.lo); break;
        case 0xa4: andReg(HL.b.hi); break;
        case 0xa5: andReg(HL.b.lo); break;
        case 0xa6: andHL(); break;
        case 0xa7: andReg(AF.b.hi); break;
        case 0xa8: xorReg(BC.b.hi); break;
        case 0xa9: xorReg(BC.b.lo); break;
        case 0xaa: xorReg(DE.b.hi); break;
        case 0xab: xorReg(DE.b.lo); break;
        case 0xac: xorReg(HL.b.hi); break;
        case 0xad: xorReg(HL.b.lo); break;
        case 0xae: xorHL(); break;
        case 0xaf: xorReg(AF.b.hi); break;
        case 0xb0: orReg(BC.b.hi); break;
        case 0xb1: orReg(BC.b.lo); break;
        case 0xb2: orReg(DE.b.hi); break;
        case 0xb3: orReg(DE.b.lo); break;
        case 0xb4: orReg(HL.b.hi); break;
        case 0xb5: orReg(HL.b.lo); break;
        case 0xb6: orHL(); break;
        case 0xb7: orReg(AF.b.hi); break;
        case 0xb8: cpReg(BC.b.hi); break;
        case 0xb9: cpReg(BC.b.lo); break;
        case 0xba: cpReg(DE.b.hi); break;
        case 0xbb: cpReg(DE.b.lo); break;
        case 0xbc: cpReg(HL.b.hi); break;
        case 0xbd: cpReg(HL.b.lo); break;
        case 0xbe: cpHL(); break;
        case 0xbf: cpReg(AF.b.hi); break;
        case 0xc0: retc(opcode); break;
        case 0xc1: pop(BC.w); break;
        case 0xc2: jumpCNN(opcode); break;
        case 0xc3: jumpNN(); break;
        case 0xc4: callCNN(opcode); break;
        case 0xc5: push(BC.w); break;
        case 0xc6: add8(); break;
        case 0xc7: rst(opcode); break;
        case 0xc8: retc(opcode); break;
        case 0xc9: ret(); break;
        case 0xca: jumpCNN(opcode); break;
        case 0xcb: execInstrCB();break;
        case 0xcc: callCNN(opcode); break;
        case 0xcd: callNN(); break;
        case 0xce: adc8(); break;
        case 0xcf: rst(opcode); break;
        case 0xd0: retc(opcode); break;
        case 0xd1: pop(DE.w); break;
        case 0xd2: jumpCNN(opcode); break;
        case 0xd3: illegal(); break;
        case 0xd4: callCNN(opcode); break;
        case 0xd5: push(DE.w); break;
        case 0xd6: sub8(); break;
        case 0xd7: rst(opcode); break;
        case 0xd8: retc(opcode); break;
        case 0xd9: reti(); break;
        case 0xda: jumpCNN(opcode); break;
        case 0xdb: illegal(); break;
        case 0xdc: callCNN(opcode); break;
        case 0xdd: illegal(); break;
        case 0xde: sbc8(); break;
        case 0xdf: rst(opcode); break;
        case 0xe0: loadIOPortN_A(); break;
        case 0xe1: pop(HL.w); break;
        case 0xe2: loadIOPortC_A(); break;
        case 0xe3: illegal(); break;
        case 0xe4: illegal(); break;
        case 0xe5: push(HL.w); break;
        case 0xe6: and8(); break;
        case 0xe7: rst(opcode); break;
        case 0xe8: addSP(); break;
        case 0xe9: jumpHL(); break;
        case 0xea: loadNN_A(); break;
        case 0xeb: illegal(); break;
        case 0xec: illegal(); break;
        case 0xed: illegal(); break;
        case 0xee: xor8(); break;
        case 0xef: rst(opcode); break;
        case 0xf0: loadIOPortN(); break;
        case 0xf1: popAF(); break;
        case 0xf2: loadIOPortC(); break;
        case 0xf3: di(); break;
        case 0xf4: illegal(); break;
        case 0xf5: push(AF.w); break;
        case 0xf6: or8(); break;
        case 0xf7: rst(opcode); break;
        case 0xf8: ldHLSP(); break;
        case 0xf9: loadSPHL(); break;
        case 0xfa: loadNN(); break;
        case 0xfb: ei(); break;
        case 0xfc: illegal(); break;
        case 0xfd: illegal(); break;
        case 0xfe: cp8(); break;
        case 0xff: rst(opcode); break;
    }
    //update current cycle
    if(opcode==0xcb) {
        T = op_t_cb[opcode];
    } else {
        T = op_t[opcode];
    }
    updateTimer();
    updateIrq();
}

void CPU::execInstrCB () {
    u8 cbop = mmu->memRead8(PC.w+1);
    
    switch(cbop) {
        case 0x00: rlc(BC.b.hi); break;
        case 0x01: rlc(BC.b.lo); break;
        case 0x02: rlc(DE.b.hi); break;
        case 0x03: rlc(DE.b.lo); break;
        case 0x04: rlc(HL.b.hi); break;
        case 0x05: rlc(HL.b.lo); break;
        case 0x06: rlcHL(); break;
        case 0x07: rlc(AF.b.hi); break;
        case 0x08: rrc(BC.b.hi); break;
        case 0x09: rrc(BC.b.lo); break;
        case 0x0a: rrc(DE.b.hi); break;
        case 0x0b: rrc(DE.b.lo); break;
        case 0x0c: rrc(HL.b.hi); break;
        case 0x0d: rrc(HL.b.lo); break;
        case 0x0e: rrcHL(); break;
        case 0x0f: rrc(AF.b.hi); break;
        case 0x10: rl(BC.b.hi); break;
        case 0x11: rl(BC.b.lo); break;
        case 0x12: rl(DE.b.hi); break;
        case 0x13: rl(DE.b.lo); break;
        case 0x14: rl(HL.b.hi); break;
        case 0x15: rl(HL.b.lo); break;
        case 0x16: rlHL(); break;
        case 0x17: rl(AF.b.hi); break;
        case 0x18: rr(BC.b.hi); break;
        case 0x19: rr(BC.b.lo); break;
        case 0x1a: rr(DE.b.hi); break;
        case 0x1b: rr(DE.b.lo); break;
        case 0x1c: rr(HL.b.hi); break;
        case 0x1d: rr(HL.b.lo); break;
        case 0x1e: rrHL(); break;
        case 0x1f: rr(AF.b.hi); break;
        case 0x20: sla(BC.b.hi); break;
        case 0x21: sla(BC.b.lo); break;
        case 0x22: sla(DE.b.hi); break;
        case 0x23: sla(DE.b.lo); break;
        case 0x24: sla(HL.b.hi); break;
        case 0x25: sla(HL.b.lo); break;
        case 0x26: slaHL(); break;
        case 0x27: sla(AF.b.hi); break;
        case 0x28: sra(BC.b.hi); break;
        case 0x29: sra(BC.b.lo); break;
        case 0x2a: sra(DE.b.hi); break;
        case 0x2b: sra(DE.b.lo); break;
        case 0x2c: sra(HL.b.hi); break;
        case 0x2d: sra(HL.b.lo); break;
        case 0x2e: sraHL(); break;
        case 0x2f: sra(AF.b.hi); break;
        case 0x30: swap(BC.b.hi); break;
        case 0x31: swap(BC.b.lo); break;
        case 0x32: swap(DE.b.hi); break;
        case 0x33: swap(DE.b.lo); break;
        case 0x34: swap(HL.b.hi); break;
        case 0x35: swap(HL.b.lo); break;
        case 0x36: swapHL(); break;
        case 0x37: swap(AF.b.hi); break;
        case 0x38: srl(BC.b.hi); break;
        case 0x39: srl(BC.b.lo); break;
        case 0x3a: srl(DE.b.hi); break;
        case 0x3b: srl(DE.b.lo); break;
        case 0x3c: srl(HL.b.hi); break;
        case 0x3d: srl(HL.b.lo); break;
        case 0x3e: srlHL(); break;
        case 0x3f: srl(AF.b.hi); break;
        case 0x40: bit(0,BC.b.hi); break;
        case 0x41: bit(0,BC.b.lo); break;
        case 0x42: bit(0,DE.b.hi); break;
        case 0x43: bit(0,DE.b.lo); break;
        case 0x44: bit(0,HL.b.hi); break;
        case 0x45: bit(0,HL.b.lo); break;
        case 0x46: bitHL(0); break;
        case 0x47: bit(0,AF.b.hi); break;
        case 0x48: bit(1,BC.b.hi); break;
        case 0x49: bit(1,BC.b.lo); break;
        case 0x4a: bit(1,DE.b.hi); break;
        case 0x4b: bit(1,DE.b.lo); break;
        case 0x4c: bit(1,HL.b.hi); break;
        case 0x4d: bit(1,HL.b.lo); break;
        case 0x4e: bitHL(1); break;
        case 0x4f: bit(1,AF.b.hi); break;
        case 0x50: bit(2,BC.b.hi); break;
        case 0x51: bit(2,BC.b.lo); break;
        case 0x52: bit(2,DE.b.hi); break;
        case 0x53: bit(2,DE.b.lo); break;
        case 0x54: bit(2,HL.b.hi); break;
        case 0x55: bit(2,HL.b.lo); break;
        case 0x56: bitHL(2); break;
        case 0x57: bit(2,AF.b.hi); break;
        case 0x58: bit(3,BC.b.hi); break;
        case 0x59: bit(3,BC.b.lo); break;
        case 0x5a: bit(3,DE.b.hi); break;
        case 0x5b: bit(3,DE.b.lo); break;
        case 0x5c: bit(3,HL.b.hi); break;
        case 0x5d: bit(3,HL.b.lo); break;
        case 0x5e: bitHL(3); break;
        case 0x5f: bit(3,AF.b.hi); break;
        case 0x60: bit(4,BC.b.hi); break;
        case 0x61: bit(4,BC.b.lo); break;
        case 0x62: bit(4,DE.b.hi); break;
        case 0x63: bit(4,DE.b.lo); break;
        case 0x64: bit(4,HL.b.hi); break;
        case 0x65: bit(4,HL.b.lo); break;
        case 0x66: bitHL(4); break;
        case 0x67: bit(4,AF.b.hi); break;
        case 0x68: bit(5,BC.b.hi); break;
        case 0x69: bit(5,BC.b.lo); break;
        case 0x6a: bit(5,DE.b.hi); break;
        case 0x6b: bit(5,DE.b.lo); break;
        case 0x6c: bit(5,HL.b.hi); break;
        case 0x6d: bit(5,HL.b.lo); break;
        case 0x6e: bitHL(5); break;
        case 0x6f: bit(5,AF.b.hi); break;
        case 0x70: bit(6,BC.b.hi); break;
        case 0x71: bit(6,BC.b.lo); break;
        case 0x72: bit(6,DE.b.hi); break;
        case 0x73: bit(6,DE.b.lo); break;
        case 0x74: bit(6,HL.b.hi); break;
        case 0x75: bit(6,HL.b.lo); break;
        case 0x76: bitHL(6); break;
        case 0x77: bit(6,AF.b.hi); break;
        case 0x78: bit(7,BC.b.hi); break;
        case 0x79: bit(7,BC.b.lo); break;
        case 0x7a: bit(7,DE.b.hi); break;
        case 0x7b: bit(7,DE.b.lo); break;
        case 0x7c: bit(7,HL.b.hi); break;
        case 0x7d: bit(7,HL.b.lo); break;
        case 0x7e: bitHL(7); break;
        case 0x7f: bit(7,AF.b.hi); break;
        case 0x80: res(0,BC.b.hi); break;
        case 0x81: res(0,BC.b.lo); break;
        case 0x82: res(0,DE.b.hi); break;
        case 0x83: res(0,DE.b.lo); break;
        case 0x84: res(0,HL.b.hi); break;
        case 0x85: res(0,HL.b.lo); break;
        case 0x86: resHL(0); break;
        case 0x87: res(0,AF.b.hi); break;
        case 0x88: res(1,BC.b.hi); break;
        case 0x89: res(1,BC.b.lo); break;
        case 0x8a: res(1,DE.b.hi); break;
        case 0x8b: res(1,DE.b.lo); break;
        case 0x8c: res(1,HL.b.hi); break;
        case 0x8d: res(1,HL.b.lo); break;
        case 0x8e: resHL(1); break;
        case 0x8f: res(1,AF.b.hi); break;
        case 0x90: res(2,BC.b.hi); break;
        case 0x91: res(2,BC.b.lo); break;
        case 0x92: res(2,DE.b.hi); break;
        case 0x93: res(2,DE.b.lo); break;
        case 0x94: res(2,HL.b.hi); break;
        case 0x95: res(2,HL.b.lo); break;
        case 0x96: resHL(2); break;
        case 0x97: res(2,AF.b.hi); break;
        case 0x98: res(3,BC.b.hi); break;
        case 0x99: res(3,BC.b.lo); break;
        case 0x9a: res(3,DE.b.hi); break;
        case 0x9b: res(3,DE.b.lo); break;
        case 0x9c: res(3,HL.b.hi); break;
        case 0x9d: res(3,HL.b.lo); break;
        case 0x9e: resHL(3); break;
        case 0x9f: res(3,AF.b.hi); break;
        case 0xa0: res(4,BC.b.hi); break;
        case 0xa1: res(4,BC.b.lo); break;
        case 0xa2: res(4,DE.b.hi); break;
        case 0xa3: res(4,DE.b.lo); break;
        case 0xa4: res(4,HL.b.hi); break;
        case 0xa5: res(4,HL.b.lo); break;
        case 0xa6: resHL(4); break;
        case 0xa7: res(4,AF.b.hi); break;
        case 0xa8: res(5,BC.b.hi); break;
        case 0xa9: res(5,BC.b.lo); break;
        case 0xaa: res(5,DE.b.hi); break;
        case 0xab: res(5,DE.b.lo); break;
        case 0xac: res(5,HL.b.hi); break;
        case 0xad: res(5,HL.b.lo); break;
        case 0xae: resHL(5); break;
        case 0xaf: res(5,AF.b.hi); break;
        case 0xb0: res(6,BC.b.hi); break;
        case 0xb1: res(6,BC.b.lo); break;
        case 0xb2: res(6,DE.b.hi); break;
        case 0xb3: res(6,DE.b.lo); break;
        case 0xb4: res(6,HL.b.hi); break;
        case 0xb5: res(6,HL.b.lo); break;
        case 0xb6: resHL(6); break;
        case 0xb7: res(6,AF.b.hi); break;
        case 0xb8: res(7,BC.b.hi); break;
        case 0xb9: res(7,BC.b.lo); break;
        case 0xba: res(7,DE.b.hi); break;
        case 0xbb: res(7,DE.b.lo); break;
        case 0xbc: res(7,HL.b.hi); break;
        case 0xbd: res(7,HL.b.lo); break;
        case 0xbe: resHL(7); break;
        case 0xbf: res(7,AF.b.hi); break;
        case 0xc0: set(0,BC.b.hi); break;
        case 0xc1: set(0,BC.b.lo); break;
        case 0xc2: set(0,DE.b.hi); break;
        case 0xc3: set(0,DE.b.lo); break;
        case 0xc4: set(0,HL.b.hi); break;
        case 0xc5: set(0,HL.b.lo); break;
        case 0xc6: setHL(0); break;
        case 0xc7: set(0,AF.b.hi); break;
        case 0xc8: set(1,BC.b.hi); break;
        case 0xc9: set(1,BC.b.lo); break;
        case 0xca: set(1,DE.b.hi); break;
        case 0xcb: set(1,DE.b.lo); break;
        case 0xcc: set(1,HL.b.hi); break;
        case 0xcd: set(1,HL.b.lo); break;
        case 0xce: setHL(1); break;
        case 0xcf: set(1,AF.b.hi); break;
        case 0xd0: set(2,BC.b.hi); break;
        case 0xd1: set(2,BC.b.lo); break;
        case 0xd2: set(2,DE.b.hi); break;
        case 0xd3: set(2,DE.b.lo); break;
        case 0xd4: set(2,HL.b.hi); break;
        case 0xd5: set(2,HL.b.lo); break;
        case 0xd6: setHL(2); break;
        case 0xd7: set(2,AF.b.hi); break;
        case 0xd8: set(3,BC.b.hi); break;
        case 0xd9: set(3,BC.b.lo); break;
        case 0xda: set(3,DE.b.hi); break;
        case 0xdb: set(3,DE.b.lo); break;
        case 0xdc: set(3,HL.b.hi); break;
        case 0xdd: set(3,HL.b.lo); break;
        case 0xde: setHL(3); break;
        case 0xdf: set(3,AF.b.hi); break;
        case 0xe0: set(4,BC.b.hi); break;
        case 0xe1: set(4,BC.b.lo); break;
        case 0xe2: set(4,DE.b.hi); break;
        case 0xe3: set(4,DE.b.lo); break;
        case 0xe4: set(4,HL.b.hi); break;
        case 0xe5: set(4,HL.b.lo); break;
        case 0xe6: setHL(4); break;
        case 0xe7: set(4,AF.b.hi); break;
        case 0xe8: set(5,BC.b.hi); break;
        case 0xe9: set(5,BC.b.lo); break;
        case 0xea: set(5,DE.b.hi); break;
        case 0xeb: set(5,DE.b.lo); break;
        case 0xec: set(5,HL.b.hi); break;
        case 0xed: set(5,HL.b.lo); break;
        case 0xee: setHL(5); break;
        case 0xef: set(5,AF.b.hi); break;
        case 0xf0: set(6,BC.b.hi); break;
        case 0xf1: set(6,BC.b.lo); break;
        case 0xf2: set(6,DE.b.hi); break;
        case 0xf3: set(6,DE.b.lo); break;
        case 0xf4: set(6,HL.b.hi); break;
        case 0xf5: set(6,HL.b.lo); break;
        case 0xf6: setHL(6); break;
        case 0xf7: set(6,AF.b.hi); break;
        case 0xf8: set(7,BC.b.hi); break;
        case 0xf9: set(7,BC.b.lo); break;
        case 0xfa: set(7,DE.b.hi); break;
        case 0xfb: set(7,DE.b.lo); break;
        case 0xfc: set(7,HL.b.hi); break;
        case 0xfd: set(7,HL.b.lo); break;
        case 0xfe: setHL(7); break;
        case 0xff: set(7,AF.b.hi); break;
    }
}

void CPU::nop() {
    PC.w++;
}

void CPU::illegal() {

}

void CPU::addReg( u8& reg ) {
    prev = AF.b.hi;
    AF.b.hi += reg;
    AF.f.c = AF.b.hi < prev;
    AF.f.h = ( AF.b.hi&0xF )<( prev&0xF );
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}
void CPU::addHL() {
    prev = AF.b.hi;
    AF.b.hi += mmu->memRead8(HL.w);
    AF.f.c = AF.b.hi < prev;
    AF.f.h = ( AF.b.hi&0xF )<( prev&0xF );
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}


void CPU::add8() {
    prev = AF.b.hi;
    AF.b.hi += mmu->memRead8(++PC.w);
    
    AF.f.c = AF.b.hi < prev;
    AF.f.h = ( AF.b.hi&0xF )<( prev&0xF );
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::adcReg( u8& reg ) {
    prev = AF.b.hi;
    u8 tmpC = AF.f.c;
    AF.f.c = (prev+reg+tmpC)>0xFF;
    AF.f.h = ((prev&0xF)+(reg&0xF)+tmpC)>0xF;
    AF.f.n = 0;
    AF.b.hi += reg+tmpC;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}
void CPU::adcHL() {
    prev = AF.b.hi;
    u8 tmpC = AF.f.c;
    u8 reg = mmu->memRead8(HL.w);
    AF.b.hi += reg+tmpC;
    AF.f.c = (prev+reg+tmpC)>0xFF;
    AF.f.h = ((prev&0xF)+(reg&0xF)+tmpC)>0xF;
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::adc8() {
    prev = AF.b.hi;
    u8 tmpC = AF.f.c;
    u8 imm8 = mmu->memRead8(++PC.w);
    AF.b.hi += imm8+tmpC;
    AF.f.c = (prev+imm8+tmpC)>0xFF;
    AF.f.h = ((prev&0xF)+(imm8&0xF)+tmpC)>0xF;
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::subReg(u8& reg) {
    prev = AF.b.hi;
    AF.b.hi -= reg; 
    AF.f.c = (AF.b.hi>prev);
    AF.f.h = (AF.b.hi&0xF)>(prev&0xF);
    AF.f.n = 1;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::subHL() {
    prev = AF.b.hi;
    AF.b.hi -= mmu->memRead8(HL.w); 
    AF.f.c = (AF.b.hi>prev);
    AF.f.h = (AF.b.hi&0xF)>(prev&0xF);
    AF.f.n = 1;
    AF.f.z = (AF.b.hi==0);
    PC.w++;   
}

void CPU::sub8() {
    prev = AF.b.hi;
    AF.b.hi -= mmu->memRead8(++PC.w); 
    AF.f.c = (AF.b.hi>prev);
    AF.f.h = (AF.b.hi&0xF)>(prev&0xF);
    AF.f.n = 1;
    AF.f.z = (AF.b.hi==0);
    PC.w++;  
}

void CPU::sbcReg(u8& reg) {
    prev = AF.b.hi;
    u16 tmp1,tmp2;
    tmp1 = prev-reg-AF.f.c;
    tmp2 = (prev&0xf)-(reg&0xf)-AF.f.c;
    AF.b.hi = tmp1;
    AF.f.c = (tmp1>0xff);
    AF.f.h = (tmp2>0xf);
    AF.f.n = 1;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::sbcHL() {
    prev = AF.b.hi;
    u8 reg = mmu->memRead8(HL.w);
    u16 tmp1,tmp2;
    tmp1 = prev-reg-AF.f.c;
    tmp2 = (prev&0xf)-(reg&0xf)-AF.f.c;
    AF.b.hi = tmp1;
    AF.f.c = (tmp1>0xff);
    AF.f.h = (tmp2>0xf);
    AF.f.n = 1;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::sbc8() {
    prev = AF.b.hi;
    u8 imm8 = mmu->memRead8(++PC.w);
    u16 tmp1,tmp2;
    tmp1 = prev-imm8-AF.f.c;
    tmp2 = (prev&0xf)-(imm8&0xf)-AF.f.c;
    AF.b.hi = tmp1;
    AF.f.c = (tmp1>0xff);
    AF.f.h = (tmp2>0xf);
    AF.f.n = 1;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::andReg(u8& reg) {
    AF.b.hi &= reg;
    AF.f.c = 0;
    AF.f.h = 1;
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::andHL() {
    AF.b.hi &= mmu->memRead8(HL.w);
    AF.f.c = 0;
    AF.f.h = 1;
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::and8() {
    AF.b.hi &= mmu->memRead8(++PC.w);
    AF.f.c = 0;
    AF.f.h = 1;
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::xorReg(u8& reg) {
    AF.b.hi ^= reg;
    AF.f.c = 0;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::xorHL() {
    AF.b.hi ^= mmu->memRead8(HL.w);
    AF.f.c = 0;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::xor8() {
    AF.b.hi ^= mmu->memRead8(++PC.w);
    AF.f.c = 0;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::orReg(u8& reg) {
    AF.b.hi |= reg;
    AF.f.c = 0;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::orHL() {
    AF.b.hi |= mmu->memRead8(HL.w);
    AF.f.c = 0;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::or8() {
    AF.b.hi |= mmu->memRead8(++PC.w);
    AF.f.c = 0;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = (AF.b.hi==0);
    PC.w++;
}

void CPU::cpReg(u8& reg) {
    u8 cmp = AF.b.hi-reg;
    AF.f.c = cmp>AF.b.hi;
    AF.f.h = (cmp&0xF)>(AF.b.hi&0xF);
    AF.f.n = 1;
    AF.f.z = (cmp==0);
    PC.w++;
}

void CPU::cpHL() {
    u8 cmp = AF.b.hi-mmu->memRead8(HL.w);
    AF.f.c = cmp>AF.b.hi;
    AF.f.h = (cmp&0xF)>(AF.b.hi&0xF);
    AF.f.n = 1;
    AF.f.z = (cmp==0);
    PC.w++;
}

void CPU::cp8() {
    u8 cmp = AF.b.hi-mmu->memRead8(++PC.w);
    AF.f.c = cmp>AF.b.hi;
    AF.f.h = (cmp&0xF)>(AF.b.hi&0xF);
    AF.f.n = 1;
    AF.f.z = (cmp==0);
    PC.w++;
}

void CPU::incReg(u8& reg) {
    prev = reg++;
    AF.f.h = (prev&0xF)>(reg&0xF);
    AF.f.n = 0;
    AF.f.z = (reg==0);
    PC.w++;
}

void CPU::incHL() {
    prev = mmu->memRead8(HL.w);
    u8 tmp = prev+1;
    mmu->memWrite8(HL.w,tmp);
    AF.f.h = (prev&0xF)>(tmp&0xF);
    AF.f.n = 0;
    AF.f.z = (tmp==0);
    PC.w++;
}
void CPU::decReg(u8& reg) {
    prev = reg--;
    AF.f.h = (prev&0xF)<(reg&0xF);
    AF.f.n = 1;
    AF.f.z = (reg==0);
    PC.w++;
}

void CPU::decHL() {
    prev = mmu->memRead8(HL.w);
    u8 tmp = prev-1;
    mmu->memWrite8(HL.w,tmp);
    AF.f.h = (prev&0xF)<(tmp&0xF);
    AF.f.n = 1;
    AF.f.z = (tmp==0);
    PC.w++;
}

void CPU::daa() {
    AF.f.z = 0;
    u16 cy = AF.f.c;
    u16 val = AF.b.hi;
    val = val&0xFF;

    if(AF.f.n==0) {
        if(AF.f.h==1||((val&0xf)>0x9)) val += 0x6;
        if(cy==1||(val>0x9f)) val += 0x60;
    } else {
        if(AF.f.h==1) {
            val =(val-6)&0xFF;
        }
        if(cy==1) val -= 0x60;
    }

    AF.f.h=0;
    AF.b.hi = val;
    AF.f.z=(val==0);
    if(val&0x100) AF.f.c=1;
    PC.w++;
}


void CPU::cpl() {
    AF.b.hi = 0xFF-AF.b.hi;
    AF.f.n = 1;
    AF.f.h = 1;
    PC.w++;
}

void CPU::addReg16(u16& reg) {
    prev16 = HL.w;
    HL.w += reg;
    //cout << "HL:" << hex << prev16 << "R:" << hex << reg << "=" << hex << HL.w << endl; 
    AF.f.c = (HL.w<prev16);
    AF.f.h = (HL.w&0xFFF)<(prev16&0xFFF);
    AF.f.n = 0;
    PC.w++;
}

void CPU::incReg16(u16& reg) {
    reg++;
    PC.w++;
}

void CPU::decReg16(u16& reg) {
    reg--;
    PC.w++;
}

void CPU::addSP() {
    s8 imm8 = mmu->memRead8(++PC.w);
    AF.f.c = 0;
    AF.f.h = 0;
    if(imm8>=0) {
        if(((SP.w&0xFF)+(imm8))>0xFF) AF.f.c = 1;
        if(((SP.w&0xF)+(imm8&0xF))>0xF) AF.f.h = 1;
    } else {
        if(((SP.w+imm8)&0xFF)<=(SP.w&0xFF)) AF.f.c = 1;
        if(((SP.w+imm8)&0xF)<=(SP.w&0xF)) AF.f.h = 1;
    }
    AF.f.n = 0;
    AF.f.z = 0;
    SP.w = (s16)SP.w + imm8;
    PC.w++;
}

void CPU::ldHLSP() {
    s8 imm8 = mmu->memRead8(++PC.w);
    AF.f.c = 0;
    AF.f.h = 0;
    if(imm8>=0) {
        if(((SP.w&0xFF)+(imm8))>0xFF) AF.f.c = 1;
        if(((SP.w&0xF)+(imm8&0xF))>0xF) AF.f.h = 1;
    } else {
        if(((SP.w+imm8)&0xFF)<=(SP.w&0xFF)) AF.f.c = 1;
        if(((SP.w+imm8)&0xF)<=(SP.w&0xF)) AF.f.h = 1;
    }
    AF.f.n = 0;
    AF.f.z = 0;
    HL.w = (s16)SP.w+(s8)imm8;
    PC.w++;
}

void CPU::loadRR( u8& dst, u8& src ) {
    dst = src;
    PC.w++;
}

void CPU::loadRHL( u8& dst ) {
    dst = mmu->memRead8(HL.w);
    PC.w++;
}

void CPU::loadHLR( u8& src ) {
    mmu->memWrite8(HL.w,src);
    PC.w++;
}

void CPU::load8( u8& reg ) {
    reg = mmu->memRead8(++PC.w);
    PC.w++;
}

void CPU::loadHL8(){
    mmu->memWrite8(HL.w,mmu->memRead8(++PC.w));
    PC.w++;
}

void CPU::loadBC() {
    AF.b.hi=mmu->memRead8(BC.w);
    PC.w++;
}
void CPU::loadDE() {
    AF.b.hi=mmu->memRead8(DE.w);
    PC.w++;
}
void CPU::loadNN() {
    AF.b.hi=mmu->memRead8(mmu->memRead16(PC.w+1));
    PC.w+=3;
}
void CPU::loadBC_A() {
    mmu->memWrite8(BC.w,AF.b.hi);
    PC.w++;
}
void CPU::loadDE_A() {
    mmu->memWrite8(DE.w,AF.b.hi);
    PC.w++;
}
void CPU::loadNN_A() {
    mmu->memWrite8(mmu->memRead16(PC.w+1),AF.b.hi);
    PC.w+=3;
}
void CPU::loadIOPortN() {
    AF.b.hi=mmu->memRead8(0xFF00+mmu->memRead8(PC.w+1));
    PC.w+=2;
}
void CPU::loadIOPortN_A() {
    mmu->memWrite8(0xFF00+mmu->memRead8(PC.w+1),AF.b.hi);
    PC.w+=2;
}
void CPU::loadIOPortC() {
    AF.b.hi=mmu->memRead8(0xFF00+BC.b.lo);
    PC.w++;
}
void CPU::loadIOPortC_A() {
    mmu->memWrite8(0xFF00+BC.b.lo,AF.b.hi);
    PC.w++;
}
void CPU::loadHLInc_A() {
    mmu->memWrite8(HL.w,AF.b.hi);
    HL.w++;
    PC.w++;
}
void CPU::loadHLInc() {
    AF.b.hi = mmu->memRead8(HL.w);
    HL.w++;
    PC.w++;
}
void CPU::loadHLDec_A() {
    mmu->memWrite8(HL.w,AF.b.hi);
    HL.w--;
    PC.w++;
}
void CPU::loadHLDec() {
    AF.b.hi = mmu->memRead8(HL.w);
    HL.w--;
    PC.w++;
}

void CPU::load16( u16& reg ) {
    reg = mmu->memRead16(PC.w+1);
    PC.w += 3;
}

void CPU::loadSPHL() {
    SP.w=HL.w;
    PC.w++;
}

void CPU::push( u16& reg ) {
        SP.w -= 2;
        mmu->memWrite16(SP.w,reg);
        PC.w++;
}

void CPU::pop( u16& reg ) {
    reg = mmu->memRead16(SP.w);
    SP.w += 2;
    PC.w++;
}
void CPU::popAF() {
    AF.w = mmu->memRead16(SP.w)&0xFFF0;
    SP.w += 2;
    PC.w++;
}

void CPU::loadNN_SP() {
    mmu->memWrite16(mmu->memRead16(PC.w+1),SP.w);
    PC.w+=3;
}
void CPU::jumpNN() {
    PC.w=mmu->memRead16(PC.w+1);
}

void CPU::jumpHL() {
    PC.w=HL.w;
}

void CPU::jumpCNN(u8 opcode) {
    
    u16 loc = mmu->memRead16(PC.w+1);
    u8 flag = (opcode>>3)&3;
    if(flag==JP_FLAG_NZ&&AF.f.z==0) { 
        PC.w=loc; 
        return; 
    }
    if(flag==JP_FLAG_Z&&AF.f.z==1) { 
        PC.w=loc; 
        return; 
        
    }
    if(flag==JP_FLAG_NC&&AF.f.c==0) { 
        PC.w=loc; return; 

    }
    if(flag==JP_FLAG_C&&AF.f.c==1) { 
        PC.w=loc; 
        return; 
        
    }
    PC.w+=3;
}

void CPU::jumpR() {
    s8 imm8 = mmu->memRead8(PC.w+1);
    if(imm8>127) imm8=-((~imm8+1));
    PC.w+=2;
    PC.w+=imm8;
}

void CPU::jumpRC(u8 opcode) {
    s8 imm8 = mmu->memRead8(PC.w+1);
    if(imm8>127) imm8=-((~imm8+1));
    u8 flag = (opcode>>3)&3;

    if(flag==JP_FLAG_NZ&&AF.f.z==0) {
        PC.w+=2;
        PC.w+=imm8;
        return;
    }
    if(flag==JP_FLAG_Z&&AF.f.z==1) {
        PC.w+=2;
        PC.w+=imm8;
        return;
    }
    if(flag==JP_FLAG_NC&&AF.f.c==0) {
        PC.w+=2;
        PC.w+=imm8;
        return;
    }
    if(flag==JP_FLAG_C&&AF.f.c==1) {
        PC.w+=2;
        PC.w+=imm8;
        return;
    }
    PC.w+=2;
}

void CPU::callNN() {
    u16 loc = mmu->memRead16(PC.w+1);
    SP.w-=2;
    PC.w+=3;
    mmu->memWrite16(SP.w,PC.w);
    PC.w=loc;
}

void CPU::callCNN(u8 opcode) {
    u16 loc = mmu->memRead16(PC.w+1);
    PC.w+=3;
    u8 flag = (opcode>>3)&3;
    if(flag==JP_FLAG_NZ&&AF.f.z==0) {
        SP.w-=2;
        mmu->memWrite16(SP.w,PC.w);
        PC.w=loc;
        return;
    }
    if(flag==JP_FLAG_Z&&AF.f.z==1) {
        SP.w-=2;
        mmu->memWrite16(SP.w,PC.w);
        PC.w=loc;
        return;
    }
    if(flag==JP_FLAG_NC&&AF.f.c==0) {
        SP.w-=2;
        mmu->memWrite16(SP.w,PC.w);
        PC.w=loc;
        return;
    }
    if(flag==JP_FLAG_C&&AF.f.c==1) {
        SP.w-=2;
        mmu->memWrite16(SP.w,PC.w);
        PC.w=loc;
        return;
    }
}

void CPU::ret() {
    PC.w = mmu->memRead16(SP.w);
    SP.w+=2;
}

void CPU::retc(u8 opcode) {
    u8 flag = (opcode>>3)&3;
 
    if(flag==JP_FLAG_NZ&&AF.f.z==0) {
        PC.w = mmu->memRead16(SP.w);
        SP.w += 2;
        return;
    }
    if(flag==JP_FLAG_Z&&AF.f.z==1) {
        PC.w = mmu->memRead16(SP.w);
        SP.w += 2;
        return;
    }
    if(flag==JP_FLAG_NC&&AF.f.c==0) {
        PC.w = mmu->memRead16(SP.w);
        SP.w += 2;
        return;
    }
    if(flag==JP_FLAG_C&&AF.f.c==1) {
        PC.w = mmu->memRead16(SP.w);
        SP.w += 2;
        return;
    }
    PC.w++;
}

void CPU::reti() {
    ret();
    IME=1;
}

void CPU::rst( u8 opcode ) {
    SP.w -= 2;
    PC.w += 1;
    mmu->memWrite16(SP.w,PC.w);
    PC.w = ((opcode>>3)&7)*8;
}

void CPU::ccf() {
    AF.f.c ^= 1;
    AF.f.h = 0;
    AF.f.n = 0;
    PC.w++;
}

void CPU::scf() {
    AF.f.c = 1;
    AF.f.h = 0;
    AF.f.n = 0;
    PC.w++;
}

void CPU::halt() {
    HALT = 1;
    if(HALT_IRQ==1) {
        PC.w++;
        HALT_IRQ = 0;
        HALT = 0;
    }
}

void CPU::stop() {
    /* to be fixed.. */
    HALT = 1;
    if(HALT_IRQ==1) {
        PC.w++;
        HALT_IRQ = 0;
        HALT = 0;
    }
}

void CPU::di() {
    IME=0;
    PC.w++;
}

void CPU::ei() {
    IME=1;
    PC.w++;
}

void CPU::rlca() {
    AF.f.c = (AF.b.hi&0x80)>>7;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = 0;
    AF.b.hi = rol8(AF.b.hi,1);
    PC.w++;
}

void CPU::rla() {
    AF.f.c = (AF.b.lo&0x10)>>4;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = 0;
    AF.b.hi = rol8(AF.b.hi,1);
    u8 b0 = AF.b.hi&1;
    AF.b.hi = (AF.b.hi&0xFE)|AF.f.c;
    AF.f.c = b0;
    PC.w++;
}

void CPU::rrca() {
    AF.f.c = AF.b.hi&0x1;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = 0;
    AF.b.hi = ror8(AF.b.hi,1);
    PC.w++;
}

void CPU::rra() {
    AF.f.c = (AF.b.lo&0x10)>>4;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = 0;
    AF.b.hi = ror8(AF.b.hi,1);
    u8 b7 = (AF.b.hi&0x80)>>7;
    AF.b.hi = (AF.b.hi&0x7f)|(AF.f.c<<7);
    AF.f.c = b7;
    PC.w++;
}

void CPU::bit( const u8 bit, u8& reg ) {
    AF.f.c = (AF.b.lo&0x10)>>4;
    AF.f.h = 1;
    AF.f.n = 0;
    AF.f.z = !(((reg>>bit)&1)==1);
    PC.w+=2;
}

void CPU::bitHL( const u8 bit ) {
    AF.f.c = (AF.b.lo&0x10)>>4;
    AF.f.h = 1;
    AF.f.n = 0;
    AF.f.z = !(((mmu->memRead8(HL.w)>>bit)&1)==1);
    PC.w+=2;
}

void CPU::res( const u8 bit, u8& reg ) {
    reg &=~(1<<bit);
    PC.w+=2;
}

void CPU::resHL( const u8 bit ) {
    u8 val = mmu->memRead8(HL.w);
    val &=~(1<<bit);
    mmu->memWrite8(HL.w,val);
    PC.w+=2;
}

void CPU::set( const u8 bit, u8& reg ) {
    reg |=1<<bit;
    PC.w+=2;
}

void CPU::setHL( const u8 bit ) {
    u8 val = mmu->memRead8(HL.w);
    val |=1<<bit;
    mmu->memWrite8(HL.w,val);
    PC.w+=2;
}

void CPU::rlc( u8& reg ) {
    AF.f.c = (reg&0x80)>>7;
    reg=rol8(reg,1);    
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = reg==0;
    PC.w+=2;
}
void CPU::rlcHL() {
    u8 val = mmu->memRead8(HL.w);
    AF.f.c = (val&0x80)>>7;
    val=rol8(val,1);
    mmu->memWrite8(HL.w,val);
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = val==0;
    PC.w+=2;
}

void CPU::rl( u8& reg ) {
    AF.f.c = (AF.b.lo&0x10)>>4;
    reg=rol8(reg,1);
    u8 b0 = reg&1;
    reg = (reg&0xFE)|AF.f.c;
    AF.f.c = b0;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = reg==0;
    PC.w += 2;
}

void CPU::rlHL() {
    u8 val = mmu->memRead8(HL.w);
    AF.f.c = (AF.b.lo&0x10)>>4;
    val=rol8(val,1);
    u8 b0 = val&1;
    val = (val&0xFE)|AF.f.c;
    mmu->memWrite8(HL.w,val);
    AF.f.c = b0;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = val==0;
    PC.w += 2;
}

void CPU::rrc( u8& reg ) {
    AF.f.c = reg&0x1;
    reg=ror8(reg,1);
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = reg==0;
    PC.w += 2;
}

void CPU::rrcHL() {
    u8 val = mmu->memRead8(HL.w);
    AF.f.c = val&0x1;
    val=ror8(val,1);
    mmu->memWrite8(HL.w,val);
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = val==0;
    PC.w += 2;
}

void CPU::rr( u8& reg ) {
    AF.f.c = (AF.b.lo&0x10)>>4;
    reg=ror8(reg,1);
    u8 b7 = (reg&0x80)>>7;
    reg = (reg&0x7f)|(AF.f.c<<7);
    AF.f.c = b7;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = reg==0;
    PC.w+=2;
}

void CPU::rrHL() {
    u8 val = mmu->memRead8(HL.w);
    AF.f.c = (AF.b.lo&0x10)>>4;
    val=ror8(val,1);
    u8 b7 = (val&0x80)>>7;
    val = (val&0x7f)|(AF.f.c<<7);
    mmu->memWrite8(HL.w,val);
    AF.f.c = b7;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = val==0;
    PC.w+=2;
}

void CPU::sla( u8& reg ) {
    AF.f.c = (reg&0x80)>>7;
    reg = reg<<1;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = reg==0;   
    PC.w+=2;
}

void CPU::slaHL() {
    u8 val = mmu->memRead8(HL.w);
    AF.f.c = (val&0x80)>>7;
    val = val<<1;
    mmu->memWrite8(HL.w,val);
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = val==0;   
    PC.w += 2;
}

void CPU::swap( u8& reg ) {
    reg = ((reg&0xf)<<4)|((reg&0xf0)>>4);
    AF.f.c = 0;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = reg==0;
    PC.w += 2;
}

void CPU::swapHL() {
    u8 val = mmu->memRead8(HL.w);
    val = ((val&0xf)<<4)|((val&0xf0)>>4);
    mmu->memWrite8(HL.w,val);
    AF.f.c = 0;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = val==0;
    PC.w += 2;
}

void CPU::sra( u8& reg ) {
    AF.f.c = reg&1;
    u8 b7 = (reg&0x80);
    reg = reg>>1;
    reg = (reg&0x7F)|b7;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = reg==0;    
    PC.w += 2;
}

void CPU::sraHL() {
    u8 val = mmu->memRead8(HL.w);
    AF.f.c = val&1;
    u8 b7 = (val&0x80);
    val = val>>1;
    val = (val&0x7F)|b7;
    mmu->memWrite8(HL.w,val);
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = val==0;    
    PC.w += 2;
}

void CPU::srl( u8& reg ) {
    AF.f.c = reg&1;
    reg = reg>>1;
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = reg==0; 
    PC.w += 2;
} 

void CPU::srlHL() {
    u8 val = mmu->memRead8(HL.w);
    AF.f.c = val&1;
    val = val>>1;
    mmu->memWrite8(HL.w,val);
    AF.f.h = 0;
    AF.f.n = 0;
    AF.f.z = val==0; 
    PC.w += 2;
} 
u8 CPU::rol8(u8 x, u8 i) {
    return (x<<i)|(x>>(8-i));
}
u8 CPU::ror8(u8 x, u8 i) {
    return (x>>i)|(x<<(8-i));
}
