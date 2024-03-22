
/*
gpe_animation2d_branch.h
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

#ifndef gpe_animation2d_branch
#define gpe_animation2d_branch

#include "gpe_animation2d.h"

namespace gpe
{
    //Introduced 1.6X to decouple animation handling within the gpe_basic_object and to allow for a branch that keeps track of and renders the animation
    class animation2d_container: branch
    {
        protected:
            animation2d * animation_ptr;
            bool animation_reached_message;
        public:
            bool animation_ended;
            float animation_speed;
            int animation_duration;
            float current_frame;

            animation2d_container(animation2d * animation_in, int start_xpos, int start_ypos);
            ~animation2d_container();
            bool change_animation(animation2d * animation_in);
            bool has_message();
            int get_frame_message_id();
            virtual void update( float delta_time );
    };

}

#endif //gpe_animation2d_branch
