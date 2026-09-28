#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>
#include <gsKit.h>
#include <dmaKit.h>

#include "totrus_input.h"




int main(int argc, char *argv[])
{
    GSGLOBAL *gs;
    u64 negro, celeste, amarillo;
    float x, y;
    const float tam = 48.0f;
    const float velocidad = 4.0f;
    iniciarMando();

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

    x = (gs->Width - tam) * 0.5f;
    y = (gs->Height - tam) * 0.5f;

    while(1)
    {
        u32 mando = leerMando();

        if(mando & PAD_LEFT)  x -= velocidad;
        if(mando & PAD_RIGHT) x += velocidad;
        if(mando & PAD_UP)    y -= velocidad;
        if(mando & PAD_DOWN)  y += velocidad;

        if(mando & PAD_START) {
            x = (gs->Width - tam) * 0.5f;
            y = (gs->Height - tam) * 0.5f;
        }

        if(x < 0) x = 0;
        if(y < 0) y = 0;
        if(x > gs->Width - tam) x = gs->Width - tam;
        if(y > gs->Height - tam) y = gs->Height - tam;

        gsKit_clear(gs, negro);

        gsKit_prim_sprite(
            gs,
            x, y,
            x + tam, y + tam,
            1,
            (mando & PAD_CROSS) ? amarillo : celeste
        );

        gsKit_queue_exec(gs);
        gsKit_sync_flip(gs);
    }

    return 0;
}