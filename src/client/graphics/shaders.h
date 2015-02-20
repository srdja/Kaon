/*
 * shaders.h
 *
 *  Created on: Oct 10, 2014
 *      Author: srdja
 */

#ifndef __SHADERS_H__
#define __SHADERS_H__

//#include <GL/gl.h>
#include <SDL2/SDL_opengl.h>

typedef enum {
    SHADER_BASIC,
    NUM_SHADERS
} Shader_E;

void  shaders_init    ();
GLint get_shader_prog (Shader_E shader);

#endif /* SHADERS_H_ */
