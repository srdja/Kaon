#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

#include "../../file-system/fs_entry.h"
#include "../../file-system/path_util.h"
#include "../../common/strutil.h"

static char *to_platform_path (const char *path, int len);
static bool  is_valid_path    (char *path, int len);

static const char  separator_char  = '/';
static const char  separator_str[] = {'/', '\0'};

/**-----------------------------------------------------------------------------
 *
 * @param
 *
 * @return
 -----------------------------------------------------------------------------*/
FS_Entry *
fsent_new(char *path)
{
    size_t      len = strlen(path);
    struct stat st;

    if (!is_valid_path(path, len))
        return NULL;

    FS_Entry *f = calloc(1, sizeof(FS_Entry));

    if (access(path, R_OK) == 0)
        f->can_read = true;

    if (access(path, W_OK) == 0)
        f->can_write = true;

    if (stat(path, &st) != -1) {
        f->exists = true;

        if (S_ISDIR(st.st_mode))
            f->is_dir = true;

        if (S_ISREG(st.st_mode))
            f->is_file = true;

        if (S_ISLNK(st.st_mode))
            f->is_symlink = true;

        if (f->is_file)
            f->size = st.st_size;
    }

    f->path     = to_platform_path(path, len);
    f->path_len = len;
    f->sep      = separator_char;

    return f;
}

/*******************************************************************************
 *
 ******************************************************************************/
void
fsent_destroy(FS_Entry *file)
{
    free(file->path);
    free(file);
}

/*******************************************************************************
 *
 ******************************************************************************/
int
fsent_mkdir(FS_Entry *file)
{
    if (!file->is_dir)
        return -1;
    return 0;
}

/*******************************************************************************
 *
 ******************************************************************************/
char
fsent_path_separator_c()
{
    return separator_char;
}

/*******************************************************************************
 *
 ******************************************************************************/
const char *
fsent_path_separator_s()
{
    return separator_str;
}


/*------------------------------------------------------------------------------
 * Returns the base directory of the executable or NULL if the path could not be
 * obtained. The returned path is null terminated and owned by the caller.
 *
 * @return base directory path of the executable
 -----------------------------------------------------------------------------*/
char *
fsent_base_dir()
{
    char path[2048];
    size_t len = 0;
#ifdef __linux__
    len = readlink("/proc/self/exe", path, 2048);
#endif
#ifdef __FreeBSD__
    len = readlink("/proc/curproc/file", path, 2048);
#endif
    if (len == 0)
        return NULL;

    /* Discard the proc name from the path */
    while (path[len - 1] != '/')
        len--;

    char *p = malloc(len + 1);
    memcpy(p, path, len);

    p[len] = '\0';

    return p;
}

/*------------------------------------------------------------------------------
 * Returns the directory listing of the specified path. If the path is not a
 * valid directory path, or if the caller doesn't have the permission to read it,
 * NULL is returned instead. The directory listing is stored inside a Vector of
 * File objects in an unspecified order.
 *
 * @param[in] dir the directory path whose listing is being returned
 *
 * @return the listing of the specified directory
 -----------------------------------------------------------------------------*/
Vector *
fsent_ls(FS_Entry *dir)
{
    if (!dir->is_dir)
        return NULL;

    DIR *dr = opendir(dir->path);
    if (!dr)
        return NULL;

    int     nl = 0;
    char   *fp = NULL;
    Vector *ls = vector_new();

    struct dirent *entry;
    while ((entry = readdir(dr))) {
        nl  = strlen(entry->d_name);

        fp = putil_pathcat(
                dir->path, entry->d_name,
                dir->path_len, nl);

        FS_Entry *file = fsent_new(fp);

        if (!file)
            goto ERROR;

        vector_add(ls, file);
        free(fp);
    }
    closedir(dr);

    return ls;

    ERROR:
    free(fp);
    vector_destroy_free(ls);
    return NULL;
}

/*******************************************************************************
 * Returns a listing of all files from the directory that match the specified
 * extension. If the path is not a valid directory path, NULL is returned
 * instead. The directory listing is stored inside a Vector of File objects in
 * an unspecified order.
 *
 * @parma[in] dir
 * @parma[in] ext
 *
 * return
 ******************************************************************************/
Vector *
fsent_ls_by_extension(FS_Entry *dir, char *ext)
{
    if (!dir || !ext || dir->is_file)
        return NULL;

    DIR *dr = opendir(dir->path);
    if (!dr)
        return NULL;

    Vector *listing = vector_new();
    struct dirent *entry;

    int   name_len;
    int   path_len;
    char *full_path;
    char *extension;

    while ((entry = readdir(dr))) {
        name_len  = strlen(entry->d_name);
        path_len  = dir->path_len;
        full_path = putil_pathcat(dir->path, entry->d_name, path_len, name_len);

        FS_Entry *file = fsent_new(full_path);

        if (!file)
            goto ERROR;

        extension = putil_extension(file);

        if (strcmp(extension, ext) == 0)
            vector_add(listing, file);
        else
            fsent_destroy(file);

        free(extension);
        free(full_path);
    }
    closedir(dr);

    return listing;

    ERROR:
    free(extension);
    free(full_path);
    vector_destroy_free(listing);
    closedir(dr);
    return NULL;
}


/*******************************************************************************
 * Converts a path to a platform specific path.
 *
 * @param[in     ] path
 * @param[in, out] len
 * @param[in     ] is_dir
 *
 * @return
 ******************************************************************************/
static char *
to_platform_path(const char *path, int len)
{
    char *new_path = malloc(len + 1);

    int i;
    for (i = 0; i < len; i++) {
        if (path[i] == '\\')
            new_path[i] = '/';
        else
            new_path[i] = path[i];
    }
    new_path[len] = '\0';

    return new_path;
}

/*******************************************************************************
 *
 ******************************************************************************/
static bool
is_valid_path(char *path, int len)
{
    // FIXME check for invalid characters like /./somedir/.

    bool is_prev_slash = false;

    int i;
    for (i = 0; i < len; i++) {
        if (path[i] == '/') {
            if (is_prev_slash)
                break;
            is_prev_slash = true;
        } else {
            is_prev_slash = false;
        }
    }
    return true;
}
