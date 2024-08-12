
#include "SDL_ttf.h"

#include "config.h"
#include "label.h"
#include "error.h"

config_t*       config;

void
label_draw( label_t* self )
{
        SDL_RenderCopy( config->renderer, self->texture, NULL, &self->rect );
}

void
label_center( label_t* self, SDL_Rect* box )
{
        int             box_center_x = box->x + ( box->w / 2 );
        int             box_center_y = box->y + ( box->h / 2 );

        int             label_center_x = self->rect.w / 2;
        int             label_center_y = self->rect.h / 2;

        self->rect.x = box_center_x - label_center_x;
        self->rect.y = box_center_y - label_center_y;
}

void
label_destroy( label_t* self )
{
        if ( self->texture )
        {
                SDL_DestroyTexture( self->texture );
        }

        free( self );
        self = NULL;
}

label_t*
label_create( const char* str, int x, int y, int size, SDL_Color* color )
{
        label_t* label = malloc( sizeof( label_t ) );
        TTF_Font* font = TTF_OpenFont( config->font_path, size );
        assert_msg( (font == NULL), TTF_GetError() );

        SDL_Surface* surface = TTF_RenderText_Solid(font, str, *color);
        assert_msg( (surface == NULL), TTF_GetError() );

        label->texture = SDL_CreateTextureFromSurface( config->renderer, surface );
        assert_msg( (label->texture == NULL), SDL_GetError() );

        label->rect.x = x;
        label->rect.y = y;

        assert_msg( ( SDL_QueryTexture( label->texture,
                                        NULL,
                                        NULL,
                                        &( label->rect.w ), 
                                        &( label->rect.h ) ) != 0 ),
                        SDL_GetError() );

        SDL_FreeSurface( surface );
        TTF_CloseFont( font );

        label->color = *color;

        label->render = label_draw;
        label->center = label_center;
        label->destroy = label_destroy;

        return label;
}

