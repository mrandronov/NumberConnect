#ifndef __LIB_GLOBAL_H__
#define __LIB_GLOBAL_H__

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "SDL2/SDL.h"

#include "config.h"

typedef struct
{
        bool                    is_game_running;
        char*                   theme_path;
        char                    winner;
        
        SDL_Window*             window;
        SDL_Renderer*           renderer;
        config_t*               config;
} global_t;

void global_init();

global_t global;

#endif /* __LIB_GLOBAL_H__ */
