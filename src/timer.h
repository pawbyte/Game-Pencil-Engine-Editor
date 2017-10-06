/*
timer.h
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

#ifndef TIMER_H
#define TIMER_H

#include "GPE_CIncludes.h"
//The frame rate
extern float FPS_CAP;
extern float FPS_RATIO;
extern int SCREEN_TICK_PER_FRAME;
extern int countedFrames;

//The timer
class Timer
{
    private:
        //The clock time when the timer started
        int startTicks;

        //The ticks stored when the timer was paused
        int pausedTicks;

        //The timer status
        bool paused;
        bool started;
    public:
        //Initializes variables
        Timer();

        //The various clock actions
        void start();
        void stop();
        void pause();
        void unpause();

        //Gets the timer's time
        int get_ticks();

        //Checks the status of the timer
        bool is_started();
        bool is_paused();
};

//The frames per second timer
extern Timer * fpsTimer;

//The frames per second cap timer
extern Timer * capTimer;
void set_fps( int newFPS);
float calculate_avg_fps();
void cap_fps();
#endif
