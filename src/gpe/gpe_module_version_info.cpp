/*
gpe_module_version_info.cpp
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


#include "gpe_module_version_info.h"

namespace gpe
{
    module_information::module_information(std::string m_name, std::string m_backend_name, float system_major_ver, float backend_major_ver  )
    {
        module_system_name = m_name;
        module_backend_name = m_backend_name;

        module_system_version_major = system_major_ver;
        module_system_version_minor = 0;
        module_system_version_update = 0;
        module_system_version_total = module_system_version_major;


        module_backend_version_major = backend_major_ver;
        module_backend_version_minor = 0;
        module_backend_version_update = 0;
        module_backend_version_total = module_system_version_major;

        system_version_was_set = false;
        backend_version_was_set = false;
    }

    module_information::~module_information()
    {

    }

    //Getters [ BEGIN ]
    std::string module_information::get_module_backend_name()
    {
        return module_backend_name;
    }

    std::string module_information::get_module_system_name()
    {
        return module_system_name;
    }

    float module_information::get_module_backend_version_major()
    {
        return module_system_version_major;
    }

    float module_information::get_module_backend_version_minor()
    {
        return module_system_version_minor;
    }

    float module_information::get_module_backend_version_update()
    {
        return module_system_version_update;

    }

    float module_information::get_module_backend_version_total()
    {
        return module_system_version_total;
    }

    float module_information::get_module_system_version_major()
    {
        return module_backend_version_major;
    }

    float module_information::get_module_system_version_minor()
    {
        return module_backend_version_minor;
    }

    float module_information::get_module_system_version_update()
    {
        return module_backend_version_update;
    }

    float module_information::get_module_system_version_total()
    {
        return module_backend_version_total;
    }

    bool module_information::is_backend_version_set()
    {
        return backend_version_was_set;
    }

    bool module_information::is_system_version_set()
    {
        return system_version_was_set;
    }

    bool module_information::set_system_versions( float system_major_ver, float system_minor_ver, float system_version_update )
    {
        if( system_version_was_set )
        {
            return false;
        }

        module_system_version_major = system_major_ver;
        module_system_version_minor = system_minor_ver;
        module_system_version_update = system_version_update;
        module_system_version_total = module_system_version_major;
        return true;
    }

    bool module_information::set_backend_versions( float backend_major_ver, float backend_minor_ver, float backend_version_update )
    {
        if( backend_version_was_set )
        {
            return false;
        }

        module_backend_version_major = backend_major_ver;
        module_backend_version_minor = backend_minor_ver;
        module_backend_version_update = backend_version_update;
        module_backend_version_total = module_system_version_major;

        return true;
    }
}
