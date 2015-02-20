/*
 * mesh.h
 *
 *  Created on: Oct 7, 2014
 *      Author: srdja
 */

#ifndef MESH_H_
#define MESH_H_

//#include <SDL/SDL_opengl.h>
#include <GL/gl.h>

#include "../../common/vec2.h"
#include "vbuffer.h"

/**
 * Each mesh has it's own set of matrices that are set by the movement / physics
 * systems
 */

typedef struct mesh_s {
    GLint   vao;

    VBuffer vertices;
    VBuffer indices;

    GLuint vertex_count;
    GLuint texcoord_count;

    GLfloat modelM[16];
    GLfloat modelViewM [16];
    GLfloat projectionM[16];

    Vec2 scale;
    Vec2 trans;
    Vec2 rotate;

    /* 4 vertices * x,y,z */
    GLfloat bbox[12];
    GLfloat bbox_transformed[12];
} Mesh;


void  get_screen_position       (Mesh *mesh, GLfloat *coord_array);
void  mesh_update_bounding_box  (Mesh *mesh);
void  mesh_set_rectangle        (Mesh *m, int w, int h);
Mesh *mesh_new_rectangle        (int w, int h);
Mesh *mesh_new_triangle         ();


#endif /* MESH_H_ */
