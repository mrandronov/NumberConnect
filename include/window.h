#ifndef __WINDOW_LIB_H__
#define __WINDOW_LIB_H__

#include <stdint.h>

#include "SDL_video.h"

#include "config.h"
#include "screen.h"

config_t*                       config;

typedef struct window_t
{
        int                     width;
        int                     height;
        char*                   title;

        float                   fps_cap;
        float                   fps_cap_in_ms;
        uint64_t                start;

        SDL_Window*             window;

        Screen*                 current_screen;
        Screen*                 game_screen;
        Screen*                 menu_screen;
        Screen*                 option_screen;

        void                    ( *init )( struct window_t* self );
        void                    ( *handle_events )( struct window_t* self );
        void                    ( *render )( struct window_t* self );
        void                    ( *run )( struct window_t* self );
        void                    ( *destroy )( struct window_t* self );
} Window;

extern Window*                  window;

Window*                         window_create();

#endif /* __WINDOW_LIB_H__ */
