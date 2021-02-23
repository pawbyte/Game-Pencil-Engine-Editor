/*
gpe_collisions.h
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

#ifndef gpe_collisions_h
#define gpe_collisions_h

//The headers
#include <algorithm>
#include "gpe_common_includes.h"
#include "gpe_shapes.h"

namespace gpe
{
    bool check_collision_rects( gpe::shape_rect A, gpe::shape_rect B );
    bool check_collision_rect_and_point( int recX, int recY, int recW, int recH, gpe::shape_rect A);
    bool check_collision_points_and_rect( int rec1X, int rec1Y, int rec1W, int rec1H, int rec2X, int rec2Y, int rec2W, int rec2H);

    bool check_collision_circles( shape_circle A, shape_circle B );
    bool check_collision_circle_rect( gpe::shape_rect A, shape_circle B );

    bool point_within(int point_x, int point_y,int mxmin,int mymin, int mxmax, int mymax);
    bool point_between(int point_x, int point_y,int mxmin,int mymin, int mxmax, int mymax);
    bool point_between_rect(int point_x, int point_y, gpe::shape_rect * rect_in= nullptr);
    bool point_within_rect(int point_x, int point_y, gpe::shape_rect * rect_in= nullptr);
    //bool mouse_btw(int mxmin,int mymin, int mxmax, int mymax);
}
#endif //gpe_collisions_h

