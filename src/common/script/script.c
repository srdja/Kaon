/*
 * script.c
 *
 *  Created on: Nov 13, 2014
 *      Author: srdja
 */

#include <hashset.h>

#include "../../lib/lua/lauxlib.h"
#include "../../lib/lua/lualib.h"
#include "../../file-system/path_util.h"
#include "../../file-system/fs_entry.h"

#include "script.h"

static HashSetProperties *set_setup    = NULL;

// should just save the absolute path and compare to that

/* Sets of script names that have been loaded */
static HashSet *entity_scripts  = NULL;
static HashSet *trigger_scripts = NULL;
static HashSet *ui_scripts      = NULL;

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
void
script_init()
{
    set_setup = hashset_properties_new();

    entity_scripts  = hashset_new(set_setup);
    trigger_scripts = hashset_new(set_setup);
    ui_scripts      = hashset_new(set_setup);
}


/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
void
script_load(lua_State *lvm, char *script_path)
{
    char *raw_name = putil_name_str(script_path);
    char *name     = putil_no_extension(raw_name);

    free(raw_name);

    if (hashset_contains(entity_scripts, name)) {
        free(name);
        return;
    }

    if (luaL_loadfile(lvm, script_path) || lua_pcall(lvm, 0, 0, 0)) {
        free(name);
        fprintf(stdout, "error loading script %s\n", script_path);
        return;
    }

    hashset_add(entity_scripts, name);
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
void
script_load_ui(lua_State *lvm, char *name)
{

}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
void
script_load_trigger(lua_State *lvm, char *name)
{

}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
void
script_load_entity(lua_State *lvm, char *name)
{

}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
void
script_load_entities_all(lua_State *lvm)
{

}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
void
script_load_triggers_all(lua_State *lvm)
{

}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
void
script_load_ui_all(lua_State *lvm)
{

}
