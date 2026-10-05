#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>
#include <gsKit.h>
#include <dmaKit.h>

#include "block_battle_input.h"
#include "block_battle_screen.h"
#include "block_battle_piece.h"


u32 frame_counter;

int main(int argc, char *argv[])
{
    s8 frames_for_input = 10;
    s8 frames_for_next_input = 0;
    u8 last_input = 0;
    frame_counter = 0;
    GSGLOBAL *gs;
    u64 negro, celeste, amarillo, rojo;
    float x, y;
    const float tam = 48.0f;
    const float velocidad = 4.0f;
    int mandoOK = iniciarMando();

    gs = gsKit_init_global();

    dmaKit_init(
        D_CTRL_RELE_OFF,
        D_CTRL_MFD_OFF,
        D_CTRL_STS_UNSPEC,
        D_CTRL_STD_OFF,
        D_CTRL_RCYC_8,
        1 << DMA_CHANNEL_GIF
    );

    dmaKit_chan_init(DMA_CHANNEL_GIF);

    gs->DoubleBuffering = GS_SETTING_ON;
    gs->ZBuffering = GS_SETTING_ON;

    gsKit_vram_clear(gs);
    gsKit_init_screen(gs);
    gsKit_mode_switch(gs, GS_ONESHOT);

    if(gs->ZBuffering == GS_SETTING_ON)
    gsKit_set_test(gs, GS_ZTEST_ON);
    

    negro    = GS_SETREG_RGBAQ(0x00, 0x00, 0x00, 0x80, 0x00);
    celeste  = GS_SETREG_RGBAQ(0x00, 0xC0, 0xFF, 0x80, 0x00);
    amarillo = GS_SETREG_RGBAQ(0xFF, 0xD0, 0x00, 0x80, 0x00);
    rojo     = GS_SETREG_RGBAQ(0xFF, 0x00, 0x00, 0x80, 0x00);


    x = (gs->Width - tam) * 0.5f;
    y = (gs->Height - tam) * 0.5f;
    createPiece(1);
    while(1)
    {
        if(frames_for_next_input <= 0){last_input = 0; frames_for_next_input = 0;}
        u32 mando = leerMando();
        if (frame_counter >= 30){
            frame_counter = 0;
            movePiece(1,0);
        }
        
        if((last_input != 1) && (mando & PAD_LEFT))  {
            last_input = 1;
            frames_for_next_input = frames_for_input;
            movePiece(0, -1);
        }

        if((last_input != 2) && (mando &  PAD_RIGHT)){
            last_input = 2;
            frames_for_next_input = frames_for_input;
            movePiece(0, 1);
        }
        if((last_input != 3) && (mando &  PAD_R1)){
            last_input = 3;
            frames_for_next_input = frames_for_input;
            rotatePiece(-1);
        }  
        if((last_input != 4) && (mando &  PAD_L1)){
            last_input = 4;
            frames_for_next_input = frames_for_input;
            rotatePiece(1);
        }  
        if((last_input != 5) && (mando &  PAD_UP)){
            last_input = 5;
            frames_for_next_input = frames_for_input;
            movePieceBottom();
        }  
        
        
        

       

        if(x < 0) x = 0;
        if(y < 0) y = 0;
        if(x > gs->Width - tam) x = gs->Width - tam;
        if(y > gs->Height - tam) y = gs->Height - tam;

        gsKit_clear(gs, negro);
        renderScreen(gs);

        
        

        

        gsKit_queue_exec(gs);
        gsKit_sync_flip(gs);
        frame_counter ++;
        frames_for_next_input--;
    }

    return 0;
}