/*
 * bindings.h
 *
 *  Created on: Nov 14, 2014
 *      Author: srdja
 */

#ifndef BINDINGS_H_
#define BINDINGS_H_


#include "../../lib/lua/lua.h"
#include "../../lib/lua/lauxlib.h"
#include "../../lib/lua/lualib.h"


void bindings_bind(lua_State *lvm);

#endif /* BINDINGS_H_ */
