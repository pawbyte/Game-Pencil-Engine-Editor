/*
gpe_audio_raylib.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef gpe_audio_raylib_h
#define gpe_audio_raylib_h

#include "../gpe/gpe_audio_base.h"
#include "raylib.h"
#include "config.h"
namespace gpe
{
    class sound_raylib: public sound_base
    {
        private:
            Sound  raylib_sound;
            int sound_plays_remaining;
            float current_volume; //In raylib this is between 0.f and 1.f.
            //The base class uses between 0 to 100, so conversions will be made on getters/setters
        public:
            sound_raylib( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            ~sound_raylib();
            sound_base * create_new( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            float get_volume();

            bool load( std::string s_file );
            void pause();
            int play( int play_count = 0, int s_channel = -1 );
            void set_volume( float vol );
            void stop();
            void unload();
    };

    class music_raylib: public music_base
    {
        private:
            Music raylib_music;
        public:
            music_raylib( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            ~music_raylib();
            sound_base * create_new( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1);
            music_base * create_new_music( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            bool is_playing();
            bool load( std::string s_file );
            void pause();
            int play( int play_count = 0, int s_channel = -1 );
            void stop();
            void unload();
    };

    bool init_raylib_audio_system();
    void quit_raylib_audio_system();
}

#endif //gpe_audio_raylib_h
