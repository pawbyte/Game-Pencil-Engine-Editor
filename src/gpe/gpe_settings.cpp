/*
gpe_settings.cpp
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

#include "gpe_settings.h"

namespace gpe
{
    engine_settings * settings = NULL;

    engine_settings::engine_settings( std::string app_publisher, std::string appTitle )
    {
        programPublisher = app_publisher;
        programTitle = appTitle;

        defaultWindowWidth = 640;
        defaultWindowHeight = 360;

        minWindowWidth = 320;
        minWindowHeight = 180;

        mainWindowIsResizable = true;
        otherWindowsAreResizable = true;

        startMaximized = false;
        startFullScreen = false;
        showWindowBorder = true;

        startScene = "default";
        startScene = "default.txt";
        useUncappedFPS = false;
        defaultFPS = 60;
        showFPS = false;
        showFPSX = 32;
        showFPSY = 32;
        settingsLeadPair = new key_pair( -1, "gpe_settings" );
        gameResourcesInstructions = new gpe::key_pair(-1, "gameResourcesInstructions", "gameResourcesInstructions", -1 );

        animationsInstructions = gameResourcesInstructions->add_keypair("animations","animations", -1 );
        audioInstructions = gameResourcesInstructions->add_keypair("audio","audio", -1 );
        fontsInstructions = gameResourcesInstructions->add_keypair("fonts","fonts", -1 );
        lightsInstructions = gameResourcesInstructions->add_keypair("lights","lights", -1 );
        particlesInstructions = gameResourcesInstructions->add_keypair("particles","particles", -1 );
        pathsInstructions = gameResourcesInstructions->add_keypair("paths","paths", -1 );
        texturesInstructions = gameResourcesInstructions->add_keypair("textures","textures", -1 );
        tilesheetInstructions = gameResourcesInstructions->add_keypair("tilesheets","tilesheets", -1 );
    }

    engine_settings::~engine_settings()
    {
        if( settingsLeadPair !=NULL )
        {
            delete settingsLeadPair;
            settingsLeadPair = NULL;
        }
    }

    float engine_settings::get_setting_float( std::string sName )
    {
        gpe::key_pair * fPair = settingsLeadPair->find_option_named( sName );

        if( fPair != NULL )
        {
            return fPair->keyValue;
        }
        return -1;
    }

    int engine_settings::get_setting_int( std::string sName )
    {
        gpe::key_pair * fPair = settingsLeadPair->find_option_named( sName );

        if( fPair != NULL )
        {
            return fPair->keyValue;
        }
        return -1;
    }

    std::string engine_settings::get_setting_string( std::string sName )
    {
        gpe::key_pair * fPair = settingsLeadPair->find_option_named( sName );

        if( fPair != NULL )
        {
            return fPair->keySubString;
        }
        return "";

    }

    void engine_settings::set_setting_float( std::string sName , float sVal )
    {
        gpe::key_pair * fPair = settingsLeadPair->find_option_named( sName );

        if( fPair != NULL )
        {
            fPair->keySubString = stg_ex::float_to_string( sVal );
            fPair->keyValue = sVal;
        }
        else
        {
            fPair->add_keypair( sName, stg_ex::float_to_string( sVal) , sVal );
        }

    }

    void engine_settings::set_setting_int( std::string sName , int sVal )
    {
        gpe::key_pair * fPair = settingsLeadPair->find_option_named( sName );

        if( fPair != NULL )
        {
            fPair->keySubString = stg_ex::int_to_string( sVal );
            fPair->keyValue = sVal;
        }
        else
        {
            fPair->add_keypair( sName, stg_ex::int_to_string( sVal) , sVal );
        }

    }

    void engine_settings::set_setting_string( std::string sName , std::string sVal )
    {
        gpe::key_pair * fPair = settingsLeadPair->find_option_named( sName );

        if( fPair != NULL )
        {
            fPair->keySubString = sVal;
            fPair->keyValue = stg_ex::string_to_float( sVal, 0 );
        }
        else
        {
            fPair->add_keypair( sName, sVal, stg_ex::string_to_float( sVal, 0 ) );
        }
    }

    bool init_settings(int argc, char* args[], std::string app_publisher, std::string app_name, std::string error_log_location  )
    {
        if( error_log == NULL )
        {
            error_log = new error_log_system( error_log_location );
        }

        if( settings != NULL )
        {
            return false;
        }
        settings = new engine_settings( app_publisher, app_name );
        return true;
    }

    void quit_settings()
    {
        if( settings!=NULL )
        {
            delete settings;
            settings = NULL;
        }
    }
}
