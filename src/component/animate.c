#include "component/animate.h"

bool
is_dimension_done( float start, float end, float rate )
{
        if ( rate <= 0 )
        {
                return ( start <= end );
        }

        return ( start >= end );
}

float
get_rate( float start, float end, float time )
{
        float px_per_frame = 60.0f * time;

        float distance = ( end - start );

        if ( distance == 0 )
        {
                return 0.0f;
        }

        return distance / px_per_frame;
}

void
animation_step( Animation* self )
{
        if ( !self->running )
        {
                return;
        }

        self->cur_x += self->rate_x;
        self->cur_y += self->rate_y;

        if ( is_dimension_done( self->box->x, self->end_x, self->rate_x ) &&
                        is_dimension_done(self->box->y, self->end_y, self->rate_y) )
        {
                self->box->x = self->end_x;
                self->box->y = self->end_y;
                self->running = false;
                return;
        }

        self->box->x = self->cur_x;
        self->box->y = self->cur_y;
}

void
animation_destroy( Animation* self )
{
        free( self );
        self = NULL;
}

Animation*
animation_create( SDL_Rect* box, float end_x, float end_y, float time )
{
        Animation*              animation = ( Animation* ) malloc( sizeof( Animation ) );

        animation->cur_x = ( float ) box->x;
        animation->cur_y = ( float ) box->y;

        animation->end_x = end_x;
        animation->end_y = end_y;

        animation->rate_x = get_rate( animation->cur_x, end_x, time );
        animation->rate_y = get_rate( animation->cur_y, end_y, time );

        animation->time = time;
        animation->running = true;
        animation->box = box;

        animation->step = animation_step;
        animation->destroy = animation_destroy;

        return animation;
}
