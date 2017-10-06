/*
GPE_Globals.cpp
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

//The headers

#include "GPE_Globals.h"
//The event structure
SDL_Event event;
int ARGS_PROCESSED = 0;
int SCREEN_WIDTH =800;
int SCREEN_HEIGHT =480;

//ends the inventory related globals

bool IN_DEBUG_MODE = true;
float TIMER_TICKS = 0;
float OLD_TIMER_TICKS=0;
//int mouse_x=0;
//int mouse_y=0;
//Actual Engine global variables
int deathInsuranceProvider=0;

SDL_Rect camera;

//The dimensions of the level

int LAST_USED_HALIGNMENT = 0;
int LAST_USED_VALIGNMENT = 0;

bool gameIsLoading = false;


std::string currentCommand = "";
int commandLength = 0;

std::string APP_FILE_NAME;
std::string APP_DIRECTORY_NAME;
int GPE_FOUND_OS = GPE_Gain_OS();


int objectTypes =14;
std::string programTitle = "Game Pencil";
std::string LANGUAGE = "American";
std::string PATH_CONFIG;
std::string FILE_KEYBINDINGS;
int SAVE_SLOT_NUMBER = 0;

bool AUDIO_WORKS = true;
int AUDIO_VOLUME = 255;

float moneyTotal=120;
float minMoney = -999999;
float maxMoney = 99999999999999;
bool USE_COLORBLINDMODE = false;
bool GLOBAL_WIN_MAXIMIZED = true;
bool GLOBAL_WIN_SHOW_BORDER = true;
bool GLOBAL_WIN_RESIZABLE = true;
bool GLOBAL_WIN_FULL_SCREEN = false;
bool GLOBAL_WIN_NULL_DECLARE = true;
GPE_Renderer * MAIN_RENDERER = NULL;

bool RENDER_RESOURCEBAR_LEFT = false;
void reset_camera()
{
    camera.x = 0;
    camera.y = 0;
    camera.w = SCREEN_WIDTH;
    camera.h = SCREEN_HEIGHT;
}


void updateCommandBox(std::string strIn)
{
    currentCommand = strIn;
    commandLength = strIn.size();
}



/*
bool init_GL()
{
    //Set clear color
    glClearColor( 0, 0, 0, 0 );

    //Set projection
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1 );

    //Initialize modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //If there was any errors
    if( glGetError() != GL_NO_ERROR )
    {
        return false;
    }

    //If everything initialized
    return true;
}
*/
