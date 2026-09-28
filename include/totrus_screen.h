#ifndef TOTRUS_SCREEN_H
#define TOTRUS_SCREEN_H

#include <tamtypes.h>
#include <gsKit.h>

#define SCREEN_WIDTH 10
#define SCREEN_HEIGHT 10

u8 getBlock(u8 row, u8 column);
void setBlock(u8 value, u8 row, u8 column);
void clearBlocks(void);
void renderScreen(GSGLOBAL *gs);

#endif