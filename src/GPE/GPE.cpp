/*
GPE.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#include "GPE.h"
#include "GPE_Math.h"

#include "GPE_Artist_SDL.h"
#include "GPE_Renderer_SDL.h"
#include "GPE_Texture_SDL.h"

bool GPE_Init( int argc, char* args[] )
{
    remove("gpe_error_log.txt");

    if( GPE_ENGINE_SETTINGS == NULL )
    {
        return false;
    }

    if( argc > 0)
    {
        APP_FILE_NAME = args[0];
        APP_DIRECTORY_NAME = get_path_from_file( APP_FILE_NAME );
        APP_DIRECTORY_NAME = string_replace_all( APP_DIRECTORY_NAME,"\\","/");
    }
    ARGS_PROCESSED = argc;

    //begins making random numbers...
    srand( time(NULL) );
    for(int rI = 0; rI < RESOURCE_TYPE_MAX; rI++)
    {
        RESOURCE_TYPE_NAMES[rI] = "";
    }

    //Sets the folder used in all get_user_settings_folder() calls based on the 2 paramers above
    GPE_Seek_Settings_Folder();

    //General Debug Info
    //GPE_Report("    WARNING: DO NOT CLOSE THE CONSOLE WINDOW. UNSAVED FILES AND DATA WILL BE LOST!");
    //Clears the error logs and such

    std::string errorLogName = get_user_settings_folder()+"gpe_error_log.txt";
    remove(errorLogName.c_str() );
    std::string buildEerrorLogName = get_user_settings_folder()+"build_errors.txt";
    remove( buildEerrorLogName.c_str() );
    buildEerrorLogName = get_user_settings_folder()+"resources_check.txt";
    remove( buildEerrorLogName.c_str() );

    GPE_Report("Attempting to initialize GPE-Engine( Version " + float_to_string(GPE_VERSION_FLOAT_NUMBER) + ")");
    GPE_Report("Program Publisher:"+ GPE_ENGINE_SETTINGS->programPublisher );
    GPE_Report("Program Title:"+GPE_ENGINE_SETTINGS->programTitle );
    GPE_Report("Settings Folder: "+get_user_settings_folder() );
    //Initialize all SDL subsystems

    GPE_Report("-Loading in dependencies...");
    GPE_Report("--SDL2...");
    if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
        GPE_Report("---Error initializing SDL!");
        return false;
    }

    GPE_Report("--Attempting to initialize SDL_INIT_TIMER");
    if( SDL_InitSubSystem( SDL_INIT_TIMER ) == -1 )
    {
        GPE_Report("---Error initializing SDL_INIT_TIMER");
        return false;
    }

    GPE_Report("--Attempting to initialize SDL_INIT_EVENTS");
    if( SDL_InitSubSystem( SDL_INIT_EVENTS ) == -1 )
    {
        GPE_Report("---Error initializing SDL_INIT_EVENTS");
        return false;
    }


    GPE_Report("--Attempting to initialize SDL_INIT_GAMECONTROLLER");
    if( SDL_InitSubSystem( SDL_INIT_GAMECONTROLLER ) == -1 )
    {
        GPE_Report("---Error initializing SDL_INIT_GAMECONTROLLER");
        return false;
    }

    GPE_Report("--Attempting to initialize SDL_INIT_HAPTIC");
    if( SDL_InitSubSystem( SDL_INIT_HAPTIC ) == -1 )
    {
        GPE_Report("---Error initializing SDL_INIT_HAPTIC");
        return false;
    }

    GPE_Report("--SDL2 systems started...");
    GPE_Report("-Setting starting cursor...");
    GPE_IS_LOADING = true;

    GPE_CAMERA_DEFAULT.x=0;
    GPE_CAMERA_DEFAULT.y=0;
    GPE_CAMERA_DEFAULT.w=(Uint16)SCREEN_WIDTH;
    GPE_CAMERA_DEFAULT.h=(Uint16)(SCREEN_HEIGHT);

    GPE_CAMERA_CURRENT.x=0;
    GPE_CAMERA_CURRENT.y=0;
    GPE_CAMERA_CURRENT.w=(Uint16)SCREEN_WIDTH;
    GPE_CAMERA_CURRENT.h=(Uint16)(SCREEN_HEIGHT);


    GPE_Report("-Setting event system..");
    SDL_EventState(SDL_DROPFILE,SDL_ENABLE);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    /*
    if(SDL_Init(SDL_INIT_AUDIO))
    {
        SDL_putenv("SDL_AUDIODRIVER=dummy");
        SDL_Init(SDL_INIT_AUDIO);
    }
    */

    gpeph = new GPE_Render_Package_Handler();
    GPE_Render_Package * defaultRenderPackage = gpeph->add_render_package( "sdl" );


    GPE_MAIN_WINDOW = new GPE_Window( GPE_ENGINE_SETTINGS->programTitle,GPE_ENGINE_SETTINGS->defaultWindowWidth, GPE_ENGINE_SETTINGS->defaultWindowHeight,  GPE_ENGINE_SETTINGS->showWindowBorder, GPE_ENGINE_SETTINGS->startFullScreen, GPE_ENGINE_SETTINGS->startMaximized, GPE_ENGINE_SETTINGS->mainWindowIsResizable );
    GPE_MAIN_RENDERER = GPE_MAIN_RENDERER_SDL = new GPE_Renderer_SDL( GPE_MAIN_WINDOW->get_window_id(), GPE_ENGINE_SETTINGS->defaultWindowWidth, GPE_ENGINE_SETTINGS->defaultWindowHeight );
    GPE_MAIN_RENDERER_SDL->init_renderer_sdl( GPE_MAIN_WINDOW->get_sdl_window() );
    GPE_MAIN_WINDOW->set_renderer( GPE_MAIN_RENDERER_SDL, true );
    GPE_MAIN_RENDERER = GPE_MAIN_RENDERER_SDL;

    defaultRenderPackage->packageRenderer = GPE_MAIN_RENDERER_SDL;
    defaultRenderPackage->packageTexture = new GPE_Texture_SDL();
    defaultRenderPackage->packageWindow = GPE_MAIN_WINDOW;
    gpeph->defaultRenderPackageName = defaultRenderPackage->get_package_name();


    SCREEN_WIDTH = GPE_MAIN_WINDOW->get_window_width();
    SCREEN_HEIGHT = GPE_MAIN_WINDOW->get_window_height();

    mainTranslator = new GPE_Translator("en");
    input = new GPE_InputManager();


    GPE_Report("-GPE_Color_System..");
    GPE_Init_Colors();
    GPE_Report("-GPE_Renderer.");
    SDL_ShowCursor(SDL_ENABLE);


    GPE_Report("-GPE_Img_System...");
    //loads support for JPG and other image formats.
    int flags=IMG_INIT_JPG|IMG_INIT_PNG |IMG_INIT_TIF;
    if( (IMG_Init(flags)&flags) != flags)
    {
        GPE_Report("--IMG_Init: Failed to init required jpg and png support!\n");
        std::string imgError = IMG_GetError();
        GPE_Report("--IMG_Init: " + imgError );
        GPE_Report("--Error initializing SDL_Mixer.");
        GPE_Report("--Error initializing SDL_Mixer.");
        return false;
        // handle error
    }

    GPE_Report("-GPE_Audio_System...");
    // load support for the OGG and MOD sample/music formats
    flags=MIX_INIT_MP3 | MIX_INIT_MOD | MIX_INIT_FLAC | MIX_INIT_OGG ;
    if ((Mix_Init(flags)&flags) != flags)
    {
        GPE_Report("--Error loading SDL_Mixer");
        std::string mixError = Mix_GetError();
        GPE_Report("--Mix_Init: " + mixError );
        //return false;
        // handle error
    }
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024  ) == -1 )
    {
        GPE_Report("-- Error initializing SDL_Mixer.");
        AUDIO_WORKS = false;
    }
    GPE_Report("-Dependencies loaded...");
    GPE_Report("-Starting run-time...");

    //Creates the SDL package renderer

    gpe = new GPE_Runtime();
    gcm = rsm = new GPE_DataManager(gpeph->get_default_render_package(), "gcm-rsm");

    gpe->cursor_change( gpe->cursor_system_name( GPE_CURSOR_WAIT ) );
    GPE_Report("-Starting GPE Artist...");

    gcanvas = new GPE_Artist_SDL( defaultRenderPackage->packageRenderer );

    //Start counting frames per second
    GPE_MAIN_RENDERER->clear_renderer( true );
    GPE_Report("-Run-time started ");
    gpe->end_loop();
    GPE_Report("-1st loop ended ");

    if( GPE_Font_Init() == false)
    {
        GPE_Report("-- Error initializing GPE_Fonts.");
        return false;
    }
    GPE_GameLoader * gpeLoader = new GPE_GameLoader( "GPE_Loader");
    gpe->state_add( gpeLoader );
    gpe->state_set( gpeLoader->get_state_name() );

    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_ANIMATION] = "Animation";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TEXTURE] = "Texture";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TILESHEET] = "Tilesheet";

    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_AUDIO] = "Audio";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_VIDEO] = "Video";

    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_FUNCTION] = "Function";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_CLASS] = "Class";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT] = "Object";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SCENE] = "Scene";
    //RESOURCE_TYPE_NAMES[RESOURCE_TYPE_ACHIEVEMENT] = "Achievement";

    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_PATH] = "Path";
    //RESOURCE_TYPE_NAMES[RESOURCE_TYPE_DICTIONARY] = "Dictionary";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_FONT] = "Font";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_EMITTER] = "Particle";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_LIGHT] = "Light";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_PLUGIN] = "Plugin";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_EVENT] = "Event";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_QUEST] = "Quest";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_PROJECT_SETTINGS] = "Project Properties";

    gpe->set_fps( FPS_CAP );
    gpe->cursor_change( gpe->cursor_system_name( GPE_CURSOR_ARROW) );

    return true;
}


bool GPE_Quit( )
{
    //Deletes in the order of dependencies from top/down
    GPE_Report("Deleting resource manager....");
    if( rsm!=NULL)
    {
        rsm->clean_up();
        delete rsm;
        rsm = NULL;
    }
    GPE_Report("Deleting gpe....");
    if( gpe!=NULL)
    {
        delete gpe;
        gpe = NULL;
    }

    GPE_Report("Deleting input object...");
    if( input!=NULL)
    {
        delete input;
        input = NULL;
    }

    GPE_Report("Quitting SDL_IMG....");
    IMG_Quit();

    GPE_Report("Quitting SDL_Mixer....");
    Mix_Quit();

    GPE_Report("Quitting SDL....");
    SDL_Quit();

    return true;
}

