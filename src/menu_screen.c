#include "menu_screen.h"

void
menu_screen_draw( Menu_Screen* self )
{
        // Add buttons?
}

void
menu_screen_handle_event( Menu_Screen* self, SDL_Event* event )
{
}

void
menu_screen_destroy( Menu_Screen* self )
{
        free( self );
        self = NULL;
}

Menu_Screen*
menu_screen_create()
{
        Menu_Screen*            screen = ( Menu_Screen* ) malloc( sizeof( Menu_Screen ) );

        screen->render = menu_screen_draw;
        screen->handle_event = menu_screen_handle_event;
        screen->destroy = menu_screen_destroy;

        return screen;
}

