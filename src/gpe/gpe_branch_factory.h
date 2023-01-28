/*
gpe_branch_factory.cpp
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

#include "gpe_branch.h"

namespace gpe
{

    class branch_factory
    {
        private:
            std::map <std::string,   branch *  > factory_map;
        public:
            branch_factory();
            ~branch_factory();
            int add_to_map( std::string branch_type_name, branch *  ); //returns 1 on success, 0 if name is assigned to another factory object, -1 if factory object is nullptr/NUll
            void clear_map(); // ONLY CALL ON GAME END!
            branch *  create_branch( std::string branch_type_name ); //returns NULLPTR if not in map
            branch *  create_branch_inited( std::string branch_type_name,  int branch_layer_id, float x_pos_in,  float y_pos_in, float z_pos_in = 0  ); //returns NULLPTR if not in map
            int get_factory_size();
            bool object_exists( std::string branch_type_name );
    };
}
