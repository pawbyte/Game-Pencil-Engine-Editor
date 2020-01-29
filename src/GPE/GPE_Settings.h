/*
GPE_Settings.h
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

#ifndef GPE_SETTINGS_H
#define GPE_SETTINGS_H

#include "GPE_String_Ex.h"
#include "GPE_Parser.h"

class GPE_Settings
{
    private:
        GPE_KeyPair * settingsLeadPair;
    public:
        std::string programPublisher;
        std::string programTitle;
        //Used by the GPE_Game_load_State object to load all of the objects from the start of application
        GPE_KeyPair * gameResourcesInstructions;

        int defaultWindowWidth, defaultWindowHeight;
        int minWindowWidth, minWindowHeight;

        bool mainWindowIsResizable;
        bool otherWindowsAreResizable;
        bool startMaximized;
        bool startFullScreen;
        bool showWindowBorder;
        std::string startScene;
        std::string startSceneFile;
        bool useUncappedFPS;
        int defaultFPS;
        bool showFPS;
        int showFPSX, showFPSY;
        GPE_Settings( std::string appPublisher, std::string appTitle );
        ~GPE_Settings();
        float get_setting_float( std::string sName );
        int get_setting_int( std::string sName );
        std::string get_setting_string( std::string sName );
        void set_setting_float( std::string sName , float sVal );
        void set_setting_int( std::string sName , int sVal );
        void set_setting_string( std::string sName , std::string sVal );
};

extern GPE_Settings * GPE_ENGINE_SETTINGS;

bool GPE_Init_Settings( int argc, char* args[], std::string appPublisher, std::string appTitle );
void GPE_Quit_Settings();

#endif //GPE_SETTINGS_H
