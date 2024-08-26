#include "component/button.h"
#include "screen/screen.h"
#include "util/color.h"

Button*         exit_button;
label_t*        volume_option_label;
Button*         sound_on_button;
Button*         sound_off_button;

void
return_options_to_menu( Button* self )
{
        window->current_screen = window->menu_screen;
}

void
sound_on( Button* self )
{
        config->is_sound_enabled = true;
}

void
sound_off( Button* self )
{
        config->is_sound_enabled = false;
}

void
option_screen_draw( Screen* self )
{
        exit_button->render( exit_button );
        volume_option_label->render( volume_option_label );
        sound_on_button->render( sound_on_button );
        sound_off_button->render( sound_off_button );
}

void
option_screen_handle_event( Screen* self, SDL_Event* event )
{
        if ( event->type == SDL_MOUSEBUTTONDOWN )
        {
                exit_button->press( exit_button );
                sound_on_button->press( sound_on_button );
                sound_off_button->press( sound_off_button );
        }

        if ( event->type == SDL_MOUSEBUTTONUP )
        {
                exit_button->unpress( exit_button );
                sound_on_button->unpress( sound_on_button );
                sound_off_button->unpress( sound_off_button );
        }
}

void
option_screen_destroy( Screen* self )
{
        exit_button->destroy( exit_button );
        volume_option_label->destroy( volume_option_label );
        sound_on_button->destroy( sound_on_button );
        sound_off_button->destroy( sound_off_button );
}

Screen*
option_screen_create()
{
        Screen*         self = ( Screen* ) malloc( sizeof( Screen ) );

        exit_button = button_create( "return", 48, 650, 1000, 200, 50, &default_color, return_options_to_menu, button_sound );
        volume_option_label = label_create( "Sound: ", 325, 150, 48, &white );
        sound_on_button = button_create( "ON", 48, 325, 200, 100, 50, &default_color, sound_on, button_sound );
        sound_off_button = button_create( "OFF", 48, 475, 200, 100, 50, &default_color, sound_off, button_sound );

        self->render = option_screen_draw;
        self->handle_event = option_screen_handle_event;
        self->destroy = option_screen_destroy;

        return self;
}
