#include <SDL2/SDL.h>
#include <string.h>
#include <stdlib.h>

#include "input_context.h"

HashTableProperties *tp;

/** ----------------------------------------------------------------------------
 *
 *
 * @brief input_context_new
 * @param name the name of the context
 * @param use_scancodes whether or not to use raw scancodes or utf8 codes
 * @param priority the priority of the context. High priority context process
 *                 input events first and then pass them onto lower priority
 *                 contexts
 * @param fallthrough if true input events will not be consumed by this context
 * @param fallthrough_unused
 * @return
 -----------------------------------------------------------------------------*/
InputContext *
icontext_new(char     *name,
             bool      use_scancodes,
             uint32_t  priority,
             bool      fallthrough,
             bool      fallthrough_unused)
{
    InputContext *c = calloc(1, sizeof(InputContext));
    
    if (!tp) {
        tp = hashtable_properties_new();
        tp->hash        = POINTER_HASH;
        tp->key_compare = CMP_POINTER;
        tp->key_length  = KEY_LENGTH_POINTER;
    }

    c->key_maps = hashtable_new(tp);

    strncpy(c->name, name, 50);

    c->use_key_scan_codes = use_scancodes;
    c->is_active          = true;
    c->priority           = priority;
    c->fallthrough        = fallthrough;
    c->fallthrough_unused = fallthrough_unused;

    return c;
}

void icontext_destroy(InputContext *con)
{

}

/**
 *
 */
void
icontext_add_key_map(InputContext *c, ButtonMap *map)
{
    HashTable *keys = c->key_maps;

    if (hashtable_contains_key(keys, (void*) map->key)) {
        HashTable *mods = hashtable_get(keys, (void*) map->key);
        hashtable_put(mods, (void*) map->mod, map);
    } else {
        HashTable *mod_table = hashtable_new(tp);
        hashtable_put(mod_table, map->mod, map);
        hashtable_put(c->key_maps, map->key, mod_table);
    }
}

/**
 *
 */
ButtonMap *
icontext_get_key_map (InputContext *c, uint32_t key, uint32_t mod)
{
    HashTable *t = hashtable_get(c->key_maps, key);

    if (!t) {
        return NULL;
   }
    return hashtable_get(t, mod);
}

/** ----------------------------------------------------------------------------
 * @brief input_context_key_map_exists
 * @param c
 * @param map
 * @return
 -----------------------------------------------------------------------------*/
bool
icontext_key_map_exists(InputContext *c, ButtonMap *map)
{
    return false;
}

/** ----------------------------------------------------------------------------
 * @param[in] on_release
 * @param[in] action
 *
 * @return 
 -----------------------------------------------------------------------------*/
ButtonMap *
button_map_new(uint32_t key,
               uint32_t mod,
               bool on_release,
               bool on_repeat,
               ButtonAction *action)
{
    ButtonMap *map = calloc(1, sizeof(ButtonMap));
  
    map->key               = key;
    map->mod               = mod;
    map->key_state         = SDL_RELEASED;
    map->invoke_on_release = on_release;
    map->combo_was_down    = false;
    map->action            = action;
    map->invoke_on_repeat  = on_repeat;

    return map;
}

/**
 * Does not free the action associated with it
 *
 */ 
void
button_map_destroy(ButtonMap *map)
{
    free(map);
}
