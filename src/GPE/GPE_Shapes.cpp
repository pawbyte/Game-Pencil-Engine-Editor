/*
GPE_Shapes.cpp
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

#include "GPE_Shapes.h"

GPE_Circle::GPE_Circle(float xpos, float ypos, float radiusIn)
{
    x = xpos;
    y = ypos;
    radius = radiusIn;
}

GPE_Circle::~GPE_Circle()
{

}


GPE_Rect::GPE_Rect(float rX, float rY, float rW, float rH)
{
    x = rX;
    xCenter = rX+rW/2;
    x2Pos = rX+rW;
    y = rY;
    yMiddle = rY+rH/2;
    y2Pos = rY+rH;
    y2Pos = 0;
    w = rW;
    h = rH;
}

GPE_Rect::~GPE_Rect()
{

}

void GPE_Rect::add_x( float xPlus)
{
    x+=xPlus;
    x2Pos+=xPlus;
    xCenter+=xPlus;
}

void GPE_Rect::add_y( float yPlus)
{
    y+=yPlus;
    y2Pos+=yPlus;
    yMiddle+=yPlus;
}

void GPE_Rect::copy_rect( GPE_Rect * otherRect )
{
    if( otherRect!=NULL)
    {
        x = otherRect->x;
        y = otherRect->y;
        x2Pos = otherRect->x2Pos;
        y2Pos = otherRect->y2Pos;
        w = otherRect->w;
        h = otherRect->h;
        xCenter = otherRect->xCenter;
        yMiddle = otherRect->yMiddle;
    }
}

float GPE_Rect::get_center()
{
    return xCenter;
}

float GPE_Rect::get_height()
{
    return h;
}

float GPE_Rect::get_middle()
{
    return yMiddle;
}

float GPE_Rect::get_width()
{
    return w;
}

float GPE_Rect::get_x()
{
    return x;
}

float GPE_Rect::get_x2()
{
    return x2Pos;
}

float GPE_Rect::get_y()
{
    return y;
}

float GPE_Rect::get_y2()
{
    return y2Pos;
}

void GPE_Rect::limit_space_to_rect( int * limitedX, int * limitedY )
{
    if( limitedX!=NULL && limitedY!=NULL)
    {
        if( *limitedX < x)
        {
            *limitedX = 0;
        }

        if( *limitedX > x+w)
        {
            *limitedX = x+w;
        }

        if( *limitedY < y)
        {
            *limitedY = 0;
        }

        if( *limitedY < y+h)
        {
            *limitedY = y+h;
        }
    }
}

void GPE_Rect::set_x( float newX)
{
    x = newX;
    x2Pos = x+w;
    xCenter = x+ w/2;
}

void GPE_Rect::set_y( float newY )
{
    y = newY;
    y2Pos = y+h;
    yMiddle = y+h/2;
}

void GPE_Rect::set_w( float newW)
{
    w = newW;
    x2Pos = x+w;
    xCenter = x+ w/2;
}

void GPE_Rect::set_h( float newH )
{
    h = newH;
    y2Pos = y+h;
    yMiddle = y+h/2;
}

void GPE_Rect::update_position( float xNew, float yNew)
{
    x = xNew;
    y = yNew;
    x2Pos = x+w;
    y2Pos = y+h;
    xCenter = x + w/2;
    yMiddle = y+h/2;
}

void GPE_Rect::update_box( float xNew, float yNew, float widNew, float hgtNew)
{
    update_shape(xNew, yNew, widNew, hgtNew );
}

void GPE_Rect::update_shape( float xNew, float yNew, float widNew, float hgtNew)
{
    x = xNew;
    y = yNew;
    w = widNew;
    h = hgtNew;

    if( widNew < 0)
    {
        x = xNew-widNew;
        w = fabs(widNew);
        /*if( GPE_SETTINGS_IS_DEBUGGING )
        console.log( rectWidth);*/
    }

    if( hgtNew < 0)
    {
        y = yNew-hgtNew;
        h = fabs(hgtNew);
        /*if( GPE_SETTINGS_IS_DEBUGGING )
        console.log( rectHeigh);*/
    }
    x2Pos = x+w;
    y2Pos = y+h;

    xCenter = x+w/2;
    yMiddle = y+h/2;
}

void GPE_Rect::update_size( float widNew, float hgtNew)
{
    w = widNew;
    h = hgtNew;
    if( widNew < 0)
    {
        x = x-widNew;
        w = fabs(widNew);
    }

    if( hgtNew < 0)
    {
        y = y-hgtNew;
        h = fabs(hgtNew);
    }

    x2Pos = x+w;
    y2Pos = y+h;
    xCenter = x+w/2;
    yMiddle = y+h/2;
}
