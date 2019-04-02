/*
GPE_Spatial_Patitions.h
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

#ifndef GPE_SPATIAL_PARTITIONS_H
#define GPE_SPATIAL_PARTITIONS_H

#include "GPE_Basic_Object.h"
#include "GPE_Shapes.h"


class GPE_SuperSpatialPartition
{
private:
    std::vector<GPE_GameObject * > internalArray;
    std::vector<GPE_GameObject_List* >heldObjectsArray;
    int objectsHeld;
    GPE_Rect * boxArea;
public:
    std::string spaceName;
    GPE_SuperSpatialPartition();
    ~GPE_SuperSpatialPartition();
    void add_object(GPE_GameObject * newObject);
    void empty_list();
    int get_size();
    GPE_GameObject * get_object(int pos);
    void set_space(int newX, int newY, int newW, int newHeight);
    void remove_held_object(GPE_GameObject * objectIn);
};

#endif
