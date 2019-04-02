/*
GPE_Spatial_Patitions.cpp
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

#include "GPE_Spatial_Partitions.h"

GPE_SuperSpatialPartition::GPE_SuperSpatialPartition()
{
    boxArea = new GPE_Rect();
    objectsHeld = 0;
    spaceName= "CAS_X2015_";
}

GPE_SuperSpatialPartition::~GPE_SuperSpatialPartition()
{
    empty_list();
    if( boxArea!=NULL)
    {
        delete boxArea;
        boxArea = NULL;
    }
}

void GPE_SuperSpatialPartition::add_object(GPE_GameObject * newObject)
{
    if( newObject!=NULL )
    {
        int fObjType = newObject->get_object_type();
        if( fObjType >=0 && fObjType < (int)heldObjectsArray.size() )
        {
            internalArray[objectsHeld] = newObject;
            heldObjectsArray[newObject->get_object_type() ]->add_object( newObject );
            objectsHeld+=1;
        }
    }
}

void GPE_SuperSpatialPartition::empty_list()
{
    objectsHeld = 0;
    GPE_GameObject_List *  tempList = NULL;
    for( int i = (int)heldObjectsArray.size()-1; i >=0; i-- )
    {
        tempList = heldObjectsArray[i];
        if( tempList!=NULL)
        {
            delete tempList;
            tempList = NULL;
        }
    }
    heldObjectsArray.clear();
    internalArray.clear();
}

int GPE_SuperSpatialPartition::get_size()
{
    return (int)internalArray.size();
}

GPE_GameObject * GPE_SuperSpatialPartition::get_object(int pos)
{
    if( pos >=0 && pos < (int)internalArray.size() )
    {
        return internalArray[pos];
    }
    return NULL;
}

void GPE_SuperSpatialPartition::remove_held_object(GPE_GameObject * objectIn)
{
    if( objectIn!=NULL )
    {
        int fObjType = objectIn->get_object_type();
        GPE_GameObject * foundHeldObject = NULL;
        for( int ii = (int)internalArray.size()-1;  ii>=0; ii--)
        {
            foundHeldObject = internalArray[ii];
            if( foundHeldObject->get_id()==objectIn->get_id() )
            {
                internalArray.erase( internalArray.begin()+ii);
                objectsHeld-=1;
            }
        }
        if( fObjType >=0 && fObjType < (int)heldObjectsArray.size() )
        {
            GPE_GameObject_List * subHeldArray = heldObjectsArray[ fObjType ];
            subHeldArray->remove_held_object( objectIn );
        }
    }
}

void GPE_SuperSpatialPartition::set_space(int newX, int newY, int newW, int newHeight)
{
    boxArea->update_shape(newX, newY, newW, newHeight);
}
