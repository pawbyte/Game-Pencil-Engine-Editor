/*
GPE_Timer.cpp
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

#include "GPE_Timer.h"
#include "GPE_String_Ex.h"

double FPS_CAP = 30;
double FPS_RATIO = 60/30;
int CURRENT_FPS = 0;
int CURRENT_DELTA = 0;

//The frames per second cap timer
GPE_Timer::GPE_Timer()
{
    //Initialize the variables
    myFPS = 0;
    currentTick = lastTime = startTicks = SDL_GetTicks();
    pausedTicks = 0;
    paused = false;
    started = false;
    if( FPS_CAP <=0 )
    {
        FPS_CAP = 30;
    }
    ticksPerFrame = 1000 / FPS_CAP;
    deltaTime = ticksPerFrame;
}

GPE_Timer::~GPE_Timer()
{

}

int GPE_Timer::calculate_avg_fps()
{
    //Calculate and correct fps
    //currentTick = SDL_GetTicks();
    deltaTime = currentTick- lastTime;
    if ( deltaTime != 0 )
    {
        recordedDeltas.push_back( deltaTime );
        double tempFPS = 1000.l / (double)deltaTime;
        //CURRENT_FPS = round( avgFPS );
        recordedFPS.push_back( tempFPS );
        myFPS = round( tempFPS );
        int fpsTotalStored = std::min( (int) recordedDeltas.size() ,(int) recordedFPS.size() );
        if( fpsTotalStored >= 250 )
        {
            tempFPS = 0;
            double TEMP_DELTA = 0;
            for( int i = 0; i < (int) fpsTotalStored; i++)
            {
                tempFPS+= recordedFPS[i];
                TEMP_DELTA+= recordedDeltas[i];
            }
            tempFPS = (double)tempFPS /(double)fpsTotalStored;
            myFPS = CURRENT_FPS = round( tempFPS );
            TEMP_DELTA = (double)TEMP_DELTA /(double)fpsTotalStored;
            CURRENT_DELTA = round( TEMP_DELTA );
            recordedFPS.clear();
            recordedDeltas.clear();
        }
        return myFPS;
    }
    return FPS_CAP;
}

void GPE_Timer::cap_fps()
{
    currentTick = SDL_GetTicks();
    deltaTime = currentTick - lastTime;
    //If frame finished early
    if( deltaTime < ticksPerFrame )
    {
        //Wait remaining time
        int msDelay = ticksPerFrame - deltaTime;
        if( msDelay >= 1)
        {
            delay( msDelay );
        }
    }
    //currentTick = 0;
    //
    deltaTime = ticksPerFrame;
    myFPS = FPS_CAP;
}

void GPE_Timer::delay( int delayTime )
{
    if( delayTime > 1)
    {
        currentTick+=delayTime;
        SDL_Delay( delayTime);
        currentTick = SDL_GetTicks();
        return;
    }

    if( delayTime > 0 )
    {
        int msPassed = 0;
        int cTick = currentTick;
        int lTick = cTick;
        while( msPassed < delayTime )
        {
            msPassed+= cTick - lTick;
            lTick = cTick;
            cTick = currentTick = SDL_GetTicks();
        }
    }
}

int GPE_Timer::get_delta_time()
{
    return deltaTime;
}

int GPE_Timer::get_fps()
{
    return myFPS;
}

int GPE_Timer::get_ticks()
{
    //If the timer is running
    if( started )
    {
        //If the timer is paused
        if( paused )
        {
            //Return the number of ticks when the the timer was paused
            currentTick =  pausedTicks;
            return currentTick;
        }
        else
        {
            currentTick =  SDL_GetTicks();
            //Return the current time minus the start time
            return (currentTick - startTicks);
        }
    }

    //If the timer isn't running
    return currentTick;
}

bool GPE_Timer::is_started()
{
    return started;
}

bool GPE_Timer::is_paused()
{
    return paused;
}

void GPE_Timer::pause()
{
    //If the timer is running and isn't already paused
    if( started &&  !paused )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks();
        startTicks = 0;
    }
}


void GPE_Timer::set_fps( int newFPS )
{
    recordedDeltas.clear();
    recordedFPS.clear();
    //resets the timers and such since a new FPS cap has been set.

    if( newFPS > 0 && newFPS <= 300)
    {
        FPS_CAP = newFPS;
    }
    else
    {
        //defaults to 30 fps for out of range FPSs given.
        FPS_CAP = 30;
    }
    myFPS = FPS_CAP;
    ticksPerFrame = 1000.l / FPS_CAP;
    deltaTime = ticksPerFrame;
    CURRENT_DELTA = ticksPerFrame;
    FPS_RATIO = FPS_CAP/30.l;
    CURRENT_FPS = newFPS;
    if( FPS_RATIO < 0)
    {
        FPS_RATIO = 1;
    }

    start();
    currentTick = lastTime = startTicks = SDL_GetTicks();

}

void GPE_Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    pausedTicks = 0;
    currentTick = lastTime  = SDL_GetTicks();
}

void GPE_Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;

    pausedTicks = SDL_GetTicks();
}


void GPE_Timer::unpause()
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

