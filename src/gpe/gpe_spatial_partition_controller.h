/*
gpe_spatial_partition_controller.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_collision_controller_base.h"
#include "gpe_spatial_partition_layer.h"
#include "gpe_error_logger.h"

namespace gpe
{
    class spatial_partition_controller : public collision_controller_base
    {
        protected:
            std::map<std::string, bool > collisionsCheckedInFrame;
            int spatial_map_width_size;
            int spatial_map_height_size;
        public:
            static const int maxLayerCount = 32;
            spatial_partition_layer * collisionlayers[maxLayerCount];

            spatial_partition_controller( std::string c_name );
            ~spatial_partition_controller();
            void activate_layer(int layer_id );
            void activate_all_layers();
            void add_object( game_object * g_obj, int layer_id = -1 );
            void clear_spaces();
            void clear_checked_map();
            void check_collisions();
            void deactivate_layer(int layer_id );
            void deactivate_all_layers();
            bool init_system( int scene_width, int scene_height );
            bool quit_system();
            void remove_object( game_object * g_obj );
    };

    extern spatial_partition_controller * main_spatial_grid;
}

#endif //gpe_spatial_partition_controller_h
