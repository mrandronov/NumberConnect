#ifndef __RECT_LIB_H__
#define __RECT_LIB_H__

#include "config.h"

config_t*               config;

void            render_rect( SDL_Rect* rect, SDL_Color color );
bool            is_mouse_over_rect( SDL_Rect* rect );

#endif /* __RECT_LIB_H__ */
