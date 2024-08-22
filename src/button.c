#include "button.h"
#include "color.h"
#include "rect.h"

void
test_press( Button* self )
{
        printf( "The button was pressed!\n" );
}

void
button_render( Button* self )
{
        SDL_Color*      color = self->color;
        if ( self->is_pressed )
        {
                color = &press_color;
        }

        render_rect( &self->box, *color );

        self->label->center( self->label, &self->box );
        self->label->render( self->label );
}

void
button_press( Button* self )
{
        if ( !is_mouse_over_rect( &self->box ) )
        {
                self->is_pressed = false;
                return;
        }

        self->is_pressed = true;
}

void
button_unpress( Button* self )
{
        if ( !self->is_pressed )
        {
                return;
        }
        
        self->is_pressed = false;

        if ( self->on_press != NULL )
        {
                self->on_press( self );
        }
        pressed_button = self;
}

void
button_destroy( Button* self )
{
        self->label->destroy( self->label );
        free( self );
        self = NULL;
}

Button*
button_create( const char* str, int font_size, int x, int y, int w, int h, SDL_Color* color, void ( *on_press )( Button* self ) )
{
        Button*         self = ( Button* ) malloc( sizeof( Button ) );
        
        self->box = ( SDL_Rect ){ .x = x, .y = y, .w = w, .h = h };
        self->color = color;
        self->label = label_create( str, x, y, font_size, &white );
        self->is_pressed = false;

        self->render = button_render;
        self->on_press = on_press;
        self->press = button_press;
        self->unpress = button_unpress;
        self->destroy = button_destroy;

        return self;
}

