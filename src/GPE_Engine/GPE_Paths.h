/*
GPE_Paths.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef GPE_PATHS_H
#define GPE_PATHS_H

#include "GPE_CIncludes.h"


class GPE_PathPoint
{
public:

    double xPos;
    double yPos;
    double pointSpeed;
    GPE_PathPoint(int pointX, int pointY, double speed = 1);
    ~GPE_PathPoint();
};

class GPE_GamePath
{
private:
    int pathId;
    bool isClosedPath;
    std::vector< GPE_PathPoint * > pathPoints;
    std::vector< GPE_PathPoint * > pathOriginalPoints;
    int pathLength;
    int originalPathLength;
public:
    GPE_GamePath(int pId, bool pIsClosed);
    ~GPE_GamePath();
};

#endif
