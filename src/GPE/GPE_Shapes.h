/*
GPE_Shapes.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef GPE_SHAPES_H
#define GPE_SHAPES_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class GPE_Circle
{
    public:
        float x;
        float y;
        float radius;
        GPE_Circle(float xpos, float ypos, float radiusIn);
        ~GPE_Circle();
};

class GPE_Rect
{
    public:
        float x;
        float y;
        float xCenter;
        float x2Pos;
        float yMiddle;
        float y2Pos;
        float w;
        float h;
        GPE_Rect( float rX= 0, float rY= 0, float rW= 0, float rH = 0);
        ~GPE_Rect();
        void add_x( float xPlus);
        void add_y( float yPlus);
        void copy_rect( GPE_Rect * otherRect );
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
        void set_x( float newX );
        void set_y( float newY );
        void set_w( float newW );
        void set_h( float newH );
        void update_position( float xNew, float yNew);
        void update_box( float xNew, float yNew, float widNew, float hgtNew);
        void update_shape( float xNew, float yNew, float widNew, float hgtNew);
        void update_size( float widNew, float hgtNew);
};

#endif
