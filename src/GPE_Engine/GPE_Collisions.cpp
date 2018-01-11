/*
GPE_Collisions.h.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

#include "GPE_Collisions.h"


bool check_collision( GPE_Rect A, GPE_Rect B)
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

bool check_collision( GPE_Rect A, int recX, int recY, int recW, int recH )
{
    GPE_Rect newB;
    newB.x = (Sint16)recX;
    newB.y = (Sint16)recY;
    newB.w = (Uint16)recW;
    newB.h = (Uint16)recH;
    return check_collision(A,newB);
}

bool check_collision(  int recX, int recY, int recW, int recH ,GPE_Rect A)
{
    GPE_Rect newB;
    newB.x = (Sint16)recX;
    newB.y = (Sint16)recY;
    newB.w = (Uint16)recW;
    newB.h = (Uint16)recH;
    return check_collision(A, newB);
}

bool check_collision( int rec1X, int rec1Y, int rec1W, int rec1H, int rec2X, int rec2Y, int rec2W, int rec2H )
{
    GPE_Rect newA;
    newA.x = (Sint16)rec1X;
    newA.y = (Sint16)rec1Y;
    newA.w = (Uint16)rec1W;
    newA.h = (Uint16)rec1H;

    GPE_Rect newB;
    newB.x = (Sint16)rec2X;
    newB.y = (Sint16)rec2Y;
    newB.w = (Uint16)rec2W;
    newB.h = (Uint16)rec2H;
    return check_collision(newA,newB);
}


bool point_within(int pointX, int pointY,int mxmin,int mymin, int mxmax, int mymax)
{
    int pxmin, pxmax, pymin, pymax;
    pxmin=std::min(mxmin, mxmax);
    pxmax=std::max(mxmin,mxmax);
    pymin=std::min(mymin,mymax);
    pymax=std::max(mymin,mymax);

    if((pointX>=pxmin) && (pointX<=pxmax) )
    {
        if( (pointY>=pymin) && (pointY<=pymax) )
        {
            return true;
        }
    }
    return false;
}

bool point_between(int pointX, int pointY,int mxmin,int mymin, int mxmax, int mymax)
{
    int pxmin, pxmax, pymin, pymax;
    pxmin=std::min(mxmin, mxmax);
    pxmax=std::max(mxmin,mxmax);
    pymin=std::min(mymin,mymax);
    pymax=std::max(mymin,mymax);

    if((pointX>pxmin) && (pointX<pxmax) )
    {
        if( (pointY>pymin) && (pointY<pymax) )
        {
            return true;
        }
    }
    return false;
}


bool point_within_rect(int pointX, int pointY,GPE_Rect * rectIn)
{
    if( rectIn!=NULL)
    {
        int pxmin, pxmax, pymin, pymax;
        pxmin=rectIn->x;
        pymin=rectIn->y;
        pxmax=pxmin+rectIn->w;
        pymax=pymin+rectIn->h;

        if((pointX>=pxmin) && (pointX<=pxmax) )
        {
            if( (pointY>=pymin) && (pointY<=pymax) )
            {
                return true;
            }
        }
    }
    return false;
}

bool point_between_rect(int pointX, int pointY,GPE_Rect * rectIn)
{
    if( rectIn!=NULL)
    {
        int pxmin, pxmax, pymin, pymax;
        pxmin=rectIn->x;
        pymin=rectIn->y;
        pxmax=pxmin+rectIn->w;
        pymax=pymin+rectIn->h;
        if((pointX>pxmin) && (pointX<pxmax) )
        {
            if( (pointY>pymin) && (pointY<pymax) )
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
     return point_within(mouse_x,mouse_y,mxmin, mymin, mxmax, mymax);
}
*/


