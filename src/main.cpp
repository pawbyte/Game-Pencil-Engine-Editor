/*
main.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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
SDL 2.0.9 used for this version...
*/
#include "gpe_project_resources.h"

int main( int argc, char* args[] )
{
    int gameFailed = 0;
    //Initialize
    if( GPE_Init(argc, args, "PawByte","GPE_Editor") == false )
    {
        GPE_Report("    Unable to properly initialize GPE!\n");
        gameFailed = 1;
    }
    else
    {
        /*
        For windows:
        C:/Users/YourName/AppData/PawByte/GPE_Editor
        */

        GPE_Report( "Starting Editor..." );
        //Load the resources

        if( guiRCM->load_files()==false )
        {
            GPE_Report("Unable to load rsm files!\n");
            gameFailed=1;
        }
    }

    //MessageBox( 0, "Hello World!", "Greetings", 0 );
    if(gameFailed==1)
    {
        return 1;
    }
    GPE_Editor_Init( argc, args );

    while( GPE_ProgramStateId!=STATE_EXIT )
    {
        gpe->start_loop();
        currentState->process_input();
        currentState->apply_logic();

        //Change state if needed
        change_state();
        /*Redraw variable used to calculate if any input
        changes happened in this frame to reduce rendering in the editor(when enabled)*/
        bool forceRedraw = false;
        if( input->input_received() )
        {
            forceRedraw = true;
        }
        else if( MAIN_EDITOR_SETTINGS!=NULL)
        {
            if( MAIN_EDITOR_SETTINGS->forceFrameRedraw!=NULL && MAIN_EDITOR_SETTINGS->forceFrameRedraw->is_clicked() )
            {
                forceRedraw = true;
            }
        }
        if( forceRedraw )
        {
            MAIN_RENDERER->clear_renderer();
        }
        currentState->render();
        //Update screen
        gpe->end_loop( true, true );
    }

    //Clean up
    GPE_Report("Deleting Editor....");
    GPE_Editor_Quit();
    GPE_Report("Deleting Gui...");
    PAW_GUI_END();
    GPE_Report("Deleting GPE...");
    GPE_Quit();
    GPE_Report("Program Exited with Return Status 0...");
    return 0;
}
