#ifndef __BUTTON_LIB_H__
#define __BUTTON_LIB_H__

#include "config.h"
#include "sound.h"
#include "window.h"
#include "label.h"

config_t*               config;
Window*                 window;

typedef struct button_t
{
        SDL_Rect        box;
        SDL_Color*      color;
        SDL_Color*      pressed_color;
        label_t*        label;
        Sound*          sound;
        bool            is_pressed;

        void            ( *render )( struct button_t* self );
        void            ( *on_press )( struct button_t* self );
        void            ( *press )( struct button_t* self );
        void            ( *unpress )( struct button_t* self );
        void            ( *destroy )( struct button_t* self );
} Button;

Button*                 pressed_button;

Button*                 button_create( const char* str, int font_size, int x, int y, int w, int h, SDL_Color* color, void ( *on_press )( Button* self ), Sound* sound );

#endif /* __BUTTON_LIB_H__ */
