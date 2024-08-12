
#include "mouse.h"

int mouse_x = 0;
int mouse_y = 0;

int
is_mouse_over( SDL_Rect rect )
{
        Uint32 buttons = SDL_GetMouseState( &mouse_x, &mouse_y );

        if ( ( mouse_x >= rect.x && mouse_x <= rect.x + rect.w )
                        && ( mouse_y >= rect.y && mouse_y <= rect.y + rect.h ) )
        {
                return 1;
        }

        return 0;
}

