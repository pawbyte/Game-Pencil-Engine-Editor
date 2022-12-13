/*
game_state_splash.h
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#ifndef game_state_splash_h
#define game_state_splash_h

#include "gpe/gpe.h"
#include "gpe/internal_libs/semath.h"


enum class splash_logo_state
{
    state_enter = 0,
    state_display = 1,
    state_exit = 2,
};

class splash_logo
{
    protected:
        std::string logo_name;
        gpe::texture_base * logo_texture;
        gpe::sound_base  * audio_effect_enter;
        gpe::sound_base  * audio_effect_display;
        gpe::sound_base  * audio_effect_exit;
    public:
        bool always_display_logo;
        bool always_display_text;
        splash_logo_state current_splash_state;
        std::string text_top;
        std::string text_bottom;
        gpe::color * bg_color;
        gpe::color * text_top_color;
        gpe::color * text_bottom_color;
        uint64_t ms_displayed, ms_displayed_counter;
        uint64_t ms_entered, ms_entered_counter;
        uint64_t ms_exit, ms_exit_counter;
        splash_logo( std::string name , std::string logo_file_location, std::string t_text, std::string b_text );
        ~splash_logo();
        std::string get_name();
        virtual void process_self( float delta_time );
        virtual void render();
};

class splash_screen: public gpe::program_state
{
    private:
        //Positions to keep track of loading stance
        int current_position;
        std::vector< splash_logo * > logos;

    public:
        splash_screen(std::string s_name );
        ~splash_screen();
        void add_splash_logo( splash_logo * new_logo );
        void apply_logic();
        void clean_up();

        void end_state();
        void process_input();
        void remove_all_logos( );
        void remove_splash_logo( std::string logo_name );
        void render();
        void start_state();
};

#endif // game_state_splash_h
