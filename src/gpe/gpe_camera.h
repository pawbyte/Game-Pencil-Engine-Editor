/*
gpe_camera.h
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
            gpe::shape_rect * cameraRect;
            gpe::shape_rect * cameraBoundaries;

            gpe::shape_rect * renderRect;
            gpe::shape_rect * scaledRect;
            int viewTarget;
            int horiBorder;
            int vertBorder;

            float hSpeed;
            float vSpeed;

            bool isVisible;

            int maxXTilesInView;
            int maxYTilesInView;

            int cSceneWidth;
            int cSceneHeight;
        public:
            scene_camera(float cameraX, float cameraY, float cameraW, float cameraH, int cameraIdNumber );
            ~scene_camera();
            void center_camera ( float centerX, float centerY);
            bool is_visible();
            void move_hori ( float movSpace );
            void move_vert ( float movSpace );
            void set_visible( bool visiblility );
            void setup_camera (float cameraX,float cameraY, float cameraW, float cameraH);
            void update_tile_size (int newTX, int newTY);
            void update_screen_space ( float cameraX, float cameraY, float cameraW, float cameraH);
            void update_view_space ( float cameraX, float cameraY, float cameraW, float cameraH);
    };

    extern shape_rect * camera_default;
    extern shape_rect * camera_current;

    void camera_reset();
    void update_rectangle( shape_rect * rectIn, float nx, float ny, float nw, float nh);
    shape_rect * camera_find( shape_rect * rectIn = NULL );

}
#endif //gpe_camera_h
