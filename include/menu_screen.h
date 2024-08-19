#ifndef __MENU_SCREEN_LIB_H__
#define __MENU_SCREEN_LIB_H__

#include "SDL_events.h"
#include "config.h"

config_t*               config;

typedef struct menu_screen_t
{
        void            ( *render )( struct menu_screen_t* self );
        void            ( *handle_event )( struct menu_screen_t* self, SDL_Event* event );
        void            ( *destroy )( struct menu_screen_t* self );
} Menu_Screen;

Menu_Screen*            menu_screen_create();

#endif /* __MENU_SCREEN_LIB_H__ */
