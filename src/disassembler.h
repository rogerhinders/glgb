#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "types.h"
#include "cpu.h"

class Disassembler {

public:
    Disassembler();
    static u8 disassemble(u32 opcode, char *opstr, CPU* cpu);
private:
    static void disassembleCB(u8 opcode, char *opstr);
};

#endif // DISASSEMBLER_H

