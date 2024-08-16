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

        float           rate_x;
        float           rate_y;
        
        float           time;
        bool            running;

        SDL_Rect*       box;

        void            ( *step )( struct anim_t* self );
        void            ( *destroy )( struct anim_t* self );
} Animation;

Animation*              animation_create( SDL_Rect* box, float end_x, float end_y, float time );

#endif /* __ANIMATE_LIB_H__ */
