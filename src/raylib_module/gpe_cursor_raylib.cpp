/*
gpe_cursor_base.cpp
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

#include "gpe_cursor_raylib.h"

namespace gpe
{
    cursor_controller_raylib * cursor_main_raylib_controller = NULL;

    bool init_raylib_cursor_system()
    {
        if( cursor_main_raylib_controller == NULL )
        {
            if( cursor_main_controller !=NULL )
            {
                delete cursor_main_controller;
                cursor_main_controller = NULL;
            }
            cursor_main_controller = cursor_main_raylib_controller = new cursor_controller_raylib();
            return true;
        }
        return false;
    }

    void quit_raylib_cursor_system()
    {
        if( cursor_main_raylib_controller != NULL )
        {
            if( cursor_main_controller!=NULL )
            {
                if( cursor_main_controller->equals( cursor_main_raylib_controller) )
                {
                    delete cursor_main_raylib_controller;
                    cursor_main_raylib_controller = NULL;
                    cursor_main_controller = NULL;
                }
                else
                {
                    delete cursor_main_raylib_controller;
                    cursor_main_raylib_controller = NULL;
                }
            }
            else
            {
                delete cursor_main_raylib_controller;
                cursor_main_raylib_controller = NULL;
            }
        }
    }

    cursor_controller_raylib::cursor_controller_raylib(  int window_id  )
    {
        cursor_controller_type = "raylib";
        cursor_window_id = 0;
        EnableCursor();
        ShowCursor();
        cursor_raylib_id = GetMouseCursor();

        cursor_raylib_map[ cursor_default_names[ cursor_default_type::arrow ] ] =  MOUSE_CURSOR_ARROW;
        cursor_raylib_map[ cursor_default_names[cursor_default_type::ibeam ] ] =  MOUSE_CURSOR_IBEAM;
        cursor_raylib_map[ cursor_default_names[cursor_default_type::wait ] ] =  MOUSE_CURSOR_NOT_ALLOWED;
        cursor_raylib_map[ cursor_default_names[cursor_default_type::crosshair ] ] =  MOUSE_CURSOR_CROSSHAIR;
        cursor_raylib_map[ cursor_default_names[cursor_default_type::wait_arrow ] ] =  MOUSE_CURSOR_NOT_ALLOWED;
        cursor_raylib_map[ cursor_default_names[cursor_default_type::sizenwse ] ] =   MOUSE_CURSOR_RESIZE_NWSE;
        cursor_raylib_map[ cursor_default_names[cursor_default_type::sizenesw ] ] =   MOUSE_CURSOR_RESIZE_NESW;
        cursor_raylib_map[ cursor_default_names[cursor_default_type::sizewe ] ] =  MOUSE_CURSOR_RESIZE_EW;
        cursor_raylib_map[ cursor_default_names[cursor_default_type::sizens ] ] =  MOUSE_CURSOR_RESIZE_NS;
        cursor_raylib_map[ cursor_default_names[cursor_default_type::sizeall ] ] =  MOUSE_CURSOR_RESIZE_ALL;
        cursor_raylib_map[ cursor_default_names[cursor_default_type::no ] ] =  MOUSE_CURSOR_NOT_ALLOWED;
        cursor_raylib_map[ cursor_default_names[cursor_default_type::hand ] ] =  MOUSE_CURSOR_POINTING_HAND ;

    }

    cursor_controller_raylib::~cursor_controller_raylib()
    {
        cursor_raylib_map.clear();
    }

    void cursor_controller_raylib::cursor_change( std::string new_cursor )
    {
        cursor_current = new_cursor;
    }

    void cursor_controller_raylib::cursor_change_system( int system_cursor_id )
    {
        if( system_cursor_id >=0 && system_cursor_id < (int)cursor_default_names.size() )
        {
            cursor_change( cursor_system_name( system_cursor_id ) );
        }
    }

    void cursor_controller_raylib::cursor_clear_dynamic()
    {

    }

    bool cursor_controller_raylib::cursor_contains( std::string cursor_name )
    {
        auto cCursor = cursor_raylib_map.find( cursor_name );
        if( cCursor == cursor_raylib_map.end() )
        {
            return false;
        }
        return true;
    }

    bool cursor_controller_raylib::cursor_create_from_image(std::string f_name )
    {
        if( main_file_url_manager->file_exists( f_name) == false )
        {
            gpe::error_log->report("Unable to load cursor from ["+ f_name + "]" );
            return false;
        }


        error_log->report("Unable to load cursor image from ["+ f_name + "[" );
        return false;
    }

    int cursor_controller_raylib::cursor_map_size()
    {
        return (int)cursor_raylib_map.size();
    }

    std::string cursor_controller_raylib::cursor_system_name( int cId )
    {
        if( cId >= 0 && cId <  (int)cursor_default_names.size() )
        {
            return cursor_default_names[ cId ];
        }
        return "";
    }

    void cursor_controller_raylib::name_default_cursors()
    {
        cursor_default_names.clear();
        for( int i_cursor = 0; i_cursor < gpe::cursor_default_type::max_default_cursor; i_cursor++ )
        {
            cursor_default_names[ i_cursor] = "";
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

    void cursor_controller_raylib::process_cursors()
    {
        if( cursor_current!=cursor_previous )
        {
            if( cursor_contains( cursor_current ) )
            {
                cursor_raylib_id = cursor_raylib_map[cursor_current];
                SetMouseCursor( cursor_raylib_id );
            }
        }
        cursor_previous = cursor_current;
    }
}
