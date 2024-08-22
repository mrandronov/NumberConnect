#include "button.h"
#include "color.h"
#include "screen.h"

Button*         play_button;
Button*         options_button;
Button*         quit_button;

void
play_game_press( Button* self )
{
        window->current_screen = window->game_screen;
}

void
options_game_press( Button* self )
{
        printf( "The options button was pressed!\n" );
}

void
quit_game_press( Button *self )
{
        config->is_game_running = false;
}


void
menu_screen_draw( Screen* self )
{
        play_button->render( play_button );
        options_button->render( options_button );
        quit_button->render( quit_button );
}

void
menu_screen_handle_event( Screen* self, SDL_Event* event )
{
        if ( event->type == SDL_MOUSEBUTTONDOWN )
        {
                play_button->press( play_button );
                options_button->press( options_button );
                quit_button->press( quit_button );
        }

        if ( event->type == SDL_MOUSEBUTTONUP )
        {
                play_button->unpress( play_button );
                options_button->unpress( options_button );
                quit_button->unpress( quit_button );
        }
}

void
menu_screen_destroy( Screen* self )
{
        play_button->destroy( play_button );
        options_button->destroy( options_button );
        quit_button->destroy( quit_button );

        free( self );
        self = NULL;
}

Screen*
menu_screen_create()
{
        Screen*            screen = ( Screen* ) malloc( sizeof( Screen ) );

        play_button = button_create( "play", 32, 350, 440, 200, 50, &default_color, play_game_press );
        options_button = button_create( "options", 32, 350, 500, 200, 50, &default_color, options_game_press );
        quit_button = button_create( "quit", 32, 350, 560, 200, 50, &default_color, quit_game_press );

        screen->render = menu_screen_draw;
        screen->handle_event = menu_screen_handle_event;
        screen->destroy = menu_screen_destroy;

        return screen;
}

