#include <SDL2/SDL.h>
#include <stdio.h>

#include "input.h"
#include "input_context.h"
#include "input_bindings.h"

void test()
{
    fprintf(stdout, "TEST\n");
}

void input_bindings_init()
{
    char *name = "test";

    // separate mouse and key contexts
    InputContext *cont = icontext_new(name, true, 1, true, false);

    ButtonAction *move_right = calloc(1, sizeof(ButtonAction));
    move_right->callback = test;
    move_right->callback_release = test;

    ButtonMap *map1 = button_map_new(SDL_SCANCODE_D, KMOD_NONE, true, false, move_right);

    icontext_add_key_map(cont, map1);

    input_add_context(cont);
}
