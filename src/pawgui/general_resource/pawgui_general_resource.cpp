/*
pawgui_general_resource.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )
Copyright (c) 2024 P.Industries

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

Contributors:
- PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>
- P Industries <https://p.industries>
*/

#include "pawgui_general_resource.h"

namespace pawgui
{
    general_resource::general_resource()
    {
        isFullScreenResource = false;
        resource_name = "";
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

    general_resource::~general_resource()
    {

    }

    bool general_resource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount )
    {
        return true;
    }

    bool general_resource::include_local_files( std::string pBuildDir , int buildType )
    {
        return true;
    }

    gpe::animation2d * general_resource::get_resource_animation()
    {
        return nullptr;
    }

    gpe::texture_base * general_resource::get_resource_texture()
    {
        return nullptr;
    }

    void general_resource::integrate_into_syntax()
    {

    }

    std::string general_resource::get_current_name()
    {
        return resource_name;
    }

    std::string general_resource::get_name()
    {
        return resource_name;
    }

    int general_resource::get_global_rid()
    {
        return globalResouceIdNumber;
    }

    bool general_resource::is_modified()
    {
        return isModified;
    }

    bool general_resource::is_build_ready()
    {
        recentErrorMessage = "";
        return true;
    }

    bool general_resource::matches(general_resource * otherContainer)
    {
        if( otherContainer!=nullptr)
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

    void general_resource::open_code( int lineNumb, int colNumb, std::string codeTitle )
    {

    }

    void general_resource::load_resource(std::string file_path)
    {

    }

    void general_resource::prerender_self( )
    {

    }

    void general_resource::process_resource( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        process_self(view_space,cam);
    }

    void general_resource::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {

    }

    void general_resource::render_resource( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        render_self( view_space,cam);
        justOpenedThisFrame = false;
    }

    void general_resource::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {

    }

    void general_resource::save_resource(std::string file_path, int backupId)
    {
        gpe::error_log->report("Old save resource used..");
        isModified = false;
    }

    void general_resource::set_name(std::string new_name)
    {
        resource_name = new_name;
    }

    void general_resource::set_parent_project(std::string new_name)
    {
        parentProjectName = new_name;
    }

    void general_resource::set_global_rid(int newRId)
    {
        globalResouceIdNumber = newRId;
    }
    void general_resource::update_box(int x_new, int y_new, int newW, int newH)
    {
        if( x_new!=-1)
        {
            encapBox.x = x_new;
        }
        if(y_new!=-1)
        {
            encapBox.y = y_new;
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

    void general_resource::update_string(std::string new_name)
    {
        resource_name = new_name;
    }

    bool general_resource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
    {
        return true;
    }

    int general_resource::search_for_string(std::string needle)
    {
        return 0;
    }

    int general_resource::search_and_replace_string(std::string needle, std::string newStr )
    {
        return 0;
    }

    bool general_resource::write_header_on_file(std::ofstream * fileTarget)
    {
        if( fileTarget!=nullptr && fileTarget->is_open() )
        {
            *fileTarget << "#    --------------------------------------------------  # \n";
            *fileTarget << "#     \n";
            *fileTarget << "#     \n";
            if( resourceType >=0 && resourceType < gpe::resource_type_max)
            {
                *fileTarget << "#    PawByte Ambitious Working GUI(PAWGUI) Project Game " << gpe::resource_type_names[resourceType] <<" DataFile \n";
            }
            *fileTarget << "#    Created automatically via the PawByte Ambitious Working GUI(PAWGUI) Editor \n";
            *fileTarget << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
            *fileTarget << "#    If you have any problems reading this file please report it to help@pawbyte.com . \n";
            *fileTarget << "#     \n";
            *fileTarget << "#     \n";
            *fileTarget << "#    --------------------------------------------------  # \n";
            *fileTarget << "Version=" << gpe::version_number_total << "\n";
            *fileTarget << "ResourceName=" << resource_name << "\n";
            *fileTarget << "#     \n";
            return true;
        }
        return false;
    }
}
