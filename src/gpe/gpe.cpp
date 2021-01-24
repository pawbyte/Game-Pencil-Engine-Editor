/*
gpe.cpp
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

#include "gpe.h"

namespace gpe
{
    bool init_core_engine( int argc, char* args[] , std::string error_log_location )
    {
        if( error_log == NULL )
        {
            error_log = new error_log_system( error_log_location );
        }

        if( settings == NULL )
        {
            return false;
        }

        if( argc > 0)
        {
            app_file_name = args[0];
            app_directory_name = stg_ex::get_path_from_file( app_file_name );
            app_directory_name = stg_ex::string_replace_all( app_directory_name,"\\","/");
        }
        args_processed = argc;

        //begins making random numbers...
        srand( time(NULL) );

        for(int rI = 0; rI < resource_type_max; rI++)
        {
            resource_type_names[rI] = "";
            resource_type_names_plural[rI] = "";
        }

        resource_type_names[resource_type_animation] = "Animation";
        resource_type_names[resource_type_texture] = "Texture";
        resource_type_names[resource_type_tilesheet] = "Tilesheet";

        resource_type_names[resource_type_audio] = "Audio";
        resource_type_names[resource_type_video] = "Video";

        resource_type_names[resource_type_function] = "Function";
        resource_type_names[resource_type_class] = "Class";
        resource_type_names[resource_type_object] = "Entity";
        resource_type_names[resource_type_scene] = "Scene";
        resource_type_names[resource_type_achievement] = "Achievement";

        resource_type_names[resource_type_path] = "Path";
        resource_type_names[resource_type_dictionary] = "Dictionary";
        resource_type_names[resource_type_font] = "Font";
        resource_type_names[resource_type_emitter] = "Particle";
        resource_type_names[resource_type_light] = "Light";
        resource_type_names[resource_type_plugin] = "Plugin";
        resource_type_names[resource_type_event] = "Event";
        resource_type_names[resource_type_quest] = "Quest";
        resource_type_names[resource_type_project_settings] = "Project Properties";

        //Plural names
        resource_type_names_plural[resource_type_animation] = "Animations";
        resource_type_names_plural[resource_type_texture] = "Textures";
        resource_type_names_plural[resource_type_tilesheet] = "Tilesheets";

        resource_type_names_plural[resource_type_audio] = "Audios";
        resource_type_names_plural[resource_type_video] = "Videos";

        resource_type_names_plural[resource_type_function] = "Functions";
        resource_type_names_plural[resource_type_class] = "Classes";
        resource_type_names_plural[resource_type_object] = "Entities";
        resource_type_names_plural[resource_type_scene] = "Scenes";
        resource_type_names_plural[resource_type_achievement] = "Achievements";

        resource_type_names_plural[resource_type_path] = "Paths";
        resource_type_names_plural[resource_type_dictionary] = "Dictionaries";
        resource_type_names_plural[resource_type_font] = "Fonts";
        resource_type_names_plural[resource_type_emitter] = "Particles";
        resource_type_names_plural[resource_type_light] = "Lights";
        resource_type_names_plural[resource_type_plugin] = "Plugins";
        resource_type_names_plural[resource_type_event] = "Events";
        resource_type_names_plural[resource_type_quest] = "Quests";
        resource_type_names_plural[resource_type_project_settings] = "Project Properties";

        for( int i_sound_format = 0; i_sound_format < sound_format_max; i_sound_format++ )
        {
            sound_is_format_supported[ i_sound_format ] = false;
            sound_type_names[ i_sound_format ] = "";
        }
        sound_type_names[ sound_format_acc ] = "acc";
        sound_type_names[ sound_format_mod ] = "mod";
        sound_type_names[ sound_format_mp3 ] = "mp3";
        sound_type_names[ sound_format_ogg ] = "ogg";
        sound_type_names[ sound_format_wav ] = "wav";

        //Sets the folder used in all  get_user_settings_folder() calls based on the 2 paramers above
        seek_settings_folder();

        //General Debug Info
        //error_log->report("    WARNING: DO NOT CLOSE THE CONSOLE WINDOW. UNSAVED FILES AND DATA WILL BE LOST!");
        //Clears the error logs and such

        std::string errorLogName =  get_user_settings_folder()+"gpe_error_log.txt";
        remove(errorLogName.c_str() );
        std::string buildEerrorLogName =  get_user_settings_folder()+"build_errors.txt";
        remove( buildEerrorLogName.c_str() );
        buildEerrorLogName =  get_user_settings_folder()+"resources_check.txt";
        remove( buildEerrorLogName.c_str() );

        error_log->report("Attempting to initialize GPE-Engine( Version " + stg_ex::float_to_string( gpe::version_number_total ) + ")");
        error_log->report("Program Publisher:"+ settings->programPublisher );
        error_log->report("Program Title:"+settings->programTitle );
        error_log->report("Settings Folder: "+ get_user_settings_folder() );

        error_log->report("Initializing Color_System..");
        init_colors();

        camera_default = new shape_rect(0,0,screen_width, screen_height);
        camera_current = new shape_rect( 0,0,screen_width, screen_height );

        renderer_main = new renderer_base();
        gcanvas = new artist_base( renderer_main );
        rph = new render_package_handler();

        input = new input_manager_base();
        time_keeper = new time_keeper_base();

        if( init_font_system() == false)
        {
            error_log->report("-- Error initializing fonts.");
            return false;
        }

        rsm = new asset_manager(rph->get_default_render_package(), "gcm-rsm");

        cursor_main_controller = new cursor_controller_base();
        cursor_main_controller->cursor_change( cursor_main_controller->cursor_system_name( cursor_default_type::wait ) );

        window_controller_main = new window_controller_base();
        game_runtime = new runtime_master();
        game_runtime->loading_data = true;
        return true;
    }

    //inits the run-time and game-states
    bool init_engine()
    {
        error_log->report("-Run-time started ");
        //game_runtime->end_loop();
        //error_log->report("-1st loop ended ");

        time_keeper->set_fps( fps_cap );
        cursor_main_controller->cursor_change( cursor_main_controller->cursor_system_name( cursor_default_type::arrow) );
        return true;
    }

    bool quit_engine( )
    {
        //Deletes in the order of dependencies from top/down
        error_log->report("Deleting resource manager....");
        if( rsm!=NULL)
        {
            rsm->clean_up();
            delete rsm;
            rsm = NULL;
        }
        error_log->report("Deleting game_runtime....");
        if( game_runtime!=NULL)
        {
            delete game_runtime;
            game_runtime = NULL;
        }

        error_log->report("Deleting input object...");
        if( input!=NULL)
        {
            delete input;
            input = NULL;
        }
        return true;
    }

    bool quit_core_engine()
    {
        if( error_log!=NULL )
        {
            delete error_log;
            error_log = NULL;
        }
        return true;
    }
}
