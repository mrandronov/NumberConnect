#include "util/rect.h"
#include "util/color.h"

void
render_rect( SDL_Rect* rect, SDL_Color color )
{
        set_color( color );
        SDL_RenderFillRect( config->renderer, rect );
}

bool
is_mouse_over_rect( SDL_Rect* rect )
{
        return ( config->mouse_x >= rect->x &&
                        config->mouse_x < ( rect->x + rect->w ) &&
                        config->mouse_y >= rect->y &&
                        config->mouse_y < ( rect->y + rect->h ) );
}

