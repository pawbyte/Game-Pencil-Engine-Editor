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

-Game Pencil Engine  - SDL2 Module <https://gamepencil.net/sdl2-module/>


*/

#include "gpe_cursor_sdl2.h"

namespace gpe
{
    cursor_controller_sdl2 * cursor_main_sdl2_controller = NULL;

    bool init_sdl2_cursor_system()
    {
        if( cursor_main_sdl2_controller == NULL )
        {
            if( cursor_main_controller !=NULL )
            {
                delete cursor_main_controller;
                cursor_main_controller = NULL;
            }
            cursor_main_controller = cursor_main_sdl2_controller = new cursor_controller_sdl2();
            return true;
        }
        return false;
    }

    void quit_sdl2_cursor_system()
    {
        if( cursor_main_sdl2_controller != NULL )
        {
            if( cursor_main_controller!=NULL )
            {
                if( cursor_main_controller->equals( cursor_main_sdl2_controller) )
                {
                    delete cursor_main_sdl2_controller;
                    cursor_main_sdl2_controller = NULL;
                    cursor_main_controller = NULL;
                }
                else
                {
                    delete cursor_main_sdl2_controller;
                    cursor_main_sdl2_controller = NULL;
                }
            }
            else
            {
                delete cursor_main_sdl2_controller;
                cursor_main_sdl2_controller = NULL;
            }
        }
    }

    cursor_controller_sdl2::cursor_controller_sdl2(  int window_id  )
    {
        cursor_controller_type = "sdl";
        cursor_window_id = window_id;
        SDL_Cursor * temp_cursor_sdl2 = NULL;
        cursor_sdl2 = NULL;
        for( int i_cursor = 0; i_cursor < gpe::cursor_default_type::max_default_cursor; i_cursor++ )
        {
            temp_cursor_sdl2 = SDL_CreateSystemCursor( (SDL_SystemCursor) i_cursor );
            if( (int)cursor_default_names.size() < i_cursor )
            {
                //name_default_cursors();
            }
            cursor_sdl2_map[ cursor_default_names[ i_cursor] ] =  temp_cursor_sdl2 ;
        }
    }

    cursor_controller_sdl2::~cursor_controller_sdl2()
    {
        SDL_Cursor * temp_sdl2_cursor = NULL;
        std::map<std::string, SDL_Cursor *>::iterator it;

        for ( it = cursor_sdl2_map.begin(); it != cursor_sdl2_map.end(); it++ )
        {
            temp_sdl2_cursor = it->second;
            if( temp_sdl2_cursor !=NULL )
            {
                SDL_FreeCursor( temp_sdl2_cursor );
                temp_sdl2_cursor = NULL;
            }
            temp_sdl2_cursor = NULL;
            it->second = NULL;

        }
        cursor_sdl2_map.clear();
    }

    void cursor_controller_sdl2::cursor_change( std::string new_cursor )
    {
        cursor_current = new_cursor;
    }

    void cursor_controller_sdl2::cursor_change_system( int system_cursor_id )
    {
        if( system_cursor_id >=0 && system_cursor_id < (int)cursor_default_names.size() )
        {
            cursor_change( cursor_system_name( system_cursor_id ) );
        }
    }

    void cursor_controller_sdl2::cursor_clear_dynamic()
    {
        SDL_Cursor * cCursor = NULL;

        std::map<std::string, SDL_Cursor *>::iterator it = cursor_sdl2_map.begin();
        int jCursor = 0;
        bool isSystemCursor = false;
        while (it != cursor_sdl2_map.end() )
        {
            cCursor = it->second;
            isSystemCursor = false;
            if( cCursor !=NULL )
            {
                for( jCursor = 0; jCursor < (int)cursor_default_names.size(); jCursor++)
                {
                    if( it->first == cursor_default_names[jCursor] )
                    {
                        isSystemCursor = true;
                    }
                }

                if( isSystemCursor == false )
                {
                    SDL_FreeCursor( cCursor);
                    cursor_sdl2_map.erase( it );
                }
            }
        }
    }

    bool cursor_controller_sdl2::cursor_contains( std::string cursor_name )
    {
        auto cCursor = cursor_sdl2_map.find( cursor_name );
        if( cCursor == cursor_sdl2_map.end() )
        {
            return false;
        }
        return true;
    }

    bool cursor_controller_sdl2::cursor_create_from_image(std::string f_name )
    {
        if( sff_ex::file_exists( f_name) == false )
        {
            gpe::error_log->report("Unable to load cursor from ["+ f_name + "]" );
            return false;
        }

        SDL_Surface * loaded_cursor_image = sdl2_surface_ex::load_surface_image( f_name.c_str() );
        if( loaded_cursor_image!=NULL && loaded_cursor_image->w!=0 )
        {
            SDL_Cursor * new_cursor_sdl2 = SDL_CreateColorCursor( loaded_cursor_image, 0, 0 );
            f_name = stg_ex::get_local_from_global_file( f_name );
            return (int)cursor_sdl2_map.size();
            cursor_sdl2_map[ stg_ex::get_file_noext( f_name ) ] =  new_cursor_sdl2 ;
            error_log->report("Loaded cursor ["+ f_name + "[" );
            return true;
        }
        error_log->report("Unable to load cursor surface from ["+ f_name + "[" );
        return false;
    }

    int cursor_controller_sdl2::cursor_map_size()
    {
        return (int)cursor_sdl2_map.size();
    }

    std::string cursor_controller_sdl2::cursor_system_name( int cId )
    {
        if( cId >= 0 && cId <  (int)cursor_default_names.size() )
        {
            return cursor_default_names[ cId ];
        }
        return "";
    }

    void cursor_controller_sdl2::name_default_cursors()
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

    void cursor_controller_sdl2::process_cursors()
    {
        if( cursor_current!=cursor_previous )
        {
            if( cursor_contains( cursor_current ) )
            {
                cursor_sdl2 = cursor_sdl2_map[cursor_current];
                SDL_SetCursor( cursor_sdl2 );
            }
        }
        cursor_previous = cursor_current;
    }
}
