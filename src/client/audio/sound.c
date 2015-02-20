/*
 * sound.c
 *
 *  Created on: Nov 16, 2014
 *      Author: srdja
 */

#include <stdio.h>
#include <SDL2/SDL.h>

#include "../../file-system/paths.h"

#include "sound.h"

//The music that will be played
Mix_Music *music = NULL;


void sound_init()
{
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        return;
    }
/*
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;

    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
        fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
        exit(1);
    }*/

    if (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) {
        fprintf(stdout, "Error while initializing sound!\n");
        return;
    }

    char *res = paths_sound("Sign of Evil.ogg");

    //Load the music
    music = Mix_LoadMUS(res);

    //If there was a problem loading the music
    if( music == NULL ) {
        fprintf(stdout, "Error loading the sound file!\n");
        return;
    }

    Mix_PlayMusic(music, -1);
}
