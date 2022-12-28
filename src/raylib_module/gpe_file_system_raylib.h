/*
gpe_file_system_raylb.h
This file is part of:GAME PENCIL ENGINE
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

#ifndef gpe_file_system_raylib_h
#define gpe_file_system_raylib_h


#include "../gpe/gpe_file_system.h"
#include "raylib.h"

namespace gpe
{
    class file_manager_raylib : public file_and_url_manager
    {
        protected:

            public:
            file_manager_raylib();
            ~file_manager_raylib();

            void external_open_program(std::string program_location, std::string program_info="", bool show_program_info_on_fail = false);
            void external_open_url(std::string url_string);

            void file_ammend_string(std::string file_name, std::string str_in );
            bool file_copy(std::string source_file_name, std::string destination_file_name, bool overwrite_existing = true);
            bool file_delete( std::string f_name);

            bool file_exists(std::string new_file_name);


            int file_get_size_bytes(const std::string &file_name);
            std::string file_get_size_string(const std::string &file_name);

            int folder_clean(std::string folder_name);
            int folder_copy(std::string folder_name, std::string folder_target, bool copy_subfolders = false, bool overwrite_existing_files = false );
            int folder_create( std::string new_path_name);
            bool folder_exists(std::string path_name);


            bool seek_settings_folder();

            int path_clean(std::string folder_name);
            int path_copy(std::string folder_name, std::string folder_target, bool copy_subfolders = false, bool overwrite_existing_files = false );
            int path_create( std::string new_path_name);
            bool path_exists(std::string path_name);
    };

    extern file_manager_raylib * main_raylib_file_url_manager;

    bool init_raylib_file_system();
    void quit_raylib_file_system();
}

#endif //gpe_file_system_raylib_h
