/*
GPE_Collisons.h
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

#ifndef COLLISION_CHECKS_H
#define COLLISION_CHECKS_H

//The headers
#include <algorithm>
#include "GPE_CIncludes.h"
#include "GPE_Renderer.h"

bool check_collision( GPE_Rect A, GPE_Rect B );
bool check_collision( GPE_Rect A, int recX, int recY, int recW, int recH  );
bool check_collision( int recX, int recY, int recW, int recH ,GPE_Rect A);
bool check_collision( int rec1X, int rec1Y, int rec1W, int rec1H, int rec2X, int rec2Y, int rec2W, int rec2H);
bool point_within(int pointX, int pointY,int mxmin,int mymin, int mxmax, int mymax);
bool point_between(int pointX, int pointY,int mxmin,int mymin, int mxmax, int mymax);
bool point_between_rect(int pointX, int pointY, GPE_Rect * rectIn= NULL);
bool point_within_rect(int pointX, int pointY, GPE_Rect * rectIn= NULL);
//bool mouse_btw(int mxmin,int mymin, int mxmax, int mymax);

#endif


