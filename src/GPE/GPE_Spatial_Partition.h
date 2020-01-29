/*
GPE_Spatial_Partition.h
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

#ifndef GPE_SPATIAL_PARTITIONS_H
#define GPE_SPATIAL_PARTITIONS_H

#include "GPE_Basic_Object.h"
#include "GPE_Shapes.h"


class GPE_SpatialPartition
{
protected:
    std::vector<GPE_GameObject * > objectsArray;
    int objectsHeld;
    GPE_Rect * boxArea;
public:
    std::string spaceName;
    GPE_SpatialPartition();
    ~GPE_SpatialPartition();
    void add_object(GPE_GameObject * newObject);
    void check_collisions();
    void check_collisions_with_other( GPE_SpatialPartition * other);
    void empty_list();
    GPE_GameObject * get_object(int pos);
    int get_size();
    void set_space(int newX, int newY, int newW, int newHeight);
    void remove_held_object(GPE_GameObject * objectIn);
};

#endif
