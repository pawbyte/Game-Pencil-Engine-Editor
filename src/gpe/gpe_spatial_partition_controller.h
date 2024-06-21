/*
gpe_spatial_partition_controller.h
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

#ifndef gpe_spatial_partition_controller_h
#define gpe_spatial_partition_controller_h

#include "gpe_collision_controller_base.h"
#include "gpe_spatial_partition_layer.h"
#include "gpe_error_logger.h"

namespace gpe
{
    class spatial_partition_controller : public collision_controller_base
    {
        private:
            gpe::shape_point2d * temp_collision_point2d;
            gpe::shape_point3d * temp_collision_point3d;
            gpe::shape_circle * temp_collision_circle;
            gpe::shape_rect * temp_collision_rect;
        protected:
            std::map<std::string, bool > collisionsCheckedInFrame;
            int spatial_map_width_size;
            int spatial_map_height_size;
        public:
            spatial_partition_layer * collisionlayers[max_collision_layer_count];

            spatial_partition_controller( std::string c_name );
            ~spatial_partition_controller();
            void activate_layer(int layer_id );
            void activate_all_layers();
            void add_object( game_object * g_obj, int layer_id = -1 );
            void clear_spaces();
            void clear_checked_map();
            void check_collisions();

            game_object * check_objects_circle(int layer_id, int x, int y, int r, int object_type, bool check_for_children  );
            bool check_objects_circle_all(int layer_id, int x, int y, int r, int object_type, std::vector<game_object *>& obj_vector, bool check_for_children  );
            game_object * check_objects_point(int layer_id, int x, int y, int object_type, bool check_for_children );
            bool check_objects_point_all(int layer_id, int x, int y, int object_type, std::vector<game_object *>& obj_vector, bool check_for_children );
            game_object * check_objects_rectangle(int layer_id, int x, int y, int w, int h, int object_type, bool check_for_children  );
            bool check_objects_rectangle_all(int layer_id, int x, int y, int w, int h, int object_type, std::vector<game_object *>& obj_vector, bool check_for_children  );

            void deactivate_layer(int layer_id );
            void deactivate_all_layers();

            bool detect_objects_circle(int layer_id,int x, int y, int r, int object_type );
            bool detect_objects_rectangle(int layer_id,int x, int y, int w, int h, int object_type );

            void end_frame();
            bool init_system( int scene_width, int scene_height );
            bool load_collision_grid( std::string map_file );
            bool quit_system();
            void remove_object( game_object * g_obj );
            void reset_system();
            void start_frame();
    };

    extern spatial_partition_controller * main_spatial_grid;
}

#endif //gpe_spatial_partition_controller_h
