#ifndef Piece_BATTLE_SCREEN_H
#define Piece_BATTLE_SCREEN_H

#include <tamtypes.h>
#include <gsKit.h>
#include "block_battle_screen.h"


void createPiece(s8 id);
void drawPiece(s8 row, s8 column);
void erasePiece(s8 row, s8 column);
void movePiece(s8 vertical_offset, s8 horizontal_offset);
void rotatePiece(s8 direction);
void movePieceBottom();
void Land();
s8 CheckCollission(s8 vertical_offset, s8 horizontal_offset);


#endif