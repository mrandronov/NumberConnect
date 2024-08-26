#ifndef __SOUND_LIB_H__
#define __SOUND_LIB_H__

#include "SDL2/SDL_mixer.h"

#include "config.h"

config_t*               config;

typedef struct sound_t
{
        Mix_Music*              sound;
        void                    ( *play )( struct sound_t* self );
        void                    ( *destroy )( struct sound_t* self );
} Sound;

Sound*          start_game_sound;
Sound*          button_sound;
Sound*          cell_connect_sound;
Sound*          cell_join_sound;

void            sounds_init();
void            sounds_destroy();

#endif /* __SOUND_LIB_H__ */
