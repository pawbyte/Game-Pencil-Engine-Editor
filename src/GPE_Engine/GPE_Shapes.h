/*
GPE_Shapes.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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

struct GPE_Rect
{
    double x;
    double y;
    double xCenter;
    double x2Pos;
    double yMiddle;
    double y2Pos;
    double w;
    double h;
    GPE_Rect()
    {
        x = 0;
        xCenter = 0;
        y = 0;
        x2Pos = 0;
        yMiddle = 0;
        y2Pos = 0;
        w = 0;
        h = 0;
    }

    void add_x( double xPlus)
    {
        x+=xPlus;
        x2Pos+=xPlus;
        xCenter+=xPlus;
    }

    void add_y( double yPlus)
    {
        y+=yPlus;
        y2Pos+=yPlus;
        yMiddle+=yPlus;
    }

    void copy_rect( GPE_Rect * otherRect )
    {
        x = otherRect->x;
        y = otherRect->y;
        x2Pos = otherRect->x2Pos;
        y2Pos = otherRect->y2Pos;
        w = otherRect->w;
        h = otherRect->h;
        xCenter = otherRect->xCenter;
        yMiddle = otherRect->yMiddle;
    };

    double get_center()
    {
        return xCenter;
    };

    double get_height()
    {
        return h;
    };

    double get_middle()
    {
        return yMiddle;
    };

    double get_width()
    {
        return w;
    };

    double get_x()
    {
        return x;
    };

    double get_x2()
    {
        return x2Pos;
    };

    double get_y()
    {
        return y;
    };

    double get_y2()
    {
        return y2Pos;
    };

    void set_x( double newX)
    {
        x = newX;
        x2Pos = x+w;
        xCenter = newX+w/2;
        xCenter = x+ w/2;
    };

    void set_y( double newY )
    {
        y = newY;
        y2Pos = y+h;
        yMiddle = y+h/2;
    };

    void update_position( double xNew, double yNew)
    {
        x = xNew;
        y = yNew;
        x2Pos = x+w;
        y2Pos = y+h;
        xCenter = x + w/2;
        yMiddle = y+h/2;
    };

    void update_box( double xNew, double yNew, double widNew, double hgtNew)
    {
        update_shape(xNew, yNew, widNew, hgtNew );
    }

    void update_shape( double xNew, double yNew, double widNew, double hgtNew)
    {
        x = xNew;
        y = yNew;
        w = widNew;
        h = hgtNew;

        if( widNew < 0)
        {
            x = xNew-widNew;
            w = abs(widNew);
            /*if( GPE_SETTINGS_IS_DEBUGGING )
            console.log( rectWidth);*/
        }

        if( hgtNew < 0)
        {
            y = yNew-hgtNew;
            h =abs(hgtNew);
            /*if( GPE_SETTINGS_IS_DEBUGGING )
            console.log( rectHeigh);*/
        }
        x2Pos = x+w;
        y2Pos = y+h;

        xCenter = x+w/2;
        yMiddle = y+h/2;
    }

    void update_size( double widNew, double hgtNew)
    {
        w = widNew;
        h = hgtNew;
        if( widNew < 0)
        {
            x = x-widNew;
            w = abs(widNew);
        }

        if( hgtNew < 0)
        {
            y = y-hgtNew;
            h = abs(hgtNew);
        }

        x2Pos = x+w;
        y2Pos = y+h;
        xCenter = x+w/2;
        yMiddle = y+h/2;
    };
};

#endif
