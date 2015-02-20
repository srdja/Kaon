/*
 * paths.h
 *
 *  Created on: Nov 4, 2014
 *      Author: srdja
 */

#ifndef PATHS_H_
#define PATHS_H_

int    paths_init            ();
char  *paths_texture         (char *tex_name);
char  *paths_shader          (char *shader_name);
char  *paths_sound           (char *sound_name);
char  *paths_map             (char *map_name);
char  *paths_script_entities (char *script_name);
char  *paths_script_triggers (char *script_name);
char  *paths_script_ui       (char *script_name);
char  *paths_pref            ();
char  *paths_base            ();

char  *paths_texture_base    ();
char  *paths_map_base        ();
char  *paths_shader_base     ();
char  *paths_entity_base     ();
char  *paths_trigger_base    ();
char  *paths_ui_base         ();

#endif /* PATHS_H_ */
