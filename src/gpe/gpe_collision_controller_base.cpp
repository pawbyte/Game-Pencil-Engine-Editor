/*
gpe_collision_controller_base.cpp
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
/*
gpe_collision_controller_base.h
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

#include "gpe_collision_controller_base.h"

namespace gpe
{

    collision_controller_base * main_collision_controller = nullptr;

    collision_controller_base::collision_controller_base()
    {
        controller_id = collision_controller_counter;
        collision_controller_counter++;
        layer_id = -1;
        collision_controller_counter = 0;
    }

    collision_controller_base::~collision_controller_base()
    {

    }

    void collision_controller_base::activate_layer(int layer_id )
    {

    }

    void collision_controller_base::activate_all_layers()
    {

    }

    void collision_controller_base::add_object( game_object * g_obj, int l_id  )
    {

    }

    void collision_controller_base::check_collisions()
    {

    }

    void collision_controller_base::clear_all()
    {

    }

    void collision_controller_base::clear_layer( int l_id )
    {

    }

    int collision_controller_base::compare( collision_controller_base * other )
    {
        if( other == nullptr )
        {
            return -1;
        }
        if( other->controller_id != controller_id )
        {
            return 0;
        }
        if( other->controller_name != controller_name )
        {
            return 0;
        }
        if( other->controller_type != controller_type )
        {
            return 0;
        }
        return 1;
    }

    void collision_controller_base::deactivate_layer(int l_id )
    {

    }

    void collision_controller_base::deactivate_all_layers()
    {

    }

    bool collision_controller_base::init_system( int scene_width, int scene_height )
    {
        return false; //WIPNOTFUNCTIONALYET
    }

    void collision_controller_base::remove_object( game_object * g_obj )
    {

    }

    bool collision_controller_base::quit_system()
    {
        return true;
    }
}
