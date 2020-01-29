/*
paw_gui_general_resource.cpp
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

#include "paw_gui_general_resource.h"

generalGameResource::generalGameResource()
{
    isFullScreenResource = false;
    resourceName = "";
    resourceFileName = "";
    parentProjectName = "";
    parentProjectDirectory = "";
    globalResouceIdNumber = -1;
    subResouceId = -1;
    resourceType = -1;
    encapBox.x = 0;
    encapBox.y = 0;
    encapBox.w = 32;
    encapBox.h = 32;
    isModified = false;
    editorMode = 0;
    exportBuildGlobalId = -1;
    justOpenedThisFrame = false;
    requestedMode = editorMode = 0;
}

generalGameResource::~generalGameResource()
{

}

bool generalGameResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount )
{
    return true;
}

bool generalGameResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount )
{
    return true;
}

bool generalGameResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

GPE_Animation * generalGameResource::get_resource_animation()
{
    return NULL;
}

GPE_Texture_Base * generalGameResource::get_resource_texture()
{
    return NULL;
}

void generalGameResource::integrate_into_syntax()
{

}

std::string generalGameResource::get_current_name()
{
    return resourceName;
}

std::string generalGameResource::get_name()
{
    return resourceName;
}

int generalGameResource::get_global_rid()
{
    return globalResouceIdNumber;
}

bool generalGameResource::is_modified()
{
    return isModified;
}

bool generalGameResource::matches(generalGameResource * otherContainer)
{
    if( otherContainer!=NULL)
    {
        if( globalResouceIdNumber==otherContainer->globalResouceIdNumber)
        {
            if( resourceType== otherContainer->resourceType)
            {
                if( subResouceId==otherContainer->subResouceId)
                {
                    if( parentProjectName.compare(otherContainer->parentProjectName)==0)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void generalGameResource::open_code( int lineNumb, int colNumb, std::string codeTitle )
{

}

void generalGameResource::preprocess_self(std::string alternatePath)
{

}

void generalGameResource::prerender_self( )
{

}

void generalGameResource::process_resource(GPE_Rect * viewedSpace,GPE_Rect *cam)
{
    process_self(viewedSpace,cam);
}

void generalGameResource::process_self(GPE_Rect * viewedSpace,GPE_Rect *cam)
{

}

void generalGameResource::render_resource(GPE_Rect * viewedSpace,GPE_Rect *cam)
{
    render_self( viewedSpace,cam);
    justOpenedThisFrame = false;
}

void generalGameResource::render_self(GPE_Rect * viewedSpace,GPE_Rect *cam)
{

}

void generalGameResource::save_resource(std::string alternatePath, int backupId)
{
    GPE_Report("Old save resource used..");
    isModified = false;
}

void generalGameResource::set_name(std::string newName)
{
    resourceName = newName;
}

void generalGameResource::set_parent_project(std::string newName)
{
    parentProjectName = newName;
}

void generalGameResource::set_global_rid(int newRId)
{
    globalResouceIdNumber = newRId;
}
void generalGameResource::update_box(int newX, int newY, int newW, int newH)
{
    if( newX!=-1)
    {
        encapBox.x = newX;
    }
    if(newY!=-1)
    {
        encapBox.y = newY;
    }

    if( newW!=-1)
    {
        encapBox.w = newW;
    }
    if(newH!=-1)
    {
        encapBox.h = newH;
    }
}

void generalGameResource::update_string(std::string newName)
{
    resourceName = newName;
}

bool generalGameResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    return true;
}

int generalGameResource::search_for_string(std::string needle)
{
    return 0;
}

int generalGameResource::search_and_replace_string(std::string needle, std::string newStr )
{
    return 0;
}

bool generalGameResource::write_header_on_file(std::ofstream * fileTarget)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        *fileTarget << "#    --------------------------------------------------  # \n";
        *fileTarget << "#     \n";
        *fileTarget << "#     \n";
        if( resourceType >=0 && resourceType < RESOURCE_TYPE_MAX)
        {
            *fileTarget << "#    Game Pencil Engine Project Game " << RESOURCE_TYPE_NAMES[resourceType] <<" DataFile \n";
        }
        *fileTarget << "#    Created automatically via the Game Pencil Engine Editor \n";
        *fileTarget << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
        *fileTarget << "#    If you have any problems reading this file please report it to help@pawbyte.com . \n";
        *fileTarget << "#     \n";
        *fileTarget << "#     \n";
        *fileTarget << "#    --------------------------------------------------  # \n";
        *fileTarget << "Version=" << GPE_VERSION_FLOAT_NUMBER << "\n";
        *fileTarget << "ResourceName=" << resourceName << "\n";
        *fileTarget << "#     \n";
        return true;
    }
    return false;
}
