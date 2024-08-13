#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

#include "SDL_mouse.h"
#include "connector.h"
#include "error.h"
#include "color.h"
#include "cell.h"
#include "config.h"
#include "number_set.h"

const int               SCREEN_WIDTH = 900;
const int               SCREEN_HEIGHT = 1100;
const char*             TITLE = "Number Connect";

const float             fps_cap = 60.0f;
const float             fps_cap_in_ms = 1000.0f / fps_cap;
uint64_t                start;
uint64_t                end;
float                   delta;

char*                   theme_path = "./themes/Default.th";
char*                   line_text_path = "./red_line.png";

config_t*               config;

/* Debug */
Connector_Chain*        chain;
Board*                  board;

void
clean_up()
{
        /* SDL_DestroyTexture( config->line_texture ); */
        /* config->line_texture = NULL; */

        SDL_DestroyRenderer( config->renderer );
        config->renderer = NULL;

        SDL_DestroyWindow( config->window );
        config->window = NULL;

        TTF_Quit();
        SDL_Quit();
}

void
handle_events( )
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
                }
        }
}

void
loadTexture( const char* path )
{
        config->line_texture = IMG_LoadTexture( config->renderer, path );
        if ( config->line_texture == NULL )
        {
                printf( "Unable to create texture from %s! SDL_image error: %s\n", path, IMG_GetError() );
                exit( 1 );
        }
}

void
init()
{
        assert_msg( ( SDL_Init( SDL_INIT_VIDEO ) != 0 ), SDL_GetError() );

        SDL_Window*             window = SDL_CreateWindow( TITLE, 
	                                        SDL_WINDOWPOS_UNDEFINED, 
	                                        SDL_WINDOWPOS_UNDEFINED, 
	                                        SCREEN_WIDTH, 
	                                        SCREEN_HEIGHT, 
	                                        SDL_WINDOW_SHOWN );
        
        assert_msg( ( window == NULL ), SDL_GetError() );

        SDL_Renderer*           renderer = SDL_CreateRenderer( window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED );

        assert_msg( ( renderer == NULL ), SDL_GetError() );

        assert_msg( ( TTF_Init() != 0 ), TTF_GetError() );

        config_init();
        config->window = window;
        config->renderer = renderer;

        /* loadTexture( line_text_path ); */
        /* assert_msg( ( ( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) == 0 ), IMG_GetError() ); */

        color_init( theme_path );

        num_set_init();
        tile_set_init();

        board = cell_board_create();
        chain = cnctr_chain_create();
}

void
draw_all_components()
{
        chain->render( chain );
        board->render( board );
}

int
main( int argc, char* argv[] )
{
        srand(time(NULL));

        init();

        while ( config->is_game_running == true )
        {
                start = SDL_GetPerformanceCounter();

                handle_events();

                /* Clear the display */

                set_color( background );
                SDL_RenderClear( config->renderer );
                
                /* All process drawing */

                draw_all_components();

                /* Render the display */

                SDL_RenderPresent( config->renderer );

                /* Fix the framerate to 60 frames per second */

                end = SDL_GetPerformanceCounter();
                
                delta = ( end - start ) / ( float ) SDL_GetPerformanceFrequency() * 1000.0f;
                
                if ( delta < fps_cap_in_ms )
                {
                        SDL_Delay( fps_cap_in_ms - delta );
                }
        }

        SDL_DestroyRenderer( config->renderer );
        SDL_DestroyWindow( config->window );
        clean_up();

        return 0;
}

