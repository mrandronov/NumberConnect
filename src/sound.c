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
sounds_init()
{
        start_game_sound = sound_create( "./res/melancholy-ui-chime.mp3" );
        button_sound = sound_create( "./res/button.mp3" );
        cell_connect_sound = sound_create( "./res/button.mp3" );
        cell_join_sound = sound_create( "./res/system-notification.mp3" );
}

void
sounds_destroy()
{
        start_game_sound->destroy( start_game_sound );
        start_game_sound = NULL;

        button_sound->destroy( button_sound );
        button_sound = NULL;

        cell_connect_sound->destroy( cell_connect_sound );
        cell_connect_sound = NULL;

        cell_join_sound->destroy( cell_join_sound );
        cell_join_sound = NULL;
}

