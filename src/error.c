
#include "error.h"

void
assert_msg( int condition, const char* msg )
{
        if ( !condition )
        {
                return;
        }

        printf( "Error: %s\n", msg );
        exit( 1 );
}
