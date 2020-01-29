/*
GPE_Math.h
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

#ifndef GPE_MATH_H
#define GPE_MATH_H

#include <cmath>

namespace gpemath
{
    float bound_number(float numbIn,float xMin =0, float xMax = 100);
    bool compare_floats(float a,float b);

    float point_direction(float x1,float y1,float x2,float y2);
    float point_distance(float x1, float y1, float x2, float y2);

    //random function
     int random_int(int maxIn );
     int random(int low,int high);
     float random_float(float low,float high);
     int sign(float valIn);


    //Degree math
     float deg_cos( float dAngle  );
     float deg_sin( float dAngle  );

    //Directional math
     float get_direction( float x1, float y1, float x2, float y2 );
     float get_distance( float x1, float y1, float x2, float y2 );

    //Length/Direction Functions
     float lengthdir_x( float length, float angle);
     float lengthdir_y( float length, float angle);
};

#endif //GPE_MATH_H


