#ifndef __LIB_CONFIG_H__
#define __LIB_CONFIG_H__

#include <stdbool.h>

#include "SDL_render.h"

typedef struct
{
        bool                    is_game_running;
        bool                    is_sound_enabled;
        int                     playerScore;
        char*                   theme_path;
        char*                   font_path;

        SDL_Renderer*           renderer;

        int                     mouse_x;
        int                     mouse_y;

} config_t;

void                            config_init();
void                            config_destroy();

extern config_t*                config;

#endif /* __LIB_CONFIG_H__*/
