#include <vector.h>

#include "cl_scene.h"


CLScene_S *new_scene()
{
    CLScene_S *scene = calloc(1, sizeof(CLScene_S));

    scene->static_ent = vector_new();

    return scene;
}

void *cl_scene_update(CLScene_S *scene)
{
    return NULL; // TODO
}
