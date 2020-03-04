/*
gpe_cursor_base.cpp
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

#include "gpe_cursor_base.h"

namespace gpe
{
    cursor_controller_base * cursor_main_controller = NULL;

    cursor_controller_base::cursor_controller_base()
    {
        for( int iC = 0; iC <= cursor_default_type::max_default_cursor; iC ++ )
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

    void cursor_controller_base::cursor_change( std::string newCursor )
    {
        cursor_current = newCursor;
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

    bool cursor_controller_base::cursor_contains( std::string cursorName )
    {
        return false;
    }

    bool cursor_controller_base::cursor_create_from_image(std::string fName )
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

    void cursor_controller_base::process_cursors()
    {

    }
}
