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
    {0,0,0,0,0,0,0,0,245,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {15,2,0,244,0,0,0,0,0,241},
};

static u64 red   = GS_SETREG_RGBAQ(0xFF, 0x06, 0x06, 0x80, 0x00);
static u64 green = GS_SETREG_RGBAQ(0x06, 0xFF, 0x06, 0x80, 0x00);
static u64 blue = GS_SETREG_RGBAQ(0x06, 0x06, 0xFF, 0x80, 0x00);
static u64 yellow = GS_SETREG_RGBAQ(0xFF, 0xD0, 0x00, 0x80, 0x00);
static u64 orange = GS_SETREG_RGBAQ(0xFF, 0xB0, 0x00, 0x80, 0x00);
static u64 cyan    = GS_SETREG_RGBAQ(0x00, 0x00, 0x00, 0x80, 0x00);
static u64 black  = GS_SETREG_RGBAQ(0x00, 0xC0, 0xFF, 0x80, 0x00);





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

    u64 color = GS_SETREG_RGBAQ(0x00, 0xC0, 0xFF, 0x80, 0x00);

    u8 column, row;

    for(column = 0; column < SCREEN_WIDTH; column++){
        for(row = 0; row < SCREEN_HEIGHT; row++){
            u8 blocks = getBlock(row, column);
            float x,y;
            u8 momentary;
            if ((momentary = (blocks & 0xF0)) != 0 )
            {
                switch (momentary - 240){
                case 1: color = red; break;
                case 2: color = blue; break;
                case 3: color = green; break;
                case 4: color = cyan; break;
                case 5: color = yellow; break;
                default: color = orange; break;
                };
                x = startX + column * blockSize;
                y = startY + row * 2 * blockSize;
                gsKit_prim_sprite(gs, x + gap, y + gap, x + blockSize - gap, y + blockSize - gap, 1, color);
               
            }  
            if ((momentary = (blocks & 0x0F)) != 0)
            {
                switch (momentary){
                case 1: color = red; break;
                case 2: color = blue; break;
                case 3: color = green; break;
                case 4: color = cyan; break;
                case 5: color = yellow; break;
                default: color = orange; break;
                };
                x = startX + column * blockSize;
                y = startY + ((row * 2) + 1) * blockSize;
                gsKit_prim_sprite(gs, x + gap, y + gap, x + blockSize - gap, y + blockSize - gap, 1, color);
                
            }
                      
        }
    }
    
};