/*
GPE_Globals.h
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


*/

#ifndef GLOBALS_H
#define GLOBALS_H

//The header
#include "GPE_CIncludes.h"
#include "GPE_Color_Manager.h"
#include "GPE_Renderer.h"
#include <string>
//#include "functions.h"

extern int ARGS_PROCESSED;
//The event structure
extern SDL_Event event;
//The camera
extern SDL_Rect camera;

//The dimensions of the level
extern int LEVEL_WIDTH;
extern int LEVEL_HEIGHT;

extern int LEVEL_CURRENT_ID;
extern std::string LEVEL_CURRENT_NAME;
//The dimensions of the level
extern int LEVEL_TILE_WIDTH;
extern int LEVEL_TILE_HEIGHT;

//The dimenstions of the screen
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

//Keep track of the frame count
extern double fps;
extern float TIMER_TICKS;
extern float OLD_TIMER_TICKS;
extern int room_speed;

extern bool IN_DEBUG_MODE;
//extern int mouse_x;
//extern int mouse_y;
//Actual Engine global variables
extern int deathInsuranceProvider;


extern std::string currentCommand;
extern int commandLength;

extern std::string APP_FILE_NAME;
extern std::string APP_DIRECTORY_NAME;
extern int GPE_FOUND_OS;

extern int LAST_USED_HALIGNMENT;
extern int LAST_USED_VALIGNMENT;
extern std::string programTitle;
extern std::string LANGUAGE;
extern std::string PATH_CONFIG;
extern std::string FILE_KEYBINDINGS;
extern int SAVE_SLOT_NUMBER;

//for AUDIO Properties
extern bool AUDIO_WORKS;
extern int AUDIO_VOLUME;

extern bool USE_COLORBLINDMODE;
extern bool GLOBAL_WIN_MAXIMIZED;
extern bool GLOBAL_WIN_SHOW_BORDER;
extern bool GLOBAL_WIN_RESIZABLE;
extern bool GLOBAL_WIN_FULL_SCREEN;
extern bool GLOBAL_WIN_NULL_DECLARE;

extern bool RENDER_RESOURCEBAR_LEFT;
extern GPE_Renderer * MAIN_RENDERER;
void reset_camera();

void updateCommandBox(std::string strIn);

class GPE_IDE_Audio
{
    public:
        bool playGPEAudio;
        int gpeAudioVolume;
        Mix_Chunk * startUpAudio;
        GPE_IDE_Audio();
        ~GPE_IDE_Audio();
        void load_audio_settings(std::string settingsFileName);
        void save_audio_settings(std::string settingsFileName);
};

extern GPE_IDE_Audio * GPE_Audio;

//Initialization
//bool init_GL();
//bool init();

#endif
