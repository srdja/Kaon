#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <winbase.h>

#include <SDL2/SDL.h>

#include "../../file-system/fs_entry.h"
#include "../../file-system/path_util.h"
#include "../../common/strutil.h"


static char *to_platform_path (const char *path, int len);
static bool  is_valid_path    (char *path, int len);

static const char separator_char   = '\\';
static const char separator_str[2] = {'\\', '\0'};

/*******************************************************************************
 *
 ******************************************************************************/
FS_Entry *
fsent_new(char *path)
{
    size_t len = strlen(path);

    FS_Entry *f = calloc(1, sizeof(FS_Entry));

    //DWORD attrib = GetFileAttributes(path);

   // if (attrib != INVALID_FILE_ATTRIBUTES)
        f->exists = true;

    //if (attrib & FILE_ATTRIBUTE_DIRECTORY)
    //    f->is_dir = true;

    //if (attrib & FILE_ATTRIBUTE_ARCHIVE)
        f->is_file = true;

    f->is_symlink = false; // XXX

    //if (!(attrib & FILE_ATTRIBUTE_READONLY))
    //    f->can_write = true;

    f->can_read = true;
    f->can_write = true;

    f->is_dir = false;

    f->path = to_platform_path(path, len);
    f->path_len = len;
    f->sep = separator_char;

    size_t sz;

    FILE *fp = fopen(f->path, "r");

    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);

    fseek(fp, 0L, SEEK_SET);

    f->size = sz;
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
    return &separator_str;
}


/*******************************************************************************
 * Returns the base directory of the executable or NULL if the path could not be
 * obtained. The returned path is null terminated and owned by the caller.
 *
 * @return base directory path of the executable
 ******************************************************************************/
char *
fsent_base_dir()
{
    /*
    WCHAR  path[2048];
    size_t len = 0;

    HMODULE module = GetModuleHandleW(NULL);
    GetModuleFileNameW(module, path, 2048);

    while (path[len - 1] != '\\')
        len--;

    char *p = malloc(len + 1);
    memcpy(p, path, len);

    p[len] = '\0';
    */
    char *p = SDL_GetBasePath();
    char *p2 = strucpy(p);
    SDL_free(p);

    return p2;
}

/*******************************************************************************
 * Returns the directory listing of the specified path. If the path is not a
 * valid directory path, or if the caller doesn't have the permission to read it,
 * NULL is returned instead. The directory listing is stored inside a Vector of
 * File objects in an unspecified order.
 *
 * @param[in] dir the directory path whose listing is being returned
 *
 * @return the listing of the specified directory
 ******************************************************************************/
Vector *
fsent_ls(FS_Entry *dir)
{
    /*
    if (!dir->is_dir)
        return NULL;

    WIN32_FIND_DATA file;
    HANDLE          find;

    wchar_t sPath[2048];

    //Specify a file mask. *.* = We want everything!
    wsprintf(sPath, L"%s\\*.*", sDir);

    if((find = FindFirstFile(sPath, &file)) == INVALID_HANDLE_VALUE) {
        wprintf(L"Path not found: [%s]\n", sDir);
        return false;
    }

    do {
        //Find first file will always return "."
        //    and ".." as the first two directories.
        if(wcscmp(file.cFileName, L"." ) != 0 &&
           wcscmp(file.cFileName, L"..") != 0)
        {
            //Build up our file path using the passed in
            //  [sDir] and the file/foldername we just found:
            wsprintf(sPath, L"%s\\%s", sDir, file.cFileName);

            //Is the entity a File or Folder?
            if(file.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) {
                wprintf(L"Directory: %s\n", sPath);
                ListDirectoryContents(sPath); //Recursion, I love it!
            } else {
                wprintf(L"File: %s\n", sPath);
            }
        }
    } while(FindNextFile(find, &file)); //Find the next file.

    FindClose(hFind); //Always, Always, clean things up!
*/
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
        if (path[i] == '/')
            new_path[i] = '\\';
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
    // TODO
    return true;
}
