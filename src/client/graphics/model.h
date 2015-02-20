#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>

#include "mesh.h"
#include "texture.h"

typedef struct model_s {
    char *name;

    Mesh mesh;

    int textures_i;
    int textures_n;

    /* array of textures */
    Texture *textures;
} Model;

void model_set(Model *m);
Model *model_new();

void model_destroy(Model *model);

void model_update_animation(Model *model);

#endif
