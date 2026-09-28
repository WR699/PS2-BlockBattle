#include <stddef.h>
#include <sifrpc.h>
#include <loadfile.h>
#include "block_battle_input.h"

static unsigned char padBuf[256] __attribute__((aligned(64)));

int iniciarMando(void)
{
    SifInitRpc(0);

    SifLoadModule("rom0:SIO2MAN", 0, NULL);
    SifLoadModule("rom0:PADMAN", 0, NULL);

    if(padInit(0) != 1) return 0;
    if(padPortOpen(0, 0, padBuf) == 0) return 0;

    return 1;
}

u32 leerMando(void)
{
    struct padButtonStatus botones;
    int estado = padGetState(0, 0);

    if(estado != PAD_STATE_STABLE && estado != PAD_STATE_FINDCTP1)
        return 0;

    if(!padRead(0, 0, &botones))
        return 0;

    return ((u32)(~botones.btns)) & 0xFFFF;
}