/*
gpe_basic_resource_page.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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


#ifndef GPE_BASIC_PROJECT_RESOURCE_H
#define GPE_BASIC_PROJECT_RESOURCE_H

#include "gpe_editor.h"
#include "gpe_dock_system.h"

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
    void rename_component(std::string newName);
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
    GPE_TextInputBasic * renameBox;
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
    virtual void integrate_into_syntax();
    void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
    virtual void preprocess_self(std::string alternatePath = "" );
    virtual void prerender_self( );
    void process_export();
    virtual void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    virtual void process_resource(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    virtual void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void seek_parent_name();
    void set_name(std::string newName);
    void set_parent_name(std::string newName);
    virtual int search_for_string(std::string needle);
    virtual int search_and_replace_string(std::string needle, std::string newStr = "");
};

#endif
