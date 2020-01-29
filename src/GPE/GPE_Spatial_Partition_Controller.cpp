/*
GPE_Spatial_Partition_Controller.cpp
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

#include "GPE_Spatial_Partition_Controller.h"

GPE_SpatialPartition_Controller::GPE_SpatialPartition_Controller()
{
    for( int i = 0; i < maxLayerCount; i++ )
    {
        collisionlayers[i] = new GPE_Spatial_Partition_Layer( i );
    }
}

GPE_SpatialPartition_Controller::~GPE_SpatialPartition_Controller()
{
    GPE_Spatial_Partition_Layer * tempLayer = NULL;
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


void GPE_SpatialPartition_Controller::activate_layer(int layerId )
{
    if( layerId < 0 || layerId  >= maxLayerCount )
    {
        return;
    }
    GPE_Spatial_Partition_Layer * tempLayer = collisionlayers[layerId];
    tempLayer->activate_layer();

}

void GPE_SpatialPartition_Controller::activate_all_layers()
{
    GPE_Spatial_Partition_Layer * tempLayer = NULL;
    for( int i = maxLayerCount -1; i >=0 ; i--)
    {
        tempLayer = collisionlayers[i];
        if( tempLayer!=NULL)
        {
            tempLayer->activate_layer();
        }
    }
}

void GPE_SpatialPartition_Controller::clear_checked_map()
{

}

void GPE_SpatialPartition_Controller::add_object_to_grid( GPE_GameObject * gameObject , int layerId )
{
    if( gameObject == NULL)
    {
        return;
    }
    if( layerId < 0 )
    {
        layerId =0;
    }
    else if( layerId >= maxLayerCount )
    {
        layerId = maxLayerCount -1;
    }

    if( collisionlayers[layerId] == NULL)
    {
        return;
    }

    collisionlayers[layerId]->add_object( gameObject );
}

void GPE_SpatialPartition_Controller::clear_spaces()
{
    GPE_Spatial_Partition_Layer * tempLayer = NULL;
    for( int iLayer = 0; iLayer < maxLayerCount; iLayer++ )
    {
        tempLayer = collisionlayers[iLayer];

        if( tempLayer!=NULL )
        {
            tempLayer->clear_spaces();
        }
    }
}

void GPE_SpatialPartition_Controller::check_collisions()
{
    clear_checked_map();
    int iLayer, jLayer = 0;
    //loop through every active layer
    GPE_Spatial_Partition_Layer * tempLayerRow = NULL;
    GPE_Spatial_Partition_Layer * tempLayerColumn = NULL;
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

void GPE_SpatialPartition_Controller::deactivate_all_layers()
{
    GPE_Spatial_Partition_Layer * tempLayer = NULL;
    for( int i = maxLayerCount -1; i >=0 ; i--)
    {
        tempLayer = collisionlayers[i];
        if( tempLayer!=NULL)
        {
            tempLayer->deactivate_layer();
        }
    }
}

void GPE_SpatialPartition_Controller::deactivate_layer(int layerId )
{
    if( layerId < 0 || layerId  >= maxLayerCount )
    {
        return;
    }
    GPE_Spatial_Partition_Layer * tempLayer = collisionlayers[layerId];
    tempLayer->deactivate_layer();
}

void GPE_SpatialPartition_Controller::init_spatialpartioning(  int cSceneWidth, int cSceneHeight )
{
    GPE_Spatial_Partition_Layer * tempLayer = NULL;
    for( int i = maxLayerCount -1; i >=0 ; i--)
    {
        tempLayer = collisionlayers[i];
        if( tempLayer!=NULL)
        {
            tempLayer->init_spatialpartioning( cSceneWidth, cSceneHeight );
        }
    }
}

void GPE_SpatialPartition_Controller::remove_object_from_grid( GPE_GameObject * gameObject )
{
    if( gameObject == NULL)
    {
        return;
    }
    int layerId = gameObject->get_layer_id();
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

    collisionlayers[layerId]->remove_object( gameObject );
}
