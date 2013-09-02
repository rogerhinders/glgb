#ifndef MMU_H
#define MMU_H

#include "types.h"
#include "cart.h"
#include "input.h"
#include "cpu.h"


#define MEM_SIZE 0x10000

class Input;
class CPU;

class MMU {

public:
    u8 mem[MEM_SIZE];
    MMU(Cart* cartridge);
    ~MMU();
    void setInput(Input* keys);
    void setCPU(CPU* cpu);
    void initMem();
    void memWrite8( u16 addr, u8 value );
    void memWrite16( u16 addr, u16 value );
    u8 memRead8( u16 addr );
    u16 memRead16( u16 addr );
    void printMem( u16 start, u16 end );
private:
    Cart* cart;
    Input* keys;
    CPU* cpu;
    
    u16 mbc1RomBank;
    u16 mbc1RamBank;
    
    void dma();
};

#endif // MMU_H
