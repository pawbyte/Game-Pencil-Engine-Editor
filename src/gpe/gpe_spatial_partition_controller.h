/*
GPE_Spatial_Partition_Controller.h
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

#ifndef gpe_spatial_partition_controller_h
#define gpe_spatial_partition_controller_h

#include "gpe_spatial_partition_layer.h"

namespace gpe
{
    class spatial_partition_controller
    {
        protected:
            std::map<std::string, bool > collisionsCheckedInFrame;
            int spatialMapWidthSize;
            int spatialMapHeightSize;
        public:
            static const int maxLayerCount = 32;
            spatial_partition_layer * collisionlayers[maxLayerCount];
            spatial_partition_controller();
            ~spatial_partition_controller();
            void activate_layer(int layerId );
            void activate_all_layers();
            void add_object_to_grid( game_object * gameObject, int layerId = -1 );
            void clear_spaces();
            void clear_checked_map();
            void check_collisions();
            void deactivate_layer(int layerId );
            void deactivate_all_layers();
            void init_spatialpartioning( int cSceneWidth, int cSceneHeight );
            void remove_object_from_grid( game_object * gameObject );

    };

    extern spatial_partition_controller * GPE_SPATIAL_GRID;
}

#endif //gpe_spatial_partition_controller_h
