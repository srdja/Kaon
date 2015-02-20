#ifndef __IMAGE_H__
#define __IMAGE_H__

typedef struct image_s {
    char *name;
    SDL_Surface *surface;
} Image;

void         image_init ();
SDL_Surface *image_load (char *resource);

#endif
