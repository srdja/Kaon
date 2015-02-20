/*
 * bindings.c
 *
 *  Created on: Nov 14, 2014
 *      Author: srdja
 */

/*******************************************************************************
 *
 * Functions callable from lua scripts
 *
 ******************************************************************************/
#include <stdio.h>

#include "bindings.h"
#include "lua_util.h"

#include "../entity/entity.h"

static int set_animation(lua_State *lvm);

void
bindings_bind(lua_State *lvm)
{
    lua_pushcfunction(lvm, set_animation);
    lua_setglobal(lvm, "set_animation");
}

static int set_animation(lua_State *lvm)
{
    Entity *e = (Entity*) lua_touserdata(lvm, 1);
    const char *a = luaL_checkstring(lvm, 2);

    entity_set_animation(e, a);

    return 0; // nothing returned back to lua
}
