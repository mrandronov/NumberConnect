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

        Window*         window = window_create();

        window->run( window );

        return 0;
}

