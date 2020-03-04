/*
main.cpp
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

 ____                ____        _
|  _ \ __ ___      _| __ ) _   _| |_ ___
| |_) / _` \ \ /\ / /  _ \| | | | __/ _ \
|  __/ (_| |\ V  V /| |_) | |_| | ||  __/
|_|   \__,_| \_/\_/ |____/ \__, |\__\___|
                           |___/
Created By PawByte
Attribution required at start of program and in credits.
SDL 2.0.9 used for this version...
*/

#include "gpe/gpe.h"
#include "sdl_libs/gpe_sdl.h"
#include "gpe_editor/gpe_game_master_itenary.h"

int main( int argc, char* args[] )
{
    int gameFailed = 0;
    gpe::init_settings(argc, args , "PawByte","GPE_Editor","game_errors.txt" );

    //Initialize
    if( gpe::init_core_engine(argc, args,"game_errors.txt" ) == false )
    {
        gpe::error_log->report("Unable to properly initialize gpe_core_engine!\n");
        return -2;
    }

    if( gpe::init_sdl_all_systems() == false )
    {
        gameFailed = -3;
    }

    if(gameFailed !=0)
    {
        gpe::quit_core_engine();
        return gameFailed;
    }

    if( gpe::init_engine() == false )
    {
        gameFailed = 1;
    }

    gpe::time_keeper->set_fps( gpe::settings->defaultFPS );

    if( init_gpe_master_itenary( argc, args ) == false )
    {
        gameFailed = -2000;
    }


    if(gameFailed !=0 )
    {
        gpe::quit_engine();
        gpe::quit_core_engine();
        return gameFailed;
    }

    if( gpe::game_runtime->game_loop() )
    {
        gpe::error_log->report("Completed Game Loop....");
    }

    //Clean up
    quit_gpe_master_itenary();
    gpe::error_log->report("Deleting GPE...");
    gpe::quit_engine();

    //We now remove the independent audio engine
    gpe::quit_sdl_font_system();
    gpe::quit_sdl_mixer_audio_system();
    gpe::quit_sdl_render_package();
    gpe::quit_sdl_window_system();
    gpe::quit_sdl_main_system();

    gpe::error_log->report("Program Exited with Return Status 0...");
    gpe::quit_core_engine(); //Removes the  remaining pieces of the engine
    return 0;
}
