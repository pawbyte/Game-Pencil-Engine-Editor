/*
gpe_file_system.h
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

#ifndef gpe_file_system_h
#define gpe_file_system_h

#include "gpe_common_includes.h"
#include "gpe_constants.h"
#include "gpe_globals.h"
#include "gpe_settings.h"
#include "gpe_texture_base.h"
#include "../other_libs/stg_ex.h"

namespace gpe
{
    class filetype_filter
    {
        public:
            std::string desc;
            std::string exts;
            filetype_filter(std::string filter_desc = "", std::string filter_extensions = "");
            ~filetype_filter();
    };

    std::string generate_filters(const std::vector<filetype_filter *> filters);
    std::string parse_file_types(std::string fileTypesIn, std::vector <std::string> &fileTypeVector);
    bool file_passes_filter(std::string fileTypeIn, std::vector <std::string> &fileTypeVector);

    class file_object
    {
        private:
            std::string file_name;
            std::string fileType;
            bool file_is_dir;
        public:
            std::string file_directory_location;
            texture_base * thumbnail_texture;
            int file_size_info;
            std::string date_creation_info;
            std::string date_modified_info;
            file_object(std::string f_name,bool f_is_dir);
            ~file_object();
            bool is_directory();
            std::string get_name();
            std::string get_type();
    };

    class file_directory_class
    {
        private:
            std::vector< file_object * >files_list;
            int file_position;
            int file_count;
            int sub_directory_count;
        public:
            file_directory_class();
            ~file_directory_class();
            bool open_directory(std::string directory_string);
            bool open_directory_sorted(std::string directory_string);
            void close_directory();
            void filter_directory(bool only_folders, std::vector <std::string> &file_types_to_use);
            file_object * get_file(int position);
            file_object * get_next_file();
            bool has_next_file();
            int get_count();
            int get_subdirectory_count();
            int get_file_count();
            bool set_position(int position);
    };

    const float byte_size_kb = 1024; //if this is wrong, change it to 1000 in your own time. :-)
    const float byte_size_mb = byte_size_kb*1024;
    const float byte_size_gb = byte_size_mb*1024;
    const float byte_size_tb = byte_size_gb*1024;
    const float byte_size_pb = byte_size_tb*1024;

    void external_open_program(std::string program_location, std::string program_info="", bool show_program_info_on_fail = false);
    void external_open_url(std::string url_string);

    int get_file_size_bytes(const std::string &file_name);
    std::string get_file_size_string(const std::string &file_name);

    int clean_folder(std::string folder_name);
    int copy_folder(std::string folder_name, std::string folder_target, bool copy_subfolders = false, bool overwrite_existing_files = false );

    std::string  get_user_settings_folder();
    bool seek_settings_folder();
    std::string get_user_temp_folder();
    std::string get_user_screenshot_folder();
    bool delete_file( std::string f_name);

}

#endif //gpe_file_system_h
