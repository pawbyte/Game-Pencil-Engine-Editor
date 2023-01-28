/*
game_state_main_menu_base.h
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

#ifndef game_state_main_menu_base_h
#define game_state_main_menu_base_h

#include "gpe/gpe.h"
#include "gpe/internal_libs/semath.h"

//This is optional of course, but the basis for our first party credits

class main_menu_option
{
    public:
        bool is_unlocked;
        gpe::color * color_hover;
        gpe::color * color_normal;
        gpe::color * color_selected;
        gpe::sound_base * sound_hover;
        gpe::sound_base * sound_select;
        std::string seeked_state_on_selected;
        main_menu_option();
        ~main_menu_option();
};

class game_state_main_menu_base: public gpe::program_state
{
    private:
        gpe::sound_base * option_sound_hover;
        gpe::sound_base * option_sound_select;

        gpe::color * option_color_hover;
        gpe::color * option_color_normal;
        gpe::color * option_color_selected;
    public:
        game_state_main_menu_base(std::string s_name );
        virtual ~game_state_main_menu_base();
        virtual void apply_logic();
        virtual void clean_up();

        virtual void end_state();
        virtual void process_input();
        virtual void render();
        virtual void start_state();
};

#endif // game_state_main_menu_base_h
