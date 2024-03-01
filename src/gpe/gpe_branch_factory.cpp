/*
gpe_branch_factory.cpp
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

#include "gpe_branch_factory.h"

namespace gpe
{
    branch_factory::branch_factory()
    {
        //ctor
    }

    branch_factory::~branch_factory()
    {
        clear_map();
    }

    int branch_factory::add_to_map( std::string branch_type_name, branch * factory_object )
    {
        if( factory_object == nullptr || factory_object == nullptr )
        {
            return -1;
        }

        if( (int)branch_type_name.size() == 0 )
        {
            return -2;
        }

        if( factory_map.find( branch_type_name) == factory_map.end() )
        {
            return 0; // already in map, will not override
        }

        factory_map[ branch_type_name ] = factory_object;
        return 1;
    }

    void branch_factory::clear_map()
    {
        std::map<std::string, branch * >::iterator it;
        branch * temp_branch = nullptr;
        for (it = factory_map.begin(); it != factory_map.end(); it++)
        {
            temp_branch = it->second;

            if( temp_branch != nullptr )
            {
                delete temp_branch;
                temp_branch = nullptr;
            }
        }
        factory_map.clear();
    }

    branch *  branch_factory::create_branch( std::string branch_type_name )
    {
        if( (int) branch_type_name.size() == 0 )
        {
            return nullptr;
        }

        std::map<std::string, branch * >::iterator  branch_mold = factory_map.find( branch_type_name);

        if( branch_mold == factory_map.end() )
        {
            return nullptr;
        }

        branch * found_branch_mold = branch_mold->second;

        if( found_branch_mold == nullptr )
        {
            return nullptr;
        }
        return found_branch_mold->branch_create();
    }

    branch *  branch_factory::create_branch_inited( std::string branch_type_name,  int branch_layer_id, float x_pos_in,  float y_pos_in, float z_pos_in  )
    {
        if( (int) branch_type_name.size() == 0 )
        {
            return nullptr;
        }

        std::map<std::string, branch * >::iterator  branch_mold = factory_map.find( branch_type_name);

        if( branch_mold == factory_map.end() )
        {
            return nullptr;
        }

        branch * found_branch_mold = branch_mold->second;

        if( found_branch_mold == nullptr )
        {
            return nullptr;
        }
        return found_branch_mold->branch_create_and_init( branch_layer_id, x_pos_in, y_pos_in, z_pos_in );
    }

    int branch_factory::get_factory_size()
    {
        return (int)factory_map.size();
    }

    bool branch_factory::object_exists( std::string branch_type_name )
    {
        if( (int)branch_type_name.size() == 0 )
        {
            return false;
        }

        if( factory_map.find( branch_type_name) == factory_map.end() )
        {
            return false;
        }
        return true;
    }
}
