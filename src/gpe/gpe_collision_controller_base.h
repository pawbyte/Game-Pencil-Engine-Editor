/*
gpe_collision_controller_base.h
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

#ifndef gpe_collision_controller_base_h
#define gpe_collision_controller_base_h

#include "gpe_basic_object.h"
#include <string>

namespace gpe
{
    extern int collision_controller_counter;

    class collision_controller_base
    {
        protected:
            int controller_id;
            std::string controller_name;
            std::string controller_type;
        public:
            collision_controller_base();
            virtual ~collision_controller_base();
            virtual void activate_layer(int layer_id );
            virtual void activate_all_layers();
            virtual void add_object( game_object * g_obj, int layer_id = -1 );
            virtual void check_collisions();
            virtual void clear_all();
            virtual void clear_layer( int layer_id );
            int compare( collision_controller_base * other );
            virtual void deactivate_layer(int layer_id );
            virtual void deactivate_all_layers();
            virtual bool init_system( int scene_width, int scene_height );
            virtual void remove_object( game_object * g_obj );
            virtual bool quit_system();
    };

    extern collision_controller_base * main_collision_controller;
}

#endif //gpe_collision_controller_base_h
