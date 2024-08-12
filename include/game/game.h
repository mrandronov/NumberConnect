#ifndef __LIB_GAME_H__
#define __LIB_GAME_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "game/board.h"
#include "game/player.h"

enum computer_type_t
{
        RANDOM,
        SMART
};

typedef struct
{
        player_t*       humanPlayer;
        player_t*       computerPlayer;
        move_t*         moveHistory;
        board_t*        board;
} game_t;

game_t* game_init( int rows, int cols, char playerPiece, char computerPiece, enum computer_type_t computerType );

int apply_move( board_t* board, move_t* move );

char find_winner( board_t* board, char playerPiece, char computerPiece );

char get_winner( game_t* game );

int game_loop( game_t* game );

int game_free( game_t* game );

#endif /* __LIB_GAME_H__ */
