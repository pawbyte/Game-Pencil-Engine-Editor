/*
GPE_Spatial_Partition.h
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

#ifndef gpe_spatial_partition_h
#define gpe_spatial_partition_h

#include "gpe_basic_object.h"
#include "gpe_collision_controller_base.h"
#include "gpe_shapes.h"

namespace gpe
{
    class spatial_partition
    {
        private:
            std::vector< collision_event >  collisions_found;
            game_object * sought_first_object;
            game_object * sought_second_object;
        protected:
            /*
            std::vector<game_object * > objects_static;
            std::vector<game_object * > objects_rigid;
            std::vector<game_object * > objects_characters;
            */
            std::vector<game_object * > objectsArray;
            int objectsHeld;
            gpe::shape_rect * boxArea;
        public:
            std::string spaceName;
            spatial_partition();
            ~spatial_partition();

            void add_object(game_object * newObject);
            void check_collisions();


            void check_collisions_with_other( spatial_partition * other);
            void check_for_leaving_collisions( bool execute_collision_function = true );


            game_object * check_objects_circle( gpe::shape_circle * checked_circle, int object_type, bool check_for_children  );
            bool check_objects_circle_all( gpe::shape_circle * checked_circle, int object_type, std::vector<game_object *>& obj_vector, bool check_for_children  );
            game_object * check_objects_point( gpe::shape_point2d * checked_point, int object_type, bool check_for_children  );
            bool check_objects_point_all( gpe::shape_point2d * checked_point, int object_type, std::vector<game_object *>& obj_vector, bool check_for_children  );
            game_object * check_objects_rectangle( gpe::shape_rect * checked_rect, int object_type, bool check_for_children  );
            bool check_objects_rectangle_all( gpe::shape_rect * checked_rect, int object_type, std::vector<game_object *>& obj_vector, bool check_for_children  );

            bool collides_with_circle( gpe::shape_circle * checked_circle);
            bool collides_with_point2d( gpe::shape_point2d * checked_point);
            bool collides_with_point3d( gpe::shape_point3d * checked_point);
            bool collides_with_rectangle( gpe::shape_rect * checked_rect);
            collision_event * collision_exist( game_object * object_one, game_object * object_two );
            void empty_list();
            game_object * get_object(int pos);
            int get_size();
            bool matching_collision( collision_event * col_event );
            void set_space(int x_new, int y_new, int newW, int new_height);
            void remove_held_object(game_object * object_in);
    };
}
#endif //gpe_spatial_partition_h
