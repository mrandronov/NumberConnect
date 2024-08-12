#ifndef __LIB_BOARD_DISPLAY_H__
#define __LIB_BOARD_DISPLAY_H__

#include <stdbool.h>

#include "selector.h"
#include "config.h"

#define X_OFFSET 15
#define Y_OFFSET 11
#define CELL_WIDTH 190
#define CELL_HEIGHT 170

typedef struct
{
        selector_t**              cells;
} board_display_t;

board_display_t*        board_init( SDL_Renderer* renderer, config_t* config );
void                    draw_board( SDL_Renderer* rendrer );
void                    highlight_board();
void                    disable_board();
void                    board_destroy();
void                    draw_grid_lines( SDL_Renderer* renderer );

#endif /* __LIB_BOARD_DISPLAY_H__ */
