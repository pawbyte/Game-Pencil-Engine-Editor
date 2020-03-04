/*
gpe_timer_base.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#ifndef gpe_timer_base_h
#define gpe_timer_base_h
#include <cstdint>
#include <string>
#include <vector>

namespace gpe
{
    //The frame rate
    extern float fps_cap;
    extern float fps_ratio;
    extern float fps_current;
    extern float delta_current;
    extern int time_keeper_count;

    //The timer
    class time_keeper_base
    {
        protected:
            int timer_keeper_id;
            std::string keeper_type;
            std::string keeper_name;
            std::vector < float > recorded_fps;
            float  my_fps;
            float  seconds_per_frame;

            uint64_t ticks_now;
            uint64_t ticks_start;
            uint64_t ticks_elapsed;
            uint64_t ticks_paused;

            //Performance
            float  time_now;
            float  time_past;

            float delta_ticks;
            float delta_performance;

            float frames_passed_counter;
            float timer_frequency;

            //Ticks
            float  ticks_per_frame;

            //The timer statuses
            bool paused;
            bool started;
            int fps_average_count;
        public:
            //Initializes variables
            bool system_cap_on;
            float min_delay_ms;
            int average_fps;
            time_keeper_base();
            time_keeper_base( std::string t_name );
            virtual ~time_keeper_base();

            virtual void calculate_avg_fps( bool is_minimized );
            virtual void cap_fps( bool is_minimized );

            virtual void delay( float delay_time );
            int equals( time_keeper_base * other );
            virtual void finish_timer();

            virtual float get_needed_ticks();
            virtual void start_timer();
            virtual void stop_timer();
            virtual void pause_timer();
            virtual void unpause_timer();

            virtual float get_delta_performance();
            virtual float get_delta_ticks();

            virtual float get_fps();
            virtual float get_performance_ms();
            virtual float get_performance_seconds();
            virtual uint64_t get_ticks();
            virtual float get_time_difference( uint64_t time_p, uint64_t time_c );

            virtual bool is_started();
            virtual bool is_paused();

            virtual void reset_timer();

            virtual void set_fps( float fps_new = 30 );
            virtual void set_average_fps_count( int new_count );
    };

    extern time_keeper_base * time_keeper;
}

#endif //gpe_timer_base_h
