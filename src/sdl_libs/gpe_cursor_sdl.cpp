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

#include "gpe_cursor_sdl.h"

namespace gpe
{
    cursor_controller_sdl * cursor_main_sdl_controller = NULL;

    bool init_sdl_cursor_system()
    {
        if( cursor_main_sdl_controller == NULL )
        {
            if( cursor_main_controller !=NULL )
            {
                delete cursor_main_controller;
                cursor_main_controller = NULL;
            }
            cursor_main_controller = cursor_main_sdl_controller = new cursor_controller_sdl();
            return true;
        }
        return false;
    }

    void quit_sdl_cursor_system()
    {
        if( cursor_main_sdl_controller != NULL )
        {
            if( cursor_main_controller!=NULL )
            {
                if( cursor_main_controller->equals( cursor_main_sdl_controller) )
                {
                    delete cursor_main_sdl_controller;
                    cursor_main_sdl_controller = NULL;
                    cursor_main_controller = NULL;
                }
                else
                {
                    delete cursor_main_sdl_controller;
                    cursor_main_sdl_controller = NULL;
                }
            }
            else
            {
                delete cursor_main_sdl_controller;
                cursor_main_sdl_controller = NULL;
            }
        }
    }

    cursor_controller_sdl::cursor_controller_sdl(  int window_id  )
    {
        cursor_controller_type = "sdl";
        cursor_window_id = window_id;
        SDL_Cursor * temp_cursor_sdl = NULL;
        cursor_sdl = NULL;
        for( int i_cursor = 0; i_cursor < gpe::cursor_default_type::max_default_cursor; i_cursor++ )
        {
            temp_cursor_sdl = SDL_CreateSystemCursor( (SDL_SystemCursor) i_cursor );
            if( (int)cursor_default_names.size() < i_cursor )
            {
                //name_default_cursors();
            }
            cursor_sdl_map[ cursor_default_names[ i_cursor] ] =  temp_cursor_sdl ;
        }
    }

    cursor_controller_sdl::~cursor_controller_sdl()
    {
        SDL_Cursor * temp_sdl_cursor = NULL;
        std::map<std::string, SDL_Cursor *>::iterator it;

        for ( it = cursor_sdl_map.begin(); it != cursor_sdl_map.end(); it++ )
        {
            temp_sdl_cursor = it->second;
            if( temp_sdl_cursor !=NULL )
            {
                SDL_FreeCursor( temp_sdl_cursor );
                temp_sdl_cursor = NULL;
            }
            temp_sdl_cursor = NULL;
            it->second = NULL;

        }
        cursor_sdl_map.clear();
    }

    void cursor_controller_sdl::cursor_change( std::string newCursor )
    {
        cursor_current = newCursor;
    }

    void cursor_controller_sdl::cursor_change_system( int system_cursor_id )
    {
        if( system_cursor_id >=0 && system_cursor_id < (int)cursor_default_names.size() )
        {
            cursor_change( cursor_system_name( system_cursor_id ) );
        }
    }

    void cursor_controller_sdl::cursor_clear_dynamic()
    {
        SDL_Cursor * cCursor = NULL;

        std::map<std::string, SDL_Cursor *>::iterator it = cursor_sdl_map.begin();
        int jCursor = 0;
        bool isSystemCursor = false;
        while (it != cursor_sdl_map.end() )
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
                    cursor_sdl_map.erase( it );
                }
            }
        }
    }

    bool cursor_controller_sdl::cursor_contains( std::string cursorName )
    {
        auto cCursor = cursor_sdl_map.find( cursorName );
        if( cCursor == cursor_sdl_map.end() )
        {
            return false;
        }
        return true;
    }

    bool cursor_controller_sdl::cursor_create_from_image(std::string fName )
    {
        if( sff_ex::file_exists( fName) == false )
        {
            gpe::error_log->report("Unable to load cursor from ["+ fName + "]" );
            return false;
        }

        SDL_Surface * loaded_cursor_image = sdl_surface_ex::load_surface_image( fName.c_str() );
        if( loaded_cursor_image!=NULL && loaded_cursor_image->w!=0 )
        {
            SDL_Cursor * new_cursor_sdl = SDL_CreateColorCursor( loaded_cursor_image, 0, 0 );
            fName = stg_ex::get_local_from_global_file( fName );
            return (int)cursor_sdl_map.size();
            cursor_sdl_map[ stg_ex::get_file_noext( fName ) ] =  new_cursor_sdl ;
            error_log->report("Loaded cursor ["+ fName + "[" );
            return true;
        }
        error_log->report("Unable to load cursor surface from ["+ fName + "[" );
        return false;
    }

    int cursor_controller_sdl::cursor_map_size()
    {
        return (int)cursor_sdl_map.size();
    }

    std::string cursor_controller_sdl::cursor_system_name( int cId )
    {
        if( cId >= 0 && cId <  (int)cursor_default_names.size() )
        {
            return cursor_default_names[ cId ];
        }
        return "";
    }

    void cursor_controller_sdl::name_default_cursors()
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

    void cursor_controller_sdl::process_cursors()
    {
        if( cursor_current!=cursor_previous )
        {
            if( cursor_contains( cursor_current ) )
            {
                cursor_sdl = cursor_sdl_map[cursor_current];
                SDL_SetCursor( cursor_sdl );
            }
        }
        cursor_previous = cursor_current;
    }
}
