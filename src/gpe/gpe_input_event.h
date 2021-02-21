/*
gpe_input_event.h
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
#ifndef gpe_input_event_h
#define gpe_input_event_h

#include <cstdint>
#include <string>

namespace gpe
{
    enum class input_event_type
    {
        event_none,

        file_dropped_event,

        gamepad_added_event,
        gamepad_axis_moved_event,
        gamepad_button_down_event,
        gamepad_button_released_event,
        gamepad_removed_event,

        keyboard_down_event,
        keyboard_released_event,
        keyboard_text_event,

        mouse_button_down_event,
        mouse_button_released_event,
        mouse_moved_event,
        mouse_wheel_event,

        program_quit_event,

        window_closed_event,
        window_entered_event,
        window_exposed_event,
        window_focus_gained_event,
        window_focus_lost_event,
        window_hidden_event,
        window_leave_event,
        window_minimized_event,
        window_maximized_event,
        window_resized_event,
        window_shown_event,
        event_max,
    };

    class input_event_container
    {
        public:
            //basic event related variables
            //add more if needed
            uint64_t event_time;
            input_event_type event_type;


            int gamepad_axis_id;
            float gamepad_axis_value;
            int gamepad_button_id;
            int gamepad_id;

            std::string input_string;
            int keyboard_button_id;

            int mouse_button_id;
            int mouse_position_x;
            int mouse_position_y;
            int mouse_wheel_id;
            float mouse_wheel_value;

            int window_id;
            int window_resize_w;
            int window_resize_h;

            //Simple Functions
            input_event_container();
            virtual ~input_event_container();
            virtual void reset_event();
    };
}

#endif //gpe_input_event_h
