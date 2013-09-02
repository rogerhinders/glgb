#ifndef GPU_H
#define GPU_H

#define SCREEN_W 160
#define SCREEN_H 144

#define BG_W 256
#define BG_H 256

#define WND_W 256
#define WND_H 256

#define NUM_SPRITES 40

#define MODE_HBL 0
#define MODE_VBL 1
#define MODE_R_OAM 2
#define MODE_R_VRAM 3

#define LCDC_BG_ENABLE 0x1
#define LCDC_OBJ_ENABLE 0x2
#define LCDC_BG_TILE_MAP 0x8
#define LCDC_BG_WND_TILES 0x10
#define LCDC_WND_ENABLE 0x20
#define LCDC_WND_TILE_MAP 0x40
#define LCDC_DISPLAY_ENABLE 0x80

#define ADDR_TILEDATA_0 0x8800
#define ADDR_TILEDATA_1 0x8000

#include <vector>
#include "mmu.h"
#include "cpu.h"
#include "types.h"
#include "oam.h"

class GPU {
public:
    
    GPU( MMU* mmu, CPU* cpu, u32* screen );
    ~GPU();
    u8 update();
    static const u32 lcdShades[];
    u8 line;
private:
    u8 mode;
    u16 mclock;
    MMU* mmu;
    CPU* cpu;
    u32* screen;
    u32 tempLineBG[BG_W];
    u32 tempLineWND[WND_W];
    std::vector<OAM> oam;
    
    
    static const u16 tileDataLoc[];
    
    void initGPU();
    void updateStat();
    void drawLine(u32 *screen);
    void fillBGLine();
    void fillWNDLine();
};

#endif // GPU_H
