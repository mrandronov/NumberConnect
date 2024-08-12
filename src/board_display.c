
#include "board_display.h"
#include "color.h"

#include "game/board.h"
#include "game/game.h"

const int               rows = 3;
const int               cols = 3;

board_display_t*        board;

char                    currentPlayerMarker;
char                    currentComputerMarker;

int                     selector_start_index;
int                     selector_count;
int                     label_start_index = 0;
int                     label_count = 0;

typedef struct
{
        int row;
        int col;
} position_t;

position_t*
get_cell_position( selector_t* s )
{
        position_t*     pos = ( position_t* ) malloc( sizeof( position_t ) );

        for ( int i = 0; i < rows; i++ )
        {
                for ( int j = 0; j < cols; j++ )
                {
                        selector_t* checking = &board->cells[ i ][ j ];
                        if ( ( checking->box.x == s->box.x ) && ( checking->box.y == s->box.y ) )
                        {
                                pos->row = j;
                                pos->col = i;
                                return pos;
                        }
                }
        }
        return NULL;
}

int
computer_press( selector_t* s )
{
        s->is_selected = 1;

        char   mark[ 2 ];
        mark[ 0 ] = currentComputerMarker;
        mark[ 1 ] = '\0';

        /* label_create( mark, */
                        /* s->box.x + (s->box.w)/4,  */
                        /* s->box.y + (s->box.h / 4) - 5 ,  */
                        /* 160 ); */

        return 0;
}

int
cell_press( selector_t* s )
{
        position_t*     pos = get_cell_position( s );
        s->is_selected = 1;

        /*
                Mark the player's cell.
         */

        char            mark[ 2 ];
        mark[ 0 ] = currentPlayerMarker;
        mark[ 1 ] = '\0';

        /* label_create( mark, */
                        /* s->box.x + (s->box.w)/4,  */
                        /* s->box.y + (s->box.h / 4) - 5 ,  */
                        /* 160 );  */
        label_count++;

        /* Player makes a move at this cell. */
        
        move_t*         playerMove = &(move_t) { .row = pos->row+1, 
                                                .col = pos->col+1, 
                                                .piece = config->playerMarker };

        apply_move( config->game->board, playerMove );

        /* Check if player's move causes a game over scenario. */

        if ( get_winner( config->game ) != ' ' || 
                        num_vacant_cells( config->game->board ) == 0 )
        {
                return 0;
        }


        /* Now computer takes it's turn. */

        move_t*         computerMove = config->game->computerPlayer->makeMove( config->game->board,
                                                                                config->game->computerPlayer->piece,
                                                                                config->game->humanPlayer->piece );

        selector_t*     computerCell = get_selector( &board->cells[ computerMove->col - 1 ][ computerMove->row - 1 ] );

        computer_press( computerCell );
        apply_move( config->game->board, computerMove );

        return 0;
}

int
cell_unpress( selector_t* s )
{
        return 0;
}

board_display_t*
board_init( SDL_Renderer* renderer, config_t* game_config )
{
        config = game_config;

        currentPlayerMarker = config->playerMarker;
        currentComputerMarker = config->computerMarker;

        selector_start_index = get_selector_index();
        /* label_start_index = get_label_index(); */

        board = ( board_display_t* ) malloc( sizeof( board_display_t* ) );

        board->cells = ( selector_t** ) malloc( 3 * sizeof( selector_t* ) ); 
        for ( int i = 0; i < rows; i++ )
        {
                board->cells[ i ] = ( selector_t* ) malloc( 3 * sizeof( selector_t ) );
                for ( int j = 0; j < cols; j++ )
                {
                        board->cells[ i ][ j ] = *selector_init( X_OFFSET + ( CELL_WIDTH * i ),
                                                                        Y_OFFSET + ( CELL_HEIGHT * j ),
                                                                        CELL_WIDTH,
                                                                        CELL_HEIGHT,
                                                                        &default_color,
                                                                        &highlight_color,
                                                                        &select_color,
                                                                        &press_color,
                                                                        NULL,
                                                                        &cell_press,
                                                                        &cell_unpress );
                        selector_count++;
                }
        }

        return board;
}

void
disable_board()
{
        for ( int i = 0; i < rows; i++ )
        {
                for ( int j = 0; j < cols; j++ )
                {
                        get_selector( &board->cells[ j ][ i ] )->is_disabled = 1;
                }
        }
}

void
board_destroy()
{
        if ( board == NULL )
        {
                return;
        }

        for ( int i = 0; i < rows; i++ )
        {
                free( board->cells[ i ] );
        }

        free( board );
}

