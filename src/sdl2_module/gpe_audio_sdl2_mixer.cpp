/*
gpe_audio_sdl2_mixer.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine  - SDL2 Module <https://gamepencil.net/sdl2-module/>


*/

#include "../gpe/gpe_common_includes.h"
#include "gpe_audio_sdl2_mixer.h"
#include "../gpe/gpe_error_logger.h"

namespace gpe
{
    bool init_sdl2_mixer_audio_system( int audio_frequency, Uint16 audio_format, int audio_max_channels, int audio_chunksize  )
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
        error_log->report("-audio_sdl2_system...");
        // load support for the OGG and MOD sample/music formats

        int flags[sound_format_max];
        std::string flag_names[sound_format_max];

        //Initialize SDL_mixer
        if( Mix_OpenAudio( audio_frequency, audio_format, audio_max_channels, audio_chunksize  ) == -1 )
        {
            error_log->report("-- Error initializing SDL_Mixer.");
            sound_is_working = false;
        }
        else
        {
            sound_is_working = true;
        }

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
        sound_is_format_supported[sound_format_wav] = true;
        at_least_one_type_supported = true;



        if( sound_music_object != NULL )
        {
            delete sound_music_object;
            sound_music_object = NULL;
        }

        sound_music_object = new music_sdl2_mixer("","",-1, -1 );

        if( standard_sound_object != NULL )
        {
            delete standard_sound_object;
            standard_sound_object = NULL;
        }
        standard_sound_object = new sound_sdl2_mixer("","",-1, -1 );
        sound_system_name = "sdl2_mixer";
        return sound_is_working;
    }

    void quit_sdl2_mixer_audio_system()
    {
        error_log->report("Quitting SDL_Mixer....");
        Mix_Quit();
        sound_system_name = "undefined";
    }

    Uint16 sdl2_mixer_audio_format_sample_size(Uint16 format)
    {
        return (format & 0xFF) / 8;
    }

    sound_sdl2_mixer::sound_sdl2_mixer( std::string s_name, std::string s_file , int group_id_number, int s_id )
    {
        sound_chunk = NULL;
        sound_loaded = false;
        sound_id = s_id;
        sound_name = s_name;
        file_name = s_file;
        load( s_file );
    }

    sound_sdl2_mixer::~sound_sdl2_mixer()
    {
        unload();
    }

     // Get chunk time length (in ms) given its size and current audio format
    int sound_sdl2_mixer::calculate_music_length()
    {
        if( sound_chunk == nullptr || sound_chunk == NULL )
        {
            sound_length_ms = 0;
            return 0;
        }

        Uint16 audioFormat;  // current audio format constant
        int audioFrequency,  // frequency rate of the current audio format
        audioChannelCount,  // number of channels of the current audio format
        audioAllocatedMixChannelsCount;  // number of mix channels allocated

        /* bytes / samplesize == sample points */
        Mix_QuerySpec( &audioFrequency, &audioFormat, &audioChannelCount );

        const Uint32 points = sound_chunk->alen / sdl2_mixer_audio_format_sample_size(audioFormat);

        /* sample points / channels == sample frames */
        const Uint32 frames = (points / audioChannelCount);

        /* (sample frames * 1000) / frequency == play length, in ms */
        sound_length_ms_total =  ((frames * 1000) / audioFrequency);
        computer_lengths();
        return sound_length_ms_total;
    }

    sound_base * sound_sdl2_mixer::create_new( std::string s_name, std::string s_file , int group_id_number, int s_id )
    {
        return new sound_sdl2_mixer( s_name, s_file, group_id_number, s_id );
    }

    float sound_sdl2_mixer::get_volume()
    {
        if( sound_chunk != NULL )
        {
            float vol = Mix_VolumeChunk( sound_chunk, -1 );
            return 100.f * vol/ 128.f;
        }
        return -1;
    }

    bool sound_sdl2_mixer::load( std::string s_file)
    {
        unload();
        sound_chunk = Mix_LoadWAV( s_file.c_str() );
        if( !sound_chunk )
        {
            sound_chunk = NULL;
            sound_loaded = false;
            sound_error =  Mix_GetError();
            sound_length_ms = 0;
            sound_length_seconds = 0;
            sound_length_minutes = 0;
            sound_length_hours = 0;
            sound_length_days = 0;
        }
        else
        {
            sound_loaded = true;
            sound_error = "";
            calculate_music_length();
        }

        return sound_loaded;
    }

    void sound_sdl2_mixer::pause()
    {
        if( sound_chunk != NULL)
        {
            sound_error = "";
        }
    }

    int sound_sdl2_mixer::play( int play_count ,int s_channel )
    {
        if( sound_chunk != NULL )
        {
            Mix_PlayChannel( s_channel, sound_chunk, play_count );
        }
        return -1;
    }

    void sound_sdl2_mixer::set_volume( float vol )
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

    void sound_sdl2_mixer::stop()
    {

    }

    void sound_sdl2_mixer::unload()
    {
        if( sound_chunk != NULL )
        {
            Mix_FreeChunk( sound_chunk );
            sound_chunk = NULL;
        }
    }

    music_sdl2_mixer::music_sdl2_mixer( std::string s_name, std::string s_file , int group_id_number, int s_id )
    {
        sound_id = s_id;
        sound_name = s_name;
        file_name = s_file;
        music_is_playing = false;

        sdl2_music = NULL;
        load( s_file );
    }

    music_sdl2_mixer::~music_sdl2_mixer()
    {
        unload();
    }


    sound_base * music_sdl2_mixer::create_new( std::string s_name, std::string s_file , int group_id_number, int s_id  )
    {
        return new music_sdl2_mixer( s_name, s_file, group_id_number, s_id);
    }

    music_base * music_sdl2_mixer::create_new_music( std::string s_name, std::string s_file , int group_id_number, int s_id )
    {
        return new music_sdl2_mixer( s_name, s_file, group_id_number, s_id);
    }

    bool music_sdl2_mixer::is_playing()
    {
        return false;
    }

    bool music_sdl2_mixer::load( std::string s_file )
    {
        unload();
        sdl2_music = Mix_LoadMUS( s_file.c_str() );
        if( sdl2_music != NULL )
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

    void music_sdl2_mixer::pause()
    {
        if( sdl2_music != NULL)
        {
            Mix_PauseMusic();
        }
    }

    int music_sdl2_mixer::play( int play_count , int s_channel )
    {
        if( sdl2_music != NULL)
        {
            Mix_PlayMusic( sdl2_music, play_count );
            return true;
        }
        return false;
    }

    void music_sdl2_mixer::stop()
    {
        if( sdl2_music != NULL)
        {

        }
    }

    void music_sdl2_mixer::unload()
    {
        if( sdl2_music != NULL )
        {
            Mix_FreeMusic( sdl2_music );
            sdl2_music = NULL;
        }
    }
}
