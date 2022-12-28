/*
semath.h
This file is part of:
Short Easy Math Library
https://www.pawbyte.com/semath
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 Short Easy Math Library contributors ( Contributors Page )

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

-Short Easy Math Library <https://www.pawbyte.com/semath>

*/

#ifndef semath_h
#define semath_h

#include <cmath>
#include <stdlib.h>

namespace semath
{
    const int version_major = 1;
    const int version_minor = 2;
    const int version_patch_level = 0;

    const float math_pi = 3.141592653589793;
    const float math_to_radians = 180.f/math_pi;
    const float math_degrees_multiplier = 180.f/-math_pi;
    const float math_radians_multiplier = math_pi/-180.f;

    float bound_number(float numbIn,float xMin =0, float xMax = 100);
    bool compare_floats(float a,float b);

    //Special thanks to YellowAfterLife for help in these calculations
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
    //Special thanks to YellowAfterLife for help in these calculations
    float get_direction( float x1, float y1, float x2, float y2 );
    float get_distance( float x1, float y1, float x2, float y2 );

    //Length/Direction Functions
    //Special thanks to YellowAfterLife for help in these calculations
    float lengthdir_x( float length, float angle);
    float lengthdir_y( float length, float angle);
}

#endif //semath_h


