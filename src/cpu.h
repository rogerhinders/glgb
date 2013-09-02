#ifndef CPU_H
#define CPU_H

#include "types.h"
#include "mmu.h"

#define CPU_START_ADDRESS 0x100

#define FLAG_C 0x10
#define FLAG_H 0x20
#define FLAG_N 0x40
#define FLAG_Z 0x80

#define JP_FLAG_NZ 0
#define JP_FLAG_Z  1
#define JP_FLAG_NC 2
#define JP_FLAG_C  3

#define TIMER_FREQ_0 0
#define TIMER_FREQ_1 1
#define TIMER_FREQ_2 2
#define TIMER_FREQ_3 3

#define CLOCK_FREQ 4194304

#define TIMER_FREQ_0_HZ CLOCK_FREQ/4096
#define TIMER_FREQ_1_HZ CLOCK_FREQ/262144
#define TIMER_FREQ_2_HZ CLOCK_FREQ/65536
#define TIMER_FREQ_3_HZ CLOCK_FREQ/16384

#define TIMER_STOP 0
#define TIMER_START 1

#define DIV_FREQ_HZ CLOCK_FREQ/16384

#define REFRESH_RATE 59.7
#define REFRESH_CYCLES CLOCK_FREQ/REFRESH_RATE

#define IRQ_VBL 1
#define IRQ_LCD_STAT 2
#define IRQ_TIMER 4
#define IRQ_SERIAL 8
#define IRQ_JOYPAD 16

#define IRQ_VBL_ADDR 0x0040
#define IRQ_LCD_STAT_ADDR 0x0048
#define IRQ_TIMER_ADDR 0x0050
#define IRQ_SERIAL_ADDR 0x0058
#define IRQ_JOYPAD_ADDR 0x0060

class MMU;

class CPU {
public:

    typedef union {
        u16 w;
        struct {
            u8 lo,hi;
        } b;
        struct {
            u8 b0 :1;
            u8 b1 :1;
            u8 b2 :1;
            u8 b3 :1;
            u8 c  :1;
            u8 h  :1;
            u8 n  :1;
            u8 z  :1;
            u8 hi;
        } f;

    } Reg16;


    Reg16 AF;
    Reg16 BC;
    Reg16 DE;
    Reg16 HL;
    Reg16 SP;
    Reg16 PC;

    u8 IME;
    u8 T;
    u8 HALT;
    u8 HALT_IRQ;

    CPU( MMU* mmu );
    void resetTimer();
    void releaseHalt();
    void execInstr( u8 opcode );
    void execInstrCB();

private:
    //tmp vars for instructions:
    u8 prev;
    u16 prev16;
    
    //timer stuff
    s16 timer;
    s16 rdiv;
    
    //misc
    MMU* mmu;
    
    void initCpu();
    s16 getFreq();
    void updateTimer();
    void updateIrq();
    void execIrq(u16 addr);
    
    //opcodes
    void nop();
    void illegal();
    void addReg( u8& reg );
    void addHL();
    void add8();
    void adcReg( u8& reg );
    void adcHL();
    void adc8();
    void subReg( u8& reg );
    void subHL();
    void sub8();
    void sbcReg( u8& reg );
    void sbcHL();
    void sbc8();
    void andReg( u8& reg );
    void andHL();
    void and8();
    void xorReg( u8& reg );
    void xorHL();
    void xor8();
    void orReg( u8& reg );
    void orHL();
    void or8();
    void cpReg( u8& reg );
    void cpHL();
    void cp8();
    void incReg( u8& reg );
    void incHL();
    void decReg( u8& reg );
    void decHL();
    void daa();
    void cpl();
    void addReg16( u16& reg );
    void incReg16( u16& reg );
    void decReg16( u16& reg );
    void addSP();
    void ldHLSP();
    void loadRR( u8& dst, u8& src );
    void loadRHL( u8& dst );
    void loadHLR( u8& src );
    void load8( u8& reg );
    void loadHL8();
    void loadBC();
    void loadDE();
    void loadNN();
    void loadBC_A();
    void loadDE_A();
    void loadNN_A();
    void loadIOPortN();
    void loadIOPortN_A();
    void loadIOPortC();
    void loadIOPortC_A();
    void loadHLInc_A();
    void loadHLInc();
    void loadHLDec_A();
    void loadHLDec();
    void load16( u16& reg );
    void loadSPHL();
    void push( u16& reg );
    void pop( u16& reg );
    void popAF();
    void loadNN_SP();
    void jumpNN();
    void jumpHL();
    void jumpCNN( u8 opcode );
    void jumpR();
    void jumpRC( u8 opcode );
    void callNN();
    void callCNN( u8 opcode );
    void ret();
    void retc( u8 opcode );
    void reti();
    void ccf();
    void scf();
    void halt();
    void stop();
    void di();
    void ei();
    void rst( u8 opcode );
    void bit( const u8 bit, u8& reg );
    void bitHL( const u8 bit );
    void res( const u8 bit, u8& reg );
    void resHL( const u8 bit );
    void set( const u8 bit, u8& reg );
    void setHL( const u8 bit );
    void rlca();
    void rla();
    void rrca();
    void rra();
    void rlc( u8& reg );
    void rlcHL();
    void rl( u8& reg );
    void rlHL();
    void rrc( u8& reg );
    void rrcHL();
    void rr( u8& reg );
    void rrHL();
    void sla( u8& reg );
    void slaHL();
    void swap( u8& reg );
    void swapHL();
    void sra( u8& reg );
    void sraHL();
    void srl( u8& reg );
    void srlHL();
    
    u8 rol8(u8 x, u8 i);
    u8 ror8(u8 x, u8 i);
};

#endif // CPU_H
