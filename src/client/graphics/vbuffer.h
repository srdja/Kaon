/*
 * vbuffer.h
 *
 *  Created on: Oct 15, 2014
 *      Author: srdja
 */

#ifndef VBUFFER_H_
#define VBUFFER_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct vbuffer_s VBuffer;

enum VBUFFER_ACCESS {
    VBUFF_READ,
    VBUFF_WRITE
};

struct vbuffer_s {
    size_t size;
    bool   is_mapped;
    GLuint vbo;
    GLenum usage;
    GLenum type;
};

VBuffer *vbuffer_new          (GLenum type, GLenum usage);
void     vbuffer_destroy      (VBuffer *buff);

void     vbuffer_new_fill     (VBuffer *buff, GLenum type, GLenum usage); // FIXME rename to something less misleading
void     vbuffer_destroy_fill (VBuffer *buff);

bool     vbuffer_allocate     (VBuffer *buff, void *data, size_t len);
void     vbuffer_deallocate   (VBuffer *buff);
void     vbuffer_update       (VBuffer *buff, void *data);
void     vbuffer_update_range (VBuffer *buff, void *data, size_t offset, size_t len);
void     vbuffer_bind         (VBuffer *buff);
void     vbuffer_unbind       (VBuffer *buff);

#endif /* VBUFFER_H_ */
