/*
gpe_file_system_raylb.cpp
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

#include "gpe_file_system_raylib.h"

namespace gpe
{
    file_manager_raylib * main_raylib_file_url_manager = nullptr;

    bool init_raylib_file_system()
    {
        error_log->report( "Starting raylib_module File System... ");
        if( main_raylib_file_url_manager != nullptr )
        {
            delete main_raylib_file_url_manager;
            main_raylib_file_url_manager = nullptr;
        }
        else if( main_file_url_manager  != nullptr )
        {
            delete main_file_url_manager;
            main_file_url_manager = nullptr;
        }
        main_file_url_manager = main_raylib_file_url_manager =  new file_manager_raylib();
        error_log->report( "File System swapped from base to raylib class... ");

        return ( main_file_url_manager != nullptr );
    }

    void quit_raylib_file_system()
    {
        gpe::error_log->report("Quitting raylib_module file system....");

        if( main_raylib_file_url_manager!=nullptr )
        {
            delete main_raylib_file_url_manager;
            main_raylib_file_url_manager = nullptr;
        }
        main_file_url_manager = new file_and_url_manager();
        gpe::error_log->report("raylib_module file system quit...");
    }

    file_manager_raylib::file_manager_raylib()
    {

    }

    file_manager_raylib::~file_manager_raylib()
    {

    }

    void file_manager_raylib::external_open_program(std::string program_location, std::string program_info, bool show_program_info_on_fail )
    {
        OpenURL( program_location.c_str() );
    }

    void file_manager_raylib::external_open_url(std::string url_string)
    {
        OpenURL( url_string.c_str() );
    }

    void file_manager_raylib::file_ammend_string(std::string file_name, std::string str_in )
    {
        file_and_url_manager::file_ammend_string(file_name, str_in );
    }

    bool file_manager_raylib::file_copy(std::string source_file_name, std::string destination_file_name, bool overwrite_existing )
    {
        return file_and_url_manager::file_copy( source_file_name, destination_file_name, overwrite_existing );
    }

    bool file_manager_raylib::file_delete( std::string f_name)
    {
        return file_and_url_manager::file_delete( f_name );
    }

    bool file_manager_raylib::file_exists(std::string new_file_name)
    {
        if( FileExists( new_file_name.c_str() ) )
        {
            if(  file_and_url_manager::folder_exists( new_file_name) == false )
            {
                return true;
            }
        }

        return false;
    }

    int file_manager_raylib::file_get_size_bytes(const std::string &file_name)
    {
        return file_and_url_manager::file_get_size_bytes( file_name );
    }

    std::string file_manager_raylib::file_get_size_string(const std::string &file_name)
    {
        return file_and_url_manager::file_get_size_string( file_name );
    }

    int file_manager_raylib::folder_clean(std::string folder_name)
    {
        return file_and_url_manager::folder_clean( folder_name );
    }

    int file_manager_raylib::folder_copy(std::string folder_name, std::string folder_target, bool copy_subfolders, bool overwrite_existing_files )
    {
        return file_and_url_manager::folder_copy( folder_name, folder_target, copy_subfolders, overwrite_existing_files );
    }

    int file_manager_raylib::folder_create( std::string new_path_name)
    {
        return file_and_url_manager::folder_create( new_path_name );
    }

    bool file_manager_raylib::folder_exists(std::string path_name)
    {
        return DirectoryExists( path_name.c_str() );
    }

    bool file_manager_raylib::seek_settings_folder()
    {
        return file_and_url_manager::seek_settings_folder();
    }


    int file_manager_raylib::path_clean(std::string folder_name)
    {
        return file_and_url_manager::folder_clean( folder_name );
    }

    int file_manager_raylib::path_copy(std::string folder_name, std::string folder_target, bool copy_subfolders, bool overwrite_existing_files  )
    {
        return file_and_url_manager::folder_copy( folder_name, folder_target, copy_subfolders, overwrite_existing_files );
    }

    int file_manager_raylib::path_create( std::string new_path_name)
    {
        return file_and_url_manager::folder_create( new_path_name );
    }

    bool file_manager_raylib::path_exists(std::string path_name)
    {
        return DirectoryExists( path_name.c_str() );
    }
}

