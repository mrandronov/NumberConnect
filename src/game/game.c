
#include "game/game.h"

/*
        Use a single global to manage the game state.
 */

game_t*         globalGame;

game_t*
game_init( int rows, int cols, char humanPiece, char computerPiece, enum computer_type_t computerType )
{
        game_t*                 game = ( game_t* ) malloc( sizeof( game_t ) );

        game->humanPlayer = init_human_player( humanPiece );
        if ( computerType == SMART )
                game->computerPlayer = init_smart_computer_player( computerPiece );
        else if ( computerType == RANDOM )
                game->computerPlayer = init_random_computer_player( computerPiece );

        game->moveHistory = ( move_t* ) malloc( rows * cols * sizeof( move_t ) );
        game->board = board_create( rows, cols );

        printf( "Human piece is %c\n", game->humanPlayer->piece );
        printf( "Computer piece is %c\n", game->computerPlayer->piece );

        return game;
}

char
get_horizontal_winner( board_t* board, char playerPiece, char computerPiece )
{
        for( int i = 0; i < board->rows; i++ )
        {
                int                     playerPieceCount = 0;
                int                     computerPieceCount = 0;

                for ( int j = 0; j < board->cols; j++ )
                {
                        char checkingPiece = board->cells[ i ][ j ].piece;

                        if ( checkingPiece == playerPiece )
                        {
                                playerPieceCount++;
                        }
                        else if ( checkingPiece == computerPiece )
                        {
                                computerPieceCount++;
                        }

                }

                if ( playerPieceCount == board->cols )
                {
                        return playerPiece;
                }

                if ( computerPieceCount == board->cols )
                {
                        return computerPiece;
                }
        }

        return ' ';
}

char
get_vertical_winner( board_t* board, char playerPiece, char computerPiece )
{
        for( int i = 0; i < board->cols; i++ )
        {
                int                     playerPieceCount = 0;
                int                     computerPieceCount = 0;

                for ( int j = 0; j < board->rows; j++ )
                {
                        char checkingPiece = board->cells[ j ][ i ].piece;

                        if ( checkingPiece == playerPiece )
                        {
                                playerPieceCount++;
                        }
                        else if ( checkingPiece == computerPiece )
                        {
                                computerPieceCount++;
                        }

                }

                if ( playerPieceCount == board->cols )
                {
                        return playerPiece;
                }

                if ( computerPieceCount == board->cols )
                {
                        return computerPiece;
                }
        }

        return ' ';
}

char
get_diagonal_winner( board_t* board, char playerPiece, char computerPiece ) 
{
        int                     playerPieceCount = 0;
        int                     computerPieceCount = 0;

        for( int i = 0; i < board->rows; i++ )
        {
                char                    checkingPiece = board->cells[ i ][ i ].piece;

                if ( checkingPiece == playerPiece )
                {
                        playerPieceCount++;
                }
                else if ( checkingPiece == computerPiece )
                {
                        computerPieceCount++;
                }
        }

        if ( playerPieceCount == board->cols )
        {
                return playerPiece;
        }
        if ( computerPieceCount == board->cols )
        {
                return computerPiece;
        }

        playerPieceCount = 0;
        computerPieceCount = 0;

        for( int i = 0; i < board->rows; i++ )
        {
                char            checkingPiece = board->cells[ i ][ board->cols - i - 1 ].piece;

                if ( checkingPiece == playerPiece )
                {
                        playerPieceCount++;
                }
                else if ( checkingPiece == computerPiece )
                {
                        computerPieceCount++;
                }
        }

        if ( playerPieceCount == board->cols )
        {
                return playerPiece;
        }
        if ( computerPieceCount == board->cols )
        {
                return computerPiece;
        }

        return ' ';
}

char
find_winner( board_t* board, char playerPiece, char computerPiece )
{
        char            winner;

        /*
                Check horizontal condition
         */

        winner = get_horizontal_winner( board, playerPiece, computerPiece );

        if ( winner != ' ' )
        {
                return winner;
        }

        /*
                Check vertical condition
         */

        winner = get_vertical_winner( board, playerPiece, computerPiece );

        if ( winner != ' ' )
        {
                return winner;
        }

        /*
                For now, do cross diagonal checks assuming
                the game board is a square.
         */

        winner = get_diagonal_winner( board, playerPiece, computerPiece );

        if ( winner != ' ' )
        {
                return winner;
        }

        /*
                If no winner has been determined, just return an
                empty space ( ' ' ) character.
         */

        return ' ';
}

char
get_winner( game_t* game )
{
        return find_winner( game->board, game->humanPlayer->piece, game->computerPlayer->piece );
}

int
apply_move( board_t* board, move_t* move )
{
        set_board_piece( board, move->row-1, move->col-1, move->piece );

        return 0;
}

int
game_loop( game_t* game )
{
        player_t*               currentPlayer = game->humanPlayer;
        char                    opposingPiece = game->computerPlayer->piece;

        char                    winner = ' ';

        while ( winner == ' ' && num_vacant_cells( game->board ) != 0 )
        {
                move_t*         playerMove = currentPlayer->makeMove( game->board,
                                                                currentPlayer->piece,
                                                                opposingPiece );

                apply_move( game->board, playerMove );

                if ( currentPlayer == game->humanPlayer )
                {
                        currentPlayer = game->computerPlayer;
                        opposingPiece = game->humanPlayer->piece;
                }
                else
                {
                        currentPlayer = game->humanPlayer;
                        opposingPiece = game->computerPlayer->piece;
                }

                winner = get_winner( game );
        }

        return 0;
}

int
game_free( game_t* game )
{
        free( game->moveHistory );
        board_free( game->board );
        free( game );

        return 0;
}

