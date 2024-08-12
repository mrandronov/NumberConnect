
#include "config.h"
#include "color.h"
#include "button.h"
#include "mouse.h"

int             button_index;
button_t**      button_list;

config_t*       config;

int
new_game()
{
        return 0;
}

int
quit_game()
{
        printf( "Quitting the game!\n" );

        config->is_game_running = false;

        return 0;
}

button_t*
button_list_init()
{
        button_index = 0;
        button_list = ( button_t** ) malloc( BUTTON_LIST_SIZE * sizeof( button_t* ) );

        int new_game_x = 375;
        int new_game_y = 650;
        /* label_create( renderer, "NEW GAME", new_game_x + 5, new_game_y + 10, 35 );  */
        button_create( new_game_x, 
                        new_game_y, 
                        180,
                        50, 
                        &default_color, 
                        &highlight_color, 
                        &orange, 
                        &new_game );

        int quit_game_x = 375;
        int quit_game_y = 725;
        /* label_create( renderer, "QUIT", quit_game_x + 5, quit_game_y + 10, 35 ); */
        button_create( quit_game_x, 
                        quit_game_y, 
                        180,
                        50, 
                        &default_color, 
                        &highlight_color, 
                        &orange, 
                        &quit_game );

        return *button_list;
}

button_t*
button_create( int x,
        int y,
        int w,
        int h,
        SDL_Color* color,
        SDL_Color* highlight,
        SDL_Color* select,
        int (*pressFunc)() )
{
        button_t*       button = ( button_t* ) malloc( sizeof( button_t ) );

        button->box = (SDL_Rect) { .x = x, .y = y, .w = w, .h = h };
        button->color = color;
        button->highlight_color = highlight;
        button->selected_color = select;
        button->is_highlighted = 0;
        button->is_pressed = 0;
        button->pressFunc = pressFunc;

        button_list[ button_index ] = button;
        button_index++;

        return button;
}

void
draw_button(button_t* button )
{
        if ( button->is_highlighted )
        {
                set_color( *button->highlight_color );
        }
        else
        {
                set_color( *button->color );
        }

        if ( button->is_pressed )
        {
                set_color( *button->selected_color );
        }

        SDL_RenderFillRect( config->renderer, &button->box );
}

void
button_destroy( button_t* button )
{
        free( button );
}

void
press_buttons()
{
        for ( int i = 0; i < button_index; i++ )
        {
                if ( is_mouse_over( button_list[ i ]->box ) )
                {
                        button_list[ i ]->is_pressed = 1;
                }
        }
}

void
unpress_buttons()
{
        for ( int i = 0; i < button_index; i++ )
        {
                if ( is_mouse_over( button_list[ i ]->box ) )
                {
                        button_list[ i ]->is_pressed = 0;
                }
        }
}

void
handle_button_press()
{
        for ( int i = 0; i < button_index; i++ )
        {
                if ( is_mouse_over( button_list[ i ]->box ) )
                {
                        button_list[ i ]->pressFunc();
                }
        }
}

void
draw_button_list()
{
        for ( int i = 0; i < button_index; i++ )
        {
                draw_button(button_list[ i ] );
        }
}

void
highlight_buttons()
{
        for ( int i = 0; i < button_index; i++ )
        {
                if ( is_mouse_over( button_list[ i ]->box ) )
                {
                        button_list[ i ]->is_highlighted = 1;
                }
                else
                {
                        button_list[ i ]->is_highlighted = 0;
                }
        }
}

void
button_list_destroy()
{
        if ( button_index == 0 )
        {
                return;
        }

        for ( int i = 0; i < button_index; i++ )
        {
                button_destroy( button_list[ i ] );
        }
        button_index = 0;
}

