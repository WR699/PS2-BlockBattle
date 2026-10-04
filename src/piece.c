#include "block_battle_piece.h"

u16 current_piece = 0b0110011000100000;

s8 piece_position[2] = {0,0};

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
void createPiece(s8 id){
  piece_color = id;
  piece_position[0] = 0;
  piece_position[1] = 4;
  switch (id){
    case 0: current_piece = 0b0000011001100010; break; //square piece with knob
    case 1: current_piece = 0b0100011000100000; break; //s piece
    case 2: current_piece = 0b0000111101100000; break; //fat t piece
    case 3: current_piece = 0b0000011001000000; break; //short l piece
    case 4: current_piece = 0b0000001001100000; break; //short l piece mirrored
  }
  drawPiece(piece_position[0], piece_position[1]);
};

void drawPiece(s8 row, s8 column){

    for (s8 r = 0; r < 4; r++){
        for (s8 c = 0; c < 2; c++){
          u8 pair = (current_piece >> (14 - (r*4 + c*2))) & 0b11;
          s8 byteColumn = (column >> 1) + c;
          if (pair){
            u8 value = 0xFF;
            u8 color = 0;
            if ((column & 0x01) == 0){
              if(byteColumn < 0 || byteColumn > 4) continue;
              if(pair & 0b10){ value &= 0x0F; color +=  (piece_color << 4);}
              if(pair & 0b01){ value &= 0xF0; color +=  piece_color;}
              setBlock((getBlock(r + row, byteColumn) & value) | color, r + row, byteColumn);
              continue;
            }
            
            if(pair & 0b10 && byteColumn > -1 && byteColumn < 5){ value = 0xF0;  setBlock((getBlock(r + row, byteColumn) & (value) | piece_color), r + row, byteColumn);}
            if(pair & 0b01 && byteColumn > -2  && byteColumn < 4){ value = 0x0F;  setBlock((getBlock(r + row, byteColumn+1) & (value) | piece_color << 4), r + row, byteColumn+1);}
            
          
          }  
        }
    }
};
void erasePiece(s8 row, s8 column){

    for (s8 r = 0; r < 4; r++){
        for (s8 c = 0; c < 2; c++){
          u8 pair = (current_piece >> (14 - (r*4 + c*2))) & 0b11;
          s8 byteColumn = (column >> 1) + c;
          if (pair){
            u8 value = 0xFF;
            u8 color = 0;
            if ((column & 0x01) == 0){
              if(byteColumn < 0 || byteColumn > 4) continue;
              if(pair & 0b10){ value &= 0x0F; }
              if(pair & 0b01){ value &= 0xF0;}
              setBlock((getBlock(r + row, byteColumn) & value), r + row, byteColumn);
              continue;
            }

            if(pair & 0b10 && byteColumn > -1 && byteColumn < 5){ value = 0xF0;  setBlock(getBlock(r + row, byteColumn) & (value), r + row, byteColumn);}
            if(pair & 0b01&& byteColumn > -2  && byteColumn < 4) { value = 0x0F;  setBlock(getBlock(r + row, byteColumn+1) & (value), r + row, byteColumn+1);}
            
          
          }  
        }
    }
};


void movePiece(s8 vertical_offset, s8 horizontal_offset){
  s8 collision = CheckCollission(vertical_offset, horizontal_offset);
  if (collision == 0){
      erasePiece(piece_position[0], piece_position[1]);
      piece_position[0] += vertical_offset;
      piece_position[1] += horizontal_offset;
      drawPiece(piece_position[0], piece_position[1]);
  }
  else if (collision == 1 && vertical_offset > 0){
    Land();
  }
};
void movePieceBottom(){

};
void Land(){
  createPiece(3);

};
s8 CheckCollission(s8 vertical_offset, s8 horizontal_offset){
  erasePiece(piece_position[0], piece_position[1]);
  s8 answer = 0;
  s8 new_pos[2] = {piece_position[0] + vertical_offset, piece_position[1] + horizontal_offset};
  u8 block_to_check;
  for (s8 r = 0; r < 4; r++){ //cada fila de la pieza
      
      for (s8 c = 0; c < 2; c++){ //cada columna de la pieza
        u8 pair = (current_piece >> (14 - (r*4 + c*2))) & 0b11;
        s8 byteColumn = (new_pos[1] >> 1) + c; 
        if (pair){
            if (new_pos[0] + r > 21){answer = 1; break;}


            if ((new_pos[1] & 0x01) == 0){
              if(byteColumn < 0 || byteColumn > 4){answer = 1; break;}
              block_to_check = getBlock(new_pos[0] + r, byteColumn);

              if(pair & 0b10){ if ((block_to_check & 0xF0) > 0){answer = 1; break;}}
              if(pair & 0b01){ if ((block_to_check & 0x0F) > 0){answer = 1; break;}}
              continue;
            }
           
            if(pair & 0b10){
              if (byteColumn < 0 || byteColumn > 4){answer = 1; break;}
              block_to_check = getBlock(new_pos[0] + r, byteColumn);
              if ((block_to_check & 0x0F) > 0){answer = 1; break;}}
            

            if(pair & 0b01){
              if (byteColumn < -1 || byteColumn > 3){answer = 1; break;}
              block_to_check = getBlock(new_pos[0] + r, byteColumn + 1);
              if ((block_to_check & 0xF0) > 0){answer = 1; break;}}
            
          }  
        }
    }

  drawPiece(piece_position[0], piece_position[1]);
      
  
  
 
  //*/
  return answer;
}; 