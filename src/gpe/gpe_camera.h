/*
gpe_camera.h
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

#ifndef gpe_camera_h
#define gpe_camera_h

#include "gpe_globals.h"
#include "gpe_shapes.h"

namespace gpe
{
    const int max_cameras_allowed = 8;
    class scene_camera
    {
        private:
            gpe::shape_rect * camera_rect;
            gpe::shape_rect * camera_boundaries;

            gpe::shape_rect * render_rect;
            gpe::shape_rect * scaled_rect;
            int view_target;
            int border_horizontal;
            int border_vertical;

            float speed_horizontal;
            float speed_vertical;

            bool camera_visible;

            int max_tiles_x_in_view;
            int max_tiles_y_in_view;

            int camera_scene_width;
            int camera_scene_height;

            float camera_angle;
        public:
            scene_camera(float camera_x, float camera_y, float camera_w, float camera_h, int cameraIdNumber );
            ~scene_camera();
            void center_camera ( float center_x, float center_y);
            float get_angle();
            bool is_visible();
            void move_hori ( float move_px );
            void move_vert ( float move_px );
            void set_visible( bool visiblility );
            void set_angle( float new_angle );
            void setup_camera (float camera_x,float camera_y, float camera_w, float camera_h);
            void update_tile_size (int new_xt, int new_yt);
            void update_screen_space ( float camera_x, float camera_y, float camera_w, float camera_h);
            void update_view_space ( float camera_x, float camera_y, float camera_w, float camera_h);
    };

    extern shape_rect * camera_default;
    extern shape_rect * camera_current;

    void camera_reset();
    void update_rectangle( shape_rect * rect_in, float nx, float ny, float nw, float nh);
    shape_rect * camera_find( shape_rect * rect_in = nullptr );

}
#endif //gpe_camera_h
