#ifndef __LIB_CONFIG_H__
#define __LIB_CONFIG_H__

#include "SDL_render.h"
#include "SDL_video.h"

#include "game/game.h"

typedef struct
{
        char                    playerMarker;
        char                    computerMarker;
        enum computer_type_t    computerType;
        int                     playerScore;
        int                     computerScore;
        game_t*                 game;

        char*                   font_path;

        bool                    is_game_ongoing;
        bool                    is_game_running;
        SDL_Window*             window;
        SDL_Renderer*           renderer;
        SDL_Texture*            line_texture;

        int                     mouse_x;
        int                     mouse_y;

} config_t;

void config_init();

extern config_t*                config;

#endif /* __LIB_CONFIG_H__*/
