#include <stdio.h>

#include "config.h"
#include "util/color.h"
#include "util/error.h"

#define THEME_FILE "./themes/Cool_Blue.th"
#define NUM_THEME_COLORS 5

config_t* config;

int
hexToInt( char* hexVal )
{
        unsigned        n;
        sscanf( hexVal, "%x", &n );
        return n;
}

SDL_Color hexToColor( char* hexVal )
{
        char            hexColor[ 3 ];
        hexColor[ 2 ] = '\0';

        memcpy( hexColor, hexVal, 2 );
        int r = hexToInt( hexColor );

        memcpy( hexColor, hexVal+2, 2 );
        int g = hexToInt( hexColor );

        memcpy( hexColor, hexVal+4, 2 );
        int b = hexToInt( hexColor );

        return ( SDL_Color ) { .r =   r,
                                .g =  g,
                                .b =  b,
                                .a = 255 };
}

char** theme;

int
load_theme( char* theme_path )
{
        int             ret = 0;
        FILE*           fptr = NULL;

        fptr = fopen( theme_path, "r" );

        assert_msg( ( fptr == NULL ), "Failed to open theme file!" );

        size_t bufSize = 1024 * sizeof( char* );
        char** buffer = ( char** ) malloc( 1024 * sizeof( char* ) );

        /* Get name of theme */
        ret = getline( buffer, &bufSize, fptr );
        assert_msg( ( ret <= 1 ), "Theme name could not be read!" );

        (*buffer)[ ret - 1 ] = '\0';
        printf( "Loading theme [ %s ]...\n", *buffer );

        theme = (char**) malloc( 5 * sizeof( char* ) );
        for ( int i = 0; i < NUM_THEME_COLORS; i++ )
        {
                theme[ i ] = ( char* ) malloc( 7 * sizeof( char ) );

                ret = getline( buffer, &bufSize, fptr );
                assert_msg( ( ret <= 0 ), "Theme color could not be read!" );
                (*buffer)[ ret - 1 ] = '\0';

                memcpy( theme[ i ], *buffer, 7 * sizeof( char ) );

                printf( "theme[ %d ] = {%s}\n", i, theme[ i ] );
        }

        fclose( fptr );

        return 0;
}

void
color_init( char* theme_path )
{
        load_theme( theme_path );

        clear           = hexToColor( "000000" );
        white           = hexToColor( "ffffff" );

        background      = hexToColor( theme[ 0 ] );
        default_color   = hexToColor( theme[ 1 ] );
        highlight_color = hexToColor( theme[ 2 ] );
        select_color    = hexToColor( theme[ 3 ] );
        press_color     = hexToColor( theme[ 4 ] );

        orange          = hexToColor( "E8985E" );
        green           = hexToColor( "64c864" );
}

void
set_color( SDL_Color color )
{
        SDL_SetRenderDrawColor( config->renderer, 
                        color.r, 
                        color.g, 
                        color.b, 
                        color.a );
}


