#include <SDL2/SDL_net.h>

#include "net.h"

void net_init()
{
    if( SDLNet_Init() == -1) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(2);
    }
}

void net_close()
{
    SDLNet_Quit();
}
