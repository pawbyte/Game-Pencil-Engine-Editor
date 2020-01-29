/*
GPE_Timer.cpp
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

#include "GPE_Timer.h"
#include "GPE_String_Ex.h"

float FPS_CAP = 20;
float FPS_RATIO = 60/20;
float CURRENT_FPS = 0;
float CURRENT_DELTA = 0;

//The frames per second cap timer
GPE_Timer::GPE_Timer()
{
    pastTime = 0;
    averageFPSCount = 5;
    useOnlySystemCap = false;
    minDelayMS = 16;
    //Initialize the variables
    myFPS = 0;
    paused = false;
    started = false;
    if( FPS_CAP <=0 )
    {
        FPS_CAP = 20;
    }
    ticksPerFrame = (float)(1000.f / FPS_CAP);
    secondsPerFrame = 1.f/FPS_CAP;
    deltaTicks = ticksPerFrame;
    deltaPerformance =  ticksPerFrame;
    pastTime = 0;
    nowTicks =0;
    nowTime = 0;
    pausedTicks = 0;
    timerFrequency = (float)SDL_GetPerformanceFrequency();
    elapsedTicks = 0;
    framesTimePassed = 0;
}

GPE_Timer::~GPE_Timer()
{
    recordedFPS.clear();
}

void GPE_Timer::calculate_avg_fps( bool isMimimized )
{
    //Returns if the window is minimized to prevent strange behaviors
    if( isMimimized)
    {
        framesTimePassed = 0.f;
        elapsedTicks = 0.f;
        return;
    }

    //Adds the current delta to elapsed ticks
    elapsedTicks+= deltaTicks;
    //Increments our framecount
    framesTimePassed += 1.f;

    //Once our ticks have reached 1000ms or more we calculate
    if( elapsedTicks >= 1000.f )
    {
        //Our FPS is simple its now our frame counter
        CURRENT_FPS = framesTimePassed;
        //We add this to our vector/list of frames
        recordedFPS.push_back( framesTimePassed );

        //If our list is now greater than our average
        int fpsSize =  (int)recordedFPS.size();
        if(  fpsSize >= averageFPSCount )
        {
            float avgFPS = 0.f;
            for(int i = 0; i < fpsSize; i++)
            {
                avgFPS += recordedFPS[i];
            }
            myFPS = avgFPS / (float)fpsSize;
            CURRENT_DELTA = 1000.f / myFPS;

            //We erase the top of the list
            recordedFPS.erase( recordedFPS.begin()  );
        }

        //Reset our counter and elapsticks
        elapsedTicks = 0.f;
        framesTimePassed = 0;
    }
}

void GPE_Timer::cap_fps( bool isMimimized )
{
    //Returns if the window is minimized to prevent strange behaviors
    if( isMimimized)
    {
        deltaTicks = 0;
        deltaPerformance = 0;
        pastTime = 0;
        startTicks = 0;
        return;
    }
    nowTime = get_performance_ms();
    nowTicks = get_ticks();
    deltaTicks =  nowTicks - startTicks;
    deltaPerformance = nowTime - pastTime;

    if( deltaTicks < ticksPerFrame )
    {
        delay( ticksPerFrame - deltaTicks );
        nowTime = get_performance_ms();
        deltaPerformance = nowTime - pastTime;
    }
}

void GPE_Timer::delay( float delayTime )
{
    if( delayTime <= 0.f )
    {
        return;
    }

    if( useOnlySystemCap || delayTime >= minDelayMS )
    {
        //GPE_Report("Via SDL_Delay..");
        SDL_Delay( floor( delayTime ) );
        deltaTicks = ticksPerFrame;
        return;
    }

    float cTime = 0;
    float pTime = get_ticks();
    float timeDiff = 0;
    int loopCount = 0;
    while( timeDiff < delayTime )
    {
        cTime = get_ticks();
        timeDiff = get_time_difference(pTime, cTime );
        loopCount++;
    }
    deltaTicks = ticksPerFrame;
    deltaPerformance = ticksPerFrame;
}

void GPE_Timer::finish_timer()
{

}

float GPE_Timer::get_delta_performance()
{
    return deltaPerformance;
}

float GPE_Timer::get_delta_ticks()
{
    return deltaTicks;
}

float GPE_Timer::get_fps()
{
    return myFPS;
}

float GPE_Timer::get_needed_ticks()
{
    return ticksPerFrame;
}

float GPE_Timer::get_performance_ms()
{
    return ( ( (float)SDL_GetPerformanceCounter() )/( (float)SDL_GetPerformanceFrequency() ) )* 1000.0f;
}

float GPE_Timer::get_performance_seconds()
{
    return ( (float)SDL_GetPerformanceCounter() )/( (float)SDL_GetPerformanceFrequency() );
}

Uint64 GPE_Timer::get_ticks()
{
    return SDL_GetTicks();
}

float GPE_Timer::get_time_difference( Uint64 pTime, Uint64 cTime )
{
    return cTime - pTime;
}

bool GPE_Timer::is_started()
{
    return started;
}

bool GPE_Timer::is_paused()
{
    return paused;
}

void GPE_Timer::pause_timer()
{
    //If the timer is running and isn't already paused
    if( started &&  !paused )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = get_ticks();
    }
}

void GPE_Timer::reset_timer()
{
    nowTime = 0;
    pastTime = 0;
    nowTicks = 0;
    startTicks = 0;

    elapsedTicks = 0;
    framesTimePassed = 0;
    deltaTicks = 0;
    recordedFPS.clear();
}

void GPE_Timer::set_fps( float newFPS )
{
    //resets the timers and such since a new FPS cap has been set.
    if( newFPS > 0 && newFPS <= 1000.f )
    {
        FPS_CAP = newFPS;
    }
    else
    {
        //defaults to 20 fps for out of range FPSs given.
        FPS_CAP = 20;
    }
    myFPS = FPS_CAP;
    ticksPerFrame = 1000.f / myFPS;
    secondsPerFrame = 1.f/myFPS;
    FPS_RATIO = myFPS/20.f;
    CURRENT_FPS = myFPS;
    if( FPS_RATIO < 1.f)
    {
        FPS_RATIO = 1.f;
    }
    recordedFPS.clear();
}

void GPE_Timer::set_average_fps_count( int newCount )
{
    if( averageFPSCount != newCount)
    {
        averageFPSCount = newCount;
        if( averageFPSCount < 1 )
        {
            averageFPSCount = 1 ;
        }
        recordedFPS.clear();
    }

}

void GPE_Timer::start_timer()
{
    startTicks = get_ticks();
    pastTime = get_performance_ms();
}

void GPE_Timer::stop_timer()
{
    reset_timer();

    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;

    pausedTicks = get_ticks();
}


void GPE_Timer::unpause_timer()
{
    //If the timer is paused
    if(  paused  )
    {
        start_timer();
    }
}

