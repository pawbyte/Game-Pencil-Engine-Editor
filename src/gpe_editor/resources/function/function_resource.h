/*
function_resource.h
This file is part of:
GAME PENCIL ENGINE
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

#ifndef GPE_function_RESOURCES_H
#define GPE_function_RESOURCES_H

#include "../../resources/basic/gpe_basic_resource_page.h"


class functionResource: public standardEditableGameResource
{
public:
    pawgui::widget_tabbar * codeCategoryTabs;
    pawgui::widget_input_text * parametersField;
    pawgui::widget_input_text * functionReturnType;
    pawgui::widget_text_editor * functionCodeHeader;
    pawgui::widget_text_editor * functionCodeSource;
    pawgui::widget_button_icon * save_button;
    pawgui::widget_panel_list * functionEditorList;
    functionResource(pawgui::widget_resource_container * pFolder = nullptr);
    ~functionResource();
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    bool include_local_files( std::string pBuildDir , int buildType );
    void integrate_into_syntax();
    bool is_build_ready();
    void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
    void load_resource(std::string file_path = "");
    void prerender_self( );
    void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void save_resource(std::string file_path = "", int backupId = -1);
    int search_for_string(std::string needle);
    int search_and_replace_string(std::string needle, std::string newStr = "");
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif
