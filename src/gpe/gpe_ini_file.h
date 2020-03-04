/*
gpe_ini_file.h
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

#ifndef gpe_ini_file_h
#define gpe_ini_file_h

#include "../other_libs/sff_ex.h"
#include "../other_libs/stg_ex.h"
#include "gpe_parser.h"

namespace gpe
{
    class gpe_ini_file
    {
        private:
            std::vector <gpe::key_pair *> iniFileSections;
        public:
        gpe_ini_file();
        ~gpe_ini_file();
        gpe::key_pair * add_section( std::string sectionName);
        void clear_all_sections();
        void clear_section( std::string sectionName );
        float find_float_keypair( std::string sectionName,std::string sKey);
        std::string find_string_keypair( std::string sectionName,std::string sKey);
        gpe::key_pair * find_section( std::string sectionName);
        bool map_key_pair( std::string sectionName,std::string sKey, std::string sValue);
        bool read_ini_file(std::string fName, int lineTrimStyle = 0);
        void remove_all_sections();
        void remove_section( std::string sectionName );
        bool write_ini_file(std::string fName);
    };
}
#endif //gpe_ini_file_h
