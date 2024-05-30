/*
gpe_spatial_partition_controller.cpp
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

#include "gpe_spatial_partition_controller.h"

namespace gpe
{
    spatial_partition_controller * main_spatial_grid = nullptr;

    spatial_partition_controller::spatial_partition_controller( std::string c_name )
    {
        controller_name = c_name;
        controller_type = "gpe_spatial_partition_controller";
        for( int i = 0; i < max_collision_layer_count; i++ )
        {
            collisionlayers[i] = new spatial_partition_layer( i );
        }
    }

    spatial_partition_controller::~spatial_partition_controller()
    {
        spatial_partition_layer * tempLayer = nullptr;
        for( int i = max_collision_layer_count -1; i >=0 ; i--)
        {
            tempLayer = collisionlayers[i];
            if( tempLayer!=nullptr)
            {
                delete tempLayer;
                tempLayer = nullptr;
            }
            collisionlayers[i] = nullptr;
        }
        clear_checked_map();
    }


    void spatial_partition_controller::activate_layer(int layer_id )
    {
        if( layer_id < 0 || layer_id  >= max_collision_layer_count )
        {
            return;
        }
        spatial_partition_layer * tempLayer = collisionlayers[layer_id];
        tempLayer->activate_layer();

    }

    void spatial_partition_controller::activate_all_layers()
    {
        spatial_partition_layer * tempLayer = nullptr;
        for( int i = max_collision_layer_count -1; i >=0 ; i--)
        {
            tempLayer = collisionlayers[i];
            if( tempLayer!=nullptr)
            {
                tempLayer->activate_layer();
            }
        }
    }

    void spatial_partition_controller::clear_checked_map()
    {

    }

    void spatial_partition_controller::add_object( game_object * g_obj , int layer_id )
    {
        if( g_obj == nullptr)
        {
            return;
        }
        if( layer_id < 0 )
        {
            layer_id =0;
        }
        else if( layer_id >= max_collision_layer_count )
        {
            layer_id = max_collision_layer_count -1;
        }

        if( collisionlayers[layer_id] == nullptr)
        {
            return;
        }

        collisionlayers[layer_id]->add_object( g_obj );
    }

    void spatial_partition_controller::clear_spaces()
    {
        spatial_partition_layer * tempLayer = nullptr;
        for( int iLayer = 0; iLayer < max_collision_layer_count; iLayer++ )
        {
            tempLayer = collisionlayers[iLayer];

            if( tempLayer!=nullptr )
            {
                tempLayer->clear_spaces();
            }
        }
    }

    void spatial_partition_controller::check_collisions()
    {
        clear_checked_map();
        int iLayer, jLayer = 0;
        //loop through every active layer
        spatial_partition_layer * tempLayerRow = nullptr;
        spatial_partition_layer * tempLayerColumn = nullptr;
        for( iLayer = 0; iLayer < max_collision_layer_count; iLayer++ )
        {
            tempLayerRow = collisionlayers[iLayer];

            //If the row is active
            if( tempLayerRow->is_active() )
            {
                if( collision_layer_grid[iLayer][iLayer] )
                {
                    tempLayerRow->check_collisions_self();
                }
                //Our for loop starts at iLayer, to avoid double-calculating
                for( jLayer = iLayer; jLayer < max_collision_layer_count; jLayer++ )
                {
                    tempLayerColumn = collisionlayers[jLayer];
                    //if the current column is active
                    if( tempLayerRow->is_active() )
                    {
                        //We have the row check collision with it
                        tempLayerRow->check_collisions_with_layer(tempLayerColumn,collision_layer_grid[iLayer][jLayer] );
                    }
                }
            }
        }
    }

    void spatial_partition_controller::deactivate_all_layers()
    {
        spatial_partition_layer * tempLayer = nullptr;
        for( int i = max_collision_layer_count -1; i >=0 ; i--)
        {
            tempLayer = collisionlayers[i];
            if( tempLayer!=nullptr)
            {
                tempLayer->deactivate_layer();
            }
        }
    }

    void spatial_partition_controller::deactivate_layer(int layer_id )
    {
        if( layer_id < 0 || layer_id  >= max_collision_layer_count )
        {
            return;
        }
        spatial_partition_layer * tempLayer = collisionlayers[layer_id];
        tempLayer->deactivate_layer();
    }

    bool spatial_partition_controller::detect_objects_circle(int layer_id,int x, int y, int r, int object_type )
    {
        return false;
    }

    bool spatial_partition_controller::detect_objects_rectangle(int layer_id,int x, int y, int w, int h, int object_type )
    {
        return false;
    }

    void spatial_partition_controller::end_frame()
    {

    }

    bool spatial_partition_controller::init_system(  int camera_scene_width, int camera_scene_height )
    {
        spatial_partition_layer * tempLayer = nullptr;
        for( int i = max_collision_layer_count -1; i >=0 ; i--)
        {
            tempLayer = collisionlayers[i];
            if( tempLayer!=nullptr)
            {
                tempLayer->init_collision_handler( camera_scene_width, camera_scene_height );
            }
            else
            {
                error_log->report("Spatial Partition Controller Error - Unable to create Layer["+stg_ex::int_to_string(i)+"]" );
                return false;
            }
        }
        return true;
    }


    bool spatial_partition_controller::load_collision_grid( std::string map_file )
    {
        return false;
    }

    bool spatial_partition_controller::quit_system()
    {
        return true;
    }

    void spatial_partition_controller::remove_object( game_object * g_obj )
    {
        if( g_obj == nullptr)
        {
            return;
        }
        int layer_id = g_obj->get_layer_id();
        if( layer_id < 0 )
        {
            layer_id = 0;
        }
        else if( layer_id >= max_collision_layer_count )
        {
            layer_id = max_collision_layer_count -1;
        }

        if( collisionlayers[layer_id] == nullptr)
        {
            return;
        }

        collisionlayers[layer_id]->remove_object( g_obj );
    }

    void spatial_partition_controller::reset_system()
    {

    }

    void spatial_partition_controller::start_frame()
    {
        spatial_partition_layer * tempLayerRow = nullptr;
        for( int i_layer = 0; i_layer < max_collision_layer_count; i_layer++ )
        {
            tempLayerRow = collisionlayers[i_layer];

            //If the row is active
            if( tempLayerRow->is_active() )
            {
                tempLayerRow->check_for_leaving_collisions( collision_layer_grid[i_layer][i_layer] );
            }
        }
    }
}
