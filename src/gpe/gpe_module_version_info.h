/*
gpe_module_version_info.h
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

#ifndef gpe_module_version_info_h
#define gpe_module_version_info_h

#include <string>

namespace gpe
{
    class module_information
    {
        protected:
            //To avoid abuse of setters this will only be set once
            bool backend_version_was_set;
            bool system_version_was_set;

            //Module version information [ BEGIN ]
            float module_backend_version_major, module_backend_version_minor, module_backend_version_update, module_backend_version_total;
            float module_system_version_major, module_system_version_minor, module_system_version_update, module_system_version_total;
            std::string module_backend_name;
            std::string module_system_name;
            //Module version information [ END ]

        public:
            module_information(std::string m_name, std::string m_backend_name, float system_major_ver, float backend_major_ver  );
            ~module_information();

            //Getters [ BEGIN ]
            std::string get_module_backend_name();
            std::string get_module_system_name();

            float get_module_backend_version_major();
            float get_module_backend_version_minor();
            float get_module_backend_version_update();
            float get_module_backend_version_total();

            float get_module_system_version_major();
            float get_module_system_version_minor();
            float get_module_system_version_update();
            float get_module_system_version_total();

            bool is_backend_version_set();
            bool is_system_version_set();
            //Getters [ END ]

            //Setters [ BEGIN ]
            //Returns false if already set before
            bool set_system_versions( float system_major_ver, float system_minor_ver, float system_version_update );
            bool set_backend_versions( float backend_major_ver, float backend_minor_ver, float backend_version_update );
            //Setters [ END ]
    };
}


#endif //gpe_module_version_info_h
