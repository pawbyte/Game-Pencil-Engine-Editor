/*
main.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

 ___                ___
|   \ _ __      | _ )    | |_ __
| |_) / ` \ \ /\ / /   \| | | | _/  \
|  _/ (_| |\ V  V /| |_) | |_| | ||  _/
|_|   \__,_| \_/\_/ |____/ \__, |\__\___|
                           |___/
Created By PawByte
Attribution required at start of program and in credits.
SDL 2.0.9 used for this version...
*/

#include "gpe/gpe.h"
#include "sdl2_module/gpe_sdl2.h"
#include "game_state_credits_base.h"
#include "game_state_load.h"
#include "game_state_main_menu_base.h"
#include "game_state_master.h"
#include "game_state_splash.h"
#include "gpe_editor/editor/gpe_editor_state.h"
#include "game_state_shape_test.h"

int main( int argc, char* args[] )
{
    int gameFailed = 0;
    gpe::init_settings(argc, args , "PawByte","GPE_Editor","game_errors.txt" );

    //Initialize the base objects of gpe
    if( gpe::init_core_engine(argc, args,"game_errors.txt" ) == false )
    {
        gpe::error_log->report("Unable to properly initialize gpe_core_engine!\n");
        return -2;
    }

    if( gpe::init_sdl2_all_systems() == false )
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

    //Our preferred order, although some people may do it in their own way
    game_state_credits_base  * game_credits = new game_state_credits_base("game_credits");
    game_state_main_menu_base  * game_main_menu = new game_state_main_menu_base("main_menu");
    game_loader  * game_load_screen = new game_loader("game_loading");
    game_master * game_main_master = new game_master( "gpe_master");
    splash_screen * game_splash_screen = new splash_screen( "game_splash" );
    shape_test_state * shape_tester = new shape_test_state("shape_tester");

    gpe::game_runtime->state_add( game_load_screen );
    gpe::game_runtime->state_add( game_credits );
    gpe::game_runtime->state_add( game_main_menu );
    gpe::game_runtime->state_add( game_main_master );
    gpe::game_runtime->state_add( game_splash_screen );
    gpe::game_runtime->state_add( shape_tester );

    game_splash_screen->set_state_name_next( game_load_screen->get_state_name() );
    game_load_screen->set_state_name_next( game_main_menu->get_state_name() );
    game_main_menu->set_state_name_next( game_credits->get_state_name() );
    game_credits->set_state_name_next( game_main_menu->get_state_name() );
    game_main_master->set_state_name_next( game_splash_screen->get_state_name() );

    gpe::game_runtime->state_set( shape_tester->get_state_name() );


    if( init_gpe_editor( argc, args ) == false )
    {
        gameFailed = -2000;
    }

    gpe::time_keeper->set_fps( 60 );

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
    quit_gpe_editor();
    gpe::error_log->report("Deleting GPE...");
    gpe::quit_engine();

    //We now remove the independent audio engine
    gpe::quit_sdl2_font_system();
    gpe::quit_sdl2_mixer_audio_system();
    gpe::quit_sdl2_render_package();
    gpe::quit_sdl2_window_system();
    gpe::quit_sdl2_main_system();

    gpe::error_log->report("Program Exited with Return Status 0...");
    gpe::quit_core_engine(); //Removes the  remaining pieces of the engine
    return 0;
}
