/*
gpe_input_event.cpp
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

#include "gpe_input_event.h"

namespace gpe
{
    input_event_container::input_event_container()
    {
        event_time = 0;
        event_type = input_event_type::event_none;

        input_string = "";
        keyboard_button_id = -1;


        gamepad_axis_id = -1;
        gamepad_axis_value = 0;
        gamepad_button_id = -1;
        gamepad_id = -1;

        input_string = "";

        mouse_button_id = -1;
        mouse_position_x = -1;
        mouse_position_y = -1;
        mouse_wheel_id = -1;
        mouse_wheel_value = 0;

        window_id = -1;
        window_resize_w = -1;
        window_resize_h = -1;
        reset_event();
    }

    input_event_container::~input_event_container()
    {

    }

    void input_event_container::reset_event()
    {
        event_time = 0;
        event_type = input_event_type::event_none;

        input_string = "";
        keyboard_button_id = -1;


        gamepad_axis_id = -1;
        gamepad_axis_value = 0;
        gamepad_button_id = -1;
        gamepad_id = -1;

        input_string = "";

        mouse_button_id = -1;
        mouse_position_x = -1;
        mouse_position_y = -1;
        mouse_wheel_id = -1;
        mouse_wheel_value = 0;

        window_id = -1;
        window_resize_w = -1;
        window_resize_h = -1;
    }
}
