/*
gpe_sdl.cpp
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

#include "gpe_sdl.h"

namespace gpe
{
    bool init_sdl_all_systems()
    {
        bool inittd_succesffully = true;
        //First we initialize SDL2
        if( init_sdl_main_system() == false )
        {
            error_log->report( "Unable to properly initialize sdl_main_system! \n" );
            inittd_succesffully = false;
        }

        //Then we begin our window
        if( init_sdl_window_system() == false )
        {
            error_log->report( "Unable to properly initialize sdl_window_system! \n" );
            inittd_succesffully = false;
        }

        //We will be using the SDL Graphics System...
        if( init_sdl_render_package() == false )
        {
            error_log->report( "Unable to properly initialize sdl_render_package! \n" );
            inittd_succesffully = false;
        }

        if( init_sdl_cursor_system()  )
        {
            error_log->report( "Cursor system updated to sdl_cursor_system! \n" );
        }
        else
        {
            error_log->report( "Unable to properly initialize sdl_cursor_system! \n" );
            inittd_succesffully = false;
        }

        if( init_sdl_input_system() == false )
        {
            error_log->report( "Unable to properly initialize sdl_input_system! \n" );
            inittd_succesffully = false;
        }

        if( init_sdl_font_system() == false )
        {
            error_log->report( "Unable to properly initialize sdl_font_system! \n" );
            inittd_succesffully = false;
        }

        //We will be using the SDL Audio System...
        if( init_sdl_mixer_audio_system() == false )
        {
            error_log->report( "Unable to properly initialize sdl_audio_system! \n" );
            inittd_succesffully = false;
        }

        if( init_sdl_time_system() == false )
        {
            error_log->report( "Unable to properly initialize sdl_time_system! \n" );
            inittd_succesffully = false;
        }

        return inittd_succesffully;
    }

    void quit_sdl_all_systems()
    {

    }

}
