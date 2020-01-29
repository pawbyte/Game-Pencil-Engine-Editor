/*
GPE_Spatial_Partition_Controller.h
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

#ifndef GPE_SPATIAL_PARTITIONS_CONTROLLER_H
#define GPE_SPATIAL_PARTITIONS_CONTROLLER_H

#include "GPE_Spatial_Partition_Layer.h"

class GPE_SpatialPartition_Controller
{
    protected:
        std::map<std::string, bool > collisionsCheckedInFrame;
        int spatialMapWidthSize;
        int spatialMapHeightSize;
    public:
        static const int maxLayerCount = 32;
        GPE_Spatial_Partition_Layer * collisionlayers[maxLayerCount];
        GPE_SpatialPartition_Controller();
        ~GPE_SpatialPartition_Controller();
        void activate_layer(int layerId );
        void activate_all_layers();
        void add_object_to_grid( GPE_GameObject * gameObject, int layerId = -1 );
        void clear_spaces();
        void clear_checked_map();
        void check_collisions();
        void deactivate_layer(int layerId );
        void deactivate_all_layers();
        void init_spatialpartioning( int cSceneWidth, int cSceneHeight );
        void remove_object_from_grid( GPE_GameObject * gameObject );

};

#endif //
