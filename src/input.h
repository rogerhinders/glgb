#ifndef INPUT_H
#define INPUT_H

#include "mmu.h"

#define GB_KEY_RIGHT  0x1
#define GB_KEY_LEFT   0x2
#define GB_KEY_UP     0x4
#define GB_KEY_DOWN   0x8

#define GB_KEY_A      0x10
#define GB_KEY_B      0x20
#define GB_KEY_SELECT 0x40
#define GB_KEY_START  0x80

#define GB_KEY_MODE_DIRECTION 0x10
#define GB_KEY_MODE_BUTTON 0x20

#define GB_KEY_EVENT_PRESS 0
#define GB_KEY_EVENT_RELEASE 1

class MMU;
class Input {

public:
    Input(MMU* mmu);
    void setKey(u8 key,u8 event);
    void updateKeyReg(u8 val);
private:
    MMU* mmu;
    u8 keysDir;
    u8 keysBtn;
};

#endif // INPUT_H
