/*
 *
 *
 *
 */



#include <SDL2/SDL.h>
#include <vector.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <hashtable.h>

#include "input.h"

static void process_key_event   (InputContext *context);
static void process_mouse_event (InputContext *context);

static Vector *input_contexts;
static Vector *updated_button_maps;
static Vector *updated_range_maps;
static Vector *keys_pressed_down;


/**
 * ADD a binding context
 *
 * a binding context registers all the keys presses until return is pressed
 *
 */

void
input_init()
{
    input_contexts      = vector_new();
    updated_button_maps = vector_new();
    updated_range_maps  = vector_new();
    keys_pressed_down   = vector_new();
}

void
input_destroy()
{
   // the vector should accept a callback for destroy
    vector_destroy_free(input_contexts);
}


void
input_add_context(InputContext *c)
{
    vector_add(input_contexts, c);
}

/** ----------------------------------------------------------------------------
 * @brief input_get_context
 * @param context_name
 * @return
 -----------------------------------------------------------------------------*/

InputContext *
input_get_context (char *context_name)
{
    int i;
    for (i = 0; i < vector_size(input_contexts); i++) {
        InputContext *cont = vector_get(input_contexts, i);

        if (strcmp(cont->name, context_name) == 0) {
            return cont;
        }
    }
    return NULL;
}

static SDL_Event event;

/** ----------------------------------------------------------------------------
 * Polls SDL events and dipatches them to active contexts.
 *
 * @brief input_poll
 -----------------------------------------------------------------------------*/
void
input_poll()
{
    size_t i;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_WINDOWEVENT:
            break;
        case SDL_QUIT:
            SDL_Quit();
            exit(0);
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            break; //
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            for (i = 0; i < vector_size(input_contexts); i++)
                process_key_event(vector_get(input_contexts, i));
            break;
        case SDL_MOUSEMOTION:
            break;
        case SDL_MOUSEWHEEL:
            break;
        default:
            break;
        }
    }
}

/** ----------------------------------------------------------------------------
 *
 *
 * @brief process_key_event
 * @param context
 -----------------------------------------------------------------------------*/
static void
process_key_event(InputContext *context)
{
    uint8_t  repeat = event.key.repeat;
    uint16_t mod    = event.key.keysym.mod;
    uint32_t key;


    if (context->use_key_scan_codes)
        key = event.key.keysym.scancode;
    else
        key = event.key.keysym.sym;

    ButtonMap *key_map = icontext_get_key_map(context, key, mod);

    if (key_map) {
        key_map->key_state = event.key.state;
        key_map->last_ts   = event.key.timestamp;
        vector_add(updated_button_maps, key_map);
    }

    int i;
    for (i = 0; i < vector_size(updated_button_maps); i++) {
        ButtonMap *map = vector_get(updated_button_maps, i);

        if (map->key_state == SDL_RELEASED) {
            if (map->invoke_on_release)
                map->action->callback_release();

            map->combo_was_down = true;
        } else {
            if (!repeat || (repeat && map->invoke_on_repeat))
                map->action->callback();          
        }                
    }

    vector_remove_all(updated_button_maps);
}

/** ----------------------------------------------------------------------------
 *
 *
 * @brief process_mouse_event
 * @param context
 -----------------------------------------------------------------------------*/
static void
process_mouse_event(InputContext *context)
{

}
