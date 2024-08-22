#ifndef __GAME_SCREEN_LIB_H__
#define __GAME_SCREEN_LIB_H__

#include "SDL_events.h"

#include "config.h"

config_t*       config;

typedef struct screen_t
{
        void            ( *render )( struct screen_t* self );
        void            ( *handle_event )( struct screen_t* self, SDL_Event* event );
        void            ( *destroy )( struct screen_t* self );
} Screen;

Screen*            game_screen_create();
Screen*            menu_screen_create();

#endif /* __GAME_SCREEN_LIB_H__ */
