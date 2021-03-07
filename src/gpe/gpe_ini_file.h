/*
gpe_ini_file.h
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

#ifndef gpe_ini_file_h
#define gpe_ini_file_h

#include "gpe_file_system.h"
#include "gpe_parser.h"

#include "../other_libs/stg_ex.h"

namespace gpe
{
    class gpe_ini_file
    {
        private:
            std::vector <gpe::key_pair *> ini_sections;
        public:
        gpe_ini_file();
        virtual ~ gpe_ini_file();
        virtual gpe::key_pair * add_section( std::string section_name);
        virtual void clear_all_sections();
        virtual void clear_section( std::string section_name );
        virtual float find_float_keypair( std::string section_name,std::string section_key);
        virtual std::string find_string_keypair( std::string section_name,std::string section_key);
        virtual gpe::key_pair * find_section( std::string section_name);
        virtual bool map_key_pair( std::string section_name,std::string section_key, std::string section_value);
        virtual bool read_ini_file(std::string f_name, int lineTrimStyle = 0);
        virtual void remove_all_sections();
        virtual void remove_section( std::string section_name );
        virtual bool write_ini_file(std::string f_name);
    };
}
#endif //gpe_ini_file_h
