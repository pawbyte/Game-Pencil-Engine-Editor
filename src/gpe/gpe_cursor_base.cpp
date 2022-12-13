/*
gpe_cursor_base.cpp
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

#include "gpe_cursor_base.h"

namespace gpe
{
    cursor_controller_base * cursor_main_controller = nullptr;
    int cursor_controller_counter = 0;

    cursor_controller_base::cursor_controller_base()
    {
        cursor_controller_counter++;
        cursor_window_id = cursor_controller_counter;
        cursor_controller_type = "base";
        cursor_window_id = -1;

        for( int i_cursor = 0; i_cursor < cursor_default_type::max_default_cursor; i_cursor++ )
        {
            cursor_default_names.push_back("");
        }

        cursor_default_names[ cursor_default_type::arrow] = "arrow";
        cursor_default_names[ cursor_default_type::ibeam] = "ibeam";
        cursor_default_names[ cursor_default_type::wait ] = "wait";
        cursor_default_names[ cursor_default_type::crosshair ] = "crosshair";
        cursor_default_names[ cursor_default_type::wait_arrow] = "waitarrow";

        cursor_default_names[ cursor_default_type::sizenwse] = "sizenwse";
        cursor_default_names[ cursor_default_type::sizenesw] = "sizenesw";
        cursor_default_names[ cursor_default_type::sizewe ] = "sizewe";
        cursor_default_names[ cursor_default_type::sizens] = "sizens";
        cursor_default_names[ cursor_default_type::sizeall ] = "sizeall";

        cursor_default_names[ cursor_default_type::no ] = "no";
        cursor_default_names[ cursor_default_type::hand ] = "hand";
        cursor_previous = cursor_current = cursor_default_names[ cursor_default_type::arrow ];
    }

    cursor_controller_base::~cursor_controller_base()
    {

    }

    void cursor_controller_base::cursor_change( std::string new_cursor )
    {
        cursor_current = new_cursor;
    }

    void cursor_controller_base::cursor_change_system( int system_cursor_id )
    {
        if( system_cursor_id >=0 && system_cursor_id < (int)cursor_default_names.size() )
        {
            cursor_change( cursor_system_name( system_cursor_id ) );
        }
    }

    void cursor_controller_base::cursor_clear_dynamic()
    {

    }

    bool cursor_controller_base::cursor_contains( std::string cursor_name )
    {
        return false;
    }

    bool cursor_controller_base::cursor_create_from_image(std::string f_name )
    {
        return false;
    }

    int cursor_controller_base::cursor_map_size()
    {
        return 0;
    }

    std::string cursor_controller_base::cursor_system_name( int cId )
    {
        return "";
    }

    int cursor_controller_base::equals( cursor_controller_base * other )
    {
        if( other !=nullptr )
        {
            if( other->get_id() == cursor_controller_id )
            {
                if( other->cursor_controller_type == cursor_controller_type )
                {
                    if( other->cursor_window_id == cursor_window_id )
                    {
                        return true;
                    }
                }
            }
            return 0;
        }
        return -1;
    }

    std::string cursor_controller_base::get_type()
    {
        return cursor_controller_type;
    }

    int cursor_controller_base::get_id()
    {
        return cursor_controller_id;
    }

    int cursor_controller_base::get_window_id()
    {
        return cursor_window_id;
    }

    void cursor_controller_base::process_cursors()
    {

    }
}
