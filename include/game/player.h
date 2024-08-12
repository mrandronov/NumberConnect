#ifndef __LIB_PLAYER_H__
#define __LIB_PLAYER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "game/board.h"

typedef struct
{
        char*           name;
        char            piece;
        move_t*         (*makeMove)( board_t*, char, char );
} player_t;

player_t* init_human_player( char playerPiece );

player_t* init_smart_computer_player( char computerPiece );

player_t* init_random_computer_player( char computerPiece );

move_t* get_player_move( board_t* board, char playerPiece, char computerPiece );

move_t* get_smart_computer_move( board_t* board, char playerPiece, char computerPiece );

move_t* get_random_computer_move( board_t* board, char playerPiece, char computerPiece );

#endif /* __LIB_PLAYER_H__ */
