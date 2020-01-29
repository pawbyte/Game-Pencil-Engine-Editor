/*
functions.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#include "GPE_Functions.h"
#include "GPE_Globals.h"
#include "GPE_Collisions.h"
#include "GPE_Constants.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <limits>
//#include <direct.h>

std::string get_file_ext(std::string fileLocationIn)
{
    if( (int)fileLocationIn.size() > 0)
    {
        int dotPos = fileLocationIn.find_first_of(".");
        if(dotPos != (int)std::string::npos)
        {
            return fileLocationIn.substr( dotPos+ 1);
        }
    }
    return "NULL";
}

std::string get_file_ext_last_dot(std::string fileLocationIn)
{
    if( (int)fileLocationIn.size() > 0)
    {
        int dotPos = fileLocationIn.find_last_of(".");
        if(dotPos != (int)std::string::npos)
        {
            return fileLocationIn.substr( dotPos+ 1);
        }
    }
    return "NULL";
}

bool file_is_audio(std::string fileLocationIn)
{
    std::string foundExt = get_file_ext(fileLocationIn);
    if( foundExt=="aac" || foundExt=="mp3"  || foundExt=="ogg"  || foundExt=="wav" )
    {
        return true;
    }
    return false;
}

bool file_is_font(std::string fileLocationIn)
{
    std::string foundExt = get_file_ext(fileLocationIn);
    if( foundExt=="eot" || foundExt=="svg"  || foundExt=="ttf"  || foundExt=="woff" || foundExt=="woff2" )
    {
        return true;
    }
    return false;
}

bool file_is_image(std::string fileLocationIn)
{
    std::string foundExt = get_file_ext(fileLocationIn);
    if( foundExt=="bmp" || foundExt=="gif"  ||foundExt=="jpg"  || foundExt=="jpeg"  || foundExt=="png" )
    {
        return true;
    }
    return false;
}


bool file_is_video(std::string fileLocationIn)
{
    std::string foundExt = get_file_ext(fileLocationIn);
    if( foundExt=="mp4" || foundExt=="ogg" || foundExt=="webm" )
    {
        return true;
    }
    return false;
}

std::string get_file_noext(std::string fileLocationIn)
{
    if( fileLocationIn.size() > 0)
    {
        int dotPos = fileLocationIn.find_first_of(".");
        if(dotPos != (int)std::string::npos)
        {
            return fileLocationIn.substr( 0,dotPos);
        }
        else
        {
            return fileLocationIn;
        }
    }
    return "NULL";
}

//Operating System Functions
int GPE_Gain_OS()
{
    int returnOS = GPE_IDE_WINDOWS;
#ifdef _WIN32
    returnOS = GPE_IDE_WINDOWS;
#elif __APPLE__
    returnOS = GPE_IDE_MAC;
#else
    returnOS = GPE_IDE_LINUX;
#endif

    return returnOS;
}
//File Functions
void appendToFile(std::string fileName, std::string stringIn,std::string stringIn2,std::string stringIn3,std::string stringIn4, std::string stringIn5, std::string stringIn6, std::string stringIn7, std::string stringIn8, std::string stringIn9)
{
    std::ofstream filestr(fileName.c_str(), std::ios::out | std::ios::app);
    filestr << "ERROR: "<< stringIn << stringIn2 << stringIn3 << stringIn4 << stringIn5 << stringIn6 << stringIn7 << stringIn8 << stringIn9;
    filestr << " \n";
    filestr.close();
}

bool copy_file(std::string sourceFile, std::string desinationFile , bool overwriteExisting)
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

            CopyFile(sourceFile.c_str(), desinationFile.c_str(), FALSE );
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
            GPE_Report("Unable to create directory named: "+newPathToCreate+".");
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
            GPE_Report("Unable to create directory named: "+newPathToCreate+".");
        }
#endif
    }
    return -1;
}

bool file_exists(std::string fileNameIn)
{
    if( (int)fileNameIn.size() > 0 )
    {
        /*
            Code derieved from http://stackoverflow.com/questions/146924/how-can-i-tell-if-a-given-path-is-a-directory-or-a-file-c-c
            http://stackoverflow.com/a/146938
            Mk12 - http://stackoverflow.com/users/148195/mk12
        */
        struct stat s;
        if( stat(fileNameIn.c_str(),&s) == 0 )
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

std::string get_path_from_file(std::string fullFilePath)
{
    if( fullFilePath.size()>0)
    {
        size_t found;
        found=fullFilePath.find_last_of("/\\");
        if( found != std::string::npos)
        {
            std::string rString = fullFilePath.substr(0,found);
            if( path_exists(rString) )
            {
                return rString+"\\";
            }
            else
            {
                return rString+"\\";
            }
        }
    }
    return fullFilePath;
}

std::string get_local_from_global_file(std::string fullFilePath)
{
    if( fullFilePath.size()>0)
    {
        size_t found;
        found=fullFilePath.find_last_of("/\\");
        if( found != std::string::npos)
        {
            std::string dString = fullFilePath.substr(0,found);
            std::string rString = fullFilePath.substr(found+1);
            //if( path_exists(dString) )
            {
                return rString;
            }
        }
    }
    return fullFilePath;
}

bool path_exists(std::string pathNameIn)
{
    int pNameSize = (int)pathNameIn.size();
    if( pNameSize > 0 )
    {

        if( pathNameIn[ pNameSize-1] =='/')
        {
            pathNameIn = get_substring(pathNameIn,0,pNameSize-1);
        }
        else if( pathNameIn[ pNameSize-1] =='\\')
        {
            pathNameIn = get_substring(pathNameIn,0,pNameSize-1);
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


void update_rect(GPE_Rect * rectIn, int nx, int ny, int nw, int nh)
{
    if( rectIn!=NULL)
    {
        rectIn->x = nx;
        rectIn->y = ny;
        rectIn->w = nw;
        rectIn->h = nh;
    }
}
