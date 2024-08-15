#ifndef __ANIMATE_LIB_H__
#define __ANIMATE_LIB_H__

#include <stdbool.h>

#include "SDL_rect.h"

typedef struct anim_t
{
        float           cur_x;
        float           cur_y;
        float           end_x;
        float           end_y;
        float           rate;
        bool            running;

        SDL_Rect*       box;

        void            ( *step )( struct anim_t* self );
} Animation;

#endif /* __ANIMATE_LIB_H__ */
