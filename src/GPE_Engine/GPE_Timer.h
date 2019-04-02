/*
GPE_Timer.h
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

#ifndef TIMER_H
#define TIMER_H

#include "GPE_CIncludes.h"
#include <math.h>
//The frame rate
extern double FPS_CAP;
extern double FPS_RATIO;
extern int CURRENT_FPS;
extern int CURRENT_DELTA;

//The timer
class GPE_Timer
{
private:
    std::vector < double > recordedFPS;
    std::vector < double > recordedDeltas;
    int myFPS;
    int ticksPerFrame;
    int lastTime;
    int currentTick;
    int deltaTime;
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;
public:
    //Initializes variables
    GPE_Timer();
    ~GPE_Timer();

    //The various clock actions
    void delay( int delayTime );
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_delta_time();
    int get_fps();
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();

    //The frames per second cap timer
    void set_fps( int newFPS = 30 );
    int calculate_avg_fps();
    void cap_fps();
};

//The frames per second timer

#endif
