#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include "window.h"
#include "connector.h"
#include "error.h"
#include "color.h"

Connector_Chain*        chain;
Board*                  board;
label_t*                score;
Tile*                   result_tile;

void
window_init( Window* self )
{
        assert_msg( ( SDL_Init( SDL_INIT_VIDEO ) != 0 ), SDL_GetError() );

        SDL_Window*             window = SDL_CreateWindow( self->title, 
	                                        SDL_WINDOWPOS_UNDEFINED, 
	                                        SDL_WINDOWPOS_UNDEFINED, 
	                                        self->width, 
	                                        self->height, 
	                                        SDL_WINDOW_SHOWN & SDL_WINDOW_ALLOW_HIGHDPI );
        
        assert_msg( ( window == NULL ), SDL_GetError() );

        SDL_Renderer*           renderer = SDL_CreateRenderer( window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED );

        assert_msg( ( renderer == NULL ), SDL_GetError() );

        assert_msg( ( TTF_Init() != 0 ), TTF_GetError() );

        self->window = window;
        config->renderer = renderer;

        color_init( config->theme_path );

        num_set_init();
        tile_set_init();

        board = cell_board_create();
        chain = cnctr_chain_create();
        score = label_create( "Score: 0", 175, 60, 48, &white ); 
}

void
window_handle_events( Window* self )
{
        SDL_Event event;
        while( SDL_PollEvent( &event ) )
        {
                if ( event.type == SDL_QUIT )
                {
                        config->is_game_running = false;
                }

                if ( event.type == SDL_KEYDOWN )
                {
                        if ( event.key.keysym.scancode == SDL_SCANCODE_Q )
                        {
                                config->is_game_running = false;
                        }
                }
                
                if ( event.type == SDL_MOUSEMOTION )
                {
                        SDL_GetMouseState( &config->mouse_x, &config->mouse_y );
                        chain->check_neighbor( chain );

                        if ( chain->total > 0 )
                        {
                                int             correlate_index = num_set_get_lowest_square( chain->total );
                                result_tile = &tile_set[ correlate_index ];
                        }
                }
                
                if ( event.type == SDL_MOUSEBUTTONDOWN )
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

                if ( event.type == SDL_MOUSEBUTTONUP )
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
}

void
frame_delay( float start, float fps_cap_in_ms )
{
        float end = SDL_GetPerformanceCounter();
        
        float delta = ( end - start ) / ( float ) SDL_GetPerformanceFrequency() * 1000.0f;
        
        if ( delta < fps_cap_in_ms )
        {
                SDL_Delay( fps_cap_in_ms - delta );
        }
}

void
window_draw( Window* self )
{
        /* Clear the display from previous frame */

        set_color( background );
        SDL_RenderClear( config->renderer );

        chain->render( chain );
        board->render( board );
        score->render( score );
        
        if ( result_tile )
        {
                result_tile->box.x = 600;
                result_tile->box.y = 20;
                result_tile->render( result_tile );
        }

        /* Render the display */

        SDL_RenderPresent( config->renderer );

        /* Fix the frame rate to 60 frames per second */

        frame_delay( self->start, self->fps_cap_in_ms );
}

void
window_run( Window* self )
{
        self->init( self );
        while ( config->is_game_running == true )
        {
                self->start = SDL_GetPerformanceCounter();

                self->handle_events( self );

                /* All process drawing */

                self->render( self );
        }
}

void
window_destroy( Window* self )
{
        SDL_DestroyRenderer( config->renderer );
        config->renderer = NULL;

        SDL_DestroyWindow( self->window );
        self->window = NULL;

        TTF_Quit();
        SDL_Quit();

        self = NULL;
}

Window*
window_create()
{
        Window*         self = ( Window* ) malloc( sizeof( Window ) );

        self->width = 900;
        self->height = 1100;
        self->title = "Number Connect";

        self->fps_cap = 60.0f;
        self->fps_cap_in_ms = 1000.0f / self->fps_cap;
        self->start = 0;

        self->init = window_init;
        self->handle_events = window_handle_events;
        self->render = window_draw;
        self->run = window_run;
        self->destroy = window_destroy;

        return self;
}
