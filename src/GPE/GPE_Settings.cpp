/*
GPE_Settings.cpp
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

#include "GPE_Settings.h"

GPE_Settings * GPE_ENGINE_SETTINGS = NULL;

GPE_Settings::GPE_Settings( std::string appPublisher, std::string appTitle )
{
    programPublisher = appPublisher;
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
    settingsLeadPair = new GPE_KeyPair( -1, "gpe_settings" );
    gameResourcesInstructions = new GPE_KeyPair(-1, "gameResourcesInstructions", "gameResourcesInstructions", -1 );
}

GPE_Settings::~GPE_Settings()
{
    if( settingsLeadPair !=NULL )
    {
        delete settingsLeadPair;
        settingsLeadPair = NULL;
    }
}

float GPE_Settings::get_setting_float( std::string sName )
{
    GPE_KeyPair * fPair = settingsLeadPair->find_option_named( sName );

    if( fPair != NULL )
    {
        return fPair->keyValue;
    }
    return -1;
}

int GPE_Settings::get_setting_int( std::string sName )
{
    GPE_KeyPair * fPair = settingsLeadPair->find_option_named( sName );

    if( fPair != NULL )
    {
        return fPair->keyValue;
    }
    return -1;
}

std::string GPE_Settings::get_setting_string( std::string sName )
{
    GPE_KeyPair * fPair = settingsLeadPair->find_option_named( sName );

    if( fPair != NULL )
    {
        return fPair->keySubString;
    }
    return "";

}

void GPE_Settings::set_setting_float( std::string sName , float sVal )
{
    GPE_KeyPair * fPair = settingsLeadPair->find_option_named( sName );

    if( fPair != NULL )
    {
        fPair->keySubString = float_to_string( sVal );
        fPair->keyValue = sVal;
    }
    else
    {
        fPair->add_keypair( sName, float_to_string( sVal) , sVal );
    }

}

void GPE_Settings::set_setting_int( std::string sName , int sVal )
{
    GPE_KeyPair * fPair = settingsLeadPair->find_option_named( sName );

    if( fPair != NULL )
    {
        fPair->keySubString = int_to_string( sVal );
        fPair->keyValue = sVal;
    }
    else
    {
        fPair->add_keypair( sName, int_to_string( sVal) , sVal );
    }

}

void GPE_Settings::set_setting_string( std::string sName , std::string sVal )
{
    GPE_KeyPair * fPair = settingsLeadPair->find_option_named( sName );

    if( fPair != NULL )
    {
        fPair->keySubString = sVal;
        fPair->keyValue = string_to_float( sVal, 0 );
    }
    else
    {
        fPair->add_keypair( sName, sVal, string_to_float( sVal, 0 ) );
    }
}

bool GPE_Init_Settings(int argc, char* args[], std::string appPublisher, std::string appName  )
{
    if( GPE_ENGINE_SETTINGS != NULL )
    {
        return false;
    }
    remove("gpe_error_log.txt");
    GPE_ENGINE_SETTINGS = new GPE_Settings( appPublisher, appName );
    return true;
}

void GPE_Quit_Settings()
{
    if( GPE_ENGINE_SETTINGS!=NULL )
    {
        delete GPE_ENGINE_SETTINGS;
        GPE_ENGINE_SETTINGS = NULL;
    }
}
