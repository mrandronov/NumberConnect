#ifndef __WINDOW_LIB_H__
#define __WINDOW_LIB_H__

#include <stdint.h>

#include "SDL_video.h"

#include "config.h"
#include "game_screen.h"

config_t*               config;

enum screen_type
{
        GAME,
        MENU,
        OPTIONS
};

typedef struct window_t
{
        int                     width;
        int                     height;
        char*                   title;

        float                   fps_cap;
        float                   fps_cap_in_ms;
        uint64_t                start;

        SDL_Window*             window;

        enum screen_type        current_screen;
        Game_Screen*            game_screen;

        void                    ( *init )( struct window_t* self );
        void                    ( *handle_events )( struct window_t* self );
        void                    ( *render )( struct window_t* self );
        void                    ( *run )( struct window_t* self );
        void                    ( *destroy )( struct window_t* self );
} Window;

Window*
window_create();

#endif /* __WINDOW_LIB_H__ */
