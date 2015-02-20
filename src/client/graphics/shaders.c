/*
 * shaders.c
 *
 *  Created on: Oct 10, 2014
 *      Author: srdja
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <GL/glew.h>

#include "../console.h"
#include "../../file-system/fs_entry.h"
#include "../../file-system/fs_io.h"
#include "../../file-system/paths.h"

#include "shaders.h"


static GLint create_program      (GLint vertShader, GLint fragShader);
static GLint compile             (GLint type, char *source);
static void  shader_program_init (Shader_E shader, char *frag_name, char *vert_name);

GLint shaders[NUM_SHADERS];

/*------------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
void
shaders_init()
{
    shader_program_init(SHADER_BASIC, "basic_fragment.gls", "basic_vertex.gls");
}

/*------------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
GLint
get_shader_prog(Shader_E shader)
{
    return shaders[shader];
}

/**-----------------------------------------------------------------------------
 *
 * @param[in] shader_name the name of the shader file
 * @param[in] type the shader type
 *
 * @return the handle of the loaded shader
 -----------------------------------------------------------------------------*/
static GLint
shader_load(char *shader_name, GLenum type)
{
    GLint  shader;
    char  *shader_buff = NULL;
    char  *shader_path = paths_shader(shader_name);

    fsio_read_txt_alloc_c(shader_path, &shader_buff);

    if (shader_buff == NULL) {
        console_msg_post("Error loading shader: %s\n", shader_path);
        free(shader_path);
        return 0;
    }

    shader = compile(type, shader_buff);

    free(shader_path);
    free(shader_buff);

    return shader;
}

/*------------------------------------------------------------------------------
 *
 * @param[in] shader
 * @param[in] frag_name
 * @param[in] vert_name
 -----------------------------------------------------------------------------*/
static void
shader_program_init(Shader_E shader, char *frag_name, char *vert_name)
{
    GLint v = shader_load(vert_name, GL_VERTEX_SHADER);
    GLint f = shader_load(frag_name, GL_FRAGMENT_SHADER);
    GLint p = create_program(v, f);

    shaders[shader] = p;
}

/**-----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
static GLint
compile(GLint type, char *source)
{
    GLint id = glCreateShader(type);

    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    GLint status  = GL_FALSE;
    GLint msg_len = 0;

    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &msg_len);

    if (status == GL_FALSE) {
        char msg[msg_len];
        glGetShaderInfoLog(id, msg_len, NULL, &msg);
        console_msg_post(msg);
    }
    return id;
}

/**-----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
static GLint
create_program(GLint vertShader, GLint fragShader)
{
    GLint prog = glCreateProgram();

    glAttachShader(prog, vertShader);
    glAttachShader(prog, fragShader);
    glLinkProgram(prog);

    GLint status  = GL_FALSE;
    GLint msg_len = 0;

    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &msg_len);

    if (status == GL_FALSE) {
        char msg[msg_len];
        glGetProgramInfoLog(prog, msg_len, NULL, &msg);
        console_msg_post(msg);
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return prog;
}
