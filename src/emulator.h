#ifndef EMULATOR_H
#define EMULATOR_H

#include "cart.h"
#include "cpu.h"
#include "gpu.h"
#include "input.h"
#include "debugger.h"

class Emulator {

public:
    Emulator(u32* screen);
    ~Emulator();
    void run();
    void setKey(u8 key,u8 event);
    void activateDebugger();
private:
    Cart* rom;
    CPU* cpu;
    MMU* mmu;
    GPU* gpu;
    Input* keys;
    Debugger* debugger;
    u32* screen;
    u32 cycles;
    u8 bp; //remove me!
};

#endif // EMULATOR_H
