#include "block_battle_piece.h"

u16 current_piece = 0b0110011000100000;

u8 piece_position[2] = {0,0};

u8 getPiecePositionRow(){
  return piece_position[0];
}
u8 getPiecePositionColumn(){
  return piece_position[1];
}
void setPiecePositionRow(u8 value){
  piece_position[0] = value;
}
void setPiecePositionColumn(u8 value){
  if (value >= 10) value = 9;
  piece_position[1] = value;
}

u8 piece_color = 1;
void createPiece(u8 id){
    drawPiece(piece_position[0], piece_position[1]);
};

void drawPiece(u8 row, u8 column){

    for (u8 r = 0; r < 4; r++){
        for (u8 c = 0; c < 2; c++){
          u8 pair = (current_piece >> (14 - (r*4 + c*2))) & 0b11;
          u8 byteColumn = (column >> 1) + c;
          if (pair){
            u8 value = 0xFF;
            u8 color = 0;
            if ((column & 0x01) == 0){
              if(pair & 0b10){ value &= 0x0F; color +=  (piece_color << 4);}
              if(pair & 0b01){ value &= 0xF0; color +=  piece_color;}
              setBlock((getBlock(r + row, byteColumn) & value) | color, r + row, byteColumn);
              continue;
            }

            if(pair & 0b10){ value = 0xF0;  setBlock((getBlock(r + row, byteColumn) & (value) | piece_color), r + row, byteColumn);}
            if(pair & 0b01) { value = 0x0F;  setBlock((getBlock(r + row, byteColumn+1) & (value) | piece_color << 4), r + row, byteColumn+1);}
            
          
          }  
        }
    }
};
void erasePiece(u8 row, u8 column){

    for (u8 r = 0; r < 4; r++){
        for (u8 c = 0; c < 2; c++){
          u8 pair = (current_piece >> (14 - (r*4 + c*2))) & 0b11;
          u8 byteColumn = (column >> 1) + c;
          if (pair){
            u8 value = 0xFF;
            u8 color = 0;
            if ((column & 0x01) == 0){
              if(pair & 0b10){ value &= 0x0F; }
              if(pair & 0b01){ value &= 0xF0;}
              setBlock((getBlock(r + row, byteColumn) & value), r + row, byteColumn);
              continue;
            }

            if(pair & 0b10){ value = 0xF0;  setBlock(getBlock(r + row, byteColumn) & (value), r + row, byteColumn);}
            if(pair & 0b01) { value = 0x0F;  setBlock(getBlock(r + row, byteColumn+1) & (value), r + row, byteColumn+1);}
            
          
          }  
        }
    }
};


void movePieceDown(){
    erasePiece(piece_position[0], piece_position[1]);
    piece_position[0]++;
    drawPiece(piece_position[0], piece_position[1]);
};
void movePieceBottom(){

};
void Land(){

};
void CheckDownwardCollission(){

}; 