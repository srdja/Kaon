#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef enum display_mode DisplayMode_E;

typedef struct display_res {
    int w;
    int h;
} DisplayResolution_S;

enum display_mode {
    FULLSCREEN,
    FULLSCREEN_WINDOW,
    FULLSCREEN_WINDOW_BORDERLESS,
    WINDOW
};

 enum aspect_r {
     S
};

bool         window_init                             ();
bool         screen_destroy                          ();
void         screen_add_resolution_change_listener   (void (*rcl) (int w, int h));
void         screen_rm_resolution_change_listener    (void (*rcl) (int w, int h));
void         screen_set_fullscreen                   ();
void         screen_set_fullscreen_window            ();
void         screen_set_windowed                     ();

void         screen_available_fullscreen_resolutions ();
SDL_Window  *screen_get_sdl_window                   ();

#endif
