/*
 * path_util.c
 *
 *  Created on: Nov 7, 2014
 *      Author: srdja
 */

#include <string.h>
#include <stdlib.h>

#include "../common/strutil.h"

#include "path_util.h"

/*******************************************************************************
 * Returns the name of the file. The name is considered to be the last path
 * segment separated by the path separator character.
 *
 * @note The pointer returned by this function is owned by the caller.
 *
 * @param[in] file the file whose name is being returned
 *
 * @return the name of the file
 ******************************************************************************/
char *
putil_name(FS_Entry *ent)
{
    size_t len   = ent->path_len;
    size_t p_len = len - 1;
    size_t n_len = 0;

    while (ent->path[p_len - 1] != ent->sep)
        p_len--;

    n_len = len - p_len;

    if (ent->path[len - 1] == ent->sep)
        n_len--;

    char *name = malloc(n_len + 1);

    int i;
    for (i = 0; i < n_len; i++)
        name[i] = *(ent->path + i + p_len);

    name[n_len] = '\0';

    return name;
}

/*******************************************************************************
 *
 * @param[in]
 *
 * @return
 ******************************************************************************/
char *
putil_name_str(char *path)
{
    size_t len   = strlen(path);
    size_t p_len = len - 1;
    size_t n_len = 0;

   // while (path[p_len - 1] != fsent_path_separator_c())
    while (path[p_len -1] != '/')
        p_len--;

    n_len = len - p_len;

  //  if (path[len - 1] == fsent_path_separator_c())
    if (path[len - 1] == '/')
        n_len--;

    char *name = malloc(n_len + 1);

    int i;
    for (i = 0; i < n_len; i++)
        name[i] = *(path + i + p_len);

    name[n_len] = '\0';

    return name;
}

/*******************************************************************************
 * Returns the extension of the file if one exists. The extension is considered
 * to be the last segment of the path separated by the '.' character. If no
 * extension is present an empty string is returned.
 *
 * @note The returned pointer is owned by the caller.
 *
 * @param[in] file the file whose extension is being returned
 *
 * @return the extension if one exists
 ******************************************************************************/
char *
putil_extension(FS_Entry *ent)
{
    char *ext;

    if (!putil_has_extension(ent->path, ent->path_len)) {
        ext = malloc(1);
        ext[0] = '\0';
        return ext;
    }

    size_t ext_len = 0;
    size_t len     = ent->path_len;
    size_t n_len   = len;

    while (ent->path[n_len - 1] != '.')
        n_len--;

    ext_len = len - n_len;
    ext     = malloc(ext_len + 1);

    int i;
    for (i = 0; i < ext_len; i++)
        ext[i] = *(ent->path + i + n_len);

    ext[ext_len] = '\0';
    return ext;
}

/*******************************************************************************
 *
 * @param[in]
 *
 * @return
 ******************************************************************************/
char *
putil_no_extension(char *path)
{
    char   *new_path = NULL;
    size_t  len      = strlen(path);

    if (!putil_has_extension(path, len)) {
        new_path = malloc(len);
        strncpy(new_path, path, len);
        return new_path;
    }

    size_t n_len = len;

    while (path[n_len - 1] != '.')
        n_len--;

    new_path = malloc(n_len + 1);

    int i;
    for (i = 0; i < n_len; i++)
        new_path[i] = path[i];

    new_path[n_len] = '\0';

    return new_path;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
char *
putil_pathcat(char *p1, char *p2, size_t p1_len, size_t p2_len)
{
    if (p1[p1_len] == fsent_path_separator_c())
        return strucat(3, p1, fsent_path_separator_s(), p2);
    else
        return strucat(2, p1, p2);
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
bool
putil_has_extension(char *path, int len)
{
    char sep = fsent_path_separator_c();

    int i;
    for (i = len - 1; i > 0; i--) {
        if (path[i] == '.' && path[i - 1] != sep)
            return true;
    }
    return false;
}
