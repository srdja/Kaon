/*
 * fs_io.c
 *
 *  Created on: Nov 6, 2014
 *      Author: srdja
 */

#include <stdlib.h>
#include <stdio.h>

#include "fs_io.h"

/**-----------------------------------------------------------------------------
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
int
fsio_write_txt(FS_Entry *e, char *buff, size_t buff_size)
{
    if (e->exists && (e->is_dir || !e->can_write))
        return -1;

    FILE *f = fopen(e->path, "w+");
    int   w = fprintf(f, buff);
    fclose(f);

    return w;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
int
fsio_write_txt_c(char *path, char *buff, size_t buff_size)
{
    FS_Entry *entry = fsent_new(path);
    if (!entry)
        return -1;

    int ret = fsio_write_txt(entry, buff, buff_size);

    fsent_destroy(entry);
    return ret;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
int
fsio_write_bin(FS_Entry *e, char *buff, size_t buff_size)
{
    if (e->exists && (e->is_dir || !e->can_write))
        return -1;

    FILE *f = fopen(e->path, "w+");
    int   w = fwrite(buff, 1, e->size, f);
    fclose(f);

    return w;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
int
fsio_write_bin_c(char *path, char *buff, size_t buff_size)
{
    FS_Entry *entry = fsent_new(path);
    if (!entry)
        return -1;

    int ret = fsio_write_bin(entry, buff, buff_size);
    fsent_destroy(entry);

    return ret;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
int
fsio_read_txt_alloc(FS_Entry *e, char **buff)
{
    if (!e->exists || e->is_dir || !e->can_read)
        return -1;

    *buff = malloc(e->size + 1);

    FILE *f = fopen(e->path, "r");
    int   r = fread(*buff, 1, e->size, f);
    fclose(f);

    (*buff)[e->size] = '\0';

    return r;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
int
fsio_read_txt_alloc_c(char *path, char **buff)
{
    FS_Entry *entry = fsent_new(path);
    if (!entry)
        return -1;

    int ret = fsio_read_txt_alloc(entry, buff);
    fsent_destroy(entry);

    return ret;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
int
fsio_read_bin_alloc(FS_Entry *e, char **buff)
{
    if (!e->exists || e->is_dir || !e->can_read)
        return -1;

    *buff = malloc(e->size);

    FILE *f = fopen(e->path, "r");
    int   r = fread(*buff, 1, e->size, f);
    fclose(f);

    return r;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
int
fsio_read_bin_alloc_c(char *path, char **buff)
{
    FS_Entry *entry = fsent_new(path);
    if (!entry)
        return -1;

    int ret = fsio_read_bin_alloc(entry, buff);
    fsent_destroy(entry);

    return ret;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
int
fsio_read_txt(FS_Entry *e, char *buff, size_t buff_size)
{
    if (!e->exists || e->is_dir || !e->can_read)
        return -1;

    FILE *f = fopen(e->path, "r");
    int   r = fread(buff, 1, e->size, f);
    fclose(f);

    buff[e->size] = '\0';

    return r;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
int
fsio_read_txt_c(char *path, char *buff, size_t buff_size)
{
    FS_Entry *entry = fsent_new(path);
    if (!entry)
        return -1;

    int ret = fsio_read_txt(entry, buff, buff_size);
    fsent_destroy(entry);

    return ret;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
int
fsio_read_bin(FS_Entry *e, char *buff, size_t buff_size)
{
    if (!e->exists || e->is_dir || !e->can_read)
        return -1;

    FILE *f = fopen(e->path, "r");
    int   r = fread(buff, 1, e->size, f);
    fclose(f);

    return r;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
int
fsio_read_bin_c(char *path, char *buff, size_t buff_size)
{
    FS_Entry *entry = fsent_new(path);
    if (!entry)
        return -1;

    int ret = fsio_read_bin(entry, buff, buff_size);
    fsent_destroy(entry);

    return ret;
}
