#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

#include "component/connector.h"
#include "screen/screen.h"
#include "util/error.h"
#include "util/color.h"
#include "util/sound.h"
#include "window.h"

void
window_init( Window* self )
{
        assert_msg( ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0 ), SDL_GetError() );

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

        assert_msg( ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ), Mix_GetError() );

        self->window = window;
        config->renderer = renderer;

        sounds_init();
        color_init( config->theme_path );

        num_set_init();
        tile_set_init();

        // All visual components are created on screens, so
        // SDL2 needs to be initialized before creating them.
        self->game_screen = game_screen_create();
        self->menu_screen = menu_screen_create();
        self->option_screen = option_screen_create();

        self->current_screen = self->menu_screen;
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
                }
                
                self->current_screen->handle_event( self->current_screen, &event );
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
        self->start = SDL_GetPerformanceCounter();

        /* Clear the display from previous frame */

        set_color( background );
        SDL_RenderClear( config->renderer );

        self->current_screen->render( self->current_screen );

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
                self->handle_events( self );

                self->render( self );
        }
}

void
window_destroy( Window* self )
{
        self->game_screen->destroy( self->game_screen );
        self->option_screen->destroy( self->option_screen );
        self->menu_screen->destroy( self->menu_screen );

        SDL_DestroyRenderer( config->renderer );
        config->renderer = NULL;

        SDL_DestroyWindow( self->window );
        self->window = NULL;

        sounds_destroy();
        config_destroy();

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
