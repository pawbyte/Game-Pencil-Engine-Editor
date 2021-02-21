/*
spatial_partition_layer.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef gpe_spatial_partition_layer_h
#define gpe_spatial_partition_layer_h

#include "gpe_spatial_partition.h"

namespace gpe
{
    class spatial_partition_layer
    {
        protected:
            std::vector<spatial_partition * > layerPartitions; //Used for spatial partitioning collision checker
            bool superDebugCollisions;
            bool spatial_grid_created;

            std::map<std::string,bool> object_collision_is_checked;
            std::vector<game_object * > collisionPairA;
            std::vector<game_object * > collisionPairB;
            int layerID;
            bool isActive;
            int spatial_grid_width_size ,  spatial_grid_height_size;
            int spatial_grid_x_amount, spatial_grid_y_amount;
        public:
            spatial_partition_layer( int myLayerId );
            ~spatial_partition_layer();
            void activate_layer();
            void add_object( game_object * gameObject );
            void check_collisions_self();
            void check_collisions_with_layer( spatial_partition_layer * otherLayer);
            void clear_spaces();
            void deactivate_layer();
            int find_spatial_at( int xIn, int yIn);
            void init_collision_handler( int camera_scene_width, int camera_scene_height );
            bool is_active();
            void remove_object( game_object * gameObject );
    };
}

#endif //gpe_spatial_partition_layer_h
