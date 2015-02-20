#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL_opengl.h>
#include <string.h>

#include "vbuffer.h"
#include "../console.h"

static bool  vbuffer_unmap     (VBuffer *buff);
static void *vbuffer_map_range (VBuffer *buff, enum VBUFFER_ACCESS acc,
                                GLintptr off, size_t len);

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
VBuffer *
vbuffer_new(GLenum type, GLenum usage)
{
    VBuffer *buff = malloc(sizeof(VBuffer));

    buff->is_mapped = false;
    buff->size      = 0;
    buff->vbo       = 0;
    buff->usage     = usage;
    buff->type      = type;

    return buff;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
void
vbuffer_destroy(VBuffer *buff)
{
    vbuffer_deallocate(buff);
    free(buff);
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
void
vbuffer_new_fill(VBuffer *buff, GLenum type, GLenum usage)
{
    buff->is_mapped = false;
    buff->size      = 0;
    buff->vbo       = 0;
    buff->usage     = usage;
    buff->type      = type;
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
void
vbuffer_destroy_fill(VBuffer *buff)
{
    vbuffer_deallocate(buff);
    memset(buff, 0, sizeof(VBuffer));
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
void
vbuffer_bind(VBuffer *buff)
{
    glBindBuffer(buff->type, buff->vbo);
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
bool
vbuffer_allocate(VBuffer *buff, void *data, size_t data_len)
{
    if (buff->vbo) {
        console_msg_post("VBuffer allocation failed:"
                        " buffer already allocated!\n");
        return false;
    }

    GLuint buffer = 0;

    glGenBuffers(1, &buffer);
    glBindBuffer(buff->type, buffer);
    glBufferData(buff->type, data_len, data, buff->usage);

    if (glGetError() == GL_OUT_OF_MEMORY) {
        console_msg_post("Vertex Buffer allocation failed: Out of memory!\n");
        return false;
    }
    buff->vbo = buffer;

    return true;
}

/*******************************************************************************
 * Frees the specified vertex buffer from VRAM.
 *
 * @param[in] buff
 ******************************************************************************/
void
vbuffer_deallocate(VBuffer *buff)
{
    if (buff->is_mapped)
        vbuffer_unmap(buff);

    glDeleteBuffers(1, &(buff->vbo));
}

/** ----------------------------------------------------------------------------
 * Updates the entire buffer with new data. The data length is assumed to be the
 * same as the buffer length.
 *
 * @param[in] buff the buffer being updated
 * @param[in] data the new data
 -----------------------------------------------------------------------------*/
void
vbuffer_update(VBuffer *buff, void *data)
{
    vbuffer_update_range(buff, data, 0, buff->size);
}

/** ----------------------------------------------------------------------------
 * Updates a portion of the buffer specified by the offset and len.
 *
 * @param[in] buff   the buffer being updated
 * @param[in] data   the new data
 * @param[in] offset the offset in the buffer from which the new data should be
 *                   written
 * @param[in] len    the total length of the data being written to the buffer
 -----------------------------------------------------------------------------*/
void
vbuffer_update_range(VBuffer *buff, void *data, size_t offset, size_t len)
{
    if (buff->is_mapped)
        return;

    if (offset > len || len > buff->size) {
        console_msg_post("VBuffer update out of range!\n");
        return;
    }

    void *buffer = (void*) vbuffer_map_range(buff, VBUFF_WRITE, offset, len);
    memcpy(buffer, data, len);
    vbuffer_unmap(buff);
}

/*******************************************************************************
 *
 * @param[in]
 * @param[in]
 *
 * @return
 ******************************************************************************/
static void *
vbuffer_map_range(VBuffer *buff, enum VBUFFER_ACCESS access,
                       GLintptr offset, size_t len)
{
    void *mapped_data = NULL;

    glBindBuffer(buff->type, buff->vbo);

    switch (access) {
    case VBUFF_READ:
        mapped_data = glMapBufferRange(buff->type, offset, len,
                GL_MAP_READ_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
        break;
    case VBUFF_WRITE:
        mapped_data = glMapBufferRange(buff->type, offset, len,
                GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
        break;
    }

    if (!mapped_data) {
        console_msg_post("VBuffer range map failed!\n");
        return mapped_data;
    }
    buff->is_mapped = true;

    return mapped_data;
}

/*******************************************************************************
 *
 * @param[in] buff
 ******************************************************************************/
static bool
vbuffer_unmap(VBuffer *buff)
{
    glBindBuffer(buff->type, buff->vbo);

    if (glUnmapBuffer(buff->type)) {
        buff->is_mapped = false;
        return true;
    }
    return false;
}
