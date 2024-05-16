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

    void spatial_partition_layer::check_collisions_with_layer( spatial_partition_layer * otherLayer)
    {

    }

    void spatial_partition_layer::check_for_leaving_collisions()
    {
        spatial_partition * tempPartition = nullptr;
        for( int i = (int)layerPartitions.size()-1; i >=0 ; i-- )
        {
            tempPartition = layerPartitions[i];
            if( tempPartition!=nullptr )
            {
                tempPartition->check_for_leaving_collisions();
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
