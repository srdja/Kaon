#ifndef FILE_H_
#define FILE_H_

#include <stdint.h>
#include <stdbool.h>
#include <vector.h>

typedef struct fs_entry_s {
    char *path;
    char  sep;

    bool is_file;
    bool is_dir;
    bool is_symlink;
    bool exists;

    bool can_read;
    bool can_write;

    size_t size;
    size_t path_len;
} FS_Entry;

FS_Entry   *fsent_new              (char *path);
void        fsent_destroy          (FS_Entry *entry);

char       *fsent_name             (FS_Entry *entry);
char       *fsent_extension        (FS_Entry *entry);

int         fsent_mkdir            (FS_Entry *entry);
int         fsent_rmdir            (FS_Entry *entry, bool r, bool f);

Vector     *fsent_ls               (FS_Entry *dir);
Vector     *fsent_ls_by_extension  (FS_Entry *dir, char *ext);

char       *fsent_base_dir         ();
char        fsent_path_separator_c ();
const char *fsent_path_separator_s ();

#endif
