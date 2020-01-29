/*
GPE_Tilesheet.cpp
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

#include "GPE_Tilesheet.h"

GPE_Tilesheet::GPE_Tilesheet()
{
    tsId = -1;
    tsImage = NULL;
    tsWidth = 0;
    tsHeight = 0;
    tsXOff = 0;
    tsYOff = 0;
    tsXPadding = 0;
    tsYPadding = 0;
    tilesPerRow= 0;
    tilesPerCol = 0;
    name = "";
    fileNameLocation = "";
    isPreloaded = true;
}

GPE_Tilesheet::~GPE_Tilesheet()
{
    tsRects.clear();
    if( tsImage!=NULL)
    {
        delete tsImage;
        tsImage = NULL;
    }
}

void GPE_Tilesheet::organize_tilesheet()
{
    if( tsImage!=NULL)
    {
        tsRects.clear();
        GPE_Rect tempRect;
        if( tsWidth > 0 && tsHeight > 0 && tsXPadding>=0 && tsYPadding>=0 )
        {
            int startX = tsXOff;
            int startY = tsYOff;
            tilesPerRow= 0;
            tilesPerCol = 0;
            tempRect.x = startX;
            tempRect.y = startY;
            tempRect.w = tsWidth;
            tempRect.h = tsHeight;
            for( int i = startY; i < tsImage->get_height(); i+=tsHeight+tsYPadding)
            {
                tilesPerCol++;
                tilesPerRow = 0;
                for( int j = startX; j < tsImage->get_width(); j+=tsWidth+tsXPadding)
                {
                    tilesPerRow++;
                    tempRect.x = j;
                    tempRect.y = i;
                    tempRect.w = tsWidth;
                    tempRect.h = tsHeight;
                    tsRects.push_back(tempRect);
                }
            }
        }
        else
        {
            tempRect.x = 0;
            tempRect.y = 0;
            tempRect.w = tsImage->get_width();
            tempRect.h = tsImage->get_height();
        }
    }
}


