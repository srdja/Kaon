/*
 * fsio.h
 *
 *  Created on: Nov 4, 2014
 *      Author: srdja
 */

#ifndef __FSIO_H__
#define __FSIO_H__

#include <stdbool.h>

#include "fs_entry.h"

int fsio_write_txt        (FS_Entry *e, char *buff, size_t buff_size);
int fsio_write_bin        (FS_Entry *e, char *buff, size_t buff_size);

int fsio_read_txt_alloc   (FS_Entry *e, char **buff);
int fsio_read_bin_alloc   (FS_Entry *e, char **buff);

int fsio_read_txt         (FS_Entry *e, char *buff, size_t buff_size);
int fsio_read_bin         (FS_Entry *e, char *buff, size_t buff_size);

int fsio_write_txt_c      (char *path, char *buff, size_t buff_size);
int fsio_write_bin_c      (char *path, char *buff, size_t buff_size);

int fsio_read_txt_alloc_c (char *path, char **buff);
int fsio_read_bin_alloc_c (char *path, char **buff);

int fsio_read_txt_c       (char *path, char *buff, size_t buff_size);
int fsio_read_bin_c       (char *path, char *buff, size_t buff_size);

#endif
