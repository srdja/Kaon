/*
 * trigger.h
 *
 *  Created on: Oct 21, 2014
 *      Author: srdja
 */

#ifndef TRIGGER_H_
#define TRIGGER_H_

#include "entity.h"

typedef struct trigger_s Trigger;

struct trigger_s {
    char *name;
    uint32_t id;

    lua_State *lua;

    bool is_active;

    Vec2 position;

    void (*on_activate) (Trigger *trig);
};

// TODO timer / lifetime of the trigger


#endif /* TRIGGER_H_ */
