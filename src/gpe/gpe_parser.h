/*
gpe_parser.h
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

#ifndef GPE_PARSER
#define GPE_PARSER

#include <string>
#include <vector>

namespace gpe
{
    class key_pair
    {
        public:
            int id_unique_number;
            int key_id;
            float key_value;
            std::string key_string;
            std::string key_substring;
            std::vector <gpe::key_pair *> sub_options;
            gpe::key_pair * parent_pair;
            key_pair(int idNumb, std::string str, std::string ksubstr = "", float kValue = -1);
            ~key_pair();
            gpe::key_pair * add_keypair(std::string optionName, std::string optionSubStr= "",float optionValue = 0 );
            gpe::key_pair *  find_option_id( int kId );
            gpe::key_pair *  find_option_named( std::string str );
            gpe::key_pair *  find_option_value( float kVal );
            int get_unique_id();
            bool name_exists( std::string str);
            bool remove_option_id( int kId, bool nest_down = true );
            bool remove_option_named( std::string str, bool nest_down = true );
            bool remove_option_value( float kVal, bool nest_down = true );
            void remove_all();
    };
}
#endif //GPE_PARSER
