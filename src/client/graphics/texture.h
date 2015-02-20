#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <hashtable.h>

#include "vbuffer.h"

typedef struct texture_s {
    int       widht_pix;
    int       height_pix;

    VBuffer   coord_buff;
    GLint     gl_id;

    GLint     subtex_n; // number of subtextures
    GLint     subtex_i; // current subtexture;
} Texture;

struct texture_properties {
   // HashTable
};

Texture *texture_new          (SDL_Surface *s, GLint filter, GLint wrap);
Texture *texture_new_img      (char *res);

void     texture_set          (Texture *tex, SDL_Surface *s, GLint f, GLint wrap);
void     texture_set_img      (Texture *tex, char *res);
void     texture_destroy      (Texture *tex);

void     texture_bind_sampler (Texture *texture, GLint sampler);

void     texture_bind         (Texture *tex);
void     texture_unbind       (Texture *tex);

#endif /*TEXTURE_H*/
