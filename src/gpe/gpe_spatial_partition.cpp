/*
GPE_Spatial_Partition.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#include "GPE_Spatial_Partition.h"

namespace gpe
{
    spatial_partition::spatial_partition()
    {
        boxArea = new gpe::shape_rect();
        objectsHeld = 0;
        spaceName= "";
    }

    spatial_partition::~spatial_partition()
    {
        empty_list();
        if( boxArea!=NULL)
        {
            delete boxArea;
            boxArea = NULL;
        }
    }

    void spatial_partition::add_object(game_object * newObject)
    {
        if( newObject!=NULL )
        {
            objectsArray.push_back( newObject );
        }
    }

    void spatial_partition::check_collisions()
    {
        game_object * foundHeldObject = NULL;
        game_object * secondHeldObject = NULL;
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
                        tempColPairStr = stg_ex::int_to_string( foundHeldObject->get_id() ) + "-" + stg_ex::int_to_string( secondHeldObject->get_id() );
                        otherColPairStr = stg_ex::int_to_string( secondHeldObject->get_id() ) + "-" + stg_ex::int_to_string( foundHeldObject->get_id() );

                        //If the pair hasn't been found
                        if( gpeSpatialGridData::collisionsCheckedInFrame.find(tempColPairStr) ==  gpeSpatialGridData::collisionsCheckedInFrame.end() )
                        {
                            if( foundHeldObject->check_collison_with_object( secondHeldObject ) )
                            {
                                foundHeldObject->add_collision_object( secondHeldObject );
                                secondHeldObject->add_collision_object( foundHeldObject );
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

    void spatial_partition::check_collisions_with_other( spatial_partition * other)
    {
        if( other == NULL )
        {
            return;
        }
        game_object * foundHeldObject = NULL;
        game_object * secondHeldObject = NULL;
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
                        tempColPairStr = stg_ex::int_to_string( foundHeldObject->get_id() ) + "-" + stg_ex::int_to_string( secondHeldObject->get_id() );
                        otherColPairStr = stg_ex::int_to_string( secondHeldObject->get_id() ) + "-" + stg_ex::int_to_string( foundHeldObject->get_id() );

                        //If the pair hasn't been found
                        if( gpeSpatialGridData::collisionsCheckedInFrame.find(tempColPairStr) ==  gpeSpatialGridData::collisionsCheckedInFrame.end() )
                        {
                            if( foundHeldObject->check_collison_with_object( secondHeldObject ) )
                            {
                                foundHeldObject->add_collision_object( secondHeldObject );
                                secondHeldObject->add_collision_object( foundHeldObject );
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

    void spatial_partition::empty_list()
    {
        objectsHeld = 0;
        objectsArray.clear();
    }

    game_object * spatial_partition::get_object(int pos)
    {
        if( pos >=0 && pos < (int)objectsArray.size() )
        {
            return objectsArray[pos];
        }
        return NULL;
    }

    void spatial_partition::remove_held_object(game_object * objectIn)
    {
        if( objectIn==NULL )
        {
            return;
        }
        game_object * foundHeldObject = NULL;
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

    void spatial_partition::set_space(int newX, int newY, int newW, int newHeight)
    {
        boxArea->update_shape(newX, newY, newW, newHeight);
    }
}
