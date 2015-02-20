/*
 * paths.c
 *
 *  Created on: Nov 4, 2014
 *      Author: srdja
 */

/*******************************************************************************
 *
 *
 *
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_filesystem.h>
#include <SDL2/SDL_stdinc.h>

#include "../file-system/fs_entry.h"
#include "../common/strutil.h"

#include "paths.h"

static char *get_path(const char *rel, const char *name);

/* cache */
static char *base_path  = NULL;
static char *pref_path  = NULL;

static char *text_rel   = NULL;
static char *shdr_rel   = NULL;
static char *maps_rel   = NULL;
static char *sound_rel  = NULL;
static char *ent_s_rel  = NULL;
static char *trig_s_rel = NULL;
static char *ui_s_rel   = NULL;

/** ----------------------------------------------------------------------------
 * Initializes system paths
 *
 * @return 0 on success
 -----------------------------------------------------------------------------*/
int
paths_init()
{
    base_path = fsent_base_dir();
    pref_path = SDL_GetPrefPath("Company", "Game");

    char sep[2];
    sep[0] = fsent_path_separator_c();
    sep[1] = '\0';

    text_rel   = strucat(4, "resources", &sep, "textures", &sep);
    shdr_rel   = strucat(4, "resources", &sep, "shaders", &sep);
    maps_rel   = strucat(4, "resources", &sep, "maps", &sep);
    ent_s_rel  = strucat(6, "resources", &sep, "scripts", &sep, "entities", &sep);
    trig_s_rel = strucat(6, "resources", &sep, "scripts", &sep, "triggers", &sep);
    ui_s_rel   = strucat(6, "resources", &sep, "scripts", &sep, "interface", &sep);
    sound_rel  = strucat(4, "resources", &sep, "sounds", &sep);

    return 0;
}

/** ----------------------------------------------------------------------------
 * Returns the absolute path of the texture. The texture need not already exist.
 *
 * @param[in] name the name of the texture.
 *
 * @return absolute path of the texture
 -----------------------------------------------------------------------------*/
char *
paths_texture(char *name)
{
    char *path = get_path(text_rel, name);
    return path;
}

/*******************************************************************************
 * Returns the absolute path of the shader file. The shader file need not exist.
 *
 * @param[in] name the name of the shader file
 *
 * @return absolute path to the shader file.
 ******************************************************************************/
char *
paths_shader(char *name)
{
    char *path = get_path(shdr_rel, name);
    return path;
}

/*******************************************************************************
 * Returns the absolute path of the map file. The map file need not exist.
 *
 * @param[in] name the name of the map file.
 *
 * @return absolute path to the map file.
 ******************************************************************************/
char *
paths_map(char *name)
{
    char *path = get_path(maps_rel, name);
    return path;
}

/*******************************************************************************
 * Returns the absolute path of the sound file. The sound file need not exist.
 *
 * @param[in] name the name of the sound file.
 *
 * @return absolute path to the sound file.
 ******************************************************************************/
char *
paths_sound(char *name)
{
    char *path = get_path(sound_rel, name);
    return path;
}

/*******************************************************************************
 *
 ******************************************************************************/
char *
paths_script_entities(char *name)
{
    char *path = get_path(ent_s_rel, name);
    return path;
}

/*******************************************************************************
 *
 ******************************************************************************/
char *
paths_script_triggers(char *name)
{
    char *path = get_path(trig_s_rel, name);
    return path;
}

/*******************************************************************************
 *
 ******************************************************************************/
char *
paths_script_ui(char *name)
{
    char *path = get_path(ui_s_rel, name);
    return path;
}

/*******************************************************************************
 *
 ******************************************************************************/
char *
paths_texture_base()
{
    char *path = strucat(2, base_path, text_rel);
    return path;
}

/*******************************************************************************
 *
 ******************************************************************************/
char *
paths_map_base()
{
    char *path = strucat(2, base_path, maps_rel);
    return path;
}

/*******************************************************************************
 *
 ******************************************************************************/
char *
paths_shader_base()
{
    char *path = strucat(2, base_path, shdr_rel);
    return path;
}

/*******************************************************************************
 *
 ******************************************************************************/
char *
paths_entity_base()
{
    char *path = strucat(2, base_path, ent_s_rel);
    return path;
}

/*******************************************************************************
 *
 ******************************************************************************/
char *
paths_trigger_base()
{
    char *path = strucat(2, base_path, trig_s_rel);
    return path;
}

/*******************************************************************************
 *
 ******************************************************************************/
char *
paths_ui_base()
{
    char *path = strucat(2, base_path, ui_s_rel);
    return path;
}

/*******************************************************************************
 *
 ******************************************************************************/
char *
paths_pref()
{
    if (!pref_path)
        pref_path = SDL_GetPrefPath("Company", "Game");

    if (!pref_path)
        return NULL;

    return strucpy(pref_path);
}

/*******************************************************************************
 *
 ******************************************************************************/
char *
paths_base()
{
    if (!base_path)
        base_path = fsent_base_dir();

    if (!base_path)
        return NULL;

    return strucpy(base_path);
}

/*******************************************************************************
 *
 ******************************************************************************/
static char *
get_path(const char *rel, const char *name)
{
    char *new_path = strucat(3, base_path, rel, name);
    return new_path;
}
