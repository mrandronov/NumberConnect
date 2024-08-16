
#include "mouse.h"

int mouse_x = 0;
int mouse_y = 0;

int
is_mouse_over( SDL_Rect rect )
{
        if ( ( mouse_x >= rect.x && mouse_x <= rect.x + rect.w )
                        && ( mouse_y >= rect.y && mouse_y <= rect.y + rect.h ) )
        {
                return 1;
        }

        return 0;
}

