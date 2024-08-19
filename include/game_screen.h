#ifndef __GAME_SCREEN_LIB_H__
#define __GAME_SCREEN_LIB_H__

#include "SDL_events.h"

#include "config.h"

config_t*       config;

typedef struct game_screen_t
{
        void            ( *render )( struct game_screen_t* self );
        void            ( *handle_event )( struct game_screen_t* self, SDL_Event* event );
        void            ( *destroy )( struct game_screen_t* self );
} Game_Screen;

Game_Screen*            game_screen_create();

#endif /* __GAME_SCREEN_LIB_H__ */
