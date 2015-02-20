#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "image.h"
#include "../../file-system/paths.h"
#include "../console.h"

/*******************************************************************************
 * Initialize the image module.
 ******************************************************************************/
void image_init()
{
    int flags  = IMG_INIT_PNG | IMG_INIT_JPG;
    int iflags = IMG_Init(flags);

    if ((flags & iflags) != flags) {
        console_msg_post("Error, unable to initialize image format support: %s",
                IMG_GetError());
    }
}

/** ----------------------------------------------------------------------------
 *
 *
 * @brief image_load
 * @param texture
 * @return
 -----------------------------------------------------------------------------*/
SDL_Surface *image_load(char *texture)
{
    char *tex_p = paths_texture(texture);
    SDL_Surface *img = IMG_Load(tex_p);
    if (!img) {
        fprintf(stdout, "img %s not loaded\n", tex_p);
    }
    free(tex_p);

    return img;
}
