/*
game_state_credits_base.h
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

#ifndef game_state_credits_base_h
#define game_state_credits_base_h

#include "gpe/gpe.h"
#include "other_libs/semath.h"

class app_credits_basic: public gpe::program_state
{
    private:
        gpe::color * bg_color;
        gpe::texture_base * bg_texture;

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

#endif // game_state_credits_base_h
