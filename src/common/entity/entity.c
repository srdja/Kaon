#include <hashtable.h>
#include <stdint.h>
#include <stdlib.h>

#include "../script/lua_util.h"

#include "../../lib/lua/lua.h"
#include "../../lib/lua/lauxlib.h"
#include "../../lib/lua/lualib.h"

#include "entity.h"

static void entity_update_animation (Entity *self, uint32_t delta_t);
static void set_properties          (lua_State *lvm, Entity *e);
static void set_properties_cached   (lua_State *lvm, Entity *self);

static HashTableProperties *htprop;

/* Stores the first copy of each entity to be used as a template for future
 * entities of the same type. This saves some time by not having to set all
 * properties each time. */

/* Entities could be pre-loaded to avoid any potential slow-downs during game play */
static HashTable           *ent_cache;

/** ----------------------------------------------------------------------------
 *
 * @param[in] lvm the lua state
 * @param[in] name the name of the the global table representing the entity
 *
 * @return
 -----------------------------------------------------------------------------*/
Entity *
entity_new(lua_State *lvm, char *name, Entity *parent)
{
    if (!htprop)
        htprop = hashtable_properties_new();

    Entity *e = calloc(1, sizeof(Entity));

    e->on_collide = entity_on_collide;
    e->on_destroy = entity_on_destroy;
    e->on_spawn   = entity_on_spawn;
    e->think      = entity_think;
    e->anims      = hashtable_new(htprop);

    lua_getglobal(lvm, name);
    luau_assert_table(lvm, -1);

    lua_pushstring(lvm, "new");
    lua_gettable(lvm, -2);

    luau_assert_function(lvm, -1);
    luau_assert_pcall(lvm, lua_pcall(lvm, 0, 1, 0));
    luau_assert_table(lvm, -1);

    lua_pushstring(lvm, "sysent");
    lua_pushlightuserdata(lvm, e);
    lua_settable(lvm, -3);

    /* keep the lua object reference */
    e->lua_ref = luaL_ref(lvm, LUA_REGISTRYINDEX);

    set_properties(lvm, e);

    lua_pop(lvm, 1);

    return e;
}

/*******************************************************************************
 *
 ******************************************************************************/
static void
set_properties(lua_State *lvm, Entity *self)
{
    char     *name;
    bool      grav = false;
    int       think;

    uint32_t  anims;
    uint32_t  anim_frame_time;
    char     *anim_name;
    char     *anim_data;
    char     *tex;

    lua_rawgeti(lvm, LUA_REGISTRYINDEX, self->lua_ref);
    luau_assert_table(lvm, -1);

    luau_get_table_string(lvm, &name, "name", -2);
    luau_get_table_number(lvm, &think, "think_time", -2);
    luau_get_table_boolean(lvm, &grav, "gravity", -2);
    luau_get_table_number(lvm, &anims, "animations", -2);

    self->textures = malloc(anims * sizeof(Texture));

    self->physics.speed = 0;
    self->physics.gravity = grav;

    int i = 1;
    char var[128] = {0};
    for (i = 0; i < anims; i++) {
        // FIXME each animation can have a different frame time
        snprintf(var, 128, "%s%d%s", "animation", (i + 1), "_name");
        luau_get_table_string(lvm, &anim_name, var, -2);

        snprintf(var, 128, "%s%d%s", "animation", (i + 1), "_data");
        luau_get_table_string(lvm, &anim_data, var, -2);

        snprintf(var, 128, "%s%d%s", "animation", (i + 1), "_frame_time");
        luau_get_table_number(lvm, &anim_frame_time, var, -2);

        self->frame_time = anim_frame_time;
        texture_set_img(&(self->textures[i]), anim_data);

        AnimState *astate = malloc(sizeof(AnimState)); // performance?
        astate->frame_time = anim_frame_time;
        astate->index      = i;

        hashtable_put(self->anims, anim_name, astate);
    }
    mesh_set_rectangle(&(self->mesh), 1, 1);
}

/*******************************************************************************
 *
 ******************************************************************************/
static void
set_properties_cached(lua_State *lvm, Entity *self)
{

}

/*******************************************************************************
 *
 ******************************************************************************/
void
entity_destroy(Entity *self, lua_State *lua_vm)
{

}

/*******************************************************************************
 *
 ******************************************************************************/
void
entity_on_collide(Entity *self, Entity *other, lua_State *lua_vm)
{
    // run lua
}

/*******************************************************************************
 *
 ******************************************************************************/
void
entity_on_spawn(Entity *self, lua_State *lua_vm)
{
    lua_rawgeti(lua_vm, LUA_REGISTRYINDEX, self->lua_ref);

    lua_pushstring(lua_vm, "on_spawn");
    lua_gettable(lua_vm, -2);

    luau_assert_function(lua_vm, -1);
    lua_pushvalue(lua_vm, -2);

    luau_assert_pcall(lua_vm, lua_pcall(lua_vm, 1, 1, 0));

    const char *ret = lua_tostring(lua_vm, -1);

    lua_pop(lua_vm, 1); // pop string
    lua_pop(lua_vm, 1); // pop tale
}

/*******************************************************************************
 *
 ******************************************************************************/
void
entity_on_destroy(Entity *self, lua_State *lua_vm)
{

}

/*******************************************************************************
 *
 ******************************************************************************/
void
entity_think(Entity *self)
{

}

/*******************************************************************************
 *
 ******************************************************************************/
void
entity_init_move(Entity *self, Vec2 *dir, Vec2 *vel, Vec2 *dest)
{
    size_t len = sizeof(Vec2);

    memcpy(&(self->direction), dir, len);
    memcpy(&(self->velocity), vel, len);
    memcpy(&(self->destination), dest, len);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
entity_move(Entity *self, uint32_t delta_time)
{

}

/*******************************************************************************
 *
 ******************************************************************************/
void
entity_update(Entity *self, uint32_t delta_time)
{
    entity_update_animation(self, delta_time);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
entity_update_position(Entity *self, Vec2 *new_pos)
{
    size_t len = sizeof(Vec2);
    memcpy(&(self->position), new_pos, len);
    memcpy(&(self->mesh.trans), new_pos, len);
    mesh_update_bounding_box(&(self->mesh));
}

/*******************************************************************************
 *
 ******************************************************************************/
static void
entity_update_animation(Entity *self, uint32_t delta_t)
{
    uint32_t new_delta = self->frame_time_delta + delta_t;

    if (new_delta >= self->frame_time) {
        Texture *t = &(self->textures[self->texture_i]);

        /* not already the max index */
        if (t->subtex_i < t->subtex_n - 1)
            t->subtex_i++;
        else
            t->subtex_i = 0;

        self->frame_time_delta = 0;
    } else {
        self->frame_time_delta = new_delta;
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
void
entity_set_animation(Entity *self, const char *anim)
{
    // The key doesn't exits it will simply be set to first animation
    AnimState *a = hashtable_get(self->anims, anim);
    if (a) {
        self->texture_i = a->index;
        self->frame_time = a->frame_time;
    } else {
        self->texture_i = 0;
    }
}

/// XXX EXPERIMENTAL
void entity_on_move(lua_State *lvm, Entity *self, char *dir)
{
    // should be a generalized function XXX
    lua_rawgeti(lvm, LUA_REGISTRYINDEX, self->lua_ref);

    lua_pushstring(lvm, "on_move");
    lua_gettable(lvm, -2);

    luau_assert_function(lvm, -1);
    lua_pushvalue(lvm, -2);
    lua_pushstring(lvm, dir);

    luau_assert_pcall(lvm, lua_pcall(lvm, 2, 0, 0));

    lua_pop(lvm, 1); // pop table
}
