
#include "color.h"
#include "selector.h"
#include "mouse.h"
#include "label.h"

int             selector_index;
selector_t**    selector_list;

selector_t*     o_marker_toggle;
selector_t*     x_marker_toggle;

selector_t*     smart_style_toggle;
selector_t*     rand_style_toggle;

int
marker_unselect( selector_t* s )
{
        s->is_selected = 0;
        return 0;
}

int
o_marker_select( selector_t* s )
{
        marker_unselect( x_marker_toggle );
        s->is_selected = 1;

        config->playerMarker = 'O';
        config->computerMarker = 'X';

        return 0;
}

int
x_marker_select( selector_t* s )
{
        marker_unselect( o_marker_toggle );
        s->is_selected = 1;

        config->playerMarker = 'X';
        config->computerMarker = 'O';

        return 0;
}

int
style_unselect( selector_t* s )
{
        s->is_selected = 0;
        return 0;
}

int
smart_style_select( selector_t* s )
{
        style_unselect( rand_style_toggle );
        s->is_selected = 1;
        
        config->computerType = SMART;

        return 0;
}

int
rand_style_select( selector_t* s )
{
        style_unselect( smart_style_toggle );
        s->is_selected = 1;

        config->computerType = RANDOM;

        return 0;
}

/*
        This function's purpose is for when we want a selector
        to do nothing when pressed.
 */
int
do_nothing( selector_t* s )
{
        return 0;
}

int
get_selector_index()
{
        return selector_index;
}

selector_t*
selector_list_init()
{
        selector_index = 0;
        selector_list = ( selector_t** ) malloc( SELECTOR_LIST_SIZE * sizeof( selector_t* ) );

        int O_option_x = 50;
        int O_option_y = 600;
        /* label_create( "O", O_option_x+7, O_option_y+7, 60 ); */
        o_marker_toggle = selector_init( O_option_x,
                                                O_option_y,
                                                50,
                                                50,
                                                &default_color,
                                                &highlight_color,
                                                &select_color,
                                                &press_color,
                                                NULL,
                                                &o_marker_select,
                                                &do_nothing ); 

        int X_option_x = 120;
        int X_option_y = 600;
        /* label_create( "X", X_option_x + 7, X_option_y + 7, 60 ); */
        x_marker_toggle = selector_init( X_option_x,
                                                X_option_y,
                                                50,
                                                50,
                                                &default_color,
                                                &highlight_color,
                                                &select_color,
                                                &press_color,
                                                NULL,
                                                &x_marker_select,
                                                &do_nothing );
        

        int SMART_option_x = 50;
        int SMART_option_y = 730;
        /* label_create( "SMART", SMART_option_x + 10, SMART_option_y + 5, 35 ); */
        smart_style_toggle = selector_init( SMART_option_x, 
                                                SMART_option_y,
                                                130,
                                                40,
                                                &default_color,
                                                &highlight_color,
                                                &select_color,
                                                &press_color,
                                                NULL,
                                                &smart_style_select,
                                                &do_nothing );

        int RAND_option_x = 200;
        int RAND_option_y = 730;
        /* label_create( "RAND", RAND_option_x + 15, RAND_option_y + 5, 40 ); */
        rand_style_toggle = selector_init( RAND_option_x,
                                                RAND_option_y,
                                                130,
                                                40,
                                                &default_color,
                                                &highlight_color,
                                                &select_color,
                                                &press_color,
                                                NULL,
                                                &rand_style_select,
                                                &do_nothing ); 

        /*
                By default, we start with the O marker for the player
                and the SMART computer play style selected.
         */

        o_marker_select( selector_list[ selector_index - 4 ] );
        smart_style_select( selector_list[ selector_index - 2 ] );

        return *selector_list;
}

selector_t*
selector_init( int x,
        int y,
        int w,
        int h,
        SDL_Color* color,
        SDL_Color* highlight,
        SDL_Color* select,
        SDL_Color* press,
        const char* text,
        int (*pressFunc)(),
        int (*unpressFunc)() )
{
        selector_t*       selector = ( selector_t* ) malloc( sizeof( selector_t ) );

        selector->box                   = (SDL_Rect) { .x = x, .y = y, .w = w, .h = h };
        selector->color                 = color;
        selector->highlight_color       = highlight;
        selector->selected_color        = select;
        selector->pressed_color         = press;
        selector->is_highlighted        = 0;
        selector->is_selected           = 0;
        selector->is_pressed            = 0;
        selector->is_disabled           = 0;
        selector->pressFunc             = pressFunc;
        selector->unpressFunc           = unpressFunc;

        if ( text != NULL )
        {
                /* label_create( text, x+7, y+7, 40 ); */
        }

        selector_list[ selector_index ] = selector;
        selector_index++;

        return selector;
}

void
draw_selector( selector_t* selector )
{
        if ( selector->is_highlighted )
        {
                set_color( *selector->highlight_color );
        }
        else if ( selector->is_selected )
        {
                set_color( *selector->selected_color );
        }
        else
        {
                set_color( *selector->color );
        }

        if ( selector->is_pressed )
        {
                if ( !selector->is_selected )
                {
                        set_color( *selector->pressed_color );
                }
        }

        SDL_RenderFillRect( config->renderer, &selector->box );
}

void
selector_destroy( selector_t* s )
{
        free( s );
}

void
press_selectors()
{
        for ( int i = 0; i < selector_index; i++ )
        {
                if ( is_mouse_over( selector_list[ i ]->box ) )
                {
                        selector_list[ i ]->is_pressed = 1;
                }
        }
}

void
unpress_selectors()
{
        for ( int i = 0; i < selector_index; i++ )
        {
                if ( is_mouse_over( selector_list[ i ]->box ) )
                {
                        selector_list[ i ]->is_pressed = 0;
                }
        }
}

void
handle_selector_press()
{
        for ( int i = 0; i < selector_index; i++ )
        {
                if ( is_mouse_over( selector_list[ i ]->box ) )
                {
                        if ( selector_list[ i ]->is_disabled == 1 )
                        {
                                continue;
                        }

                        if ( selector_list[ i ]->is_selected == 1 )
                        {
                                selector_list[ i ]->unpressFunc( selector_list[ i ] );
                        }
                        else
                        {
                                selector_list[ i ]->pressFunc( selector_list[ i ] );
                        }
                }
        }
}

void
draw_selector_list()
{
        for ( int i = 0; i < selector_index; i++ )
        {
                draw_selector( selector_list[ i ] );
        }
}

void
highlight_selectors()
{
        for ( int i = 0; i < selector_index; i++ )
        {
                if ( is_mouse_over( selector_list[ i ]->box ) )
                {
                        selector_list[ i ]->is_highlighted = 1;
                }
                else
                {
                        selector_list[ i ]->is_highlighted = 0;
                }
        }
}

selector_t*
get_selector( selector_t* s )
{
        for ( int i = 0; i < selector_index; i++ )
        {
                if ( ( selector_list[ i ]->box.x == s->box.x ) && 
                                ( selector_list[ i ]->box.y == s->box.y ) )
                {
                        return selector_list[ i ];
                }
        }

        return NULL;
}

void
selector_list_destroy_from_index( int start_index )
{
        for ( int i = start_index; i < selector_index; i++ )
        {
                selector_destroy( selector_list[ i ] );
        }
        selector_index -= ( selector_index - start_index );
}

void
selector_list_destroy()
{
        if ( selector_index == 0 )
        {
                return;
        }

        for ( int i = 0; i < selector_index; i++ )
        {
                selector_destroy( selector_list[ i ] );
        }
        
        selector_index = 0;

        free( selector_list );
}

