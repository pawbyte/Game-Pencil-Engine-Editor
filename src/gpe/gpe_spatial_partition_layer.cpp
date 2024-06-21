/*
spatial_partition_layer.cpp
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

#include "gpe_spatial_partition_layer.h"

namespace gpe
{
    spatial_partition_layer::spatial_partition_layer( int myLayerId )
    {
        spatial_grid_created = false;
        layerID = myLayerId;
    }

    spatial_partition_layer::~spatial_partition_layer()
    {

    }

    void spatial_partition_layer::activate_layer()
    {
        if( !isActive)
        {
            clear_spaces();
            init_collision_handler( spatial_grid_width_size * spatial_grid_x_amount, spatial_grid_height_size * spatial_grid_y_amount);
        }
        isActive = true;
    }

    void spatial_partition_layer::add_object( game_object * gameObject )
    {
        int partitionsSize = (int)layerPartitions.size();
        if( gameObject == nullptr || partitionsSize == 0 )
        {
            return;
        }
        set_game_object_space_id( gameObject,  layerID );
        int objectSpace = find_spatial_at( gameObject->getx(), gameObject->gety() );
        if( objectSpace < 0 )
        {
            objectSpace = 0;
        }
        else if( objectSpace >= partitionsSize )
        {
            objectSpace = partitionsSize -1;
        }
        layerPartitions[ objectSpace]->add_object( gameObject );

    }

    void spatial_partition_layer::check_collisions_self()
    {
        spatial_partition * tempPartition = nullptr;
        spatial_partition * tempPartitionLeft = nullptr;
        spatial_partition * tempPartitionAbove = nullptr;
        spatial_partition * tempPartitionAboveLeft = nullptr;

        int collisionSpaceCount = (int)layerPartitions.size();

        int tempLeftPos = -1;
        int tempAbovePos = -1;
        int tempAboveLeftPos = -1;
        for( int i = 0; i < collisionSpaceCount; i++ )
        {
            tempPartition = layerPartitions[i];
            if( tempPartition!=nullptr )
            {
                //Check all of the collisions within itself
                tempPartition->check_collisions();

                //Now look and see if it can check with 3 neighbors..
                tempLeftPos = i -1;
                if( tempLeftPos > 0 && tempLeftPos < collisionSpaceCount)
                {
                    tempPartitionLeft = layerPartitions[tempLeftPos];
                    if( tempPartitionLeft !=nullptr )
                    {
                        tempPartition->check_collisions_with_other( tempPartitionLeft );
                    }
                }

                tempAbovePos = i - spatial_grid_data::sGridYAmount;
                if( tempAbovePos > 0 && tempAbovePos < collisionSpaceCount)
                {
                    tempPartitionAbove = layerPartitions[tempAbovePos];
                    if( tempPartitionAbove !=nullptr )
                    {
                        tempPartition->check_collisions_with_other( tempPartitionAbove );
                    }
                }

                tempAboveLeftPos = i - spatial_grid_data::sGridYAmount -1;
                if( tempAboveLeftPos > 0 && tempAboveLeftPos < collisionSpaceCount)
                {
                    tempPartitionAboveLeft = layerPartitions[tempAboveLeftPos];
                    if( tempPartitionAboveLeft !=nullptr )
                    {
                        tempPartition->check_collisions_with_other( tempPartitionAboveLeft );
                    }
                }
            }
        }
    }

    void spatial_partition_layer::check_collisions_with_layer( spatial_partition_layer * other_layer,  bool execute_collisions)
    {
        if(execute_collisions == false )
        {
            return;
        }

        if(other_layer == nullptr)
        {
            return;
        }

        int i = 0, j = 0, max_i = 0, max_j = 0;
        if( (int)layerPartitions.size() == (int)other_layer->layerPartitions.size() )
        {
            max_i = max_j = (int)layerPartitions.size();
        }
        else
        {
            max_i = (int)layerPartitions.size();
            max_j = other_layer->layerPartitions.size();
        }

        for( i = 0; i < max_i; i++ )
        {
            if( layerPartitions[i] == nullptr )
            {
                continue;
            }

            for( j = 0; j < max_j; j++)
            {
                if( other_layer->layerPartitions[j] == nullptr )
                {
                    continue;
                }
                layerPartitions[i]->check_collisions_with_other(  other_layer->layerPartitions[j] );
            }
        }
    }


    game_object * spatial_partition_layer::check_objects_circle(gpe::shape_circle * checked_circle, int object_type, bool check_for_children  )
    {
        if( checked_circle == nullptr )
        {
            return nullptr;
        }
        game_object *  temp_found_obj = nullptr;
        for( int i = (int)layerPartitions.size() -1; i >=0; i-- )
        {
            if( layerPartitions[i]->collides_with_circle( checked_circle) )
            {
                temp_found_obj = layerPartitions[i]->check_objects_circle( checked_circle, object_type, check_for_children );

                if( temp_found_obj != nullptr )
                {
                    return temp_found_obj;
                }
            }
        }
        return nullptr;
    }

    bool spatial_partition_layer::check_objects_circle_all(gpe::shape_circle * checked_circle, int object_type, std::vector<game_object *>& obj_vector, bool check_for_children  )
    {
        if( checked_circle == nullptr )
        {
            return false;
        }

        bool  found_an_object = false;
        for( int i = (int)layerPartitions.size() -1; i >=0; i-- )
        {
            if( layerPartitions[i]->collides_with_circle( checked_circle) )
            {
                if( layerPartitions[i]->check_objects_circle_all( checked_circle, object_type,obj_vector, check_for_children ) )
                {
                    found_an_object = true;
                }
            }
        }
        return found_an_object;
    }

     game_object * spatial_partition_layer::check_objects_point( gpe::shape_point2d * checked_point, int object_type, bool check_for_children  )
     {
        if( checked_point == nullptr )
        {
            return nullptr;
        }
        int found_spatial = find_spatial_at(checked_point->x, checked_point->y);
        if( found_spatial == is_null )
        {
            return nullptr;
        }
        return layerPartitions[found_spatial]->check_objects_point( checked_point, object_type, check_for_children );
     }

    bool spatial_partition_layer::check_objects_point_all( gpe::shape_point2d * checked_point, int object_type, std::vector<game_object *>& obj_vector, bool check_for_children  )
    {
        if( checked_point == nullptr )
        {
            return false;
        }
        int found_spatial = find_spatial_at(checked_point->x, checked_point->y );
        if( found_spatial == is_null )
        {
            return false;
        }
        return layerPartitions[found_spatial]->check_objects_point_all( checked_point, object_type,obj_vector, check_for_children );
    }

    game_object * spatial_partition_layer::check_objects_rectangle( gpe::shape_rect * checked_rect, int object_type, bool check_for_children  )
    {
        if( checked_rect == nullptr )
        {
            return nullptr;
        }


        game_object *  temp_found_obj = nullptr;
        for( int i = (int)layerPartitions.size() -1; i >=0; i-- )
        {
            if( layerPartitions[i]->collides_with_rectangle( checked_rect) )
            {
                temp_found_obj = layerPartitions[i]->check_objects_rectangle( checked_rect, object_type, check_for_children );

                if( temp_found_obj != nullptr )
                {
                    return temp_found_obj;
                }
            }
        }
        return nullptr;
    }

    bool spatial_partition_layer::check_objects_rectangle_all( gpe::shape_rect * checked_rect, int object_type, std::vector<game_object *>& obj_vector, bool check_for_children  )
    {
        if( checked_rect == nullptr )
        {
            return false;
        }
        bool  found_an_object = false;
        for( int i = (int)layerPartitions.size() -1; i >=0; i-- )
        {
            if( layerPartitions[i]->collides_with_rectangle( checked_rect) )
            {
                if( layerPartitions[i]->check_objects_rectangle_all( checked_rect, object_type,obj_vector, check_for_children ) )
                {
                    found_an_object = true;
                }
            }
        }
        return found_an_object;
    }


    void spatial_partition_layer::check_for_leaving_collisions( bool execute_collisions)
    {
        spatial_partition * tempPartition = nullptr;
        for( int i = (int)layerPartitions.size()-1; i >=0 ; i-- )
        {
            tempPartition = layerPartitions[i];
            if( tempPartition!=nullptr )
            {
                tempPartition->check_for_leaving_collisions( execute_collisions);
            }
        }
    }

    void spatial_partition_layer::clear_spaces( )
    {
        spatial_partition *  tempSpace = nullptr;
        for( int i = (int)layerPartitions.size()-1; i >=0; i-- )
        {
            tempSpace = layerPartitions[i];
            if( tempSpace!=nullptr)
            {
                delete tempSpace;
                tempSpace = nullptr;
            }
        }
        layerPartitions.clear();
        spatial_grid_created = false;
    }

    void spatial_partition_layer::deactivate_layer()
    {
        if( isActive)
        {
            clear_spaces();
        }
        isActive = false;
    }



    int spatial_partition_layer::find_spatial_at ( int xIn, int yIn)
    {
        int sPar = is_null;
        if( xIn < 0 )
        {
            xIn = 0;
        }

        if( yIn < 0)
        {
            yIn = 0;
        }

        if( xIn >=0 && yIn >=0 )
        {
            sPar =  (( xIn / spatial_grid_width_size)|0 )*spatial_grid_y_amount;
            sPar+=( yIn / spatial_grid_height_size)|0;
            if( sPar < 0 )
            {
                sPar = 0;
            }
            else if( sPar >= (int)layerPartitions.size() )
            {
                sPar = (int)layerPartitions.size() - 1;
            }
        }
        return sPar;
    }

    void spatial_partition_layer::init_collision_handler(  int camera_scene_width, int camera_scene_height )
    {

        spatial_grid_width_size = camera_scene_width;
        spatial_grid_height_size = camera_scene_height;

        spatial_grid_x_amount = ceil(camera_scene_width/spatial_grid_width_size);
        if( spatial_grid_x_amount<2)
        {
            spatial_grid_width_size = camera_scene_width/2;
            spatial_grid_x_amount = 2;
        }
        spatial_grid_y_amount = ceil(camera_scene_height/spatial_grid_height_size);
        if( spatial_grid_y_amount<2 )
        {
            spatial_grid_height_size = camera_scene_height/2;
            spatial_grid_y_amount = 2;
        }


        //smallest size possible is 2x2
        int iGrid, jGrid;
        int x_start=0, y_start=0;
        spatial_partition * newSpace = nullptr;
        for( iGrid =0; iGrid < spatial_grid_x_amount; iGrid+=1)
        {
            for( jGrid=0; jGrid < spatial_grid_y_amount; jGrid+=1)
            {
                newSpace = new spatial_partition();
                newSpace->set_space(x_start,y_start,spatial_grid_width_size,spatial_grid_height_size);
                y_start+=spatial_grid_height_size;
                newSpace->spaceName+="testrun_"+ stg_ex::int_to_string(iGrid)+"_"+ stg_ex::int_to_string(jGrid)+"_test";
                layerPartitions.push_back(newSpace);
            }
            y_start = 0;
            x_start+=spatial_grid_width_size;
        }
        spatial_grid_created = true;
    }

    bool spatial_partition_layer::is_active()
    {
        return isActive;
    }

    void spatial_partition_layer::remove_object( game_object * gameObject )
    {
        if( gameObject == nullptr)
        {
            return;
        }
    }

    void spatial_partition_layer::set_game_object_space_id( game_object * g_object, int n_space_id, bool updated_on_grid  )
    {
        if( g_object == nullptr )
        {
            return;
        }

        g_object->collision_system_data[collision_system_partion_next] = n_space_id;
        if( updated_on_grid )
        {
            g_object->collision_system_data[collision_system_partion_index] = n_space_id;
        }
    }
}
