#include "sound.h"
#include "error.h"

void
sound_play( Sound* self )
{
        if ( !config->is_sound_enabled )
        {
                return;
        }

        Mix_PlayMusic( self->sound, 0 );
}

void
sound_destroy( Sound* self )
{
        Mix_FreeMusic( self->sound );

        free( self );
        self = NULL;
}

Sound*
sound_create( const char* file_path )
{
        Sound*          self = ( Sound* ) malloc( sizeof( Sound ) );

        self->sound = Mix_LoadMUS( file_path );
        assert_msg( ( self->sound == NULL ), Mix_GetError() );

        self->play = sound_play;
        self->destroy = sound_destroy;

        return self;
}

void
sound_init()
{
        test_sound = sound_create( "./res/interface-1.mp3" );
        start_game_sound = sound_create( "./res/melancholy-ui-chime.mp3" );
        button_sound = sound_create( "./res/button.mp3" );
        cell_connect_sound = sound_create( "./res/button.mp3" );
        cell_join_sound = sound_create( "./res/system-notification.mp3" );
}

