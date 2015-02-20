/*
 * script.h
 *
 *  Created on: Nov 13, 2014
 *      Author: srdja
 */

#ifndef SCRIPT_H_
#define SCRIPT_H_

#include "../../lib/lua/lua.h"

#include <vector.h>

void script_init();

void script_load(lua_State *lvm, char *script);

#endif /* SCRIPT_H_ */
