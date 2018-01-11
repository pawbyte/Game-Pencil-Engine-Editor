/*
main.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine <https://create.pawbyte.com>

 ____                ____        _
|  _ \ __ ___      _| __ ) _   _| |_ ___
| |_) / _` \ \ /\ / /  _ \| | | | __/ _ \
|  __/ (_| |\ V  V /| |_) | |_| | ||  __/
|_|   \__,_| \_/\_/ |____/ \__, |\__\___|
                           |___/
Created By PawByte
Attribution required at start of program and in credits.
SDL 2.0.5 used for this version...
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include "gpe_editor.h"
#include "gpe_project_resources.h"
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char* args[] )
{
    record_error("Starting Game Pencil Engine");
    record_error("Version "+double_to_string(GPE_VERSION_DOUBLE_NUMBER));
    record_error("WARNING: DO NOT CLOSE THE CONSOLE WINDOW. UNSAVED FILES AND DATA WILL BE LOST!");
    //Whether or not to cap the frame rate

    //begins making random numbers...
    srand(time(NULL));
    int gameFailed=0;

    if( argc > 0)
    {
        APP_FILE_NAME = args[0];
        APP_DIRECTORY_NAME = get_path_from_file(APP_FILE_NAME);
        APP_DIRECTORY_NAME = string_replace_all(APP_DIRECTORY_NAME,"\\","/");
    }
    record_error("Using ["+get_user_settings_folder()+"] as settings folder...");
    std::string errorLogName = get_user_settings_folder()+"gpe_error_log.txt";
    remove(errorLogName.c_str() );
    std::string buildEerrorLogName = get_user_settings_folder()+"build_errors.txt";
    remove( buildEerrorLogName.c_str() );
    buildEerrorLogName = get_user_settings_folder()+"resources_check.txt";
    remove( buildEerrorLogName.c_str() );
    ARGS_PROCESSED = argc;
    //Initialize
    if( GPE_Init() == false )
    {
        record_error("Unable to properly initialize GPE!\n");
        gameFailed= 1;
    }
    else
    {
        //Load the resources
        if( rsm->load_files()==false )
        {
            record_error("Unable to load rsm files!\n");
            gameFailed=1;
        }

        if( !PAW_GUI_START() )
        {
            record_error("Unable to start Paw_GUI LIBRARY!\n");
            gameFailed=1;
        }

        GPE_MAIN_GUI = new GPE_Gui_Engine();
    }

    //MessageBox( 0, "Hello World!", "Greetings", 0 );
    if(gameFailed==1)
    {
        return 1;
    }
    record_error("Starting GPE Editor...");
    GPE_Editor_Init();
    record_error("WARNING: DO NOT CLOSE THE CONSOLE WINDOW. UNSAVED FILES AND DATA WILL BE LOST!");

    //Sets the 1st Game State;
    GPE_NextProgramState = STATE_NULL;
    GPE_ProgramStateId = STATE_GPE_EDITOR;
    GPE_IS_LOADING = false;
    currentState = new GPE_Editor_State();
    input->handle(true);

    if( argc > 1)
    {
        for( int iArgc = 1; iArgc <argc; iArgc++ )
        {
            GPE_MAIN_GUI->open_project(args[iArgc]);
            record_error(args[iArgc]);
        }
    }
    else if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->launchLastProjectBox!=NULL)
    {
        if( MAIN_EDITOR_SETTINGS->launchLastProjectBox->is_clicked() )
        {
            int iRPList = 0;
            int maxRPList = GPE_MAIN_GUI->get_recent_project_list_size();
            if( maxRPList > 0)
            {
                if( file_exists(GPE_MAIN_GUI->get_recent_project_name(0) ) )
                {
                    GPE_MAIN_GUI->open_project( GPE_MAIN_GUI->get_recent_project_name(0) );
                }
            }
        }
    }
    set_fps( FPS_CAP);
    while( GPE_ProgramStateId!=STATE_EXIT )
    {
        GPE_TIMER_CAP->start();
        //gets user input
        input->handle(true);
        currentState->process_input();
        currentState->apply_logic();
        //Change state if needed
        change_state();

        CURRENT_FPS = calculate_avg_fps();
        if( input->input_received() )
        {
            MAIN_RENDERER->clear_renderer();
        }
        currentState->render();
        //Update screen
        //if( input->input_received() )
        {
            MAIN_RENDERER->update_renderer();
        }

        cap_fps();
        if( input->programJustStarted )
        {
            input->programStartTicks++;
            if( input->programStartTicks > FPS_CAP*10)
            {
                input->programJustStarted = false;
            }
        }
    }

    //Clean up
    record_error("Deleting Editor....");
    GPE_Editor_Quit();
    record_error("Deleting Gui...");
    PAW_GUI_END();
    record_error("Deleting GPE...");
    GPE_Quit();
    record_error("Program Exited with Return Status 0...");
    return 0;
}
