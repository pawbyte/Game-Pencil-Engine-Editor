/*
GPE_Globals.h
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


*/

#ifndef GLOBALS_H
#define GLOBALS_H

//The header
#include "GPE_CIncludes.h"
#include "GPE_Color_Manager.h"
#include "GPE_Renderer.h"
#include <string>
//#include "functions.h"

//Start Cursor stuff
extern SDL_Cursor * GPE_Cursor;
extern SDL_SystemCursor GPE_PreviousCursor;
extern SDL_SystemCursor GPE_CurrentCursor;
extern SDL_Cursor * GPE_LoadingCursor;
extern SDL_Cursor * GPE_HoverCursor;
//End Cursor stuff

extern int ARGS_PROCESSED;
//The event structure
extern SDL_Event event;

//The dimenstions of the screen
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

//Keep track of the frame count
extern bool SHOW_FPS_COUNTER;

extern bool IN_DEBUG_MODE;
//extern int mouse_x;
//extern int mouse_y;
//Actual Engine global variables

extern std::string currentCommand;
extern int commandLength;

extern std::string APP_FILE_NAME;
extern std::string APP_DIRECTORY_NAME;
extern std::string GPE_APP_SETTINGS_FOLDER;
extern int GPE_FOUND_OS;

extern int LAST_USED_HALIGNMENT;
extern int LAST_USED_VALIGNMENT;
extern std::string GPE_PROGRAM_PUBLISHER;
extern std::string GPE_PROGRAM_TITLE;
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

extern GPE_Renderer * MAIN_RENDERER;
extern GPE_Renderer * CURRENT_RENDERER;

void updateCommandBox(std::string strIn);


//Initialization
//bool init_GL();
//bool init();

#endif
