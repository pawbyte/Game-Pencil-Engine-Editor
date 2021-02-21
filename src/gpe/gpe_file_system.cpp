/*
gpe_file_system.cpp
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

#include <cstring>

#include "gpe_file_system.h"
#include "gpe_globals.h"

namespace gpe
{
    file_and_url_manager * main_file_url_manager = NULL;

    std::string generate_filters(const std::vector<filetype_filter *> filters)
    {
        std::string result ="";
        filetype_filter * tempFilter = NULL;
        for(int i = 0; i < (int)filters.size(); i++)
        {
            tempFilter = filters[i];
            if( tempFilter!=NULL)
            {
                result += tempFilter->desc;
                result += '\0';
                result += tempFilter->exts;
                result += '\0';
            }
        }
        return result + '\0';
    }

    std::string parse_file_types(std::string allowedFileTypes,std::vector <std::string> &fileTypeVector)
    {
        fileTypeVector.clear();
        std::string returnFileFilterstring = "All types(*.*)\0*.*\0";
        std::vector<filetype_filter *> filters;
        filetype_filter * tempNewFilter = NULL;
        if( (int)allowedFileTypes.size() > 0)
        {
            if( allowedFileTypes=="All types(*.*)" ||  allowedFileTypes=="All Files" || allowedFileTypes=="None" ||  allowedFileTypes=="All Files*")
            {
                fileTypeVector.clear();
            }
            else if( allowedFileTypes=="Image Only" || allowedFileTypes=="Image" || allowedFileTypes=="Images" || allowedFileTypes=="Photos")
            {
                tempNewFilter = new filetype_filter("Images (*.bmp | *.png | *.jpg)", "*.bmp;*.png;*.jpg");
                filters.push_back(tempNewFilter );
                fileTypeVector.push_back("bmp");
                fileTypeVector.push_back("BMP");
                fileTypeVector.push_back("gif");
                fileTypeVector.push_back("GIF");
                fileTypeVector.push_back("jpg");
                fileTypeVector.push_back("JPG");
                fileTypeVector.push_back("jpeg");
                fileTypeVector.push_back("JPEG");
                fileTypeVector.push_back("png");
                fileTypeVector.push_back("PNG");
            }
            else if( allowedFileTypes=="Audio Only" || allowedFileTypes=="Audio")
            {
                fileTypeVector.push_back("aac");
                fileTypeVector.push_back("AAC");
                fileTypeVector.push_back("mp3");
                fileTypeVector.push_back("MP3");
                fileTypeVector.push_back("ogg");
                fileTypeVector.push_back("OGG");
                fileTypeVector.push_back("wav");
                fileTypeVector.push_back("WAV");

                tempNewFilter = new filetype_filter("Audio (*.aac | *.mp3 | *.ogg| *.wav)", "*.aac;*.mp3;*.ogg;*.wav");
                filters.push_back(tempNewFilter );
            }
            else if( allowedFileTypes=="Video Only" || allowedFileTypes=="Video" || allowedFileTypes=="Videos")
            {
                tempNewFilter = new filetype_filter("Video (*.mp4 | *.ogg| *.webm)", "*.mp4;*.ogg;*.webm");
                filters.push_back(tempNewFilter );
                fileTypeVector.push_back("mp4");
                fileTypeVector.push_back("MP4");
                fileTypeVector.push_back("ogg");
                fileTypeVector.push_back("OGG");
                fileTypeVector.push_back("webm");
                fileTypeVector.push_back("WEBM");
            }
            else if( allowedFileTypes=="Fonts Only" || allowedFileTypes=="Font" || allowedFileTypes=="Fonts")
            {
                tempNewFilter = new filetype_filter("Fonts (*.eot | *.svg| *.ttf)", "*.eot;*.svg;*.ttf");
                filters.push_back(tempNewFilter );
                fileTypeVector.push_back("eot");
                fileTypeVector.push_back("EOT");
                fileTypeVector.push_back("svg");
                fileTypeVector.push_back("SVG");
                fileTypeVector.push_back("ttf");
                fileTypeVector.push_back("TTF");
            }
            else if( allowedFileTypes=="Game Pencil Projects" || allowedFileTypes=="GPE Project")
            {
                tempNewFilter = new filetype_filter("Game Pencil Projects (*.gppf)", "*.gppf");
                fileTypeVector.push_back("gppf");
            }
            else if( allowedFileTypes.size() > 2)
            {
                allowedFileTypes = stg_ex::string_replace_all(allowedFileTypes," ","");
                int asterikPosition = allowedFileTypes.find("*");
                int maxIterations = 10;
                int otherTypesIterations = 0;
                if( asterikPosition!=(int)std::string::npos)
                {
                    while( asterikPosition!=(int)std::string::npos && otherTypesIterations < maxIterations )
                    {
                        fileTypeVector.push_back( stg_ex::split_first_string(allowedFileTypes,"*") );
                        otherTypesIterations++;
                    }
                }
                else
                {
                    fileTypeVector.push_back(allowedFileTypes);
                }
            }
        }
        tempNewFilter = new filetype_filter("All Files (*.*)", "*.*");
        filters.push_back(tempNewFilter );
        returnFileFilterstring = generate_filters(filters);
        for( int iDelete = (int)filters.size()-1; iDelete>=0; iDelete--)
        {
            tempNewFilter = filters.at(iDelete);
            if( tempNewFilter!=NULL)
            {
                delete tempNewFilter;
                tempNewFilter = NULL;
            }
        }
        filters.clear();
        return returnFileFilterstring;
    }

    bool file_passes_filter(std::string fileTypeIn, std::vector <std::string> &fileTypeVector)
    {
        if( (int)fileTypeVector.size() > 0 )
        {
            if( (int)fileTypeIn.size() > 0)
            {
                std::string tempStr = "";
                for( int i = (int)fileTypeVector.size()-1; i >=0; i--)
                {
                    tempStr = fileTypeVector[i];
                    if(fileTypeIn==tempStr)
                    {
                        return true;
                    }
                }
            }
        }
        else
        {
            return true;
        }
        return false;
    }

    filetype_filter::filetype_filter(std::string filter_desc, std::string filter_extensions )
    {
        desc = filter_desc;
        exts = filter_extensions;
    }

    filetype_filter::~filetype_filter()
    {

    }

    file_object::file_object(std::string f_name,bool f_is_dir)
    {
        file_is_dir = f_is_dir;
        if( (int)f_name.size() > 0)
        {
            file_name = f_name;
            if( file_is_dir)
            {
                fileType = "";
            }
            else
            {
                fileType = stg_ex::get_file_ext(f_name);
            }
        }
        else
        {
            file_name = "";
            fileType = "";
        }
        thumbnail_texture = NULL;
        file_size_info = 0;
        date_creation_info = "";
        date_modified_info = "";
    }

    file_object::~file_object()
    {
        if( thumbnail_texture!=NULL)
        {
            delete thumbnail_texture;
            thumbnail_texture = NULL;
        }
    }

    bool file_object::is_directory()
    {
        return file_is_dir;
    }

    std::string file_object::get_name()
    {
        return file_name;
    }

    std::string file_object::get_type()
    {
        return fileType;
    }

    file_directory_class::file_directory_class()
    {
        file_position= 0;
        file_count= 0;
        sub_directory_count= 0;
    }

    file_directory_class::~file_directory_class()
    {
        close_directory();
    }


    bool file_directory_class::open_directory(std::string directory_string)
    {
        return open_directory_sorted(directory_string);
    }

    void file_directory_class::close_directory()
    {
        file_object * tFile = NULL;
        for( int i = (int)files_list.size()-1; i>=0; i--)
        {
            tFile = files_list[i];
            if( tFile!=NULL)
            {
                delete tFile;
                tFile = NULL;
            }
        }
        files_list.clear();
        file_position= 0;
        file_count= 0;
        sub_directory_count= 0;
    }

    void file_directory_class::filter_directory(bool only_folders, std::vector <std::string> &file_types_to_use)
    {
        file_object * tempFile = NULL;
        file_position = 0;
        file_count = 0;
        sub_directory_count = 0;
        for( int i = (int)files_list.size() - 1; i >=0; i--)
        {
            tempFile = files_list[i];
            if( tempFile!=NULL)
            {
                if( only_folders)
                {
                    if( tempFile->is_directory() )
                    {
                        sub_directory_count++;
                    }
                }
                else if( tempFile->is_directory() )
                {
                    sub_directory_count++;
                }
                else if( file_passes_filter (tempFile->get_type(), file_types_to_use) )
                {
                    file_count++;
                }
                else
                {
                    delete tempFile;
                    tempFile = NULL;
                    files_list.erase(files_list.begin()+i);
                }
            }
            else
            {
                files_list.erase(files_list.begin()+i);
            }
        }
    }

    bool file_directory_class::open_directory_sorted(std::string directory_string)
    {
        if( main_file_url_manager->folder_exists(directory_string) )
        {
            close_directory();
            std::string foundErrorMessage;
            DIR *dir;
            struct dirent *ent;
            dir = opendir( directory_string.c_str() );
            if( dir==NULL)
            {
                foundErrorMessage = strerror(errno);

                error_log->report("Unable to open directory["+directory_string+"]");
                error_log->report("Error: "+foundErrorMessage);

                //tinydir_close(&dir);
                return false;
            }
            else
            {
                file_object * newFile = NULL;
                file_object * tFile = NULL;
                int i = 0;
                int j = 0;
                bool foundNewPosition = false;
                bool newFileIsDirectory = false;
                std::string new_file_name = "";
                while ( (ent = readdir(dir)) != NULL )
                {
                    std::string entry( ent->d_name );
                    //std::string lcEntry( std::strToLower(entry) );
                    /*
                    if( tinydir_readfile(&dir, &file)==-1)
                    {
                        foundErrorMessage = strerror(errno) ;
                        error_log->report("Unable to open directory's file #"+ stg_ex::int_to_string(i)+"]");
                        error_log->report("Error: "+foundErrorMessage);
                        tinydir_close(&dir);
                        return false;
                    }
                    else*/
                    {
                        new_file_name = ent->d_name;
                        if(new_file_name!=".." && new_file_name!=".")
                        {
                            newFileIsDirectory = main_file_url_manager->folder_exists(directory_string+"/"+ent->d_name);
                            //if( ( main_editor_settings!=NULL && main_editor_settings->showHiddenFilesInBrowser->is_clicked() ) || system_found_os!=GPE_IDE_LINUX || (system_found_os==GPE_IDE_LINUX && new_file_name.find_first_of(".",0)!=0) )
                            {
                                newFile = new file_object(new_file_name, newFileIsDirectory);
                                if( newFile!=NULL)
                                {
                                    newFile->file_directory_location = directory_string;
                                    foundNewPosition = false;

                                    for( j = (int)files_list.size()-1; j >=0 && foundNewPosition==false; j--)
                                    {
                                        tFile = files_list[j];
                                        if( tFile!=NULL)
                                        {
                                            if( tFile->get_name() >= new_file_name )
                                            {
                                                files_list.insert(files_list.begin()+j,newFile);
                                                foundNewPosition = true;
                                            }
                                        }
                                    }
                                    if( !foundNewPosition)
                                    {
                                        files_list.push_back(newFile);
                                    }
                                    if( newFileIsDirectory )
                                    {
                                        sub_directory_count++;
                                    }
                                    else
                                    {
                                        file_count++;
                                    }
                                }
                            }
                        }
                        //tinydir_next(&dir);
                    }
                    i++;
                }
                //tinydir_close(&dir);

            }

            return true;
        }
        return false;
    }

    file_object * file_directory_class::get_file(int position)
    {
        if( position >=0 && position < (int)files_list.size() )
        {
            return files_list.at(position);
        }
        return NULL;
    }

    int file_directory_class::get_count()
    {
        return (int)files_list.size();
    }

    int file_directory_class::get_subdirectory_count()
    {
        return sub_directory_count;
    }

    int file_directory_class::get_file_count()
    {
        return file_count;
    }

    file_object * file_directory_class::get_next_file()
    {
        if( has_next_file() )
        {
            file_position++;
            return get_file(file_position);
        }
        return NULL;
    }

    bool file_directory_class::has_next_file()
    {
        if( file_position>=0 && file_position < (int)files_list.size()-1 )
        {
            return true;
        }
        return false;
    }

    bool file_directory_class::set_position(int position)
    {
        if( position >=0 && position < (int)files_list.size() )
        {
            file_position = position;
        }
        return false;
    }

    file_and_url_manager::file_and_url_manager()
    {

    }

    file_and_url_manager::~file_and_url_manager()
    {

    }

    void file_and_url_manager::external_open_program(std::string program_location, std::string program_info, bool show_program_info_on_fail )
    {

    }

    void file_and_url_manager::external_open_url(std::string url_string)
    {

    }

    int file_and_url_manager::file_get_size_bytes(const std::string &file_name)
    {
        return 0 ;
    }

    std::string file_and_url_manager::file_get_size_string(const std::string &file_name)
    {
        return "0";
    }

    int file_and_url_manager::folder_clean(std::string folder_name)
    {
        return -1;
    }


    int file_and_url_manager::folder_copy(std::string folder_name, std::string folder_target, bool copy_subfolders, bool overwrite_existing_files )
    {
        return -1;
    }

    int file_and_url_manager::folder_create( std::string new_path_name)
    {
        return -1;
    }

    bool file_and_url_manager::folder_exists(std::string path_name)
    {
        return false;
    }

    std::string  file_and_url_manager::get_user_settings_folder()
    {
        return "";
    }

    bool file_and_url_manager::seek_settings_folder()
    {
        return false;
    }

    std::string file_and_url_manager::get_user_temp_folder()
    {
        return "";
    }

    std::string file_and_url_manager::get_user_screenshot_folder()
    {
        return "";
    }

    bool file_and_url_manager::delete_file( std::string f_name)
    {
        return false;
    }

}
