#include "number_set.h"
#include "cell.h"
#include "color.h"
#include "rect.h"

int
num_set_get_lowest_square( int value )
{
        int             i = 0;

        while ( num_set[ i ] < value )
        {
                i++;
        }

        return i;
}

void
num_set_init()
{
        int             value = 1;

        for ( int i = 0; i < NUM_LIMIT; i++ )
        {
                value *= 2;
                num_set[ i ] = value;
        }
}

void
tile_draw( Tile* self )
{
        render_rect( &self->box, self->background_color );

        // Center the label's position to the background box.
        // We want to lock the label text to the tile.
        self->label->center( self->label, &self->box );

        self->label->render( self->label );
}

void
tile_destroy( Tile* self )
{
        self->label->destroy( self->label );
}

Tile
tile_init( int index, SDL_Color background_color, SDL_Color* text_color, int font_size )
{
        int             num = num_set[ index ];
        int             size = snprintf( NULL, 0, "%d", num );
        char*           label_str = ( char* ) malloc( size + 1 );

        snprintf( label_str, size + 1, "%d", num );

        label_t*        label = label_create( label_str, 0, 0, font_size, text_color );
        SDL_Rect        box = ( SDL_Rect ){ .x = 0, .y = 0, .w = CELL_BOX_WIDTH, .h = CELL_BOX_HEIGHT }; 

        return ( Tile ) { .index = index, 
                .label = label, 
                .box = box, 
                .background_color = background_color,
                .render = tile_draw,
                .destroy = tile_destroy 
        };
}

void
tile_set_init()
{
        SDL_Color               text_color = hexToColor( "FDFFF7" );

        tile_set[ 0 ] = tile_init( 0, hexToColor( "F24C00" ), &text_color, 64 ); // 2
        tile_set[ 1 ] = tile_init( 1, hexToColor( "713E5A" ), &text_color, 64 ); // 4
        tile_set[ 2 ] = tile_init( 2, hexToColor( "F28123" ), &text_color, 64 ); // 8
        tile_set[ 3 ] = tile_init( 3, hexToColor( "688E26" ), &text_color, 64 ); // 16
        tile_set[ 4 ] = tile_init( 4, hexToColor( "A8DCD9" ), &text_color, 64 ); // 32
        tile_set[ 5 ] = tile_init( 5, hexToColor( "FAA613" ), &text_color, 64 ); // 64
        tile_set[ 6 ] = tile_init( 6, hexToColor( "25283D" ), &text_color, 48 ); // 128
        tile_set[ 7 ] = tile_init( 7, hexToColor( "4F3824" ), &text_color, 48 ); // 256
        tile_set[ 8 ] = tile_init( 8, hexToColor( "07A0C3" ), &text_color, 48 ); // 512
        tile_set[ 9 ] = tile_init( 9, hexToColor( "086788" ), &text_color, 40 ); // 1024
        tile_set[ 10 ] = tile_init( 10, hexToColor( "4ECDC4" ), &text_color, 40 ); // 2048
        tile_set[ 11 ] = tile_init( 11, hexToColor( "617073" ), &text_color, 40 ); // 4096
}

