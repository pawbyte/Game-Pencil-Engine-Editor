/*
paw_gui_general_resource.h
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

#ifndef PAW_GUI_GENERAL_RESOURCE_H
#define PAW_GUI_GENERAL_RESOURCE_H

#include <string>
#include "../GPE/GPE_Shapes.h"
#include "../GPE/GPE_Renderer_SDL.h"
#include "../GPE/GPE_Resources_Controller.h"

class generalGameResource
{
protected:
    GPE_Rect encapBox;
    bool isModified;
    int editorMode;
    int requestedMode;
public:
    bool isFullScreenResource;
    int globalResouceIdNumber;
    std::string resourceFileName;
    std::string resourceName;
    std::string parentProjectName;
    std::string parentProjectDirectory;
    int resourceType;
    int subResouceId;
    int exportBuildGlobalId;
    bool justOpenedThisFrame;
    generalGameResource();
    virtual ~generalGameResource() = 0;
    virtual bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    virtual bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    int get_global_rid();
    virtual std::string get_current_name();
    std::string get_name();
    virtual GPE_Animation * get_resource_animation();
    virtual GPE_Texture_Base * get_resource_texture();
    virtual bool is_modified();
    bool matches(generalGameResource * otherResource);
    virtual bool include_local_files( std::string pBuildDir , int buildType );
    virtual void integrate_into_syntax();
    virtual void open_code(int lineNumb, int colNumb, std::string codeTitle = "");
    virtual void preprocess_self(std::string alternatePath = "");
    virtual void prerender_self( );
    virtual void process_resource(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    virtual void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_resource(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    virtual void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    virtual void save_resource(std::string alternatePath = "", int backupId = -1);
    virtual void set_global_rid(int newRId);
    virtual void set_name(std::string newName);
    virtual void set_parent_project(std::string newName);
    virtual void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    virtual void update_string(std::string newName);
    virtual bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0 );
    virtual int search_for_string(std::string needle);
    virtual int search_and_replace_string(std::string needle, std::string newStr = "");
    bool write_header_on_file(std::ofstream * fileTarget);
};

#endif // PAW_GUI_GENERAL_RESOURCE_H
