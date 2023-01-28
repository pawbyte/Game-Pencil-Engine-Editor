/*
gpe_timer.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_timer_base.h"

namespace gpe
{
    float fps_cap = 20;
    float fps_ratio = 60/20;
    float fps_current = 0;
    float delta_current = 0;
    int time_keeper_count = 0;

    time_keeper_base * time_keeper = nullptr;
    //The frames per second cap timer
    time_keeper_base::time_keeper_base()
    {
        timer_keeper_id = time_keeper_count;
        time_keeper_count++;
        keeper_name = "base";
        keeper_type = "base";
        time_past = 0;
        fps_average_count = 5;
        system_cap_on = false;
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

    time_keeper_base::~time_keeper_base()
    {
        recorded_fps.clear();
    }

    void time_keeper_base::calculate_avg_fps( bool is_minimized )
    {
        //Returns if the window is minimized to prevent strange behaviors
        if( is_minimized)
        {
            frames_passed_counter = 0.f;
            ticks_elapsed = 0.f;
            return;
        }

        //Adds the current delta to elapsed ticks
        ticks_elapsed+= delta_ticks;
        //Increments our framecount
        frames_passed_counter += 1.f;

        //Once our ticks have reached 1000ms or more we calculate
        if( ticks_elapsed >= 1000.f )
        {
            //Our FPS is simple its now our frame counter
            fps_current = frames_passed_counter;
            //We add this to our vector/list of frames
            recorded_fps.push_back( frames_passed_counter );

            //If our list is now greater than our average
            int fpsSize =  (int)recorded_fps.size();
            if(  fpsSize >= fps_average_count )
            {
                float avgFPS = 0.f;
                for(int i = 0; i < fpsSize; i++)
                {
                    avgFPS += recorded_fps[i];
                }
                my_fps = avgFPS / (float)fpsSize;
                delta_current = 1000.f / my_fps;

                //We erase the top of the list
                recorded_fps.erase( recorded_fps.begin()  );
            }

            //Reset our counter and elapsticks
            ticks_elapsed = 0.f;
            frames_passed_counter = 0;
        }
    }

    void time_keeper_base::cap_fps( bool is_minimized )
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

        if( delta_ticks < ticks_per_frame )
        {
            delay( ticks_per_frame - delta_ticks );
            time_now = get_performance_ms();
            delta_performance = time_now - time_past;
        }
    }

    void time_keeper_base::delay( float delay_time )
    {
        return;
    }

    int time_keeper_base::equals( time_keeper_base * other )
    {
        if( other == nullptr )
        {
            return -1;
        }

        if( timer_keeper_id == other->timer_keeper_id )
        {
            if( keeper_name == other->keeper_name )
            {
                if( keeper_type == other->keeper_type )
                {
                    return 1;
                }
            }
        }
        return 0;
    }

    void time_keeper_base::finish_timer()
    {

    }

    float time_keeper_base::get_delta_performance()
    {
        return delta_performance;
    }

    float time_keeper_base::get_delta_ticks()
    {
        return delta_ticks;
    }

    float time_keeper_base::get_fps()
    {
        return my_fps;
    }

    float time_keeper_base::get_fps_cap()
    {
        return fps_cap;
    }

    std::string time_keeper_base::get_name()
    {
        return keeper_name;
    }
    float time_keeper_base::get_needed_ticks()
    {
        return ticks_per_frame;
    }

    float time_keeper_base::get_performance_ms()
    {
        return 0;
    }

    float time_keeper_base::get_performance_seconds()
    {
        return 0;
    }

    uint64_t time_keeper_base::get_ticks()
    {
        return 0;
    }

    float time_keeper_base::get_time_difference( uint64_t time_p, uint64_t time_c )
    {
        return time_c - time_p;
    }

    bool time_keeper_base::get_vysnc()
    {
        return vsync_is_on;
    }

    bool time_keeper_base::is_started()
    {
        return started;
    }

    bool time_keeper_base::is_paused()
    {
        return paused;
    }

    void time_keeper_base::pause_timer()
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

    void time_keeper_base::reset_timer()
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

    void time_keeper_base::set_fps( float fps_new )
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

    void time_keeper_base::set_average_fps_count( int new_count )
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

    void time_keeper_base::set_vysnc( bool vs_on )
    {
        vsync_is_on = vs_on;
    }

    void time_keeper_base::start_timer()
    {
        ticks_start = get_ticks();
        time_past = get_performance_ms();
    }

    void time_keeper_base::stop_timer()
    {
        reset_timer();

        //Stop the timer
        started = false;

        //Unpause the timer
        paused = false;

        ticks_paused = get_ticks();
    }


    void time_keeper_base::unpause_timer()
    {
        //If the timer is paused
        if(  paused  )
        {
            start_timer();
        }
    }
}
