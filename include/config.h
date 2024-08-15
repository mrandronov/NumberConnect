#ifndef __LIB_CONFIG_H__
#define __LIB_CONFIG_H__

#include <stdbool.h>

#include "SDL_render.h"
#include "SDL_video.h"

typedef struct
{
        bool                    is_game_running;
        int                     playerScore;
        char*                   font_path;
        SDL_Window*             window;
        SDL_Renderer*           renderer;

        int                     mouse_x;
        int                     mouse_y;

} config_t;

void config_init();

extern config_t*                config;

#endif /* __LIB_CONFIG_H__*/
