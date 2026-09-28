#ifndef BLOCK_BATTLE_SCREEN_H
#define BLOCK_BATTLE_SCREEN_H

#include <tamtypes.h>
#include <gsKit.h>

#define SCREEN_WIDTH 5
#define SCREEN_HEIGHT 22

u8 getBlock(u8 row, u8 column);
void setBlock(u8 value, u8 row, u8 column);
void clearBlocks(void);
void renderScreen(GSGLOBAL *gs);

#endif