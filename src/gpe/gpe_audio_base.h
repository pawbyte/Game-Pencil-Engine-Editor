/*
gpe_audio_base.h
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

#include <string>

#ifndef gpe_audio_base_h
#define gpe_audio_base_h

namespace gpe
{
    //For AUDIO Properties
    extern bool sound_is_working;
    extern float sound_global_volume;

    const int sound_format_acc = 0;
    const int sound_format_mod = 1;
    const int sound_format_mp3 = 2;
    const int sound_format_ogg = 3;
    const int sound_format_wav = 4;
    const int sound_format_xm = 5;

    const int sound_format_max = 10; //Adding some extras for extensions...

    extern bool sound_is_format_supported[ sound_format_max ];
    extern std::string sound_type_names[ sound_format_max ];
    extern std::string sound_system_name;

    class sound_base
    {
        protected:
            std::string sound_type;
            std::string sound_name;
            std::string sound_error;
            int sound_id;
            int sound_group_id;
            bool sound_loaded;
            std::string file_name;
            int last_channel_played;
        public:
            sound_base();
            sound_base( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            virtual ~sound_base();
            virtual sound_base * create_new( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            std::string get_error();
            std::string get_file_name();
            int get_group_id();
            int get_id();
            std::string get_name();
            std::string get_type();
            virtual float get_volume();
            bool is_loaded();

            virtual bool load( std::string s_file );
            virtual void pause();
            virtual int play( int play_count = 0, int s_channel = -1 );
            virtual void set_volume( float vol );

            virtual void stop();
            virtual void unload();
    };

    class music_base: public sound_base
    {
        protected:
            bool music_is_playing;
        public:
            music_base();
            music_base( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            virtual ~music_base();
            virtual sound_base * create_new( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1 );
            virtual music_base * create_new_music( std::string s_name, std::string s_file , int group_id_number =-1, int s_id = -1);
            virtual bool is_playing();
            virtual bool load( std::string s_file );
            virtual void pause();
            virtual int play( int play_count = 0, int s_channel = -1 );
            virtual void stop();
            virtual void unload();
    };

    extern music_base * sound_music_object;
    extern sound_base * standard_sound_object;
};

#endif //gpe_audio_base_h
