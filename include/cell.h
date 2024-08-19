#ifndef __LIB_CELL_H__
#define __LIB_CELL_H__

#include "SDL_rect.h"

#include "config.h"
#include "number_set.h"
#include "animate.h"

#define BOARD_WIDTH 5
#define BOARD_HEIGHT 7
#define BOARD_SIZE BOARD_WIDTH * BOARD_HEIGHT

#define CELL_WIDTH 125
#define CELL_HEIGHT 122
#define CELL_PADDING 20
#define CELL_BOX_WIDTH CELL_WIDTH - CELL_PADDING
#define CELL_BOX_HEIGHT CELL_HEIGHT - CELL_PADDING


config_t*       config;

typedef struct cell_t
{
        int             row;
        int             col;
        SDL_Rect        box;
        SDL_Color       *color;
        Tile*           tile;
        Animation*      animation;

        void            ( *render )( struct cell_t* );
        void            ( *get_center )( struct cell_t*, int* center_x, int* center_y );
        bool            ( *on_hover )( struct cell_t* );
        void            ( *destroy )( struct cell_t* );
} Cell;

typedef struct board_t
{
        int             width;
        int             height;
        Cell**          cells;

        void            ( *render )( struct board_t* );
        Cell*           ( *on_hover )( struct board_t* );
        void            ( *trickle_down )( struct board_t* );
        void            ( *destroy )( struct board_t* );
} Board;

extern Board*           board;

Cell*                   cell_create( int x, int y, int row, int col );
Board*                  cell_board_create();

#endif /* __LIB_CELL_H__ */
