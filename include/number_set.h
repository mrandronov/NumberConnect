#ifndef __LIB_NUMBER_SET_H__
#define __LIB_NUMBER_SET_H__

#include "SDL_rect.h"

#include "config.h"
#include "label.h"

#define NUM_LIMIT 32

config_t*               config;
int                     num_set[ 32 ];

typedef struct tile_t
{
        int             index;
        label_t*        label;
        SDL_Rect        box;
        SDL_Color       background_color;

        void            ( *render )( struct tile_t* self );
        void            ( *destroy )( struct tile_t* self );
} Tile;

// Predefine all tiles and reuse throughout the game
Tile                    tile_set[ 32 ];

int                     num_set_get_lowest_square( int value );
void                    num_set_init();
void                    tile_set_init();

#endif /* __LIB_NUMBER_SET_H__ */
