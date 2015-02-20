/*
 * lua_util.h
 *
 *  Created on: Nov 13, 2014
 *      Author: srdja
 */

#ifndef LUA_UTIL_H_
#define LUA_UTIL_H_

#include <stdbool.h>
#include <vector.h>

#include "../../lib/lua/lua.h"
#include "../../lib/lua/lauxlib.h"
#include "../../lib/lua/lualib.h"

void  luau_get_table_number     (lua_State *lua, int *num, char *var, int stack_i);
void  luau_get_table_string     (lua_State *lua, char **str, char *var, int stack_i);
void  luau_get_table_boolean    (lua_State *lua, bool *b, char *var, int stack_i);

void  luau_assert_table         (lua_State *lvm, int stack_pos);
void  luau_assert_number        (lua_State *lvm, int stack_pos);
void  luau_assert_string        (lua_State *lvm, int stack_pos);
void  luau_assert_function      (lua_State *lvm, int stack_pos);
void  luau_assert_boolean       (lua_State *lvm, int stack_pos);
void  luau_assert_lightuserdata (lua_State *lvm, int stack_pos);
void  luau_assert_userdata      (lua_State *lvm, int stack_pos);
void  luau_assert_thread        (lua_State *lvm, int stack_pos);

void  luau_assert_pcall         (lua_State *lvm, int err);
void  luau_type_tostring        (int type, char *buff, size_t n);

void  print_lua_stack           (lua_State *lua);

#endif /* LUA_UTIL_H_ */
