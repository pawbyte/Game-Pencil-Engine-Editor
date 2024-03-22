
/*
gpe_animation2d_branch.cpp
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/


#include "gpe_animation2d_branch.h"

namespace gpe
{
    animation2d_container::animation2d_container(animation2d * animation_in, int start_xpos, int start_ypos)
    {
        animation_ptr = animation_in;
        animation_reached_message = false;
        xpos = start_xpos;
        ypos = start_ypos;

        animation_ended = false;
        animation_speed = 1.f;
        animation_duration = 0;
        if( animation_ptr != nullptr)
        {
            animation_duration = animation_ptr->get_frame_count();
        }
        current_frame = 0;
    }

    animation2d_container::~animation2d_container()
    {
        animation_ptr = nullptr; //No deletion needed
    }


    bool animation2d_container::change_animation(animation2d * animation_in )
    {
        animation_ended = false;
        animation_ptr = animation_in;
        current_frame = 0;

        if( animation_ptr != nullptr)
        {
            animation_duration = animation_ptr->get_frame_count();
            return true;

        }
        return false;
    }

    bool animation2d_container::has_message()
    {
        if( animation_ptr == nullptr)
        {
            return false;
        }

        return animation_reached_message;
    }

    int animation2d_container::get_frame_message_id()
    {
        if( animation_ptr == nullptr)
        {
            return -1;
        }
        if( animation_reached_message )
        {

        }
        return -1;
    }

    void animation2d_container::update( float delta_time )
    {
        if( animation_ptr == nullptr)
        {
            return;
        }
    }

}
