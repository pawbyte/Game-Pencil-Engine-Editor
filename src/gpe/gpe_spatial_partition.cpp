/*
GPE_Spatial_Partition.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_spatial_partition.h"

namespace gpe
{
    spatial_partition::spatial_partition()
    {
        boxArea = new gpe::shape_rect();
        objectsHeld = 0;
        spaceName= "";
        sought_first_object = nullptr;
        sought_second_object = nullptr;
    }

    spatial_partition::~spatial_partition()
    {
        empty_list();
        if( boxArea!=nullptr)
        {
            delete boxArea;
            boxArea = nullptr;
        }
    }

    void spatial_partition::add_object(game_object * newObject)
    {
        if( newObject!=nullptr )
        {
            objectsArray.push_back( newObject );
        }
    }

    void spatial_partition::check_collisions()
    {
        game_object * foundHeldObject = nullptr;
        game_object * secondHeldObject = nullptr;
        int ii = 0, jj = 0, objectsCount = (int)objectsArray.size();
        std::string tempColPairStr = "";
        std::string otherColPairStr = "";
        collision_event * found_collision_event = NULL;
        for( ii = 0;  ii < objectsCount; ii++)
        {
            foundHeldObject = objectsArray[ii];
            if( foundHeldObject!=nullptr )
            {
                for( jj = ii +1;  jj < objectsCount; jj++ )
                {
                    secondHeldObject  = objectsArray[jj];
                    if( secondHeldObject != nullptr)
                    {
                        //Check the two objects
                        tempColPairStr = stg_ex::int_to_string( foundHeldObject->get_id() ) + "-" + stg_ex::int_to_string( secondHeldObject->get_id() );
                        otherColPairStr = stg_ex::int_to_string( secondHeldObject->get_id() ) + "-" + stg_ex::int_to_string( foundHeldObject->get_id() );

                        //If the pair hasn't been found
                        if( spatial_grid_data::collisionsCheckedInFrame.find(tempColPairStr) ==  spatial_grid_data::collisionsCheckedInFrame.end() )
                        {
                            if( foundHeldObject->check_collison_with_object( secondHeldObject ) )
                            {
                                found_collision_event = collision_exist( foundHeldObject, secondHeldObject);
                                if( found_collision_event != NULL )
                                {
                                    found_collision_event->type = collision_event_type::collision_stay;
                                    foundHeldObject->process_collision( secondHeldObject);
                                    secondHeldObject->process_collision( foundHeldObject );
                                }
                                else
                                {
                                    collisions_found.push_back( { foundHeldObject, secondHeldObject,collision_event_type::collision_enter } );
                                    foundHeldObject->process_collision_enter( secondHeldObject);
                                    secondHeldObject->process_collision_enter( foundHeldObject );
                                }
                            }
                            //We add the pair as checked to the giant list
                            spatial_grid_data::collisionsCheckedInFrame[tempColPairStr] = true;
                            spatial_grid_data::collisionsCheckedInFrame[otherColPairStr] = true;
                        }
                    }
                }
            }
        }
    }

    void spatial_partition::check_collisions_with_other( spatial_partition * other)
    {
        if( other == nullptr )
        {
            return;
        }
        game_object * foundHeldObject = nullptr;
        game_object * secondHeldObject = nullptr;
        int ii = 0, jj = 0, objectsCount = (int)objectsArray.size();
        int otherObjectsCount = (int)other->objectsArray.size();
        std::string tempColPairStr = "";
        std::string otherColPairStr = "";
        collision_event * found_collision_event = NULL;
        for( ii = 0;  ii < objectsCount; ii++)
        {
            foundHeldObject = objectsArray[ii];
            if( foundHeldObject!=nullptr )
            {
                for( jj = 0;  jj < otherObjectsCount; jj++ )
                {
                    secondHeldObject  = other->objectsArray[jj];
                    if( secondHeldObject != nullptr)
                    {
                        //Check the two objects
                        tempColPairStr = stg_ex::int_to_string( foundHeldObject->get_id() ) + "-" + stg_ex::int_to_string( secondHeldObject->get_id() );
                        otherColPairStr = stg_ex::int_to_string( secondHeldObject->get_id() ) + "-" + stg_ex::int_to_string( foundHeldObject->get_id() );

                        //If the pair hasn't been found
                        if( spatial_grid_data::collisionsCheckedInFrame.find(tempColPairStr) ==  spatial_grid_data::collisionsCheckedInFrame.end() )
                        {
                            if( foundHeldObject->check_collison_with_object( secondHeldObject ) )
                            {
                                found_collision_event = collision_exist( foundHeldObject, secondHeldObject);
                                if( found_collision_event != NULL )
                                {
                                    found_collision_event->type = collision_event_type::collision_stay;
                                    foundHeldObject->process_collision( secondHeldObject);
                                    secondHeldObject->process_collision( foundHeldObject );
                                }
                                else
                                {
                                    collisions_found.push_back( { foundHeldObject, secondHeldObject,collision_event_type::collision_enter } );
                                    foundHeldObject->process_collision_enter( secondHeldObject);
                                    secondHeldObject->process_collision_enter( foundHeldObject );
                                }
                            }
                            //We add the pair as checked to the giant list
                            spatial_grid_data::collisionsCheckedInFrame[tempColPairStr] = true;
                            spatial_grid_data::collisionsCheckedInFrame[otherColPairStr] = true;
                        }
                    }
                }
            }
        }
    }

    void spatial_partition::check_for_leaving_collisions()
    {
        bool process_leave_collision = false;

        for( int i = (int)collisions_found.size()-1; i >= 0; i-- )
        {
            process_leave_collision = false;
            if( collisions_found[i].type == collision_event_type::collision_leave )
            {
                collisions_found.erase( collisions_found.begin() + i );
                continue;
            }
            if( collisions_found[i].object_one->being_deleted() || collisions_found[i].object_two->being_deleted() )
            {
                process_leave_collision = true;
            }
            else if( collisions_found[i].object_one->check_collison_with_object( collisions_found[i].object_two) == false )
            {
                process_leave_collision = true;
            }

            if( process_leave_collision )
            {
                collisions_found[i].type = collision_event_type::collision_leave;
                collisions_found[i].object_one->process_collision_leave( collisions_found[i].object_two );
                collisions_found[i].object_two->process_collision_leave( collisions_found[i].object_one );
            }
        }
    }

    collision_event * spatial_partition::collision_exist( game_object * object_one, game_object * object_two )
    {
        sought_first_object = object_one;
        sought_first_object = object_two;

        for( int i = (int)collisions_found.size()-1; i >= 0; i-- )
        {
            if( matching_collision( &collisions_found[i] ) )
            {
                return &collisions_found[i];
            }
        }
        return nullptr;
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
        return nullptr;
    }

    void spatial_partition::remove_held_object(game_object * object_in)
    {
        if( object_in==nullptr )
        {
            return;
        }
        game_object * foundHeldObject = nullptr;
        int ii = 0, objectsCount = (int)objectsArray.size();
        for( ii = objectsCount-1;  ii>=0; ii--)
        {
            foundHeldObject = objectsArray[ii];
            if( foundHeldObject!=nullptr && foundHeldObject->get_id()==object_in->get_id() )
            {
                objectsArray.erase( objectsArray.begin()+ii);
                objectsHeld-=1;
            }
        }
    }

    bool spatial_partition::matching_collision( collision_event * col_event )
    {
        if( col_event == nullptr)
        {
            return false;
        }
        if( col_event->object_one->equals(sought_first_object) && col_event->object_two->equals( sought_second_object ) )
        {
            return true;
        }

        if( col_event->object_one->equals(sought_second_object ) && col_event->object_two->equals(  sought_first_object ) )
        {
            return true;
        }
        return false;
    }

    void spatial_partition::set_space(int x_new, int y_new, int newW, int new_height)
    {
        boxArea->update_shape(x_new, y_new, newW, new_height);
    }
}
