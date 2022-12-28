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

//This is optional of course, but the basis for our first party credits

class game_credit_section_base
{
    public:
        std::vector< std::string > section_lines;
        game_credit_section_base(std:: string name );
        virtual ~game_credit_section_base();
        virtual void add_line( std::string n_line );
        virtual void clear_lines();
        int get_line_count();
        std::string get_line_text();
};

class game_state_credits_base: public gpe::program_state
{
    private:
        std::vector< game_credit_section_base * > credits_sections;;
        //Positions to keep track of loading stance
    public:
        game_state_credits_base(std::string s_name );
        virtual ~game_state_credits_base();
        virtual void apply_logic();
        virtual void clean_up();

        virtual void end_state();
        virtual void process_input();
        virtual void render();
        virtual void start_state();
};

#endif // game_state_credits_base_h
