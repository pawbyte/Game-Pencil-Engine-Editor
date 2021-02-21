/*
gpe_program_state.cpp
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

#include "gpe_program_state.h"

namespace gpe
{
    program_state::program_state( )
    {
        error_number = -1;
        error_occurred = false;
        state_name = "gpe_default_state_name";
        bg_color = new color("bg_color",0,0,0,255 );
        bg_texture = NULL;
        if( rph!=NULL )
        {
            bg_texture = rph->get_new_texture();
        }
    }

    program_state::~program_state()
    {

    }

    std::string program_state::get_state_name()
    {
        return state_name;
    }

    std::string program_state::get_state_name_next()
    {
        return state_next_name;
    }

    std::string program_state::get_state_name_previous()
    {
        return state_previous_name;
    }

    void program_state::render()
    {
        if( gcanvas!= NULL )
        {
            gcanvas->render_rectangle( 0, 0, gpe::screen_width, gpe::screen_height, bg_color, false, 255 );
        }

        if( bg_texture!= NULL )
        {
            bg_texture->render_tex_resized( 0, 0, gpe::screen_width, gpe::screen_height, NULL );
        }
    }

    void program_state::set_background_color( uint8_t r , uint8_t g, uint8_t b, uint8_t a )
    {
        if( bg_color != NULL )
        {
            bg_color->change_rgba(r, g, b, a );
            return;
        }
        bg_color = new color("bg_color",r,g,b,a );

    }

    void program_state::set_background_image( std::string bg_file )
    {
        if( bg_texture = NULL )
        {
            bg_texture->load_new_texture( renderer_main, bg_file );
            return;
        }

        if( rph!=NULL )
        {
            bg_texture = rph->get_new_texture();
            bg_texture->load_new_texture( renderer_main, bg_file );
        }
    }

    bool program_state::set_state_name_next( std::string s_name )
    {
        state_next_name = s_name;
    }

    bool program_state::set_state_name_previous( std::string s_name )
    {
        state_previous_name = s_name;
        return true;
    }
}
