#ifndef Piece_BATTLE_SCREEN_H
#define Piece_BATTLE_SCREEN_H

#include <tamtypes.h>
#include <gsKit.h>
#include "block_battle_screen.h"
u8 getPiecePositionRow();
u8 getPiecePositionColumn();
void setPiecePositionRow(u8 value);
void setPiecePositionColumn(u8 value);

void createPiece(u8 id);
void drawPiece(u8 row, u8 column);
void erasePiece(u8 row, u8 column);
void movePieceDown();
void movePieceBottom();
void Land();
void CheckDownwardCollission();


#endif