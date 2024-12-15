/*
gpe_timer.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine  - SDL2 Module <https://gamepencil.net/sdl2-module/>


*/

#ifndef gpe_timer_sdl2_h
#define gpe_timer_sdl2_h

#include "../gpe/gpe_runtime.h"
#include "../gpe/gpe_timer_base.h"
#include <math.h>
#include "SDL.h"

namespace gpe
{
    //The timer
    class time_keeper_sdl2: public time_keeper_base
    {
        protected:

        public:
            time_keeper_sdl2( std::string t_name );
            ~time_keeper_sdl2();

            void cap_fps( bool is_minimized );

            void delay( float delay_time );
            void finish_timer();

            float get_needed_ticks();
            void start_timer();
            void stop_timer();
            void pause_timer();
            void unpause_timer();

            float get_delta_performance();
            float get_delta_ticks();

            float get_fps();
            float get_fps_cap();
            float get_performance_ms();
            float get_performance_seconds();
            uint64_t get_ticks();
            float get_time_difference( uint64_t time_p, uint64_t time_c );

            bool is_started();
            bool is_paused();

            void reset_timer();

            void set_average_fps_count( int new_count );
            void set_fps( float fps_new = 60 );
            void set_vysnc( bool vs_on );

    };

    bool init_sdl2_time_system();
    void quit_sdl2_time_system();
}

//The frames per second timer
#endif //gpe_timer_sdl2_h
