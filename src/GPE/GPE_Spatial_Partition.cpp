/*
GPE_Spatial_Partition.cpp
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

#include "GPE_Spatial_Partition.h"

GPE_SpatialPartition::GPE_SpatialPartition()
{
    boxArea = new GPE_Rect();
    objectsHeld = 0;
    spaceName= "";
}

GPE_SpatialPartition::~GPE_SpatialPartition()
{
    empty_list();
    if( boxArea!=NULL)
    {
        delete boxArea;
        boxArea = NULL;
    }
}

void GPE_SpatialPartition::add_object(GPE_GameObject * newObject)
{
    if( newObject!=NULL )
    {
        objectsArray.push_back( newObject );
    }
}

void GPE_SpatialPartition::check_collisions()
{
    GPE_GameObject * foundHeldObject = NULL;
    GPE_GameObject * secondHeldObject = NULL;
    int ii = 0, jj = 0, objectsCount = (int)objectsArray.size();
    std::string tempColPairStr = "";
    std::string otherColPairStr = "";
    for( ii = 0;  ii < objectsCount; ii++)
    {
        foundHeldObject = objectsArray[ii];
        if( foundHeldObject!=NULL )
        {
            for( jj = ii +1;  jj < objectsCount; jj++ )
            {
                secondHeldObject  = objectsArray[jj];
                if( secondHeldObject != NULL)
                {
                    //Check the two objects
                    tempColPairStr = int_to_string( foundHeldObject->get_id() ) + "-" + int_to_string( secondHeldObject->get_id() );
                    otherColPairStr = int_to_string( secondHeldObject->get_id() ) + "-" + int_to_string( foundHeldObject->get_id() );

                    //If the pair hasn't been found
                    if( gpeSpatialGridData::collisionsCheckedInFrame.find(tempColPairStr) ==  gpeSpatialGridData::collisionsCheckedInFrame.end() )
                    {
                        if( foundHeldObject->check_collison_with_object( secondHeldObject ) )
                        {
                            foundHeldObject->collisionList.push_back( secondHeldObject );
                            secondHeldObject->collisionList.push_back( foundHeldObject );
                        }
                        //We add the pair as checked to the giant list
                        gpeSpatialGridData::collisionsCheckedInFrame[tempColPairStr] = true;
                        gpeSpatialGridData::collisionsCheckedInFrame[otherColPairStr] = true;
                    }
                }
            }
        }
    }
}

void GPE_SpatialPartition::check_collisions_with_other( GPE_SpatialPartition * other)
{
    if( other == NULL )
    {
        return;
    }
    GPE_GameObject * foundHeldObject = NULL;
    GPE_GameObject * secondHeldObject = NULL;
    int ii = 0, jj = 0, objectsCount = (int)objectsArray.size();
    int otherObjectsCount = (int)other->objectsArray.size();
    std::string tempColPairStr = "";
    std::string otherColPairStr = "";
    for( ii = 0;  ii < objectsCount; ii++)
    {
        foundHeldObject = objectsArray[ii];
        if( foundHeldObject!=NULL )
        {
            for( jj = 0;  jj < otherObjectsCount; jj++ )
            {
                secondHeldObject  = other->objectsArray[jj];
                if( secondHeldObject != NULL)
                {
                    //Check the two objects
                    tempColPairStr = int_to_string( foundHeldObject->get_id() ) + "-" + int_to_string( secondHeldObject->get_id() );
                    otherColPairStr = int_to_string( secondHeldObject->get_id() ) + "-" + int_to_string( foundHeldObject->get_id() );

                    //If the pair hasn't been found
                    if( gpeSpatialGridData::collisionsCheckedInFrame.find(tempColPairStr) ==  gpeSpatialGridData::collisionsCheckedInFrame.end() )
                    {
                        if( foundHeldObject->check_collison_with_object( secondHeldObject ) )
                        {
                            foundHeldObject->collisionList.push_back( secondHeldObject );
                            secondHeldObject->collisionList.push_back( foundHeldObject );
                        }
                        //We add the pair as checked to the giant list
                        gpeSpatialGridData::collisionsCheckedInFrame[tempColPairStr] = true;
                        gpeSpatialGridData::collisionsCheckedInFrame[otherColPairStr] = true;
                    }
                }
            }
        }
    }
}

void GPE_SpatialPartition::empty_list()
{
    objectsHeld = 0;
    objectsArray.clear();
}

GPE_GameObject * GPE_SpatialPartition::get_object(int pos)
{
    if( pos >=0 && pos < (int)objectsArray.size() )
    {
        return objectsArray[pos];
    }
    return NULL;
}

void GPE_SpatialPartition::remove_held_object(GPE_GameObject * objectIn)
{
    if( objectIn==NULL )
    {
        return;
    }
    GPE_GameObject * foundHeldObject = NULL;
    int ii = 0, objectsCount = (int)objectsArray.size();
    for( ii = objectsCount-1;  ii>=0; ii--)
    {
        foundHeldObject = objectsArray[ii];
        if( foundHeldObject!=NULL && foundHeldObject->get_id()==objectIn->get_id() )
        {
            objectsArray.erase( objectsArray.begin()+ii);
            objectsHeld-=1;
        }
    }
}

void GPE_SpatialPartition::set_space(int newX, int newY, int newW, int newHeight)
{
    boxArea->update_shape(newX, newY, newW, newHeight);
}
