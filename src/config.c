
#include "config.h"

void
config_init()
{
        config = ( config_t* ) malloc( sizeof( config_t ) );

        config->playerScore = 0;
        config->is_game_running = true;

        config->font_path = "./fonts/8bitOperatorPlus8-Regular.ttf";

        config->mouse_x = 0;
        config->mouse_y = 0;
}

