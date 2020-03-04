/*
sff_ex.cpp
This file is part of:
Simple File Functions Extension
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

-Simple File Functions Extension <https://www.pawbyte.com/opensource>


*/

#ifdef _WIN32

#include <windows.h>
#include <Commdlg.h>

#endif

#include "sff_ex.h"

namespace sff_ex
{
    //File Functions
    void append_to_file(std::string fileName, std::string output_string )
    {
        std::ofstream filestr(fileName.c_str(), std::ios::out | std::ios::app);
        filestr << output_string;
        filestr << " \n";
        filestr.close();
    }

    bool file_copy(std::string sourceFile, std::string desinationFile , bool overwriteExisting)
    {
        if( sourceFile!=desinationFile)
        {
            if(file_exists(sourceFile) )
            {
                if( !overwriteExisting && file_exists( desinationFile ) )
                {
                    return false;
                }
    #ifdef _WIN32

                CopyFile(sourceFile.c_str(), desinationFile.c_str(), false );
    #else
                std::ifstream srce( sourceFile.c_str(), std::ios::binary ) ;
                std::ofstream dest( desinationFile.c_str(), std::ios::binary ) ;
                dest << srce.rdbuf() ;
    #endif
                return true;
            }
        }
        return false;
    }

    int create_directory( std::string newPathToCreate )
    {
        if( (int)newPathToCreate.size() > 2)
        {
            if( path_exists(newPathToCreate) )
            {
                return 0;
            }
    #ifdef _WIN32
            if (CreateDirectory(newPathToCreate.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
            {
                return true;
            }
            else
            {
                -1;
            }
    #else
            int foundError = -1;
            foundError = mkdir(newPathToCreate.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if( foundError==0)
            {
                return true;
            }
            else
            {
                return -1;
            }
    #endif
        }
        return -1;
    }

    bool file_exists(std::string new_file_name)
    {
        if( (int)new_file_name.size() > 0 )
        {
            /*
                Code derieved from http://stackoverflow.com/questions/146924/how-can-i-tell-if-a-given-path-is-a-directory-or-a-file-c-c
                http://stackoverflow.com/a/146938
                Mk12 - http://stackoverflow.com/users/148195/mk12
            */
            struct stat s;
            if( stat(new_file_name.c_str(),&s) == 0 )
            {
                if( s.st_mode & S_IFREG )
                {
                    //it's a file
                    return true;
                }
            }
        }
        return false;
    }

    bool path_exists(std::string pathNameIn)
    {
        int pNameSize = (int)pathNameIn.size();
        if( pNameSize > 0 )
        {

            if( pathNameIn[ pNameSize-1] =='/' || pathNameIn[ pNameSize-1] =='\\')
            {
                pathNameIn = pathNameIn.substr(0,pNameSize-1);
            }
            /*
                Code derieved from http://stackoverflow.com/questions/146924/how-can-i-tell-if-a-given-path-is-a-directory-or-a-file-c-c
                http://stackoverflow.com/a/146938
                Mk12 - http://stackoverflow.com/users/148195/mk12
            */
            struct stat s;
            if( stat(pathNameIn.c_str(),&s) == 0 )
            {
                if( s.st_mode & S_IFDIR )
                {
                    //it's a directory
                    return true;
                }
            }
        }
        return false;
    }
}
