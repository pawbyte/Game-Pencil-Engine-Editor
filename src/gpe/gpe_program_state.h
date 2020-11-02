/*
gpe_program_state.h
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

#ifndef gpe_program_state_h
#define gpe_program_state_h

#include "gpe_artist_base.h"
#include "gpe_common_includes.h"
#include "gpe_render_package_handler.h"

namespace gpe
{
    //Program State base class
    class program_state
    {
        protected:
            color * bg_color;
            texture_base * bg_texture;
            std::string error_message;
            int error_number;
            bool error_occurred;
            std::string state_name;
            //These are used to easily transition from one state to another
            std::string state_next_name;
            std::string state_previous_name;
        public:
            program_state();
            virtual ~program_state();

            virtual void apply_logic() = 0;
            virtual void clean_up() = 0;
            virtual void end_state() = 0;

            std::string get_state_name();
            std::string get_state_name_next();
            std::string get_state_name_previous();

            virtual void process_input() = 0;

            virtual void render();

            virtual void set_background_color( uint8_t r , uint8_t g, uint8_t b, uint8_t a=255 );
            virtual void set_background_image( std::string bg_file );

            virtual bool set_state_name_next( std::string s_name );
            virtual bool set_state_name_previous( std::string s_name );
            virtual void start_state() = 0;
    };
}

#endif //gpe_program_state_h
