
#include "animate.h"

void
animation_step( Animation* self )
{
}

Animation*
animation_create( SDL_Rect* box, float end_x, float end_y )
{
        Animation*              animation = ( Animation* ) malloc( sizeof( Animation ) );

        animation->end_x = end_x;
        animation->end_y = end_y;

        animation->box = box;

        animation->step = animation_step;

        return animation;
}

