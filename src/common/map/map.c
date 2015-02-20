/*
 * map.c
 *
 *  Created on: Nov 13, 2014
 *      Author: srdja
 */


/*******************************************************************************
 * In case the map format changes, all of these functions will probably be
 * horribly broken.
 *
 ******************************************************************************/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <hashtable.h>

#include "map.h"

#include "../../lib/lua/lua.h"
#include "../../lib/lua/lauxlib.h"
#include "../../lib/lua/lualib.h"

#include "../script/lua_util.h"

#include "../../file-system/paths.h"
#include "../../file-system/path_util.h"
#include "../strutil.h"


static HashTableProperties *htprop = NULL;

MapInfo *
load_map()
{
    MapInfo *mapinfo = calloc(1, sizeof(MapInfo));

    htprop = hashtable_properties_new();
    htprop->hash        = POINTER_HASH;
    htprop->key_length  = KEY_LENGTH_POINTER;
    htprop->key_compare = CMP_POINTER;

    mapinfo->tile_info = hashtable_new(htprop);
    hashtable_properties_destroy(htprop);

    char *map_path = paths_map("testmap3.lua");

    lua_State *lua_vm;

    lua_vm = luaL_newstate();
    luaL_openlibs(lua_vm);

    if (luaL_loadfile(lua_vm, map_path) /* || lua_pcall(lua_vm, 0, 0, 0)*/)
        fprintf(stdout, "error loading map file %s\n", map_path);

    lua_pcall(lua_vm, 0, LUA_MULTRET, 0);

    luau_assert_table(lua_vm, -1);

    lua_pushstring(lua_vm, "width");
    lua_gettable(lua_vm, -2);

    mapinfo->tiles_width = (int) lua_tonumber(lua_vm, -1);
    lua_pop(lua_vm, 1);

    lua_pushstring(lua_vm, "height");
    lua_gettable(lua_vm, - 2);

    mapinfo->tiles_height = (int) lua_tonumber(lua_vm, -1);
    lua_pop(lua_vm, 1);

    lua_pushstring(lua_vm, "tilesets");
    lua_gettable(lua_vm, -2);

    lua_rawgeti(lua_vm, -1, 1);

    lua_pushstring(lua_vm, "tiles");
    lua_gettable(lua_vm, -2);

    int t_size = lua_rawlen(lua_vm, -1);

    int i;
    for (i = 1; i <= t_size; i++) {
        int   id;
        char *name;
        char *name_raw;

        lua_rawgeti(lua_vm, -1, i);

        lua_pushstring(lua_vm, "id");
        lua_gettable(lua_vm, -2);

        id = (int) lua_tonumber(lua_vm, -1);
        lua_pop(lua_vm, 1);

        lua_pushstring(lua_vm, "image");
        lua_gettable(lua_vm, -2);

        name_raw = (char*) lua_tostring(lua_vm, -1);
        name     = putil_name_str(name_raw);
        lua_pop(lua_vm, 1);

        lua_pop(lua_vm, 1);

        hashtable_put(mapinfo->tile_info, (void*) id, name);
    }

    lua_pop(lua_vm, 1);
    lua_pop(lua_vm, 1);
    lua_pop(lua_vm, 1);

    lua_pushstring(lua_vm, "layers");
    lua_gettable(lua_vm, -2);

    lua_rawgeti(lua_vm, -1, 1);

    lua_pushstring(lua_vm, "data");
    lua_gettable(lua_vm, -2);

    t_size = lua_rawlen(lua_vm, -1);

    mapinfo->data = calloc(mapinfo->tiles_width * mapinfo->tiles_height, sizeof(int));

    for (i = 1; i <= t_size; i++) {
        lua_rawgeti(lua_vm, -1, i);
        mapinfo->data[i - 1] = lua_tonumber(lua_vm, -1) - 1;
        lua_pop(lua_vm, 1);
    }

    lua_pop(lua_vm, 1);
    lua_pop(lua_vm, 1);
    // at layer

    lua_rawgeti(lua_vm, -1, 2);

    luau_assert_table(lua_vm, -1);

    lua_pushstring(lua_vm, "objects");
    lua_gettable(lua_vm, -2);

    lua_rawgeti(lua_vm, -1, 1);

    luau_assert_table(lua_vm, -1);

    lua_pushstring(lua_vm, "properties");
    lua_gettable(lua_vm, -2);

    luau_assert_table(lua_vm, -1);

    lua_pushstring(lua_vm, "entity");
    lua_gettable(lua_vm, -2);

    lua_close(lua_vm);

    return mapinfo;
}

void
map_spawn_point()
{

}
