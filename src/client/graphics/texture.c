#include <stdlib.h>
#include <stdio.h>

#include "texture.h"
#include "image.h"

/**
 *
 *  CACHING TEXTURES IS SUPER IMPORTANT
 *
 */

static void gen_subtex_coords   (GLfloat *coords, int subtex);

Texture *
texture_new_img(char *res)
{
    Texture *t = calloc(1, sizeof(Texture));
    texture_set_img(t, res);
    return t;
}

void
texture_set_img(Texture *texture, char *res)
{
    SDL_Surface *s = image_load(res);
    texture_set(texture, s, GL_LINEAR, GL_CLAMP_TO_EDGE);
    //SDL_FreeSurface(s);
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
Texture *
texture_new(SDL_Surface *s, GLint filter, GLint wrap)
{
    Texture *texture = calloc(1, sizeof(Texture));
    texture_set(texture, s, filter, wrap);
    return texture;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 ******************************************************************************/
void
texture_set(Texture *texture, SDL_Surface *s, GLint filter, GLint wrap)
{
    /* XXX This assumes that the textures are squares */
    int subtex = s->w / s->h;

    size_t   chunk  = sizeof(float) * 8 * subtex;
    GLfloat *coords = calloc(1, chunk);

    gen_subtex_coords(coords, subtex);

    GLuint texid = 0;

    glGenTextures(1, &texid);

    texture->subtex_n = subtex;
    texture->subtex_i = 0;
    texture->gl_id    = texid;

    glBindTexture(GL_TEXTURE_2D, texture->gl_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 s->w,
                 s->h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 s->pixels);

    vbuffer_new_fill(&(texture->coord_buff), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vbuffer_allocate(&(texture->coord_buff), coords, chunk);
}

/*******************************************************************************
 *
 * @param[in]
 ******************************************************************************/
void
texture_destroy(Texture *texture)
{

}

/*******************************************************************************
 *
 * @param[in] texture
 * @param[in] subtex the number of sub-textures
 ******************************************************************************/
static void
gen_subtex_coords(GLfloat *coords, int subtex)
{
    GLfloat sub = 1.0f / subtex;

    int i;
    for (i = 0; i < subtex; i++) {
        coords[(i * 8) + 0] = sub * i;
        coords[(i * 8) + 1] = 1;
        coords[(i * 8) + 2] = sub * i + sub;
        coords[(i * 8) + 3] = 1;
        coords[(i * 8) + 4] = sub * i + sub;
        coords[(i * 8) + 5] = 0;
        coords[(i * 8) + 6] = sub * i;
        coords[(i * 8) + 7] = 0;
    }
}

