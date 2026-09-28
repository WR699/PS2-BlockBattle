#include "totrus_screen.h"
#include <debug.h>

//0 = both empty, 240 = top full bottom empty; 15 = bottom full top empty; 255 both full


//         column row
u8 screen [10][10] = {
    {255,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {15,0,0,0,0,0,0,0,0,240},
};

//negro    = GS_SETREG_RGBAQ(0x00, 0x00, 0x00, 0x80, 0x00);
//celeste  = GS_SETREG_RGBAQ(0x00, 0xC0, 0xFF, 0x80, 0x00);



u8 getBlock(u8 row, u8 column){
    if(row >= SCREEN_HEIGHT || column >= SCREEN_WIDTH) return 0;
    return screen[column][row];
}

void setBlock(u8 value, u8 row, u8 column){
    if(row >= SCREEN_HEIGHT || column >= SCREEN_WIDTH) return;
    screen[column][row] = value;
}

void clearBlocks(void){
    u8 column, row;
    for(column = 0; column < SCREEN_WIDTH; column++)
        for(row = 0; row < SCREEN_HEIGHT; row++)
            screen[column][row] = 0;
}

void renderScreen(GSGLOBAL *gs){
 
    const float blockSize = 15.0f;
    const float gap = 2.0f;

    const float width = SCREEN_WIDTH * blockSize;
    const float height = SCREEN_HEIGHT * 2 * blockSize;

    const float startX = (gs->Width - width) * 0.5f;
    const float startY = (gs->Height - height) * 0.5f;

    const u64 cyan = GS_SETREG_RGBAQ(0x00, 0xC0, 0xFF, 0x80, 0x00);

    u8 column, row;

    for(column = 0; column < SCREEN_WIDTH; column++){
        for(row = 0; row < SCREEN_HEIGHT; row++){
            u8 blocks = getBlock(row, column);
            float x,y;
            if ((blocks & 0xF0) != 0)
            {
                x = startX + column * blockSize;
                y = startY + row * 2 * blockSize;
                gsKit_prim_sprite(gs, x + gap, y + gap, x + blockSize - gap, y + blockSize - gap, 1, cyan);
               
            }  
            if ((blocks & 0x0F) != 0)
            {
                x = startX + column * blockSize;
                y = startY + ((row * 2) + 1) * blockSize;
                gsKit_prim_sprite(gs, x + gap, y + gap, x + blockSize - gap, y + blockSize - gap, 1, cyan);
                
            }
                      
        }
    }
    
};