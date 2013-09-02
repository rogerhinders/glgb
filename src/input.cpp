#include "input.h"
#include "regs.h"

Input::Input(MMU* mmu) {
    this->mmu = mmu;
    mmu->mem[REG_JOYP] = 0xFF;
    keysDir = 0xf;
    keysBtn = 0xf;
}

void Input::setKey(u8 key,u8 event) {
    if(event==GB_KEY_EVENT_RELEASE) {
        switch(key) {
            case GB_KEY_RIGHT:
            case GB_KEY_LEFT:
            case GB_KEY_UP:
            case GB_KEY_DOWN:
                keysDir = keysDir|key;
            break;
            case GB_KEY_A:
            case GB_KEY_B:
            case GB_KEY_SELECT:
            case GB_KEY_START:
                keysBtn = keysBtn|(key>>4);
            break;

        }
    }
    if(event==GB_KEY_EVENT_PRESS) {
        switch(key) {
            case GB_KEY_RIGHT:
            case GB_KEY_LEFT:
            case GB_KEY_UP:
            case GB_KEY_DOWN:
                keysDir = keysDir&(0xF^key);
            break;
            case GB_KEY_A:
            case GB_KEY_B:
            case GB_KEY_SELECT:
            case GB_KEY_START:
                keysBtn = keysBtn&(0xF^(key>>4));
            break;

        }
    }
}
void Input::updateKeyReg(u8 val) {
    mmu->mem[REG_JOYP] = 0xFF;
    if((val&GB_KEY_MODE_DIRECTION)==0) {
        mmu->mem[REG_JOYP] = 0xE0|keysDir;
    }
    if((val&GB_KEY_MODE_BUTTON)==0) {
        mmu->mem[REG_JOYP] = 0xD0|keysBtn;
    }
}