/*
gpe_audio_sdl_mixer.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 Game Pencil Engine contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#include <climits>
#include "gpe_audio_sdl_mixer.h"
#include "../gpe/gpe_error_logger.h"

namespace gpe
{
    bool init_sdl_mixer_audio_system( int audio_frequency, Uint16 audio_format, int audio_max_channels, int audio_chunksize  )
    {
        if( audio_frequency < 0 )
        {
            audio_frequency = 0;
        }

        if( audio_max_channels < 0 )
        {
            audio_max_channels = 0;
        }

        if( audio_chunksize < 0 )
        {
            audio_chunksize = 0;
        }
        error_log->report("-audio_sdl_system...");
        // load support for the OGG and MOD sample/music formats

        int flags[sound_format_max];
        std::string flag_names[sound_format_max];

        int i_format = 0;
        for( i_format = 0; i_format < sound_format_max; i_format++ )
        {
            flags[i_format] = INT_MIN;
            flag_names[i_format] = "";
        }
        flags[ sound_format_ogg] = MIX_INIT_OGG;
        flag_names[sound_format_ogg] = "ogg";

        flags[sound_format_acc] = MIX_INIT_FLAC;
        flag_names[sound_format_acc] = "flac";

        flags[sound_format_mp3] = MIX_INIT_MP3;
        flag_names[sound_format_mp3] = "mp3";

        flags[sound_format_mod] = MIX_INIT_MOD;
        flag_names[sound_format_mod] = "mod";

        bool at_least_one_type_supported = false;
        for( i_format = 0; i_format < sound_format_max; i_format++ )
        {
            if( flags[i_format] > INT_MIN )
            {
                if ( (Mix_Init(flags[i_format])&flags[i_format]) != flags[i_format])
                {
                    error_log->report("--Error Initializing Sound Format via SDL_Mixer["+flag_names[i_format]+"]");
                    std::string mixError = Mix_GetError();
                    error_log->report("--Mix_Init: " + mixError );
                    sound_is_format_supported[ i_format ] = false;
                }
                else
                {
                    sound_is_format_supported[ i_format ] = true;
                    at_least_one_type_supported = true;
                }
            }
        }

        //Initialize SDL_mixer
        if( Mix_OpenAudio( audio_frequency, audio_format, audio_max_channels, audio_chunksize  ) == -1 )
        {
            error_log->report("-- Error initializing SDL_Mixer.");
            sound_is_working = false;
        }

        if( sound_music_object != NULL )
        {
            delete sound_music_object;
            sound_music_object = NULL;
        }

        sound_music_object = new music_sdl_mixer("","",-1, -1 );

        if( standard_sound_object != NULL )
        {
            delete standard_sound_object;
            standard_sound_object = NULL;
        }
        standard_sound_object = new sound_sdl_mixer("","",-1, -1 );
        sound_system_name = "sdl_mixer";
        return true;
    }

    void quit_sdl_mixer_audio_system()
    {
        error_log->report("Quitting SDL_Mixer....");
        Mix_Quit();
        sound_system_name = "undefined";
    }

    sound_sdl_mixer::sound_sdl_mixer( std::string s_name, std::string s_file , int group_id_number, int s_id )
    {
        sound_chunk = NULL;
        sound_loaded = false;
        sound_id = s_id;
        sound_name = s_name;
        file_name = s_file;
        load( s_file );
    }

    sound_sdl_mixer::~sound_sdl_mixer()
    {
        unload();
    }

    sound_base * sound_sdl_mixer::create_new( std::string s_name, std::string s_file , int group_id_number, int s_id )
    {
        return new sound_sdl_mixer( s_name, s_file, group_id_number, s_id );
    }

    float sound_sdl_mixer::get_volume()
    {
        if( sound_chunk != NULL )
        {
            float vol = Mix_VolumeChunk( sound_chunk, -1 );
            return 100.f * vol/ 128.f;
        }
        return -1;
    }

    bool sound_sdl_mixer::load( std::string s_file)
    {
        unload();
        sound_chunk = Mix_LoadWAV( s_file.c_str() );
        if( !sound_chunk )
        {
            sound_chunk = NULL;
            sound_loaded = false;
            sound_error =  Mix_GetError();
        }
        else
        {
            sound_loaded = true;
            sound_error = "";
        }
        return sound_loaded;
    }

    void sound_sdl_mixer::pause()
    {
        if( sound_chunk != NULL)
        {
            sound_error = "";
        }
    }

    int sound_sdl_mixer::play( int play_count ,int s_channel )
    {
        if( sound_chunk != NULL )
        {
            Mix_PlayChannel( s_channel, sound_chunk, play_count );
        }
        return -1;
    }

    void sound_sdl_mixer::set_volume( float vol )
    {
        //Since Mix_Chunk's volume goes from 0 to 128, we will do a quick conversion...
        if( vol < 0 )
        {
            vol = 0;
        }
        else
        {
            vol =  vol * 1.28;
        }
        Mix_VolumeChunk( sound_chunk, (int)vol );
    }

    void sound_sdl_mixer::stop()
    {

    }

    void sound_sdl_mixer::unload()
    {
        if( sound_chunk != NULL )
        {
            Mix_FreeChunk( sound_chunk );
            sound_chunk = NULL;
        }
    }

    music_sdl_mixer::music_sdl_mixer( std::string s_name, std::string s_file , int group_id_number, int s_id )
    {
        sound_id = s_id;
        sound_name = s_name;
        file_name = s_file;
        music_is_playing = false;

        sdl_music = NULL;
        load( s_file );
    }

    music_sdl_mixer::~music_sdl_mixer()
    {
        unload();
    }

    sound_base * music_sdl_mixer::create_new( std::string s_name, std::string s_file , int group_id_number, int s_id  )
    {
        return new music_sdl_mixer( s_name, s_file, group_id_number, s_id);
    }

    music_base * music_sdl_mixer::create_new_music( std::string s_name, std::string s_file , int group_id_number, int s_id )
    {
        return new music_sdl_mixer( s_name, s_file, group_id_number, s_id);
    }

    bool music_sdl_mixer::is_playing()
    {
        return false;
    }

    bool music_sdl_mixer::load( std::string s_file )
    {
        unload();
        sdl_music = Mix_LoadMUS( s_file.c_str() );
        if( sdl_music != NULL )
        {
            sound_loaded = true;
            sound_error = "";
        }
        else
        {
            sound_error = Mix_GetError();
            sound_loaded = false;
        }
        return sound_loaded;

    }

    void music_sdl_mixer::pause()
    {
        if( sdl_music != NULL)
        {
            Mix_PauseMusic();
        }
    }

    int music_sdl_mixer::play( int play_count , int s_channel )
    {
        if( sdl_music != NULL)
        {
            Mix_PlayMusic( sdl_music, play_count );
        }
    }

    void music_sdl_mixer::stop()
    {
        if( sdl_music != NULL)
        {

        }
    }

    void music_sdl_mixer::unload()
    {
        if( sdl_music != NULL )
        {
            Mix_FreeMusic( sdl_music );
            sdl_music = NULL;
        }
    }
}
