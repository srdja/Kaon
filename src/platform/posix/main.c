#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define EVER (;;)

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

#include "../../server/sv_frame.h"
#include "../../network/net.h"
#include "../../network/stp.h"

int
main(int argc, char **argv)
{
    fprintf(stdout, "initializing screen...\n");
    if (!window_init()) {
        fprintf(stdout, "Error: Unable to create a window! Shutting down...\n");
        exit(1);
    }

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

    STPChannel *chan = stp_channel_open("127.0.0.1", 999);

    //configuring

    fprintf(stdout, "entering game loop\n");

    for EVER {
        sv_frame_update();
        cl_frame_update();
    }
    return 0;
}
