/*
gpe_window_controller_base.h
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

#ifndef gpe_window_base_h
#define gpe_window_base_h

#include "gpe_common_includes.h"
#include "gpe_input_event.h"
#include "gpe_shapes.h"
#include "gpe_renderer_base.h"
#include "gpe_settings.h"

namespace gpe
{
    class window_controller_base
    {
        protected:
            //_scale based properties [ start]
            bool previously_scaled;
            bool window_scaling;
            bool use_integer_scaling;
            float scale_percentage_x, scale_percentage_y;
            int scale_source_width, scale_source_height;
            //_scale based properties [ end ]

            bool windowed;
            //Whether the window is windowed or not
            int position_x, position_y;
            //Whether the window is fine
            std::string local_window_name;
            bool minimized;
            bool resized;
            renderer_base * window_base_renderer;
            bool window_error_occurred;
            int window_id;
            int window_width, window_height;
        public:
            bool window_has_focus;
            bool window_has_mouse;
            bool window_closed;

            window_controller_base();
            virtual ~window_controller_base();

            //Check if anything's wrong with the window
            virtual bool disable_scaling();
            virtual bool enable_scaling();
            bool error_check();

            //Window getters
            int get_blend_mode();
            int get_window_id();
            int get_window_width();
            int get_window_height();
            std::string get_window_name();

            int get_scale_width();
            int get_scale_height();

            bool get_scaled_previous();
            float get_scale_x();
            float get_scale_y();

            bool has_focus();
            bool has_mouse();
            virtual bool hide_window();
            bool is_fullscreen();
            bool is_integer_scaling();

            bool is_minimized();
            bool is_resized();
            bool is_scaling();

            virtual void process_event( input_event_container * event_holder );

            virtual void reset_input();
            virtual void resize_window();
            virtual bool show_window();
            virtual bool scale_window( int s_width, int s_height , bool scale_int );
            virtual bool scale_window_factor( float s_width, float s_height, bool scale_int );

            virtual void set_window_position( int new_x, int new_y );
            virtual bool set_window_size( int n_width, int n_height );
            virtual void set_window_title(std::string new_title);
            virtual std::string save_screenshot(std::string file_location = "");

            virtual void start_loop();
            //Turn fullscreen on/off
            virtual void toggle_fullscreen();

            bool window_changed();

    };

    extern window_controller_base * window_controller_main;
}
#endif //gpe_window_base_h
