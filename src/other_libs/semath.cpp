/*
semath.cpp
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

#include "semath.h"

namespace semath
{
    float bound_number(float numbIn, float xMin,float xMax)
    {
        if( numbIn < xMin)
        {
            numbIn = xMin;
        }
        else if( numbIn > xMax)
        {
            numbIn = xMax;
        }
        return numbIn;
    }

    bool compare_floats(float a, float b)
    {
        return fabs(a - b) < 0.00001;
    }

    float deg_cos( float dAngle  )
    {
        return cos( dAngle *math_radians_multiplier );
    }

    float deg_sin( float dAngle  )
    {
        return sin(dAngle *math_radians_multiplier);
    }

    //Special thanks to YellowAfterLife for help in these calculations
    float get_direction( float x1, float y1, float x2, float y2 )
    {
        float returnVal =  atan2(y2-y1,x2-x1) * math_degrees_multiplier;
        if( returnVal < 0 )
        {
            returnVal+=360;
        }
        return returnVal;
    }

    float get_distance( float x1, float y1, float x2, float y2 )
    {
        if( x1==x2 && y1==y2 )
        {
            return 0;
        }
        return sqrt( pow(x2 - x1, 2) + pow(y2 - y1, 2) );
    }

    float lengthdir_x( float length, float angle  )
    {
        return deg_cos( angle  ) * length;
    }

    float lengthdir_y( float length, float angle )
    {
        return  deg_sin( angle  ) * length;
    }

    float point_direction(float x1,float y1,float x2,float y2)
    {
        if(x1==x2&&y1==y2)
        {
            return 0;
        }
        return atan2(y2-y1,x2-x1);
    }

    float point_distance(float x1, float y1, float x2, float y2)
    {
        if(x1==x2&&y1==y2)
        {
            return 0;
        }
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

    int random_int(int maxIn)
    {
        return ( rand()  % (maxIn + 1));
    }

    int random(int low,int high)
    {
        return(rand()  % (high-low + 1) + low);
    }

    float random_float(float low,float high)
    {
        float temp;

        //Flips values if wrong order given
        if (low > high)
        {
            temp = low;
            low = high;
            high = temp;
        }

        return( rand() / ( (float)(RAND_MAX) + 1.0 ) ) * (high - low) + low;
    }

    int sign(float valIn)
    {
        if(valIn==0)
        {
            return 0;
        }
        else if(valIn<0)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
}
