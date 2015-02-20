#ifndef ENTITY_H_
#define ENTITY_H_

#include <hashtable.h>
#include <stdbool.h>

#include "../../lib/lua/lua.h"
#include "../vec2.h"
#include "../../client/graphics/model.h"

typedef struct entity_physics_s {
    bool gravity;
    float speed;
} EntityPhysics;

typedef struct anim_state_s {
    uint32_t index;
    uint32_t frame_time;
} AnimState;

typedef struct entity_s Entity;

struct entity_s {
    uint32_t   id;

    /* Lua object associated with this entity */
    int        lua_ref;


    char      *name;

    Mesh       mesh;

    HashTable *anims;

    /* Frame represents an animation frame */
    uint32_t   frame_time_delta;
    uint32_t   frame_time;

    uint32_t   texture_i;
    uint32_t   texture_n;

    /* Array of textures (animations) where each individual
     * texture is indexed by texture_i */
    Texture   *textures;

    Vec2 position;
    Vec2 direction;
    Vec2 velocity;
    Vec2 destination;

    uint32_t think_time;

    /* Physics properties of this entity */
    EntityPhysics physics;

    void (*on_destroy) (Entity *ent, lua_State *lua_vm);
    void (*on_spawn)   (Entity *ent, lua_State *lua_vm);

    void (*think)      (Entity *ent);

    void (*on_collide) (Entity *self, Entity *other, lua_State *lua_vm);

    void (*message)    (Entity *ent); // + some message TODO
    void (*ai)         (Entity *ent);

    void (*init_move)  (Entity *ent, Vec2 dir, Vec2 vel, Vec2 dest);
    void (*update)     (Entity *ent, uint32_t delta_time);

    Entity *parent;
    Vector *children;

    uint32_t life_time;
};

Entity *entity_new             (lua_State *lvm, char *name, Entity *parent);
void    entity_destroy         (Entity *self, lua_State *lua_vm);

void    entity_on_collide      (Entity *self, Entity *other, lua_State *lvm);
void    entity_on_destroy      (Entity *self, lua_State *lvm);
void    entity_on_spawn        (Entity *self, lua_State *lvm);
void    entity_think           (Entity *self);

/* not yet defined */
void    entity_ai              (Entity *self);
void    entity_message         (Entity *self);

void    entity_init_move       (Entity *self, Vec2 *dir, Vec2 *vel, Vec2 *des);
void    entity_move            (Entity *self, uint32_t dt);

void    entity_update          (Entity *self, uint32_t delta_time);
void    entity_update_position (Entity *self, Vec2 *new_pos);
void    entity_set_animation   (Entity *self, const char *anim);

// experimental
void    entity_on_move         (lua_State *lvm, Entity *self, char *dir);

#endif
