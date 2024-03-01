/*
gpe_file_system.cpp
This file is part of:GAME PENCIL ENGINE
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

#include <cstring>

#include "gpe_file_system.h"
#include "gpe_globals.h"

namespace gpe
{
    std::string generate_filters(const std::vector<filetype_filter *> filters)
    {
        std::string result ="";
        filetype_filter * tempFilter = nullptr;
        for(int i = 0; i < (int)filters.size(); i++)
        {
            tempFilter = filters[i];
            if( tempFilter!=nullptr)
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
        filetype_filter * tempNewFilter = nullptr;
        if( (int)allowedFileTypes.size() > 0)
        {
            if( allowedFileTypes=="All types(*.*)" ||  allowedFileTypes=="All Files" || allowedFileTypes=="None" ||  allowedFileTypes=="All Files*")
            {
                fileTypeVector.clear();
            }
            else if( allowedFileTypes=="Image Only" || allowedFileTypes=="Image" || allowedFileTypes=="Images" || allowedFileTypes=="Photos")
            {
                tempNewFilter = new filetype_filter("Images (*.bmp | *.png | *.jpg| *.jpeg | *.jif  | *.jfif  | .gif )", "*.bmp;*.png;*.jpg;*.jpeg;*.jif;*.jfif;*.gif");
                filters.push_back(tempNewFilter );
                fileTypeVector.push_back("bmp");
                fileTypeVector.push_back("BMP");
                fileTypeVector.push_back("gif");
                fileTypeVector.push_back("GIF");
                fileTypeVector.push_back("jif");
                fileTypeVector.push_back("JIF");
                fileTypeVector.push_back("jpg");
                fileTypeVector.push_back("JPG");
                fileTypeVector.push_back("jpeg");
                fileTypeVector.push_back("JPEG");
                fileTypeVector.push_back("jfif");
                fileTypeVector.push_back("JFIF");
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
            if( tempNewFilter!=nullptr)
            {
                delete tempNewFilter;
                tempNewFilter = nullptr;
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
        thumbnail_texture = nullptr;
        file_size_info = 0;
        date_creation_info = "";
        date_modified_info = "";
    }

    file_object::~file_object()
    {
        if( thumbnail_texture!=nullptr)
        {
            delete thumbnail_texture;
            thumbnail_texture = nullptr;
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
        file_object * tFile = nullptr;
        for( int i = (int)files_list.size()-1; i>=0; i--)
        {
            tFile = files_list[i];
            if( tFile!=nullptr)
            {
                delete tFile;
                tFile = nullptr;
            }
        }
        files_list.clear();
        file_position= 0;
        file_count= 0;
        sub_directory_count= 0;
    }

    void file_directory_class::filter_directory(bool only_folders, std::vector <std::string> &file_types_to_use)
    {
        file_object * tempFile = nullptr;
        file_position = 0;
        file_count = 0;
        sub_directory_count = 0;
        for( int i = (int)files_list.size() - 1; i >=0; i--)
        {
            tempFile = files_list[i];
            if( tempFile!=nullptr)
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
                    tempFile = nullptr;
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
        if( sff_ex::path_exists(directory_string) )
        {
            close_directory();
            std::string foundErrorMessage;
            DIR *dir;
            struct dirent *ent;
            dir = opendir( directory_string.c_str() );
            if( dir==nullptr)
            {
                foundErrorMessage = strerror(errno);

                error_log->report("Unable to open directory["+directory_string+"]");
                error_log->report("Error: "+foundErrorMessage);

                //tinydir_close(&dir);
                return false;
            }
            else
            {
                file_object * newFile = nullptr;
                file_object * tFile = nullptr;
                int i = 0;
                int j = 0;
                bool foundNewPosition = false;
                bool newFileIsDirectory = false;
                std::string new_file_name = "";
                while ( (ent = readdir(dir)) != nullptr )
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
                            newFileIsDirectory = sff_ex::path_exists(directory_string+"/"+ent->d_name);
                            //if( ( main_editor_settings!=nullptr && main_editor_settings->showHiddenFilesInBrowser->is_clicked() ) || system_found_os!=GPE_IDE_LINUX || (system_found_os==GPE_IDE_LINUX && new_file_name.find_first_of(".",0)!=0) )
                            {
                                newFile = new file_object(new_file_name, newFileIsDirectory);
                                if( newFile!=nullptr)
                                {
                                    newFile->file_directory_location = directory_string;
                                    foundNewPosition = false;

                                    for( j = (int)files_list.size()-1; j >=0 && foundNewPosition==false; j--)
                                    {
                                        tFile = files_list[j];
                                        if( tFile!=nullptr)
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
        return nullptr;
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
        return nullptr;
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



    void external_open_program(std::string program_location, std::string program_info, bool show_program_info_on_fail )
    {
        error_log->report("Attempting to open ["+program_location+"] with ["+program_info+"] data...");
        if( sff_ex::file_exists(program_location) || sff_ex::path_exists(program_location) )
        {
    #ifdef _WIN32
            // win implementation
            //program_location = "start "+url_string;
            program_info = stg_ex::string_replace_all(program_info,"/","\\");
            ShellExecute(nullptr,nullptr,program_location.c_str(),program_info.c_str(),nullptr,SW_SHOW );
    #elif _APPLE__
            // apple implementation
            program_location = "open "+program_location;

            system(program_location.c_str() );
    #else
            // Linux and others implementation
            program_location = "xdg-open \""+program_location+"\"";
            system(program_location.c_str() );
    #endif
        }
        else if( show_program_info_on_fail )
        {
            external_open_url(program_info);
        }
    }

    void external_open_url(std::string url_string)
    {
        //url_string+="&";
        /**/
    #ifdef _WIN32
        // win implementation
        ShellExecute(nullptr, "open", url_string.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    #elif _APPLE__
        // apple implementation
        url_string = "open "+url_string;
        system(url_string.c_str() );
    #else
        // Linux and others implementation
        url_string = "xdg-open \""+url_string+"\"";
        system(url_string.c_str() );
    #endif
    }

    int get_file_size_bytes(const std::string &file_name)
    {
        std::ifstream file(file_name.c_str(), std::ifstream::in | std::ifstream::binary);

        if(!file.is_open())
        {
            return -1;
        }

        file.seekg(0, std::ios::end);
        int fileSize = file.tellg();
        file.close();

        return fileSize;
    }

    std::string get_file_size_string(const std::string &file_name)
    {
        float f_sizeInBytes = get_file_size_bytes( file_name );
        if( f_sizeInBytes <  0 )
        {
            return "Error: File Size not found...";
        }
        if( f_sizeInBytes >=byte_size_tb )
        {
            return stg_ex::float_to_string( f_sizeInBytes/byte_size_tb )+"GB";
        }
        else if( f_sizeInBytes >=byte_size_gb )
        {
            return stg_ex::float_to_string( f_sizeInBytes/byte_size_gb )+"GB";
        }
        else if( f_sizeInBytes >=byte_size_mb )
        {
            return stg_ex::float_to_string( f_sizeInBytes/byte_size_mb )+"MB";
        }
        else if( f_sizeInBytes >=byte_size_kb )
        {
            return stg_ex::float_to_string( f_sizeInBytes/byte_size_kb )+"KB";
        }
        return stg_ex::float_to_string( f_sizeInBytes )+" bytes";
    }

    int clean_folder(std::string folder_name)
    {
        file_directory_class * dir = new file_directory_class();
        file_object * file = nullptr;
        int iFile = 0;
        int iDirectory = 0;

        std::string fileToClick = "";
        std::vector< std::string > foldersToDelete;
        foldersToDelete.push_back(folder_name);
        std::string currentFolderToClear = folder_name;
        int filesDeletedCount = 0;
        if( dir!=nullptr)
        {
            while( (int)foldersToDelete.size() > 0 )
            {
                currentFolderToClear = foldersToDelete[0];
                dir->open_directory(currentFolderToClear);
                for (iFile = (int)dir->get_count()-1; iFile>=0; iFile--)
                {
                    file = dir->get_file(iFile);
                    if( file!=nullptr)
                    {
                        fileToClick = file->get_name();
                        if( fileToClick!="." && fileToClick!="..")
                        {
                            fileToClick = currentFolderToClear+"/"+fileToClick;
                            if( file->is_directory() )
                            {
                                foldersToDelete.push_back(fileToClick );
                            }
                            else
                            {
                                remove(fileToClick.c_str() );
                                filesDeletedCount++;
                            }
                        }
                    }
                }
                for( iDirectory = (int)foldersToDelete.size()-1; iDirectory >=0; iDirectory--)
                {
                    if( currentFolderToClear==foldersToDelete.at(iDirectory) )
                    {
                        foldersToDelete.erase( foldersToDelete.begin()+iDirectory);
                    }
                }
            }
            delete dir;
            dir = nullptr;
            return filesDeletedCount;
        }
        return 0;
    }

    int copy_folder(std::string folder_name, std::string folder_target, bool copy_subfolders, bool overwrite_existing_files )
    {
        if( sff_ex::path_exists( folder_name ) == false )
        {
            return -1;
        }
        if( sff_ex::path_exists(folder_target) == false )
        {
            return -2;
        }

        int filesCopiedCount = 0;
        file_directory_class * dir = new file_directory_class();
        file_object * file = nullptr;
        std::string currentFileName = "";
        int iFile = 0;
        int iDirectory = 0;
        int addedFolderFileCount = 0;
        if( dir!=nullptr )
        {
            dir->open_directory(folder_name);
            for (iFile = 0; iFile < (int)dir->get_count(); iFile++)
            {
                file = dir->get_file(iFile);
                if( file!=nullptr)
                {
                    currentFileName = file->get_name();
                    if( currentFileName!="." && currentFileName!="..")
                    {
                        //displayMessagestring = "Copying "+currentFileName;
                        currentFileName = folder_name+"/"+currentFileName;
                        if( file->is_directory() )
                        {
                            if( copy_subfolders)
                            {
                                sff_ex::create_directory(folder_target+"/"+file->get_name() );
                                addedFolderFileCount= copy_folder( currentFileName, folder_target+"/"+ file->get_name(), true, overwrite_existing_files );

                                if( addedFolderFileCount > 0 )
                                {
                                    filesCopiedCount += addedFolderFileCount;
                                }
                            }
                        }
                        else
                        {
                            if( sff_ex::file_copy(currentFileName,folder_target+"/"+ file->get_name(), overwrite_existing_files ) )
                            {
                                filesCopiedCount++;
                            }
                        }
                    }
                }
            }
            delete dir;
            dir = nullptr;
            return filesCopiedCount;
        }
        return 0;
    }


    bool seek_settings_folder()
    {
        //Sets the folder used in all  get_user_settings_folder() calls based on the 2 parameters above
        settings_app_folder = app_directory_name;
        if( (int)settings->programPublisher.size() ==0 )
        {
            return false;
        }
        //if a publisher is said to be to this program
        //Here is where he fun happens...
        bool useProgramFolder = false;
        bool homeDirFound  = true;

        //Going thru all of the possible getenv paramters that i know of...
        char* homeDir = getenv("%UserProfile%");
        std::string foundPath = "";
        //Attempt 1...
        if( homeDir!=nullptr)
        {
            //Attempt 1...
            foundPath = homeDir;
        }
        else
        {
            //Attempt 2...
            homeDir = getenv("home");
            if( homeDir!=nullptr )
            {
                foundPath = homeDir;
            }
            else
            {
                //Attempt 3...
                homeDir = getenv("HOME");
                if( homeDir!=nullptr)
                {
                    foundPath = homeDir;
                }
                else
                {
                    //final shot...
                    homeDir = getenv("homepath");
                    if( homeDir!=nullptr )
                    {
                        foundPath = homeDir;
                    }
                    else
                    {
                        //All attempts failed defaults to program folder...
                        return false;
                    }
                }
            }
        }

        if( homeDirFound && sff_ex::path_exists(foundPath) )
        {
            std::string appDataPath = foundPath;
            if( system_found_os== system_os_windows)
            {
                //First Layer of Windows unlocked :-)
                if( sff_ex::create_directory(appDataPath+"/AppData")!=-1)
                {
                    foundPath = appDataPath = appDataPath+"/AppData";

                    //See if we can get into the Roaming folder now
                    if( sff_ex::create_directory(appDataPath+"/Roaming")!=-1)
                    {
                        foundPath = appDataPath = appDataPath+"/Roaming";
                    }
                    else if( sff_ex::create_directory(appDataPath+"/Local")!=-1 )
                    {
                        //If failed, lets see if we can get into the Local folder now
                        foundPath = appDataPath = appDataPath+"/Local";
                    }
                    else if( sff_ex::create_directory(appDataPath+"/LocalLow")!=-1 )
                    {
                        foundPath = appDataPath = appDataPath+"/LocalLow";
                    }
                    else
                    {
                        //Windows failed to get into settings...
                        return false;
                    }
                }
                else
                {
                    useProgramFolder = true;
                    appDataPath = foundPath = "";
                }
            }
            else if( system_found_os== system_os_mac)
            {
                appDataPath = foundPath+"/~/Library/Preferences";
                if( sff_ex::create_directory(appDataPath)==-1)
                {
                    useProgramFolder = true;
                    appDataPath = foundPath = "";
                }
                else
                {
                    return false;
                }
            }

            if( useProgramFolder )
            {
                //Something failed above so now we just use the program folder...
                return false;
            }

            //If we have some sort of file path...
            if( system_found_os== system_os_linux )
            {
                appDataPath = appDataPath+"/."+settings->programPublisher;
            }
            else
            {
                //Windows, Mac, etc afaik...
                appDataPath = appDataPath+"/"+settings->programPublisher;
            }

            //If we are able to get into the publisher folder
            if( sff_ex::create_directory(appDataPath)!=-1)
            {
                foundPath = appDataPath;
                settings_app_folder = foundPath;
                if( (int)settings->programTitle.size() > 0 )
                {
                    //If we are able to get into the titled program's folder
                    if( sff_ex::create_directory( foundPath+"/"+settings->programTitle )!=-1 )
                    {
                        foundPath = foundPath+"/"+settings->programTitle;
                        settings_app_folder = foundPath;
                    }
                    settings_app_folder = settings_app_folder+"/";
                    //If this fails we will just default to the publisher and call it a day...
                }
                return true;
            }
            else
            {
                return false;
            }
        }


        //save the settings and local directory to the application directory
        if( useProgramFolder )
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    std::string  get_user_settings_folder()
    {
        return settings_app_folder;
    }

    std::string get_user_temp_folder()
    {
        std::string foundPath =  get_user_settings_folder();
        std::string tempFolderstring = foundPath;
        foundPath = foundPath+"/temp_files";
        if( sff_ex::create_directory(foundPath)!=-1)
        {
            tempFolderstring = foundPath+"/";
        }
        return tempFolderstring;
    }

    std::string get_user_screenshot_folder()
    {
        std::string foundPath =  get_user_settings_folder();
        std::string tempScreenshotstring = foundPath;
        foundPath = foundPath+"/screenshots";
        if( sff_ex::create_directory(foundPath)!=-1)
        {
            tempScreenshotstring = foundPath+"/";
        }
        return tempScreenshotstring;
    }

    bool delete_file( std::string f_name)
    {
        //Function not yet created
        return false;
    }
}
