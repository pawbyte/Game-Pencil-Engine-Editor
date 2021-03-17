/*
gpe_timer.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#include "../other_libs/stg_ex.h"
#include "gpe_timer_raylib.h"

namespace gpe
{
    bool init_raylib_time_system()
    {
        error_log->report("Starting raylib_module timekeeper...");
        if( time_keeper != nullptr )
        {
            delete time_keeper;
            time_keeper = nullptr;
        }
        time_keeper = new time_keeper_raylib("raylib");
        time_keeper->set_fps( fps_cap );
        error_log->report("Timekeeper swapped from base to raylib class...");
        return true;
    }

    void quit_raylib_time_system()
    {
        if( time_keeper !=nullptr )
        {
            delete time_keeper;
            time_keeper = nullptr;
        }
        time_keeper = new time_keeper_base();
    }


    //The frames per second cap timer
    time_keeper_raylib::time_keeper_raylib(std::string t_name)
    {
        keeper_type = "raylib";
        keeper_name = t_name;
        time_past = 0;
        fps_average_count = 5;
        system_cap_on = true;
        min_delay_ms = 16;
        //Initialize the variables
        my_fps = 0;
        paused = false;
        started = false;
        if( fps_cap <=0 )
        {
            fps_cap = 20;
        }
        ticks_per_frame = (float)(1000.f / fps_cap);
        seconds_per_frame = 1.f/fps_cap;
        delta_ticks = ticks_per_frame;
        delta_performance =  ticks_per_frame;
        time_past = 0;
        ticks_now =0;
        time_now = 0;
        ticks_paused = 0;
        timer_frequency = 0;
        ticks_elapsed = 0;
        frames_passed_counter = 0;
    }

    time_keeper_raylib::~time_keeper_raylib()
    {
        recorded_fps.clear();
    }

    void time_keeper_raylib::cap_fps( bool is_minimized )
    {
        //Returns if the window is minimized to prevent strange behaviors
        if( is_minimized)
        {
            delta_ticks = 0;
            delta_performance = 0;
            time_past = 0;
            ticks_start = 0;
            return;
        }
        time_now = get_performance_ms();
        ticks_now = get_ticks();
        delta_ticks =  ticks_now - ticks_start;
        delta_performance = time_now - time_past;

        /*
        if( delta_ticks < ticks_per_frame )
        {
            delay( ticks_per_frame - delta_ticks );
            time_now = get_performance_ms();
            delta_performance = time_now - time_past;
        }
        */
    }

    void time_keeper_raylib::delay( float delay_time )
    {
        if( delay_time <= 0.f )
        {
            return;
        }

        if( system_cap_on || delay_time >= min_delay_ms )
        {
            //error_log->report("Via raylib_Delay..");
            //raylib_Delay( floor( delay_time + 0.5 ) );
            delta_ticks = ticks_per_frame;
            return;
        }
        return;
        float time_c = 0;
        float time_p = get_ticks();
        float timeDiff = 0;
        int loopCount = 0;
        while( timeDiff < delay_time )
        {
            time_c = get_ticks();
            timeDiff = get_time_difference(time_p, time_c );
            loopCount++;
        }
        delta_ticks = ticks_per_frame;
        delta_performance = ticks_per_frame;
    }

    void time_keeper_raylib::finish_timer()
    {

    }

    float time_keeper_raylib::get_delta_performance()
    {
        return (float)GetFrameTime() * 1000.f;
    }

    float time_keeper_raylib::get_delta_ticks()
    {
        return (float)GetFrameTime() * 1000.f;
    }

    float time_keeper_raylib::get_fps()
    {
        return my_fps;
    }

    float time_keeper_raylib::get_needed_ticks()
    {
        return ticks_per_frame;
    }

    float time_keeper_raylib::get_performance_ms()
    {
        return get_ticks();
    }

    float time_keeper_raylib::get_performance_seconds()
    {
        return (float)GetFrameTime();
    }

    uint64_t time_keeper_raylib::get_ticks()
    {
        return (float)GetFrameTime() * 1000.f;
    }

    float time_keeper_raylib::get_time_difference( uint64_t time_p, uint64_t time_c )
    {
        return time_c - time_p;
    }

    bool time_keeper_raylib::is_started()
    {
        return started;
    }

    bool time_keeper_raylib::is_paused()
    {
        return paused;
    }

    void time_keeper_raylib::pause_timer()
    {
        //If the timer is running and isn't already paused
        if( started &&  !paused )
        {
            //Pause the timer
            paused = true;

            //Calculate the paused ticks
            ticks_paused = get_ticks();
        }
    }

    void time_keeper_raylib::reset_timer()
    {
        time_now = 0;
        time_past = 0;
        ticks_now = 0;
        ticks_start = 0;

        ticks_elapsed = 0;
        frames_passed_counter = 0;
        delta_ticks = 0;
        recorded_fps.clear();
    }

    void time_keeper_raylib::set_fps( float fps_new )
    {
        //resets the timers and such since a new FPS cap has been set.
        if( fps_new > 0 && fps_new <= 1000.f )
        {
            fps_cap = fps_new;
        }
        else
        {
            //defaults to 20 fps for out of range FPSs given.
            fps_cap = 20;
        }
        my_fps = fps_cap;
        SetTargetFPS( my_fps );
        ticks_per_frame = 1000.f / my_fps;
        seconds_per_frame = 1.f/my_fps;
        fps_ratio = my_fps/20.f;
        fps_current = my_fps;
        if( fps_ratio < 1.f)
        {
            fps_ratio = 1.f;
        }
        recorded_fps.clear();
    }

    void time_keeper_raylib::set_average_fps_count( int new_count )
    {
        if( fps_average_count != new_count)
        {
            fps_average_count = new_count;
            if( fps_average_count < 1 )
            {
                fps_average_count = 1 ;
            }
            recorded_fps.clear();
        }

    }

    void time_keeper_raylib::start_timer()
    {
        ticks_start = get_ticks();
        time_past = ticks_start;
    }

    void time_keeper_raylib::stop_timer()
    {
        reset_timer();

        //Stop the timer
        started = false;

        //Unpause the timer
        paused = false;

        ticks_paused = get_ticks();
    }


    void time_keeper_raylib::unpause_timer()
    {
        //If the timer is paused
        if(  paused  )
        {
            start_timer();
        }
    }
}
