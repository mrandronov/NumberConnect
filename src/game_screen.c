
#include "game_screen.h"
#include "color.h"
#include "connector.h"

Connector_Chain*        chain;
Board*                  board;
label_t*                score;
Tile*                   result_tile;

void
game_screen_draw( Game_Screen* self )
{
        chain->render( chain );
        board->render( board );
        score->render( score );

        if ( result_tile )
        {
                result_tile->box.x = 600;
                result_tile->box.y = 20;
                result_tile->render( result_tile );
        }

}

void
game_screen_handle_event( Game_Screen* self, SDL_Event* event )
{
        if ( event->type == SDL_MOUSEMOTION )
        {
                SDL_GetMouseState( &config->mouse_x, &config->mouse_y );
                chain->check_neighbor( chain );

                if ( chain->total > 0 )
                {
                        int             correlate_index = num_set_get_lowest_square( chain->total );
                        result_tile = &tile_set[ correlate_index ];
                }
        }

        if ( event->type == SDL_MOUSEBUTTONDOWN )
        {
                if ( chain->enabled )
                {
                        return;
                }

                current_cell = board->on_hover( board );

                if ( !current_cell )
                {
                        return;
                }

                if ( !current_cell->tile )
                {
                        return;
                }

                current_cell->color = &orange;
                chain = cnctr_chain_create();
                chain->add_link( chain, current_cell );
                chain->enabled = true;
        }

        if ( event->type == SDL_MOUSEBUTTONUP )
        {
                if ( !chain->enabled )
                {
                        return;
                }

                int             correlate_index = num_set_get_lowest_square( chain->total );
                int             current_row = current_cell->row;
                int             current_col = current_cell->col;

                if ( chain->length > 1 )
                {
                        // Clear the tiles
                        chain->clear_tiles( chain );
                }
                else
                {
                        chain->clear( chain );
                        chain->total = 0;
                        chain->enabled = false;
                        return;
                }
                chain->clear( chain );

                // Add new tile to the board, where the chain head was.
                board->cells[ current_row ][ current_col ].tile = &tile_set[ correlate_index ];

                config->playerScore += chain->total;
                chain->total = 0;
                chain->enabled = false;

                // Update the cells with new tiles;
                board->trickle_down( board );

                // Update the score meter
                int size = snprintf( NULL, 0, "Score: %d", config->playerScore );
                char*   str = ( char* ) malloc( sizeof( char ) * ( size + 1 ) );

                snprintf( str, size + 1, "Score: %d", config->playerScore );

                score->update( score, str );

                result_tile = NULL;
        }
}

void
game_screen_destroy( Game_Screen* self )
{
        chain->destroy( chain );
        board->destroy( board );
        score->destroy( score );
        result_tile->destroy( result_tile );

        free( self );
        self = NULL;
}

Game_Screen*
game_screen_create()
{
        Game_Screen*            screen = ( Game_Screen* ) malloc( sizeof( Game_Screen ) );

        screen->render = game_screen_draw;
        screen->handle_event = game_screen_handle_event;
        screen->destroy = game_screen_destroy;

        board = cell_board_create();
        chain = cnctr_chain_create();
        score = label_create( "Score: 0", 175, 60, 48, &white );

        return screen;
}

