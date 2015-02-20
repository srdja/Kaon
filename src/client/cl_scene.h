#include <vector.h>

#include "graphics/model.h"
#include "../common/entity/entity.h"



typedef struct cl_scene_s {
    int static_ent_count;

    // actors
    // triggers
    // static
    Vector *static_ent;
    // lights

    //UI_Stack ui;
} CLScene_S;

void scene_update();
CLScene_S *new_scene();
