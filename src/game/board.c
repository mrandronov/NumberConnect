
#include "game/board.h"

board_t*
board_create( int rows, int cols )
{
        board_t*                board = ( board_t* ) malloc( sizeof( board_t ) );

        board->rows = rows;
        board->cols = cols;

        board->cells = ( cell_t** ) malloc( rows * cols * sizeof( cell_t* ) );

        for ( int i = 0; i < rows; i++ )
        {
                board->cells[ i ] = ( cell_t* ) calloc( 1, cols * sizeof( cell_t ) );

                for( int j = 0; j < cols; j++ )
                {
                        board->cells[ i ][ j ].piece = EMPTY_PIECE;
                }
        }

        return board;
}

int
set_board_piece( board_t* board, int x, int y, char piece )
{
        board->cells[ x ][ y ].piece = piece;

        return 0;
}

void
print_board( board_t* board )
{
        for ( int i = 0; i < board->rows; i++ )
        {
                for ( int j = 0; j < board->cols; j++ )
                {
                        printf( "%c ", board->cells[ i ][ j ].piece );

                        if ( j != board->cols - 1 )
                        {
                                printf( "| " );
                        }
                }

                printf( "\n" );

                if ( i == board->rows - 1 )
                {
                        continue;
                }

                for ( int j = 0; j < board->cols * 3; j++ )
                {
                        printf( "-" );
                }
                printf( "\n" );
        }
}

int
num_vacant_cells( board_t* board )
{
        int             numVacantCells = 0;

        for ( int i = 0; i < board->rows; i++ )
        {
                for ( int j = 0; j < board->cols; j++ )
                {
                        if ( board->cells[ i ][ j ].piece == EMPTY_PIECE )
                        {
                                numVacantCells++;
                        }
                }
        }

        return numVacantCells;
}

int
board_free( board_t* board )
{
        free( board->cells );
        free( board );

        return 0;
}

