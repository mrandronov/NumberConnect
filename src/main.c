#include "config.h"
#include "window.h"

void
main_init()
{
        srand(time(NULL));
        config_init();
}

int
main( int argc, char* argv[] )
{
        main_init();

        window = window_create();

        window->run( window );

        /*window->destroy( window );*/

        return 0;
}

