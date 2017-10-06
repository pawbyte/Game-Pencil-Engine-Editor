/*
timer.cpp
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

#include "timer.h"

float FPS_CAP = 60;
float FPS_RATIO = 60/30;
int SCREEN_TICK_PER_FRAME = 1000 / FPS_CAP;
int countedFrames = 0;
Timer * fpsTimer = NULL;

//The frames per second cap timer
Timer * capTimer = NULL;

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;

    startTicks = 0;
    pausedTicks = 0;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( started &&  !paused )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( started && paused  )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //The actual timer time
	Uint32 time = 0;
    //If the timer is running
    if( started )
    {
        //If the timer is paused
        if( paused )
        {
            //Return the number of ticks when the the timer was paused
            time =  pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time =  SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return time;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

float calculate_avg_fps()
{
    //Calculate and correct fps
    float avgFPS = countedFrames / ( fpsTimer->get_ticks() / 1000.f );
    if( avgFPS > 2000000 )
    {
        avgFPS = 0;
    }
}

void set_fps( int newFPS)
{
    if( newFPS > 0 && newFPS <= 300)
    {
        FPS_CAP = newFPS;
        SCREEN_TICK_PER_FRAME = 1000 / FPS_CAP;
    }
    else
    {
        //defaults to 30 fps for out of range FPSs given.
        FPS_CAP = 30;
        SCREEN_TICK_PER_FRAME = 1000 / FPS_CAP;
    }
    FPS_RATIO = (float)FPS_CAP/30;
    if( FPS_RATIO < 0)
    {
        FPS_RATIO = 1;
    }
}
void cap_fps()
{
    ++countedFrames;
    //If frame finished early
    int frameTicks = capTimer->get_ticks();
    if( frameTicks < SCREEN_TICK_PER_FRAME )
    {
        //Wait remaining time
        SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
    }
}
