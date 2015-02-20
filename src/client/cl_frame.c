#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "cl_frame.h"

#include "graphics/window.h"
#include "graphics/image.h"
#include "graphics/draw.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"
#include "graphics/matrix.h"

#include "input/input.h"
#include "input/input_context.h"

#include "cl_scene.h"
#include "../common/entity/entity.h"
#include "../common/vec2.h"
#include "../common/map/map.h"

#include "../lib/lua/lua.h"
#include "../lib/lua/lauxlib.h"
#include "../lib/lua/lualib.h"

#include "../common/script/script.h"
#include "../common/script/bindings.h" // rename to lua_binding

#include "../file-system/paths.h"
#include "../game/collision/collision.h"

static CLScene_S *active_scene;
static uint32_t   init_time;

static lua_State *game_state;

bool dir_left = false;
bool dir_right = false;
bool dir_up  = false;
bool dir_down = false;

Entity *player;

void left()
{
    dir_left = true;
    dir_right = false;
    entity_on_move(game_state, player, "left");
}

void right()
{
    dir_right = true;
    dir_left = false;
    entity_on_move(game_state, player, "right");
}

void up()
{
    dir_up = true;
    dir_down = false;
    entity_on_move(game_state, player, "up");
}

void down()
{
    dir_down = true;
    dir_up = false;
    entity_on_move(game_state, player, "down");
}

void station_left()
{
    dir_left = false;
    entity_on_move(game_state, player, "stat");
}

void station_right()
{
    dir_right = false;
    entity_on_move(game_state, player, "stat");
}

void station_up()
{
    dir_up = false;
    entity_on_move(game_state, player, "stat");
}

void station_down()
{
    dir_down = false;
    entity_on_move(game_state, player, "stat");
}

void init_bindings()
{
    char *name = "test";

    // separate mouse and key contexts
    InputContext *cont = icontext_new(name, true, 1, false, false);

    ButtonAction *move_left = calloc(1, sizeof(ButtonAction));
    move_left->callback = left;
    move_left->callback_release = station_left;

    ButtonAction *move_right = calloc(1, sizeof(ButtonAction));
    move_right->callback = right;
    move_right->callback_release = station_right;

    ButtonAction *move_up = calloc(1, sizeof(ButtonAction));
    move_up->callback = up;
    move_up->callback_release = station_up;

    ButtonAction *move_down = calloc(1, sizeof(ButtonAction));
    move_down->callback = down;
    move_down->callback_release = station_down;

    ButtonMap *map1 = button_map_new(SDL_SCANCODE_D, KMOD_NONE, true, false, move_right);
    ButtonMap *map2 = button_map_new(SDL_SCANCODE_A, KMOD_NONE, true, false, move_left);
    ButtonMap *map3 = button_map_new(SDL_SCANCODE_W, KMOD_NONE, true, false, move_up);
    ButtonMap *map4 = button_map_new(SDL_SCANCODE_S, KMOD_NONE, true, false, move_down);

    icontext_add_key_map(cont, map1);
    icontext_add_key_map(cont, map2);
    icontext_add_key_map(cont, map3);
    icontext_add_key_map(cont, map4);

    input_add_context(cont);
}

static Entity *
new_entity(char *image, int x, int y)
{
    Entity *ent = calloc(1, sizeof(Entity));

    ent->texture_n = 1;
    ent->texture_i = 0;

    Texture *t = calloc(5, sizeof(Texture));
    ent->textures = t;

    mesh_set_rectangle(&(ent->mesh), 1, 1);
    texture_set_img(&(ent->textures[0]), image);

    Vec2 pos;
    pos.x = 120 * x - 60;
    pos.y = 120 * y + 60;
    entity_update_position(ent, &pos);

    return ent;
}

void
init_lua_ent_vm()
{
    game_state = luaL_newstate();
    luaL_openlibs(game_state);


    bindings_bind(game_state);
}


void cl_frame_init()
{
    fprintf(stdout, "initializing bindings\n");
    init_bindings();

    render_init(screen_get_sdl_window(), NULL);

    init_lua_ent_vm();

    char *script = paths_script_entities("Player.lua");
    script_load(game_state, script);

    Entity *entity_test = entity_new(game_state, "Player", NULL);
    entity_on_spawn(entity_test, game_state);

    Vec2 pos;
    pos.x = 120 * 5 - 60;
    pos.y = 120 * 5 + 60;
    entity_update_position(entity_test, &pos);

    active_scene = new_scene();

    MapInfo *map = load_map();


    int count = 0;
    int i;
    int x = 1;
    int y = 1;

    for (i = 0; i < map->tiles_height * map->tiles_width; i++) {
        int id = map->data[i];

        if (id != 0 && id != 39 && id != -1) {
            char *name = hashtable_get(map->tile_info, id);
            // y is inverted to match the opengl coordinate system
            Entity *e = new_entity(name, x, map->tiles_height - y);
            vector_add(active_scene->static_ent, e);
            count++;           
        }

        if ((i+1) % map->tiles_width == 0) {
            y++;
            x = 0;
        }
        x++;
    }
    vector_add(active_scene->static_ent, entity_test);
    count++;
    player = entity_test;
    active_scene->static_ent_count = count;

    init_time = SDL_GetTicks();
}

bool collide()
{
    int i;
    for (i = 0; i < active_scene->static_ent_count; i++) {
        Entity *e = vector_get(active_scene->static_ent, i);
        if (e != player && collision_AABB(&(e->mesh), &(player->mesh))) {
            return true;
        }
    }
    return false;
}

void cl_poll_cmd()
{

}

void cl_process_cmd()
{

}


void cl_frame_update()
{
    uint32_t time_delta = SDL_GetTicks() - init_time;

    //int game_ticks;

    input_poll();

    // process network events
    int i;
    for (i = 0; i < active_scene->static_ent_count; i++) {
        Entity *e = vector_get(active_scene->static_ent, i);
        entity_update(e, time_delta);
    }

    Vec2 pos = player->position;
    float speed = 3;

    if (dir_left) {
        pos.x -= speed;
        entity_update_position(player, &pos);
        if (collide()) {
            pos.x += speed;
            entity_update_position(player, &pos);
        }
    }

    if (dir_right) {
        pos.x += speed;
        entity_update_position(player, &pos);
        if (collide()) {
            pos.x -= speed;
            entity_update_position(player, &pos);
        }
    }

    if (dir_up && !player->physics.gravity) {
        pos.y += speed;
        entity_update_position(player, &pos);
        if (collide()) {
            pos.y -= speed;
            entity_update_position(player, &pos);
        }
    }

    if (dir_down && !player->physics.gravity) {
        pos.y -= speed;
        entity_update_position(player, &pos);
        if (collide()) {
            pos.y += speed;
            entity_update_position(player, &pos);
        }
    }

    if (player->physics.gravity) {
        pos.y -= 4;
        entity_update_position(player, &pos);
        if (collide()) {
            pos.y += 4;
            entity_update_position(player, &pos);
        }
    }

   // update game state
    render_update(active_scene);

    init_time = SDL_GetTicks();
    SDL_Delay(16); // temporary until the timers are sorted out
}
