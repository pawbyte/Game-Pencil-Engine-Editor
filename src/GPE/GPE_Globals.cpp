/*
GPE_Globals.cpp
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

//The headers

#include "GPE_Globals.h"

//The event structure
SDL_Event event;
int ARGS_PROCESSED = 0;
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 480;

bool IN_DEBUG_MODE = false;
bool SHOW_FPS_COUNTER = false;

//The dimensions of the level
int LAST_USED_HALIGNMENT = 0;
int LAST_USED_VALIGNMENT = 0;

std::string APP_FILE_NAME = "";
std::string APP_DIRECTORY_NAME = "";
std::string GPE_APP_SETTINGS_FOLDER = "";
int GPE_FOUND_OS = GPE_Gain_OS();

int objectTypes =14;

std::string LANGUAGE = "American";
std::string PATH_CONFIG;
std::string FILE_KEYBINDINGS;
int SAVE_SLOT_NUMBER = 0;

bool AUDIO_WORKS = true;
int AUDIO_VOLUME = 255;

bool USE_COLORBLINDMODE = false;
bool GLOBAL_WIN_MAXIMIZED = true;
bool GLOBAL_WIN_SHOW_BORDER = true;
bool GLOBAL_WIN_RESIZABLE = true;
bool GLOBAL_WIN_FULL_SCREEN = false;
bool GLOBAL_WIN_NULL_DECLARE = true;

GPE_Window * GPE_MAIN_WINDOW = NULL;
GPE_Renderer_Base * GPE_MAIN_RENDERER = NULL;
