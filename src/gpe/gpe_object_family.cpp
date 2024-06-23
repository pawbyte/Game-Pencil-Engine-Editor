/*
gpe_object_family.cpp
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

 #include "gpe_object_family.h"


object_family_library::object_family_library()
{
    families_are_finalized = false;
}

object_family_library::~object_family_library()
{

}

bool object_family_library::add_to_object_family( int family_id, int object_type_in )
{
    auto found_family = objects_family_map.find( family_id );
    // TODO, too sleepy for now 6/22/24
    if( found_family == std::npos )
    {
        objects_family_map.add( ); // TODO, too sleepy for now 6/22/24
        new_family.second.push_back( object_type_in );
    }
}

bool object_family_library::families_finalized()
{
    return families_are_finalized;
}

bool object_family_library::load_object_families( std::string f_name )
{
    return false; //Save Implementation for new Issue...
}

void object_family_library::make_final()
{
    families_are_finalized = true;
}

bool object_family_library::object_is_in_family( int family_id, int object_type_in )
{
    auto found_family = objects_family_map.find( family_id );
    // TODO, too sleepy for now 6/22/24
    if( found_family == std::npos )
    {
        //defaults to false;
        return false;
    }
     // TODO, too sleepy for now 6/22/24
    if( found_family.find( object_type_in) != std::npos)
    {
        return true;
    }

    return false;
}
