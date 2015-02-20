/*
 * path_util.h
 *
 *  Created on: Nov 7, 2014
 *      Author: srdja
 */

#ifndef PATH_UTIL_H_
#define PATH_UTIL_H_

#include <stdbool.h>

#include "fs_entry.h"

char  *putil_name          (FS_Entry *entry);
char  *putil_extension     (FS_Entry *entry);

char  *putil_name_str      (char *path);
char  *putil_extension_str (char *path);

char  *putil_no_extension  (char *path);

char  *putil_pathcat       (char *p1, char *p2, size_t p1_len, size_t p2_len);
bool   putil_has_extension (char *path, int len);

#endif /* PATH_UTIL_H_ */
