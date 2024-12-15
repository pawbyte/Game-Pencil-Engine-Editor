/*
gpe_audio_sdl2_mixer.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef gpe_audio_sdl2_mixer_h
#define gpe_audio_sdl2_mixer_h

#include "SDL_mixer.h"
#include "../gpe/gpe_audio_base.h"
namespace gpe
{
    class sound_sdl2_mixer: public sound_base
    {
        private:
            Mix_Chunk * sound_chunk;
        public:
            sound_sdl2_mixer( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            ~sound_sdl2_mixer();
            int calculate_music_length();
            sound_base * create_new( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            float get_volume();

            bool load( std::string s_file );
            void pause();
            int play( int play_count = 0, int s_channel = -1 );
            void set_volume( float vol );
            void stop();
            void unload();
    };

    class music_sdl2_mixer: public music_base
    {
        private:
            Mix_Music * sdl2_music;
        public:
            music_sdl2_mixer( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            ~music_sdl2_mixer();
            sound_base * create_new( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1);
            music_base * create_new_music( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            bool is_playing();
            bool load( std::string s_file );
            void pause();
            int play( int play_count = 0, int s_channel = -1 );
            void stop();
            void unload();
    };

    class audio_controller_sdl2_mixer: public audio_controller_base
    {
        protected:


        public:
            audio_controller_sdl2_mixer();
            ~audio_controller_sdl2_mixer();
    };

    bool init_sdl2_mixer_audio_system(int audio_frequency  = 22050, Uint16 audio_format = MIX_DEFAULT_FORMAT, int audio_max_channels = MIX_DEFAULT_CHANNELS, int audio_chunksize  = 1024);
    void quit_sdl2_mixer_audio_system();
    Uint16 sdl2_mixer_audio_format_sample_size(Uint16 format);
}

#endif //gpe_audio_sdl2_h
