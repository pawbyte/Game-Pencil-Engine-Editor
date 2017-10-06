/*
main.cpp
This file is part of:
GAME PENCI ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

#include "GPE_CIncludes.h"
#include "GPE_Color_Manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include "sprite.h"
#include "stateManager.h"
#include "functions.h"
#include "color_master.h"
#include "GPE_Globals.h"
#include "resources_controller.h"
#include "timer.h"
#include "shared_resources.h"
#include "gameobjectresource.h"
#include "pathresource.h"
#include "dictionaryresource.h"
#include "project_browser.h"
#include "gpe_editor.h"
#include <stdio.h>
#include <stdlib.h>

bool init()
{
    //Initialize all SDL subsystems
    record_error("Attempting to initialize SDL");
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        record_error("Error initializing SDL");
        return false;
    }
    record_error("Setting starting cursor...");
    SDL_SetCursor(SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_WAIT) );
    GPE_IS_LOADING = true;
    camera.x=0;
    camera.y=0;
    camera.w=(Uint16)SCREEN_WIDTH;
    camera.h=(Uint16)(SCREEN_HEIGHT);
    record_error("Setting event system..");
    SDL_EventState(SDL_DROPFILE,SDL_ENABLE);
    /*
    if(SDL_Init(SDL_INIT_AUDIO))
    {
        SDL_putenv("SDL_AUDIODRIVER=dummy");
        SDL_Init(SDL_INIT_AUDIO);
    }
    */
    record_error("Creating colors..");
    activate_colors();
    record_error("Setting up renderer.");
    MAIN_RENDERER = new GPE_Renderer(SCREEN_WIDTH,SCREEN_HEIGHT,true,false,true);
    userInput = new GPE_InputManager();

    rsm = new ResourceController();

    SDL_ShowCursor(SDL_ENABLE);
    //Initialize SDL_ttf
    record_error("Starting Font systems...");
    if( TTF_Init() == -1 )
    {
         record_error("Error loading SDL_TTF");
         return false;
    }
    /*
    //Initialize OpenGL
    if( init_GL() == false )
    {
        return false;
    }*/

    //loads support for JPG and other image formats.
    int flags=IMG_INIT_JPG|IMG_INIT_PNG |IMG_INIT_TIF;
    if( (IMG_Init(flags)&flags) != flags)
    {
        record_error("IMG_Init: Failed to init required jpg and png support!\n");
        record_error("IMG_Init: ", IMG_GetError());
        record_error("Error initializing SDL_Mixer.");
        record_error("Error initializing SDL_Mixer.");
        return false;
        // handle error
    }
    // load support for the OGG and MOD sample/music formats
    flags=MIX_INIT_OGG|MIX_INIT_MP3;
    if ((Mix_Init(flags)&flags) != flags)
    {
        record_error("Error loading SDL_Mixer");
        record_error("Mix_Init: Failed to init required ogg and mod support!\n");
        record_error("Mix_Init: ", Mix_GetError());
        return false;
    // handle error
    }
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024  ) == -1 )
    {
        record_error("Error initializing SDL_Mixer.");
        AUDIO_WORKS = false;
    }
    return true;
}

void clean_up()
{

}

int main( int argc, char* args[] )
{
    record_error("Starting Game Pencil Engine");
    record_error("Version "+double_to_string(GPE_VERSION_DOUBLE_NUMBER));
    record_error("WARNING: DO NOT CLOSE THE CONSOLE WINDOW. UNSAVED FILES AND DATA WILL BE LOST!");
    //Whether or not to cap the frame rate
   // bool cap = true;

    //begins making random numbers...
    srand(time(NULL));
    int gameFailed=0;

    if( argc > 0)
    {
        APP_FILE_NAME = args[0];
        APP_DIRECTORY_NAME = get_path_from_file(APP_FILE_NAME);
        APP_DIRECTORY_NAME = string_replace_all(APP_DIRECTORY_NAME,"\\","/");
    }
    record_error("Starting to win again...");
    record_error("Using ["+get_user_settings_folder()+"] as settings folder...");
    std::string errorLogName = get_user_settings_folder()+"gpe_error_log.txt";
    remove(errorLogName.c_str() );
    std::string buildEerrorLogName = get_user_settings_folder()+"build_errors.txt";
    remove( buildEerrorLogName.c_str() );
    buildEerrorLogName = get_user_settings_folder()+"resources_check.txt";
    remove( buildEerrorLogName.c_str() );
    ARGS_PROCESSED = argc;
    record_error("Beginning init function...");
    //Initialize
    if( init() == false )
    {
        record_error("Unable to properly initialize IDE!\n");
        gameFailed= 1;
    }
    else
    {
        record_error("IDE was properly initialized... \n");
    }

    //Load the resources
    if( rsm->load_files() )
    {
        record_error("IDE properly added all files... \n");
    }
    else
    {
        record_error("Unable to load files!\n");
        gameFailed=1;
    }

    if( load_fonts() )
    {
        record_error("IDE properly added all fonts... \n");
    }
    else
    {
        record_error("Unable to load fonts!\n");
        gameFailed=1;
    }
    if(gameFailed==0)
    {
        record_error("IDE started smoothly.\n");
        record_error("WARNING: DO NOT CLOSE THE CONSOLE WINDOW. UNSAVED FILES AND DATA WILL BE LOST!");
    }
    //MessageBox( 0, "Hello World!", "Greetings", 0 );
    if(gameFailed==1)
    {
        return 1;
    }
    fpsTimer = new Timer();
    capTimer = new Timer();
    GPE_Audio = new GPE_IDE_Audio();
    GPE_Audio->load_audio_settings( get_user_settings_folder()+"gpe_audio_settings.txt" );
    if( GPE_Audio->playGPEAudio && GPE_Audio->gpeAudioVolume > 0)
    {
        Mix_Volume(0,GPE_Audio->gpeAudioVolume );
        //Mix_PlayChannel(-1,GPE_Audio->startUpAudio,0);
    }
    //If everything initialized fine
    SDL_Surface *windowIcon = load_surface_image(APP_DIRECTORY_NAME+"gamepencil_icon_128px.png");
    SDL_SetWindowIcon(MAIN_RENDERER->get_window(), windowIcon);
    GPE_Main_TabManager = new GPE_TabManager();
    GPE_MAIN_GUI = new GPE_Gui_Engine();

    MAIN_EDITOR_SETTINGS = new gamePencilEditorSettingsResource();
    MAIN_EDITOR_SETTINGS->set_global_rid(-1);
    MAIN_EDITOR_SETTINGS->set_name("User Settings");
    MAIN_EDITOR_SETTINGS->preprocess_self();

    MAIN_ABOUT_PAGE = new gamePencilAboutPageResource();
    MAIN_ABOUT_PAGE->set_global_rid(-2);
    MAIN_ABOUT_PAGE->set_name("About...");

    MAIN_HELP_PAGE = new gamePencilHelpPageResource();
    MAIN_HELP_PAGE->set_global_rid(-3);
    MAIN_HELP_PAGE->set_name("Help...");


    MAIN_START_PAGE = new gamePencilStartPageResource();
    MAIN_START_PAGE->set_global_rid(-4);
    MAIN_START_PAGE->set_name("Start Page");

    PROJECT_BROWSER_PAGE = new gamePencilProjectBrowserResource();
    PROJECT_BROWSER_PAGE->set_global_rid(-5);
    PROJECT_BROWSER_PAGE->set_name("Project Browser");
    //GPE_Main_TabManager->add_new_tab(MAIN_START_PAGE);

    GPE_Main_Statusbar = new GPE_StatusBar();
    GPE_Main_Statusbar->set_coords(0,0);
    GPE_Main_Statusbar->set_width(SCREEN_WIDTH);
    GPE_Main_Statusbar->set_height(32);

    GPE_Main_Logs = new GPE_LogManager();
    GPE_Main_Logs->set_coords(0,SCREEN_HEIGHT-96);
    GPE_Main_Logs->set_width(SCREEN_WIDTH);
    GPE_Main_Logs->set_height(64);


    GPE_MINI_COMPILER = new GPE_Gui_Engine_MiniCompiler();
    //Sets the 1st Game State;
    nextProgramState = STATE_NULL;
    programStateId = STATE_INTRO;
    GPE_IS_LOADING = false;
    currentState = new GPE_Editor_State();

    //Start counting frames per second
    fpsTimer->start();

    userInput->handle(true);
    //
    //While the user hasn't quit
    record_error("Starting IDE...");
    if( GPE_VERSION_UPDATE_NUMBER==0)
    {
        GPE_Main_Logs->log_general_line("Starting Game Pencil Engine Version "+float_to_string(GPE_VERSION_DOUBLE_NUMBER)+".0...");
    }
    else
    {
        GPE_Main_Logs->log_general_line("Starting Game Pencil Engine Version "+float_to_string(GPE_VERSION_DOUBLE_NUMBER)+"...");
    }
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
    while( programStateId!=STATE_EXIT )
    {

        capTimer->start();
        //gets user input
        userInput->handle(true);
        currentState->process_input();
        currentState->apply_logic();
        //Change state if needed
        change_state();

        calculate_avg_fps();
        if( userInput->input_received() )
        {
            MAIN_RENDERER->clear_renderer();
        }
        currentState->render();
        //Update screen
        //if( userInput->input_received() )
        {
            MAIN_RENDERER->update_renderer();
        }

        cap_fps();
        if( userInput->programJustStarted )
        {
            userInput->programStartTicks++;
            if( userInput->programStartTicks > FPS_CAP*10)
            {
                userInput->programJustStarted = false;
            }
        }
    }
    record_error("Exiting program....");
    record_error("Closing all tabs....");
    if( GPE_Main_TabManager!=NULL)
    {
        GPE_Main_TabManager->close_all_tabs();
    }
    record_error("Deleting Settings....");
    if( MAIN_EDITOR_SETTINGS!=NULL)
    {
        MAIN_EDITOR_SETTINGS->save_resource();
        delete MAIN_EDITOR_SETTINGS;
        MAIN_EDITOR_SETTINGS = NULL;
    }
    record_error("Deleting about page....");
    if( MAIN_ABOUT_PAGE!=NULL)
    {
        delete MAIN_ABOUT_PAGE;
        MAIN_ABOUT_PAGE = NULL;
    }
    record_error("Deleting help page....");
    if( MAIN_HELP_PAGE!=NULL)
    {
        delete MAIN_HELP_PAGE;
        MAIN_HELP_PAGE = NULL;
    }
    record_error("Deleting start page....");
    if( MAIN_START_PAGE!=NULL)
    {
        delete MAIN_START_PAGE;
        MAIN_START_PAGE = NULL;
    }
    record_error("Deleting context menu....");
    if( MAIN_CONTEXT_MENU!=NULL)
    {
        delete MAIN_CONTEXT_MENU;
        MAIN_CONTEXT_MENU = NULL;
    }
    record_error("Deleting toolbar recent projects....");
    if( MAIN_TOOLBAR_RECENT_PROJECTS!=NULL)
    {
        delete MAIN_TOOLBAR_RECENT_PROJECTS;
        MAIN_TOOLBAR_RECENT_PROJECTS = NULL;
    }
    record_error("Deleting mini-compiler....");
    if( GPE_MINI_COMPILER!=NULL)
    {
        delete GPE_MINI_COMPILER;
        GPE_MINI_COMPILER = NULL;
    }
    record_error("Deleting gui....");
    if( GPE_MAIN_GUI!=NULL)
    {
        GPE_MAIN_GUI->save_settings();
        delete GPE_MAIN_GUI;
        GPE_MAIN_GUI = NULL;
    }
    record_error("Deleting audio....");
    /*if( GPE_Audio!=NULL)
    {
        if( GPE_Audio->playGPEAudio && GPE_Audio->gpeAudioVolume > 0)
        {
            Mix_Volume(0,GPE_Audio->gpeAudioVolume );
            Mix_PlayChannel(-1,GPE_Audio->startUpAudio,0);
            SDL_Delay( 3000 );
        }
        delete GPE_Audio;
        GPE_Audio = NULL;
    }*/
    record_error("Deleting resource manager....");
    //Clean up
    if( rsm!=NULL)
    {
        rsm->clean_up();
        clean_up();
        delete rsm;
        rsm = NULL;
    }
    record_error("Quitting SDL_Mixer....");
    Mix_Quit();
    record_error("Deleting Window Icon....");
    if( windowIcon!=NULL)
    {
        SDL_FreeSurface(windowIcon);
        windowIcon = NULL;
    }

    record_error("Program Exited with Return Status 0...");
    return 0;
}
