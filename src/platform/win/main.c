#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define EVER (;1;)

#include "../../client/graphics/window.h"
#include "../../client/graphics/shaders.h"
#include "../../client/graphics/image.h"

#include "../../client/input/input.h"
#include "../../client/input/input_bindings.h"
#include "../../client/cl_frame.h"
#include "../../file-system/paths.h"

#include "../../common/map/map.h"
#include "../../common/script/script.h"
#include "../../client/audio/sound.h"

#include "../../network/net.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    fprintf(stdout, "initializing screen\n");
    window_init();

    fprintf(stdout, "initializing paths\n");
    paths_init();

    fprintf(stdout, "initializing input\n");
    input_init();

    fprintf(stdout, "initializing sound\n");
    sound_init();

    fprintf(stdout, "initializing images\n");
    image_init();

    fprintf(stdout, "initializing shaders\n");
    shaders_init();

    fprintf(stdout, "initializing scripts\n");
    script_init();

    fprintf(stdout, "initializing client frame\n");
    cl_frame_init();

    fprintf(stdout, "initializing networking\n");
    net_init();

    // configuring

    while (1) {
        cl_frame_update();
    }
    return 0;
}
