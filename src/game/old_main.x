#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "player.h"

/*
        TODO: Use custom characters for computer and players?
        TODO: Validate user input.
 */

char
get_player_piece()
{
        size_t                  bufferSize = 64;
        char*                   inputBuffer = ( char* ) malloc( bufferSize * sizeof( char ) );

        printf( "Please enter your piece marker ( 'O' or 'X' ): " );

        getline( &inputBuffer, &bufferSize, stdin );

        char                    piece = inputBuffer[ 0 ];
        free( inputBuffer );

        if ( piece == 'O' )
        {
                return O_PIECE;
        }

        return X_PIECE;
}

int
main( int argc, char* argv[] )
{
        char                    humanPiece = get_player_piece();
        char                    computerPiece = ( humanPiece == O_PIECE ) ? X_PIECE : O_PIECE;

        game_t*                 game = game_init( 3, 3, humanPiece, computerPiece );

        game_loop( game );

        game_free( game );

        return 0;
}

