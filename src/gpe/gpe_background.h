/*
gpe_background.h
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

#ifndef gpe_background_h
#define gpe_background_h

#include "gpe_shared_resources.h"

namespace gpe
{
    class background: public branch
    {
    private:
        texture_base * texture_background;
        float bg_xoffset;
        float bg_yoffset;
        float bg_xspeed;
        float bg_yspeed;
        int bg_id;
        color * bg_color;
        bool render_using_bg_speed;
        bool loop_bg_hori;
        bool loop_bg_vert;
        bool strech_bg;
    public:
        background();
        ~background();
        int get_background_id();
        color * get_scene_background_color();
        void render();
        void render_bg_speed( int xx, int yy, shape_rect *  cam, shape_rect * render_camera);
        void render_bg( shape_rect * renderToArea, shape_rect *  cam, shape_rect * render_camera, int scene_width, int scene_height);
        void scroll_bg();
        void set_bgspeed( float speed_x, float speed_y );
        void set_bgoffsets( float nextX, float nextY );
        void set_background( int new_bg_id);
        void set_background_color( color * color_in);
        void update( float delta_time );
    };
}

#endif //gpe_background_h
