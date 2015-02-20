#include <vector.h>
#include <GL/glew.h>

#include "draw.h"
#include "model.h"
#include "shaders.h"
#include "../cl_scene.h"
#include "matrix.h"
#include "window.h"

void render_on_screen_size_changed(int width, int height);

static SDL_Window    *window = NULL;
static SDL_GLContext *glc;

static GLfloat modelM[16];
static GLfloat viewM[16];
static GLfloat projM[16];

int width;
int height;

void draw_model(Mesh *m, Texture *t);

void
render_init(SDL_Window *win, SDL_GLContext *gl)
{
    window = win;
    glc    = gl;

    glViewport(0, 0, 1920, 1080);

    float top    = 1080 /  2;
    float bottom = 1080 / -2;
    float left   = 1920 / -2;
    float right  = 1920 /  2;

    glClearColor(0.0f, 0.0f, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    matrix_ortho(projM, 0, 1920, 0, 1080, 1, -1);
    matrix_look_at(viewM, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

    screen_add_resolution_change_listener(render_on_screen_size_changed);
}

void
render_on_screen_size_changed(int width, int height)
{
    // most common aspect ratio is 16:9

    //fprintf(stdout, "calling back\n");

    glViewport(0, 0, width, height);

    //float top    = height /  2;
    //float bottom = height / -2;
    //float left   = width  / -2;
    //float right  = width  /  2;

    matrix_ortho(projM, 0, width, 0, height, 1, -1);
}

void
clear_frame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
render_update(CLScene_S *scene)
{
    clear_frame();
    // main render stage

    int i;
    for (i = 0; i < scene->static_ent_count; i++) {
        Entity *e = vector_get(scene->static_ent, i);
        draw_model(&(e->mesh), &(e->textures[e->texture_i]));
    }

    SDL_GL_SwapWindow(window);
}

void
draw_background()
{

}

void
draw_model(Mesh *mesh, Texture *texture)
{
    GLint shader = get_shader_prog(SHADER_BASIC);
    glUseProgram(shader);

    GLint sampler = glGetUniformLocation(shader, "tex");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->gl_id);
    glUniform1i(sampler, 0);

    glBindVertexArray(mesh->vao);

    vbuffer_bind(&(texture->coord_buff));
    GLint t = glGetAttribLocation(shader, "v_texcoord");
    glEnableVertexAttribArray(t);

    glVertexAttribPointer(t,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*)(texture->subtex_i * 8 * sizeof(GLfloat)));

    GLint mvpptr = glGetUniformLocation(shader, "mvp");

    float modelViewM[16];
    float mvp[16];

    matrix_multiply(modelViewM, mesh->modelM, viewM);
    matrix_multiply(mvp, projM, modelViewM);

    glUniformMatrix4fv(1, 1, GL_FALSE, mvp);

    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    glUseProgram(0);
}
