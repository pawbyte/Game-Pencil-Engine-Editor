/*
gpe_collisions.cpp
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

#include "gpe_collisions.h"

namespace gpe
{
    bool check_collision_rects( gpe::shape_rect A, gpe::shape_rect B)
    {
        //The sides of the rectangles
        int leftA, leftB;
        int rightA, rightB;
        int topA, topB;
        int bottomA, bottomB;

        //Calculate the sides of rect A
        leftA = A.x;
        rightA = A.x + A.w;
        topA = A.y;
        bottomA = A.y + A.h;

        //Calculate the sides of rect B
        leftB = B.x;
        rightB = B.x + B.w;
        topB = B.y;
        bottomB = B.y + B.h;
        return !(  bottomA <= topB ||  topA >= bottomB  || rightA <= leftB || leftA >= rightB );
    }

    bool check_collision_rect_and_point( gpe::shape_rect A, int recX, int recY, int recW, int recH )
    {
        gpe::shape_rect newB;
        newB.x = (int)recX;
        newB.y = (int)recY;
        newB.w = recW;
        newB.h = recH;
        return check_collision_rects(A,newB);
    }

    bool check_collision_point_and_rect( int rec1X, int rec1Y, int rec1W, int rec1H, int rec2X, int rec2Y, int rec2W, int rec2H )
    {
        gpe::shape_rect newA;
        newA.x = (int)rec1X;
        newA.y = (int)rec1Y;
        newA.w = rec1W;
        newA.h = rec1H;

        gpe::shape_rect newB;
        newB.x = (int)rec2X;
        newB.y = (int)rec2Y;
        newB.w = rec2W;
        newB.h = rec2H;
        return check_collision_rects(newA,newB);
    }


    bool check_collision_circles( shape_circle A, shape_circle B )
    {
        //Gets the difference between the circle points
        float x_new = A.position.x - B.position.x;
        float y_new = A.position.y - B.position.y;

        float circleSquaredDist = x_new * x_new + y_new * y_new;

        float newRadius = A.radius + B.radius;
        float radiusSquared = newRadius * newRadius;

        //If the squared distance between circles are less than the squared radius
        //They are overlapping.
        return (circleSquaredDist <= radiusSquared);
    }

    //Thanks YellowAfterLife
    bool check_collision_circle_rect( gpe::shape_rect A, shape_circle B )
    {
        int DeltaX = B.position.x - std::max( A.x, std::min(B.position.x, A.x + A.w ) );
        int DeltaY = B.position.y - std::max( A.y, std::min(B.position.y, A.y + A.h ) );
        return (DeltaX * DeltaX + DeltaY * DeltaY) < (B.radius * B.radius);
    }

    bool point_within(int point_x, int point_y,int mxmin,int mymin, int mxmax, int mymax)
    {
        int pxmin, pxmax, pymin, pymax;
        pxmin=std::min(mxmin, mxmax);
        pxmax=std::max(mxmin,mxmax);
        pymin=std::min(mymin,mymax);
        pymax=std::max(mymin,mymax);

        if((point_x>=pxmin) && (point_x<=pxmax) )
        {
            if( (point_y>=pymin) && (point_y<=pymax) )
            {
                return true;
            }
        }
        return false;
    }

    bool point_between(int point_x, int point_y,int mxmin,int mymin, int mxmax, int mymax)
    {
        int pxmin, pxmax, pymin, pymax;
        pxmin=std::min(mxmin, mxmax);
        pxmax=std::max(mxmin,mxmax);
        pymin=std::min(mymin,mymax);
        pymax=std::max(mymin,mymax);

        if((point_x>pxmin) && (point_x<pxmax) )
        {
            if( (point_y>pymin) && (point_y<pymax) )
            {
                return true;
            }
        }
        return false;
    }


    bool point_within_rect(int point_x, int point_y, gpe::shape_rect * rect_in)
    {
        if( rect_in!=nullptr)
        {
            int pxmin, pxmax, pymin, pymax;
            pxmin=rect_in->x;
            pymin=rect_in->y;
            pxmax=pxmin+rect_in->w;
            pymax=pymin+rect_in->h;

            if((point_x>=pxmin) && (point_x<=pxmax) )
            {
                if( (point_y>=pymin) && (point_y<=pymax) )
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool point_between_rect(int point_x, int point_y, gpe::shape_rect * rect_in)
    {
        if( rect_in!=nullptr)
        {
            int pxmin, pxmax, pymin, pymax;
            pxmin=rect_in->x;
            pymin=rect_in->y;
            pxmax=pxmin+rect_in->w;
            pymax=pymin+rect_in->h;
            if((point_x>pxmin) && (point_x<pxmax) )
            {
                if( (point_y>pymin) && (point_y<pymax) )
                {
                    return true;
                }
            }
        }
        return false;
    }

    /*
    bool mouse_btw(int mxmin,int mymin, int mxmax, int mymax)
    {
         return point_within(mouse_position_x,mouse_position_y,mxmin, mymin, mxmax, mymax);
    }
    */
}

