#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <vector.h>
#include <hashtable.h>
#include <stdlib.h>

#include "window.h"
#include "../console.h"


static void load_defaults          ();
static void load_settings          ();
static void write_settings         ();
static void get_display_resolution (int display, DisplayResolution_S *res);
static void notify_rcl             (int w, int h);

/* Resolution change listener */
typedef struct rcl_s {
    void (*rcl) (int w, int h);
} RCL_S;

int width  = 1920;
int height = 1080;

SDL_Window    *window;
SDL_GLContext *gl;

/*  */
Vector *rcl;

static int active_display;
static int n_displays;

static DisplayMode_E current_dmode;

/*
 * The current display mode
 */
SDL_DisplayMode mode;

// TODO Resolution settings

bool
window_init()
{
    active_display = 0;
    rcl = vector_new();

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        console_msg_post("Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("GAME",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              width,
                              height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) {
        console_msg_post("Error while creating the window: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetWindowGrab(window, SDL_TRUE);

    gl = SDL_GL_CreateContext(window);

    if (!gl) {
        console_msg_post("Error initializing OpenGL: %s\n", SDL_GetError());
        return false;
    }
    //SDL_GL_MakeCurrent(window, gl);
    glewExperimental = GL_TRUE;
    int err = glewInit();

    if (err == GLEW_OK)
        console_msg_post("initializing glew\n");
    else
        console_msg_post("glew has not initialized correctly\n");

    // v-sync
    SDL_GL_SetSwapInterval(1);

    if (SDL_GetWindowDisplayMode(window, &mode) < 0) {
        console_msg_post("Error while getting the display mode: %s\n",
                         SDL_GetError());
    }

    //screen_available_fullscreen_resolutions();

    screen_set_fullscreen_window();

    SDL_ShowCursor(0);

    return true;
}

/**
 * Displays range from 0 to n.
 */
int
screen_available_displays()
{
    return n_displays;
}

/**
 *
 */
void
screen_set_active_display(int display)
{
    if (display > n_displays) {
        console_msg_post(
                "Display %d not found, reverting to dafault display 0",display);
        if (active_display != 0) {
            SDL_SetWindowPosition(window, 0, 0);
        }
        return;
    }
    SDL_Rect dbounds;
    if (!SDL_GetDisplayBounds(display, &dbounds)) {
        console_msg_post("Unable to obtain display information for display %d"
                         ": %s", SDL_GetError());
        return;
    }
/*
    DisplayResolution_S resolution;
    get_display_resolution(display, &resolution);

    SDL_SetWindowPosition(window, dbounds->x, dbounds->y);
    SDL_SetWindowSize();*/
}

//void
//screen_set_

// TODO
void
screen_available_fullscreen_resolutions() // for the given aspect ratio
{
    static int display_in_use = 0; /* Only using first display */

    int i, display_mode_count;
    SDL_DisplayMode mode;
    Uint32 f;

    SDL_Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());

    display_mode_count = SDL_GetNumDisplayModes(display_in_use);
    if (display_mode_count < 1) {
        SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
        return;
    }
    SDL_Log("SDL_GetNumDisplayModes: %i", display_mode_count);

    for (i = 0; i < display_mode_count; ++i) {
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
            return;
        }
        f = mode.format;

        SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i", i,
        SDL_BITSPERPIXEL(f), SDL_GetPixelFormatName(f), mode.w, mode.h);
    }

    SDL_GetCurrentDisplayMode(0, &mode);
    SDL_Log("Current mode %i\tbpp %i\t%s\t%i x %i", i,
            SDL_BITSPERPIXEL(f), SDL_GetPixelFormatName(f), mode.w, mode.h);
}

void
screen_set_display_in_use(int display_num)
{
    // display one / two
}

void
screen_set_windowed()
{
    if (SDL_SetWindowFullscreen(window, 0) < 0) {

    }
    int w;
    int h;
    SDL_SetWindowSize(window, 1024, 768);
    SDL_GetWindowSize(window, &w, &h);
    notify_rcl(w, h);
}

// + set video mode
void
screen_set_fullscreen()
{
    if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) < 0) {
        console_msg_post("Error while setting fullscreen: %s\n",
                         SDL_GetError());
    }
    int w;
    int h;
    SDL_GetWindowSize(window, &w, &h);
    notify_rcl(w, h);
}

void
screen_set_fullscreen_window()
{
    if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0) {
        console_msg_post("Error while setting fullscreen window: %s\n",
                         SDL_GetError());
                         fprintf(stdout, "error\n");
    }
    int w;
    int h;
    SDL_GetWindowSize(window, &w, &h);
    notify_rcl(w, h);
}

SDL_Window *
screen_get_sdl_window()
{
    return window;
}

void
screen_add_resolution_change_listener(void (*rcl_callback) (int, int))
{
    RCL_S *l = malloc(sizeof(RCL_S));
    l->rcl = rcl_callback;
    vector_add(rcl, l);
}

void
screen_remove_resolution_change_listener(void (*rcl_callback) (int, int))
{
    RCL_S *listener = vector_remove(rcl, rcl_callback);
    if (listener)
        free(listener);
}

static void
notify_rcl(int w, int h)
{
    int i;
    for (i = 0; i < vector_size(rcl); i++) {
        RCL_S *l = vector_get(rcl, i);
        l->rcl(w, h);
    }
}

