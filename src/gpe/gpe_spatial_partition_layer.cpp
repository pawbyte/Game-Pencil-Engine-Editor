/*
spatial_partition_layer.cpp
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

#include "gpe_spatial_partition_layer.h"

namespace gpe
{
    spatial_partition_layer::spatial_partition_layer( int myLayerId )
    {
        spatialGridIsCreated = false;
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
            init_collision_handler( spatialGridWidthSize * spatialGridWidthAmount, spatialGridHeightSize * spatialGridHeightAmount);
        }
        isActive = true;
    }

    void spatial_partition_layer::add_object( game_object * gameObject )
    {
        int partitionsSize = (int)layerPartitions.size();
        if( gameObject == NULL || partitionsSize == 0 )
        {
            return;
        }
        gameObject->set_collision_space_id( layerID );
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
        spatial_partition * tempPartition = NULL;
        spatial_partition * tempPartitionLeft = NULL;
        spatial_partition * tempPartitionAbove = NULL;
        spatial_partition * tempPartitionAboveLeft = NULL;

        int collisionSpaceCount = (int)layerPartitions.size();

        int tempLeftPos = -1;
        int tempAbovePos = -1;
        int tempAboveLeftPos = -1;
        for( int i = 0; i < collisionSpaceCount; i++ )
        {
            tempPartition = layerPartitions[i];
            if( tempPartition!=NULL )
            {
                //Check all of the collisions within itself
                tempPartition->check_collisions();

                //Now look and see if it can check with 3 neighbors..
                tempLeftPos = i -1;
                if( tempLeftPos > 0 && tempLeftPos < collisionSpaceCount)
                {
                    tempPartitionLeft = layerPartitions[tempLeftPos];
                    if( tempPartitionLeft !=NULL )
                    {
                        tempPartition->check_collisions_with_other( tempPartitionLeft );
                    }
                }

                tempAbovePos = i - gpeSpatialGridData::sGridYAmount;
                if( tempAbovePos > 0 && tempAbovePos < collisionSpaceCount)
                {
                    tempPartitionAbove = layerPartitions[tempAbovePos];
                    if( tempPartitionAbove !=NULL )
                    {
                        tempPartition->check_collisions_with_other( tempPartitionAbove );
                    }
                }

                tempAboveLeftPos = i - gpeSpatialGridData::sGridYAmount -1;
                if( tempAboveLeftPos > 0 && tempAboveLeftPos < collisionSpaceCount)
                {
                    tempPartitionAboveLeft = layerPartitions[tempAboveLeftPos];
                    if( tempPartitionAboveLeft !=NULL )
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

    void spatial_partition_layer::clear_spaces( )
    {
        spatial_partition *  tempSpace = NULL;
        for( int i = (int)layerPartitions.size()-1; i >=0; i-- )
        {
            tempSpace = layerPartitions[i];
            if( tempSpace!=NULL)
            {
                delete tempSpace;
                tempSpace = NULL;
            }
        }
        layerPartitions.clear();
        spatialGridIsCreated = false;
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
            sPar =  (( xIn / spatialGridWidthSize)|0 )*spatialGridHeightAmount;
            sPar+=( yIn / spatialGridHeightSize)|0;
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

    void spatial_partition_layer::init_collision_handler(  int cSceneWidth, int cSceneHeight )
    {

        spatialGridWidthSize = cSceneWidth;
        spatialGridHeightSize = cSceneHeight;

        spatialGridWidthAmount = ceil(cSceneWidth/spatialGridWidthSize);
        if( spatialGridWidthAmount<2)
        {
            spatialGridWidthSize = cSceneWidth/2;
            spatialGridWidthAmount = 2;
        }
        spatialGridHeightAmount = ceil(cSceneHeight/spatialGridHeightSize);
        if( spatialGridHeightAmount<2 )
        {
            spatialGridHeightSize = cSceneHeight/2;
            spatialGridHeightAmount = 2;
        }


        //smallest size possible is 2x2
        int iGrid, jGrid;
        int xStart=0, yStart=0;
        spatial_partition * newSpace = NULL;
        for( iGrid =0; iGrid < spatialGridWidthAmount; iGrid+=1)
        {
            for( jGrid=0; jGrid < spatialGridHeightAmount; jGrid+=1)
            {
                newSpace = new spatial_partition();
                newSpace->set_space(xStart,yStart,spatialGridWidthSize,spatialGridHeightSize);
                yStart+=spatialGridHeightSize;
                newSpace->spaceName+="testrun_"+ stg_ex::int_to_string(iGrid)+"_"+ stg_ex::int_to_string(jGrid)+"_test";
                layerPartitions.push_back(newSpace);
            }
            yStart = 0;
            xStart+=spatialGridWidthSize;
        }
        spatialGridIsCreated = true;
    }

    bool spatial_partition_layer::is_active()
    {
        return isActive;
    }

    void spatial_partition_layer::remove_object( game_object * gameObject )
    {
        if( gameObject == NULL)
        {
            return;
        }
    }
}
