/*
gpe_basic_object_type_mapper.cpp
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

#include "gpe_basic_object_type_mapper.h"

namespace gpe
{
    object_mapper::object_mapper()
    {

    }

    object_mapper::~object_mapper()
    {
        clear_map();
    }

    bool object_mapper::add_to_map(std::string object_name, game_object *( *mFunction)(int x_pos_in,  int y_pos_in,  int object_layer_id) )
    {
        auto pairExists = objMap.find( object_name );

        if (pairExists == objMap.end() )
        {
            return false;
        }
        objMap[object_name] = mFunction;
        return true;
    }

    void object_mapper::clear_map()
    {
        objMap.clear();
    }

    game_object * object_mapper::create_object(std::string object_name, int x_pos_in,  int y_pos_in,  int object_layer_id)
    {
        auto pairExists = objMap.find( object_name );
        if (pairExists == objMap.end() )
        {
            return nullptr;
        }
        return objMap[object_name](x_pos_in, y_pos_in, object_layer_id);
    }
}
