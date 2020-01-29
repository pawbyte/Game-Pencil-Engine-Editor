/*
GPE_Timer.h
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

#ifndef TIMER_H
#define TIMER_H

#include "GPE_CIncludes.h"
#include <math.h>
#include <chrono>

//The frame rate
extern float FPS_CAP;
extern float FPS_RATIO;
extern float CURRENT_FPS;
extern float CURRENT_DELTA;

//The timer
class GPE_Timer
{
private:
    std::vector < float > recordedFPS;
    float  myFPS;
    float  secondsPerFrame;

    //Performance
    float  nowTime;
    float  pastTime;
    float deltaPerformance;

    float framesTimePassed;
    float timerFrequency;

    //Ticks
    float  ticksPerFrame;
    float deltaTicks;
    Uint32 nowTicks;
    Uint32 startTicks;
    Uint32 elapsedTicks;
    Uint32 pausedTicks;

    //The timer statuses
    bool paused;
    bool started;
    int averageFPSCount;
public:
    //Initializes variables
    bool useOnlySystemCap;
    float minDelayMS;
    int fpsAverage;
    GPE_Timer();
    ~GPE_Timer();

    void calculate_avg_fps( bool isMimimized );
    void cap_fps( bool isMimimized );

    void delay( float delayTime );
    void finish_timer();

    float get_needed_ticks();
    void start_timer();
    void stop_timer();
    void pause_timer();
    void unpause_timer();

    float get_delta_performance();
    float get_delta_ticks();

    float get_fps();
    float get_performance_ms();
    float get_performance_seconds();
    Uint64 get_ticks();
    float get_time_difference( Uint64 pTime, Uint64 cTime );

    bool is_started();
    bool is_paused();

    void reset_timer();

    void set_fps( float newFPS = 30 );
    void set_average_fps_count( int newCount );
};

//The frames per second timer
#endif
