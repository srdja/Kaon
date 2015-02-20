/*
 * mesh.c

 *
 *  Created on: Oct 7, 2014
 *      Author: srdja
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

#include "mesh.h"
#include "matrix.h"
#include "vbuffer.h"

/** ----------------------------------------------------------------------------
 * Dynamically allocates a new square mesh.
 *
 *
 -----------------------------------------------------------------------------*/
Mesh *mesh_new_rectangle(int w, int h)
{
    Mesh *m = calloc(1, sizeof(Mesh));
    mesh_set_rectangle(m, w, h);
    return m;
}

/** ----------------------------------------------------------------------------
 *
 *
 *
 -----------------------------------------------------------------------------*/
void mesh_set_rectangle(Mesh *m, int w, int h)
{
    m->scale.x = 1;
    m->scale.y = 1;

    m->trans.x = 0;
    m->trans.y = 0;

    m->rotate.x = 0;
    m->rotate.y = 0;

    GLfloat ratio = w / h;

    GLfloat vert[12];

    vert[0] = -0.5f; vert[1 ] =  0.5f; vert[2 ] = 0.0f;
    vert[3] = -0.5f; vert[4 ] = -0.5f; vert[5 ] = 0.0f;
    vert[6] =  0.5f; vert[7 ] = -0.5f; vert[8 ] = 0.0f;
    vert[9] =  0.5f; vert[10] =  0.5f; vert[11] = 0.0f;

    GLuint indices[6] = {
            0,1,2,0,3,2
    };

    memcpy(m->bbox, vert, 12 * sizeof(GLfloat));
    // set position and texture coordinates
    vbuffer_new_fill(&(m->vertices), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vbuffer_allocate(&(m->vertices), vert, sizeof(vert));

    vbuffer_new_fill(&(m->indices), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    vbuffer_allocate(&(m->indices), indices, sizeof(indices));

    glGenVertexArrays(1, &(m->vao));
    glBindVertexArray(m->vao);

    vbuffer_bind(&(m->indices));
    vbuffer_bind(&(m->vertices));

    glEnableVertexAttribArray(0); // enum?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
}

void mesh_set_qube()

/** ----------------------------------------------------------------------------
 *
 *
 *
 -----------------------------------------------------------------------------*/
Mesh *mesh_new_triangle()
{
    Mesh *m = calloc(1, sizeof(Mesh));

    m->scale.x = 1;
    m->scale.y = 1;

    GLfloat vert[12];

    vert[0] =  0.0f; vert[1 ] =  0.5f; vert[2 ] = 0.0f;
    vert[3] = -0.5f; vert[4 ] = -0.5f; vert[5 ] = 0.0f;
    vert[6] =  0.5f; vert[7 ] = -0.5f; vert[8 ] = 0.0f;

    glGenVertexArrays(1, &(m->vao));
    glBindVertexArray(m->vao);

    // set position and texture coordinates
    vbuffer_new_fill(&(m->vertices), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vbuffer_allocate(&(m->vertices), vert, sizeof(vert));

    vbuffer_bind(&(m->vertices));
    glEnableVertexAttribArray(0); // enum?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    return m;
}

/** ----------------------------------------------------------------------------
 *
 *
 *
 -----------------------------------------------------------------------------*/
void mesh_destroy(Mesh *mesh)
{
    // properly unbind everything
}

/** ----------------------------------------------------------------------------
 *
 *
 *
 -----------------------------------------------------------------------------*/
Mesh *mesh_new(GLfloat *vertices)
{
    Mesh *m = calloc(1, sizeof(Mesh));
    return m;
}

/** ----------------------------------------------------------------------------
 *
 *
 *
 -----------------------------------------------------------------------------*/
void mesh_update_bounding_box(Mesh *mesh)
{
    matrix_identity(mesh->modelM);
    matrix_translate(mesh->modelM, mesh->trans.x, mesh->trans.y, 0);
    matrix_scale(mesh->modelM, 119.5, 119.5, 1.0);

    int i;
    for (i = 0; i < 12; i += 3) {
        float v[3] = {
                mesh->bbox[i  ],
                mesh->bbox[i+1],
                mesh->bbox[i+2]
        };

        matrix_multiply_vertex_ip(mesh->modelM, v, 0);

        mesh->bbox_transformed[i  ] = v[0];
        mesh->bbox_transformed[i+1] = v[1];
        mesh->bbox_transformed[i+2] = v[2];
    }
}

/** ----------------------------------------------------------------------------
 *
 *
 *
 -----------------------------------------------------------------------------*/
void mesh_calculate_bounding_box(Mesh *s)
{

}
