#include <stddef.h>
#include <sifrpc.h>
#include <loadfile.h>
#include "totrus_input.h"



unsigned char padBuf[256] __attribute__((aligned(64)));

int iniciarMando(void)
{
    SifInitRpc(0);
    SifLoadModule("rom0:XSIO2MAN", 0, NULL);
    SifLoadModule("rom0:XPADMAN", 0, NULL);

    if(padInit(0) != 1) return 0;
    if(padPortOpen(0, 0, padBuf) == 0) return 0;

    return 1;
}

u32 leerMando(void)
{
    struct padButtonStatus botones;

    if(padGetState(0, 0) != PAD_STATE_STABLE) return 0;
    if(!padRead(0, 0, &botones)) return 0;

    return 0xFFFF ^ botones.btns;
}