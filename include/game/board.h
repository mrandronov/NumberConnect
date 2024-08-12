#ifndef __LIB_BOARD_H__
#define __LIB_BOARD_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define EMPTY_PIECE ' '
#define X_PIECE 'X'
#define O_PIECE 'O'

typedef struct
{
        char            piece;
} cell_t;

typedef struct
{
        int             row;
        int             col;
        char            piece;
} move_t;

typedef struct
{
        int             rows;
        int             cols;
        cell_t**        cells;
} board_t;

board_t* board_create( int rows, int cols );

int set_board_piece( board_t* board, int x, int y, char piece );

void print_board( board_t* board );

int num_vacant_cells( board_t* board );

int board_free( board_t* board );

#endif /* __LIB_BOARD_H__ */
