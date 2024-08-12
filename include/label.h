#ifndef __LIB_LABEL_H__
#define __LIB_LABEL_H__

#include "SDL_render.h"

#define LABEL_LIST_SIZE 10
#define DYNAMIC_LABEL_LIST_SIZE 10

typedef struct _label_t
{
    SDL_Texture*        texture; 
    SDL_Rect            rect;
    SDL_Color           color;

    void                ( *render )( struct _label_t* self );
    void                ( *destroy )( struct _label_t* self );
    void                ( *center )( struct _label_t* self, SDL_Rect* box );
} label_t;

label_t*        label_create( const char* str, int x, int y, int size, SDL_Color* color );

#endif /* __LIB_LABEL_H__ */
