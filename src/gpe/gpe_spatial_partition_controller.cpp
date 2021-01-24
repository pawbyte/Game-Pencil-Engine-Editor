/*
gpe_spatial_partition_controller.cpp
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

#include "gpe_spatial_partition_controller.h"

namespace gpe
{
    spatial_partition_controller * main_spatial_grid = NULL;

    spatial_partition_controller::spatial_partition_controller( std::string c_name )
    {
        controller_name = c_name;
        controller_type = "gpe_spatial_partition_controller";
        for( int i = 0; i < maxLayerCount; i++ )
        {
            collisionlayers[i] = new spatial_partition_layer( i );
        }
    }

    spatial_partition_controller::~spatial_partition_controller()
    {
        spatial_partition_layer * tempLayer = NULL;
        for( int i = maxLayerCount -1; i >=0 ; i--)
        {
            tempLayer = collisionlayers[i];
            if( tempLayer!=NULL)
            {
                delete tempLayer;
                tempLayer = NULL;
            }
            collisionlayers[i] = NULL;
        }
        clear_checked_map();
    }


    void spatial_partition_controller::activate_layer(int layerId )
    {
        if( layerId < 0 || layerId  >= maxLayerCount )
        {
            return;
        }
        spatial_partition_layer * tempLayer = collisionlayers[layerId];
        tempLayer->activate_layer();

    }

    void spatial_partition_controller::activate_all_layers()
    {
        spatial_partition_layer * tempLayer = NULL;
        for( int i = maxLayerCount -1; i >=0 ; i--)
        {
            tempLayer = collisionlayers[i];
            if( tempLayer!=NULL)
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
        if( g_obj == NULL)
        {
            return;
        }
        if( layer_id < 0 )
        {
            layer_id =0;
        }
        else if( layer_id >= maxLayerCount )
        {
            layer_id = maxLayerCount -1;
        }

        if( collisionlayers[layer_id] == NULL)
        {
            return;
        }

        collisionlayers[layer_id]->add_object( g_obj );
    }

    void spatial_partition_controller::clear_spaces()
    {
        spatial_partition_layer * tempLayer = NULL;
        for( int iLayer = 0; iLayer < maxLayerCount; iLayer++ )
        {
            tempLayer = collisionlayers[iLayer];

            if( tempLayer!=NULL )
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
        spatial_partition_layer * tempLayerRow = NULL;
        spatial_partition_layer * tempLayerColumn = NULL;
        for( iLayer = 0; iLayer < maxLayerCount; iLayer++ )
        {
            tempLayerRow = collisionlayers[iLayer];

            //If the row is active
            if( tempLayerRow->is_active() )
            {
                tempLayerRow->check_collisions_self();

                //Our for loop starts at iLayer, to avoid double-calculating
                for( jLayer = iLayer; jLayer < maxLayerCount; jLayer++ )
                {
                    tempLayerColumn = collisionlayers[jLayer];
                    //if the current column is active
                    if( tempLayerRow->is_active() )
                    {
                        //We have the row check collision with it
                        tempLayerRow->check_collisions_with_layer(tempLayerColumn);
                    }
                }
            }
        }
    }

    void spatial_partition_controller::deactivate_all_layers()
    {
        spatial_partition_layer * tempLayer = NULL;
        for( int i = maxLayerCount -1; i >=0 ; i--)
        {
            tempLayer = collisionlayers[i];
            if( tempLayer!=NULL)
            {
                tempLayer->deactivate_layer();
            }
        }
    }

    void spatial_partition_controller::deactivate_layer(int layerId )
    {
        if( layerId < 0 || layerId  >= maxLayerCount )
        {
            return;
        }
        spatial_partition_layer * tempLayer = collisionlayers[layerId];
        tempLayer->deactivate_layer();
    }

    bool spatial_partition_controller::init_system(  int cSceneWidth, int cSceneHeight )
    {
        spatial_partition_layer * tempLayer = NULL;
        for( int i = maxLayerCount -1; i >=0 ; i--)
        {
            tempLayer = collisionlayers[i];
            if( tempLayer!=NULL)
            {
                tempLayer->init_collision_handler( cSceneWidth, cSceneHeight );
            }
            else
            {
                error_log->report("Spatial Partition Controller Error - Unable to create Layer["+stg_ex::int_to_string(i)+"]" );
                return false;
            }
        }
        return true;
    }

    bool spatial_partition_controller::quit_system()
    {
        return true;
    }

    void spatial_partition_controller::remove_object( game_object * g_obj )
    {
        if( g_obj == NULL)
        {
            return;
        }
        int layerId = g_obj->get_layer_id();
        if( layerId < 0 )
        {
            layerId = 0;
        }
        else if( layerId >= maxLayerCount )
        {
            layerId = maxLayerCount -1;
        }

        if( collisionlayers[layerId] == NULL)
        {
            return;
        }

        collisionlayers[layerId]->remove_object( g_obj );
    }
}
