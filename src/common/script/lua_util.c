/*
 * lua_util.c
 *
 *  Created on: Nov 13, 2014
 *      Author: srdja
 */

#include <string.h>

#include "lua_util.h"
#include "../strutil.h"
#include "../../client/console.h" // should be in common

static void print_error(lua_State *lua, int expected_type);

/*
Vector *


lua_pushnil(L);
while (lua_next(L, t) != 0) {

   printf("%s - %s\n",
          lua_typename(L, lua_type(L, -2)),
          lua_typename(L, lua_type(L, -1)));
   lua_pop(L, 1);
}
*/
void
luau_get_table_number(lua_State *lua, int *num, char *var, int table_stack)
{
    lua_pushstring(lua, var);
    lua_gettable(lua, table_stack);
    luau_assert_number(lua, -1);

    *num = (int) lua_tonumber(lua, -1);
    lua_pop(lua, 1);
}

/**-----------------------------------------------------------------------------
 * Sets a dynamically allocated string...
 -----------------------------------------------------------------------------*/
void
luau_get_table_string(lua_State *lua, char **str, char *var, int table_stack)
{
    lua_pushstring(lua, var);
    lua_gettable(lua, table_stack);
    luau_assert_string(lua, -1);

    *str = strucpy((char*) lua_tostring(lua, -1));
    lua_pop(lua, 1);
}

/**-----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
void
luau_get_table_boolean(lua_State *lua, bool *b, char *var, int table_stack)
{
    lua_pushstring(lua, var);
    lua_gettable(lua, table_stack);
    luau_assert_boolean(lua, -1);

    *b = lua_toboolean(lua, -1);
    lua_pop(lua, 1);
}


void
luau_assert_table(lua_State *lua, int stack_pos)
{
    if (lua_istable(lua, stack_pos))
        return;

    print_error(lua, LUA_TTABLE);
    luaL_traceback(lua, lua, NULL, 1);

    char *msg = (char*) lua_tostring(lua, -1);

    fprintf(stdout, "%s\n", msg);
}

void
luau_assert_number(lua_State *lua, int stack_pos)
{
    if (lua_isnumber(lua, stack_pos))
        return;

    print_error(lua, LUA_TNUMBER);
}

void
luau_assert_string(lua_State *lua, int stack_pos)
{
    if (lua_isstring(lua, stack_pos))
        return;

    print_error(lua, LUA_TSTRING);
}

void
luau_assert_function(lua_State *lua, int stack_pos)
{
    if (lua_isfunction(lua, stack_pos))
        return;

    print_error(lua, LUA_TFUNCTION);
    luaL_traceback(lua, lua, NULL, 1);
}

void
luau_assert_boolean(lua_State *lua, int stack_pos)
{
    if (lua_isboolean(lua, stack_pos))
        return;

    print_error(lua, LUA_TBOOLEAN);
}

void
luau_assert_lightuserdata(lua_State *lua, int stack_pos)
{
    if (lua_islightuserdata(lua, stack_pos))
        return;

    print_error(lua, LUA_TLIGHTUSERDATA);
}

void
luau_assert_userdata(lua_State *lua, int stack_pos)
{
    if (lua_isuserdata(lua, stack_pos))
        return;

    print_error(lua, LUA_TUSERDATA);
}

void
luau_assert_thread(lua_State *lua, int stack_pos)
{
    if (lua_isthread(lua, stack_pos))
        return;

    print_error(lua, LUA_TTHREAD);
}

void
luau_assert_pcall(lua_State *lua, int err)
{
    if (err == 0)
        return;

    switch (err) {
    case LUA_ERRRUN:
        console_msg_post("Lua runtime error on `pcall`!\n");
        break;
    case LUA_ERRMEM:
        console_msg_post("Lua out of memory error on `pcall`!\n");
        break;
    case LUA_ERRERR:
        console_msg_post("Lua error handler error on `pcall`!\n");
        break;
    }
    luaL_traceback(lua, lua, NULL, 1);
}

static void
print_error(lua_State *lua, int expected_type)
{
    int type = lua_type(lua, -1);
    char str_type    [32];
    char str_type_exp[32];
    luau_type_tostring(type, str_type, 20);
    luau_type_tostring(expected_type, str_type_exp, 20);
    console_msg_post("Expected a %s but got %s!\n", str_type_exp, str_type);
}

void
print_lua_stack(lua_State *lua)
{
    size_t i;
    for (i = 1; i <= lua_gettop(lua); i++) {
        int type = lua_type(lua, i);

        switch (type) {
        case LUA_TNIL:
            fprintf(stdout, "lua null\n");
            break;
        case LUA_TNUMBER:
            fprintf(stdout, "lua number\n");
            break;
        case LUA_TTABLE:
            fprintf(stdout, "lua table\n");
            break;
        case LUA_TSTRING:
            fprintf(stdout, "lua string\n");
            break;
        case LUA_TUSERDATA:
            fprintf(stdout, "lua user data\n");
            break;
        case LUA_TFUNCTION:
            fprintf(stdout, "lua function\n");
            break;
        case LUA_TBOOLEAN:
            fprintf(stdout, "lua boolean\n");
            break;
        case LUA_TTHREAD:
            fprintf(stdout, "lua thread\n");
            break;
        case LUA_TLIGHTUSERDATA:
            fprintf(stdout, "lua light user data\n");
            break;
        }
    }
}

void
luau_type_tostring(int type, char *str, size_t n)
{
    switch (type) {
    case LUA_TNIL:
        strncpy(str, "nil", n);
        break;
    case LUA_TNUMBER:
        strncpy(str, "number", n);
        break;
    case LUA_TTABLE:
        strncpy(str, "table", n);
        break;
    case LUA_TSTRING:
        strncpy(str, "string", n);
        break;
    case LUA_TUSERDATA:
        strncpy(str, "userdata", n);
        break;
    case LUA_TFUNCTION:
        strncpy(str, "function", n);
        break;
    case LUA_TBOOLEAN:
        strncpy(str, "boolean", n);
        break;
    case LUA_TTHREAD:
        strncpy(str, "thread", n);
        break;
    case LUA_TLIGHTUSERDATA:
        strncpy(str, "lightuserdata",n );
        break;
    default:
        strncpy(str, "undefined", n);
        break;
    }
}
