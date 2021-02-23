/*
gpe_window_controller_base.cpp
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

#include "gpe_window_controller_base.h"

namespace gpe
{
    window_controller_base * window_controller_main = nullptr;

    window_controller_base::window_controller_base()
    {
        window_base_renderer = nullptr;
        window_id = -1;
        window_closed = false;
        window_has_mouse= false;
        window_has_focus = false;
        previously_scaled = false;
        window_width = 0;
        window_height = 0;
        resized = false;
        minimized = false;
        position_x = 0;
        position_y = 0;

        //Set up the window and render area
        //Set window flag
        windowed = true;
        window_id = -1;
        window_scaling = false;
        use_integer_scaling = false;
        scale_percentage_x = 1.f;
        scale_percentage_y = 1.f;
        scale_source_width = -1;
        scale_source_height = -1;

    }

    window_controller_base::~window_controller_base()
    {

    }

    bool window_controller_base::disable_scaling()
    {
        window_scaling = false;
        return false;
    }

    bool window_controller_base::enable_scaling()
    {
        return false;
    }

    bool window_controller_base::error_check()
    {
        return !window_error_occurred;
    }

    bool window_controller_base::get_scaled_previous()
    {
        return previously_scaled;
    }

    int window_controller_base::get_scale_width()
    {
        return scale_source_width;
    }

    int window_controller_base::get_scale_height()
    {
        return scale_source_height;
    }

    float window_controller_base::get_scale_x()
    {
        return scale_percentage_x;
    }

    float window_controller_base::get_scale_y()
    {
        return scale_percentage_y;
    }


    int window_controller_base::get_window_id()
    {
        return window_id;
    }

    int window_controller_base::get_window_width()
    {
        return window_width;
    }

    int window_controller_base::get_window_height()
    {
        return window_height;
    }

    bool window_controller_base::has_focus()
    {
        return window_has_focus;
    }

    bool window_controller_base::has_mouse()
    {
        return window_has_focus;
    }

    bool window_controller_base::hide_window()
    {
        return false;
    }

    bool window_controller_base::is_fullscreen()
    {
        return !windowed;
    }

    bool window_controller_base::is_integer_scaling()
    {
        return use_integer_scaling;
    }

    bool window_controller_base::is_minimized()
    {
        return minimized;
    }

    bool window_controller_base::is_resized()
    {
        return resized;
    }

    bool window_controller_base::is_scaling()
    {
        return window_scaling;
    }


    void window_controller_base::process_event( input_event_container * event_holder )
    {

    }

    void window_controller_base::reset_input()
    {
        resized = false;
        window_closed = false;
        if( window_base_renderer!=nullptr )
        {
            window_base_renderer->resize_renderer( window_width, window_height );
            window_base_renderer->reset_input();
        }
    }

    void window_controller_base::resize_window()
    {
        if( window_base_renderer!=nullptr )
        {
            window_base_renderer->resize_renderer( window_width, window_height );
        }
        minimized = false;
    }

    std::string window_controller_base::save_screenshot(std::string file_location )
    {
        return "";
    }

    bool window_controller_base::scale_window( int s_width, int s_height , bool scale_int )
    {
        if( window_base_renderer !=nullptr )
        {
            return window_base_renderer->scale_renderer(s_width, s_height, scale_int );
        }
        return false;
    }

    bool window_controller_base::scale_window_factor( float s_width, float s_height, bool scale_int )
    {
        return false;
    }

    void window_controller_base::set_window_position( int new_x, int new_y )
    {

    }

    bool window_controller_base::set_window_size( int n_width, int n_height )
    {
        return false;
    }

    void window_controller_base::set_window_title(std::string new_title)
    {

    }

    bool window_controller_base::show_window()
    {
        return false;
    }

    void window_controller_base::start_loop()
    {
        resized = false;
    }

    void window_controller_base::toggle_fullscreen()
    {

    }

    bool window_controller_base::window_changed()
    {
        return resized;
    }
}
