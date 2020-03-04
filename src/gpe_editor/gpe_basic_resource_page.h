/*
gpe_basic_resource_page.h
This file is part of:
GAME PENCIL ENGINE
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/


#ifndef GPE_BASIC_project_RESOURCE_H
#define GPE_BASIC_project_RESOURCE_H


#include "gpe_dock_system.h"
#include "gpe_editor_globals.h"
#include "gpe_editor_project.h"
#include "../paw_gui/paw_gui_general_resource.h"
#include "../paw_gui/paw_gui.h"
#include "../sdl_libs/gpe_sdl_converter.h"
#include "../sdl_libs/sdl_surface_ex.h"

class GPE_ObjectComponent
{
private:
    GPE_GeneralGuiElement * component;
    GPE_ToolIconButton * settingsGear;
public:
    GPE_ObjectComponent();
    ~GPE_ObjectComponent();

    void load_data(std::string dataString);
    void remove_data(std::string dataString);
    void rename_component(std::string new_name);
    std::string get_data();
    std::string get_name();
    std::string get_plain_string();
    std::string get_type();
    GPE_GeneralGuiElement * get_component();
    GPE_ToolIconButton * get_gear();
    void set_component(GPE_GeneralGuiElement * newComponent);
};

class standardEditableGameResource: public generalGameResource
{
protected:
    GPE_ToolIconButton * loadResourceButton;
    GPE_ToolIconButton * exportResourceButton;
    GPE_ToolIconButton * saveResourceButton;
    GPE_ToolPushButton * confirmResourceButton;
    GPE_ToolPushButton * cancelResourceButton;
    gpe_text_widget_string * renameBox;
public:
    bool resourcePostProcessed;
    int commonButtonAlignment;
    std::vector < GPE_ObjectComponent * > customComponentRealList;
    GPE_GeneralResourceContainer * projectParentFolder;
    standardEditableGameResource(GPE_GeneralResourceContainer * ppFolder = NULL);
    virtual ~standardEditableGameResource() = 0;
    std::string get_current_name();
    GPE_GeneralGuiElement * add_gui_component(std::string componentData);
    virtual bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    virtual bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    virtual void compile_cpp();
    virtual bool include_local_files( std::string pBuildDir , int buildType );
    virtual void integrate_into_syntax();
    virtual bool is_build_ready();
    void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
    virtual void load_resource(std::string file_path = "" );
    virtual void prerender_self( );
    void process_export();
    virtual void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    virtual void process_resource( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    virtual void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void seek_parent_name();
    void set_name(std::string new_name);
    void set_parent_name(std::string new_name);
    virtual int search_for_string(std::string needle);
    virtual int search_and_replace_string(std::string needle, std::string newStr = "");
};

#endif
