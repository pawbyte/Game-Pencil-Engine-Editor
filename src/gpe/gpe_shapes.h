/*
gpe_shapes.h
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

#ifndef gpe_shapes_h
#define gpe_shapes_h

#include <stdio.h>
#include <math.h>

namespace gpe
{
    struct shape_point2d
    {
        float x, y;
    };

    struct shape_point3d
    {
        float x, y, z;
    };

    class shape_circle
    {
        public:
            shape_point2d position;
            float radius;
            shape_circle(float xpos, float ypos, float radiusIn);
            ~shape_circle();
    };

    class shape_sphere
    {
        public:
            shape_point3d position;
            float radius;
            shape_sphere(float xpos, float ypos, float zpos, float radiusIn);
            ~shape_sphere();
    };

    class shape_rect
    {
        public:
            float x;
            float y;
            float w;
            float h;
            shape_rect( float rX= 0, float rY= 0, float rW= 0, float rH = 0);
            ~shape_rect();
            void add_x( float xPlus);
            void add_y( float yPlus);
            void copy_rect( shape_rect * otherRect );
            float get_center();
            float get_height();
            float get_middle();
            float get_width();
            float get_x();
            float get_x2();
            float get_y();
            float get_y2();
            //Limitations
            void limit_space_to_rect(int * limitedX = NULL, int * limitedY = NULL);
            void set_x( float x_new );
            void set_y( float y_new );
            void set_w( float newW );
            void set_h( float newH );
            void update_position( float xNew, float yNew);
            void update_box( float xNew, float yNew, float widNew, float hgtNew);
            void update_shape( float xNew, float yNew, float widNew, float hgtNew);
            void update_size( float widNew, float hgtNew);
    };

    class shape_triangle2d
    {
        public:
            shape_point2d vertices[3];
            shape_triangle2d();
            ~shape_triangle2d();
            void sort_by_x();
            void sort_by_y();
    };

}

#endif //gpe_shapes_h
